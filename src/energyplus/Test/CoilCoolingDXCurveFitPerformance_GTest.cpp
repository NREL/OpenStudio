/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"

#include "../../model/CoilCoolingDX.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Node.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../utilities/idd/IddObject.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXCurveFitPerformance) {
  Model m;

  CoilCoolingDXCurveFitOperatingMode operatingMode(m);
  CoilCoolingDXCurveFitPerformance performance(m, operatingMode);

  EXPECT_TRUE(performance.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(performance.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));

  CoilCoolingDX dx(m, performance);

  // put it inside a Unitary, and put Unitary on an AirLoopHVAC so it gets translated
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(dx);
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfPerformances(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_Performance));
  ASSERT_EQ(1u, idfPerformances.size());
  WorkspaceObject idfPerformance(idfPerformances[0]);

  boost::optional<WorkspaceObject> woBaseOperatingMode(idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode));
  EXPECT_TRUE(woBaseOperatingMode);
  if (woBaseOperatingMode) {
    EXPECT_EQ(woBaseOperatingMode->iddObject().type(), IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode);
  }
  boost::optional<WorkspaceObject> woAlternativeOperatingMode1(
    idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1));
  EXPECT_FALSE(woAlternativeOperatingMode1);
  boost::optional<WorkspaceObject> woAlternativeOperatingMode2(
    idfPerformance.getTarget(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2));
  EXPECT_FALSE(woAlternativeOperatingMode2);

  WorkspaceObjectVector idfOperatingModes(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_CurveFit_OperatingMode));
  ASSERT_EQ(1u, idfOperatingModes.size());
  WorkspaceObject idfOperatingMode(idfOperatingModes[0]);

  EXPECT_EQ(woBaseOperatingMode.get(), idfOperatingMode);

  EXPECT_EQ(105.0, idfPerformance.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacity, false).get());
  EXPECT_EQ(crankCurve.nameString(),
            idfPerformance.getString(Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(
    -25.0, idfPerformance.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, false).get());
  EXPECT_EQ(
    10.0,
    idfPerformance.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, false).get());
  EXPECT_EQ(773.3, idfPerformance.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::UnitInternalStaticAirPressure, false).get());
  EXPECT_EQ("Discrete", idfPerformance.getString(Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod, false).get());
  EXPECT_EQ(0.0, idfPerformance.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterCapacity, false).get());
  EXPECT_EQ(2.0,
            idfPerformance.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterSetpointTemperature, false).get());
  EXPECT_EQ("Always On Discrete",
            idfPerformance.getString(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingScheduleName, false).get());
  EXPECT_EQ("Electricity", idfPerformance.getString(Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType, false).get());
}
