/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/ShadingControl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/IlluminanceMap.hpp"
#include "../../model/IlluminanceMap_Impl.hpp"
#include "../../model/GlareSensor.hpp"
#include "../../model/GlareSensor_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::radiance;

std::string printPaths(const std::vector<path>& paths) {
  std::stringstream result;
  for (const auto& path : paths) {
    result << toString(path) << '\n';
  }
  return result.str();
}

std::string printLogMessages(const std::vector<LogMessage>& messages) {
  std::stringstream result;
  for (const auto& message : messages) {
    result << message.logMessage() << '\n';
  }
  return result.str();
}

TEST(Radiance, ForwardTranslator_SurfaceOnlyOnGround) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices{
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
    {0, 1, 0},
  };
  Surface surface(vertices, model);

  surface.setSpace(space);

  Point3dVectorVector polygons = ForwardTranslator::getPolygons(surface);
  ASSERT_EQ(1u, polygons.size());
  EXPECT_EQ(vertices.size(), polygons[0].size());

  double area = 0;
  for (const Point3dVector& polygon : polygons) {
    OptionalDouble d = getArea(polygon);
    ASSERT_TRUE(d);
    area += *d;
    for (const Point3d& vertex : polygon) {
      LOG_FREE(::Info, "Radiance", vertex);
    }
  }
  EXPECT_NEAR(1.0, area, 0.0001);
}

TEST(Radiance, ForwardTranslator_SurfaceOnlyOnXZ) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices{
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
    {0, 0, 1},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);

  Point3dVectorVector polygons = ForwardTranslator::getPolygons(surface);
  ASSERT_EQ(1u, polygons.size());
  EXPECT_EQ(vertices.size(), polygons[0].size());

  double area = 0;
  for (const Point3dVector& polygon : polygons) {
    OptionalDouble d = getArea(polygon);
    ASSERT_TRUE(d);
    area += *d;
    for (const Point3d& vertex : polygon) {
      LOG_FREE(::Info, "Radiance", vertex);
    }
  }
  EXPECT_NEAR(1.0, area, 0.0001);
}

TEST(Radiance, ForwardTranslator_SurfaceWithHoleOnGround) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices{
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
    {0, 1, 0},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices = {
    {0.25, 0.25, 0},
    {0.75, 0.25, 0},
    {0.75, 0.75, 0},
    {0.25, 0.75, 0},
  };
  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);

  Point3dVectorVector polygons = ForwardTranslator::getPolygons(surface);
  ASSERT_FALSE(polygons.empty());

  double area = 0;
  for (const Point3dVector& polygon : polygons) {
    OptionalDouble d = getArea(polygon);
    ASSERT_TRUE(d);
    area += *d;
    for (const Point3d& vertex : polygon) {
      LOG_FREE(::Info, "Radiance", vertex);
    }
  }
  EXPECT_NEAR(0.75, area, 0.0001);
}

TEST(Radiance, ForwardTranslator_SurfaceWithHoleOnXZ) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices{
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
    {0, 0, 1},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices = {
    {0.25, 0, 0.25},
    {0.75, 0, 0.25},
    {0.75, 0, 0.75},
    {0.25, 0, 0.75},
  };
  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);

  Point3dVectorVector polygons = ForwardTranslator::getPolygons(surface);
  ASSERT_FALSE(polygons.empty());

  double area = 0;
  for (const Point3dVector& polygon : polygons) {
    OptionalDouble d = getArea(polygon);
    ASSERT_TRUE(d);
    area += *d;
    for (const Point3d& vertex : polygon) {
      LOG_FREE(::Info, "Radiance", vertex);
    }
  }
  EXPECT_NEAR(0.75, area, 0.0001);
}

TEST(Radiance, ForwardTranslator_ExampleModel) {
  Model model = exampleModel();

  openstudio::path outpath = toPath("./ForwardTranslator_ExampleModel");
  openstudio::filesystem::remove_all(outpath);
  ASSERT_FALSE(openstudio::filesystem::exists(outpath));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_TRUE(openstudio::filesystem::exists(outpath));
  EXPECT_FALSE(outpaths.empty());
  EXPECT_TRUE(ft.errors().empty());
  EXPECT_TRUE(ft.warnings().empty());
}

TEST(Radiance, ForwardTranslator_ExampleModelWithShadingControl) {
  Model model = exampleModel();
  Construction shadedConstruction(model);

  model::ShadingControl shadingControl(shadedConstruction);
  for (auto& subSurface : model.getConcreteModelObjects<model::SubSurface>()) {
    if (istringEqual(subSurface.subSurfaceType(), "FixedWindow") || istringEqual(subSurface.subSurfaceType(), "OperableWindow")) {
      subSurface.setShadingControl(shadingControl);
    }
  }

  openstudio::path outpath = toPath("./ForwardTranslator_ExampleModelWithShadingControl");
  openstudio::filesystem::remove_all(outpath);
  ASSERT_FALSE(openstudio::filesystem::exists(outpath));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_TRUE(openstudio::filesystem::exists(outpath));
  EXPECT_FALSE(outpaths.empty()) << printPaths(outpaths);
  EXPECT_TRUE(ft.errors().empty()) << printLogMessages(ft.errors());
  EXPECT_TRUE(ft.warnings().empty()) << printLogMessages(ft.warnings());
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoIllumMaps) {
  Model model = exampleModel();

  for (IlluminanceMap illuminanceMap : model.getConcreteModelObjects<IlluminanceMap>()) {
    illuminanceMap.remove();
  }

  openstudio::path outpath = toPath("./ForwardTranslator_ExampleModel_NoIllumMaps");
  openstudio::filesystem::remove_all(outpath);
  ASSERT_FALSE(openstudio::filesystem::exists(outpath));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_FALSE(openstudio::filesystem::exists(outpath));
  EXPECT_TRUE(outpaths.empty());
  EXPECT_FALSE(ft.errors().empty());
  EXPECT_TRUE(ft.warnings().empty());
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoDaylightingControls) {
  Model model = exampleModel();

  for (DaylightingControl daylightingControl : model.getConcreteModelObjects<DaylightingControl>()) {
    daylightingControl.remove();
  }

  openstudio::path outpath = toPath("./ForwardTranslator_ExampleModel_NoDaylightingControls");
  openstudio::filesystem::remove_all(outpath);
  ASSERT_FALSE(openstudio::filesystem::exists(outpath));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_FALSE(openstudio::filesystem::exists(outpath));
  EXPECT_TRUE(outpaths.empty());
  EXPECT_FALSE(ft.errors().empty());
  EXPECT_TRUE(ft.warnings().empty());
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoGlareSensors) {
  Model model = exampleModel();

  for (GlareSensor glareSensor : model.getConcreteModelObjects<GlareSensor>()) {
    glareSensor.remove();
  }

  openstudio::path outpath = toPath("./ForwardTranslator_ExampleModel_NoGlareSensors");
  openstudio::filesystem::remove_all(outpath);
  ASSERT_FALSE(openstudio::filesystem::exists(outpath));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_TRUE(openstudio::filesystem::exists(outpath));
  EXPECT_FALSE(outpaths.empty());
  EXPECT_TRUE(ft.errors().empty());
  EXPECT_FALSE(ft.warnings().empty());
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoThermalZoneLinks) {
  Model model = exampleModel();

  for (ThermalZone thermalZone : model.getConcreteModelObjects<ThermalZone>()) {
    thermalZone.resetSecondaryDaylightingControl();
    thermalZone.resetPrimaryDaylightingControl();
    thermalZone.resetIlluminanceMap();
  }

  openstudio::path outpath = toPath("./ForwardTranslator_ExampleModel_NoThermalZoneLinks");
  openstudio::filesystem::remove_all(outpath);
  ASSERT_FALSE(openstudio::filesystem::exists(outpath));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_FALSE(openstudio::filesystem::exists(outpath));
  EXPECT_TRUE(outpaths.empty());
  EXPECT_FALSE(ft.errors().empty());
  EXPECT_FALSE(ft.warnings().empty());
}

TEST(Radiance, ForwardTranslator_formatString) {
  EXPECT_EQ("44", formatString(44.12345, 0));
  EXPECT_EQ("44.1", formatString(44.12345, 1));
  EXPECT_EQ("44.12", formatString(44.12345, 2));

  EXPECT_EQ("45", formatString(44.6789, 0));
  EXPECT_EQ("44.7", formatString(44.6789, 1));
  EXPECT_EQ("44.68", formatString(44.6789, 2));

  EXPECT_EQ("0", formatString(0.4412345, 0));
  EXPECT_EQ("0.4", formatString(0.4412345, 1));
  EXPECT_EQ("0.44", formatString(0.4412345, 2));
}
