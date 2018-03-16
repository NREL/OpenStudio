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
#include "GeographicLib/Geocentric.hpp"
#include "Geocentric.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Unable to allocate memory for a GeographicLib::Geocentric";

//*****************************************************************************
Geocentric::!Geocentric()
{
    if ( m_pGeocentric != NULL )
    {
        delete m_pGeocentric;
        m_pGeocentric = NULL;
    }
}

//*****************************************************************************
Geocentric::Geocentric(void)
{
    try
    {
        m_pGeocentric = new GeographicLib::Geocentric(
            GeographicLib::Geocentric::WGS84() );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
Geocentric::Geocentric(double a, double f)
{
    try
    {
        m_pGeocentric = new GeographicLib::Geocentric( a, f );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch (std::exception err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
Geocentric::Geocentric( const GeographicLib::Geocentric& g )
{
    try
    {
        m_pGeocentric = new GeographicLib::Geocentric( g );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
void Geocentric::Forward(double lat, double lon, double h,
    [System::Runtime::InteropServices::Out] double% X,
    [System::Runtime::InteropServices::Out] double% Y,
    [System::Runtime::InteropServices::Out] double% Z)
{
    double lX, lY, lZ;
    m_pGeocentric->Forward( lat, lon, h, lX, lY, lZ);
    X = lX;
    Y = lY;
    Z = lZ;
}

//*****************************************************************************
void Geocentric::Forward(double lat, double lon, double h,
    [System::Runtime::InteropServices::Out] double% X,
    [System::Runtime::InteropServices::Out] double% Y,
    [System::Runtime::InteropServices::Out] double% Z,
    [System::Runtime::InteropServices::Out] array<double,2>^% M)
{
    double lX, lY, lZ;
    std::vector<double> lM(9);
    m_pGeocentric->Forward( lat, lon, h, lX, lY, lZ, lM);
    X = lX;
    Y = lY;
    Z = lZ;
    M = gcnew array<double,2>( 3, 3 );
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            M[i,j] = lM[3*i+j];
}

//*****************************************************************************
void Geocentric::Reverse(double X, double Y, double Z,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon,
    [System::Runtime::InteropServices::Out] double% h)
{
    double llat, llon, lh;
    m_pGeocentric->Reverse(X, Y, Z, llat, llon, lh);
    lat = llat;
    lon = llon;
    h = lh;
}

//*****************************************************************************
void Geocentric::Reverse(double X, double Y, double Z,
    [System::Runtime::InteropServices::Out] double% lat,
    [System::Runtime::InteropServices::Out] double% lon,
    [System::Runtime::InteropServices::Out] double% h,
    [System::Runtime::InteropServices::Out] array<double,2>^% M)
{
    std::vector<double> lM(9);
    double llat, llon, lh;
    m_pGeocentric->Reverse(X, Y, Z, llat, llon, lh, lM);
    lat = llat;
    lon = llon;
    h = lh;
    M = gcnew array<double,2>( 3, 3 );
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            M[i,j] = lM[3*i+j];
}

//*****************************************************************************
System::IntPtr^ Geocentric::GetUnmanaged()
{
    return gcnew System::IntPtr( const_cast<void*>(
        reinterpret_cast<const void*>(m_pGeocentric) ) );
}

//*****************************************************************************
double Geocentric::MajorRadius::get()
{ return m_pGeocentric->MajorRadius(); }

//*****************************************************************************
double Geocentric::Flattening::get()
{ return m_pGeocentric->Flattening(); }
