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
#include "GeographicLib/AlbersEqualArea.hpp"
#include "AlbersEqualArea.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
AlbersEqualArea::!AlbersEqualArea()
{
    if ( m_pAlbersEqualArea != NULL )
    {
        delete m_pAlbersEqualArea;
        m_pAlbersEqualArea = NULL;
    }
}

//*****************************************************************************
AlbersEqualArea::AlbersEqualArea( StandardTypes type )
{
    try
    {
        switch ( type )
        {
        case StandardTypes::CylindricalEqualArea:
            m_pAlbersEqualArea = new GeographicLib::AlbersEqualArea( GeographicLib::AlbersEqualArea::CylindricalEqualArea() );
            break;
        case StandardTypes::AzimuthalEqualAreaNorth:
            m_pAlbersEqualArea = new GeographicLib::AlbersEqualArea( GeographicLib::AlbersEqualArea::AzimuthalEqualAreaNorth() );
            break;
        case StandardTypes::AzimuthalEqualAreaSouth:
            m_pAlbersEqualArea = new GeographicLib::AlbersEqualArea( GeographicLib::AlbersEqualArea::AzimuthalEqualAreaSouth() );
            break;
        }
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::AlbersEqualArea" );
    }
}

//*****************************************************************************
AlbersEqualArea::AlbersEqualArea(double a, double f, double stdlat1, double stdlat2, double k1)
{
    try
    {
        m_pAlbersEqualArea = new GeographicLib::AlbersEqualArea( a, f, stdlat1, stdlat2, k1 );
    }
    catch ( GeographicLib::GeographicErr err )
    {
        throw gcnew GeographicErr( err.what() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::AlbersEqualArea" );
    }
}

//*****************************************************************************
AlbersEqualArea::AlbersEqualArea(double a, double f, double stdlat, double k0)
{
    try
    {
        m_pAlbersEqualArea = new GeographicLib::AlbersEqualArea( a, f, stdlat, k0 );
    }
    catch ( GeographicLib::GeographicErr err )
    {
        throw gcnew GeographicErr( err.what() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::AlbersEqualArea" );
    }
}

//*****************************************************************************
AlbersEqualArea::AlbersEqualArea(double a, double f,
                        double sinlat1, double coslat1,
                        double sinlat2, double coslat2,
                        double k1)
{
    try
    {
        m_pAlbersEqualArea = new GeographicLib::AlbersEqualArea(
            a, f, sinlat1, coslat1, sinlat2, coslat2, k1 );
    }
    catch ( GeographicLib::GeographicErr err )
    {
        throw gcnew GeographicErr( err.what() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::AlbersEqualArea" );
    }
}

//*****************************************************************************
void AlbersEqualArea::SetScale(double lat, double k)
{
    try
    {
        m_pAlbersEqualArea->SetScale( lat, k );
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
}

//*****************************************************************************
void AlbersEqualArea::Forward(double lon0, double lat, double lon,
                [System::Runtime::InteropServices::Out] double% x,
                [System::Runtime::InteropServices::Out] double% y,
                [System::Runtime::InteropServices::Out] double% gamma,
                [System::Runtime::InteropServices::Out] double% k)
{
    double lx, ly, lgamma, lk;
    m_pAlbersEqualArea->Forward( lon0, lat, lon, lx, ly, lgamma, lk );
    x = lx;
    y = ly;
    gamma = lgamma;
    k = lk;
}

//*****************************************************************************
void AlbersEqualArea::Reverse(double lon0, double x, double y,
                     [System::Runtime::InteropServices::Out] double% lat,
                     [System::Runtime::InteropServices::Out] double% lon,
                     [System::Runtime::InteropServices::Out] double% gamma,
                     [System::Runtime::InteropServices::Out] double% k)
{
    double llat, llon, lgamma, lk;
    m_pAlbersEqualArea->Reverse( lon0, x, y, llat, llon, lgamma, lk );
    lat = llat;
    lon = llon;
    gamma = lgamma;
    k = lk;
}

//*****************************************************************************
void AlbersEqualArea::Forward(double lon0, double lat, double lon,
                     [System::Runtime::InteropServices::Out] double% x,
                     [System::Runtime::InteropServices::Out] double% y)
{
    double lx, ly, lgamma, lk;
    m_pAlbersEqualArea->Forward( lon0, lat, lon, lx, ly, lgamma, lk );
    x = lx;
    y = ly;
}

//*****************************************************************************
void AlbersEqualArea::Reverse(double lon0, double x, double y,
                     [System::Runtime::InteropServices::Out] double% lat,
                     [System::Runtime::InteropServices::Out] double% lon)
{
    double llat, llon, lgamma, lk;
    m_pAlbersEqualArea->Reverse( lon0, x, y, llat, llon, lgamma, lk );
    lat = llat;
    lon = llon;
}

//*****************************************************************************
double AlbersEqualArea::MajorRadius::get()
{ return m_pAlbersEqualArea->MajorRadius(); }

//*****************************************************************************
double AlbersEqualArea::Flattening::get()
{ return m_pAlbersEqualArea->Flattening(); }

//*****************************************************************************
double AlbersEqualArea::OriginLatitude::get()
{ return m_pAlbersEqualArea->OriginLatitude(); }

//*****************************************************************************
double AlbersEqualArea::CentralScale::get()
{ return m_pAlbersEqualArea->CentralScale(); }
