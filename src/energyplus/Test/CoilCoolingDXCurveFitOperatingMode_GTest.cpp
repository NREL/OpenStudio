/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"

#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Node.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_OperatingMode_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXCurveFitOperatingMode) {
  Model m;
  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);
  CoilCoolingDX dx(m, performance);

  CoilCoolingDXCurveFitSpeed speed1(m);
  CoilCoolingDXCurveFitSpeed speed2(m);

  // put it inside a Unitary, and put Unitary on an AirLoopHVAC so it gets translated
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(dx);
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  EXPECT_TRUE(operatingMode.addSpeed(speed1));
  EXPECT_TRUE(operatingMode.addSpeed(speed2));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfOperatingModes(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode));
  ASSERT_EQ(1u, idfOperatingModes.size());
  WorkspaceObject idfOperatingMode(idfOperatingModes[0]);

  EXPECT_EQ("Autosize", idfOperatingMode.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedGrossTotalCoolingCapacity, false).get());
  EXPECT_EQ("Autosize", idfOperatingMode.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedEvaporatorAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idfOperatingMode.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatedCondenserAirFlowRate, false).get());
  EXPECT_EQ(0.0, idfOperatingMode.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::MaximumCyclingRate, false).get());
  EXPECT_EQ(0.0, idfOperatingMode
                   .getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, false)
                   .get());
  EXPECT_EQ(0.0, idfOperatingMode.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::LatentCapacityTimeConstant, false).get());
  EXPECT_EQ(0.0, idfOperatingMode.getDouble(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalTimeforCondensateRemovaltoBegin, false).get());
  EXPECT_EQ("No", idfOperatingMode.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::ApplyLatentDegradationtoSpeedsGreaterthan1, false).get());
  EXPECT_EQ("AirCooled", idfOperatingMode.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::CondenserType, false).get());
  EXPECT_EQ("Autosize", idfOperatingMode.getString(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalEvaporativeCondenserPumpPower, false).get());
  EXPECT_EQ(2, idfOperatingMode.getInt(Coil_Cooling_DX_CurveFit_OperatingModeFields::NominalSpeedNumber, false).get());
  EXPECT_EQ(2u, idfOperatingMode.numExtensibleGroups());
}
