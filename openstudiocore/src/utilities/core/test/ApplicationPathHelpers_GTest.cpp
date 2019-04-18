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

#include "../ApplicationPathHelpers.hpp"
#include "../PathHelpers.hpp"
#include <stdlib.h>

using namespace openstudio;

TEST(ApplicationPathHelpers, Strings)
{
  path applicationPath = getApplicationPath();
  path applicationDir = getApplicationDirectory();
  path openstudioModule = getOpenStudioModule();
  path openstudioModuleDirectory = getOpenStudioModuleDirectory();
  path openstudioCLI = getOpenStudioCLI();
  path energyplusDirectory = getEnergyPlusDirectory();
  path energyplusExecutable = getEnergyPlusExecutable();

  EXPECT_TRUE(exists(applicationPath));
  EXPECT_TRUE(exists(applicationDir));
  EXPECT_TRUE(exists(openstudioModule));
  EXPECT_TRUE(exists(openstudioModuleDirectory));
  EXPECT_TRUE(exists(openstudioCLI));
  EXPECT_TRUE(exists(energyplusDirectory));
  EXPECT_TRUE(exists(energyplusExecutable));
}



// This is really in PathHelpers but used by ApplicationPathHelpers
TEST(ApplicationPathHelpers, findInSystemPath) {

  openstudio::path dummy_dir = getApplicationBuildDirectory() / toPath("Testing/Temporary");
  openstudio::filesystem::create_directories(dummy_dir);
  openstudio::path dummy_file_path = dummy_dir / "dummy_file";

  boost::filesystem::ofstream outfile(dummy_file_path);
  outfile.close();

  // This should fail
  openstudio::path absolute_path_to_dummy = findInSystemPath(dummy_file_path.filename());
  EXPECT_FALSE(exists(absolute_path_to_dummy));

  // Append the containing dir to the PATH
  auto current_path = std::getenv("PATH");
  std::string new_path = toString(dummy_dir) + ":" + current_path;
  setenv("PATH", new_path.c_str(), 1);
  setenv("PathWhenSymlinkInPathUnixOnly_Setup", "true", 1);

  // Locate the file with only its name
  absolute_path_to_dummy = findInSystemPath(dummy_file_path.filename());
  EXPECT_TRUE(exists(absolute_path_to_dummy));
  EXPECT_EQ(dummy_file_path, absolute_path_to_dummy);

  // Put it back
  setenv("PATH", current_path, 1);
  boost::filesystem::remove(dummy_file_path);

}
