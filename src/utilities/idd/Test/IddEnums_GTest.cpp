/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IddFixture.hpp"

#include "../IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>

TEST_F(IddFixture, IddEnums_IddFileType) {
  openstudio::IddFileType epIdd(openstudio::IddFileType::EnergyPlus);
  openstudio::IddFileType osIdd(openstudio::IddFileType::OpenStudio);
  EXPECT_TRUE(epIdd != osIdd);

  epIdd = openstudio::IddFileType("EnergyPlus");
  EXPECT_TRUE(epIdd != osIdd);

  EXPECT_EQ("OpenStudio", osIdd.valueName());
  EXPECT_EQ("OpenStudio", osIdd.valueDescription());
}

TEST_F(IddFixture, IddEnums_IddObjectType) {
  openstudio::IddObjectType iddType(openstudio::IddObjectType::Catchall);
  iddType = openstudio::IddObjectType::UserCustom;
  // cppcheck-suppress redundantAssignment
  iddType = openstudio::IddObjectType::CommentOnly;

  // cppcheck-suppress redundantAssignment
  iddType = openstudio::IddObjectType("BuildingSurface:Detailed");
  EXPECT_EQ("BuildingSurface_Detailed", iddType.valueName());
  EXPECT_EQ("BuildingSurface:Detailed", iddType.valueDescription());

  iddType = openstudio::IddObjectType("zone");
  EXPECT_EQ("Zone", iddType.valueDescription());
  iddType = openstudio::IddObjectType("zoNe");
  EXPECT_EQ("Zone", iddType.valueDescription());

  iddType = openstudio::IddObjectType("simulation data");
  EXPECT_EQ("SimulationData", iddType.valueName());
  EXPECT_EQ("Simulation Data", iddType.valueDescription());
}

TEST_F(IddFixture, IddEnums_FieldEnums) {
  openstudio::ZoneFields zoneField(openstudio::ZoneFields::Name);
  EXPECT_NO_THROW(zoneField = openstudio::ZoneFields("x origin"));
  EXPECT_EQ("XOrigin", zoneField.valueName());
  EXPECT_EQ("X Origin", zoneField.valueDescription());

  openstudio::BuildingSurface_DetailedExtensibleFields bldgSurfExtField(openstudio::BuildingSurface_DetailedExtensibleFields::VertexYcoordinate);
  EXPECT_EQ("VertexYcoordinate", bldgSurfExtField.valueName());
  EXPECT_EQ("Vertex Y-coordinate", bldgSurfExtField.valueDescription());
}
