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
#include "GeographicLib/MagneticCircle.hpp"
#include "MagneticCircle.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
MagneticCircle::!MagneticCircle(void)
{
    if ( m_pMagneticCircle != NULL )
    {
        delete m_pMagneticCircle;
        m_pMagneticCircle = NULL;
    }
}

//*****************************************************************************
MagneticCircle::MagneticCircle( const GeographicLib::MagneticCircle& c )
{
    try
    {
        m_pMagneticCircle = new GeographicLib::MagneticCircle( c );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::MagneticCircle" );
    }
}

//*****************************************************************************
void MagneticCircle::Field(double lon,
    [System::Runtime::InteropServices::Out] double% Bx,
    [System::Runtime::InteropServices::Out] double% By,
    [System::Runtime::InteropServices::Out] double% Bz)
{
    double lx, ly, lz;
    m_pMagneticCircle->operator()( lon, lx, ly, lz );
    Bx = lx;
    By = ly;
    Bz = lz;
}

//*****************************************************************************
void MagneticCircle::Field(double lon,
    [System::Runtime::InteropServices::Out] double% Bx,
    [System::Runtime::InteropServices::Out] double% By,
    [System::Runtime::InteropServices::Out] double% Bz,
    [System::Runtime::InteropServices::Out] double% Bxt,
    [System::Runtime::InteropServices::Out] double% Byt,
    [System::Runtime::InteropServices::Out] double% Bzt)
{
    double lx, ly, lz, lxt, lyt, lzt;
    m_pMagneticCircle->operator()( lon, lx, ly, lz, lxt, lyt, lzt );
    Bx = lx;
    By = ly;
    Bz = lz;
    Bxt = lxt;
    Byt = lyt;
    Bzt = lzt;
}

//*****************************************************************************
double MagneticCircle::MajorRadius::get()
{
    if ( m_pMagneticCircle->Init() )
        return m_pMagneticCircle->MajorRadius();
    throw  gcnew GeographicErr("MagneticCircle::MajorRadius failed because the MagneticCircle is not initialized.");
}

//*****************************************************************************
double MagneticCircle::Flattening::get()
{
    if ( m_pMagneticCircle->Init() )
        return m_pMagneticCircle->Flattening();
    throw  gcnew GeographicErr("MagneticCircle::Flattening failed because the MagneticCircle is not initialized.");
}

//*****************************************************************************
double MagneticCircle::Latitude::get()
{
    if ( m_pMagneticCircle->Init() )
        return m_pMagneticCircle->Latitude();
    throw  gcnew GeographicErr("MagneticCircle::Latitude failed because the MagneticCircle is not initialized.");
}

//*****************************************************************************
double MagneticCircle::Height::get()
{
    if ( m_pMagneticCircle->Init() )
        return m_pMagneticCircle->Height();
    throw  gcnew GeographicErr("MagneticCircle::Height failed because the MagneticCircle is not initialized.");
}

//*****************************************************************************
double MagneticCircle::Time::get()
{
    if ( m_pMagneticCircle->Init() )
        return m_pMagneticCircle->Height();
    throw  gcnew GeographicErr("MagneticCircle::Height failed because the MagneticCircle is not initialized.");
}

//*****************************************************************************
bool MagneticCircle::Init::get() { return m_pMagneticCircle->Init(); }
