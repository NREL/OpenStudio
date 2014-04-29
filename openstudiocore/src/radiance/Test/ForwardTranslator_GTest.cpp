/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include <radiance/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Space.hpp>
#include <model/Surface.hpp>
#include <model/SubSurface.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/DaylightingControl.hpp>
#include <model/DaylightingControl_Impl.hpp>
#include <model/IlluminanceMap.hpp>
#include <model/IlluminanceMap_Impl.hpp>
#include <model/GlareSensor.hpp>
#include <model/GlareSensor_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/geometry/Point3d.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::radiance;

TEST(Radiance, ForwardTranslator_SurfaceOnlyOnGround)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices;
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,1,0));
  vertices.push_back(Point3d(0,1,0));
  Surface surface(vertices, model);

  surface.setSpace(space);

  Point3dVector polygon = ForwardTranslator::getPolygon(surface);
  EXPECT_EQ(vertices.size(), polygon.size());

  BOOST_FOREACH(const Point3d& vertex, polygon){
    LOG_FREE(::Info, "Radiance", vertex);
  }

}

TEST(Radiance, ForwardTranslator_SurfaceOnlyOnXZ)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices;
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(0,0,1));
  Surface surface(vertices, model);
  surface.setSpace(space);

  Point3dVector polygon = ForwardTranslator::getPolygon(surface);
  EXPECT_EQ(vertices.size(), polygon.size());

  BOOST_FOREACH(const Point3d& vertex, polygon){
    LOG_FREE(::Info, "Radiance", vertex);
  }

}

TEST(Radiance, ForwardTranslator_SurfaceWithHoleOnGround)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices;
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,1,0));
  vertices.push_back(Point3d(0,1,0));
  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices.clear();
  vertices.push_back(Point3d(0.25,0.25,0));
  vertices.push_back(Point3d(0.75,0.25,0));
  vertices.push_back(Point3d(0.75,0.75,0));
  vertices.push_back(Point3d(0.25,0.75,0));
  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);

  Point3dVector polygon = ForwardTranslator::getPolygon(surface);
  EXPECT_TRUE(vertices.size() < polygon.size());

  BOOST_FOREACH(const Point3d& vertex, polygon){
    LOG_FREE(::Info, "Radiance", vertex);
  }

}

TEST(Radiance, ForwardTranslator_SurfaceWithHoleOnXZ)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Space space(model);

  Point3dVector vertices;
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(0,0,1));
  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices.clear();
  vertices.push_back(Point3d(0.25,0,0.25));
  vertices.push_back(Point3d(0.75,0,0.25));
  vertices.push_back(Point3d(0.75,0,0.75));
  vertices.push_back(Point3d(0.25,0,0.75));
  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);

  Point3dVector polygon = ForwardTranslator::getPolygon(surface);
  EXPECT_TRUE(vertices.size() < polygon.size());

  BOOST_FOREACH(const Point3d& vertex, polygon){
    LOG_FREE(::Info, "Radiance", vertex);
  }

}


TEST(Radiance, ForwardTranslator_ExampleModel)
{
  Model model = exampleModel();

  openstudio::path outpath = toPath(toString(openstudio::createUUID()));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_FALSE(outpaths.empty());
  EXPECT_TRUE(ft.errors().empty());
  EXPECT_TRUE(ft.warnings().empty());

  boost::filesystem::remove_all(outpath);
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoIllumMaps)
{
  Model model = exampleModel();

  BOOST_FOREACH(IlluminanceMap illuminanceMap, model.getModelObjects<IlluminanceMap>()){
    illuminanceMap.remove();
  } 
  
  openstudio::path outpath = toPath(toString(openstudio::createUUID()));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_TRUE(outpaths.empty());
  EXPECT_FALSE(ft.errors().empty());
  EXPECT_TRUE(ft.warnings().empty());

  boost::filesystem::remove_all(outpath);
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoDaylightingControls)
{
  Model model = exampleModel();

  BOOST_FOREACH(DaylightingControl daylightingControl, model.getModelObjects<DaylightingControl>()){
    daylightingControl.remove();
  } 
  
  openstudio::path outpath = toPath(toString(openstudio::createUUID()));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_TRUE(outpaths.empty());
  EXPECT_FALSE(ft.errors().empty());
  EXPECT_TRUE(ft.warnings().empty());

  boost::filesystem::remove_all(outpath);
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoGlareSensors)
{
  Model model = exampleModel();

  BOOST_FOREACH(GlareSensor glareSensor, model.getModelObjects<GlareSensor>()){
    glareSensor.remove();
  } 
  
  openstudio::path outpath = toPath(toString(openstudio::createUUID()));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_FALSE(outpaths.empty());
  EXPECT_TRUE(ft.errors().empty());
  EXPECT_FALSE(ft.warnings().empty());

  boost::filesystem::remove_all(outpath);
}

TEST(Radiance, ForwardTranslator_ExampleModel_NoThermalZoneLinks)
{
  Model model = exampleModel();

  BOOST_FOREACH(ThermalZone thermalZone, model.getModelObjects<ThermalZone>()){
    thermalZone.resetSecondaryDaylightingControl();
    thermalZone.resetPrimaryDaylightingControl();
    thermalZone.resetIlluminanceMap();
  } 
  
  openstudio::path outpath = toPath(toString(openstudio::createUUID()));

  ForwardTranslator ft;
  std::vector<path> outpaths = ft.translateModel(outpath, model);
  EXPECT_TRUE(outpaths.empty());
  EXPECT_FALSE(ft.errors().empty());
  EXPECT_FALSE(ft.warnings().empty());

  boost::filesystem::remove_all(outpath);
}