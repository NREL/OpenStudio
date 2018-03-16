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
#include "GeographicLib/Ellipsoid.hpp"
#include "Ellipsoid.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::Ellipsoid";

//*****************************************************************************
Ellipsoid::!Ellipsoid(void)
{
    if ( m_pEllipsoid != NULL )
    {
        delete m_pEllipsoid;
        m_pEllipsoid = NULL;
    }
}

//*****************************************************************************
Ellipsoid::Ellipsoid()
{
    try
    {
        m_pEllipsoid = new GeographicLib::Ellipsoid( GeographicLib::Ellipsoid::WGS84() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
}

//*****************************************************************************
Ellipsoid::Ellipsoid(double a, double f)
{
    try
    {
        m_pEllipsoid = new GeographicLib::Ellipsoid( a, f );
    }
    catch ( std::bad_alloc err )
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double Ellipsoid::ParametricLatitude(double phi)
{
    return m_pEllipsoid->ParametricLatitude( phi );
}

//*****************************************************************************
double Ellipsoid::InverseParametricLatitude(double beta)
{
    return m_pEllipsoid->InverseParametricLatitude( beta );
}

//*****************************************************************************
double Ellipsoid::GeocentricLatitude(double phi)
{
    return m_pEllipsoid->GeocentricLatitude( phi );
}

//*****************************************************************************
double Ellipsoid::InverseGeocentricLatitude(double theta)
{
    return m_pEllipsoid->InverseGeocentricLatitude( theta );
}

//*****************************************************************************
double Ellipsoid::RectifyingLatitude(double phi)
{
    return m_pEllipsoid->RectifyingLatitude( phi );
}

//*****************************************************************************
double Ellipsoid::InverseRectifyingLatitude(double mu)
{
    return m_pEllipsoid->InverseRectifyingLatitude( mu );
}

//*****************************************************************************
double Ellipsoid::AuthalicLatitude(double phi)
{
    return m_pEllipsoid->AuthalicLatitude( phi );
}

//*****************************************************************************
double Ellipsoid::InverseAuthalicLatitude(double xi)
{
    return m_pEllipsoid->InverseAuthalicLatitude( xi );
}

//*****************************************************************************
double Ellipsoid::ConformalLatitude(double phi)
{
    return m_pEllipsoid->ConformalLatitude( phi );
}

//*****************************************************************************
double Ellipsoid::InverseConformalLatitude(double chi)
{
    return m_pEllipsoid->InverseConformalLatitude( chi );
}

//*****************************************************************************
double Ellipsoid::IsometricLatitude(double phi)
{
    return m_pEllipsoid->IsometricLatitude( phi );
}

//*****************************************************************************
double Ellipsoid::InverseIsometricLatitude(double psi)
{
    return m_pEllipsoid->InverseIsometricLatitude( psi );
}

//*****************************************************************************
double Ellipsoid::CircleRadius(double phi)
{
    return m_pEllipsoid->CircleRadius( phi );
}

//*****************************************************************************
double Ellipsoid::CircleHeight(double phi)
{
    return m_pEllipsoid->CircleHeight( phi );
}

//*****************************************************************************
double Ellipsoid::MeridianDistance(double phi)
{
    return m_pEllipsoid->MeridianDistance( phi );
}

//*****************************************************************************
double Ellipsoid::MeridionalCurvatureRadius(double phi)
{
    return m_pEllipsoid->MeridionalCurvatureRadius( phi );
}

//*****************************************************************************
double Ellipsoid::TransverseCurvatureRadius(double phi)
{
    return m_pEllipsoid->TransverseCurvatureRadius( phi );
}

//*****************************************************************************
double Ellipsoid::NormalCurvatureRadius(double phi, double azi)
{
    return m_pEllipsoid->NormalCurvatureRadius( phi, azi );
}

//*****************************************************************************
double Ellipsoid::SecondFlatteningToFlattening(double fp)
{
    return GeographicLib::Ellipsoid::SecondFlatteningToFlattening( fp );
}

//*****************************************************************************
double Ellipsoid::FlatteningToSecondFlattening(double f)
{
    return GeographicLib::Ellipsoid::FlatteningToSecondFlattening( f );
}

//*****************************************************************************
double Ellipsoid::ThirdFlatteningToFlattening(double n)
{
    return GeographicLib::Ellipsoid::ThirdFlatteningToFlattening( n );
}

//*****************************************************************************
double Ellipsoid::FlatteningToThirdFlattening(double f)
{
    return GeographicLib::Ellipsoid::FlatteningToThirdFlattening( f );
}

//*****************************************************************************
double Ellipsoid::EccentricitySqToFlattening(double e2)
{
    return GeographicLib::Ellipsoid::EccentricitySqToFlattening( e2 );
}

//*****************************************************************************
double Ellipsoid::FlatteningToEccentricitySq(double f)
{
    return GeographicLib::Ellipsoid::FlatteningToEccentricitySq( f );
}

//*****************************************************************************
double Ellipsoid::SecondEccentricitySqToFlattening(double ep2)
{
    return GeographicLib::Ellipsoid::SecondEccentricitySqToFlattening( ep2 );
}

//*****************************************************************************
double Ellipsoid::FlatteningToSecondEccentricitySq(double f)
{
    return GeographicLib::Ellipsoid::FlatteningToSecondEccentricitySq( f );
}

//*****************************************************************************
double Ellipsoid::ThirdEccentricitySqToFlattening(double epp2)
{
    return GeographicLib::Ellipsoid::ThirdEccentricitySqToFlattening( epp2 );
}

//*****************************************************************************
double Ellipsoid::FlatteningToThirdEccentricitySq(double f)
{
    return GeographicLib::Ellipsoid::FlatteningToThirdEccentricitySq( f );
}

//*****************************************************************************
double Ellipsoid::MajorRadius::get()
{ return m_pEllipsoid->MajorRadius(); }

//*****************************************************************************
double Ellipsoid::MinorRadius::get()
{ return m_pEllipsoid->MinorRadius(); }

//*****************************************************************************
double Ellipsoid::QuarterMeridian::get()
{ return m_pEllipsoid->QuarterMeridian(); }

//*****************************************************************************
double Ellipsoid::Area::get()
{ return m_pEllipsoid->Area(); }

//*****************************************************************************
double Ellipsoid::Volume::get()
{ return m_pEllipsoid->Volume(); }

//*****************************************************************************
double Ellipsoid::Flattening::get()
{ return m_pEllipsoid->Flattening(); }

//*****************************************************************************
double Ellipsoid::SecondFlattening::get()
{ return m_pEllipsoid->SecondFlattening(); }

//*****************************************************************************
double Ellipsoid::ThirdFlattening::get()
{ return m_pEllipsoid->ThirdFlattening(); }

//*****************************************************************************
double Ellipsoid::EccentricitySq::get()
{ return m_pEllipsoid->EccentricitySq(); }

//*****************************************************************************
double Ellipsoid::SecondEccentricitySq::get()
{ return m_pEllipsoid->SecondEccentricitySq(); }

//*****************************************************************************
double Ellipsoid::ThirdEccentricitySq::get()
{ return m_pEllipsoid->ThirdEccentricitySq(); }
