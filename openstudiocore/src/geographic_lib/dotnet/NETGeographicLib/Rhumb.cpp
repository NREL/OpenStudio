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
#include "GeographicLib/Rhumb.hpp"
#include "Rhumb.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
Rhumb::!Rhumb(void)
{
    if ( m_pRhumb != NULL )
    {
        delete m_pRhumb;
        m_pRhumb = NULL;
    }
}

//*****************************************************************************
Rhumb::Rhumb(double a, double f, bool exact)
{
    try
    {
        m_pRhumb = new GeographicLib::Rhumb( a, f, exact );
    }
    catch ( GeographicLib::GeographicErr& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew System::Exception("Failed to allocate memory for a Rhumb.");
    }
}

//*****************************************************************************
void Rhumb::Direct(double lat1, double lon1, double azi12, double s12,
            [System::Runtime::InteropServices::Out] double% lat2,
            [System::Runtime::InteropServices::Out] double% lon2,
            [System::Runtime::InteropServices::Out] double% S12)
{
    double ilat2, ilon2, iS12;
    m_pRhumb->Direct( lat1, lon1, azi12, s12, ilat2, ilon2, iS12 );
    lat2 = ilat2;
    lon2 = ilon2;
    S12 = iS12;
}

//*****************************************************************************
void Rhumb::Direct(double lat1, double lon1, double azi12, double s12,
            [System::Runtime::InteropServices::Out] double% lat2,
            [System::Runtime::InteropServices::Out] double% lon2)
{
    double ilat2, ilon2;
    m_pRhumb->Direct( lat1, lon1, azi12, s12, ilat2, ilon2 );
    lat2 = ilat2;
    lon2 = ilon2;
}

//*****************************************************************************
void Rhumb::GenDirect(double lat1, double lon1, double azi12, double s12,
                Rhumb::mask outmask,
                [System::Runtime::InteropServices::Out] double% lat2,
                [System::Runtime::InteropServices::Out] double% lon2,
                [System::Runtime::InteropServices::Out] double% S12)
{
    double ilat2, ilon2, iS12;
    unsigned int iMask = (unsigned int)outmask;
    m_pRhumb->GenDirect( lat1, lon1, azi12, s12, iMask, ilat2, ilon2, iS12 );
    lat2 = ilat2;
    lon2 = ilon2;
    S12 = iS12;
}

//*****************************************************************************
void Rhumb::Inverse(double lat1, double lon1, double lat2, double lon2,
            [System::Runtime::InteropServices::Out] double% s12,
            [System::Runtime::InteropServices::Out] double% azi12,
            [System::Runtime::InteropServices::Out] double% S12)
{
    double is12, iazi12, iS12;
    m_pRhumb->Inverse( lat1, lon1, lat2, lon2, is12, iazi12, iS12 );
    s12 = is12;
    azi12 = iazi12;
    S12 = iS12;
}

//*****************************************************************************
void Rhumb::Inverse(double lat1, double lon1, double lat2, double lon2,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% azi12)
{
    double is12, iazi12;
    m_pRhumb->Inverse( lat1, lon1, lat2, lon2, is12, iazi12 );
    s12 = is12;
    azi12 = iazi12;
}

//*****************************************************************************
void Rhumb::GenInverse(double lat1, double lon1, double lat2, double lon2,
                Rhumb::mask outmask,
                [System::Runtime::InteropServices::Out] double% s12,
                [System::Runtime::InteropServices::Out] double% azi12,
                [System::Runtime::InteropServices::Out] double% S12)
{
    double is12, iazi12, iS12;
    unsigned int iMask = (unsigned int)outmask;
    m_pRhumb->GenInverse( lat1, lon1, lat2, lon2, iMask, is12, iazi12, iS12 );
    s12 = is12;
    azi12 = iazi12;
    S12 = iS12;
}

//*****************************************************************************
RhumbLine^ Rhumb::Line(double lat1, double lon1, double azi12)
{
    return gcnew RhumbLine( new GeographicLib::RhumbLine(m_pRhumb->Line( lat1, lon1, azi12 )) );
}

//*****************************************************************************
double Rhumb::MajorRadius::get() { return m_pRhumb->MajorRadius(); }

//*****************************************************************************
double Rhumb::Flattening::get() { return m_pRhumb->Flattening(); }

//*****************************************************************************
double Rhumb::EllipsoidArea::get() { return m_pRhumb->EllipsoidArea(); }

//*****************************************************************************
Rhumb^ Rhumb::WGS84()
{
    return gcnew Rhumb( GeographicLib::Constants::WGS84_a(),
                        GeographicLib::Constants::WGS84_f(), false );
}

//*****************************************************************************
System::IntPtr^ Rhumb::GetUnmanaged()
{
    return gcnew System::IntPtr( const_cast<void*>(reinterpret_cast<const void*>(m_pRhumb)) );
}

//*****************************************************************************
// RhumbLine functions
//*****************************************************************************
RhumbLine::!RhumbLine(void)
{
    if ( m_pRhumbLine != NULL )
    {
        delete m_pRhumbLine;
        m_pRhumbLine = NULL;
    }
}

//*****************************************************************************
RhumbLine::RhumbLine( GeographicLib::RhumbLine* pRhumbLine )
{
    if ( pRhumbLine == NULL )
        throw gcnew System::Exception("Invalid pointer in RhumbLine constructor.");
    m_pRhumbLine = pRhumbLine;
}

//*****************************************************************************
void RhumbLine::Position(double s12,
              [System::Runtime::InteropServices::Out] double% lat2,
              [System::Runtime::InteropServices::Out] double% lon2,
              [System::Runtime::InteropServices::Out] double% S12)
{
    double ilat2, ilon2, iS12;
    m_pRhumbLine->Position( s12, ilat2, ilon2, iS12);
    lat2 = ilat2;
    lon2 = ilon2;
    S12 = iS12;
}

//*****************************************************************************
void RhumbLine::Position(double s12,
        [System::Runtime::InteropServices::Out] double% lat2,
        [System::Runtime::InteropServices::Out] double% lon2)
{
    double ilat2, ilon2;
    m_pRhumbLine->Position( s12, ilat2, ilon2 );
    lat2 = ilat2;
    lon2 = ilon2;
}

//*****************************************************************************
void RhumbLine::GenPosition(double s12, RhumbLine::mask outmask,
                 [System::Runtime::InteropServices::Out] double% lat2,
                 [System::Runtime::InteropServices::Out] double% lon2,
                 [System::Runtime::InteropServices::Out] double% S12)
{
    double ilat2, ilon2, iS12;
    unsigned int iMask = (unsigned int)outmask;
    m_pRhumbLine->GenPosition( s12, iMask, ilat2, ilon2, iS12);
    lat2 = ilat2;
    lon2 = ilon2;
    S12 = iS12;
}

//*****************************************************************************
double RhumbLine::Latitude::get()
{
    return m_pRhumbLine->Latitude();
}

//*****************************************************************************
double RhumbLine::Longitude::get()
{
    return m_pRhumbLine->Longitude();
}

//*****************************************************************************
double RhumbLine::Azimuth::get()
{
    return m_pRhumbLine->Azimuth();
}

//*****************************************************************************
double RhumbLine::MajorRadius::get()
{
    return m_pRhumbLine->MajorRadius();
}

//*****************************************************************************
double RhumbLine::Flattening::get()
{
    return m_pRhumbLine->Flattening();
}
