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
#include "GeometryFixture.hpp"

#include "../FloorplanJS.hpp"
#include "../ThreeJS.hpp"

#include <resources.hxx>

using namespace openstudio;

TEST_F(GeometryFixture, FloorplanJS)
{
  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan.json");
  ASSERT_TRUE(exists(p));

  std::ifstream ifs(toSystemFilename(p));
  EXPECT_TRUE(ifs.is_open());

  std::istreambuf_iterator<char> eos;
  std::string contents(std::istreambuf_iterator<char>(ifs), eos);
  ifs.close();
  EXPECT_FALSE(contents.empty());

  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(contents);
  ASSERT_TRUE(floorplan);

  floorplan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorplan);

  ThreeScene scene = floorplan->toThreeScene(true);
  std::string json = scene.toJSON(false);
  openstudio::path out = resourcesPath() / toPath("utilities/Geometry/floorplan_threejs_triangulated.json");
  {
    openstudio::filesystem::ofstream file(out);
    ASSERT_TRUE(file.is_open());
    file << json;
    file.close();
  }

  scene = floorplan->toThreeScene(false);
  json = scene.toJSON(false);
  out = resourcesPath() / toPath("utilities/Geometry/floorplan_threejs_nontriangulated.json");
  {
    openstudio::filesystem::ofstream file(out);
    ASSERT_TRUE(file.is_open());
    file << json;
    file.close();
  }

}
