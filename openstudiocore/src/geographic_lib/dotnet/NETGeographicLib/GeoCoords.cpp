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
#include "GeographicLib/GeoCoords.hpp"
#include "GeoCoords.h"
#include "UTMUPS.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::GeoCoords";

//*****************************************************************************
GeoCoords::!GeoCoords(void)
{
    if ( m_pGeoCoords != NULL )
    {
        delete m_pGeoCoords;
        m_pGeoCoords = NULL;
    }
}

//*****************************************************************************
GeoCoords::GeoCoords()
{
    try
    {
        m_pGeoCoords = new GeographicLib::GeoCoords();
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
GeoCoords::GeoCoords(System::String^ s, bool centerp, bool longfirst )
{
    try
    {
        m_pGeoCoords = new GeographicLib::GeoCoords(StringConvert::ManagedToUnmanaged(s), centerp, longfirst);
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
GeoCoords::GeoCoords(double latitude, double longitude, int zone )
{
    try
    {
        m_pGeoCoords = new GeographicLib::GeoCoords(latitude, longitude, zone);
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
GeoCoords::GeoCoords(int zone, bool northp, double easting, double northing)
{
    try
    {
        m_pGeoCoords = new GeographicLib::GeoCoords(zone, northp, easting, northing);
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void GeoCoords::Reset( System::String^ s, bool centerp, bool longfirst )
{
    try
    {
        m_pGeoCoords->Reset(StringConvert::ManagedToUnmanaged(s), centerp, longfirst);
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void GeoCoords::Reset(double latitude, double longitude, int zone)
{
    try
    {
        m_pGeoCoords->Reset(latitude, longitude, zone);
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void GeoCoords::Reset(int zone, bool northp, double easting, double northing)
{
    try
    {
        m_pGeoCoords->Reset(zone, northp, easting, northing);
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void GeoCoords::AltZone::set( int zone )
{
    try
    {
        m_pGeoCoords->SetAltZone(zone);
    }
    catch ( GeographicLib::GeographicErr err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
int GeoCoords::AltZone::get() { return m_pGeoCoords->AltZone(); }

//*****************************************************************************
double GeoCoords::Latitude::get() { return m_pGeoCoords->Latitude(); }

//*****************************************************************************
double GeoCoords::Longitude::get() { return m_pGeoCoords->Longitude(); }

//*****************************************************************************
double GeoCoords::Easting::get() { return m_pGeoCoords->Easting(); }

//*****************************************************************************
double GeoCoords::Northing::get() { return m_pGeoCoords->Northing(); }

//*****************************************************************************
double GeoCoords::Convergence::get() { return m_pGeoCoords->Convergence(); }

//*****************************************************************************
double GeoCoords::Scale::get() { return m_pGeoCoords->Scale(); }

//*****************************************************************************
bool GeoCoords::Northp::get() { return m_pGeoCoords->Northp(); }

//*****************************************************************************
char GeoCoords::Hemisphere::get() { return m_pGeoCoords->Hemisphere(); }

//*****************************************************************************
int GeoCoords::Zone::get() { return m_pGeoCoords->Zone(); }

//*****************************************************************************
double GeoCoords::AltEasting::get() { return m_pGeoCoords->AltEasting(); }

//*****************************************************************************
double GeoCoords::AltNorthing::get() { return m_pGeoCoords->AltNorthing(); }

//*****************************************************************************
double GeoCoords::AltConvergence::get()
{ return m_pGeoCoords->AltConvergence(); }

//*****************************************************************************
double GeoCoords::AltScale::get() { return m_pGeoCoords->AltScale(); }

//*****************************************************************************
double GeoCoords::MajorRadius::get() { return UTMUPS::MajorRadius(); }

//*****************************************************************************
double GeoCoords::Flattening::get() { return UTMUPS::Flattening(); }

//*****************************************************************************
System::String^ GeoCoords::GeoRepresentation(int prec, bool longfirst )
{
    return gcnew System::String( m_pGeoCoords->GeoRepresentation(prec, longfirst).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::DMSRepresentation(int prec, bool longfirst,
                                char dmssep )
{
    return gcnew System::String( m_pGeoCoords->DMSRepresentation(prec, longfirst, dmssep).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::MGRSRepresentation(int prec)
{
    return gcnew System::String( m_pGeoCoords->MGRSRepresentation(prec).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::UTMUPSRepresentation(int prec, bool abbrev)
{
    return gcnew System::String( m_pGeoCoords->UTMUPSRepresentation(prec, abbrev).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::UTMUPSRepresentation(bool northp, int prec, bool abbrev)
{
    return gcnew System::String( m_pGeoCoords->UTMUPSRepresentation(northp, prec, abbrev).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::AltMGRSRepresentation(int prec)
{
    return gcnew System::String( m_pGeoCoords->AltMGRSRepresentation(prec).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::AltUTMUPSRepresentation(int prec, bool abbrev)
{
    return gcnew System::String( m_pGeoCoords->AltUTMUPSRepresentation(prec, abbrev).c_str() );
}

//*****************************************************************************
System::String^ GeoCoords::AltUTMUPSRepresentation(bool northp, int prec, bool abbrev)
{
    return gcnew System::String( m_pGeoCoords->AltUTMUPSRepresentation(northp, prec, abbrev).c_str() );
}
