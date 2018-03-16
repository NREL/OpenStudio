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
#include "GeographicLib/CircularEngine.hpp"
#include "CircularEngine.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
CircularEngine::CircularEngine(const GeographicLib::CircularEngine& c)
{
    try
    {
        m_pCircularEngine = new GeographicLib::CircularEngine( c );
    }
    catch ( GeographicLib::GeographicErr err )
    {
        throw gcnew GeographicErr( err.what() );
    }
    catch ( std::bad_alloc )
    {
        throw gcnew GeographicErr( "Failed to allocate memory for a GeographicLib::CircularEngine" );
    }
}

//*****************************************************************************
CircularEngine::!CircularEngine()
{
    if ( m_pCircularEngine != NULL )
    {
        delete m_pCircularEngine;
        m_pCircularEngine = NULL;
    }
}

//*****************************************************************************
double CircularEngine::LongitudeSum(double coslon, double sinlon)
{
    return m_pCircularEngine->operator()( coslon, sinlon );
}

//*****************************************************************************
double CircularEngine::LongitudeSum(double lon)
{
    return m_pCircularEngine->operator()( lon );
}

//*****************************************************************************
double CircularEngine::LongitudeSum(double coslon, double sinlon,
                        [System::Runtime::InteropServices::Out] double% gradx,
                        [System::Runtime::InteropServices::Out] double% grady,
                        [System::Runtime::InteropServices::Out] double% gradz)
{
    double lgradx, lgrady, lgradz;
    double output = m_pCircularEngine->operator()( coslon, sinlon, lgradx, lgrady, lgradz );
    gradx = lgradx;
    grady = lgrady;
    gradz = lgradz;
    return output;
}

//*****************************************************************************
double CircularEngine::LongitudeSum(double lon,
                        [System::Runtime::InteropServices::Out] double% gradx,
                        [System::Runtime::InteropServices::Out] double% grady,
                        [System::Runtime::InteropServices::Out] double% gradz)
{
    double lgradx, lgrady, lgradz;
    double output = m_pCircularEngine->operator()( lon, lgradx, lgrady, lgradz );
    gradx = lgradx;
    grady = lgrady;
    gradz = lgradz;
    return output;
}
