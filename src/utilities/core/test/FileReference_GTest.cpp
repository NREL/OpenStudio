/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../FileReference.hpp"

#include <resources.hxx>

using openstudio::toPath;
using openstudio::FileReference;
using openstudio::FileReferenceType;

TEST(FileReference, Constructor) {
  FileReference fileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/dummyname.osm"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::OSM);

  fileReference = FileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/eplusout.sql"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::SQL);

  fileReference = FileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.epw"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::EPW);

  fileReference = FileReference(resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf"));
  EXPECT_TRUE(fileReference.fileType() == FileReferenceType::IDF);
}
