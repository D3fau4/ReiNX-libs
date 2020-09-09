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
    #define AMS_LDR_I_TX_INTERFACE_INTERFACE_INFO(C, H)                                           \
        AMS_SF_METHOD_INFO(C, H, 123, Result, IsSelectedGamecard, (const sf::InPointerBuffer data)) \
        AMS_SF_METHOD_INFO(C, H, 124, Result, TXGetGamecardStatus, (sf::Out<u32> ret)) \
        AMS_SF_METHOD_INFO(C, H, 125, Result, TX_125, ()) \
        AMS_SF_METHOD_INFO(C, H, 126, Result, IsLicenseValid, (sf::Out<u32> ret)) \
        AMS_SF_METHOD_INFO(C, H, 127, Result, TX_127, ()) \
        AMS_SF_METHOD_INFO(C, H, 128, Result, TX_128, ()) \
        AMS_SF_METHOD_INFO(C, H, 129, Result, IsPro, ()) \
        AMS_SF_METHOD_INFO(C, H, 130, Result, SetFTP, ()) \
        AMS_SF_METHOD_INFO(C, H, 131, Result, GetFTP, ()) \
        AMS_SF_METHOD_INFO(C, H, 132, Result, GetIPAddr, ()) \
        AMS_SF_METHOD_INFO(C, H, 133, Result, TX_133, ()) \
        AMS_SF_METHOD_INFO(C, H, 134, Result, SetStealthMode, ()) \
        AMS_SF_METHOD_INFO(C, H, 135, Result, GetStealthMode, ()) \
        AMS_SF_METHOD_INFO(C, H, 136, Result, TXIsEmuNand, ()) \
        AMS_SF_METHOD_INFO(C, H, 137, Result, TXIsFat32, ()) \
        AMS_SF_METHOD_INFO(C, H, 138, Result, TXSetLdnmitm, ()) \
        AMS_SF_METHOD_INFO(C, H, 139, Result, TXGetLdnmitm, ()) 

    AMS_SF_DEFINE_INTERFACE(TXService, AMS_LDR_I_TX_INTERFACE_INTERFACE_INFO)

} // namespace ams::ldr::impl
