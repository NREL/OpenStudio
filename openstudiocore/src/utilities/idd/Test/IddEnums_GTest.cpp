/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
