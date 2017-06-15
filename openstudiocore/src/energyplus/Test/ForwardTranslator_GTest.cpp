/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/SiteGroundReflectance.hpp"
#include "../../model/SiteGroundReflectance_Impl.hpp"
#include "../../model/SiteGroundTemperatureBuildingSurface.hpp"
#include "../../model/SiteGroundTemperatureBuildingSurface_Impl.hpp"
#include "../../model/SiteGroundTemperatureDeep.hpp"
#include "../../model/SiteGroundTemperatureDeep_Impl.hpp"
#include "../../model/SiteGroundTemperatureFCfactorMethod.hpp"
#include "../../model/SiteGroundTemperatureFCfactorMethod_Impl.hpp"
#include "../../model/SiteGroundTemperatureShallow.hpp"
#include "../../model/SiteGroundTemperatureShallow_Impl.hpp"
#include "../../model/SiteWaterMainsTemperature.hpp"
#include "../../model/SiteWaterMainsTemperature_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Lights.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/Construction.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"
#include "../../model/ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../../model/ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/ZoneCapacitanceMultiplier_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <QThread>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/*
TEST_F(EnergyPlusFixture,ForwardTranslator_LightShelf)
{
  ASSERT_TRUE(true);
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Building)
{
  openstudio::model::Model model;
  openstudio::model::Building building(model);
  ForwardTranslator forwardTranslator(model);
  OptionalWorkspace outWorkspace = forwardTranslator.convert();
  ASSERT_TRUE(outWorkspace);
  EXPECT_EQ(static_cast<unsigned>(1), outWorkspace->getObjectsByType(IddObjectType::Building).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Zone)
{
  openstudio::model::Model model;
  openstudio::model::Zone zone(model);
  openstudio::model::Lights lights(model);
  lights.addToZone(zone);

  ForwardTranslator forwardTranslator(model);
  OptionalWorkspace outWorkspace = forwardTranslator.convert();
  ASSERT_TRUE(outWorkspace);
  ASSERT_EQ(static_cast<unsigned>(1), outWorkspace->getObjectsByType(IddObjectType::Zone).size());
  Handle zoneHandle = outWorkspace->getObjectsByType(IddObjectType::Zone)[0].handle();
  ASSERT_EQ(static_cast<unsigned>(1), outWorkspace->getObjectsByType(IddObjectType::Lights).size());
  WorkspaceObject lightsObject = outWorkspace->getObjectsByType(IddObjectType::Lights)[0];
  ASSERT_TRUE(lightsObject.getTarget(openstudio::LightsFields::ZoneorZoneListName));
  EXPECT_TRUE(zoneHandle == lightsObject.getTarget(openstudio::LightsFields::ZoneorZoneListName)->handle());
}
*/
TEST_F(EnergyPlusFixture,ForwardTranslator_ExampleModel) {
  Model model = exampleModel();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Version>()) << "Example model does not include a Version object.";
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Version).size());

  model.save(toPath("./example.osm"), true);
  workspace.save(toPath("./example.idf"), true);
}


TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateAirLoopHVAC) {
  openstudio::model::Model model;
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Version>()) << "Blank model does not include a Version object.";

  ThermalZone zone1(model);
  ThermalZone zone2(model);
  ThermalZone zone3(model);

  openstudio::model::AirLoopHVAC airLoopHVAC(model);
  airLoopHVAC.addBranchForZone(zone1, boost::none);
  airLoopHVAC.addBranchForZone(zone2, boost::none);
  airLoopHVAC.addBranchForZone(zone3, boost::none);

  ForwardTranslator trans;

  openstudio::Workspace workspace = trans.translateModel(model);
  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Version).size());

  ASSERT_NE(unsigned(0),workspace.objects().size());

  openstudio::path outDir = resourcesPath() / openstudio::toPath("airLoopHVAC.idf");
  openstudio::filesystem::ofstream ofs(outDir);
  workspace.toIdfFile().print(ofs);
  ofs.close();
}
TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateCoolingCoil)
{
  Model model;
  ScheduleCompact scheduleCompact(model);
  scheduleCompact.setName("Daytime Ventilation For Cooling Coil Test");
  scheduleCompact.setString(1,"Fraction");
  scheduleCompact.setString(2,"Through: 12/31");
  scheduleCompact.setString(3,"For: Weekdays SummerDesignDay");
  scheduleCompact.setString(4,"Until: 08:00");
  scheduleCompact.setString(5,"0.0");
  scheduleCompact.setString(6,"Until: 18:00");
  scheduleCompact.setString(7,"1.0");
  scheduleCompact.setString(8,"Until: 24:00");
  scheduleCompact.setString(9,"0.0");
  scheduleCompact.setString(10,"For: Weekends WinterDesignDay");
  scheduleCompact.setString(11,"Until: 10:00");
  scheduleCompact.setString(12,"0.0");
  scheduleCompact.setString(13,"Until: 16:00");
  scheduleCompact.setString(14,"1.0");
  scheduleCompact.setString(15,"Until: 24:00");
  scheduleCompact.setString(16,"0.0");
  scheduleCompact.setString(17,"For: Holidays AllOtherDays");
  scheduleCompact.setString(18,"Until: 24:00");
  scheduleCompact.setString(19,"0.0");

  CurveBiquadratic ccFofT(model);
  CurveBiquadratic eirFofT(model);
  CurveQuadratic ccFofF(model);
  CurveQuadratic eirFofF(model);
  CurveQuadratic plf(model);

  ccFofT.setCoefficient1Constant(1.0);
  ccFofT.setCoefficient2x(2.0);
  ccFofT.setCoefficient3xPOW2(3.0);
  ccFofT.setCoefficient4y(4.0);
  ccFofT.setCoefficient5yPOW2(5.0);
  ccFofT.setCoefficient6xTIMESY(6.0);
  ccFofT.setMinimumValueofx(-10.0);
  ccFofT.setMaximumValueofx(100.03);
  ccFofT.setMinimumValueofy(-99999);
  ccFofT.setMaximumValueofy(100232);
  ccFofT.setMinimumCurveOutput(-1000);
  ccFofT.setMaximumCurveOutput(99999);
  ccFofT.setInputUnitTypeforX("Temperature");
  ccFofT.setInputUnitTypeforY("MassFlow");

  eirFofT.setCoefficient1Constant(10.0);
  // eirFofT.setCoefficient2x(20.0); check default
  eirFofT.setCoefficient3xPOW2(30.0);
  eirFofT.setCoefficient4y(40.0);
  eirFofT.setCoefficient5yPOW2(50.0);
  eirFofT.setCoefficient6xTIMESY(60.0);
  eirFofT.setInputUnitTypeforX("VolumetricFlow");
  eirFofT.setInputUnitTypeforY("Power");

  plf.setCoefficient1Constant(5.50);
  plf.setCoefficient2x(55.50);
  plf.setCoefficient3xPOW2(23.50);

  CoilCoolingDXSingleSpeed cool(model,
                                scheduleCompact,
                                ccFofT,
                                ccFofF,
                                eirFofT,
                                eirFofF,
                                plf);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Version).size());
  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed).size());
  EXPECT_EQ(1u,workspace.getObjectsByType(IddObjectType::Schedule_Compact).size());
  EXPECT_EQ(2u,workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(3u,workspace.getObjectsByType(IddObjectType::Curve_Quadratic).size());

  path outDir = resourcesPath() / openstudio::toPath("CoolingCoilDXSingleSpeed.idf");
  openstudio::filesystem::ofstream ofs(outDir);
  workspace.toIdfFile().print(ofs);
  ofs.close();

  openstudio::path idfPath = resourcesPath() / openstudio::toPath("CoolingCoilDXSingleSpeed.idf");
  OptionalIdfFile idfFile = IdfFile::load(idfPath, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace inWorkspace(*idfFile);

  // Get the Model by calling the translator
  ReverseTranslator trans2;
  ASSERT_NO_THROW(trans2.translateWorkspace(inWorkspace));
  Model model2 =  trans2.translateWorkspace(inWorkspace);
}

TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateScheduleCompact) {
  openstudio::model::Model model;

  openstudio::model::ScheduleCompact scheduleCompact(model);

  scheduleCompact.setName("Daytime Ventilation");

  scheduleCompact.setString(2,"Fraction");
  scheduleCompact.setString(3,"Through: 12/31");
  scheduleCompact.setString(4,"For: Weekdays SummerDesignDay");
  scheduleCompact.setString(5,"Until: 08:00");
  scheduleCompact.setString(6,"0.0");
  scheduleCompact.setString(7,"Until: 18:00");
  scheduleCompact.setString(8,"1.0");
  scheduleCompact.setString(9,"Until: 24:00");
  scheduleCompact.setString(10,"0.0");
  scheduleCompact.setString(11,"For: Weekends WinterDesignDay");
  scheduleCompact.setString(12,"Until: 10:00");
  scheduleCompact.setString(13,"0.0");
  scheduleCompact.setString(14,"Until: 16:00");
  scheduleCompact.setString(15,"1.0");
  scheduleCompact.setString(16,"Until: 24:00");
  scheduleCompact.setString(17,"0.0");
  scheduleCompact.setString(18,"For: Holidays AllOtherDays");
  scheduleCompact.setString(19,"Until: 24:00");
  scheduleCompact.setString(20,"0.0");


  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(scheduleCompact);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Schedule_Compact));

  openstudio::IdfObject scheduleCompactIdf = workspace.getObjectsByType(IddObjectType::Schedule_Compact)[0];

  EXPECT_EQ(20u,scheduleCompactIdf.numFields());
  EXPECT_EQ(21u,scheduleCompact.numFields());

  ASSERT_EQ(scheduleCompactIdf.numFields() + 1,scheduleCompact.numFields());

  ASSERT_TRUE(scheduleCompact.name());
  ASSERT_TRUE(scheduleCompactIdf.name());
  EXPECT_EQ(scheduleCompact.name().get(),scheduleCompactIdf.name().get());

  for( size_t i = OS_Schedule_CompactFields::getValues().size(); i < scheduleCompact.numFields(); i++ )
  {
    boost::optional<std::string> s1 = scheduleCompactIdf.getString((int)i-1);
    boost::optional<std::string> s2 = scheduleCompact.getString((int)i);

    ASSERT_TRUE(s1);
    ASSERT_TRUE(s2);

    EXPECT_EQ(s1.get(),s2.get());
  }
}

TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateStandardOpaqueMaterial) {
  openstudio::model::Model model;
  openstudio::model::StandardOpaqueMaterial mat(model);

  mat.setName("Test Material");
  mat.setRoughness("Rough");
  mat.setThickness(0.0125);
  mat.setThermalConductivity(0.5);
  mat.setDensity(3.5);
  mat.setSpecificHeat(2.5);
  mat.setThermalAbsorptance(0.9);
  mat.setSolarAbsorptance(0.7);
  mat.setVisibleAbsorptance(0.7);


  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(mat);

  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Material));

  openstudio::IdfObject matIdf = workspace.getObjectsByType(IddObjectType::Material)[0];

  EXPECT_EQ(unsigned(9), matIdf.numFields());

  EXPECT_EQ( "Test Material", *(matIdf.name()) );
  EXPECT_EQ( "Rough", *(matIdf.getString(1)) );
  EXPECT_EQ( 0.0125, *(matIdf.getDouble(2)) );
  EXPECT_EQ( 0.5, *(matIdf.getDouble(3)) );
  EXPECT_EQ( 3.5, *(matIdf.getDouble(4)) );
  EXPECT_EQ( 2.5, *(matIdf.getDouble(5)) );
  EXPECT_EQ( 0.9, *(matIdf.getDouble(6)) );
  EXPECT_EQ( 0.7, *(matIdf.getDouble(7)) );
  EXPECT_EQ( 0.7, *(matIdf.getDouble(8)) );
}

TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateConstruction) {
  openstudio::model::Model model;

  openstudio::model::StandardOpaqueMaterial mat1(model);
  mat1.setName("test layer0");
  mat1.setRoughness("MediumRough");
  mat1.setThickness(0.02);
  mat1.setThermalConductivity(1.5);
  mat1.setDensity(20.0);
  mat1.setSpecificHeat(2.0);

  openstudio::model::StandardOpaqueMaterial mat2(model);
  mat2.setName("test layer1");
  mat2.setRoughness("MediumSmooth");
  mat2.setThickness(0.005);
  mat2.setThermalConductivity(0.3);
  mat2.setDensity(3.0);
  mat2.setSpecificHeat(0.3);

  openstudio::model::Construction construction(model);
  construction.setName("test construction");
  construction.insertLayer(0, mat1);
  construction.insertLayer(1, mat2);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(construction);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Construction));

  IdfObject constructionIdf = workspace.getObjectsByType(IddObjectType::Construction)[0];

  EXPECT_EQ(unsigned(3), constructionIdf.numFields());
  EXPECT_EQ("test construction", *(constructionIdf.name()) );
  EXPECT_EQ("test layer0", *(constructionIdf.getString(1)) );
  EXPECT_EQ("test layer1", *(constructionIdf.getString(2)) );
}

TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateSite) {
  openstudio::model::Model model;
  openstudio::model::Site site = model.getUniqueModelObject<openstudio::model::Site>();

  site.setName("Test Site");
  site.setLatitude(39.6);
  site.setLongitude(105.2);
  site.setTimeZone(-7.0);
  site.setElevation(1729.74);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(site);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_Location));

  IdfObject siteIdf = workspace.getObjectsByType(IddObjectType::Site_Location)[0];
  EXPECT_EQ(unsigned(5), siteIdf.numFields());

  EXPECT_EQ( "Test Site", *(siteIdf.name()) );
  EXPECT_EQ( 39.6, *(siteIdf.getDouble(1)) );
  EXPECT_EQ( 105.2, *(siteIdf.getDouble(2)) );
  EXPECT_EQ( -7.0, *(siteIdf.getDouble(3)) );
  EXPECT_EQ( 1729.74, *(siteIdf.getDouble(4)) );
}

TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateSiteGroundReflectance) {
  openstudio::model::Model model;
  openstudio::model::SiteGroundReflectance groundreflect = model.getUniqueModelObject<openstudio::model::SiteGroundReflectance>();

  groundreflect.setJanuaryGroundReflectance(0.11);
  groundreflect.setFebruaryGroundReflectance(0.12);
  groundreflect.setMarchGroundReflectance(0.13);
  groundreflect.setAprilGroundReflectance(0.14);
  groundreflect.setMayGroundReflectance(0.15);
  groundreflect.setJuneGroundReflectance(0.16);
  groundreflect.setJulyGroundReflectance(0.17);
  groundreflect.setAugustGroundReflectance(0.18);
  groundreflect.setSeptemberGroundReflectance(0.19);
  groundreflect.setOctoberGroundReflectance(0.20);
  groundreflect.setNovemberGroundReflectance(0.21);
  groundreflect.setDecemberGroundReflectance(0.22);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(groundreflect);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_GroundReflectance));

  IdfObject groundreflectIdf = workspace.getObjectsByType(IddObjectType::Site_GroundReflectance)[0];
  EXPECT_EQ(unsigned(12), groundreflectIdf.numFields());

  EXPECT_EQ( 0.11, *(groundreflectIdf.getDouble(0)) );
  EXPECT_EQ( 0.12, *(groundreflectIdf.getDouble(1)) );
  EXPECT_EQ( 0.13, *(groundreflectIdf.getDouble(2)) );
  EXPECT_EQ( 0.14, *(groundreflectIdf.getDouble(3)) );
  EXPECT_EQ( 0.15, *(groundreflectIdf.getDouble(4)) );
  EXPECT_EQ( 0.16, *(groundreflectIdf.getDouble(5)) );
  EXPECT_EQ( 0.17, *(groundreflectIdf.getDouble(6)) );
  EXPECT_EQ( 0.18, *(groundreflectIdf.getDouble(7)) );
  EXPECT_EQ( 0.19, *(groundreflectIdf.getDouble(8)) );
  EXPECT_EQ( 0.20, *(groundreflectIdf.getDouble(9)) );
  EXPECT_EQ( 0.21, *(groundreflectIdf.getDouble(10)) );
  EXPECT_EQ( 0.22, *(groundreflectIdf.getDouble(11)) );
}

TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateSiteGroundTemperatureBuildingSurface) {
  openstudio::model::Model model;
  openstudio::model::SiteGroundTemperatureBuildingSurface groundtemp = model.getUniqueModelObject<openstudio::model::SiteGroundTemperatureBuildingSurface>();

  groundtemp.setJanuaryGroundTemperature(19.527);
  groundtemp.setFebruaryGroundTemperature(19.502);
  groundtemp.setMarchGroundTemperature(19.536);
  groundtemp.setAprilGroundTemperature(19.598);
  groundtemp.setMayGroundTemperature(20.002);
  groundtemp.setJuneGroundTemperature(21.64);
  groundtemp.setJulyGroundTemperature(22.225);
  groundtemp.setAugustGroundTemperature(22.375);
  groundtemp.setSeptemberGroundTemperature(21.449);
  groundtemp.setOctoberGroundTemperature(20.121);
  groundtemp.setNovemberGroundTemperature(19.802);
  groundtemp.setDecemberGroundTemperature(19.633);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(groundtemp);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_GroundTemperature_BuildingSurface));

  IdfObject groundtempIdf = workspace.getObjectsByType(IddObjectType::Site_GroundTemperature_BuildingSurface)[0];
  EXPECT_EQ(unsigned(12), groundtempIdf.numFields());

  EXPECT_EQ( 19.527, *(groundtempIdf.getDouble(0)) );
  EXPECT_EQ( 19.502, *(groundtempIdf.getDouble(1)) );
  EXPECT_EQ( 19.536, *(groundtempIdf.getDouble(2)) );
  EXPECT_EQ( 19.598, *(groundtempIdf.getDouble(3)) );
  EXPECT_EQ( 20.002, *(groundtempIdf.getDouble(4)) );
  EXPECT_EQ( 21.64, *(groundtempIdf.getDouble(5)) );
  EXPECT_EQ( 22.225, *(groundtempIdf.getDouble(6)) );
  EXPECT_EQ( 22.375, *(groundtempIdf.getDouble(7)) );
  EXPECT_EQ( 21.449, *(groundtempIdf.getDouble(8)) );
  EXPECT_EQ( 20.121, *(groundtempIdf.getDouble(9)) );
  EXPECT_EQ( 19.802, *(groundtempIdf.getDouble(10)) );
  EXPECT_EQ( 19.633, *(groundtempIdf.getDouble(11)) );
}

TEST_F(EnergyPlusFixture, ForwardTranslatorTest_TranslateSiteGroundTemperatureDeep) {
  openstudio::model::Model model;
  openstudio::model::SiteGroundTemperatureDeep groundtemp = model.getUniqueModelObject<openstudio::model::SiteGroundTemperatureDeep>();

  std::vector<double> monthly_temps = {19.527, 19.502, 19.536, 19.598, 20.002, 21.64, 22.225, 22.375, 21.449, 20.121, 19.802, 19.633};
  groundtemp.setAllMonthlyTemperatures(monthly_temps);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(groundtemp);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_GroundTemperature_Deep));

  IdfObject groundtempIdf = workspace.getObjectsByType(IddObjectType::Site_GroundTemperature_Deep)[0];
  EXPECT_EQ(unsigned(12), groundtempIdf.numFields());

  for (int i=0; i < 12; ++i) {
    ASSERT_NEAR(monthly_temps[i], *groundtempIdf.getDouble(i), 0.001);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorTest_TranslateSiteGroundTemperatureFCfactorMethod) {
  openstudio::model::Model model;
  openstudio::model::SiteGroundTemperatureFCfactorMethod groundtemp = model.getUniqueModelObject<openstudio::model::SiteGroundTemperatureFCfactorMethod>();

  std::vector<double> monthly_temps = {19.527, 19.502, 19.536, 19.598, 20.002, 21.64, 22.225, 22.375, 21.449, 20.121, 19.802, 19.633};
  groundtemp.setAllMonthlyTemperatures(monthly_temps);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(groundtemp);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_GroundTemperature_FCfactorMethod));

  IdfObject groundtempIdf = workspace.getObjectsByType(IddObjectType::Site_GroundTemperature_FCfactorMethod)[0];
  EXPECT_EQ(unsigned(12), groundtempIdf.numFields());

  for (int i=0; i < 12; ++i) {
    ASSERT_NEAR(monthly_temps[i], *groundtempIdf.getDouble(i), 0.001);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorTest_TranslateSiteGroundTemperatureShallow) {
  openstudio::model::Model model;
  openstudio::model::SiteGroundTemperatureShallow groundtemp = model.getUniqueModelObject<openstudio::model::SiteGroundTemperatureShallow>();

  std::vector<double> monthly_temps = {19.527, 19.502, 19.536, 19.598, 20.002, 21.64, 22.225, 22.375, 21.449, 20.121, 19.802, 19.633};
  groundtemp.setAllMonthlyTemperatures(monthly_temps);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(groundtemp);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_GroundTemperature_Shallow));

  IdfObject groundtempIdf = workspace.getObjectsByType(IddObjectType::Site_GroundTemperature_Shallow)[0];
  EXPECT_EQ(unsigned(12), groundtempIdf.numFields());

  for (int i=0; i < 12; ++i) {
    ASSERT_NEAR(monthly_temps[i], *groundtempIdf.getDouble(i), 0.001);
  }
}


TEST_F(EnergyPlusFixture,ForwardTranslatorTest_TranslateSiteWaterMainsTemperature) {
  openstudio::model::Model model;
  openstudio::model::SiteWaterMainsTemperature watertemp = model.getUniqueModelObject<openstudio::model::SiteWaterMainsTemperature>();

  watertemp.setAnnualAverageOutdoorAirTemperature(9.69);
  watertemp.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(28.1);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(watertemp);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::Site_WaterMainsTemperature));

  IdfObject watertempIdf = workspace.getObjectsByType(IddObjectType::Site_WaterMainsTemperature)[0];
  EXPECT_EQ(unsigned(4), watertempIdf.numFields());

  EXPECT_EQ( "Correlation", *(watertempIdf.getString(0)) );
  EXPECT_EQ( "", *(watertempIdf.getString(1)) );
  EXPECT_EQ( 9.69, *(watertempIdf.getDouble(2)) );
  EXPECT_EQ( 28.1, *(watertempIdf.getDouble(3)) );
}

/*
TEST_F(EnergyPlusFixture,ForwardTranslatorTest_AllObjects) {
  // ETH@20130319 - Would like to have a test like this so we can inspect common errors and warnings
  // (and eliminate false positives). However, right now translateModel throws (and throws and throws),
  // because AirLoopHVAC assumes that so many things are hooked up properly and now is not the time to
  // wrap lots of different pieces of code in try-catches.
  //
  // Perhaps we can make a model and add a template HVAC, screen out object types already there, and
  // then try to add default objects for the rest ... Or maybe this test should be able to run as-is?
  // In other words, should ForwardTranslator be robust to missing related objects even if the App
  // and the API try their darndest to keep that from happening? (Wherever related objects are assumed
  // to exist (airLoopHVAC.sizingSystem() is the first one), we could wrap them in try-catch blocks and
  // log translator errors rather than having the exceptions propagate out of ForwardTranslator.)
  Workspace ws;
  IddObjectVector iddObjects = IddFactory::instance().getObjects(IddFileType(IddFileType::OpenStudio));
  for (const IddObject& iddObject : iddObjects) {
    ws.addObject(IdfObject(iddObject));
  }
  Model model(ws);
  ForwardTranslator translator;
  EXPECT_NO_THROW(translator.translateModel(model));
  // look at log to see errors, warnings, info.
}
*/

TEST_F(EnergyPlusFixture, ForwardTranslatorTest_MultiThreadedLogMessages) {

  // This thread calls forward translator, this is not a good example of threading
  // just used for testing
  class ForwardTranslatorThread : public QThread {
  public:

    ForwardTranslator translator;
    Model model;
    boost::optional<Workspace> workspace;

    ForwardTranslatorThread(Model _model)
      : model(_model)
    {}

  protected:
    void run() override{
      workspace = translator.translateModel(model);
    }
  };

  Logger::instance().standardOutLogger().enable();

  Model model;
  Space space(model); // not in thermal zone will generate a warning

  // run in current thread
  size_t numWarnings = 0;
  {
    ForwardTranslator translator;
    boost::optional<Workspace> workspace = translator.translateModel(model);
    ASSERT_TRUE(workspace);
    numWarnings = translator.warnings().size();
    EXPECT_NE(0, numWarnings);
  }

  // run single thread
  {
    ForwardTranslatorThread thread(model);
    EXPECT_FALSE(thread.workspace);
    thread.start();
    thread.wait();
    ASSERT_TRUE(thread.workspace);
    numWarnings = thread.translator.warnings().size();
    EXPECT_EQ(numWarnings, thread.translator.warnings().size());
  }

  // run four threads
  {
    ForwardTranslatorThread thread1(model);
    ForwardTranslatorThread thread2(model);
    ForwardTranslatorThread thread3(model);
    ForwardTranslatorThread thread4(model);
    EXPECT_FALSE(thread1.workspace);
    EXPECT_FALSE(thread2.workspace);
    EXPECT_FALSE(thread3.workspace);
    EXPECT_FALSE(thread4.workspace);
    thread1.start();
    thread2.start();
    thread3.start();
    thread4.start();
    thread1.wait();
    thread2.wait();
    thread3.wait();
    thread4.wait();
    ASSERT_TRUE(thread1.workspace);
    ASSERT_TRUE(thread2.workspace);
    ASSERT_TRUE(thread3.workspace);
    ASSERT_TRUE(thread4.workspace);
    EXPECT_EQ(numWarnings, thread1.translator.warnings().size());
    EXPECT_EQ(numWarnings, thread2.translator.warnings().size());
    EXPECT_EQ(numWarnings, thread3.translator.warnings().size());
    EXPECT_EQ(numWarnings, thread4.translator.warnings().size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorTest_TranslateZoneCapacitanceMultiplierResearchSpecial) {
  openstudio::model::Model model;
  openstudio::model::ZoneCapacitanceMultiplierResearchSpecial zcm = model.getUniqueModelObject<openstudio::model::ZoneCapacitanceMultiplierResearchSpecial>();

  zcm.setTemperatureCapacityMultiplier(2.0);
  zcm.setHumidityCapacityMultiplier(3.0);
  zcm.setCarbonDioxideCapacityMultiplier(4.0);

  ForwardTranslator trans;
  Workspace workspace = trans.translateModelObject(zcm);
  ASSERT_EQ(1u, workspace.numObjectsOfType(IddObjectType::ZoneCapacitanceMultiplier_ResearchSpecial));

  IdfObject zcmidf = workspace.getObjectsByType(IddObjectType::ZoneCapacitanceMultiplier_ResearchSpecial)[0];
  EXPECT_FLOAT_EQ(zcmidf.getDouble(ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier).get(), 2.0);
  EXPECT_FLOAT_EQ(zcmidf.getDouble(ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier).get(), 3.0);
  EXPECT_FLOAT_EQ(zcmidf.getDouble(ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier).get(), 4.0);
}


TEST_F(EnergyPlusFixture, BadVariableName)
{
  // this test checks that string values are properly escaped through translation

  Model model;

  OutputVariable goodVar("Good Name", model);
  EXPECT_EQ("Good Name", goodVar.variableName());

  OutputVariable badVar("Bad, !Name", model);
  EXPECT_EQ("Bad, !Name", badVar.variableName());

  {
    std::stringstream ss;
    ss << model;

    boost::optional<IdfFile> idf = IdfFile::load(ss, IddFileType::OpenStudio);
    ASSERT_TRUE(idf);

    Model model2;
    model2.addObjects(idf->objects());

    ASSERT_EQ(2u, model2.getConcreteModelObjects<OutputVariable>().size());
    for (auto outputVariable : model2.getConcreteModelObjects<OutputVariable>()){
      std::string s = outputVariable.variableName();
      EXPECT_TRUE(s == "Good Name" || s == "Bad, !Name") << s;
    }
  }

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Output_Variable).size());
  for (auto object : workspace.getObjectsByType(IddObjectType::Output_Variable)){
    ASSERT_TRUE(object.getString(Output_VariableFields::VariableName)) << object;
    std::string s = object.getString(Output_VariableFields::VariableName).get();
    EXPECT_TRUE(s == "Good Name" || s == "Bad, !Name") << s;
  }

  std::stringstream ss;
  ss << workspace;

  boost::optional<IdfFile> idf2 = IdfFile::load(ss, IddFileType::EnergyPlus);
  ASSERT_TRUE(idf2);

  Workspace workspace2(idf2.get());
  ASSERT_EQ(2u, workspace2.getObjectsByType(IddObjectType::Output_Variable).size());
  for (auto object : workspace2.getObjectsByType(IddObjectType::Output_Variable)){
    ASSERT_TRUE(object.getString(Output_VariableFields::VariableName)) << object;
    std::string s = object.getString(Output_VariableFields::VariableName).get();
    EXPECT_TRUE(s == "Good Name" || s == "Bad, !Name") << s;
  }

  ReverseTranslator rt;
  boost::optional<Model> model2 = rt.translateWorkspace(workspace2);
  ASSERT_TRUE(model2);
  ASSERT_EQ(2u, model2->getConcreteModelObjects<OutputVariable>().size());
  for (auto outputVariable : model2->getConcreteModelObjects<OutputVariable>()){
    std::string s = outputVariable.variableName();
    EXPECT_TRUE(s == "Good Name" || s == "Bad, !Name") << s;
  }
}
