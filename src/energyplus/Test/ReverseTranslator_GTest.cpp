/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
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
#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/FoundationKivaSettings.hpp"
#include "../../model/FoundationKivaSettings_Impl.hpp"
#include "../../model/OutputTableSummaryReports.hpp"
#include "../../model/OutputTableSummaryReports_Impl.hpp"
#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter_Impl.hpp"
#include "../../model/PerformancePrecisionTradeoffs.hpp"
#include "../../model/PerformancePrecisionTradeoffs_Impl.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "../../model/StandardGlazing.hpp"
#include "../../model/StandardGlazing_Impl.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData_Impl.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/SurfaceControlMovableInsulation.hpp"
#include "../../model/SurfaceControlMovableInsulation_Impl.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/OtherEquipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Version_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include <utilities/idd/Foundation_Kiva_Settings_FieldEnums.hxx>
#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include <utilities/idd/Foundation_Kiva_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/SurfaceProperty_ExposedFoundationPerimeter_FieldEnums.hxx>
#include <utilities/idd/PerformancePrecisionTradeoffs_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Daylighting_ReferencePoint_FieldEnums.hxx>
#include <utilities/idd/SurfaceControl_MovableInsulation_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_CurveFit_OperatingMode_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>
#include <utilities/idd/Curve_Biquadratic_FieldEnums.hxx>
#include <utilities/idd/Curve_Quadratic_FieldEnums.hxx>

#include "../../utilities/time/Time.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ReverseTranslator_EmptyIdfFile) {
  Workspace inWorkspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);

  OptionalWorkspaceObject w = inWorkspace.addObject(IdfObject(IddObjectType::Version));
  ASSERT_TRUE(w);
  EXPECT_TRUE(w->setString(VersionFields::VersionIdentifier, "7.0"));

  model = reverseTranslator.translateWorkspace(inWorkspace);

  EXPECT_TRUE(w->setString(VersionFields::VersionIdentifier, "5.0"));

  model = reverseTranslator.translateWorkspace(inWorkspace);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_NewFileTemplate) {
  openstudio::path idfPath = resourcesPath() / openstudio::toPath("energyplus/NewFileTemplate.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.translateWorkspace(inWorkspace));
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_NewFileTemplate_LoadModel) {
  openstudio::path idfPath = resourcesPath() / openstudio::toPath("energyplus/NewFileTemplate.idf");
  ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.loadModel(idfPath));
  OptionalModel model = reverseTranslator.loadModel(idfPath);
  ASSERT_TRUE(model);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_DaylightingOffice) {
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/Daylighting_Office/in.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  // model.save( resourcesPath() / toPath("energyplus/Daylighting_Office/in.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  // workspace.save( resourcesPath() / toPath("energyplus/Daylighting_Office/in2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_BestestEx) {
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/BestestEx/in.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);

  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  // model.save( resourcesPath() / toPath("energyplus/BestestEx/in.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  // workspace.save( resourcesPath() / toPath("energyplus/BestestEx/in2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SimpleRelativeTest) {
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  // model.save( resourcesPath() / toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  // workspace.save( resourcesPath() / toPath("energyplus/SimpleSurfaces/SimpleSurfaces_Relative2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_Building) {
  Workspace inWorkspace(StrictnessLevel::None, IddFileType::EnergyPlus);
  inWorkspace.addObject(IdfObject(IddObjectType::Building));
  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  EXPECT_TRUE(model.getOptionalUniqueModelObject<openstudio::model::Building>());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_Zone) {
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

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_TranslateScheduleCompact) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::Schedule_Compact);
  idfObject.setString(1, "Fraction");
  idfObject.setString(2, "Through: 12/31");
  idfObject.setString(3, "For: Weekdays SummerDesignDay");
  idfObject.setString(4, "Until: 08:00");
  idfObject.setString(5, "0.0");
  idfObject.setString(6, "Until: 18:00");
  idfObject.setString(7, "1.0");
  idfObject.setString(8, "Until: 24:00");
  idfObject.setString(9, "0.0");
  idfObject.setString(10, "For: Weekends WinterDesignDay");
  idfObject.setString(11, "Until: 10:00");
  idfObject.setString(12, "0.0");
  idfObject.setString(13, "Until: 16:00");
  idfObject.setString(14, "1.0");
  idfObject.setString(15, "Until: 24:00");
  idfObject.setString(16, "0.0");
  idfObject.setString(17, "For: Holidays AllOtherDays");
  idfObject.setString(18, "Until: 24:00");
  idfObject.setString(19, "0.0");

  WorkspaceObject epScheduleCompact = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::ScheduleCompact>().size());
  openstudio::model::ScheduleCompact scheduleCompact = model.getModelObjects<openstudio::model::ScheduleCompact>()[0];

  EXPECT_EQ(unsigned(20), epScheduleCompact.numFields());
  EXPECT_EQ(unsigned(21), scheduleCompact.numFields());  // has handle field

  ASSERT_EQ(epScheduleCompact.numFields() + 1u, scheduleCompact.numFields());

  ASSERT_TRUE(epScheduleCompact.name());
  ASSERT_TRUE(scheduleCompact.name());
  EXPECT_EQ(epScheduleCompact.name().get(), scheduleCompact.name().get());

  for (unsigned i = 1; i < epScheduleCompact.numFields(); i++) {
    boost::optional<std::string> s1 = epScheduleCompact.getString(i);
    boost::optional<std::string> s2 = scheduleCompact.getString(i + 1);

    ASSERT_TRUE(s1);
    ASSERT_TRUE(s2);

    EXPECT_EQ(s1.get(), s2.get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_TranslateMasselessOpaqueMaterial) {
  // Initialize the workspace
  openstudio::Workspace ws(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  // Create the Material:NoMaxx idfObject
  IdfObject idfObject(openstudio::IddObjectType::Material_NoMass);
  idfObject.setString(0, "Test Mat 1");
  idfObject.setString(1, "Rough");
  idfObject.setString(2, "3.05");
  idfObject.setString(3, "0.85");
  idfObject.setString(4, "0.6");
  idfObject.setString(5, "0.6");

  WorkspaceObject epMaterial = ws.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(ws));
  Model model = trans.translateWorkspace(ws);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::MasslessOpaqueMaterial>().size());
  openstudio::model::MasslessOpaqueMaterial mat = model.getModelObjects<openstudio::model::MasslessOpaqueMaterial>()[0];

  EXPECT_EQ("Rough", mat.roughness());
  EXPECT_EQ(3.05, mat.thermalResistance());
  EXPECT_EQ(0.85, mat.thermalAbsorptance());
  EXPECT_EQ(0.6, mat.solarAbsorptance());
  EXPECT_EQ(0.6, mat.visibleAbsorptance());
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_TranslateStandardOpaqueMaterial) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::Material);
  idfObject.setString(0, "Test Material");  // Name
  idfObject.setString(1, "Smooth");         // Roughness
  idfObject.setString(2, "0.012");          // Thickness
  idfObject.setString(3, "3.2");            // Conductivity
  idfObject.setString(4, "2.5");            // Density
  idfObject.setString(5, "1400.0");         // Specific Heat
  idfObject.setString(6, "0.8");            // Thermal Absorptance
  idfObject.setString(7, "0.6");            // Solar Absorptance
  idfObject.setString(8, "0.6");            // Visible Absorptance

  openstudio::WorkspaceObject epMaterial = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::StandardOpaqueMaterial>().size());
  openstudio::model::StandardOpaqueMaterial mat = model.getModelObjects<openstudio::model::StandardOpaqueMaterial>()[0];

  EXPECT_EQ(unsigned(10), mat.numFields());

  EXPECT_EQ("Test Material", *(mat.name()));
  EXPECT_EQ("Smooth", mat.roughness());
  EXPECT_EQ(0.012, mat.thickness());
  EXPECT_EQ(3.2, mat.thermalConductivity());
  EXPECT_EQ(2.5, mat.density());
  EXPECT_EQ(1400.0, mat.specificHeat());
  EXPECT_EQ(0.8, mat.thermalAbsorptance());
  EXPECT_EQ(0.6, mat.solarAbsorptance());
  EXPECT_EQ(0.6, mat.visibleAbsorptance());
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_TranslateConstruction) {
  // Initialize the workspace from a file containing a Construction and two Materials
  openstudio::path idfPath = resourcesPath() / openstudio::toPath("energyplus/TranslatorV2_Tests/Simple_Construction.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);

  Workspace inWorkspace(*idfFile);
  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(inWorkspace));
  Model model = trans.translateWorkspace(inWorkspace);

  boost::optional<openstudio::model::Construction> construction =
    model.getModelObjectByName<openstudio::model::Construction>("Metal Building_NR_Roof");

  ASSERT_TRUE(construction);

  ASSERT_EQ("Metal Roofing", *((construction->getLayer(0)).name()));
  ASSERT_EQ("Roof Insulation [55]", *((construction->getLayer(1)).name()));
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_TranslateSite) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::Site_Location);
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

  EXPECT_EQ(7u, site.numFields());  // terrain moves from Building to Site in OS
  EXPECT_EQ("Test Site", *(site.name()));
  EXPECT_EQ(39.6, site.latitude());
  EXPECT_EQ(105.2, site.longitude());
  EXPECT_EQ(-7.0, site.timeZone());
  EXPECT_EQ(1729.74, site.elevation());
  EXPECT_EQ("Suburbs", site.terrain());  // default value
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_SmallOffice) {
  openstudio::path p = resourcesPath() / toPath("energyplus/SmallOffice/SmallOffice.idf");
  Workspace ws = Workspace::load(p).get();
  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);

  OptionalSubSurface osub = model.getModelObjectByName<SubSurface>("Perimeter_ZN_1_wall_south_door");
  ASSERT_TRUE(osub);
  EXPECT_EQ("GlassDoor", osub->subSurfaceType());
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_LargeOffice) {
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("energyplus/RefLargeOffice/RefBldgLargeOfficeNew2004_Chicago.idf");
  Workspace ws = Workspace::load(inputPath).get();
  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);

  // surface named "Core_top_ZN_5_Floor" should have "Adiabatic" string for outsideBoundaryCondition
  OptionalSurface osurf = model.getModelObjectByName<Surface>("Core_top_ZN_5_Floor");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Adiabatic", osurf->outsideBoundaryCondition());
}

TEST_F(EnergyPlusFixture, ReverseTranslatorTest_ZoneBoundaryCondition) {
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("energyplus/ZoneBoundaryCondition/Bug_486_zone_bdr_test.idf");
  Workspace ws = Workspace::load(inputPath).get();
  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);

  // surface named "1AF737" should have "surface" string for OutsideBoundaryCondition
  OptionalSurface osurf_a = model.getModelObjectByName<Surface>("1AF737");
  ASSERT_TRUE(osurf_a);
  EXPECT_EQ("Surface", osurf_a->outsideBoundaryCondition());
  // and the parent of the "outsideBoundaryObject" should be space named "8B02A8"
  OptionalSurface osurf_b = osurf_a->adjacentSurface();
  ASSERT_TRUE(osurf_b);
  OptionalSpace ospace_b = osurf_b->space();
  ASSERT_TRUE(ospace_b);
  EXPECT_EQ("8B02A8", ospace_b->name().get());
  // confirm that surf_b has surf_a  as outside boundary object
  ASSERT_TRUE(osurf_b->adjacentSurface());
  EXPECT_EQ(osurf_a->handle(), osurf_b->adjacentSurface()->handle());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ScheduleDayInterval) {
  // ScheduleDayInterval time entries contain the string 'Until: ' that must be
  // stripped off before constructing a Time object. We are also more lenient in
  // making this optional.
  Workspace ws(StrictnessLevel::Draft, IddFileType(IddFileType::EnergyPlus));
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
  ASSERT_EQ(1u, daySchedules.size());
  ScheduleDay daySchedule = daySchedules[0];
  DoubleVector values = daySchedule.values();
  ASSERT_EQ(2u, values.size());
  EXPECT_DOUBLE_EQ(21.1, values[0]);
  EXPECT_DOUBLE_EQ(20.5, values[1]);
  TimeVector times = daySchedule.times();
  ASSERT_EQ(2u, times.size());
  EXPECT_EQ(Time(0, 12, 0), times[0]);
  EXPECT_EQ(Time(0, 24, 0), times[1]);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OtherEquipment) {
  Workspace ws(StrictnessLevel::Draft, IddFileType(IddFileType::EnergyPlus));
  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::OtherEquipment));
  ASSERT_TRUE(owo);
  WorkspaceObject object = *owo;

  object.setName("Other Eq 1");
  object.setString(OtherEquipmentFields::EndUseSubcategory, "Category A");

  ReverseTranslator trans;
  Model model = trans.translateWorkspace(ws);

  OtherEquipmentVector otherEquipments = model.getModelObjects<OtherEquipment>();
  ASSERT_EQ(1u, otherEquipments.size());
  EXPECT_EQ(otherEquipments[0].name().get(), "Other Eq 1");
  EXPECT_EQ(otherEquipments[0].endUseSubcategory(), "Category A");
}

TEST_F(EnergyPlusFixture, ReverseTranslator_FoundationKivaSettings) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::Foundation_Kiva_Settings);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilConductivity, 1.731);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilDensity, 1842.3);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::SoilSpecificHeat, 418.7);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundSolarAbsorptivity, 0.9);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundThermalAbsorptivity, 0.9);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::GroundSurfaceRoughness, 0.03);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::FarFieldWidth, 40);
  idfObject.setString(Foundation_Kiva_SettingsFields::DeepGroundBoundaryCondition, "ZeroFlux");
  idfObject.setDouble(Foundation_Kiva_SettingsFields::DeepGroundDepth, 40);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::MinimumCellDimension, 0.02);
  idfObject.setDouble(Foundation_Kiva_SettingsFields::MaximumCellGrowthCoefficient, 1.5);
  idfObject.setString(Foundation_Kiva_SettingsFields::SimulationTimestep, "Hourly");

  openstudio::WorkspaceObject epFoundationKivaSettings = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_TRUE(model.getOptionalUniqueModelObject<openstudio::model::FoundationKivaSettings>());

  openstudio::model::FoundationKivaSettings foundationKivaSettings = model.getUniqueModelObject<openstudio::model::FoundationKivaSettings>();

  EXPECT_EQ(13u, foundationKivaSettings.numFields());
  EXPECT_EQ(1.731, foundationKivaSettings.soilConductivity());
  EXPECT_EQ(1842.3, foundationKivaSettings.soilDensity());
  EXPECT_EQ(418.7, foundationKivaSettings.soilSpecificHeat());
  EXPECT_EQ(0.9, foundationKivaSettings.groundSolarAbsorptivity());
  EXPECT_EQ(0.9, foundationKivaSettings.groundThermalAbsorptivity());
  EXPECT_EQ(0.03, foundationKivaSettings.groundSurfaceRoughness());
  EXPECT_EQ(40, foundationKivaSettings.farFieldWidth());
  EXPECT_EQ("ZeroFlux", foundationKivaSettings.deepGroundBoundaryCondition());
  EXPECT_EQ(40, foundationKivaSettings.deepGroundDepth().get());
  EXPECT_EQ(0.02, foundationKivaSettings.minimumCellDimension());
  EXPECT_EQ(1.5, foundationKivaSettings.maximumCellGrowthCoefficient());
  EXPECT_EQ("Hourly", foundationKivaSettings.simulationTimestep());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputTableSummaryReports) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::Output_Table_SummaryReports);
  IdfExtensibleGroup group1 = idfObject.pushExtensibleGroup();
  group1.setString(0, "LightingSummary");
  IdfExtensibleGroup group2 = idfObject.pushExtensibleGroup();
  group2.setString(0, "EquipmentSummary");
  IdfExtensibleGroup group3 = idfObject.pushExtensibleGroup();
  group3.setString(0, "HVACSizingSummary");

  openstudio::WorkspaceObject epOutputTableSummaryReports = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_TRUE(model.getOptionalUniqueModelObject<openstudio::model::OutputTableSummaryReports>());

  openstudio::model::OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<openstudio::model::OutputTableSummaryReports>();

  EXPECT_EQ(3, outputTableSummaryReports.numberofSummaryReports());
  std::vector<std::string> summaryReports = outputTableSummaryReports.summaryReports();
  EXPECT_EQ(summaryReports[0], "LightingSummary");
  EXPECT_EQ(summaryReports[1], "EquipmentSummary");
  EXPECT_EQ(summaryReports[2], "HVACSizingSummary");
}

TEST_F(EnergyPlusFixture, ReverseTranslator_FoundationKiva) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Material);
  idfObject1.setString(0, "Material 1");  // Name
  idfObject1.setString(1, "Smooth");
  idfObject1.setString(2, "0.012");
  idfObject1.setString(3, "3.2");
  idfObject1.setString(4, "2.5");
  idfObject1.setString(5, "2.04");
  idfObject1.setString(6, "0.8");
  idfObject1.setString(7, "0.6");
  idfObject1.setString(8, "0.6");

  openstudio::WorkspaceObject epMaterial = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Foundation_Kiva);
  idfObject2.setString(Foundation_KivaFields::Name, "Foundation Kiva 1");
  idfObject2.setString(Foundation_KivaFields::InteriorHorizontalInsulationMaterialName, "");
  idfObject2.setDouble(Foundation_KivaFields::InteriorHorizontalInsulationDepth, 0);
  idfObject2.setString(Foundation_KivaFields::InteriorHorizontalInsulationWidth, "");
  idfObject2.setString(Foundation_KivaFields::InteriorVerticalInsulationMaterialName, "");
  idfObject2.setString(Foundation_KivaFields::InteriorVerticalInsulationDepth, "");
  idfObject2.setString(Foundation_KivaFields::ExteriorHorizontalInsulationMaterialName, "");
  idfObject2.setDouble(Foundation_KivaFields::ExteriorHorizontalInsulationWidth, 0);
  idfObject2.setString(Foundation_KivaFields::ExteriorHorizontalInsulationDepth, "");
  idfObject2.setString(Foundation_KivaFields::ExteriorVerticalInsulationMaterialName, "");
  idfObject2.setString(Foundation_KivaFields::ExteriorVerticalInsulationDepth, "");
  idfObject2.setDouble(Foundation_KivaFields::WallHeightAboveGrade, 0.2032);
  idfObject2.setDouble(Foundation_KivaFields::WallDepthBelowSlab, 0.2032);
  idfObject2.setString(Foundation_KivaFields::FootingWallConstructionName, "");
  idfObject2.setString(Foundation_KivaFields::FootingMaterialName, "");
  idfObject2.setDouble(Foundation_KivaFields::FootingDepth, 0.3);
  IdfExtensibleGroup group1 = idfObject2.pushExtensibleGroup();  // custom block
  group1.setString(0, "Material 1");
  group1.setDouble(1, 0.5);
  group1.setDouble(2, 1);
  group1.setDouble(3, -1);

  openstudio::WorkspaceObject epFoundationKiva = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::BuildingSurface_Detailed);
  idfObject3.setString(BuildingSurface_DetailedFields::Name, "Surface 1");
  idfObject3.setString(BuildingSurface_DetailedFields::SurfaceType, "Floor");
  idfObject3.setString(BuildingSurface_DetailedFields::ConstructionName, "");
  idfObject3.setString(BuildingSurface_DetailedFields::ZoneName, "");
  idfObject3.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Foundation");
  idfObject3.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "Foundation Kiva 1");
  idfObject3.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
  idfObject3.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
  idfObject3.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
  idfObject3.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
  IdfExtensibleGroup group2 = idfObject3.pushExtensibleGroup();  // vertex 1
  group2.setDouble(0, 0);
  group2.setDouble(1, 0);
  group2.setDouble(2, 0);
  IdfExtensibleGroup group3 = idfObject3.pushExtensibleGroup();  // vertex 2
  group3.setDouble(0, 0);
  group3.setDouble(1, 6.81553519541936);
  group3.setDouble(2, 0);
  IdfExtensibleGroup group4 = idfObject3.pushExtensibleGroup();  // vertex 3
  group4.setDouble(0, 13.6310703908387);
  group4.setDouble(1, 6.81553519541936);
  group4.setDouble(2, 0);
  IdfExtensibleGroup group5 = idfObject3.pushExtensibleGroup();  // vertex 4
  group5.setDouble(0, 13.6310703908387);
  group5.setDouble(1, 0);
  group5.setDouble(2, 0);

  openstudio::WorkspaceObject epSurface = workspace.addObject(idfObject3).get();

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::SurfaceProperty_ExposedFoundationPerimeter);

  idfObject4.setString(SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName, "Surface 1");
  idfObject4.setString(SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterCalculationMethod, "TotalExposedPerimeter");
  idfObject4.setDouble(SurfaceProperty_ExposedFoundationPerimeterFields::TotalExposedPerimeter, 40.8932111725161);
  idfObject4.setDouble(SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction, 1);

  openstudio::WorkspaceObject epPerimeter = workspace.addObject(idfObject4).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<FoundationKiva> foundationKivas = model.getModelObjects<FoundationKiva>();
  ASSERT_EQ(1u, foundationKivas.size());
  FoundationKiva foundationKiva = foundationKivas[0];
  EXPECT_EQ(1, foundationKiva.numberofCustomBlocks());
  std::vector<CustomBlock> customBlocks = foundationKiva.customBlocks();
  EXPECT_EQ(customBlocks[0].material().name().get(), "Material 1");

  std::vector<Surface> surfaces = model.getModelObjects<Surface>();
  ASSERT_EQ(1u, surfaces.size());
  Surface surface = surfaces[0];
  EXPECT_EQ(surface.adjacentFoundation().get().name().get(), "Foundation Kiva 1");

  boost::optional<SurfacePropertyExposedFoundationPerimeter> surfacePropertyExposedFoundationPerimeter =
    surface.surfacePropertyExposedFoundationPerimeter();
  std::string surfaceName = surfacePropertyExposedFoundationPerimeter.get().surfaceName();
  EXPECT_EQ(surface.name().get(), surfaceName);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ScheduleFile) {
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/ScheduleFile/in.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);

  ReverseTranslator reverseTranslator;
  Model model = reverseTranslator.translateWorkspace(inWorkspace);
  // model.save( resourcesPath() / toPath("energyplus/ScheduleFile/in.osm"), true);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  // workspace.save( resourcesPath() / toPath("energyplus/ScheduleFile/in2.idf"), true);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_PerformancePrecisionTradeoffs) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::PerformancePrecisionTradeoffs);
  idfObject.setString(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "Yes");

  openstudio::WorkspaceObject epPerformancePrecisionTradeoffs = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  ASSERT_TRUE(model.getOptionalUniqueModelObject<openstudio::model::PerformancePrecisionTradeoffs>());

  openstudio::model::PerformancePrecisionTradeoffs performancePrecisionTradeoffs =
    model.getUniqueModelObject<openstudio::model::PerformancePrecisionTradeoffs>();
  EXPECT_FALSE(performancePrecisionTradeoffs.isUseCoilDirectSolutionsDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.useCoilDirectSolutions());
  EXPECT_TRUE(performancePrecisionTradeoffs.isZoneRadiantExchangeAlgorithmDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isOverrideModeDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxZoneTempDiffDefaulted());
  EXPECT_TRUE(performancePrecisionTradeoffs.isMaxAllowedDelTempDefaulted());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZonePropertyUserViewFactorsBySurfaceName) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idf_zone(openstudio::IddObjectType::Zone);
  idf_zone.setName("Thermal Zone 1");

  openstudio::WorkspaceObject epZone = workspace.addObject(idf_zone).get();

  openstudio::IdfObject idf_surface(openstudio::IddObjectType::BuildingSurface_Detailed);
  {
    idf_surface.setName("Surface 1");                                            // Name
    idf_surface.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");  // Surface Type
    idf_surface.setString(BuildingSurface_DetailedFields::ConstructionName, "");
    idf_surface.setString(BuildingSurface_DetailedFields::ZoneName, "Thermal Zone 1");
    idf_surface.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
    idf_surface.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
    idf_surface.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
    idf_surface.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
    idf_surface.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
    idf_surface.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
    IdfExtensibleGroup group2 = idf_surface.pushExtensibleGroup();  // vertex 1
    group2.setDouble(0, 0);
    group2.setDouble(1, 50);
    group2.setDouble(2, 4);
    IdfExtensibleGroup group3 = idf_surface.pushExtensibleGroup();  // vertex 2
    group3.setDouble(0, 0);
    group3.setDouble(1, 50);
    group3.setDouble(2, 0);
    IdfExtensibleGroup group4 = idf_surface.pushExtensibleGroup();  // vertex 3
    group4.setDouble(0, 0);
    group4.setDouble(1, 0);
    group4.setDouble(2, 0);
    IdfExtensibleGroup group5 = idf_surface.pushExtensibleGroup();  // vertex 4
    group5.setDouble(0, 0);
    group5.setDouble(1, 4);
    group5.setDouble(2, 0);
  }
  openstudio::WorkspaceObject epBuildingSurfaceDetailed = workspace.addObject(idf_surface).get();

  openstudio::IdfObject idf_surface2(openstudio::IddObjectType::BuildingSurface_Detailed);
  {
    idf_surface2.setName("Surface 2");                                            // Name
    idf_surface2.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");  // Surface Type
    idf_surface2.setString(BuildingSurface_DetailedFields::ConstructionName, "");
    idf_surface2.setString(BuildingSurface_DetailedFields::ZoneName, "Thermal Zone 1");
    idf_surface2.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
    idf_surface2.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
    idf_surface2.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
    idf_surface2.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
    idf_surface2.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
    idf_surface2.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
    IdfExtensibleGroup group2 = idf_surface2.pushExtensibleGroup();  // vertex 1
    group2.setDouble(0, 5);
    group2.setDouble(1, 50);
    group2.setDouble(2, 4);
    IdfExtensibleGroup group3 = idf_surface2.pushExtensibleGroup();  // vertex 2
    group3.setDouble(0, 0);
    group3.setDouble(1, 50);
    group3.setDouble(2, 0);
    IdfExtensibleGroup group4 = idf_surface2.pushExtensibleGroup();  // vertex 3
    group4.setDouble(0, 0);
    group4.setDouble(1, 0);
    group4.setDouble(2, 0);
    IdfExtensibleGroup group5 = idf_surface2.pushExtensibleGroup();  // vertex 4
    group5.setDouble(0, 0);
    group5.setDouble(1, 4);
    group5.setDouble(2, 0);
  }
  openstudio::WorkspaceObject epBuildingSurfaceDetailed2 = workspace.addObject(idf_surface2).get();

  openstudio::IdfObject idf_zoneProp(openstudio::IddObjectType::ZoneProperty_UserViewFactors_BySurfaceName);
  idf_zoneProp.setString(0, "Thermal Zone 1");  // Zone or ZoneList Name
  idf_zoneProp.setString(1, "Surface 1");       // From Surface 1
  idf_zoneProp.setString(2, "Surface 2");       // To Surface 2
  idf_zoneProp.setDouble(3, 0.25);              // View Factor 1

  openstudio::WorkspaceObject epZonePropertyUserViewFactorsBySurfaceName = workspace.addObject(idf_zoneProp).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ThermalZone> thermalZones = model.getModelObjects<ThermalZone>();
  ASSERT_EQ(1u, thermalZones.size());
  ThermalZone thermalZone = thermalZones[0];
  EXPECT_EQ(thermalZone.name().get(), "Thermal Zone 1 Thermal Zone");
  std::vector<Surface> surfaces = model.getModelObjects<Surface>();
  EXPECT_EQ(2u, surfaces.size());
  std::sort(surfaces.begin(), surfaces.end(), openstudio::WorkspaceObjectNameLess());
  EXPECT_EQ(surfaces[0].name().get(), "Surface 1");
  EXPECT_EQ(surfaces[1].name().get(), "Surface 2");
  ZonePropertyUserViewFactorsBySurfaceName zoneProp = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();
  EXPECT_EQ(1u, zoneProp.numberofViewFactors());
  std::vector<ViewFactor> viewFactors = zoneProp.viewFactors();
  ViewFactor viewFactor = viewFactors[0];
  EXPECT_EQ(zoneProp.thermalZone().name().get(), "Thermal Zone 1 Thermal Zone");
  EXPECT_EQ(viewFactor.fromSurface().name().get(), "Surface 1");
  EXPECT_EQ(viewFactor.toSurface().name().get(), "Surface 2");
  EXPECT_EQ(0.25, viewFactor.viewFactor());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZonePropertyUserViewFactorsBySurfaceName_SameSurface) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idf_zone(openstudio::IddObjectType::Zone);
  idf_zone.setName("Thermal Zone 1");

  openstudio::WorkspaceObject epZone = workspace.addObject(idf_zone).get();

  openstudio::IdfObject idf_surface(openstudio::IddObjectType::BuildingSurface_Detailed);
  {
    idf_surface.setName("Surface 1");                                            // Name
    idf_surface.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");  // Surface Type
    idf_surface.setString(BuildingSurface_DetailedFields::ConstructionName, "");
    idf_surface.setString(BuildingSurface_DetailedFields::ZoneName, "Thermal Zone 1");
    idf_surface.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
    idf_surface.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
    idf_surface.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
    idf_surface.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
    idf_surface.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
    idf_surface.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
    IdfExtensibleGroup group2 = idf_surface.pushExtensibleGroup();  // vertex 1
    group2.setDouble(0, 0);
    group2.setDouble(1, 50);
    group2.setDouble(2, 4);
    IdfExtensibleGroup group3 = idf_surface.pushExtensibleGroup();  // vertex 2
    group3.setDouble(0, 0);
    group3.setDouble(1, 50);
    group3.setDouble(2, 0);
    IdfExtensibleGroup group4 = idf_surface.pushExtensibleGroup();  // vertex 3
    group4.setDouble(0, 0);
    group4.setDouble(1, 0);
    group4.setDouble(2, 0);
    IdfExtensibleGroup group5 = idf_surface.pushExtensibleGroup();  // vertex 4
    group5.setDouble(0, 0);
    group5.setDouble(1, 4);
    group5.setDouble(2, 0);
  }
  openstudio::WorkspaceObject epBuildingSurfaceDetailed = workspace.addObject(idf_surface).get();

  openstudio::IdfObject idf_zoneProp(openstudio::IddObjectType::ZoneProperty_UserViewFactors_BySurfaceName);
  idf_zoneProp.setString(0, "Thermal Zone 1");  // Zone or ZoneList Name
  idf_zoneProp.setString(1, "Surface 1");       // From Surface 1
  idf_zoneProp.setString(2, "Surface 1");       // To Surface 1
  idf_zoneProp.setDouble(3, 0.25);              // View Factor 1

  openstudio::WorkspaceObject epZonePropertyUserViewFactorsBySurfaceName = workspace.addObject(idf_zoneProp).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ThermalZone> thermalZones = model.getModelObjects<ThermalZone>();
  ASSERT_EQ(1u, thermalZones.size());
  ThermalZone thermalZone = thermalZones[0];
  EXPECT_EQ(thermalZone.name().get(), "Thermal Zone 1 Thermal Zone");
  std::vector<Surface> surfaces = model.getModelObjects<Surface>();
  ASSERT_EQ(1u, surfaces.size());
  Surface surface = surfaces[0];
  EXPECT_EQ(surface.name().get(), "Surface 1");
  ZonePropertyUserViewFactorsBySurfaceName zoneProp = thermalZone.getZonePropertyUserViewFactorsBySurfaceName();
  EXPECT_EQ(zoneProp.thermalZone().name().get(), "Thermal Zone 1 Thermal Zone");

  // We allow toSurface to be equal to fromSurface
  EXPECT_EQ(1u, zoneProp.numberofViewFactors());
  std::vector<ViewFactor> viewFactors = zoneProp.viewFactors();
  ViewFactor viewFactor = viewFactors[0];
  EXPECT_EQ(viewFactor.fromSurface().name().get(), "Surface 1");
  EXPECT_EQ(viewFactor.toSurface().name().get(), "Surface 1");
  EXPECT_EQ(0.25, viewFactor.viewFactor());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneList) {
  ReverseTranslator reverseTranslator;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_zone1 = w.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(_i_zone1);
  EXPECT_TRUE(_i_zone1->setName("Zone1"));
  OptionalWorkspaceObject _i_zone2 = w.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(_i_zone2);
  EXPECT_TRUE(_i_zone2->setName("Zone2"));
  OptionalWorkspaceObject _i_zoneList1 = w.addObject(IdfObject(IddObjectType::ZoneList));
  ASSERT_TRUE(_i_zoneList1);
  EXPECT_TRUE(_i_zoneList1->setName("ZoneList1 for Zone1 and Zone2"));

  WorkspaceExtensibleGroup eg1 = _i_zoneList1->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(eg1.setPointer(ZoneListExtensibleFields::ZoneName, _i_zone1->handle()));
  WorkspaceExtensibleGroup eg2 = _i_zoneList1->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(eg2.setPointer(ZoneListExtensibleFields::ZoneName, _i_zone2->handle()));

  // To avoid other warnings, we add required objects
  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  {
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_TRUE(reverseTranslator.errors().empty());
    EXPECT_TRUE(reverseTranslator.warnings().empty());

    std::vector<openstudio::model::ThermalZone> zones = model.getModelObjects<openstudio::model::ThermalZone>();
    ASSERT_EQ(static_cast<unsigned>(2), model.getModelObjects<openstudio::model::ThermalZone>().size());
    boost::optional<openstudio::model::ThermalZone> _zone1 =
      model.getModelObjectByName<openstudio::model::ThermalZone>(_i_zone1->nameString() + " Thermal Zone");
    ASSERT_TRUE(_zone1);
    boost::optional<openstudio::model::ThermalZone> _zone2 =
      model.getModelObjectByName<openstudio::model::ThermalZone>(_i_zone2->nameString() + " Thermal Zone");
    ASSERT_TRUE(_zone2);

    ASSERT_EQ(static_cast<unsigned>(2), model.getModelObjects<openstudio::model::Space>().size());
    boost::optional<openstudio::model::Space> _space1 = model.getModelObjectByName<openstudio::model::Space>(_i_zone1->nameString());
    ASSERT_TRUE(_zone1);
    boost::optional<openstudio::model::Space> _space2 = model.getModelObjectByName<openstudio::model::Space>(_i_zone2->nameString());
    ASSERT_TRUE(_zone2);

    ASSERT_EQ(static_cast<unsigned>(1), model.getModelObjects<openstudio::model::SpaceType>().size());
    openstudio::model::SpaceType spaceType1 = model.getModelObjects<openstudio::model::SpaceType>()[0];
    EXPECT_EQ(spaceType1.nameString(), _i_zoneList1->nameString());

    ASSERT_TRUE(_space1->thermalZone());
    EXPECT_EQ(_zone1.get(), _space1->thermalZone().get());
    ASSERT_TRUE(_space2->thermalZone());
    EXPECT_EQ(_zone2.get(), _space2->thermalZone().get());

    ASSERT_TRUE(_space1->spaceType());
    EXPECT_EQ(spaceType1, _space1->spaceType().get());
    ASSERT_TRUE(_space2->spaceType());
    EXPECT_EQ(spaceType1, _space2->spaceType().get());
  }

  // We add another ZoneList, that references only Space2
  OptionalWorkspaceObject _i_zoneList2 = w.addObject(IdfObject(IddObjectType::ZoneList));
  ASSERT_TRUE(_i_zoneList2);
  EXPECT_TRUE(_i_zoneList2->setName("ZoneList2 for Zone2"));

  WorkspaceExtensibleGroup eg3 = _i_zoneList2->pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(eg3.setPointer(ZoneListExtensibleFields::ZoneName, _i_zone2->handle()));

  // Translate again
  {
    ASSERT_NO_THROW(reverseTranslator.translateWorkspace(w));
    Model model = reverseTranslator.translateWorkspace(w);
    EXPECT_TRUE(reverseTranslator.errors().empty());
    // This time we should have gotten one warning that the SpaceType is overriden
    EXPECT_EQ(1u, reverseTranslator.warnings().size());
    EXPECT_EQ("Overriding previously assigned SpaceType for Space 'Zone2'", reverseTranslator.warnings()[0].logMessage());

    std::vector<openstudio::model::ThermalZone> zones = model.getModelObjects<openstudio::model::ThermalZone>();
    ASSERT_EQ(static_cast<unsigned>(2), model.getModelObjects<openstudio::model::ThermalZone>().size());
    boost::optional<openstudio::model::ThermalZone> _zone1 =
      model.getModelObjectByName<openstudio::model::ThermalZone>(_i_zone1->nameString() + " Thermal Zone");
    ASSERT_TRUE(_zone1);
    boost::optional<openstudio::model::ThermalZone> _zone2 =
      model.getModelObjectByName<openstudio::model::ThermalZone>(_i_zone2->nameString() + " Thermal Zone");
    ASSERT_TRUE(_zone2);

    ASSERT_EQ(static_cast<unsigned>(2), model.getModelObjects<openstudio::model::Space>().size());
    boost::optional<openstudio::model::Space> _space1 = model.getModelObjectByName<openstudio::model::Space>(_i_zone1->nameString());
    ASSERT_TRUE(_zone1);
    boost::optional<openstudio::model::Space> _space2 = model.getModelObjectByName<openstudio::model::Space>(_i_zone2->nameString());
    ASSERT_TRUE(_zone2);

    ASSERT_EQ(static_cast<unsigned>(2), model.getModelObjects<openstudio::model::SpaceType>().size());
    boost::optional<openstudio::model::SpaceType> _spaceType1 = model.getModelObjectByName<openstudio::model::SpaceType>(_i_zoneList1->nameString());
    ASSERT_TRUE(_spaceType1);
    boost::optional<openstudio::model::SpaceType> _spaceType2 = model.getModelObjectByName<openstudio::model::SpaceType>(_i_zoneList2->nameString());
    ASSERT_TRUE(_spaceType2);

    ASSERT_TRUE(_space1->thermalZone());
    EXPECT_EQ(_zone1.get(), _space1->thermalZone().get());
    ASSERT_TRUE(_space2->thermalZone());
    EXPECT_EQ(_zone2.get(), _space2->thermalZone().get());

    ASSERT_TRUE(_space1->spaceType());
    EXPECT_EQ(_spaceType1, _space1->spaceType().get());

    // Space2 is in two zoneList, but it'll keep the last ZoneList that referenced it
    ASSERT_TRUE(_space2->spaceType());
    EXPECT_EQ(_spaceType2, _space2->spaceType().get())
      << "Expected space2 to have a SpaceType '" << _spaceType2->nameString() << "', but it has '" << _space2->spaceType()->nameString() << "'";
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_WindowMaterialGlazing) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_Glazing);
  idfObject.setName("CLEAR 6MM");
  idfObject.setString(WindowMaterial_GlazingFields::OpticalDataType, "SpectralAverage");

  openstudio::WorkspaceObject epWindowMaterialGlazing = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<StandardGlazing> standardGlazings = model.getModelObjects<StandardGlazing>();
  ASSERT_EQ(1u, standardGlazings.size());
  StandardGlazing standardGlazing = standardGlazings[0];
  EXPECT_EQ(standardGlazing.name().get(), "CLEAR 6MM");
  EXPECT_EQ(standardGlazing.opticalDataType(), "SpectralAverage");
  EXPECT_FALSE(standardGlazing.windowGlassSpectralDataSet());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_MaterialPropertyGlazingSpectralData) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::MaterialProperty_GlazingSpectralData);

  openstudio::WorkspaceObject epSpectralData = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<MaterialPropertyGlazingSpectralData> spectralDatas = model.getModelObjects<MaterialPropertyGlazingSpectralData>();
  ASSERT_EQ(1u, spectralDatas.size());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_WindowMaterialGlazing_2) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::WindowMaterial_Glazing);
  idfObject1.setName("CLEAR 8MM");

  openstudio::WorkspaceObject epWindowMaterialGlazing = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::MaterialProperty_GlazingSpectralData);

  openstudio::WorkspaceObject epMaterialPropertyGlazingSpectralData = workspace.addObject(idfObject2).get();

  EXPECT_TRUE(
    epWindowMaterialGlazing.setPointer(WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, epMaterialPropertyGlazingSpectralData.handle()));

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<StandardGlazing> standardGlazings = model.getModelObjects<StandardGlazing>();
  ASSERT_EQ(1u, standardGlazings.size());
  StandardGlazing standardGlazing = standardGlazings[0];

  std::vector<MaterialPropertyGlazingSpectralData> spectralDatas = model.getModelObjects<MaterialPropertyGlazingSpectralData>();
  ASSERT_EQ(1u, spectralDatas.size());
  MaterialPropertyGlazingSpectralData spectralData = spectralDatas[0];

  EXPECT_EQ(standardGlazing.name().get(), "CLEAR 8MM");
  EXPECT_EQ(standardGlazing.opticalDataType(), "Spectral");
  EXPECT_TRUE(standardGlazing.windowGlassSpectralDataSet());
  EXPECT_TRUE(standardGlazing.windowGlassSpectralDataSetName());
  EXPECT_EQ(standardGlazing.windowGlassSpectralDataSet().get().name().get(), spectralData.name().get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_DaylightingControl_3216) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Daylighting_ReferencePoint);
  idfObject3.setName("Reference Point 1");
  idfObject3.setDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint, 15);
  idfObject3.setDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint, 16.05);
  idfObject3.setDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint, 0);

  openstudio::WorkspaceObject epDaylightingReferencePoint1 = workspace.addObject(idfObject3).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Daylighting_Controls);
  idfObject1.setDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis, 180.0);
  IdfExtensibleGroup group1 = idfObject1.pushExtensibleGroup();
  group1.setString(0, "Reference Point 1");
  group1.setDouble(1, 1);
  group1.setDouble(2, 500);

  openstudio::WorkspaceObject epDaylightingControls = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Zone);

  openstudio::WorkspaceObject epZone = workspace.addObject(idfObject2).get();

  EXPECT_TRUE(epDaylightingControls.setPointer(Daylighting_ControlsFields::ZoneName, epZone.handle()));
  EXPECT_TRUE(epDaylightingReferencePoint1.setPointer(Daylighting_ReferencePointFields::ZoneName, epZone.handle()));

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<DaylightingControl> daylightingControls = model.getModelObjects<DaylightingControl>();
  ASSERT_EQ(1u, daylightingControls.size());
  DaylightingControl daylightingControl = daylightingControls[0];
  EXPECT_EQ(daylightingControl.name().get(), "Reference Point 1");
  EXPECT_EQ(daylightingControl.phiRotationAroundZAxis(), 180.0);
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SurfaceControlMovableInsulation) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  // surface
  openstudio::IdfObject idfObject1(openstudio::IddObjectType::BuildingSurface_Detailed);
  idfObject1.setString(BuildingSurface_DetailedFields::Name, "Surface 1");
  idfObject1.setString(BuildingSurface_DetailedFields::SurfaceType, "Floor");
  idfObject1.setString(BuildingSurface_DetailedFields::ConstructionName, "");
  idfObject1.setString(BuildingSurface_DetailedFields::ZoneName, "");
  idfObject1.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Foundation");
  idfObject1.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "Foundation Kiva 1");
  idfObject1.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
  idfObject1.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
  idfObject1.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
  idfObject1.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
  IdfExtensibleGroup group2 = idfObject1.pushExtensibleGroup();  // vertex 1
  group2.setDouble(0, 0);
  group2.setDouble(1, 0);
  group2.setDouble(2, 0);
  IdfExtensibleGroup group3 = idfObject1.pushExtensibleGroup();  // vertex 2
  group3.setDouble(0, 0);
  group3.setDouble(1, 6.81553519541936);
  group3.setDouble(2, 0);
  IdfExtensibleGroup group4 = idfObject1.pushExtensibleGroup();  // vertex 3
  group4.setDouble(0, 13.6310703908387);
  group4.setDouble(1, 6.81553519541936);
  group4.setDouble(2, 0);
  IdfExtensibleGroup group5 = idfObject1.pushExtensibleGroup();  // vertex 4
  group5.setDouble(0, 13.6310703908387);
  group5.setDouble(1, 0);
  group5.setDouble(2, 0);

  openstudio::WorkspaceObject epSurface = workspace.addObject(idfObject1).get();

  // material
  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Material);
  idfObject2.setString(0, "Material 1");  // Name
  idfObject2.setString(1, "Smooth");
  idfObject2.setString(2, "0.012");
  idfObject2.setString(3, "3.2");
  idfObject2.setString(4, "2.5");
  idfObject2.setString(5, "2.04");
  idfObject2.setString(6, "0.8");
  idfObject2.setString(7, "0.6");
  idfObject2.setString(8, "0.6");

  openstudio::WorkspaceObject epMaterial = workspace.addObject(idfObject2).get();

  // schedule
  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Schedule_Constant);
  idfObject3.setString(0, "Schedule 1");
  idfObject3.setString(1, "0.5");

  openstudio::WorkspaceObject epSchedule = workspace.addObject(idfObject3).get();

  // surface control
  openstudio::IdfObject idfObject4(openstudio::IddObjectType::SurfaceControl_MovableInsulation);
  idfObject4.setString(SurfaceControl_MovableInsulationFields::InsulationType, "Inside");
  idfObject4.setString(SurfaceControl_MovableInsulationFields::SurfaceName, "Surface 1");
  idfObject4.setString(SurfaceControl_MovableInsulationFields::MaterialName, "Material 1");
  idfObject4.setString(SurfaceControl_MovableInsulationFields::ScheduleName, "Schedule 1");

  openstudio::WorkspaceObject epMovableInsulation = workspace.addObject(idfObject4).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<SurfaceControlMovableInsulation> mis = model.getModelObjects<SurfaceControlMovableInsulation>();
  ASSERT_EQ(1u, mis.size());
  SurfaceControlMovableInsulation mi = mis[0];
  EXPECT_EQ("Inside", mi.insulationType());

  std::vector<Surface> surfaces = model.getModelObjects<Surface>();
  ASSERT_EQ(1u, surfaces.size());
  Surface surface = surfaces[0];
  EXPECT_EQ(mi.surface().name().get(), surface.name().get());

  std::vector<Material> materials = model.getModelObjects<Material>();
  ASSERT_EQ(1u, materials.size());
  Material material = materials[0];
  EXPECT_EQ(mi.material().name().get(), material.name().get());

  std::vector<Schedule> schedules = model.getModelObjects<Schedule>();
  ASSERT_EQ(2u, schedules.size());  // Schedule Constant 1 and Schedule 1
}

TEST_F(EnergyPlusFixture, ReverseTranslator_CoilCoolingDX) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject totalCoolingCapacityModifierFunctionofTemperatureCurveName(openstudio::IddObjectType::Curve_Biquadratic);
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Name,
                                                                       "Total Cooling Capacity Modifier Function of Temperature");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient1Constant, "0.766956");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient2x, "0.0107756");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient3x_POW_2, "-0.0000414703");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient4y, "0.00134961");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient5y_POW_2, "-0.000261144");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient6x_TIMES_y, "0.000457488");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MinimumValueofx, "17.0");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MaximumValueofx, "22.0");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MinimumValueofy, "13.0");
  totalCoolingCapacityModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MaximumValueofy, "46.0");

  openstudio::WorkspaceObject epTotalCoolingCapacityModifierFunctionofTemperatureCurveName =
    workspace.addObject(totalCoolingCapacityModifierFunctionofTemperatureCurveName).get();

  openstudio::IdfObject totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName(openstudio::IddObjectType::Curve_Quadratic);
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Name,
                                                                           "Total Cooling Capacity Modifier Function of Air Flow Fraction");
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Coefficient1Constant, "0.8");
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Coefficient2x, "0.2");
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Coefficient3x_POW_2, "0.0");
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::MinimumValueofx, "0.5");
  totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::MaximumValueofx, "1.5");

  openstudio::WorkspaceObject epTotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName =
    workspace.addObject(totalCoolingCapacityModifierFunctionofAirFlowFractionCurveName).get();

  openstudio::IdfObject energyInputRatioModifierFunctionofTemperatureCurveName(openstudio::IddObjectType::Curve_Biquadratic);
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Name,
                                                                   "Energy Input Ratio Modifier Function of Temperature");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient1Constant, "0.297145");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient2x, "0.0430933");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient3x_POW_2, "-0.000748766");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient4y, "0.00597727");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient5y_POW_2, "-0.000482112");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient6x_TIMES_y, "0.000956448");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MinimumValueofx, "17.0");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MaximumValueofx, "22.0");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MinimumValueofy, "13.0");
  energyInputRatioModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MaximumValueofy, "46.0");

  openstudio::WorkspaceObject epEnergyInputRatioModifierFunctionofTemperatureCurveName =
    workspace.addObject(energyInputRatioModifierFunctionofTemperatureCurveName).get();

  openstudio::IdfObject energyInputRatioModifierFunctionofAirFlowFractionCurveName(openstudio::IddObjectType::Curve_Quadratic);
  energyInputRatioModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Name,
                                                                       "Energy Input Ratio Modifier Function of Air Flow Fraction");
  energyInputRatioModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Coefficient1Constant, "1.156");
  energyInputRatioModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Coefficient2x, "-0.1816");
  energyInputRatioModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::Coefficient3x_POW_2, "0.0256");
  energyInputRatioModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::MinimumValueofx, "0.5");
  energyInputRatioModifierFunctionofAirFlowFractionCurveName.setString(Curve_BiquadraticFields::MaximumValueofx, "1.5");

  openstudio::WorkspaceObject epEnergyInputRatioModifierFunctionofAirFlowFractionCurveName =
    workspace.addObject(energyInputRatioModifierFunctionofAirFlowFractionCurveName).get();

  openstudio::IdfObject partLoadFractionCorrelationCurveName(openstudio::IddObjectType::Curve_Quadratic);
  partLoadFractionCorrelationCurveName.setString(Curve_BiquadraticFields::Name, "Part Load Fraction Correlation");
  partLoadFractionCorrelationCurveName.setString(Curve_BiquadraticFields::Coefficient1Constant, "0.75");
  partLoadFractionCorrelationCurveName.setString(Curve_BiquadraticFields::Coefficient2x, "0.25");
  partLoadFractionCorrelationCurveName.setString(Curve_BiquadraticFields::Coefficient3x_POW_2, "0.0");
  partLoadFractionCorrelationCurveName.setString(Curve_BiquadraticFields::MinimumValueofx, "0.0");
  partLoadFractionCorrelationCurveName.setString(Curve_BiquadraticFields::MaximumValueofx, "1.0");

  openstudio::WorkspaceObject epPartLoadFractionCorrelationCurveName = workspace.addObject(partLoadFractionCorrelationCurveName).get();

  openstudio::IdfObject wasteHeatModifierFunctionofTemperatureCurveName(openstudio::IddObjectType::Curve_Biquadratic);
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Name, "Waste Heat Modifier Function of Temperature");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient1Constant, "1");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient2x, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient3x_POW_2, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient4y, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient5y_POW_2, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::Coefficient6x_TIMES_y, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MinimumValueofx, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MaximumValueofx, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MinimumValueofy, "0.0");
  wasteHeatModifierFunctionofTemperatureCurveName.setString(Curve_BiquadraticFields::MaximumValueofy, "0.0");

  openstudio::WorkspaceObject epWasteHeatModifierFunctionofTemperatureCurveName =
    workspace.addObject(wasteHeatModifierFunctionofTemperatureCurveName).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_Speed);
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::Name, "Coil Cooling DX Curve Fit Speed 1");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction, "1.0");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction, "1.0");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction, "1.0");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, "Autosize");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP, "3.0");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea, "1.0");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate, "773.3");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction, "1.0");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness, "0.9");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName,
                       "Total Cooling Capacity Modifier Function of Temperature");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName,
                       "Total Cooling Capacity Modifier Function of Air Flow Fraction");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName,
                       "Energy Input Ratio Modifier Function of Temperature");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName,
                       "Energy Input Ratio Modifier Function of Air Flow Fraction");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName, "Part Load Fraction Correlation");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurveName,
                       "Waste Heat Modifier Function of Temperature");
  idfObject1.setString(Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput, "0.2");

  openstudio::WorkspaceObject epSpeed = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode);
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::Name, "Coil Cooling DX Curve Fit Operating Mode 1");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity, "Autosize");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate, "Autosize");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate, "Autosize");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::MaximumCyclingRate, "0.0");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, "0.0");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::LatentCapacityTimeConstant, "0.0");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalTimeforCondensateRemovaltoBegin, "0.0");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "No");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::CondenserType, "AirCooled");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower, "Autosize");
  idfObject2.setString(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalSpeedNumber, "1");
  IdfExtensibleGroup group1 = idfObject2.pushExtensibleGroup();  // speed
  group1.setString(0, "Coil Cooling DX Curve Fit Speed 1");

  openstudio::WorkspaceObject epOperatingMode = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_Performance);
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::Name, "Coil Cooling DX Curve Fit Performance 1");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacity, "0.0");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, "-25.0");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, "10.0");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::UnitInternalStaticAirPressure, "773.3");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod, "Discrete");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterCapacity, "0.0");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterSetpointTemperature, "2.0");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingScheduleName, "Always On Discrete");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType, "Electricity");
  idfObject3.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode, "Coil Cooling DX Curve Fit Operating Mode 1");

  openstudio::WorkspaceObject epPerformance = workspace.addObject(idfObject3).get();

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::Coil_Cooling_DX);
  idfObject4.setString(Coil_Cooling_DXFields::Name, "Coil Cooling DX 1");
  idfObject4.setString(Coil_Cooling_DXFields::PerformanceObjectName, "Coil Cooling DX Curve Fit Performance 1");

  openstudio::WorkspaceObject epDX = workspace.addObject(idfObject4).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<CurveBiquadratic> biquadratics = model.getModelObjects<CurveBiquadratic>();
  ASSERT_EQ(3u, biquadratics.size());

  std::vector<CurveQuadratic> quadratics = model.getModelObjects<CurveQuadratic>();
  ASSERT_EQ(3u, quadratics.size());

  std::vector<CoilCoolingDXCurveFitSpeed> speeds = model.getModelObjects<CoilCoolingDXCurveFitSpeed>();
  ASSERT_EQ(1u, speeds.size());
  CoilCoolingDXCurveFitSpeed speed = speeds[0];
  EXPECT_EQ(1u, speed.coilCoolingDXCurveFitOperatingModes().size());

  std::vector<CoilCoolingDXCurveFitOperatingMode> operatingModes = model.getModelObjects<CoilCoolingDXCurveFitOperatingMode>();
  ASSERT_EQ(1u, operatingModes.size());
  CoilCoolingDXCurveFitOperatingMode operatingMode = operatingModes[0];
  ASSERT_EQ(1u, operatingMode.speeds().size());
  EXPECT_EQ(1u, operatingMode.coilCoolingDXCurveFitPerformances().size());

  std::vector<CoilCoolingDXCurveFitPerformance> performances = model.getModelObjects<CoilCoolingDXCurveFitPerformance>();
  ASSERT_EQ(1u, performances.size());
  CoilCoolingDXCurveFitPerformance performance = performances[0];
  EXPECT_EQ(operatingMode, performance.baseOperatingMode());
  EXPECT_FALSE(performance.alternativeOperatingMode1());
  EXPECT_FALSE(performance.alternativeOperatingMode2());
  EXPECT_EQ(1u, performance.coilCoolingDXs().size());

  std::vector<CoilCoolingDX> dxs = model.getModelObjects<CoilCoolingDX>();
  ASSERT_EQ(1u, dxs.size());
  CoilCoolingDX dx = dxs[0];
  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_FALSE(dx.condenserZone());

  std::vector<Schedule> schedules = model.getModelObjects<Schedule>();
  ASSERT_EQ(1u, schedules.size());
}
