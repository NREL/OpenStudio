/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/AvailabilityManagerHybridVentilation.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/Curve.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"

#include <utilities/idd/AvailabilityManager_HybridVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerHybridVentilation) {

  Model m;

  AvailabilityManagerHybridVentilation avm(m);
  avm.setName("My AvailabilityManagerHybridVentilation");

  AirLoopHVAC a(m);
  a.addAvailabilityManager(avm);
  ASSERT_TRUE(avm.loop());
  EXPECT_EQ(a, avm.loop().get());

  // Controlled Zone: Optional Object
  ThermalZone z(m);
  z.setName("Zone 1");
  Space s(m);
  s.setThermalZone(z);

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, alwaysOn);
  EXPECT_TRUE(a.addBranchForZone(z, atu));

  EXPECT_TRUE(avm.setControlledZone(z));
  ASSERT_TRUE(avm.controlledZone());
  EXPECT_EQ(z, avm.controlledZone().get());

  // Ventilation Control Mode Schedule: Required Object
  ScheduleConstant ventilation_control_sch(m);
  ventilation_control_sch.setName("Ventilation Control Mode Schedule");
  EXPECT_TRUE(avm.setVentilationControlModeSchedule(ventilation_control_sch));
  EXPECT_EQ(ventilation_control_sch, avm.ventilationControlModeSchedule());

  // Use Weather File Rain Indicators: Required Boolean
  EXPECT_TRUE(avm.setUseWeatherFileRainIndicators(true));
  EXPECT_TRUE(avm.useWeatherFileRainIndicators());

  // Maximum Wind Speed: Required Double
  EXPECT_TRUE(avm.setMaximumWindSpeed(40.0));
  EXPECT_EQ(40.0, avm.maximumWindSpeed());

  // Minimum Outdoor Temperature: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorTemperature(15));
  EXPECT_EQ(15, avm.minimumOutdoorTemperature());

  // Maximum Outdoor Temperature: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorTemperature(35.0));
  EXPECT_EQ(35.0, avm.maximumOutdoorTemperature());

  // Minimum Outdoor Enthalpy: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorEnthalpy(20000.0));
  EXPECT_EQ(20000.0, avm.minimumOutdoorEnthalpy());

  // Maximum Outdoor Enthalpy: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorEnthalpy(30000.0));
  EXPECT_EQ(30000.0, avm.maximumOutdoorEnthalpy());

  // Minimum Outdoor Dewpoint: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorDewpoint(17.0));
  EXPECT_EQ(17.0, avm.minimumOutdoorDewpoint());

  // Maximum Outdoor Dewpoint: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorDewpoint(30.0));
  EXPECT_EQ(30.0, avm.maximumOutdoorDewpoint());

  // Minimum Outdoor Ventilation Air Schedule: Optional Object but set in Ctor and non optional
  ScheduleConstant min_oa_sch(m);
  min_oa_sch.setName("Min OA Schedule");
  EXPECT_TRUE(avm.setMinimumOutdoorVentilationAirSchedule(min_oa_sch));
  EXPECT_EQ(min_oa_sch, avm.minimumOutdoorVentilationAirSchedule());

  // Opening Factor Function of Wind Speed Curve: Optional Object
  CurveLinear opening_factor_curve(m);
  opening_factor_curve.setName("Opening Factor Function of Wind Speed Curve");
  EXPECT_TRUE(avm.setOpeningFactorFunctionofWindSpeedCurve(opening_factor_curve));
  ASSERT_TRUE(avm.openingFactorFunctionofWindSpeedCurve());

  // AirflowNetwork Control Type Schedule: Optional Object
  ScheduleConstant afn_control_sch(m);
  afn_control_sch.setName("AirflowNetwork Control Type Schedule");
  EXPECT_TRUE(avm.setAirflowNetworkControlTypeSchedule(afn_control_sch));
  ASSERT_TRUE(avm.airflowNetworkControlTypeSchedule());

  // Simple Airflow Control Type Schedule: Optional Object
  ScheduleConstant simple_control_sch(m);
  simple_control_sch.setName("Simple Airflow Control Type Schedule");
  EXPECT_TRUE(avm.setSimpleAirflowControlTypeSchedule(simple_control_sch));
  ASSERT_TRUE(avm.simpleAirflowControlTypeSchedule());
  EXPECT_EQ(simple_control_sch, avm.simpleAirflowControlTypeSchedule().get());

  // ZoneVentilation Object: Optional Object
  ZoneVentilationDesignFlowRate zv(m);
  EXPECT_TRUE(avm.setZoneVentilationObject(zv));
  ASSERT_TRUE(avm.zoneVentilationObject());
  EXPECT_EQ(zv, avm.zoneVentilationObject().get());

  // Minimum HVAC Operation Time: Required Double
  EXPECT_TRUE(avm.setMinimumHVACOperationTime(0.1));
  EXPECT_EQ(0.1, avm.minimumHVACOperationTime());

  // Minimum Ventilation Time: Required Double
  EXPECT_TRUE(avm.setMinimumVentilationTime(0.2));
  EXPECT_EQ(0.2, avm.minimumVentilationTime());

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManager_HybridVentilation));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  EXPECT_EQ(avm.nameString(), idf_avm.nameString());
  // HVAC Air Loop Name
  EXPECT_EQ(a.nameString(), idf_avm.getString(AvailabilityManager_HybridVentilationFields::HVACAirLoopName).get());

  EXPECT_EQ(z.nameString(), idf_avm.getString(AvailabilityManager_HybridVentilationFields::ControlZoneName).get());

  EXPECT_EQ(ventilation_control_sch.nameString(),
            idf_avm.getString(AvailabilityManager_HybridVentilationFields::VentilationControlModeScheduleName).get());

  EXPECT_EQ("Yes", idf_avm.getString(AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators).get());

  EXPECT_EQ(40.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MaximumWindSpeed).get());

  EXPECT_EQ(15.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorTemperature).get());
  EXPECT_EQ(35.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorTemperature).get());

  EXPECT_EQ(20000.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorEnthalpy).get());
  EXPECT_EQ(30000.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorEnthalpy).get());

  EXPECT_EQ(17.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorDewpoint).get());
  EXPECT_EQ(30.0, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorDewpoint).get());

  EXPECT_EQ(min_oa_sch.nameString(), idf_avm.getString(AvailabilityManager_HybridVentilationFields::MinimumOutdoorVentilationAirScheduleName).get());

  EXPECT_EQ(opening_factor_curve.nameString(),
            idf_avm.getString(AvailabilityManager_HybridVentilationFields::OpeningFactorFunctionofWindSpeedCurveName).get());

  EXPECT_EQ(afn_control_sch.nameString(),
            idf_avm.getString(AvailabilityManager_HybridVentilationFields::AirflowNetworkControlTypeScheduleName).get());

  EXPECT_EQ(simple_control_sch.nameString(),
            idf_avm.getString(AvailabilityManager_HybridVentilationFields::SimpleAirflowControlTypeScheduleName).get());

  EXPECT_EQ(zv.nameString(), idf_avm.getString(AvailabilityManager_HybridVentilationFields::ZoneVentilationObjectName).get());

  EXPECT_EQ(0.1, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MinimumHVACOperationTime).get());
  EXPECT_EQ(0.2, idf_avm.getDouble(AvailabilityManager_HybridVentilationFields::MinimumVentilationTime).get());
}
