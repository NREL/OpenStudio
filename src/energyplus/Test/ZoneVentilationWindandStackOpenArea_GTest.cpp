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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ZoneVentilationWindandStackOpenArea.hpp"
#include "../../model/ZoneVentilationWindandStackOpenArea_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/ZoneVentilation_WindandStackOpenArea_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneVentilationWindandStackOpenArea) {
  ForwardTranslator ft;

  Model m;

  ZoneVentilationWindandStackOpenArea zv(m);

  // No zone, not translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_zones(w.getObjectsByType(IddObjectType::ZoneVentilation_WindandStackOpenArea));
    EXPECT_EQ(0u, idf_zones.size());
  }

  ThermalZone z(m);
  EXPECT_TRUE(zv.addToThermalZone(z));

  EXPECT_TRUE(zv.setOpeningArea(10.0));

  ScheduleConstant openingAreaSch(m);
  openingAreaSch.setValue(1.0);
  EXPECT_TRUE(zv.setOpeningAreaFractionSchedule(openingAreaSch));

  EXPECT_TRUE(zv.setOpeningEffectiveness(0.5));
  EXPECT_TRUE(zv.setEffectiveAngle(90));
  EXPECT_TRUE(zv.setHeightDifference(10));
  EXPECT_TRUE(zv.setDischargeCoefficientforOpening(0.3));

  EXPECT_TRUE(zv.setMinimumIndoorTemperature(10.0));
  ScheduleConstant minIndoorTempSch(m);
  minIndoorTempSch.setValue(-10.0);
  EXPECT_TRUE(zv.setMinimumIndoorTemperatureSchedule(minIndoorTempSch));

  EXPECT_TRUE(zv.setMaximumIndoorTemperature(30.0));
  ScheduleConstant maxIndoorTempSch(m);
  maxIndoorTempSch.setValue(30.0);
  EXPECT_TRUE(zv.setMaximumIndoorTemperatureSchedule(maxIndoorTempSch));

  EXPECT_TRUE(zv.setDeltaTemperature(3.0));
  ScheduleConstant deltaTempSch(m);
  deltaTempSch.setValue(3.0);
  EXPECT_TRUE(zv.setDeltaTemperatureSchedule(deltaTempSch));

  EXPECT_TRUE(zv.setMinimumOutdoorTemperature(10.0));
  ScheduleConstant minOutdoorTempSch(m);
  minOutdoorTempSch.setValue(-10.0);
  EXPECT_TRUE(zv.setMinimumOutdoorTemperatureSchedule(minOutdoorTempSch));

  EXPECT_TRUE(zv.setMaximumOutdoorTemperature(-20.0));
  ScheduleConstant maxOutdoorTempSch(m);
  maxOutdoorTempSch.setValue(20.0);
  EXPECT_TRUE(zv.setMaximumOutdoorTemperatureSchedule(maxOutdoorTempSch));

  EXPECT_TRUE(zv.setMaximumWindSpeed(15.0));

  // Zone: translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_zones(w.getObjectsByType(IddObjectType::Zone));
    ASSERT_EQ(1u, idf_zones.size());
    WorkspaceObject idf_zone(idf_zones[0]);

    WorkspaceObjectVector idf_zvs(w.getObjectsByType(IddObjectType::ZoneVentilation_WindandStackOpenArea));
    ASSERT_EQ(1u, idf_zvs.size());
    WorkspaceObject idf_zv(idf_zvs[0]);

    // No ZoneHVACEquipmentList
    WorkspaceObjectVector idf_zeqs(w.getObjectsByType(IddObjectType::ZoneHVAC_EquipmentList));
    EXPECT_EQ(0u, idf_zeqs.size());

    EXPECT_EQ(idf_zone.nameString(), idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::ZoneName).get());
    EXPECT_EQ(openingAreaSch.nameString(), idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName).get());
    EXPECT_EQ(zv.openingArea(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningArea).get());
    EXPECT_EQ(zv.openingEffectiveness(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness).get());
    EXPECT_EQ(zv.effectiveAngle(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle).get());
    EXPECT_EQ(zv.heightDifference(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::HeightDifference).get());
    EXPECT_EQ(zv.dischargeCoefficientforOpening(),
              idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening).get());

    EXPECT_EQ(zv.minimumIndoorTemperature(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature).get());
    EXPECT_EQ(zv.minimumIndoorTemperatureSchedule().get().nameString(),
              idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName).get());

    EXPECT_EQ(zv.maximumIndoorTemperature(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperature).get());
    EXPECT_EQ(zv.maximumIndoorTemperatureSchedule().get().nameString(),
              idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName).get());

    EXPECT_EQ(zv.deltaTemperature(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperature).get());
    EXPECT_EQ(zv.deltaTemperatureSchedule().get().nameString(),
              idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName).get());

    EXPECT_EQ(zv.minimumOutdoorTemperature(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperature).get());
    EXPECT_EQ(zv.minimumOutdoorTemperatureSchedule().get().nameString(),
              idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName).get());

    EXPECT_EQ(zv.maximumOutdoorTemperature(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperature).get());
    EXPECT_EQ(zv.maximumOutdoorTemperatureSchedule().get().nameString(),
              idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName).get());

    EXPECT_EQ(zv.maximumWindSpeed(), idf_zv.getDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumWindSpeed).get());
  }

  zv.autocalculateOpeningEffectiveness();
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_zvs(w.getObjectsByType(IddObjectType::ZoneVentilation_WindandStackOpenArea));
    EXPECT_EQ(1u, idf_zvs.size());
    WorkspaceObject idf_zv(idf_zvs[0]);

    EXPECT_EQ("Autocalculate", idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness).get());
  }

  EXPECT_TRUE(zv.setOpeningEffectiveness(0.5));
  zv.autocalculateDischargeCoefficientforOpening();

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idf_zvs(w.getObjectsByType(IddObjectType::ZoneVentilation_WindandStackOpenArea));
    EXPECT_EQ(1u, idf_zvs.size());
    WorkspaceObject idf_zv(idf_zvs[0]);

    EXPECT_EQ("Autocalculate", idf_zv.getString(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneVentilationWindandStackOpenArea) {
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType iddFileType(IddFileType::EnergyPlus);
  Workspace workspace(level, iddFileType);

  IdfObject idf_zone(IddObjectType::Zone);
  idf_zone.setName("Space-1");

  IdfObject idf_zv(IddObjectType::ZoneVentilation_WindandStackOpenArea);
  idf_zv.setName("My ZoneVentilationWindAndStackOpenArea");
  EXPECT_TRUE(idf_zv.setString(ZoneVentilation_WindandStackOpenAreaFields::ZoneName, idf_zone.nameString()));
  EXPECT_TRUE(idf_zv.setDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningArea, 10.5));
  // Opening Area Fraction Schedule: leave blank

  // Opening Effectiveness: autocalculate is default in Ctor
  EXPECT_TRUE(idf_zv.setDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, 0.8));

  EXPECT_TRUE(idf_zv.setDouble(ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle, 180.0));

  // Height Difference: Leave blank (expect zero)

  // Discharge Coef: autocalculate
  EXPECT_TRUE(idf_zv.setString(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, "Autocalculate"));

  // Minimum Indoor Temperature
  EXPECT_TRUE(idf_zv.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature, -20.0));

  // Minimum Indoor Temperature Schedule Name
  IdfObject idf_minIndoorTempSch(IddObjectType::Schedule_Constant);
  idf_minIndoorTempSch.setName("minIndoorTempSch");
  EXPECT_TRUE(idf_minIndoorTempSch.setDouble(Schedule_ConstantFields::HourlyValue, -20.0));
  EXPECT_TRUE(idf_zv.setString(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName, idf_minIndoorTempSch.nameString()));

  IdfObjectVector objects;
  objects.push_back(idf_zone);
  objects.push_back(idf_zv);
  objects.push_back(idf_minIndoorTempSch);
  EXPECT_EQ(3u, workspace.addObjects(objects).size());

  ReverseTranslator rt;
  Model m = rt.translateWorkspace(workspace);

  Schedule alwaysOnDiscreteSchedule = m.alwaysOnDiscreteSchedule();

  ASSERT_EQ(1u, m.getModelObjects<ThermalZone>().size());
  ThermalZone zone = m.getModelObjects<ThermalZone>()[0];

  ASSERT_EQ(1u, m.getModelObjects<ZoneVentilationWindandStackOpenArea>().size());
  ZoneVentilationWindandStackOpenArea zv = m.getModelObjects<ZoneVentilationWindandStackOpenArea>()[0];

  EXPECT_EQ("My ZoneVentilationWindAndStackOpenArea", zv.name().get());
  EXPECT_EQ(10.5, zv.openingArea());
  EXPECT_EQ(alwaysOnDiscreteSchedule, zv.openingAreaFractionSchedule());

  EXPECT_FALSE(zv.isOpeningEffectivenessAutocalculated());
  ASSERT_TRUE(zv.openingEffectiveness());
  EXPECT_EQ(0.8, zv.openingEffectiveness().get());

  EXPECT_EQ(180.0, zv.effectiveAngle());

  EXPECT_EQ(0.0, zv.heightDifference());

  EXPECT_TRUE(zv.isDischargeCoefficientforOpeningAutocalculated());

  EXPECT_EQ(-20, zv.minimumIndoorTemperature());
  ASSERT_TRUE(zv.minimumIndoorTemperatureSchedule());
  EXPECT_EQ("minIndoorTempSch", zv.minimumIndoorTemperatureSchedule()->nameString());

  ASSERT_EQ(1u, zone.equipment().size());
  EXPECT_EQ(zv, zone.equipment()[0]);
}
