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
#include "GeographicLib/SphericalHarmonic.hpp"
#include "SphericalHarmonic.h"
#include "CircularEngine.h"
#include "SphericalCoefficients.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

const char BADALLOC[] = "Failed to allocate memory for a GeographicLib::SphericalHarmonic";

//*****************************************************************************
SphericalHarmonic::!SphericalHarmonic(void)
{
    if ( m_pSphericalHarmonic != NULL )
    {
        delete m_pSphericalHarmonic;
        m_pSphericalHarmonic = NULL;
    }
    if ( m_C != NULL )
    {
        delete m_C;
        m_C = NULL;
    }
    if ( m_S != NULL )
    {
        delete m_S;
        m_S = NULL;
    }
}

//*****************************************************************************
SphericalHarmonic::SphericalHarmonic(array<double>^ C,
                    array<double>^ S,
                    int N, double a, Normalization norm )
{
    try
    {
        m_C = new std::vector<double>();
        m_S = new std::vector<double>();
        for each ( double x in C ) m_C->push_back(x);
        for each ( double x in S ) m_S->push_back(x);
        m_pSphericalHarmonic = new GeographicLib::SphericalHarmonic(
            *m_C, *m_S, N, a, static_cast<unsigned>(norm) );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
    catch ( System::Exception^ sxpt )
    {
        throw gcnew GeographicErr( sxpt->Message );
    }
}

//*****************************************************************************
SphericalHarmonic::SphericalHarmonic(array<double>^ C,
                    array<double>^ S,
                    int N, int nmx, int mmx,
                    double a, Normalization norm)
{
    try
    {
        m_C = new std::vector<double>();
        m_S = new std::vector<double>();
        for each ( double x in C ) m_C->push_back(x);
        for each ( double x in S ) m_S->push_back(x);
        m_pSphericalHarmonic = new GeographicLib::SphericalHarmonic(
            *m_C, *m_S, N, nmx, mmx, a, static_cast<unsigned>(norm) );
    }
    catch (std::bad_alloc)
    {
        throw gcnew GeographicErr( BADALLOC );
    }
    catch ( const std::exception& xcpt )
    {
        throw gcnew GeographicErr( xcpt.what() );
    }
    catch ( System::Exception^ sxpt )
    {
        throw gcnew GeographicErr( sxpt->Message );
    }
}

//*****************************************************************************
double SphericalHarmonic::HarmonicSum(double x, double y, double z)
{
    return m_pSphericalHarmonic->operator()( x, y, z );
}

//*****************************************************************************
double SphericalHarmonic::HarmonicSum(double x, double y, double z,
                        double% gradx, double% grady, double% gradz)
{
    double lx, ly, lz;
    double out = m_pSphericalHarmonic->operator()( x, y, z, lx, ly, lz );
    gradx = lx;
    grady = ly;
    gradz = lz;
    return out;
}

//*****************************************************************************
CircularEngine^ SphericalHarmonic::Circle(double p, double z, bool gradp)
{
    return gcnew CircularEngine( m_pSphericalHarmonic->Circle( p, z, gradp ) );
}

//*****************************************************************************
SphericalCoefficients^ SphericalHarmonic::Coefficients()
{
    return gcnew SphericalCoefficients( m_pSphericalHarmonic->Coefficients() );
}
