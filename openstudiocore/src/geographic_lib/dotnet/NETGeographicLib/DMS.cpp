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
#include "GeographicLib/DMS.hpp"
#include "DMS.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
double DMS::Decode( System::String^ dms,
    [System::Runtime::InteropServices::Out] Flag% ind)
{
    try
    {
        GeographicLib::DMS::flag lind;
        double out =  GeographicLib::DMS::Decode( StringConvert::ManagedToUnmanaged(dms), lind );
        ind = static_cast<Flag>(lind);
        return out;
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}

//*****************************************************************************
void DMS::DecodeLatLon(System::String^ dmsa, System::String^ dmsb,
                        [System::Runtime::InteropServices::Out] double% lat,
                        [System::Runtime::InteropServices::Out] double% lon,
                        bool longfirst )
{
    try
    {
        double llat, llon;
        GeographicLib::DMS::DecodeLatLon( StringConvert::ManagedToUnmanaged( dmsa ),
            StringConvert::ManagedToUnmanaged( dmsb ), llat, llon, longfirst );
        lat = llat;
        lon = llon;
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}

//*****************************************************************************
double DMS::DecodeAngle(System::String^ angstr)
{
    try
    {
        return GeographicLib::DMS::DecodeAngle(StringConvert::ManagedToUnmanaged( angstr ));
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}

//*****************************************************************************
double DMS::DecodeAzimuth(System::String^ azistr)
{
    try
    {
        return GeographicLib::DMS::DecodeAzimuth(StringConvert::ManagedToUnmanaged( azistr ));
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}

//*****************************************************************************
System::String^ DMS::Encode(double angle, Component trailing, unsigned prec,
                            Flag ind, char dmssep)
{
    try
    {
        return StringConvert::UnmanagedToManaged(
            GeographicLib::DMS::Encode( angle,
                static_cast<GeographicLib::DMS::component>(trailing),
                prec,
                static_cast<GeographicLib::DMS::flag>(ind),
                dmssep )
            );
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}
//*****************************************************************************
System::String^ DMS::Encode(double angle, unsigned prec, Flag ind,
                            char dmssep )
{
    try
    {
        return StringConvert::UnmanagedToManaged(
            GeographicLib::DMS::Encode( angle,
                prec,
                static_cast<GeographicLib::DMS::flag>(ind),
                dmssep )
            );
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}
