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
#include "GeographicLib/Geohash.hpp"
#include "Geohash.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
void Geohash::Forward(double lat, double lon, int len, System::String^% geohash)
{
    try
    {
        std::string l;
        GeographicLib::Geohash::Forward( lat, lon, len, l );
        geohash = gcnew System::String( l.c_str() );
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}

//*****************************************************************************
void Geohash::Reverse(System::String^ geohash,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon,
    [System::Runtime::InteropServices::Out] int% len,
    bool centerp )
{
    try
    {
        double llat, llon;
        int llen;
        GeographicLib::Geohash::Reverse( StringConvert::ManagedToUnmanaged( geohash ),
            llat, llon, llen, centerp );
        lat = llat;
        lon = llon;
        len = llen;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double Geohash::LatitudeResolution(int len)
{
    return GeographicLib::Geohash::LatitudeResolution( len );
}

//*****************************************************************************
double Geohash::LongitudeResolution(int len)
{
    return GeographicLib::Geohash::LongitudeResolution( len );
}

//*****************************************************************************
int Geohash::GeohashLength(double res)
{
    return GeographicLib::Geohash::GeohashLength( res );
}

//*****************************************************************************
int Geohash::GeohashLength(double latres, double lonres)
{
    return GeographicLib::Geohash::GeohashLength( latres, lonres );
}

//*****************************************************************************
int Geohash::DecimalPrecision(int len)
{
    return GeographicLib::Geohash::DecimalPrecision( len );
}
