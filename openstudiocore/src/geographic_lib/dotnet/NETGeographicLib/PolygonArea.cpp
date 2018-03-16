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
#include "GeographicLib/PolygonArea.hpp"
#include "PolygonArea.h"
#include "Geodesic.h"
#include "GeodesicExact.h"
#include "Rhumb.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::PolygonArea";

//*****************************************************************************
PolygonArea::!PolygonArea(void)
{
    if ( m_pPolygonArea != NULL )
    {
        delete m_pPolygonArea;
        m_pPolygonArea = NULL;
    }
}

//*****************************************************************************
PolygonArea::PolygonArea(Geodesic^ earth, bool polyline )
{
    try
    {
        const GeographicLib::Geodesic* pGeodesic =
            reinterpret_cast<const GeographicLib::Geodesic*>(
                earth->GetUnmanaged()->ToPointer() );
        m_pPolygonArea = new GeographicLib::PolygonArea( *pGeodesic, polyline );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
PolygonArea::PolygonArea(const bool polyline )
{
    try
    {
        m_pPolygonArea = new GeographicLib::PolygonArea(
            GeographicLib::Geodesic::WGS84(), polyline );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void PolygonArea::Clear() { m_pPolygonArea->Clear(); }

//*****************************************************************************
void PolygonArea::AddPoint(double lat, double lon)
{
    m_pPolygonArea->AddPoint( lat, lon );
}

//*****************************************************************************
void PolygonArea::AddEdge(double azi, double s)
{
    m_pPolygonArea->AddEdge( azi, s );
}

//*****************************************************************************
unsigned PolygonArea::Compute(bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->Compute( reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
unsigned PolygonArea::TestPoint(double lat, double lon, bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->TestPoint( lat, lon, reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
unsigned PolygonArea::TestEdge(double azi, double s, bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->TestEdge( azi, s, reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
void PolygonArea::CurrentPoint(
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon)
{
    double llat, llon;
    m_pPolygonArea->CurrentPoint( llat, llon );
    lat = llat;
    lon = llon;
}

//*****************************************************************************
double PolygonArea::MajorRadius::get()
{ return m_pPolygonArea->MajorRadius(); }

//*****************************************************************************
double PolygonArea::Flattening::get() { return m_pPolygonArea->Flattening(); }

//*****************************************************************************
// PolygonAreaExact
//*****************************************************************************
PolygonAreaExact::!PolygonAreaExact(void)
{
    if ( m_pPolygonArea != NULL )
    {
        delete m_pPolygonArea;
        m_pPolygonArea = NULL;
    }
}

//*****************************************************************************
PolygonAreaExact::PolygonAreaExact(GeodesicExact^ earth, bool polyline )
{
    try
    {
        const GeographicLib::GeodesicExact* pGeodesic =
            reinterpret_cast<const GeographicLib::GeodesicExact*>(
                earth->GetUnmanaged()->ToPointer() );
        m_pPolygonArea = new GeographicLib::PolygonAreaExact( *pGeodesic, polyline );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
PolygonAreaExact::PolygonAreaExact(const bool polyline )
{
    try
    {
        m_pPolygonArea = new GeographicLib::PolygonAreaExact(
            GeographicLib::GeodesicExact::WGS84(), polyline );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void PolygonAreaExact::Clear() { m_pPolygonArea->Clear(); }

//*****************************************************************************
void PolygonAreaExact::AddPoint(double lat, double lon)
{
    m_pPolygonArea->AddPoint( lat, lon );
}

//*****************************************************************************
void PolygonAreaExact::AddEdge(double azi, double s)
{
    m_pPolygonArea->AddEdge( azi, s );
}

//*****************************************************************************
unsigned PolygonAreaExact::Compute(bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->Compute( reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
unsigned PolygonAreaExact::TestPoint(double lat, double lon, bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->TestPoint( lat, lon, reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
unsigned PolygonAreaExact::TestEdge(double azi, double s, bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->TestEdge( azi, s, reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
void PolygonAreaExact::CurrentPoint(
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon)
{
    double llat, llon;
    m_pPolygonArea->CurrentPoint( llat, llon );
    lat = llat;
    lon = llon;
}

//*****************************************************************************
double PolygonAreaExact::MajorRadius::get()
{ return m_pPolygonArea->MajorRadius(); }

//*****************************************************************************
double PolygonAreaExact::Flattening::get()
{ return m_pPolygonArea->Flattening(); }

//*****************************************************************************
// PolygonAreaRhumb
//*****************************************************************************
PolygonAreaRhumb::!PolygonAreaRhumb(void)
{
    if ( m_pPolygonArea != NULL )
    {
        delete m_pPolygonArea;
        m_pPolygonArea = NULL;
    }
}

//*****************************************************************************
PolygonAreaRhumb::PolygonAreaRhumb(Rhumb^ earth, bool polyline )
{
    try
    {
        const GeographicLib::Rhumb* pGeodesic =
            reinterpret_cast<const GeographicLib::Rhumb*>(
                earth->GetUnmanaged()->ToPointer() );
        m_pPolygonArea = new GeographicLib::PolygonAreaRhumb( *pGeodesic, polyline );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
PolygonAreaRhumb::PolygonAreaRhumb(const bool polyline )
{
    try
    {
        m_pPolygonArea = new GeographicLib::PolygonAreaRhumb(
            GeographicLib::Rhumb::WGS84(), polyline );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void PolygonAreaRhumb::Clear() { m_pPolygonArea->Clear(); }

//*****************************************************************************
void PolygonAreaRhumb::AddPoint(double lat, double lon)
{
    m_pPolygonArea->AddPoint( lat, lon );
}

//*****************************************************************************
void PolygonAreaRhumb::AddEdge(double azi, double s)
{
    m_pPolygonArea->AddEdge( azi, s );
}

//*****************************************************************************
unsigned PolygonAreaRhumb::Compute(bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->Compute( reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
unsigned PolygonAreaRhumb::TestPoint(double lat, double lon, bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->TestPoint( lat, lon, reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
unsigned PolygonAreaRhumb::TestEdge(double azi, double s, bool reverse, bool sign,
                    [System::Runtime::InteropServices::Out] double% perimeter,
                    [System::Runtime::InteropServices::Out] double% area)
{
    double lperimeter, larea;
    unsigned out = m_pPolygonArea->TestEdge( azi, s, reverse, sign, lperimeter, larea );
    perimeter = lperimeter;
    area = larea;
    return out;
}

//*****************************************************************************
void PolygonAreaRhumb::CurrentPoint(
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon)
{
    double llat, llon;
    m_pPolygonArea->CurrentPoint( llat, llon );
    lat = llat;
    lon = llon;
}

//*****************************************************************************
double PolygonAreaRhumb::MajorRadius::get()
{ return m_pPolygonArea->MajorRadius(); }

//*****************************************************************************
double PolygonAreaRhumb::Flattening::get()
{ return m_pPolygonArea->Flattening(); }
