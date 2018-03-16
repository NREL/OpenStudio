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
#include "GeographicLib/GravityCircle.hpp"
#include "GravityModel.h"
#include "GravityCircle.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
GravityCircle::!GravityCircle(void)
{
    if ( m_pGravityCircle != NULL )
    {
        delete m_pGravityCircle;
        m_pGravityCircle = NULL;
    }
}

//*****************************************************************************
GravityCircle::GravityCircle( const GeographicLib::GravityCircle& gc )
{
    try
    {
        m_pGravityCircle = new GeographicLib::GravityCircle(gc);
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::GravityCircle" );
    }
}

//*****************************************************************************
double GravityCircle::Gravity(double lon,
    [System::Runtime::InteropServices::Out] double% gx,
    [System::Runtime::InteropServices::Out] double% gy,
    [System::Runtime::InteropServices::Out] double% gz)
{
    double lgx, lgy, lgz;
    double out = m_pGravityCircle->Gravity( lon, lgx, lgy, lgz );
    gx = lgx;
    gy = lgy;
    gz = lgz;
    return out;
}

//*****************************************************************************
double GravityCircle::Disturbance(double lon,
    [System::Runtime::InteropServices::Out] double% deltax,
    [System::Runtime::InteropServices::Out] double% deltay,
    [System::Runtime::InteropServices::Out] double% deltaz)
{
    double ldeltax, ldeltay, ldeltaz;
    double out = m_pGravityCircle->Disturbance( lon, ldeltax, ldeltay, ldeltaz );
    deltax = ldeltax;
    deltay = ldeltay;
    deltaz = ldeltaz;
    return out;
}

//*****************************************************************************
double GravityCircle::GeoidHeight(double lon)
{
    return m_pGravityCircle->GeoidHeight( lon );
}

//*****************************************************************************
void GravityCircle::SphericalAnomaly(double lon,
    [System::Runtime::InteropServices::Out] double% Dg01,
    [System::Runtime::InteropServices::Out] double% xi,
    [System::Runtime::InteropServices::Out] double% eta)
{
    double lDg01, lxi, leta;
    m_pGravityCircle->SphericalAnomaly( lon, lDg01, lxi, leta );
    Dg01 = lDg01;
    xi = lxi;
    eta = leta;
}

//*****************************************************************************
double GravityCircle::W(double lon,
    [System::Runtime::InteropServices::Out] double% gX,
    [System::Runtime::InteropServices::Out] double% gY,
    [System::Runtime::InteropServices::Out] double% gZ)
{
    double lgx, lgy, lgz;
    double out = m_pGravityCircle->W( lon, lgx, lgy, lgz );
    gX = lgx;
    gY = lgy;
    gZ = lgz;
    return out;
}

//*****************************************************************************
double GravityCircle::V(double lon,
    [System::Runtime::InteropServices::Out] double% GX,
    [System::Runtime::InteropServices::Out] double% GY,
    [System::Runtime::InteropServices::Out] double% GZ)
{
    double lgx, lgy, lgz;
    double out = m_pGravityCircle->V( lon, lgx, lgy, lgz );
    GX = lgx;
    GY = lgy;
    GZ = lgz;
    return out;
}

//*****************************************************************************
double GravityCircle::T(double lon,
    [System::Runtime::InteropServices::Out] double% deltaX,
    [System::Runtime::InteropServices::Out] double% deltaY,
    [System::Runtime::InteropServices::Out] double% deltaZ)
{
    double lgx, lgy, lgz;
    double out = m_pGravityCircle->T( lon, lgx, lgy, lgz );
    deltaX = lgx;
    deltaY = lgy;
    deltaZ = lgz;
    return out;
}

//*****************************************************************************
double GravityCircle::T(double lon)
{
    return m_pGravityCircle->T( lon );
}

//*****************************************************************************
double GravityCircle::MajorRadius::get()
{
    if ( m_pGravityCircle->Init() )
        return m_pGravityCircle->MajorRadius();
    throw gcnew GeographicErr("GravityCircle::MajorRadius failed because the GravityCircle is not initialized.");
}

//*****************************************************************************
double GravityCircle::Flattening::get()
{
    if ( m_pGravityCircle->Init() )
        return m_pGravityCircle->Flattening();
    throw gcnew GeographicErr("GravityCircle::Flattening failed because the GravityCircle is not initialized.");
}

//*****************************************************************************
double GravityCircle::Latitude::get()
{
    if ( m_pGravityCircle->Init() )
        return m_pGravityCircle->Latitude();
    throw gcnew GeographicErr("GravityCircle::Latitude failed because the GravityCircle is not initialized.");
}

//*****************************************************************************
double GravityCircle::Height::get()
{
    if ( m_pGravityCircle->Init() )
        return m_pGravityCircle->Height();
    throw gcnew GeographicErr("GravityCircle::Height failed because the GravityCircle is not initialized.");
}

//*****************************************************************************
bool GravityCircle::Init::get() { return m_pGravityCircle->Init(); }

//*****************************************************************************
GravityModel::Mask GravityCircle::Capabilities()
{ return static_cast<GravityModel::Mask>(m_pGravityCircle->Capabilities()); }

//*****************************************************************************
bool GravityCircle::Capabilities(GravityModel::Mask testcaps)
{ return m_pGravityCircle->Capabilities( (unsigned int)testcaps ); }
