/*
 * Copyright (c) 2018-2020 Atmosphère-NX, D3fau4
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
#include <vapours.hpp>
#include <stratosphere/sf.hpp>

namespace ams::ldr::impl
{
    #define AMS_LDR_I_RNX_INTERFACE_INTERFACE_INFO(C, H)                                           \
        AMS_SF_METHOD_INFO(C, H, 0, Result, GetReiNXVersion, (sf::Out<u32> maj, sf::Out<u32> min)) \
        AMS_SF_METHOD_INFO(C, H, 1, Result, SetHbTidForDelta,  (u64 tid))

    AMS_SF_DEFINE_INTERFACE(RNXService, AMS_LDR_I_RNX_INTERFACE_INTERFACE_INFO)

} // namespace ams::ldr::impl
