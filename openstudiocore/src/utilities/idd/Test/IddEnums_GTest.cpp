/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <gtest/gtest.h>
#include "IddFixture.hpp"

#include "../IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>

TEST_F(IddFixture,IddEnums_IddFileType)
{
  openstudio::IddFileType epIdd(openstudio::IddFileType::EnergyPlus);
  openstudio::IddFileType osIdd(openstudio::IddFileType::OpenStudio);
  EXPECT_TRUE( epIdd != osIdd );

  epIdd = openstudio::IddFileType("EnergyPlus");
  EXPECT_TRUE( epIdd != osIdd );

  EXPECT_EQ("OpenStudio",osIdd.valueName());
  EXPECT_EQ("OpenStudio",osIdd.valueDescription());
}

TEST_F(IddFixture,IddEnums_IddObjectType)
{
  openstudio::IddObjectType iddType(openstudio::IddObjectType::Catchall);
  iddType = openstudio::IddObjectType::UserCustom;
  iddType = openstudio::IddObjectType::CommentOnly;

  iddType = openstudio::IddObjectType("BuildingSurface:Detailed");
  EXPECT_EQ("BuildingSurface_Detailed",iddType.valueName());
  EXPECT_EQ("BuildingSurface:Detailed",iddType.valueDescription());

  iddType = openstudio::IddObjectType("zone");
  EXPECT_EQ("Zone",iddType.valueDescription());
  iddType = openstudio::IddObjectType("zoNe");
  EXPECT_EQ("Zone",iddType.valueDescription());

  iddType = openstudio::IddObjectType("simulation data");
  EXPECT_EQ("SimulationData",iddType.valueName());
  EXPECT_EQ("Simulation Data",iddType.valueDescription());
}

TEST_F(IddFixture,IddEnums_FieldEnums) {
  openstudio::ZoneFields zoneField(openstudio::ZoneFields::Name);
  EXPECT_NO_THROW(zoneField = openstudio::ZoneFields("x origin"));
  EXPECT_EQ("XOrigin",zoneField.valueName());
  EXPECT_EQ("X Origin",zoneField.valueDescription());

  openstudio::BuildingSurface_DetailedExtensibleFields bldgSurfExtField(
      openstudio::BuildingSurface_DetailedExtensibleFields::VertexYcoordinate);
  EXPECT_EQ("VertexYcoordinate",bldgSurfExtField.valueName());
  EXPECT_EQ("Vertex Y-coordinate",bldgSurfExtField.valueDescription());
}
