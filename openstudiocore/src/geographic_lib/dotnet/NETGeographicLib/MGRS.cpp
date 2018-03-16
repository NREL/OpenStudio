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
#include "GeographicLib/MGRS.hpp"
#include "MGRS.h"
#include "NETGeographicLib.h"

using namespace NETGeographicLib;

//*****************************************************************************
void MGRS::Forward(int zone, bool northp, double x, double y,
                int prec,
                [System::Runtime::InteropServices::Out] System::String^% mgrs)
{
    try
    {
        std::string lmgrs;
        GeographicLib::MGRS::Forward( zone, northp, x, y, prec, lmgrs );
        mgrs = gcnew System::String( lmgrs.c_str() );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void MGRS::Forward(int zone, bool northp, double x, double y, double lat,
                int prec, System::String^% mgrs)
{
    try
    {
        std::string lmgrs;
        GeographicLib::MGRS::Forward( zone, northp, x, y, lat, prec, lmgrs );
        mgrs = gcnew System::String( lmgrs.c_str() );
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
void MGRS::Reverse(System::String^ mgrs,
                [System::Runtime::InteropServices::Out] int% zone,
                [System::Runtime::InteropServices::Out] bool% northp,
                [System::Runtime::InteropServices::Out] double% x,
                [System::Runtime::InteropServices::Out] double% y,
                [System::Runtime::InteropServices::Out] int% prec,
                bool centerp )
{
    try
    {
        double lx, ly;
        int lzone, lprec;
        bool lnorthp;
        GeographicLib::MGRS::Reverse( StringConvert::ManagedToUnmanaged( mgrs ),
            lzone, lnorthp, lx, ly, lprec, centerp );
        x = lx;
        y = ly;
        zone = lzone;
        prec = lprec;
        northp = lnorthp;
    }
    catch ( const std::exception& err )
    {
        throw gcnew GeographicErr( err.what() );
    }
}

//*****************************************************************************
double MGRS::MajorRadius() { return GeographicLib::UTMUPS::MajorRadius(); }

//*****************************************************************************
double MGRS::Flattening() { return GeographicLib::UTMUPS::Flattening(); }
