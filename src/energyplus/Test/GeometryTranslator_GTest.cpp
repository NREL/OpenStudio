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

#include "../GeometryTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/IlluminanceMap.hpp"
#include "../../model/Surface.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Wall_Exterior_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::energyplus;

Workspace testWorkspace(const CoordinateSystem& system, const CoordinateSystem& daylightSystem, const CoordinateSystem& rectSystem){

  std::stringstream ss;

  ss << "SimulationControl, \n\
    No, !- Do Zone Sizing Calculation \n\
    No, !- Do System Sizing Calculation \n\
    No, !- Do Plant Sizing Calculation \n\
    No, !- Run Simulation for Sizing Periods \n\
    Yes; !- Run Simulation for Weather File Run Periods \n\
    \n\
  RunPeriod, \n\
    ,                        !- Name \n\
    1,                       !- Begin Month \n\
    1,                       !- Begin Day of Month \n\
    ,                        !- Begin Year \n\
    1,                       !- End Month \n\
    1,                       !- End Day of Month \n\
    ,                        !- End Year \n\
    Tuesday,                 !- Day of Week for Start Day \n\
    Yes,                     !- Use Weather File Holidays and Special Days \n\
    Yes,                     !- Use Weather File Daylight Saving Period \n\
    No,                      !- Apply Weekend Holiday Rule \n\
    Yes,                     !- Use Weather File Rain Indicators \n\
    Yes;                     !- Use Weather File Snow Indicators \n\
    \n\
  Building, \n\
    Building,                !- Name \n\
    30.,                     !- North Axis {deg} \n\
    City,                    !- Terrain \n\
    0.04,                    !- Loads Convergence Tolerance Value \n\
    0.4,                     !- Temperature Convergence Tolerance Value {deltaC} \n\
    FullExterior,            !- Solar Distribution \n\
    25;                      !- Maximum Number of Warmup Days \n\
    \n\
  GlobalGeometryRules, \n\
    UpperLeftCorner,         !- Starting Vertex Position \n\
    CounterClockWise,        !- Vertex Entry Direction \n";

  ss << "    " << system.valueName() << ", !- Coordinate System\n"
     << "    " << daylightSystem.valueName() << ", !- Daylighting Coordinate System\n"
     << "    " << rectSystem.valueName() << "; !- Rectangular Surface Coordinate System\n\n";

  ss << "Zone, \n\
    Zone,                    !- Name \n\
    30.,                     !- Direction of Relative North {deg} \n\
    10.,                     !- X Origin {m} \n\
    5.,                      !- Y Origin {m} \n\
    1.;                      !- Z Origin {m} \n\
    \n\
  Construction, \n\
    Wall, \n\
    Material; \n\
    \n\
  Material, \n\
    Material,                !- Name \n\
    MediumRough,             !- Roughness \n\
    0.1016000,               !- Thickness {m} \n\
    1.310000,                !- Conductivity {W/m-K} \n\
    2243.000,                !- Density {kg/m3} \n\
    837.0000,                !- Specific Heat {J/kg-K} \n\
    0.9000000,               !- Thermal Absorptance \n\
    0.6500000,               !- Solar Absorptance \n\
    0.6500000;               !- Visible Absorptance \n\
    \n\
  BuildingSurface:Detailed, \n\
    Wall,                    !- Name \n\
    Wall,                    !- Surface Type \n\
    Wall,                    !- Construction Name \n\
    Zone,                    !- Zone Name \n\
    Outdoors,                !- Outside Boundary Condition \n\
    ,                        !- Outside Boundary Condition Object \n\
    SunExposed,              !- Sun Exposure \n\
    WindExposed,             !- Wind Exposure \n\
    0.50000,                 !- View Factor to Ground \n\
    4,                       !- Number of Vertices \n\
    0.0,0.0,1,  !- X,Y,Z ==> Vertex 1 {m} \n\
    0.0,0.0,0.0,  !- X,Y,Z ==> Vertex 2 {m} \n\
    10.0,0.0,0.0,  !- X,Y,Z ==> Vertex 3 {m} \n\
    10.0,0.0,1.;  !- X,Y,Z ==> Vertex 4 {m} \n\
    \n\
  Daylighting:Controls, \n\
    Daylighting Controls,    !- Name \n\
    Zone,                    !- Zone Name \n\
    SplitFlux,               !- Daylighting Method \n\
    ,                        !- Availability Schedule Name \n\
    Continuous,              !- Lighting Control Type \n\
    0.3,                     !- Minimum Input Power Fraction for Continuous or ContinuousOff Dimming Control \n\
    0.2,                     !- Minimum Light Output Fraction for Continuous or ContinuousOff Dimming Control \n\
    ,                        !- Number of Stepped Control Steps \n\
    1.0,                     !- Probability Lighting will be Reset When Needed in Manual Stepped Control \n\
    Daylighting RefPt1,      !- Glare Calculation Daylighting Reference Point Name \n\
    0.0,                     !- Glare Calculation Azimuth Angle of View Direction Clockwise from Zone y-Axis {deg} \n\
    20.0,                    !- Maximum Allowable Discomfort Glare Index \n\
    ,                        !- DElight Gridding Resolution {m2} \n\
    Daylighting RefPt1,      !- Daylighting Reference Point 1 Name \n\
    1.0,                     !- Fraction of Zone Controlled by Reference Point 1 \n\
    550,                     !- Illuminance Setpoint at Reference Point 1 {lux} \n\
    Daylighting RefPt2,      !- Daylighting Reference Point 2 Name \n\
    0.0,                     !- Fraction of Zone Controlled by Reference Point 2 \n\
    0;                       !- Illuminance Setpoint at Reference Point 2 {lux} \n\
    \n\
  Daylighting:ReferencePoint, \n\
    Daylighting RefPt1,      !- Name \n\
    Zone,                    !- Zone Name \n\
    0.0,                     !- X-Coordinate of Reference Point {m} \n\
    0.0,                     !- Y-Coordinate of Reference Point {m} \n\
    0.8;                     !- Z-Coordinate of Reference Point {m} \n\
    \n\
  Daylighting:ReferencePoint, \n\
    Daylighting RefPt2,      !- Name \n\
    Zone,                    !- Zone Name \n\
    1.0,                     !- X-Coordinate of Reference Point {m} \n\
    1.0,                     !- Y-Coordinate of Reference Point {m} \n\
    0.8;                     !- Z-Coordinate of Reference Point {m} \n\
  Output:IlluminanceMap, \n\
    Daylight Map,            !- Name \n\
    Zone,                    !- Zone Name \n\
    0.8,                     !- Z height {m} \n\
    0.0,                     !- X Minimum Coordinate {m} \n\
    10.0,                    !- X Maximum Coordinate {m} \n\
    5,                       !- Number of X Grid Points \n\
    0.0,                     !- Y Minimum Coordinate {m} \n\
    10.0,                    !- Y Maximum Coordinate {m} \n\
    5;                       !- Number of Y Grid Points \n\
    \n\
  Output:Surfaces:Drawing,DXF;\n";

  OptionalIdfFile oIdfFile = IdfFile::load(ss,IddFileType(IddFileType::EnergyPlus));
  return Workspace(*oIdfFile);
}

void pointEqual(const Point3d& a, const Point3d& b, double tol = 1.0e-6)
{
  EXPECT_NEAR(a.x(), b.x(), tol);
  EXPECT_NEAR(a.y(), b.y(), tol);
  EXPECT_NEAR(a.z(), b.z(), tol);
}

TEST_F(EnergyPlusFixture,GeometryTranslator_RelativeTest)
{
   /* SketchUp code for this test:

    model = Sketchup.active_model
    entities = model.entities
    entities.clear!

    building = entities.add_group
    zone = building.entities.add_group
    face = zone.entities.add_face([0.m,0.m,1.m], [0.m,0.m,0.m], [10.m,0.m,0.m], [10.m,0.m,1.m])

    building_rotation = Geom::Transformation::rotation([0,0,0], [0,0,1], -30.degrees)
    building.transform! building_rotation

    zone_rotation = Geom::Transformation::rotation([0,0,0], [0,0,1], -30.degrees)
    zone_translation = Geom::Transformation::translation([10.m,5.m,1.m])
    zone.transform! zone_translation*zone_rotation

    face.vertices.each do |vertex|
      puts "#{vertex.position[0].to_m}, #{vertex.position[1].to_m}, #{vertex.position[2].to_m}"
    end

    face.vertices.each do |vertex|
      abs_position = building_rotation*zone_translation*zone_rotation*vertex.position
      puts "#{abs_position[0].to_m}, #{abs_position[1].to_m}, #{abs_position[2].to_m}"
    end
  */

  Workspace workspace = testWorkspace(CoordinateSystem::Relative, CoordinateSystem::Relative, CoordinateSystem::Relative);

  // now create a model and check everthing is as expected
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(workspace);

  Building building = model.getUniqueModelObject<Building>();
  Transformation buildingTransformation = building.transformation();

  SpaceVector spaces = building.spaces();
  ASSERT_EQ(static_cast<unsigned>(1), spaces.size());
  Transformation spaceTransformation = spaces[0].transformation();

  SurfaceVector surfaces = spaces[0].surfaces();
  ASSERT_EQ(static_cast<unsigned>(1), surfaces.size());

  // get the vertices in relative coordinates
  Point3dVector vertices = surfaces[0].vertices();
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(0,0,1), vertices[0]);
  pointEqual(Point3d(0,0,0), vertices[1]);
  pointEqual(Point3d(10,0,0), vertices[2]);
  pointEqual(Point3d(10,0,1), vertices[3]);

  // convert the vertices to absolute coordinates
  vertices = buildingTransformation*spaceTransformation*vertices;
  pointEqual(Point3d(11.1602540378444, -0.66987298107780, 2.0), vertices[0]);
  pointEqual(Point3d(11.1602540378444, -0.66987298107780, 1.0), vertices[1]);
  pointEqual(Point3d(16.1602540378444, -9.33012701892219, 1.0), vertices[2]);
  pointEqual(Point3d(16.1602540378444, -9.33012701892219, 2.0), vertices[3]);

  // daylighting controls
  DaylightingControlVector daylightingControls = spaces[0].daylightingControls();
  ASSERT_EQ(2u, daylightingControls.size());

  // get the points in relative coordinates
  std::vector<Point3d> referencePoints;
  referencePoints.push_back(daylightingControls[0].position());
  referencePoints.push_back(daylightingControls[1].position());
  ASSERT_EQ(static_cast<unsigned>(2), referencePoints.size());
  //pointEqual(Point3d(0, 0, 0.8), referencePoints[0]);
  //pointEqual(Point3d(1, 1, 0.8), referencePoints[1]);

  // convert the points to absolute coordinates
  referencePoints = buildingTransformation*spaceTransformation*referencePoints;
  //pointEqual(Point3d(11.1602540378444, -0.66987298107780, 1.8), referencePoints[0]);
  //pointEqual(Point3d(12.5262794416288, -1.03589838486220, 1.8), referencePoints[1]);

  // illuminance maps
  IlluminanceMapVector illuminanceMaps = spaces[0].illuminanceMaps();
  ASSERT_EQ(1u, illuminanceMaps.size());

  // get the points in relative coordinates
  referencePoints = illuminanceMaps[0].referencePoints();
  ASSERT_EQ(static_cast<unsigned>(25), referencePoints.size()); // 5x5
  //pointEqual(Point3d(0, 0, 0.8), referencePoints[0]);
  //pointEqual(Point3d(10, 0, 0.8), referencePoints[4]);
  //pointEqual(Point3d(10, 10, 0.8), referencePoints[24]);

  // convert the points to absolute coordinates
  //referencePoints = buildingTransformation*spaceTransformation*referencePoints;
  //pointEqual(Point3d(11.1602540378444, -0.66987298107780, 1.8), referencePoints[0]);
  //pointEqual(Point3d(16.1602540378444, -9.33012701892220, 1.8), referencePoints[4]);
  //pointEqual(Point3d(24.8205080756888, -4.33012701892220, 1.8), referencePoints[24]);
}

TEST_F(EnergyPlusFixture,GeometryTranslator_AbsoluteTest)
{
   /* SketchUp code for this test:

    model = Sketchup.active_model
    entities = model.entities
    entities.clear!

    building = entities.add_group
    zone = building.entities.add_group
    face = zone.entities.add_face([0.m,0.m,1.m], [0.m,0.m,0.m], [10.m,0.m,0.m], [10.m,0.m,1.m])

    building_rotation = Geom::Transformation::rotation([0,0,0], [0,0,1], -30.degrees)
    building.transform! building_rotation

    zone_rotation = Geom::Transformation::rotation([0,0,0], [0,0,1], -30.degrees)
    zone_translation = Geom::Transformation::translation([10.m,5.m,1.m])
    zone.transform! zone_translation*zone_rotation

    inverse_transformation = zone_rotation.inverse*zone_translation.inverse*building_rotation.inverse
    zone.entities.transform_entities(inverse_transformation , face)

    face.vertices.each do |vertex|
      puts "#{vertex.position[0].to_m}, #{vertex.position[1].to_m}, #{vertex.position[2].to_m}"
    end

    face.vertices.each do |vertex|
      abs_position = building_rotation*zone_translation*zone_rotation*vertex.position
      puts "#{abs_position[0].to_m}, #{abs_position[1].to_m}, #{abs_position[2].to_m}"
    end
  */

  Workspace workspace = testWorkspace(CoordinateSystem::Absolute, CoordinateSystem::Absolute, CoordinateSystem::Absolute);

  // now create a model and check everthing is as expected
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(workspace);

  Building building = model.getUniqueModelObject<Building>();
  Transformation buildingTransformation = building.transformation();

  SpaceVector spaces = building.spaces();
  ASSERT_EQ(static_cast<unsigned>(1), spaces.size());
  Transformation spaceTransformation = spaces[0].transformation();

  SurfaceVector surfaces = spaces[0].surfaces();
  ASSERT_EQ(static_cast<unsigned>(1), surfaces.size());

  // get the vertices in relative coordinates
  Point3dVector vertices = surfaces[0].vertices();
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(-6.16025403784438, -9.33012701892219, 0.0), vertices[0]);
  pointEqual(Point3d(-6.16025403784438, -9.33012701892219, -1.0), vertices[1]);
  pointEqual(Point3d(-1.16025403784439, -0.66987298107780, -1.0), vertices[2]);
  pointEqual(Point3d(-1.16025403784439, -0.66987298107780, 0.0), vertices[3]);

  // convert the vertices to absolute coordinates
  vertices = buildingTransformation*spaceTransformation*vertices;
  pointEqual(Point3d(0,0,1), vertices[0]);
  pointEqual(Point3d(0,0,0), vertices[1]);
  pointEqual(Point3d(10,0,0), vertices[2]);
  pointEqual(Point3d(10,0,1), vertices[3]);

  // daylighting controls
  DaylightingControlVector daylightingControls = spaces[0].daylightingControls();
  ASSERT_EQ(2u, daylightingControls.size());

  // get the points in relative coordinates
  std::vector<Point3d> referencePoints;
  referencePoints.push_back(daylightingControls[0].position());
  referencePoints.push_back(daylightingControls[1].position());
  ASSERT_EQ(static_cast<unsigned>(2), referencePoints.size());
  //pointEqual(Point3d(-6.1602540378444, -9.3301270189222, -0.2), referencePoints[0]);
  //pointEqual(Point3d(-6.5262794416288, -7.9641016151378, -0.2), referencePoints[1]);

  // convert the points to absolute coordinates
  referencePoints = buildingTransformation*spaceTransformation*referencePoints;
  //pointEqual(Point3d(0, 0, 0.8), referencePoints[0]);
  //pointEqual(Point3d(1, 1, 0.8), referencePoints[1]);

  // illuminance maps
  IlluminanceMapVector illuminanceMaps = spaces[0].illuminanceMaps();
  ASSERT_EQ(1u, illuminanceMaps.size());

  // get the points in relative coordinates
  // note this map is transformed, cannot preserve the absolute map
  referencePoints = illuminanceMaps[0].referencePoints();
  ASSERT_EQ(static_cast<unsigned>(25), referencePoints.size()); // 5x5
  //pointEqual(Point3d(-14.820508075689, -9.3301270189222, -0.2), referencePoints[0]);
  //pointEqual(Point3d(-1.1602540378444, -9.3301270189222, -0.2), referencePoints[4]);
  //pointEqual(Point3d(-1.1602540378444, 4.3301270189222, -0.2), referencePoints[24]);

  // convert the points to absolute coordinates
  // note this map is transformed, cannot preserve the absolute map
  referencePoints = buildingTransformation*spaceTransformation*referencePoints;
  //pointEqual(Point3d(-4.3301270189222, 7.5, 0.8), referencePoints[0]);
  //pointEqual(Point3d(2.5, -4.3301270189222, 0.8), referencePoints[4]);
  //pointEqual(Point3d(7.5, 14.330127018922, 0.8), referencePoints[20]);
  //pointEqual(Point3d(14.330127018922, 2.5, 0.8), referencePoints[24]);
}

void compareSurfaces(const WorkspaceObject& refObject, const WorkspaceObject& testObject)
{

  ASSERT_EQ(refObject.iddObject().type().value(), testObject.iddObject().type().value());
  switch(refObject.iddObject().type().value()){
    case IddObjectType::BuildingSurface_Detailed:
      ASSERT_TRUE(refObject.getTarget(BuildingSurface_DetailedFields::ZoneName));
      ASSERT_TRUE(testObject.getTarget(BuildingSurface_DetailedFields::ZoneName));
      EXPECT_TRUE(refObject.getTarget(BuildingSurface_DetailedFields::ZoneName)->name().get() ==
                  testObject.getTarget(BuildingSurface_DetailedFields::ZoneName)->name().get());
      break;
    case IddObjectType::FenestrationSurface_Detailed:
      ASSERT_TRUE(refObject.getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName));
      ASSERT_TRUE(testObject.getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName));
      EXPECT_TRUE(refObject.getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName)->name().get() ==
                  testObject.getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName)->name().get());
      break;
    case IddObjectType::Shading_Site_Detailed:
      break;
    case IddObjectType::Shading_Building_Detailed:
      break;
    case IddObjectType::Shading_Zone_Detailed:
      ASSERT_TRUE(refObject.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName));
      ASSERT_TRUE(testObject.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName));
      // might not be the same since we have to look for surface, would be good to fix eventually
      //EXPECT_TRUE(refObject.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName)->name().get() ==
      //  testObject.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName)->name().get());
      break;
    default:
      EXPECT_TRUE(false) << "Unknown type " << refObject.iddObject().type().valueName();
  }

  EXPECT_TRUE(refObject.numExtensibleGroups() >= static_cast<unsigned>(3));
  ASSERT_EQ(refObject.numExtensibleGroups(), testObject.numExtensibleGroups());

  // get vertices
  Point3dVector refVerts;
  Point3dVector testVerts;
  for (unsigned i = 0; i < refObject.numExtensibleGroups(); ++i){
    IdfExtensibleGroup refGroup = refObject.getExtensibleGroup(i);
    IdfExtensibleGroup testGroup = testObject.getExtensibleGroup(i);
    ASSERT_TRUE(refGroup.getDouble(0));
    ASSERT_TRUE(refGroup.getDouble(1));
    ASSERT_TRUE(refGroup.getDouble(2));
    ASSERT_TRUE(testGroup.getDouble(0));
    ASSERT_TRUE(testGroup.getDouble(1));
    ASSERT_TRUE(testGroup.getDouble(2));
    refVerts.push_back(Point3d(refGroup.getDouble(0).get(), refGroup.getDouble(1).get(), refGroup.getDouble(2).get()));
    testVerts.push_back(Point3d(testGroup.getDouble(0).get(), testGroup.getDouble(1).get(), testGroup.getDouble(2).get()));
  }

  // reorder
  refVerts = reorderULC(refVerts);
  testVerts = reorderULC(testVerts);

  // compare
  bool compare = true;
  for (unsigned i = 0; i < refObject.numExtensibleGroups(); ++i){
    compare = compare && (std::abs(refVerts[i].x()-testVerts[i].x()) < 0.01);
    compare = compare && (std::abs(refVerts[i].y()-testVerts[i].y()) < 0.01);
    compare = compare && (std::abs(refVerts[i].z()-testVerts[i].z()) < 0.01);
  }
  EXPECT_TRUE(compare);
  if (!compare){
    std::cout << "ref = " << refObject.iddObject().type().valueName() << ", "
              << refObject.name().get() << ", " << refVerts << std::endl;
    std::cout << "test = " << testObject.iddObject().type().valueName() << ", "
              << testObject.name().get() << ", " << testVerts <<  std::endl;
  }
}

TEST_F(EnergyPlusFixture,GeometryTranslator_SimpleRelativeTest)
{
  openstudio::path refIdfPath = resourcesPath() / openstudio::toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Reference.idf");
  OptionalIdfFile oIdfFile = IdfFile::load(refIdfPath,IddFileType::EnergyPlus);
  ASSERT_TRUE(oIdfFile);
  IdfFile refIdfFile = *oIdfFile;
  Workspace refWorkspace(refIdfFile);

  openstudio::path relIdfPath = resourcesPath() / openstudio::toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative.idf");
  oIdfFile = IdfFile::load(relIdfPath,IddFileType::EnergyPlus);
  ASSERT_TRUE(oIdfFile);
  IdfFile relIdfFile = *oIdfFile;
  Workspace relWorkspace(relIdfFile);

  GeometryTranslator geometryTranslator(relWorkspace);
  EXPECT_TRUE(geometryTranslator.convert(CoordinateSystem::Relative, CoordinateSystem::Relative));

  // should not be any of the following types
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Wall_Detailed).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::RoofCeiling_Detailed).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Floor_Detailed).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Wall_Exterior).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Wall_Adiabatic).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Wall_Underground).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Wall_Interzone).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Roof).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Ceiling_Adiabatic).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Ceiling_Interzone).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Floor_GroundContact).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Floor_Adiabatic).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Floor_Interzone).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Window).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Door).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::GlazedDoor).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Window_Interzone).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Door_Interzone).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::GlazedDoor_Interzone).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Shading_Site).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Shading_Building).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Shading_Overhang).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Shading_Overhang_Projection).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Shading_Fin).empty());
  EXPECT_TRUE(relWorkspace.getObjectsByType(IddObjectType::Shading_Fin_Projection).empty());

  // types to check
  std::vector<unsigned> iddTypes;
  iddTypes.push_back(IddObjectType::BuildingSurface_Detailed);
  iddTypes.push_back(IddObjectType::FenestrationSurface_Detailed);
  iddTypes.push_back(IddObjectType::Shading_Site_Detailed);
  iddTypes.push_back(IddObjectType::Shading_Building_Detailed);
  iddTypes.push_back(IddObjectType::Shading_Zone_Detailed);

  // check all types in both files
  for (unsigned iddType : iddTypes){
    // for debugging
    IddObjectType debugIddType(iddType);
    // check sizes
    EXPECT_EQ(refWorkspace.getObjectsByType(iddType).size(),
              relWorkspace.getObjectsByType(iddType).size());
    // loop over each surf in ref
    for (WorkspaceObject refObject : refWorkspace.getObjectsByType(iddType)){
      // find in test file
      OptionalWorkspaceObject relObject = relWorkspace.getObjectByTypeAndName(iddType, refObject.name().get());
      ASSERT_TRUE(relObject);
      compareSurfaces(refObject, *relObject);
    }
  }
}

TEST_F(EnergyPlusFixture,GeometryTranslator_VerticesForAzimuthTiltXYZLengthWidthOrHeight)
{
  Point3dVector vertices;

  // a dummy floor
  vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(0, 180, 73, 14, 0, 25, 20);
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(73,34,0), vertices[0], 0.01);
  pointEqual(Point3d(73,14,0), vertices[1], 0.01);
  pointEqual(Point3d(48,14,0), vertices[2], 0.01);
  pointEqual(Point3d(48,34,0), vertices[3], 0.01);

  // a dummy wall pointing North
  vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(0, 90, 73, 14, 0, 20, 4);
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(73,14,4), vertices[0], 0.01);
  pointEqual(Point3d(73,14,0), vertices[1], 0.01);
  pointEqual(Point3d(53,14,0), vertices[2], 0.01);
  pointEqual(Point3d(53,14,4), vertices[3], 0.01);

  // a dummy wall pointing East
  vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(90, 90, 73, 14, 0, 20, 4);
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(73,14,4), vertices[0], 0.01);
  pointEqual(Point3d(73,14,0), vertices[1], 0.01);
  pointEqual(Point3d(73,34,0), vertices[2], 0.01);
  pointEqual(Point3d(73,34,4), vertices[3], 0.01);

  // a dummy wall pointing South
  vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(180, 90, 73, 14, 0, 20, 4);
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(73,14,4), vertices[0], 0.01);
  pointEqual(Point3d(73,14,0), vertices[1], 0.01);
  pointEqual(Point3d(93,14,0), vertices[2], 0.01);
  pointEqual(Point3d(93,14,4), vertices[3], 0.01);

  // a dummy wall pointing West
  vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(270, 90, 73, 14, 0, 20, 4);
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(73,14,4), vertices[0], 0.01);
  pointEqual(Point3d(73,14,0), vertices[1], 0.01);
  pointEqual(Point3d(73,-6,0), vertices[2], 0.01);
  pointEqual(Point3d(73,-6,4), vertices[3], 0.01);

  // a dummy roof
  vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(0, 0, 73, 14, 4, 25, 20);
  ASSERT_EQ(static_cast<unsigned>(4), vertices.size());
  pointEqual(Point3d(73,-6,4), vertices[0], 0.01);
  pointEqual(Point3d(73,14,4), vertices[1], 0.01);
  pointEqual(Point3d(48,14,4), vertices[2], 0.01);
  pointEqual(Point3d(48,-6,4), vertices[3], 0.01);

}

TEST_F(EnergyPlusFixture,GeometryTranslator_Swap){

  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  OptionalWorkspaceObject zoneObject = workspace.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(zoneObject);
  EXPECT_TRUE(zoneObject->setName("Zone"));
  ASSERT_TRUE(zoneObject->name());
  EXPECT_EQ("Zone", zoneObject->name().get());
  EXPECT_EQ(static_cast<unsigned>(1), workspace.objects().size());
  OptionalWorkspaceObject oldObject = workspace.addObject(IdfObject(IddObjectType::Wall_Exterior));
  EXPECT_TRUE(oldObject->setName("Wall"));
  ASSERT_TRUE(oldObject);
  EXPECT_EQ(static_cast<unsigned>(2), workspace.objects().size());
  ASSERT_EQ(static_cast<unsigned>(1), workspace.getObjectsByType(IddObjectType::Wall_Exterior).size());
  EXPECT_TRUE(oldObject->setPointer(Wall_ExteriorFields::ZoneName, zoneObject->handle()));
  ASSERT_TRUE(oldObject->getTarget(Wall_ExteriorFields::ZoneName));
  EXPECT_TRUE(zoneObject->handle() == oldObject->getTarget(Wall_ExteriorFields::ZoneName)->handle());

  IdfObject newObject(IddObjectType::BuildingSurface_Detailed);
  EXPECT_TRUE(newObject.setName("Wall"));
  EXPECT_TRUE(newObject.setString(BuildingSurface_DetailedFields::ZoneName, "Zone"));
  EXPECT_TRUE(workspace.swap(*oldObject, newObject));
  EXPECT_EQ(static_cast<unsigned>(2), workspace.objects().size());
  EXPECT_EQ(static_cast<unsigned>(0), workspace.getObjectsByType(IddObjectType::Wall_Exterior).size());
  ASSERT_EQ(static_cast<unsigned>(1), workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
  ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0].getTarget(BuildingSurface_DetailedFields::ZoneName));
  EXPECT_TRUE(zoneObject->handle() == workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0].getTarget(BuildingSurface_DetailedFields::ZoneName)->handle());
}

TEST_F(EnergyPlusFixture,GeometryTranslator_4ZoneWithShading_Simple_2)
{
  openstudio::path relIdfPath = resourcesPath() / openstudio::toPath("energyplus/SimpleSurfaces/4ZoneWithShading_Simple_2.idf");
  OptionalIdfFile oIdfFile = IdfFile::load(relIdfPath,IddFileType::EnergyPlus);
  ASSERT_TRUE(oIdfFile);
  Workspace workspace(*oIdfFile);

  ReverseTranslator reverseTranslator;
  EXPECT_NO_THROW(reverseTranslator.translateWorkspace(workspace));
}
