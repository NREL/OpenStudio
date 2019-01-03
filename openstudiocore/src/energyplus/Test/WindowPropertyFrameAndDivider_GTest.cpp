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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::energyplus;
using namespace openstudio::model;

TEST_F(EnergyPlusFixture, WindowPropertyFrameAndDivider)
{

  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 10));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(10, 0, 0));
  vertices.push_back(Point3d(10, 0, 10));

  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices.clear();
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));

  SubSurface subSurface1(vertices, model);
  subSurface1.setName("No Offset");
  subSurface1.setSurface(surface);

  SubSurface subSurface2(vertices, model);
  subSurface2.setName("Offset");
  subSurface2.setSurface(surface);

  Vector3d normal = subSurface1.outwardNormal();
  EXPECT_DOUBLE_EQ(0.0, normal.x());
  EXPECT_DOUBLE_EQ(-1.0, normal.y());
  EXPECT_DOUBLE_EQ(0.0, normal.z());

  WindowPropertyFrameAndDivider frameAndDivider(model);
  frameAndDivider.setOutsideRevealDepth(1.0);

  EXPECT_FALSE(subSurface2.allowWindowPropertyFrameAndDivider());
  EXPECT_TRUE(subSurface2.setSubSurfaceType("GlassDoor"));
  EXPECT_TRUE(subSurface2.allowWindowPropertyFrameAndDivider());

  EXPECT_TRUE(subSurface2.setWindowPropertyFrameAndDivider(frameAndDivider));
  ASSERT_TRUE(subSurface2.windowPropertyFrameAndDivider());

  ForwardTranslator forwardTranslator;
  OptionalWorkspace outWorkspace = forwardTranslator.translateModel(model);
  ASSERT_TRUE(outWorkspace);

  ReverseTranslator reverseTranslator;
  OptionalModel outModel = reverseTranslator.translateWorkspace(*outWorkspace);
  ASSERT_TRUE(outModel);

  EXPECT_EQ(1u, outModel->getConcreteModelObjects<WindowPropertyFrameAndDivider>().size());

  boost::optional<SubSurface> testSubSurface = outModel->getModelObjectByName<SubSurface>("Offset");
  ASSERT_TRUE(testSubSurface);
  EXPECT_TRUE(testSubSurface->windowPropertyFrameAndDivider());
  vertices = testSubSurface->vertices();
  ASSERT_EQ(4u, vertices.size());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].x());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].y());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].z());

  testSubSurface = outModel->getModelObjectByName<SubSurface>("No Offset");
  ASSERT_TRUE(testSubSurface);
  EXPECT_FALSE(testSubSurface->windowPropertyFrameAndDivider());
  vertices = testSubSurface->vertices();
  ASSERT_EQ(4u, vertices.size());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].x());
  EXPECT_DOUBLE_EQ(0.0, vertices[0].y());
  EXPECT_DOUBLE_EQ(1.0, vertices[0].z());

}

