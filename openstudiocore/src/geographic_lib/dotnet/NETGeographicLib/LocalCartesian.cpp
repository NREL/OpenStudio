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
#include "GeographicLib/LocalCartesian.hpp"
#include "LocalCartesian.h"
#include "Geocentric.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::LocalCartesian";

//*****************************************************************************
LocalCartesian::!LocalCartesian(void)
{
    if ( m_pLocalCartesian != NULL )
    {
        delete m_pLocalCartesian;
        m_pLocalCartesian = NULL;
    }
}

//*****************************************************************************
LocalCartesian::LocalCartesian(double lat0, double lon0, double h0,
                Geocentric^ earth )
{
    try
    {
        const GeographicLib::Geocentric* pGeocentric =
            reinterpret_cast<const GeographicLib::Geocentric*>(
                earth->GetUnmanaged()->ToPointer() );
        m_pLocalCartesian = new GeographicLib::LocalCartesian( lat0, lon0, h0, *pGeocentric );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
LocalCartesian::LocalCartesian(double lat0, double lon0, double h0 )
{
    try
    {
        m_pLocalCartesian = new GeographicLib::LocalCartesian( lat0, lon0, h0,
            GeographicLib::Geocentric::WGS84() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
LocalCartesian::LocalCartesian(Geocentric^ earth)
{
    try
    {
        const GeographicLib::Geocentric* pGeocentric =
            reinterpret_cast<const GeographicLib::Geocentric*>(
                earth->GetUnmanaged()->ToPointer() );
        m_pLocalCartesian = new GeographicLib::LocalCartesian( *pGeocentric );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
LocalCartesian::LocalCartesian()
{
    try
    {
        m_pLocalCartesian = new GeographicLib::LocalCartesian(
            GeographicLib::Geocentric::WGS84() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void LocalCartesian::Reset(double lat0, double lon0, double h0 )
{
    m_pLocalCartesian->Reset( lat0, lon0, h0 );
}

//*****************************************************************************
void LocalCartesian::Forward(double lat, double lon, double h,
    [System::Runtime::InteropServices::Out] double% x,
    [System::Runtime::InteropServices::Out] double% y,
    [System::Runtime::InteropServices::Out] double% z)
{
    double lx, ly, lz;
    m_pLocalCartesian->Forward( lat, lon, h, lx, ly, lz );
    x = lx;
    y = ly;
    z = lz;
}

//*****************************************************************************
void LocalCartesian::Forward(double lat, double lon, double h,
    [System::Runtime::InteropServices::Out] double% x,
    [System::Runtime::InteropServices::Out] double% y,
    [System::Runtime::InteropServices::Out] double% z,
    [System::Runtime::InteropServices::Out] array<double,2>^% M)
{
    double lx, ly, lz;
    std::vector<double> lM(9);
    m_pLocalCartesian->Forward( lat, lon, h, lx, ly, lz, lM );
    x = lx;
    y = ly;
    z = lz;
    M = gcnew array<double,2>(3,3);
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            M[i,j] = lM[3*i+j];
}

//*****************************************************************************
void LocalCartesian::Reverse(double x, double y, double z,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon,
    [System::Runtime::InteropServices::Out] double% h)
{
    double llat, llon, lh;
    m_pLocalCartesian->Reverse( x, y, z, llat, llon, lh );
    lat = llat;
    lon = llon;
    h = lh;
}

//*****************************************************************************
void LocalCartesian::Reverse(double x, double y, double z,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon,
    [System::Runtime::InteropServices::Out] double% h,
    [System::Runtime::InteropServices::Out] array<double,2>^% M)
{
    double llat, llon, lh;
    std::vector<double> lM(9);
    m_pLocalCartesian->Reverse( x, y, z, llat, llon, lh, lM );
    lat = llat;
    lon = llon;
    h = lh;
    M = gcnew array<double,2>(3,3);
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            M[i,j] = lM[3*i+j];
}

//*****************************************************************************
double LocalCartesian::LatitudeOrigin::get()
{ return m_pLocalCartesian->LatitudeOrigin(); }

//*****************************************************************************
double LocalCartesian::LongitudeOrigin::get()
{ return m_pLocalCartesian->LongitudeOrigin(); }

//*****************************************************************************
double LocalCartesian::HeightOrigin::get()
{ return m_pLocalCartesian->HeightOrigin(); }

//*****************************************************************************
double LocalCartesian::MajorRadius::get()
{ return m_pLocalCartesian->MajorRadius(); }

//*****************************************************************************
double LocalCartesian::Flattening::get()
{ return m_pLocalCartesian->Flattening(); }
