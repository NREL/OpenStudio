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

using namespace System;
using namespace NETGeographicLib;

int main(array<System::String ^> ^/*args*/)
{
    try {
        EllipticFunction^ ell = gcnew EllipticFunction(0.1, 1.0);  // parameter m = 0.1
        // See Abramowitz and Stegun, table 17.1
        Console::WriteLine( String::Format( "{0} {1}", ell->K(), ell->E()));
        double phi = 20 * Math::Acos(-1.0) / 180.0;;
        // See Abramowitz and Stegun, table 17.6 with
        // alpha = asin(sqrt(m)) = 18.43 deg and phi = 20 deg
        Console::WriteLine( String::Format("{0} {1}", ell->E(phi),
                ell->E(Math::Sin(phi), Math::Cos(phi),
                    Math::Sqrt(1 - ell->k2 * Math::Sin(phi) * Math::Sin(phi))) ) );
        // See Carlson 1995, Sec 3.
        Console::WriteLine(String::Format("RF(1,2,0)      = {0}", EllipticFunction::RF(1,2)));
        Console::WriteLine(String::Format("RF(2,3,4)      = {0}", EllipticFunction::RF(2,3,4)));
        Console::WriteLine(String::Format("RC(0,1/4)      = {0}", EllipticFunction::RC(0,0.25)));
        Console::WriteLine(String::Format("RC(9/4,2)      = {0}", EllipticFunction::RC(2.25,2)));
        Console::WriteLine(String::Format("RC(1/4,-2)     = {0}", EllipticFunction::RC(0.25,-2)));
        Console::WriteLine(String::Format("RJ(0,1,2,3)    = {0}", EllipticFunction::RJ(0,1,2,3)));
        Console::WriteLine(String::Format("RJ(2,3,4,5)    = {0}", EllipticFunction::RJ(2,3,4,5)));
        Console::WriteLine(String::Format("RD(0,2,1)      = {0}", EllipticFunction::RD(0,2,1)));
        Console::WriteLine(String::Format("RD(2,3,4)      = {0}", EllipticFunction::RD(2,3,4)));
        Console::WriteLine(String::Format("RG(0,16,16)    = {0}", EllipticFunction::RG(16,16)));
        Console::WriteLine(String::Format("RG(2,3,4)      = {0}", EllipticFunction::RG(2,3,4)));
        Console::WriteLine(String::Format("RG(0,0.0796,4) = {0}", EllipticFunction::RG(0.0796, 4)));
    }
    catch (GeographicErr^ e) {
        Console::WriteLine( String::Format( "Caught exception: {0}", e->Message ) );
        return -1;
    }
    return 0;
}
