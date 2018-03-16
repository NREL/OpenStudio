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
#include "GeographicLib/GeodesicExact.hpp"
#include "GeodesicExact.h"
#include "GeodesicLineExact.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::GeodesicExact";

//*****************************************************************************
GeodesicExact::!GeodesicExact(void)
{
    if ( m_pGeodesicExact != NULL )
    {
        delete m_pGeodesicExact;
        m_pGeodesicExact = NULL;
    }
}

//*****************************************************************************
GeodesicExact::GeodesicExact()
{
    try
    {
        m_pGeodesicExact = new GeographicLib::GeodesicExact( GeographicLib::GeodesicExact::WGS84() );
    }
    catch ( std::bad_alloc err )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
GeodesicExact::GeodesicExact(double a, double f)
{
    try
    {
        m_pGeodesicExact = new GeographicLib::GeodesicExact( a, f );
    }
    catch ( std::bad_alloc err )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( GeographicLib::GeographicErr err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double GeodesicExact::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21,
                    [System::Runtime::InteropServices::Out] double% S12)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesicExact->Direct( lat1, lon1, azi1, s12,
                llat2, llon2, lazi2, lm12, lM12, lM21, lS12 );
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
double GeodesicExact::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2)
{
    double llat2, llon2;
    double out = m_pGeodesicExact->Direct( lat1, lon1, azi1, s12,
                llat2, llon2 );
    lat2 = llat2;
    lon2 = llon2;
    return out;
}

//*****************************************************************************
double GeodesicExact::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2)
{
    double llat2, llon2, lazi2;
    double out = m_pGeodesicExact->Direct( lat1, lon1, azi1, s12,
                llat2, llon2, lazi2 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    return out;
}

//*****************************************************************************
double GeodesicExact::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12)
{
    double llat2, llon2, lazi2, lm12;
    double out = m_pGeodesicExact->Direct( lat1, lon1, azi1, s12,
                llat2, llon2, lazi2, lm12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    return out;
}

//*****************************************************************************
double GeodesicExact::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, lM12, lM21;
    double out = m_pGeodesicExact->Direct( lat1, lon1, azi1, s12,
                llat2, llon2, lazi2, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double GeodesicExact::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21;
    double out = m_pGeodesicExact->Direct( lat1, lon1, azi1, s12,
                llat2, llon2, lazi2, lm12, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
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
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2, lazi2, ls12, lm12, lM12, lM21, lS12 );
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
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2)
{
    double llat2, llon2;
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2 );
    lat2 = llat2;
    lon2 = llon2;
}

//*****************************************************************************
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2)
{
    double llat2, llon2, lazi2;
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2, lazi2 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
}

//*****************************************************************************
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12)
{
    double llat2, llon2, lazi2, ls12;
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2, lazi2, ls12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
}

//*****************************************************************************
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% m12)
{
    double llat2, llon2, lazi2, ls12, lm12;
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2, lazi2, ls12, lm12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
}

//*****************************************************************************
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% M12,
                [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, ls12, lM12, lM21;
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2, lazi2, ls12, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    M12 = lM12;
    M21 = lM21;
}

//*****************************************************************************
void GeodesicExact::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% m12,
                [System::Runtime::InteropServices::Out] double% M12,
                [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, ls12, lm12, lM12, lM21;
    m_pGeodesicExact->ArcDirect( lat1, lon1, azi1, a12,
                llat2, llon2, lazi2, ls12, lm12, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
}

//*****************************************************************************
double GeodesicExact::GenDirect(double lat1, double lon1, double azi1,
                        bool arcmode, double s12_a12,
                        GeodesicExact::mask outmask,
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
    double out = m_pGeodesicExact->GenDirect( lat1, lon1, azi1, arcmode, s12_a12,
                static_cast<unsigned>(outmask), llat2, llon2, lazi2, ls12, lm12, lM12,
                lM21, lS12 );
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
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21,
                    [System::Runtime::InteropServices::Out] double% S12)
{
    double ls12, lazi1, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2,
                ls12, lazi1, lazi2, lm12, lM12, lM21, lS12 );
    s12 = ls12;
    azi1 = lazi1;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    S12 = lS12;
    return out;
}

//*****************************************************************************
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12)
{
    double ls12;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2, ls12 );
    s12 = ls12;
    return out;
}

//*****************************************************************************
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2)
{
    double lazi1, lazi2;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2,
                lazi1, lazi2 );
    azi1 = lazi1;
    azi2 = lazi2;
    return out;
}

//*****************************************************************************
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2)
{
    double ls12, lazi1, lazi2;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2,
                ls12, lazi1, lazi2 );
    s12 = ls12;
    azi1 = lazi1;
    azi2 = lazi2;
    return out;
}

//*****************************************************************************
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12)
{
    double ls12, lazi1, lazi2, lm12;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2,
                ls12, lazi1, lazi2, lm12 );
    s12 = ls12;
    azi1 = lazi1;
    azi2 = lazi2;
    m12 = lm12;
    return out;
}

//*****************************************************************************
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double ls12, lazi1, lazi2, lM12, lM21;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2,
                ls12, lazi1, lazi2, lM12, lM21 );
    s12 = ls12;
    azi1 = lazi1;
    azi2 = lazi2;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double GeodesicExact::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double ls12, lazi1, lazi2, lm12, lM12, lM21;
    double out = m_pGeodesicExact->Inverse( lat1, lon1, lat2, lon2,
                ls12, lazi1, lazi2, lm12, lM12, lM21 );
    s12 = ls12;
    azi1 = lazi1;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double GeodesicExact::GenInverse(double lat1, double lon1, double lat2, double lon2,
                        GeodesicExact::mask outmask,
                        [System::Runtime::InteropServices::Out] double% s12,
                        [System::Runtime::InteropServices::Out] double% azi1,
                        [System::Runtime::InteropServices::Out] double% azi2,
                        [System::Runtime::InteropServices::Out] double% m12,
                        [System::Runtime::InteropServices::Out] double% M12,
                        [System::Runtime::InteropServices::Out] double% M21,
                        [System::Runtime::InteropServices::Out] double% S12)
{
    double ls12, lazi1, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesicExact->GenInverse( lat1, lon1, lat2, lon2,
                static_cast<unsigned>(outmask), ls12, lazi1, lazi2, lm12, lM12,
                lM21, lS12 );
    s12 = ls12;
    azi1 = lazi1;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    S12 = lS12;
    return out;
}

//*****************************************************************************
System::IntPtr^ GeodesicExact::GetUnmanaged()
{
    return gcnew System::IntPtr( const_cast<void*>(
        reinterpret_cast<const void*>(m_pGeodesicExact) ) );
}

//*****************************************************************************
GeodesicLineExact^ GeodesicExact::Line(double lat1, double lon1, double azi1,
    NETGeographicLib::Mask caps )
{
    return gcnew GeodesicLineExact( this, lat1, lon1, azi1, caps );
}

//*****************************************************************************
double GeodesicExact::MajorRadius::get()
{ return m_pGeodesicExact->MajorRadius(); }

//*****************************************************************************
double GeodesicExact::Flattening::get()
{ return m_pGeodesicExact->Flattening(); }

//*****************************************************************************
double GeodesicExact::EllipsoidArea::get()
{ return m_pGeodesicExact->EllipsoidArea(); }
