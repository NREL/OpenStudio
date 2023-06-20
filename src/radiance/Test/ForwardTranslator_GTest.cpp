/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
