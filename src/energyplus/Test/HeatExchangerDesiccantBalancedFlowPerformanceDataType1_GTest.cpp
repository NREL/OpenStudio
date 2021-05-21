/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/HVACTemplates.hpp"

#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HeatExchangerDesiccantBalancedFlowPerformanceDataType1) {
  Model m;

  AirLoopHVAC loop = addSystemType3(m).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx(m, p);
  EXPECT_TRUE(hx.addToNode(oaNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfHXPs(w.getObjectsByType(IddObjectType::HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1));
  ASSERT_EQ(1u, idfHXPs.size());
  WorkspaceObject idfHXP(idfHXPs[0]);
  
  EXPECT_EQ("Autosize", idfHXP.getString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFlowRate, false).get());
  EXPECT_EQ("Autosize", idfHXP.getString(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalAirFaceVelocity, false).get());
  EXPECT_EQ(0.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::NominalElectricPower, false).get());
  EXPECT_EQ(-7.18302E+00, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient1, false).get());
  EXPECT_EQ(-1.84967E+02, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient2, false).get());
  EXPECT_EQ(1.00051E+00, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient3, false).get());
  EXPECT_EQ(1.16033E+04, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient4, false).get());
  EXPECT_EQ(-5.07550E+01, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient5, false).get());
  EXPECT_EQ(-1.68467E-02, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient6, false).get());
  EXPECT_EQ(5.82213E+01, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient7, false).get());
  EXPECT_EQ(5.98863E-01, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::TemperatureEquationCoefficient8, false).get());
  EXPECT_EQ(0.007143, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforTemperatureEquation, false).get());
  EXPECT_EQ(0.024286, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforTemperatureEquation, false).get());
  EXPECT_EQ(17.83333, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforTemperatureEquation, false).get());
  EXPECT_EQ(48.88889, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforTemperatureEquation, false).get());
  EXPECT_EQ(0.005000, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforTemperatureEquation, false).get());
  EXPECT_EQ(0.015714, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforTemperatureEquation, false).get());
  EXPECT_EQ(4.583333, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforTemperatureEquation, false).get());
  EXPECT_EQ(21.83333, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforTemperatureEquation, false).get());
  EXPECT_EQ(2.286, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforTemperatureEquation, false).get());
  EXPECT_EQ(4.826, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforTemperatureEquation, false).get());
  EXPECT_EQ(16.66667, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirTemperatureforTemperatureEquation, false).get());
  EXPECT_EQ(46.11111, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirTemperatureforTemperatureEquation, false).get());
  EXPECT_EQ(10.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforTemperatureEquation, false).get());
  EXPECT_EQ(100.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforTemperatureEquation, false).get());
  EXPECT_EQ(80.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforTemperatureEquation, false).get());
  EXPECT_EQ(100.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforTemperatureEquation, false).get());
  EXPECT_EQ(3.13878E-03, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient1, false).get());
  EXPECT_EQ(1.09689E+00, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient2, false).get());
  EXPECT_EQ(-2.63341E-05, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient3, false).get());
  EXPECT_EQ(-6.33885E+00, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient4, false).get());
  EXPECT_EQ(9.38196E-03, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient5, false).get());
  EXPECT_EQ(5.21186E-05, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient6, false).get());
  EXPECT_EQ(6.70354E-02, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient7, false).get());
  EXPECT_EQ(-1.60823E-04, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::HumidityRatioEquationCoefficient8, false).get());
  EXPECT_EQ(0.007143, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation, false).get());
  EXPECT_EQ(0.024286, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation, false).get());
  EXPECT_EQ(17.83333, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirTemperatureforHumidityRatioEquation, false).get());
  EXPECT_EQ(48.88889, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirTemperatureforHumidityRatioEquation, false).get());
  EXPECT_EQ(0.005000, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirHumidityRatioforHumidityRatioEquation, false).get());
  EXPECT_EQ(0.015714, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirHumidityRatioforHumidityRatioEquation, false).get());
  EXPECT_EQ(4.583333, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirTemperatureforHumidityRatioEquation, false).get());
  EXPECT_EQ(21.83333, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirTemperatureforHumidityRatioEquation, false).get());
  EXPECT_EQ(2.286, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationAirVelocityforHumidityRatioEquation, false).get());
  EXPECT_EQ(4.826, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationAirVelocityforHumidityRatioEquation, false).get());
  EXPECT_EQ(0.007811, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation, false).get());
  EXPECT_EQ(0.026707, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation, false).get());
  EXPECT_EQ(10.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation, false).get());
  EXPECT_EQ(100.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation, false).get());
  EXPECT_EQ(80.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MinimumProcessInletAirRelativeHumidityforHumidityRatioEquation, false).get());
  EXPECT_EQ(100.0, idfHXP.getDouble(HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1Fields::MaximumProcessInletAirRelativeHumidityforHumidityRatioEquation, false).get());
}
