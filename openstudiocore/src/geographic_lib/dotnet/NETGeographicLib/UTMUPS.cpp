/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "stdafx.h"
#include "GeographicLib/UTMUPS.hpp"
#include "UTMUPS.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
int UTMUPS::StandardZone(double lat, double lon, int setzone)
{
    try
    {
        return GeographicLib::UTMUPS::StandardZone( lat, lon, setzone );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::Forward(double lat, double lon,
                    [System::Runtime::InteropServices::Out] int% zone,
                    [System::Runtime::InteropServices::Out] bool% northp,
                    [System::Runtime::InteropServices::Out] double% x,
                    [System::Runtime::InteropServices::Out] double% y,
                    [System::Runtime::InteropServices::Out] double% gamma,
                    [System::Runtime::InteropServices::Out] double% k,
                    int setzone, bool mgrslimits)
{
    try
    {
        int lzone;
        bool lnorthp;
        double lx, ly, lgamma, lk;
        GeographicLib::UTMUPS::Forward(lat, lon,
                                       lzone, lnorthp, lx, ly,
                                       lgamma, lk,
                                       setzone, mgrslimits);
        zone = lzone;
        northp = northp;
        x = lx;
        y = ly;
        gamma = lgamma;
        k = lk;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::Reverse(int zone, bool northp, double x, double y,
                    [System::Runtime::InteropServices::Out] double% lat,
                    [System::Runtime::InteropServices::Out] double% lon,
                    [System::Runtime::InteropServices::Out] double% gamma,
                    [System::Runtime::InteropServices::Out] double% k,
                    bool mgrslimits)
{
    try
    {
        double llat, llon, lgamma, lk;
        GeographicLib::UTMUPS::Reverse( zone, northp, x, y, llat, llon, lgamma,
                                        lk, mgrslimits );
        lat = llat;
        lon = llon;
        gamma = lgamma;
        k = lk;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::Forward(double lat, double lon,
                    [System::Runtime::InteropServices::Out] int% zone,
                    [System::Runtime::InteropServices::Out] bool% northp,
                    [System::Runtime::InteropServices::Out] double% x,
                    [System::Runtime::InteropServices::Out] double% y,
                    int setzone, bool mgrslimits )
{
    try
    {
        double gamma, k, lx, ly;
        bool lnorthp;
        int lzone;
        GeographicLib::UTMUPS::Forward(lat, lon, lzone, lnorthp, lx, ly,
                                       gamma, k, setzone, mgrslimits);
        x = lx;
        y = ly;
        zone = lzone;
        northp = lnorthp;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::Reverse(int zone, bool northp, double x, double y,
                    [System::Runtime::InteropServices::Out] double% lat,
                    [System::Runtime::InteropServices::Out] double% lon,
                    bool mgrslimits)
{
    try
    {
        double gamma, k, llat, llon;
        GeographicLib::UTMUPS::Reverse(zone, northp, x, y, llat, llon, gamma,
                                        k, mgrslimits);
        lat = llat;
        lon = llon;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::Transfer(int zonein, bool northpin, double xin, double yin,
                        int zoneout, bool northpout,
                        [System::Runtime::InteropServices::Out] double% xout,
                        [System::Runtime::InteropServices::Out] double% yout,
                        [System::Runtime::InteropServices::Out] int% zone)
{
    try
    {
        int lzone;
        double lxout, lyout;
        GeographicLib::UTMUPS::Transfer(zonein, northpin, xin, yin,
                                        zoneout, northpout, lxout, lyout,
                                        lzone);
        xout = lxout;
        yout = lyout;
        zone = lzone;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::DecodeZone(System::String^ zonestr,
    [System::Runtime::InteropServices::Out] int% zone,
    [System::Runtime::InteropServices::Out] bool% northp)
{
    try
    {
        std::string zoneIn = StringConvert::ManagedToUnmanaged( zonestr );
        int lzone;
        bool lnorthp;
        GeographicLib::UTMUPS::DecodeZone( zoneIn, lzone, lnorthp );
        zone = lzone;
        northp = lnorthp;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
System::String^ UTMUPS::EncodeZone(int zone, bool northp, bool abbrev)
{
    try
    {
        return StringConvert::UnmanagedToManaged( GeographicLib::UTMUPS::EncodeZone( zone, northp, abbrev ) );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void UTMUPS::DecodeEPSG(int epsg,
    [System::Runtime::InteropServices::Out] int% zone,
    [System::Runtime::InteropServices::Out] bool% northp)
{
    int lzone;
    bool lnorthp;
    GeographicLib::UTMUPS::DecodeEPSG( epsg, lzone, lnorthp );
    zone = lzone;
    northp = lnorthp;
}

//*****************************************************************************
int UTMUPS::EncodeEPSG(int zone, bool northp)
{
    return GeographicLib::UTMUPS::EncodeEPSG( zone, northp );
}

//****************************************************************************
double UTMUPS::UTMShift() { return GeographicLib::UTMUPS::UTMShift(); }

//****************************************************************************
double UTMUPS::MajorRadius() { return GeographicLib::UTMUPS::MajorRadius(); }

//****************************************************************************
double UTMUPS::Flattening() { return GeographicLib::UTMUPS::Flattening(); }
