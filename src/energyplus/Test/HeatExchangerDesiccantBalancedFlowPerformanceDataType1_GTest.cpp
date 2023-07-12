/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlow.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlow_Impl.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"
#include "../../model/HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"

#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatExchangerDesiccantBalancedFlowPerformanceDataType1) {
  Model m;

  AirLoopHVAC loop(m);
  Node supplyOutletNode = loop.supplyOutletNode();

  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controllerOutdoorAir);
  oaSystem.addToNode(supplyOutletNode);

  Node oaNode = oaSystem.outboardOANode().get();

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  EXPECT_TRUE(p.setNominalElectricPower(3.0));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient1(0.001));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient2(0.002));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient3(0.003));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient4(0.004));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient5(0.005));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient6(0.006));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient7(0.007));
  EXPECT_TRUE(p.setTemperatureEquationCoefficient8(0.008));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(0.009));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(0.0001));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirTemperatureforTemperatureEquation(0.0002));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirTemperatureforTemperatureEquation(0.0003));
  EXPECT_TRUE(p.setMinimumProcessInletAirHumidityRatioforTemperatureEquation(0.0004));
  EXPECT_TRUE(p.setMaximumProcessInletAirHumidityRatioforTemperatureEquation(0.0005));
  EXPECT_TRUE(p.setMinimumProcessInletAirTemperatureforTemperatureEquation(0.0006));
  EXPECT_TRUE(p.setMaximumProcessInletAirTemperatureforTemperatureEquation(0.0007));
  EXPECT_TRUE(p.setMinimumRegenerationAirVelocityforTemperatureEquation(0.0008));
  EXPECT_TRUE(p.setMaximumRegenerationAirVelocityforTemperatureEquation(0.0009));
  EXPECT_TRUE(p.setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(0.0001));
  EXPECT_TRUE(p.setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(0.0002));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(0.0003));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(0.0004));
  EXPECT_TRUE(p.setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(0.0005));
  EXPECT_TRUE(p.setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(0.0006));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient1(0.0007));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient2(0.0008));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient3(0.0009));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient4(0.001));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient5(0.002));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient6(0.003));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient7(0.004));
  EXPECT_TRUE(p.setHumidityRatioEquationCoefficient8(0.005));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(0.006));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(0.007));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(0.008));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(0.009));
  EXPECT_TRUE(p.setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(0.01));
  EXPECT_TRUE(p.setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(0.02));
  EXPECT_TRUE(p.setMinimumProcessInletAirTemperatureforHumidityRatioEquation(0.03));
  EXPECT_TRUE(p.setMaximumProcessInletAirTemperatureforHumidityRatioEquation(0.04));
  EXPECT_TRUE(p.setMinimumRegenerationAirVelocityforHumidityRatioEquation(0.05));
  EXPECT_TRUE(p.setMaximumRegenerationAirVelocityforHumidityRatioEquation(0.06));
  EXPECT_TRUE(p.setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(0.07));
  EXPECT_TRUE(p.setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(0.08));
  EXPECT_TRUE(p.setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(0.09));
  EXPECT_TRUE(p.setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(0.01));
  EXPECT_TRUE(p.setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(0.02));
  EXPECT_TRUE(p.setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(0.03));
  HeatExchangerDesiccantBalancedFlow hx(m, p);
  EXPECT_TRUE(hx.addToNode(oaNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfHXPs(w.getObjectsByType(IddObjectType::HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1));
  ASSERT_EQ(1u, idfHXPs.size());
  WorkspaceObject idfHXP(idfHXPs[0]);

  EXPECT_EQ("Autosize", idfHXP.getString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idfHXP.getString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, false).get());
  EXPECT_EQ(3.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalElectricPower, false).get());
  EXPECT_EQ(0.001, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient1, false).get());
  EXPECT_EQ(0.002, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient2, false).get());
  EXPECT_EQ(0.003, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient3, false).get());
  EXPECT_EQ(0.004, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient4, false).get());
  EXPECT_EQ(0.005, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient5, false).get());
  EXPECT_EQ(0.006, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient6, false).get());
  EXPECT_EQ(0.007, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient7, false).get());
  EXPECT_EQ(0.008, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient8, false).get());
  EXPECT_EQ(
    0.009, idfHXP
             .getDouble(
               HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforTemperatureEquation, false)
             .get());
  EXPECT_EQ(
    0.0001,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforTemperatureEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.0002,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0003,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0004,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0005,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0006,
    idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0007,
    idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0008,
    idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0009,
    idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforTemperatureEquation, false)
      .get());
  EXPECT_EQ(0.0001,
            idfHXP
              .getDouble(
                HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirTemperatureforTemperatureEquation, false)
              .get());
  EXPECT_EQ(0.0002,
            idfHXP
              .getDouble(
                HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirTemperatureforTemperatureEquation, false)
              .get());
  EXPECT_EQ(
    0.0003,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforTemperatureEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.0004,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforTemperatureEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.0005,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforTemperatureEquation, false)
      .get());
  EXPECT_EQ(
    0.0006,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforTemperatureEquation, false)
      .get());
  EXPECT_EQ(0.0007,
            idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient1, false).get());
  EXPECT_EQ(0.0008,
            idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient2, false).get());
  EXPECT_EQ(0.0009,
            idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient3, false).get());
  EXPECT_EQ(0.001, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient4, false).get());
  EXPECT_EQ(0.002, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient5, false).get());
  EXPECT_EQ(0.003, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient6, false).get());
  EXPECT_EQ(0.004, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient7, false).get());
  EXPECT_EQ(0.005, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient8, false).get());
  EXPECT_EQ(
    0.006,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.007,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.008, idfHXP
             .getDouble(
               HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforHumidityRatioEquation, false)
             .get());
  EXPECT_EQ(
    0.009, idfHXP
             .getDouble(
               HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforHumidityRatioEquation, false)
             .get());
  EXPECT_EQ(
    0.01,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforHumidityRatioEquation, false)
      .get());
  EXPECT_EQ(
    0.02,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforHumidityRatioEquation, false)
      .get());
  EXPECT_EQ(
    0.03,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforHumidityRatioEquation, false)
      .get());
  EXPECT_EQ(
    0.04,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforHumidityRatioEquation, false)
      .get());
  EXPECT_EQ(
    0.05,
    idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforHumidityRatioEquation, false)
      .get());
  EXPECT_EQ(
    0.06,
    idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforHumidityRatioEquation, false)
      .get());
  EXPECT_EQ(
    0.07,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.08,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.09,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.01,
    idfHXP
      .getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation,
                 false)
      .get());
  EXPECT_EQ(
    0.02, idfHXP
            .getDouble(
              HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforHumidityRatioEquation, false)
            .get());
  EXPECT_EQ(
    0.03, idfHXP
            .getDouble(
              HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforHumidityRatioEquation, false)
            .get());
}
