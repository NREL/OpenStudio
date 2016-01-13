/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Version_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include "../../utilities/time/Time.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ReverseTranslator_EmptyIdfFile)
{
  Workspace inWorkspace(StrictnessLevel::Draft,IddFileType::EnergyPlus);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);

  OptionalWorkspaceObject w = inWorkspace.addObject(IdfObject(IddObjectType::Version));
  ASSERT_TRUE(w);
  EXPECT_TRUE(w->setString(VersionFields::VersionIdentifier, "7.0"));

  model = reverseTranslator.translateWorkspace(inWorkspace);

  EXPECT_TRUE(w->setString(VersionFields::VersionIdentifier, "5.0"));

  model = reverseTranslator.translateWorkspace(inWorkspace);
}

TEST_F(EnergyPlusFixture,ReverseTranslator_NewFileTemplate)
{
  openstudio::path idfPath = resourcesPath() / openstudio::toPath("energyplus/NewFileTemplate.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.translateWorkspace(inWorkspace));
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
}

TEST_F(EnergyPlusFixture,ReverseTranslator_NewFileTemplate_LoadModel)
{
  openstudio::path idfPath = resourcesPath() / openstudio::toPath("energyplus/NewFileTemplate.idf");
  ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.loadModel(idfPath));
  OptionalModel model = reverseTranslator.loadModel(idfPath);
  ASSERT_TRUE(model);
}

TEST_F(EnergyPlusFixture,ReverseTranslator_DaylightingOffice)
{
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/Daylighting_Office/in.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save( resourcesPath() / toPath("energyplus/Daylighting_Office/in.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  workspace.save( resourcesPath() / toPath("energyplus/Daylighting_Office/in2.idf"), true);
}

TEST_F(EnergyPlusFixture,ReverseTranslator_BestestEx)
{
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/BestestEx/in.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);

  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save( resourcesPath() / toPath("energyplus/BestestEx/in.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  workspace.save( resourcesPath() / toPath("energyplus/BestestEx/in2.idf"), true);
}

TEST_F(EnergyPlusFixture,ReverseTranslator_SimpleRelativeTest)
{
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  model.save( resourcesPath() / toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  workspace.save( resourcesPath() / toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative2.idf"), true);
}

TEST_F(EnergyPlusFixture,ReverseTranslator_Building)
{
  Workspace inWorkspace(StrictnessLevel::None, IddFileType::EnergyPlus);
  inWorkspace.addObject(IdfObject(IddObjectType::Building));
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  EXPECT_TRUE(model.getOptionalUniqueModelObject<openstudio::model::Building>());
}

TEST_F(EnergyPlusFixture,ReverseTranslator_Zone)
{
  Workspace inWorkspace(StrictnessLevel::None, IddFileType::EnergyPlus);
  OptionalWorkspaceObject zoneObject = inWorkspace.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(zoneObject);
  OptionalWorkspaceObject lightsObject = inWorkspace.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(lightsObject);
  EXPECT_TRUE(lightsObject->setPointer(openstudio::LightsFields::ZoneorZoneListName, zoneObject->handle()));

  ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.translateWorkspace(inWorkspace));
  Model model = reverseTranslator.translateWorkspace(inWorkspace);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::ThermalZone>().size());
  openstudio::model::ThermalZone zone = model.getModelObjects<openstudio::model::ThermalZone>()[0];
  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::Space>().size());
  openstudio::model::Space space = model.getModelObjects<openstudio::model::Space>()[0];
  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::Lights>().size());
  openstudio::model::Lights lights = model.getModelObjects<openstudio::model::Lights>()[0];
  ASSERT_TRUE(lights.space());
  EXPECT_TRUE(space.handle() == lights.space()->handle());
  ASSERT_TRUE(lights.space()->thermalZone());
  EXPECT_TRUE(zone.handle() == lights.space()->thermalZone()->handle());
}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_TranslateScheduleCompact) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None,openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject( openstudio::IddObjectType::Schedule_Compact );
  idfObject.setString(1,"Fraction");
  idfObject.setString(2,"Through: 12/31");
  idfObject.setString(3,"For: Weekdays SummerDesignDay");
  idfObject.setString(4,"Until: 08:00");
  idfObject.setString(5,"0.0");
  idfObject.setString(6,"Until: 18:00");
  idfObject.setString(7,"1.0");
  idfObject.setString(8,"Until: 24:00");
  idfObject.setString(9,"0.0");
  idfObject.setString(10,"For: Weekends WinterDesignDay");
  idfObject.setString(11,"Until: 10:00");
  idfObject.setString(12,"0.0");
  idfObject.setString(13,"Until: 16:00");
  idfObject.setString(14,"1.0");
  idfObject.setString(15,"Until: 24:00");
  idfObject.setString(16,"0.0");
  idfObject.setString(17,"For: Holidays AllOtherDays");
  idfObject.setString(18,"Until: 24:00");
  idfObject.setString(19,"0.0");

  WorkspaceObject epScheduleCompact = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::ScheduleCompact>().size());
  openstudio::model::ScheduleCompact scheduleCompact = model.getModelObjects<openstudio::model::ScheduleCompact>()[0];

  EXPECT_EQ(unsigned(20),epScheduleCompact.numFields());
  EXPECT_EQ(unsigned(21),scheduleCompact.numFields()); // has handle field

  ASSERT_EQ(epScheduleCompact.numFields() + 1u,scheduleCompact.numFields());

  ASSERT_TRUE(epScheduleCompact.name());
  ASSERT_TRUE(scheduleCompact.name());
  EXPECT_EQ(epScheduleCompact.name().get(),scheduleCompact.name().get());

  for( unsigned i = 1; i < epScheduleCompact.numFields(); i++ )
  {
    boost::optional<std::string> s1 = epScheduleCompact.getString(i);
    boost::optional<std::string> s2 = scheduleCompact.getString(i+1);

    ASSERT_TRUE(s1);
    ASSERT_TRUE(s2);

    EXPECT_EQ(s1.get(),s2.get());
  }
}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_TranslateMasselessOpaqueMaterial)
{
  // Initialize the workspace
  openstudio::Workspace ws(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  // Create the Material:NoMaxx idfObject
  IdfObject idfObject( openstudio::IddObjectType::Material_NoMass );
  idfObject.setString( 0, "Test Mat 1");
  idfObject.setString( 1, "Rough" );
  idfObject.setString( 2, "3.05" );
  idfObject.setString( 3, "0.85" );
  idfObject.setString( 4, "0.6" );
  idfObject.setString( 5, "0.6" );

  WorkspaceObject epMaterial = ws.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(ws));
  Model model = trans.translateWorkspace(ws);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::MasslessOpaqueMaterial>().size());
  openstudio::model::MasslessOpaqueMaterial mat = model.getModelObjects<openstudio::model::MasslessOpaqueMaterial>()[0];

  EXPECT_EQ( "Rough", mat.roughness() );
  EXPECT_EQ( 3.05, mat.thermalResistance() );
  EXPECT_EQ( 0.85, mat.thermalAbsorptance() );
  EXPECT_EQ( 0.6, mat.solarAbsorptance() );
  EXPECT_EQ( 0.6, mat.visibleAbsorptance() );
}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_TranslateStandardOpaqueMaterial)
{
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None,openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject( openstudio::IddObjectType::Material );
  idfObject.setString(0, "Test Material"); // Name
  idfObject.setString(1, "Smooth"); // Roughness
  idfObject.setString(2, "0.012");  // Thickness
  idfObject.setString(3, "3.2");    // Conductivity
  idfObject.setString(4, "2.5");    // Density
  idfObject.setString(5, "2.04");   // Specific Heat
  idfObject.setString(6, "0.8");    // Thermal Absorptance
  idfObject.setString(7, "0.6");    // Solar Absorptance
  idfObject.setString(8, "0.6");    // Visible Absorptance

  openstudio::WorkspaceObject epMaterial = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::StandardOpaqueMaterial>().size());
  openstudio::model::StandardOpaqueMaterial mat = model.getModelObjects<openstudio::model::StandardOpaqueMaterial>()[0];

  EXPECT_EQ( unsigned(10), mat.numFields());

  EXPECT_EQ( "Test Material", *(mat.name()) );
  EXPECT_EQ( "Smooth", mat.roughness());
  EXPECT_EQ( 0.012, mat.thickness());
  EXPECT_EQ( 3.2, mat.thermalConductivity());
  EXPECT_EQ( 2.5, mat.density());
  EXPECT_EQ( 2.04, mat.specificHeat());
  EXPECT_EQ( 0.8, mat.thermalAbsorptance());
  EXPECT_EQ( 0.6, mat.solarAbsorptance());
  EXPECT_EQ( 0.6, mat.visibleAbsorptance());

}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_TranslateConstruction)
{
  // Initialize the workspace from a file containing a Construction and two Materials
  openstudio::path idfPath = resourcesPath() / openstudio::toPath("energyplus/TranslatorV2_Tests/Simple_Construction.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);

  Workspace inWorkspace(*idfFile);
  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(inWorkspace));
  Model model = trans.translateWorkspace(inWorkspace);

  boost::optional<openstudio::model::Construction> construction = model.getModelObjectByName<openstudio::model::Construction>("Metal Building_NR_Roof");

  ASSERT_TRUE( construction );

  ASSERT_EQ( "Metal Roofing", *((construction->getLayer(0)).name()) );
  ASSERT_EQ( "Roof Insulation [55]", *((construction->getLayer(1)).name()) );

}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_TranslateSite)
{
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None,openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject( openstudio::IddObjectType::Site_Location );
  idfObject.setString(Site_LocationFields::Name, "Test Site");
  idfObject.setDouble(Site_LocationFields::Latitude, 39.6);
  idfObject.setDouble(Site_LocationFields::Longitude, 105.2);
  idfObject.setDouble(Site_LocationFields::TimeZone, -7.0);
  idfObject.setDouble(Site_LocationFields::Elevation, 1729.74);

  openstudio::WorkspaceObject epSite = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_TRUE(model.getOptionalUniqueModelObject<openstudio::model::Site>());

  openstudio::model::Site site = model.getUniqueModelObject<openstudio::model::Site>();

  EXPECT_EQ( 7u, site.numFields());      // terrain moves from Building to Site in OS
  EXPECT_EQ( "Test Site", *(site.name()) );
  EXPECT_EQ( 39.6, site.latitude() );
  EXPECT_EQ( 105.2, site.longitude() );
  EXPECT_EQ( -7.0, site.timeZone() );
  EXPECT_EQ( 1729.74, site.elevation() );
  EXPECT_EQ("Suburbs", site.terrain() ); // default value
}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_SmallOffice) {
  openstudio::path p = resourcesPath() / toPath("resultsviewer/SmallOffice/SmallOffice.idf");
  Workspace ws = Workspace::load(p).get();
  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);

  OptionalSubSurface osub = model.getModelObjectByName<SubSurface>("Perimeter_ZN_1_wall_south_door");
  ASSERT_TRUE(osub);
  EXPECT_EQ("GlassDoor",osub->subSurfaceType());
}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_LargeOffice) {
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("energyplus/RefLargeOffice/RefBldgLargeOfficeNew2004_Chicago.idf");
  Workspace ws = Workspace::load(inputPath).get();
  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);

  // surface named "Core_top_ZN_5_Floor" should have "Adiabatic" string for outsideBoundaryCondition
  OptionalSurface osurf = model.getModelObjectByName<Surface>("Core_top_ZN_5_Floor");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Adiabatic",osurf->outsideBoundaryCondition());
}

TEST_F(EnergyPlusFixture,ReverseTranslatorTest_ZoneBoundaryCondition) {
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("energyplus/ZoneBoundaryCondition/Bug_486_zone_bdr_test.idf");
  Workspace ws = Workspace::load(inputPath).get();
  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);

  // surface named "1AF737" should have "surface" string for OutsideBoundaryCondition
  OptionalSurface osurf_a = model.getModelObjectByName<Surface>("1AF737");
  ASSERT_TRUE(osurf_a);
  EXPECT_EQ("Surface",osurf_a->outsideBoundaryCondition());
  // and the parent of the "outsideBoundaryObject" should be space named "8B02A8"
  OptionalSurface osurf_b = osurf_a->adjacentSurface();
  ASSERT_TRUE(osurf_b);
  OptionalSpace ospace_b = osurf_b->space();
  ASSERT_TRUE(ospace_b);
  EXPECT_EQ("8B02A8",ospace_b->name().get());
  // confirm that surf_b has surf_a  as outside boundary object
  ASSERT_TRUE(osurf_b->adjacentSurface());
  EXPECT_EQ(osurf_a->handle(),osurf_b->adjacentSurface()->handle());
}

TEST_F(EnergyPlusFixture,ReverseTranslator_ScheduleDayInterval) {
  // ScheduleDayInterval time entries contain the string 'Until: ' that must be
  // stripped off before constructing a Time object. We are also more lenient in
  // making this optional.
  Workspace ws(StrictnessLevel::Draft,IddFileType(IddFileType::EnergyPlus));
  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::Schedule_Day_Interval));
  ASSERT_TRUE(owo);
  WorkspaceObject object = *owo;
  object.setName("Heating Setpoint Design Day");
  StringVector groupValues(2u);
  groupValues[0] = std::string("Until: 12:00");
  groupValues[1] = std::string("21.1");
  object.pushExtensibleGroup(groupValues);
  groupValues[0] = std::string("24:00");
  groupValues[1] = std::string("20.5");
  object.pushExtensibleGroup(groupValues);

  ReverseTranslator translator;
  Model model = translator.translateWorkspace(ws);
  EXPECT_TRUE(translator.errors().empty());
  // There are warnings related to ws being a partial model.
  EXPECT_TRUE(translator.untranslatedIdfObjects().empty());
  ScheduleDayVector daySchedules = model.getModelObjects<ScheduleDay>();
  ASSERT_EQ(1u,daySchedules.size());
  ScheduleDay daySchedule = daySchedules[0];
  DoubleVector values = daySchedule.values();
  ASSERT_EQ(2u,values.size());
  EXPECT_DOUBLE_EQ(21.1,values[0]);
  EXPECT_DOUBLE_EQ(20.5,values[1]);
  TimeVector times = daySchedule.times();
  ASSERT_EQ(2u,times.size());
  EXPECT_EQ(Time(0,12,0),times[0]);
  EXPECT_EQ(Time(0,24,0),times[1]);
}
