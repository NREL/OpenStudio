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
#include "GeographicLib/NormalGravity.hpp"
#include "NormalGravity.h"
#include "Geocentric.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::NormalGravity";

//*****************************************************************************
NormalGravity::!NormalGravity(void)
{
    if ( m_pNormalGravity != NULL )
    {
        delete m_pNormalGravity;
        m_pNormalGravity = NULL;
    }
}

//*****************************************************************************
NormalGravity::NormalGravity(double a, double GM, double omega, double f, double J2)
{
    try
    {
        m_pNormalGravity = new GeographicLib::NormalGravity( a, GM, omega, f, J2 );
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
NormalGravity::NormalGravity(StandardModels model)
{
    try
    {
        m_pNormalGravity = model == StandardModels::WGS84 ?
            new GeographicLib::NormalGravity( GeographicLib::NormalGravity::WGS84() ) :
            new GeographicLib::NormalGravity( GeographicLib::NormalGravity::GRS80() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
NormalGravity::NormalGravity( const GeographicLib::NormalGravity& g)
{
    try
    {
        m_pNormalGravity = new GeographicLib::NormalGravity( g );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
double NormalGravity::SurfaceGravity(double lat)
{
    return m_pNormalGravity->SurfaceGravity( lat );
}

//*****************************************************************************
double NormalGravity::Gravity(double lat, double h,
    [System::Runtime::InteropServices::Out] double% gammay,
    [System::Runtime::InteropServices::Out] double% gammaz)
{
    double ly, lz;
    double out = m_pNormalGravity->Gravity( lat, h, ly, lz );
    gammay = ly;
    gammaz = lz;
    return out;
}

//*****************************************************************************
double NormalGravity::U(double X, double Y, double Z,
                [System::Runtime::InteropServices::Out] double% gammaX,
                [System::Runtime::InteropServices::Out] double% gammaY,
                [System::Runtime::InteropServices::Out] double% gammaZ)
{
    double lx, ly, lz;
    double out = m_pNormalGravity->U( X, Y, Z, lx, ly, lz );
    gammaX = lx;
    gammaY = ly;
    gammaZ = lz;
    return out;
}

//*****************************************************************************
double NormalGravity::V0(double X, double Y, double Z,
                [System::Runtime::InteropServices::Out] double% GammaX,
                [System::Runtime::InteropServices::Out] double% GammaY,
                [System::Runtime::InteropServices::Out] double% GammaZ)
{
    double lx, ly, lz;
    double out = m_pNormalGravity->V0( X, Y, Z, lx, ly, lz );
    GammaX = lx;
    GammaY = ly;
    GammaZ = lz;
    return out;
}

//*****************************************************************************
double NormalGravity::Phi(double X, double Y,
    [System::Runtime::InteropServices::Out] double% fX,
    [System::Runtime::InteropServices::Out] double% fY)
{
    double lx, ly;
    double out = m_pNormalGravity->Phi( X, Y, lx, ly );
    fX = lx;
    fY = ly;
    return out;
}

//*****************************************************************************
Geocentric^ NormalGravity::Earth()
{
    return gcnew Geocentric( m_pNormalGravity->Earth() );
}

//*****************************************************************************
double NormalGravity::MajorRadius::get()
{ return m_pNormalGravity->MajorRadius(); }

//*****************************************************************************
double NormalGravity::MassConstant::get()
{ return m_pNormalGravity->MassConstant(); }

//*****************************************************************************
double NormalGravity::DynamicalFormFactor(int n)
{ return m_pNormalGravity->DynamicalFormFactor(n); }

//*****************************************************************************
double NormalGravity::AngularVelocity::get()
{ return m_pNormalGravity->AngularVelocity(); }

//*****************************************************************************
double NormalGravity::Flattening::get()
{ return m_pNormalGravity->Flattening(); }

//*****************************************************************************
double NormalGravity::EquatorialGravity::get()
{ return m_pNormalGravity->EquatorialGravity(); }

//*****************************************************************************
double NormalGravity::PolarGravity::get()
{ return m_pNormalGravity->PolarGravity(); }

//*****************************************************************************
double NormalGravity::GravityFlattening::get()
{ return m_pNormalGravity->GravityFlattening(); }

//*****************************************************************************
double NormalGravity::SurfacePotential::get()
{ return m_pNormalGravity->SurfacePotential(); }

//*****************************************************************************
NormalGravity^ NormalGravity::WGS84()
{
    return gcnew NormalGravity( StandardModels::WGS84 );
}

//*****************************************************************************
NormalGravity^ NormalGravity::GRS80()
{
    return gcnew NormalGravity( StandardModels::GRS80 );
}

//*****************************************************************************
double NormalGravity::J2ToFlattening(double a, double GM, double omega,
                                     double J2)
{
    return GeographicLib::NormalGravity::J2ToFlattening( a, GM, omega, J2);
}

//*****************************************************************************
double NormalGravity::FlatteningToJ2(double a, double GM, double omega,
                                     double f)
{
    return GeographicLib::NormalGravity::FlatteningToJ2( a, GM, omega, f);
}
