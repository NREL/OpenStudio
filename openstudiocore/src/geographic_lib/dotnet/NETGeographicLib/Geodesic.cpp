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
#include <GeographicLib/Geodesic.hpp>
#include "Geodesic.h"
#include "GeodesicLine.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::Geodesic";

//*****************************************************************************
Geodesic::!Geodesic()
{
    if ( m_pGeodesic != NULL )
    {
        delete m_pGeodesic;
        m_pGeodesic = NULL;
    }
}

//*****************************************************************************
Geodesic::Geodesic(double a, double f)
{
    try
    {
        m_pGeodesic = new GeographicLib::Geodesic( a, f );
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
Geodesic::Geodesic()
{
    try
    {
        m_pGeodesic = new GeographicLib::Geodesic( GeographicLib::Geodesic::WGS84() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
double Geodesic::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21,
                    [System::Runtime::InteropServices::Out] double% S12)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesic->Direct(lat1, lon1, azi1, s12,
                    llat2, llon2, lazi2, lm12, lM12, lM21, lS12);
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
double Geodesic::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2)
{
    double llat2, llon2;
    double out = m_pGeodesic->Direct(lat1, lon1, azi1, s12,
                    llat2, llon2 );
    lat2 = llat2;
    lon2 = llon2;
    return out;
}

//*****************************************************************************
double Geodesic::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2)
{
    double llat2, llon2, lazi2;
    double out = m_pGeodesic->Direct(lat1, lon1, azi1, s12,
                    llat2, llon2, lazi2 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    return out;
}

//*****************************************************************************
double Geodesic::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12)
{
    double llat2, llon2, lazi2, lm12;
    double out = m_pGeodesic->Direct(lat1, lon1, azi1, s12,
                    llat2, llon2, lazi2, lm12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    return out;
}

//*****************************************************************************
double Geodesic::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, lM12, lM21;
    double out = m_pGeodesic->Direct(lat1, lon1, azi1, s12,
                    llat2, llon2, lazi2, lM12, lM21);
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double Geodesic::Direct(double lat1, double lon1, double azi1, double s12,
                    [System::Runtime::InteropServices::Out] double% lat2,
                    [System::Runtime::InteropServices::Out] double% lon2,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21;
    double out = m_pGeodesic->Direct(lat1, lon1, azi1, s12,
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
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
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
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12,
            llat2, llon2, lazi2, ls12, lm12, lM12, lM21, lS12);
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
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2)
{
    double llat2, llon2;
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12, llat2, llon2 );
    lat2 = llat2;
    lon2 = llon2;
}

//*****************************************************************************
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
                   [System::Runtime::InteropServices::Out] double% lat2,
                   [System::Runtime::InteropServices::Out] double% lon2,
                   [System::Runtime::InteropServices::Out] double% azi2)
{
    double llat2, llon2, lazi2;
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12, llat2, llon2, lazi2 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
}

//*****************************************************************************
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12)
{
    double llat2, llon2, lazi2, ls12;
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12,
            llat2, llon2, lazi2, ls12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
}

//*****************************************************************************
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% m12)
{
    double llat2, llon2, lazi2, ls12, lm12;
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12,
            llat2, llon2, lazi2, ls12, lm12 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
}

//*****************************************************************************
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% M12,
                [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, ls12, lM12, lM21;
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12,
            llat2, llon2, lazi2, ls12, lM12, lM21 );
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    s12 = ls12;
    M12 = lM12;
    M21 = lM21;
}

//*****************************************************************************
void Geodesic::ArcDirect(double lat1, double lon1, double azi1, double a12,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% azi2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% m12,
                [System::Runtime::InteropServices::Out] double% M12,
                [System::Runtime::InteropServices::Out] double% M21)
{
    double llat2, llon2, lazi2, ls12, lm12, lM12, lM21;
    m_pGeodesic->ArcDirect(lat1, lon1, azi1, a12,
            llat2, llon2, lazi2, ls12, lm12, lM12, lM21);
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    s12 = ls12;
    M12 = lM12;
    M21 = lM21;
}

//*****************************************************************************
double Geodesic::GenDirect(double lat1, double lon1, double azi1,
                        bool arcmode, double s12_a12,
                        Geodesic::mask outmask,
                        [System::Runtime::InteropServices::Out] double% lat2,
                        [System::Runtime::InteropServices::Out] double% lon2,
                        [System::Runtime::InteropServices::Out] double% azi2,
                        [System::Runtime::InteropServices::Out] double% s12,
                        [System::Runtime::InteropServices::Out] double% m12,
                        [System::Runtime::InteropServices::Out] double% M12,
                        [System::Runtime::InteropServices::Out] double% M21,
                        [System::Runtime::InteropServices::Out] double% S12)
{
    double llat2, llon2, lazi2, lm12, lM12, lM21, ls12, lS12;
    double out = m_pGeodesic->GenDirect(lat1, lon1, azi1, arcmode, s12_a12,
                    static_cast<unsigned>(outmask),
                    llat2, llon2, lazi2, ls12, lm12, lM12, lM21, lS12);
    lat2 = llat2;
    lon2 = llon2;
    azi2 = lazi2;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    s12 = ls12;
    S12 = lS12;
    return out;
}

//*****************************************************************************
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21,
                    [System::Runtime::InteropServices::Out] double% S12)
{
    double ls12, lazi1, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2,
                    ls12, lazi1, lazi2, lm12, lM12, lM21, lS12);
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
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12)
{
    double ls12;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2, ls12 );
    s12 = ls12;
    return out;
}

//*****************************************************************************
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2)
{
    double lazi1, lazi2;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2, lazi1, lazi2);
    azi1 = lazi1;
    azi2 = lazi2;
    return out;
}

//*****************************************************************************
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2)
{
    double ls12, lazi1, lazi2;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2,
                    ls12, lazi1, lazi2 );
    azi1 = lazi1;
    azi2 = lazi2;
    s12 = ls12;
    return out;
}

//*****************************************************************************
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12)
{
    double ls12, lazi1, lazi2, lm12;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2,
                ls12, lazi1, lazi2, lm12 );
    azi1 = lazi1;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    return out;
}

//*****************************************************************************
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double ls12, lazi1, lazi2, lM12, lM21;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2,
                    ls12, lazi1, lazi2, lM12, lM21 );
    azi1 = lazi1;
    azi2 = lazi2;
    s12 = ls12;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double Geodesic::Inverse(double lat1, double lon1, double lat2, double lon2,
                    [System::Runtime::InteropServices::Out] double% s12,
                    [System::Runtime::InteropServices::Out] double% azi1,
                    [System::Runtime::InteropServices::Out] double% azi2,
                    [System::Runtime::InteropServices::Out] double% m12,
                    [System::Runtime::InteropServices::Out] double% M12,
                    [System::Runtime::InteropServices::Out] double% M21)
{
    double ls12, lazi1, lazi2, lm12, lM12, lM21;
    double out = m_pGeodesic->Inverse(lat1, lon1, lat2, lon2,
                    ls12, lazi1, lazi2, lm12, lM12, lM21 );
    azi1 = lazi1;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    return out;
}

//*****************************************************************************
double Geodesic::GenInverse(double lat1, double lon1, double lat2, double lon2,
                        Geodesic::mask outmask,
                        [System::Runtime::InteropServices::Out] double% s12,
                        [System::Runtime::InteropServices::Out] double% azi1,
                        [System::Runtime::InteropServices::Out] double% azi2,
                        [System::Runtime::InteropServices::Out] double% m12,
                        [System::Runtime::InteropServices::Out] double% M12,
                        [System::Runtime::InteropServices::Out] double% M21,
                        [System::Runtime::InteropServices::Out] double% S12)
{
    double ls12, lazi1, lazi2, lm12, lM12, lM21, lS12;
    double out = m_pGeodesic->GenInverse(lat1, lon1, lat2, lon2,
                    static_cast<unsigned>(outmask),
                    ls12, lazi1, lazi2, lm12, lM12, lM21, lS12);
    azi1 = lazi1;
    azi2 = lazi2;
    s12 = ls12;
    m12 = lm12;
    M12 = lM12;
    M21 = lM21;
    S12 = lS12;
    return out;
}

//*****************************************************************************
System::IntPtr^ Geodesic::GetUnmanaged()
{
    return gcnew System::IntPtr( const_cast<void*>(reinterpret_cast<const void*>(m_pGeodesic)) );
}

//*****************************************************************************
GeodesicLine^ Geodesic::Line(double lat1, double lon1, double azi1,
                             NETGeographicLib::Mask caps )
{
    return gcnew GeodesicLine( this, lat1, lon1, azi1, caps );
}

//*****************************************************************************
double Geodesic::MajorRadius::get() { return m_pGeodesic->MajorRadius(); }

//*****************************************************************************
double Geodesic::Flattening::get() { return m_pGeodesic->Flattening(); }

//*****************************************************************************
double Geodesic::EllipsoidArea::get() { return m_pGeodesic->EllipsoidArea(); }
