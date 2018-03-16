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
#include "GeographicLib/OSGB.hpp"
#include "OSGB.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
void OSGB::Forward(double lat, double lon,
                    [System::Runtime::InteropServices::Out] double% x,
                    [System::Runtime::InteropServices::Out] double% y,
                    [System::Runtime::InteropServices::Out] double% gamma,
                    [System::Runtime::InteropServices::Out] double% k)
{
    double lx, ly, lgamma, lk;
    GeographicLib::OSGB::Forward( lat, lon, lx, ly, lgamma, lk );
    x = lx;
    y = ly;
    gamma = lgamma;
    k = lk;
}

//*****************************************************************************
void OSGB::Reverse(double x, double y,
                    [System::Runtime::InteropServices::Out] double% lat,
                    [System::Runtime::InteropServices::Out] double% lon,
                    [System::Runtime::InteropServices::Out] double% gamma,
                    [System::Runtime::InteropServices::Out] double% k)
{
    double llat, llon, lgamma, lk;
    GeographicLib::OSGB::Reverse( x, y, llat, llon, lgamma, lk );
    lat = llat;
    lon = llon;
    gamma = lgamma;
    k = lk;
}

//*****************************************************************************
void OSGB::Forward(double lat, double lon,
    [System::Runtime::InteropServices::Out] double% x,
    [System::Runtime::InteropServices::Out] double% y)
{
    double lx, ly;
    GeographicLib::OSGB::Forward( lat, lon, lx, ly );
    x = lx;
    y = ly;
}

//*****************************************************************************
void OSGB::Reverse(double x, double y,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon)
{
    double llat, llon;
    GeographicLib::OSGB::Reverse( x, y, llat, llon );
    lat = llat;
    lon = llon;
}

//*****************************************************************************
void OSGB::GridReference(double x, double y, int prec,
    [System::Runtime::InteropServices::Out] System::String^% gridref)
{
    try
    {
        std::string lgridref;
        GeographicLib::OSGB::GridReference( x, y, prec, lgridref );
        gridref = gcnew System::String( lgridref.c_str() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Memory allocation error in OSGB::GridReference" );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void OSGB::GridReference(System::String^ gridref,
                    [System::Runtime::InteropServices::Out] double% x,
                    [System::Runtime::InteropServices::Out] double% y,
                    [System::Runtime::InteropServices::Out] int% prec,
                    bool centerp )
{
    try
    {
        double lx, ly;
        int lprec;
        GeographicLib::OSGB::GridReference(
            StringConvert::ManagedToUnmanaged( gridref ),
            lx, ly, lprec, centerp );
        x = lx;
        y = ly;
        prec = lprec;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double OSGB::MajorRadius() { return GeographicLib::OSGB::MajorRadius(); }

//*****************************************************************************
double OSGB::Flattening() { return GeographicLib::OSGB::Flattening(); }

//*****************************************************************************
double OSGB::CentralScale() { return GeographicLib::OSGB::CentralScale(); }

//*****************************************************************************
double OSGB::OriginLatitude() { return GeographicLib::OSGB::OriginLatitude(); }

//*****************************************************************************
double OSGB::OriginLongitude()
{ return GeographicLib::OSGB::OriginLongitude(); }

//*****************************************************************************
double OSGB::FalseNorthing() { return GeographicLib::OSGB::FalseNorthing(); }

//*****************************************************************************
double OSGB::FalseEasting() { return GeographicLib::OSGB::FalseEasting(); }
