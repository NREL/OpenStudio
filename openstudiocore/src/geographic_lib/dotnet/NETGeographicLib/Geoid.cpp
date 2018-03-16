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
#include "GeographicLib/Geoid.hpp"
#include "Geoid.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
Geoid::!Geoid(void)
{
    if ( m_pGeoid != NULL )
    {
        delete m_pGeoid;
        m_pGeoid = NULL;
    }
}

//*****************************************************************************
Geoid::Geoid(System::String^ name, System::String^ path,
                bool cubic, bool threadsafe)
{
    if ( name == nullptr ) throw gcnew GeographicErr("name cannot be a null pointer.");
    if ( path == nullptr ) throw gcnew GeographicErr("path cannot be a null pointer.");
    try
    {
        m_pGeoid = new GeographicLib::Geoid(
            StringConvert::ManagedToUnmanaged( name ),
            StringConvert::ManagedToUnmanaged( path ),
            cubic, threadsafe );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::Geoid" );
    }
    catch ( const GeographicLib::GeographicErr& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}
//*****************************************************************************
void Geoid::CacheArea(double south, double west, double north, double east)
{
    try
    {
        m_pGeoid->CacheArea( south, west, north, east );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void Geoid::CacheAll()
{
    try
    {
        m_pGeoid->CacheAll();
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void Geoid::CacheClear()
{
    m_pGeoid->CacheClear();
}

//*****************************************************************************
double Geoid::Height(double lat, double lon)
{
    try
    {
        return m_pGeoid->operator()( lat, lon );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double Geoid::Height(double lat, double lon,
    [System::Runtime::InteropServices::Out] double% gradn,
    [System::Runtime::InteropServices::Out] double% grade)
{
    try
    {
        double lgradn, lgrade;
        double out = m_pGeoid->operator()( lat, lon, lgradn, lgrade );
        gradn = lgradn;
        grade = lgrade;
        return out;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double Geoid::ConvertHeight(double lat, double lon, double h,
                            ConvertFlag d)
{
    try
    {
        return m_pGeoid->ConvertHeight( lat, lon, h,
            static_cast<GeographicLib::Geoid::convertflag>(d) );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
System::String^ Geoid::DefaultGeoidPath()
{
    try
    {
        return StringConvert::UnmanagedToManaged(
            GeographicLib::Geoid::DefaultGeoidPath() );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
System::String^ Geoid::DefaultGeoidName()
{
    try
    {
        return StringConvert::UnmanagedToManaged(
            GeographicLib::Geoid::DefaultGeoidName() );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
System::String^ Geoid::Description::get()
{ return StringConvert::UnmanagedToManaged(m_pGeoid->Description()); }

//*****************************************************************************
System::String^ Geoid::DateTime::get()
{ return StringConvert::UnmanagedToManaged(m_pGeoid->DateTime()); }

//*****************************************************************************
System::String^ Geoid::GeoidFile::get()
{ return StringConvert::UnmanagedToManaged(m_pGeoid->GeoidFile()); }

//*****************************************************************************
System::String^ Geoid::GeoidName::get()
{ return StringConvert::UnmanagedToManaged(m_pGeoid->GeoidName()); }

//*****************************************************************************
System::String^ Geoid::GeoidDirectory::get()
{ return StringConvert::UnmanagedToManaged(m_pGeoid->GeoidDirectory()); }

//*****************************************************************************
System::String^ Geoid::Interpolation::get()
{ return StringConvert::UnmanagedToManaged(m_pGeoid->Interpolation()); }

//*****************************************************************************
double Geoid::MaxError::get() { return m_pGeoid->MaxError(); }

//*****************************************************************************
double Geoid::RMSError::get() { return m_pGeoid->RMSError(); }

//*****************************************************************************
double Geoid::Offset::get() { return m_pGeoid->Offset(); }

//*****************************************************************************
double Geoid::Scale::get() { return m_pGeoid->Scale(); }

//*****************************************************************************
bool Geoid::ThreadSafe::get() { return m_pGeoid->ThreadSafe(); }

//*****************************************************************************
bool Geoid::Cache::get() { return m_pGeoid->Cache(); }

//*****************************************************************************
double Geoid::CacheWest::get() { return m_pGeoid->CacheWest(); }

//*****************************************************************************
double Geoid::CacheEast::get() { return m_pGeoid->CacheEast(); }

//*****************************************************************************
double Geoid::CacheNorth::get() { return m_pGeoid->CacheNorth(); }

//*****************************************************************************
double Geoid::CacheSouth::get() { return m_pGeoid->CacheSouth(); }

//*****************************************************************************
double Geoid::MajorRadius::get() { return m_pGeoid->MajorRadius(); }

//*****************************************************************************
double Geoid::Flattening::get() { return m_pGeoid->Flattening(); }
