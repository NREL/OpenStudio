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
#include "GeographicLib/GeodesicLineExact.hpp"
#include "GeodesicLineExact.h"
#include "GeodesicExact.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::GeodesicLineExact";

//*****************************************************************************
GeodesicLineExact::!GeodesicLineExact(void)
{
    if ( m_pGeodesicLineExact != NULL )
    {
        delete m_pGeodesicLineExact;
        m_pGeodesicLineExact = NULL;
    }
}

//*****************************************************************************
GeodesicLineExact::GeodesicLineExact(GeodesicExact^ g, double lat1,
    double lon1, double azi1, NETGeographicLib::Mask caps )
{
    try
    {
        const GeographicLib::GeodesicExact* pGeodesicExact=
            reinterpret_cast<const GeographicLib::GeodesicExact*>(
                g->GetUnmanaged()->ToPointer() );
        m_pGeodesicLineExact = new GeographicLib::GeodesicLineExact(
            *pGeodesicExact, lat1, lon1, azi1, static_cast<unsigned>(caps) );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
GeodesicLineExact::GeodesicLineExact(double lat1, double lon1, double azi1,
                                     NETGeographicLib::Mask caps)
{
    try
    {
        m_pGeodesicLineExact = new GeographicLib::GeodesicLineExact(
            GeographicLib::GeodesicExact::WGS84(), lat1, lon1, azi1,
            static_cast<unsigned>(caps) );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
double GeodesicLineExact::Position(double s12,
        [System::Runtime::InteropServices::Out] double% lat2,
        [System::Runtime::InteropServices::Out] double% lon2,
        [System::Runtime::InteropServices::Out] double% azi2,
        [System::Runtime::InteropServices::Out] double% m12,
        [System::Runtime::InteropServices::Out] double% M12,
        [System::Runtime::InteropServices::Out] double% M21,
        [System::Runtime::InteropServices::Out] double% S12)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesicLineExact->Position( s12, llat2, llon2, lazi2,
        lm12, lM12, lM21, lS12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    S12 = lS12;
    return out;
}

//*****************************************************************************
double GeodesicLineExact::Position(double s12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2)
{
    double llat2, llon2;
    double out = m_pGeodesicLineExact->Position( s12, llat2, llon2);
    lat2 = llat2;
    lon2 = llon2;
    return out;
}

//*****************************************************************************
double GeodesicLineExact::Position(double s12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2)
{
    double llat2, llon2, lazi2;
    double out = m_pGeodesicLineExact->Position( s12, llat2, llon2, lazi2);
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    return out;
}

//*****************************************************************************
double GeodesicLineExact::Position(double s12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% m12)
{
    double llat2, llon2, lazi2, lm12;
    double out = m_pGeodesicLineExact->Position( s12, llat2, llon2, lazi2,
        lm12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    return out;
}

//*****************************************************************************
double GeodesicLineExact::Position(double s12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% M12,
    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, lM12, lM21;
    double out = m_pGeodesicLineExact->Position( s12, llat2, llon2, lazi2,
        lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double GeodesicLineExact::Position(double s12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% m12,
    [System::Runtime::InteropServices::Out] double% M12,
    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21;
    double out = m_pGeodesicLineExact->Position( s12, llat2, llon2, lazi2,
        lm12, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% s12,
    [System::Runtime::InteropServices::Out] double% m12,
    [System::Runtime::InteropServices::Out] double% M12,
    [System::Runtime::InteropServices::Out] double% M21,
    [System::Runtime::InteropServices::Out] double% S12)
{
    double llat2, llon2, lazi2, ls12, lm12, lM12, lM21, lS12;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2, lazi2, ls12,
        lm12, lM12, lM21, lS12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    S12 = lS12;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2)
{
    double llat2, llon2;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2 );
    lat2 = llat2;
    lon2 = llon2;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2)
{
    double llat2, llon2, lazi2;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2, lazi2 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% s12)
{
    double llat2, llon2, lazi2, ls12;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2, lazi2, ls12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% s12,
    [System::Runtime::InteropServices::Out] double% m12)
{
    double llat2, llon2, lazi2, ls12, lm12;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2, lazi2, ls12,
        lm12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% s12,
    [System::Runtime::InteropServices::Out] double% M12,
    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, ls12, lM12, lM21;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2, lazi2, ls12,
        lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    M12 = lM12;
    M21 = lM21;
}

//*****************************************************************************
void GeodesicLineExact::ArcPosition(double a12,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% s12,
    [System::Runtime::InteropServices::Out] double% m12,
    [System::Runtime::InteropServices::Out] double% M12,
    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, ls12, lm12, lM12, lM21;
    m_pGeodesicLineExact->ArcPosition( a12, llat2, llon2, lazi2, ls12,
        lm12, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
}

//*****************************************************************************
double GeodesicLineExact::GenPosition(bool arcmode, double s12_a12,
    GeodesicLineExact::mask outmask,
    [System::Runtime::InteropServices::Out] double% lat2,
    [System::Runtime::InteropServices::Out] double% lon2,
    [System::Runtime::InteropServices::Out] double% azi2,
    [System::Runtime::InteropServices::Out] double% s12,
    [System::Runtime::InteropServices::Out] double% m12,
    [System::Runtime::InteropServices::Out] double% M12,
    [System::Runtime::InteropServices::Out] double% M21,
    [System::Runtime::InteropServices::Out] double% S12)
{
    double llat2, llon2, lazi2, ls12, lm12, lM12, lM21, lS12;
    double out = m_pGeodesicLineExact->GenPosition( arcmode, s12_a12,
        static_cast<unsigned>(outmask),
        llat2, llon2, lazi2, ls12, lm12, lM12, lM21, lS12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    S12 = lS12;
    return out;
}

//*****************************************************************************
double GeodesicLineExact::Latitude::get()
{ return m_pGeodesicLineExact->Latitude(); }

//*****************************************************************************
double GeodesicLineExact::Longitude::get()
{ return m_pGeodesicLineExact->Longitude(); }

//*****************************************************************************
double GeodesicLineExact::Azimuth::get()
{ return m_pGeodesicLineExact->Azimuth(); }

//*****************************************************************************
double GeodesicLineExact::EquatorialAzimuth::get()
{ return m_pGeodesicLineExact->EquatorialAzimuth(); }

//*****************************************************************************
double GeodesicLineExact::EquatorialArc::get()
{ return m_pGeodesicLineExact->EquatorialArc(); }

//*****************************************************************************
double GeodesicLineExact::MajorRadius::get()
{ return m_pGeodesicLineExact->MajorRadius(); }

//*****************************************************************************
double GeodesicLineExact::Flattening::get()
{ return m_pGeodesicLineExact->Flattening(); }

//*****************************************************************************
NETGeographicLib::Mask GeodesicLineExact::Capabilities()
{ return static_cast<NETGeographicLib::Mask>(m_pGeodesicLineExact->Capabilities()); }

//*****************************************************************************
bool GeodesicLineExact::Capabilities(NETGeographicLib::Mask testcaps)
{ return m_pGeodesicLineExact->Capabilities(static_cast<unsigned>(testcaps)); }
