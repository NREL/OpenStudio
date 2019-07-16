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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"

#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleRuleset_Impl.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"


#include <utilities/idd/ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/ThermostatSetpoint_SingleHeating_FieldEnums.hxx>
#include <utilities/idd/ThermostatSetpoint_SingleCooling_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/** Case when you defined both a heating and cooling schedule for the thermostat.
 * Should end up as a ThermostatSetpoint:DualSetpoint in the IDF
 **/
TEST_F(EnergyPlusFixture,ForwardTranslator_Thermostat_Two_Schedules)
{
  Model m;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  ThermalZone zone(m);

  Space space1(m);
  space1.setThermalZone(zone);

  Surface surface1(points, m);
  surface1.setSpace(space1);

  // Create a thermostat
  ThermostatSetpointDualSetpoint thermostat(m);

  // Add Heating Setpoint Schedule
  ScheduleRuleset heat_sch = ScheduleRuleset(m, 18.0);
  thermostat.setHeatingSetpointTemperatureSchedule(heat_sch);

  // Add Cooling Setpoint Schedule
  ScheduleRuleset cool_sch = ScheduleRuleset(m, 26.0);
  thermostat.setCoolingSetpointTemperatureSchedule(cool_sch);

  // Assign to zone
  zone.setThermostatSetpointDualSetpoint(thermostat);

  // You also need an equipment, or ideal Loads, or tstat's not translated
  zone.setUseIdealAirLoads(true);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ThermostatSetpoint_DualSetpoint).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat).size());

  IdfObject idf_tstat = workspace.getObjectsByType(IddObjectType::ThermostatSetpoint_DualSetpoint)[0];

  ASSERT_EQ(idf_tstat.getString(ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName).get(),
            heat_sch.name());
  ASSERT_EQ(idf_tstat.getString(ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName).get(),
            cool_sch.name());

  IdfObject idf_zone_control = workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat)[0];
  ASSERT_EQ(1u, idf_zone_control.extensibleGroups().size());
  IdfExtensibleGroup eg = idf_zone_control.extensibleGroups()[0];
  ASSERT_EQ(eg.getString(ZoneControl_ThermostatExtensibleFields::ControlObjectType).get(),
            idf_tstat.iddObject().name());
  ASSERT_EQ(eg.getString(ZoneControl_ThermostatExtensibleFields::ControlName).get(),
            idf_tstat.name().get());

}


/** Case where you defined only a heating schedule for the thermostat.
 * Should end up as a ThermostatSetpoint:SingleHeating in the IDF
 **/
TEST_F(EnergyPlusFixture,ForwardTranslator_Thermostat_Heat_Only)
{
  Model m;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  ThermalZone zone(m);

  Space space1(m);
  space1.setThermalZone(zone);

  Surface surface1(points, m);
  surface1.setSpace(space1);

  // Create a thermostat
  ThermostatSetpointDualSetpoint thermostat(m);

  // Add Heating Setpoint Schedule
  ScheduleRuleset heat_sch = ScheduleRuleset(m, 18.0);
  thermostat.setHeatingSetpointTemperatureSchedule(heat_sch);

  // Assign to zone
  zone.setThermostatSetpointDualSetpoint(thermostat);

  // You also need an equipment, or ideal Loads, or tstat's not translated
  zone.setUseIdealAirLoads(true);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ThermostatSetpoint_SingleHeating).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat).size());

  IdfObject idf_tstat = workspace.getObjectsByType(IddObjectType::ThermostatSetpoint_SingleHeating)[0];

  ASSERT_EQ(idf_tstat.getString(ThermostatSetpoint_SingleHeatingFields::SetpointTemperatureScheduleName).get(),
            heat_sch.name());

  IdfObject idf_zone_control = workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat)[0];
  ASSERT_EQ(1u, idf_zone_control.extensibleGroups().size());
  IdfExtensibleGroup eg = idf_zone_control.extensibleGroups()[0];
  ASSERT_EQ(eg.getString(ZoneControl_ThermostatExtensibleFields::ControlObjectType).get(),
            idf_tstat.iddObject().name() );
  ASSERT_EQ(eg.getString(ZoneControl_ThermostatExtensibleFields::ControlName).get(),
            idf_tstat.name().get() );

}


/** Case where you defined only a cooling schedule for the thermostat.
 * Should end up as a ThermostatSetpoint:SingleCooling in the IDF
 **/
TEST_F(EnergyPlusFixture,ForwardTranslator_Thermostat_Cool_Only)
{
  Model m;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

  ThermalZone zone(m);

  Space space1(m);
  space1.setThermalZone(zone);

  Surface surface1(points, m);
  surface1.setSpace(space1);

  // Create a thermostat
  ThermostatSetpointDualSetpoint thermostat(m);

  // Add Cooling Setpoint Schedule
  ScheduleRuleset cool_sch = ScheduleRuleset(m, 26.0);
  thermostat.setCoolingSetpointTemperatureSchedule(cool_sch);

  // Assign to zone
  zone.setThermostatSetpointDualSetpoint(thermostat);

  // You also need an equipment, or ideal Loads, or tstat's not translated
  zone.setUseIdealAirLoads(true);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ThermostatSetpoint_SingleCooling).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat).size());

  IdfObject idf_tstat = workspace.getObjectsByType(IddObjectType::ThermostatSetpoint_SingleCooling)[0];

  ASSERT_EQ(idf_tstat.getString(ThermostatSetpoint_SingleCoolingFields::SetpointTemperatureScheduleName).get(),
            cool_sch.name());

  IdfObject idf_zone_control = workspace.getObjectsByType(IddObjectType::ZoneControl_Thermostat)[0];
  ASSERT_EQ(1u, idf_zone_control.extensibleGroups().size());

  IdfExtensibleGroup eg = idf_zone_control.extensibleGroups()[0];
  ASSERT_EQ(eg.getString(ZoneControl_ThermostatExtensibleFields::ControlObjectType).get(),
            idf_tstat.iddObject().name() );
  ASSERT_EQ(eg.getString(ZoneControl_ThermostatExtensibleFields::ControlName).get(),
            idf_tstat.name().get() );
}
