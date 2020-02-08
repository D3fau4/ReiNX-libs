/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <mesosphere.hpp>

namespace ams::kern {

    namespace {

        template<typename F>
        ALWAYS_INLINE void DoOnEachCoreInOrder(s32 core_id, F f) {
            cpu::SynchronizeAllCores();
            for (size_t i = 0; i < cpu::NumCores; i++) {
                if (static_cast<s32>(i) == core_id) {
                    f();
                }
                cpu::SynchronizeAllCores();
            }
        }

    }

    NORETURN void HorizonKernelMain(s32 core_id) {
        /* Setup the Core Local Region, and note that we're initializing. */
        Kernel::InitializeCoreLocalRegion(core_id);
        Kernel::SetState(Kernel::State::Initializing);

        /* Ensure that all cores get to this point before proceeding. */
        cpu::SynchronizeAllCores();

        /* Initialize the main and idle thread for each core. */
        DoOnEachCoreInOrder(core_id, [=]() ALWAYS_INLINE_LAMBDA {
            Kernel::InitializeMainAndIdleThreads(core_id);
        });

        if (core_id == 0) {
            /* Initialize KSystemControl. */
            KSystemControl::Initialize();

            /* Initialize the memory manager and the KPageBuffer slabheap. */
            {
                const auto &metadata_region = KMemoryLayout::GetMetadataPoolRegion();
                Kernel::GetMemoryManager().Initialize(metadata_region.GetAddress(), metadata_region.GetSize());
                init::InitializeKPageBufferSlabHeap();
            }

            /* Copy the Initial Process Binary to safe memory. */
            CopyInitialProcessBinaryToKernelMemory();

            /* Initialize the KObject Slab Heaps. */
            init::InitializeSlabHeaps();

            /* Initialize the Dynamic Slab Heaps. */
            {
                const auto &pt_heap_region = KMemoryLayout::GetPageTableHeapRegion();
                Kernel::InitializeResourceManagers(pt_heap_region.GetAddress(), pt_heap_region.GetSize());
            }
        }

        /* Initialize the supervisor page table for each core. */
        DoOnEachCoreInOrder(core_id, [=]() ALWAYS_INLINE_LAMBDA {
            /* TODO: KPageTable::Initialize(); */
            /* TODO: Kernel::GetSupervisorPageTable().Initialize(); */
        });

        /* Set ttbr0 for each core. */
        DoOnEachCoreInOrder(core_id, [=]() ALWAYS_INLINE_LAMBDA {
            /* TODO: SetTtbr0(); */
        });

        /* NOTE: Kernel calls on each core a nullsub here on retail kernel. */

        /* Register the main/idle threads and initialize the interrupt task manager. */
        DoOnEachCoreInOrder(core_id, [=]() ALWAYS_INLINE_LAMBDA {
            KThread::Register(std::addressof(Kernel::GetMainThread(core_id)));
            KThread::Register(std::addressof(Kernel::GetIdleThread(core_id)));
            /* TODO: Kernel::GetInterruptTaskManager().Initialize(); */
        });

        /* Activate the scheduler and enable interrupts. */
        DoOnEachCoreInOrder(core_id, [=]() ALWAYS_INLINE_LAMBDA {
            Kernel::GetScheduler().Activate();
            KInterruptManager::EnableInterrupts();
        });

        /* Initialize cpu interrupt threads. */
        /* TODO cpu::InitializeInterruptThreads(core_id); */

        /* Initialize the DPC manager. */
        KDpcManager::Initialize();
        cpu::SynchronizeAllCores();

        /* Perform more core-0 specific initialization. */
        if (core_id == 0) {
            /* TODO: Initialize KWorkerThreadManager */

            /* TODO: KSystemControl::InitializeSleepManagerAndAppletSecureMemory(); */

            /* TODO: KDeviceAddressSpace::Initialize(); */

            /* TODO: CreateAndRunInitialProcesses(); */

            /* We're done initializing! */
            Kernel::SetState(Kernel::State::Initialized);

            /* TODO: KThread::ResumeThreadsSuspendedForInit(); */
        }
        cpu::SynchronizeAllCores();

        /* Set the current thread priority to idle. */
        GetCurrentThread().SetPriorityToIdle();

        /* Exit the main thread. */
        {
            auto &main_thread = Kernel::GetMainThread(core_id);
            main_thread.Open();
            main_thread.Exit();
        }

        /* Main() is done, and we should never get to this point. */
        MESOSPHERE_PANIC("Main Thread continued after exit.");
        while (true) { /* ... */ }
    }

}
