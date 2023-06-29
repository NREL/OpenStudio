/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IdfFixture.hpp"
#include "../ImfFile.hpp"
#include "../../core/Path.hpp"
#include "../../core/Optional.hpp"
#include "../../core/Logger.hpp"

#include <resources.hxx>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture, ImfFile) {
  EXPECT_TRUE(imfFile.numSections() > 0);
  StringVector sectionNames = imfFile.sectionNames();
  unsigned i = 1;
  LOG(Info, "Sections in the IdfFixture ImfFile:");
  for (const std::string& name : sectionNames) {
    LOG(Info, "  " << i << ": " << name << ", " << imfFile.section(name).size() << " objects");
    ++i;
  }
}
