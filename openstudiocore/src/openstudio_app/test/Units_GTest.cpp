/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "OpenStudioAppFixture.hpp"

#include "../../osversion/VersionTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceLoad.hpp"
#include "../../model/SpaceLoad_Impl.hpp"
#include "../../model/SpaceType.hpp"

#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/units/QuantityConverter.hpp"

#include <QDir>
#include <QFileInfo>

using namespace openstudio;

#define TEST_UNITS(a,b,c) \
  EXPECT_TRUE(convert(1.0, a, b)); \
  EXPECT_TRUE(convert(1.0, a, c));

TEST_F(OpenStudioAppFixture, Units)
{
  // test unit conversions in the app
  // grep OSQuantityEdit2
  TEST_UNITS("", "", "");
  TEST_UNITS("m", "m", "ft");
  TEST_UNITS("m^2", "m^2", "ft^2");
  TEST_UNITS("m", "m", "in");
  TEST_UNITS("deg", "deg", "deg");
  TEST_UNITS("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R");
  TEST_UNITS("W/m*K", "W/m*K", "Btu/hr*ft*R");
  TEST_UNITS("W/m*K^2", "W/m*K^2", "Btu*in/hr*ft^2*R^2");
  TEST_UNITS("W/m^2*K","W/m^2*K","Btu/ft^2*hr*R");
  TEST_UNITS("m^2*K/W","m^2*K/W","ft^2*hr*R/Btu");
  TEST_UNITS("W","W","W");
  TEST_UNITS("W/m^2","W/m^2","W/m^2");
  TEST_UNITS("W/m^2","W/m^2","W/ft^2");
  TEST_UNITS("W/person","W/person","W/person");
  TEST_UNITS("m^3/s*W","m^3/s*W","ft^3/min*W"); 
  TEST_UNITS("s/m","s/m","s/in");
  TEST_UNITS("kg/m^3","kg/m^3","lb/ft^3");
  TEST_UNITS("J/kg*K","J/kg*K","Btu/lb*R");
  TEST_UNITS("s/m","s/m","s/in");
  TEST_UNITS("people/m^2", "people/m^2", "people/ft^2");
  TEST_UNITS("m^2/person", "m^2/person", "ft^2/person");
  TEST_UNITS("m^3/s*W", "L/s*W", "ft^3*hr/min*Btu");
  TEST_UNITS("W","W","Btu/hr");
  TEST_UNITS("W/m^2","W/m^2","Btu/hr*ft^2"); 
  TEST_UNITS("W/person","W/person","Btu/hr*person"); 
  TEST_UNITS("m^3/s","m^3/s","gal/min"); 
  TEST_UNITS("g/m*s","g/m*s","g/m*s");
  TEST_UNITS("g/m*s*K","g/m*s*K","g/m*s*K");
  TEST_UNITS("J/kg*K","J/kg*K","Btu/lb*R");
  TEST_UNITS("J/kg*K^2","J/kg*K^2","Btu/lb*R^2"); 
  TEST_UNITS("C", "C", "F");
  TEST_UNITS("K", "K", "R");
  TEST_UNITS("1/m","1/m","1/ft");
  TEST_UNITS("1/m","1/m","1/in");
}
