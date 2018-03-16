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
#include "GeographicLib/CassiniSoldner.hpp"
#include "Geodesic.h"
#include "CassiniSoldner.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::CassiniSoldner";

//*****************************************************************************
CassiniSoldner::!CassiniSoldner()
{
    if ( m_pCassiniSoldner != NULL )
    {
        delete m_pCassiniSoldner;
        m_pCassiniSoldner = NULL;
    }
}

//*****************************************************************************
CassiniSoldner::CassiniSoldner(double lat0, double lon0)
{
    try
    {
        m_pCassiniSoldner = new GeographicLib::CassiniSoldner( GeographicLib::Geodesic::WGS84() );
        m_pCassiniSoldner->Reset(lat0, lon0);
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
CassiniSoldner::CassiniSoldner(double lat0, double lon0,
                Geodesic^ earth )
{
    try
    {
        const GeographicLib::Geodesic* pGeodesic =
            reinterpret_cast<const GeographicLib::Geodesic*>(
                earth->GetUnmanaged()->ToPointer() );
        m_pCassiniSoldner = new GeographicLib::CassiniSoldner( *pGeodesic );
        m_pCassiniSoldner->Reset(lat0, lon0);
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void CassiniSoldner::Reset(double lat0, double lon0)
{
    m_pCassiniSoldner->Reset(lat0, lon0);
}

//*****************************************************************************
void CassiniSoldner::Forward(double lat, double lon,
                [System::Runtime::InteropServices::Out] double% x,
                [System::Runtime::InteropServices::Out] double% y,
                [System::Runtime::InteropServices::Out] double% azi,
                [System::Runtime::InteropServices::Out] double% rk)
{
    double lx, ly, lazi, lrk;
    m_pCassiniSoldner->Forward(lat, lon, lx, ly, lazi, lrk);
    x = lx;
    y = ly;
    azi = lazi;
    rk = lrk;
}

//*****************************************************************************
void CassiniSoldner::Reverse(double x, double y,
                [System::Runtime::InteropServices::Out] double% lat,
                [System::Runtime::InteropServices::Out] double% lon,
                [System::Runtime::InteropServices::Out] double% azi,
                [System::Runtime::InteropServices::Out] double% rk)
{
    double llat, llon, lazi, lrk;
    m_pCassiniSoldner->Reverse( x, y, llat, llon, lazi, lrk );
    lat = llat;
    lon = llon;
    azi = lazi;
    rk = lrk;
}
//*****************************************************************************
void CassiniSoldner::Forward(double lat, double lon,
    [System::Runtime::InteropServices::Out] double% x,
    [System::Runtime::InteropServices::Out] double% y)
{
    double lx, ly, azi, rk;
    m_pCassiniSoldner->Forward(lat, lon, lx, ly, azi, rk);
    x = lx;
    y = ly;
}

//*****************************************************************************
void CassiniSoldner::Reverse(double x, double y,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon)
{
    double llat, llon, lazi, lrk;
    m_pCassiniSoldner->Reverse( x, y, llat, llon, lazi, lrk );
    lat = llat;
    lon = llon;
}

//*****************************************************************************
double CassiniSoldner::LatitudeOrigin::get()
{ return m_pCassiniSoldner->LatitudeOrigin(); }

//*****************************************************************************
double CassiniSoldner::LongitudeOrigin::get()
{ return m_pCassiniSoldner->LongitudeOrigin(); }

//*****************************************************************************
double CassiniSoldner::MajorRadius::get()
{ return m_pCassiniSoldner->MajorRadius(); }

//*****************************************************************************
double CassiniSoldner::Flattening::get()
{ return m_pCassiniSoldner->Flattening(); }
