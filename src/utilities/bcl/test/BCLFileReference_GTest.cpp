/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "BCLFixture.hpp"

#include "../BCLFileReference.hpp"
#include "../../core/ApplicationPathHelpers.hpp"

using namespace openstudio;

TEST_F(BCLFixture, BCLFileReference) {

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path relativePath = toPath("BCLFileReference.txt");
  openstudio::path absolutePath = testDir / relativePath;
  if (exists(absolutePath)) {
    remove(absolutePath);
  }
  ASSERT_FALSE(exists(absolutePath));

  BCLFileReference ref(testDir, relativePath, true);
  EXPECT_FALSE(ref.checkForUpdate());

  EXPECT_EQ("BCLFileReference.txt", ref.fileName());
  EXPECT_EQ("txt", ref.fileType());

  openstudio::filesystem::ofstream file(absolutePath);
  ASSERT_TRUE(file.is_open());
  file << "Hi";
  file.close();

  EXPECT_TRUE(ref.checkForUpdate());

  EXPECT_FALSE(ref.checkForUpdate());

  file.open(absolutePath);
  ASSERT_TRUE(file.is_open());
  file << "Bye";
  file.close();

  EXPECT_TRUE(ref.checkForUpdate());

  EXPECT_FALSE(ref.checkForUpdate());

  ASSERT_TRUE(exists(absolutePath));
  remove(absolutePath);
  ASSERT_FALSE(exists(absolutePath));

  EXPECT_TRUE(ref.checkForUpdate());

  EXPECT_FALSE(ref.checkForUpdate());
}
