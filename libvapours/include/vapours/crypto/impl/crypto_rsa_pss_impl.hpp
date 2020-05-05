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

#pragma once
#include <vapours/common.hpp>
#include <vapours/assert.hpp>
#include <vapours/util.hpp>
#include <vapours/crypto/impl/crypto_hash_function.hpp>

namespace ams::crypto::impl {

    template<typename Hash> /* requires HashFunction<Hash> */
    class RsaPssImpl {
        NON_COPYABLE(RsaPssImpl);
        NON_MOVEABLE(RsaPssImpl);
        public:
            static constexpr size_t HashSize = Hash::HashSize;
        private:
            static constexpr u8 TailMagic = 0xBC;
        private:
            static void ComputeHashWithPadding(void *dst, const u8 *user_hash, size_t user_hash_size, const void *salt, size_t salt_size) {
                AMS_ASSERT(user_hash_size == HashSize);

                /* Initialize our buffer. */
                u8 buf[8 + HashSize];
                std::memset(buf, 0, 8);
                std::memcpy(buf + 8, user_hash, HashSize);
                ON_SCOPE_EXIT { ClearMemory(buf, sizeof(buf)); };


                /* Calculate our hash. */
                Hash hash;
                hash.Initialize();
                hash.Update(buf, sizeof(buf));
                hash.Update(salt, salt_size);
                hash.GetHash(dst, HashSize);
            }

            static void ApplyMGF1(u8 *dst, size_t dst_size, const void *src, size_t src_size) {
                u8 buf[HashSize];
                ON_SCOPE_EXIT { ClearMemory(buf, sizeof(buf)); };

                const size_t required_iters = (dst_size + HashSize - 1) / HashSize;
                for (size_t i = 0; i < required_iters; i++) {
                    Hash hash;
                    hash.Initialize();
                    hash.Update(src, src_size);

                    const u32 tmp = util::ConvertToBigEndian(static_cast<u32>(i));
                    hash.Update(std::addressof(tmp), sizeof(tmp));

                    hash.GetHash(buf, HashSize);

                    const size_t start = HashSize * i;
                    const size_t end   = std::min(dst_size, start + HashSize);
                    for (size_t j = start; j < end; j++) {
                        dst[j] ^= buf[j - start];
                    }
                }
            }
        public:
            RsaPssImpl() { /* ... */ }

            bool Verify(u8 *buf, size_t size, const u8 *hash, size_t hash_size) {
                return 1;
            }
    };

}
