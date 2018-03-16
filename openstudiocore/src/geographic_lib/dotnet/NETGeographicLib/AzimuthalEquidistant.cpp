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
#include "GeographicLib/AzimuthalEquidistant.hpp"
#include "AzimuthalEquidistant.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::AzimuthalEquidistant";

//*****************************************************************************
AzimuthalEquidistant::!AzimuthalEquidistant()
{
    if ( m_pAzimuthalEquidistant != NULL )
    {
        delete m_pAzimuthalEquidistant;
        m_pAzimuthalEquidistant = NULL;
    }
}

//*****************************************************************************
AzimuthalEquidistant::AzimuthalEquidistant(void)
{
    try
    {
        m_pAzimuthalEquidistant = new GeographicLib::AzimuthalEquidistant( GeographicLib::Geodesic::WGS84() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
AzimuthalEquidistant::AzimuthalEquidistant( Geodesic^ g )
{
    try
    {
        const GeographicLib::Geodesic* pGeodesic =
            reinterpret_cast<const GeographicLib::Geodesic*>(
                g->GetUnmanaged()->ToPointer() );
        m_pAzimuthalEquidistant = new GeographicLib::AzimuthalEquidistant( *pGeodesic );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void AzimuthalEquidistant::Forward(double lat0, double lon0, double lat, double lon,
                double% x, double% y, double% azi, double% rk)
{
    double lx, ly, lazi, lrk;
    m_pAzimuthalEquidistant->Forward( lat0, lon0, lat, lon,
            lx, ly, lazi, lrk );
    x = lx;
    y = ly;
    azi = lazi;
    rk = lrk;
}

//*****************************************************************************
void AzimuthalEquidistant::Reverse(double lat0, double lon0, double x, double y,
                double% lat, double% lon, double% azi, double% rk)
{
    double llat, llon, lazi, lrk;
    m_pAzimuthalEquidistant->Reverse(lat0, lon0, x, y,
            llat, llon, lazi, lrk);
    lat = llat;
    lon = llon;
    azi = lazi;
    rk = lrk;
}
//*****************************************************************************
void AzimuthalEquidistant::Forward(double lat0, double lon0, double lat, double lon,
                double% x, double% y)
{
    double azi, rk, lx, ly;
    m_pAzimuthalEquidistant->Forward(lat0, lon0, lat, lon, lx, ly, azi, rk);
    x = lx;
    y = ly;
}

//*****************************************************************************
void AzimuthalEquidistant::Reverse(double lat0, double lon0, double x, double y,
                double% lat, double% lon)
{
    double azi, rk, llat, llon;
    m_pAzimuthalEquidistant->Reverse(lat0, lon0, x, y, llat, llon, azi, rk);
    lat = llat;
    lon = llon;
}

//*****************************************************************************
double AzimuthalEquidistant::MajorRadius::get()
{ return m_pAzimuthalEquidistant->MajorRadius(); }

//*****************************************************************************
double AzimuthalEquidistant::Flattening::get()
{ return m_pAzimuthalEquidistant->Flattening(); }
