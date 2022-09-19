/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../ChillerElectricASHRAE205.hpp"
#include "../ChillerElectricASHRAE205_Impl.hpp"

#include "../ExternalFile.hpp"
#include "../ExternalFile_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../Mixer.hpp"
#include "../Splitter.hpp"

#include "../../utilities/core/PathHelpers.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ChillerElectricASHRAE205_GettersSetters) {
  Model m;

  EXPECT_EQ(0, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());

  openstudio::path p = resourcesPath() / toPath("model/A205ExampleChiller.RS0001.a205.cbor");
  EXPECT_TRUE(exists(p));

  openstudio::path expectedDestDir;
  std::vector<openstudio::path> absoluteFilePaths = m.workflowJSON().absoluteFilePaths();
  if (absoluteFilePaths.empty()) {
    expectedDestDir = m.workflowJSON().absoluteRootDir();
  } else {
    expectedDestDir = absoluteFilePaths[0];
  }

  if (exists(expectedDestDir)) {
    removeDirectory(expectedDestDir);
  }
  ASSERT_FALSE(exists(expectedDestDir));
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> representationFile = ExternalFile::getExternalFile(m, openstudio::toString(p));
  ASSERT_TRUE(representationFile);

  EXPECT_EQ(1, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0, representationFile->chillerElectricASHRAE205s().size());
  EXPECT_EQ(openstudio::toString(p.filename()), representationFile->fileName());
  EXPECT_TRUE(equivalent(expectedDestDir / representationFile->fileName(), representationFile->filePath()));
  EXPECT_TRUE(exists(representationFile->filePath()));
  EXPECT_NE(p, representationFile->filePath());

  ChillerElectricASHRAE205 ch(representationFile.get());
  EXPECT_EQ(1, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());
  EXPECT_EQ(1, representationFile->chillerElectricASHRAE205s().size());
  EXPECT_EQ(representationFile->handle(), ch.representationFile().handle());

  // Representation File Name: Required Object
  // ExternalFile obj(m);
  // EXPECT_TRUE(ch.setRepresentationFile(obj));
  // EXPECT_EQ(obj, ch.representationFile());

  // Performance Interpolation Method: Required String
  // Ctor default
  EXPECT_EQ("Linear", ch.performanceInterpolationMethod());
  EXPECT_TRUE(ch.setPerformanceInterpolationMethod("Cubic"));
  EXPECT_EQ("Cubic", ch.performanceInterpolationMethod());
  // Bad Value
  EXPECT_FALSE(ch.setPerformanceInterpolationMethod("BADENUM"));
  EXPECT_EQ("Cubic", ch.performanceInterpolationMethod());

  // Rated Capacity: Required Double
  // Autosize
  ch.autosizeRatedCapacity();
  EXPECT_TRUE(ch.isRatedCapacityAutosized());
  // Set
  EXPECT_TRUE(ch.setRatedCapacity(0.5));
  ASSERT_TRUE(ch.ratedCapacity());
  EXPECT_EQ(0.5, ch.ratedCapacity().get());
  EXPECT_FALSE(ch.isRatedCapacityAutosized());
  // Bad Value
  EXPECT_FALSE(ch.setRatedCapacity(-10.0));
  ASSERT_TRUE(ch.ratedCapacity());
  EXPECT_EQ(0.5, ch.ratedCapacity().get());
  EXPECT_FALSE(ch.isRatedCapacityAutosized());

  // Sizing Factor: Required Double
  // Ctor default
  EXPECT_EQ(1.0, ch.sizingFactor());
  EXPECT_TRUE(ch.setSizingFactor(0.6));
  EXPECT_EQ(0.6, ch.sizingFactor());
  // Bad Value
  EXPECT_FALSE(ch.setSizingFactor(-10.0));
  EXPECT_EQ(0.6, ch.sizingFactor());

  // Ambient Temperature Indicator: Required String
  EXPECT_EQ("Outdoors", ch.ambientTemperatureIndicator());

  // EXPECT_TRUE(ch.setAmbientTemperatureIndicator("Schedule"));
  // EXPECT_EQ("Schedule", ch.ambientTemperatureIndicator());
  // // Bad Value
  // EXPECT_FALSE(ch.setAmbientTemperatureIndicator("BADENUM"));
  // EXPECT_EQ("Schedule", ch.ambientTemperatureIndicator());

  // Ambient Temperature Schedule Name: Optional Object
  ScheduleConstant tempSch(m);
  EXPECT_TRUE(ch.setAmbientTemperatureSchedule(tempSch));
  ASSERT_TRUE(ch.ambientTemperatureSchedule());
  EXPECT_EQ(tempSch, ch.ambientTemperatureSchedule().get());
  EXPECT_EQ("Schedule", ch.ambientTemperatureIndicator());
  ch.resetAmbientTemperatureOutdoorAirNodeName();
  EXPECT_EQ("Schedule", ch.ambientTemperatureIndicator());
  ch.resetAmbientTemperatureSchedule();
  EXPECT_EQ("Outdoors", ch.ambientTemperatureIndicator());

  // Ambient Temperature Zone Name: Optional Object
  ThermalZone z(m);
  EXPECT_TRUE(ch.setAmbientTemperatureZone(z));
  ASSERT_TRUE(ch.ambientTemperatureZone());
  EXPECT_EQ(z, ch.ambientTemperatureZone().get());
  EXPECT_EQ("Zone", ch.ambientTemperatureIndicator());
  ch.resetAmbientTemperatureSchedule();
  EXPECT_EQ("Zone", ch.ambientTemperatureIndicator());
  ch.resetAmbientTemperatureZone();
  EXPECT_EQ("Outdoors", ch.ambientTemperatureIndicator());

  EXPECT_TRUE(ch.setAmbientTemperatureZone(z));
  EXPECT_EQ("Zone", ch.ambientTemperatureIndicator());
  ch.resetAmbientTemperatureOutdoorAirNodeName();
  EXPECT_EQ("Zone", ch.ambientTemperatureIndicator());

  // Ambient Temperature Outdoor Air Node Name: Optional Object
  EXPECT_TRUE(ch.setAmbientTemperatureOutdoorAirNodeName("My Outdoor Air Node Name"));
  ASSERT_TRUE(ch.ambientTemperatureOutdoorAirNodeName());
  EXPECT_EQ("My Outdoor Air Node Name", ch.ambientTemperatureOutdoorAirNodeName().get());
  EXPECT_EQ("Outdoors", ch.ambientTemperatureIndicator());

  // Chilled Water Maximum Requested Flow Rate: Required Double
  // Ctor default
  EXPECT_TRUE(ch.isChilledWaterMaximumRequestedFlowRateAutosized());
  // Set
  EXPECT_TRUE(ch.setChilledWaterMaximumRequestedFlowRate(1.3));
  ASSERT_TRUE(ch.chilledWaterMaximumRequestedFlowRate());
  EXPECT_EQ(1.3, ch.chilledWaterMaximumRequestedFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setChilledWaterMaximumRequestedFlowRate(-10.0));
  ASSERT_TRUE(ch.chilledWaterMaximumRequestedFlowRate());
  EXPECT_EQ(1.3, ch.chilledWaterMaximumRequestedFlowRate().get());
  EXPECT_FALSE(ch.isChilledWaterMaximumRequestedFlowRateAutosized());
  // Autosize
  ch.autosizeChilledWaterMaximumRequestedFlowRate();
  EXPECT_TRUE(ch.isChilledWaterMaximumRequestedFlowRateAutosized());

  // Condenser Maximum Requested Flow Rate: Required Double
  // Ctor default
  EXPECT_TRUE(ch.isCondenserMaximumRequestedFlowRateAutosized());
  // Set
  EXPECT_TRUE(ch.setCondenserMaximumRequestedFlowRate(1.6));
  ASSERT_TRUE(ch.condenserMaximumRequestedFlowRate());
  EXPECT_EQ(1.6, ch.condenserMaximumRequestedFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setCondenserMaximumRequestedFlowRate(-10.0));
  ASSERT_TRUE(ch.condenserMaximumRequestedFlowRate());
  EXPECT_EQ(1.6, ch.condenserMaximumRequestedFlowRate().get());
  EXPECT_FALSE(ch.isCondenserMaximumRequestedFlowRateAutosized());
  // Autosize
  ch.autosizeCondenserMaximumRequestedFlowRate();
  EXPECT_TRUE(ch.isCondenserMaximumRequestedFlowRateAutosized());

  // Chiller Flow Mode: Required String
  EXPECT_TRUE(ch.setChillerFlowMode("ConstantFlow"));
  EXPECT_EQ("ConstantFlow", ch.chillerFlowMode());
  // Bad Value
  EXPECT_FALSE(ch.setChillerFlowMode("BADENUM"));
  EXPECT_EQ("ConstantFlow", ch.chillerFlowMode());

  // Oil Cooler Design Flow Rate: Optional Double
  EXPECT_TRUE(ch.setOilCoolerDesignFlowRate(2.0));
  ASSERT_TRUE(ch.oilCoolerDesignFlowRate());
  EXPECT_EQ(2.0, ch.oilCoolerDesignFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setOilCoolerDesignFlowRate(-10.0));
  ASSERT_TRUE(ch.oilCoolerDesignFlowRate());
  EXPECT_EQ(2.0, ch.oilCoolerDesignFlowRate().get());

  // Auxiliary Cooling Design Flow Rate: Optional Double
  EXPECT_TRUE(ch.setAuxiliaryCoolingDesignFlowRate(2.3));
  ASSERT_TRUE(ch.auxiliaryCoolingDesignFlowRate());
  EXPECT_EQ(2.3, ch.auxiliaryCoolingDesignFlowRate().get());
  // Bad Value
  EXPECT_FALSE(ch.setAuxiliaryCoolingDesignFlowRate(-10.0));
  ASSERT_TRUE(ch.auxiliaryCoolingDesignFlowRate());
  EXPECT_EQ(2.3, ch.auxiliaryCoolingDesignFlowRate().get());

  // End-Use Subcategory: Optional String
  // Default value from IDD
  EXPECT_EQ("General", ch.endUseSubcategory());
  // Set
  EXPECT_TRUE(ch.setEndUseSubcategory("Chiller"));
  EXPECT_EQ("Chiller", ch.endUseSubcategory());
  EXPECT_FALSE(ch.isEndUseSubcategoryDefaulted());
  // Reset
  ch.resetEndUseSubcategory();
  EXPECT_TRUE(ch.isEndUseSubcategoryDefaulted());
  EXPECT_EQ("General", ch.endUseSubcategory());

  // Removing the ExternalFile removes the Chiller
  openstudio::path dstPath = representationFile->filePath();
  EXPECT_TRUE(exists(p));
  EXPECT_TRUE(exists(dstPath));

  representationFile->remove();
  EXPECT_EQ(0, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());

  EXPECT_TRUE(exists(p));
  EXPECT_FALSE(exists(dstPath));
}

TEST_F(ModelFixture, ChillerElectricASHRAE205_Loops) {
  Model m;

  EXPECT_EQ(0, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());

  openstudio::path p = resourcesPath() / toPath("model/A205ExampleChiller.RS0001.a205.cbor");
  EXPECT_TRUE(exists(p));

  openstudio::path expectedDestDir;
  std::vector<openstudio::path> absoluteFilePaths = m.workflowJSON().absoluteFilePaths();
  if (absoluteFilePaths.empty()) {
    expectedDestDir = m.workflowJSON().absoluteRootDir();
  } else {
    expectedDestDir = absoluteFilePaths[0];
  }

  if (exists(expectedDestDir)) {
    removeDirectory(expectedDestDir);
  }
  ASSERT_FALSE(exists(expectedDestDir));
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> representationFile = ExternalFile::getExternalFile(m, openstudio::toString(p));
  ASSERT_TRUE(representationFile);

  EXPECT_EQ(1, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0, representationFile->chillerElectricASHRAE205s().size());
  EXPECT_EQ(openstudio::toString(p.filename()), representationFile->fileName());
  EXPECT_TRUE(equivalent(expectedDestDir / representationFile->fileName(), representationFile->filePath()));
  EXPECT_TRUE(exists(representationFile->filePath()));
  EXPECT_NE(p, representationFile->filePath());

  ChillerElectricASHRAE205 ch(representationFile.get());
  EXPECT_FALSE(ch.chilledWaterLoop());
  EXPECT_FALSE(ch.chilledWaterInletNode());
  EXPECT_FALSE(ch.chilledWaterOutletNode());

  EXPECT_FALSE(ch.condenserWaterLoop());
  EXPECT_FALSE(ch.condenserInletNode());
  EXPECT_FALSE(ch.condenserOutletNode());

  EXPECT_FALSE(ch.heatRecoveryLoop());
  EXPECT_FALSE(ch.heatRecoveryInletNode());
  EXPECT_FALSE(ch.heatRecoveryOutletNode());

  EXPECT_FALSE(ch.oilCoolerLoop());
  EXPECT_FALSE(ch.oilCoolerInletNode());
  EXPECT_FALSE(ch.oilCoolerOutletNode());

  EXPECT_FALSE(ch.auxiliaryLoop());
  EXPECT_FALSE(ch.auxiliaryInletNode());
  EXPECT_FALSE(ch.auxiliaryOutletNode());

  auto createLoop = [&m](const std::string& prefix) {
    PlantLoop p(m);
    static constexpr std::array<std::string_view, 10> compNames = {
      "Supply Inlet", "Supply Splitter", "Supply Connection Node", "Supply Mixer", "Supply Outlet",
      "Demand Inlet", "Demand Splitter", "Demand Connection Node", "Demand Mixer", "Demand Outlet",
    };
    p.setName(prefix);
    for (size_t i = 0; auto& comp : p.components()) {
      comp.setName(prefix + " " + std::string{compNames[i++]});
    }
    return p;
  };

  auto chwLoop = createLoop("chwLoop");
  auto cndLoop = createLoop("cndLoop");
  auto hrLoop = createLoop("hrLoop");
  auto ocLoop = createLoop("ocLoop");
  auto auxLoop = createLoop("auxLoop");

  // Chilled Water
  {
    EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(ch));
    ASSERT_TRUE(ch.chilledWaterLoop());
    ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
    EXPECT_TRUE(ch.chilledWaterInletNode());
    EXPECT_TRUE(ch.chilledWaterOutletNode());

    EXPECT_FALSE(ch.condenserWaterLoop());
    EXPECT_FALSE(ch.condenserInletNode());
    EXPECT_FALSE(ch.condenserOutletNode());

    EXPECT_FALSE(ch.heatRecoveryLoop());
    EXPECT_FALSE(ch.heatRecoveryInletNode());
    EXPECT_FALSE(ch.heatRecoveryOutletNode());

    EXPECT_FALSE(ch.oilCoolerLoop());
    EXPECT_FALSE(ch.oilCoolerInletNode());
    EXPECT_FALSE(ch.oilCoolerOutletNode());

    EXPECT_FALSE(ch.auxiliaryLoop());
    EXPECT_FALSE(ch.auxiliaryInletNode());
    EXPECT_FALSE(ch.auxiliaryOutletNode());
  }

  // Condenser
  {
    EXPECT_TRUE(cndLoop.addDemandBranchForComponent(ch));
    ASSERT_TRUE(ch.chilledWaterLoop());
    EXPECT_EQ(chwLoop, ch.chilledWaterLoop().get());
    EXPECT_TRUE(ch.chilledWaterInletNode());
    EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
    EXPECT_TRUE(ch.chilledWaterOutletNode());
    EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

    ASSERT_TRUE(ch.condenserWaterLoop());
    EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
    ASSERT_TRUE(ch.condenserInletNode());
    EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
    ASSERT_TRUE(ch.condenserOutletNode());
    EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

    EXPECT_FALSE(ch.heatRecoveryLoop());
    EXPECT_FALSE(ch.heatRecoveryInletNode());
    EXPECT_FALSE(ch.heatRecoveryOutletNode());

    EXPECT_FALSE(ch.oilCoolerLoop());
    EXPECT_FALSE(ch.oilCoolerInletNode());
    EXPECT_FALSE(ch.oilCoolerOutletNode());

    EXPECT_FALSE(ch.auxiliaryLoop());
    EXPECT_FALSE(ch.auxiliaryInletNode());
    EXPECT_FALSE(ch.auxiliaryOutletNode());
  }

  auto checkHeatRecoveryLoop = [&ch, &hrLoop]() {
    if constexpr (ChillerElectricASHRAE205::isHeatRecoverySupportedByEnergyplus) {

      ASSERT_TRUE(ch.heatRecoveryLoop());
      EXPECT_EQ(hrLoop, ch.heatRecoveryLoop().get());
      ASSERT_TRUE(ch.heatRecoveryInletNode());
      EXPECT_EQ(ch.tertiaryInletModelObject()->handle(), ch.heatRecoveryInletNode()->handle());
      ASSERT_TRUE(ch.heatRecoveryOutletNode());
      EXPECT_EQ(ch.tertiaryOutletModelObject()->handle(), ch.heatRecoveryOutletNode()->handle());
    } else {
      EXPECT_FALSE(ch.heatRecoveryLoop());
      EXPECT_FALSE(ch.heatRecoveryInletNode());
      EXPECT_FALSE(ch.heatRecoveryOutletNode());
    }
  };

  // Heat Recovery
  {
    if constexpr (ChillerElectricASHRAE205::isHeatRecoverySupportedByEnergyplus) {
      EXPECT_TRUE(hrLoop.addDemandBranchForComponent(ch));
    } else {
      EXPECT_FALSE(hrLoop.addDemandBranchForComponent(ch));
    }

    ASSERT_TRUE(ch.chilledWaterLoop());
    ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
    EXPECT_TRUE(ch.chilledWaterInletNode());
    EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
    EXPECT_TRUE(ch.chilledWaterOutletNode());
    EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

    ASSERT_TRUE(ch.condenserWaterLoop());
    EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
    ASSERT_TRUE(ch.condenserInletNode());
    EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
    ASSERT_TRUE(ch.condenserOutletNode());
    EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

    checkHeatRecoveryLoop();

    EXPECT_FALSE(ch.oilCoolerLoop());
    EXPECT_FALSE(ch.oilCoolerInletNode());
    EXPECT_FALSE(ch.oilCoolerOutletNode());

    EXPECT_FALSE(ch.auxiliaryLoop());
    EXPECT_FALSE(ch.auxiliaryInletNode());
    EXPECT_FALSE(ch.auxiliaryOutletNode());
  }

  // Oil Cooler Loop
  {
    auto ocDemandConnectionNodes = ocLoop.demandComponents(ocLoop.demandSplitter(), ocLoop.demandMixer(), IddObjectType::OS_Node);

    // addToNode
    {
      ASSERT_EQ(1, ocDemandConnectionNodes.size());
      auto ocDemandConnectionNode = ocDemandConnectionNodes[0].cast<Node>();
      EXPECT_EQ(5, ocLoop.demandComponents().size());
      EXPECT_TRUE(ch.addToOilCoolerLoopNode(ocDemandConnectionNode));
      EXPECT_EQ(7, ocLoop.demandComponents().size());

      ASSERT_TRUE(ch.chilledWaterLoop());
      ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
      EXPECT_TRUE(ch.chilledWaterInletNode());
      EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
      EXPECT_TRUE(ch.chilledWaterOutletNode());
      EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

      ASSERT_TRUE(ch.condenserWaterLoop());
      EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
      ASSERT_TRUE(ch.condenserInletNode());
      EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
      ASSERT_TRUE(ch.condenserOutletNode());
      EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

      checkHeatRecoveryLoop();

      ASSERT_TRUE(ch.oilCoolerLoop());
      EXPECT_EQ(ocLoop, ch.oilCoolerLoop().get());
      ASSERT_TRUE(ch.oilCoolerInletNode());
      EXPECT_EQ(ch.oilCoolerInletModelObject()->handle(), ch.oilCoolerInletNode()->handle());
      ASSERT_TRUE(ch.oilCoolerOutletNode());
      EXPECT_EQ(ch.oilCoolerOutletModelObject()->handle(), ch.oilCoolerOutletNode()->handle());

      EXPECT_FALSE(ch.auxiliaryLoop());
      EXPECT_FALSE(ch.auxiliaryInletNode());
      EXPECT_FALSE(ch.auxiliaryOutletNode());
    }

    // Try removeFromLoop
    {
      ch.removeFromOilCoolerLoop();
      ASSERT_TRUE(ch.chilledWaterLoop());
      ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
      EXPECT_TRUE(ch.chilledWaterInletNode());
      EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
      EXPECT_TRUE(ch.chilledWaterOutletNode());
      EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

      ASSERT_TRUE(ch.condenserWaterLoop());
      EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
      ASSERT_TRUE(ch.condenserInletNode());
      EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
      ASSERT_TRUE(ch.condenserOutletNode());
      EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

      checkHeatRecoveryLoop();

      EXPECT_FALSE(ch.oilCoolerLoop());
      EXPECT_FALSE(ch.oilCoolerInletNode());
      EXPECT_FALSE(ch.oilCoolerOutletNode());

      EXPECT_FALSE(ch.auxiliaryLoop());
      EXPECT_FALSE(ch.auxiliaryInletNode());
      EXPECT_FALSE(ch.auxiliaryOutletNode());

      EXPECT_EQ(5, ocLoop.demandComponents().size());
    }
    // Try addDemandBranchOnLoop
    {

      EXPECT_TRUE(ch.addDemandBranchOnOilCoolerLoop(ocLoop));
      EXPECT_EQ(7, ocLoop.demandComponents().size());

      ASSERT_TRUE(ch.chilledWaterLoop());
      ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
      EXPECT_TRUE(ch.chilledWaterInletNode());
      EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
      EXPECT_TRUE(ch.chilledWaterOutletNode());
      EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

      ASSERT_TRUE(ch.condenserWaterLoop());
      EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
      ASSERT_TRUE(ch.condenserInletNode());
      EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
      ASSERT_TRUE(ch.condenserOutletNode());
      EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

      checkHeatRecoveryLoop();

      ASSERT_TRUE(ch.oilCoolerLoop());
      EXPECT_EQ(ocLoop, ch.oilCoolerLoop().get());
      ASSERT_TRUE(ch.oilCoolerInletNode());
      EXPECT_EQ(ch.oilCoolerInletModelObject()->handle(), ch.oilCoolerInletNode()->handle());
      ASSERT_TRUE(ch.oilCoolerOutletNode());
      EXPECT_EQ(ch.oilCoolerOutletModelObject()->handle(), ch.oilCoolerOutletNode()->handle());

      EXPECT_FALSE(ch.auxiliaryLoop());
      EXPECT_FALSE(ch.auxiliaryInletNode());
      EXPECT_FALSE(ch.auxiliaryOutletNode());
    }
  }

  // Auxiliary Loop
  {
    // AddToNode
    {
      auto auxDemandConnectionNodes = auxLoop.demandComponents(auxLoop.demandSplitter(), auxLoop.demandMixer(), IddObjectType::OS_Node);
      ASSERT_EQ(1, auxDemandConnectionNodes.size());
      auto auxDemandConnectionNode = auxDemandConnectionNodes[0].cast<Node>();
      EXPECT_EQ(5, auxLoop.demandComponents().size());
      EXPECT_TRUE(ch.addToAuxiliaryLoopNode(auxDemandConnectionNode));
      EXPECT_EQ(7, auxLoop.demandComponents().size());

      ASSERT_TRUE(ch.chilledWaterLoop());
      ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
      ASSERT_TRUE(ch.plantLoop());
      ASSERT_EQ(chwLoop, ch.plantLoop().get());
      EXPECT_TRUE(ch.chilledWaterInletNode());
      EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
      EXPECT_TRUE(ch.chilledWaterOutletNode());
      EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

      ASSERT_TRUE(ch.condenserWaterLoop());
      EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
      ASSERT_TRUE(ch.secondaryPlantLoop());
      ASSERT_EQ(cndLoop, ch.secondaryPlantLoop().get());
      ASSERT_TRUE(ch.condenserInletNode());
      EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
      ASSERT_TRUE(ch.condenserOutletNode());
      EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

      checkHeatRecoveryLoop();

      ASSERT_TRUE(ch.oilCoolerLoop());
      EXPECT_EQ(ocLoop, ch.oilCoolerLoop().get());
      ASSERT_TRUE(ch.oilCoolerInletNode());
      EXPECT_EQ(ch.oilCoolerInletModelObject()->handle(), ch.oilCoolerInletNode()->handle());
      ASSERT_TRUE(ch.oilCoolerOutletNode());
      EXPECT_EQ(ch.oilCoolerOutletModelObject()->handle(), ch.oilCoolerOutletNode()->handle());
    }
    {
      // Try removeFromLoop
      ch.removeFromAuxiliaryLoop();
      EXPECT_EQ(5, auxLoop.demandComponents().size());

      ASSERT_TRUE(ch.chilledWaterLoop());
      ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
      EXPECT_TRUE(ch.chilledWaterInletNode());
      EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
      EXPECT_TRUE(ch.chilledWaterOutletNode());
      EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

      ASSERT_TRUE(ch.condenserWaterLoop());
      EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
      ASSERT_TRUE(ch.condenserInletNode());
      EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
      ASSERT_TRUE(ch.condenserOutletNode());
      EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

      checkHeatRecoveryLoop();

      ASSERT_TRUE(ch.oilCoolerLoop());
      EXPECT_EQ(ocLoop, ch.oilCoolerLoop().get());
      ASSERT_TRUE(ch.oilCoolerInletNode());
      EXPECT_EQ(ch.oilCoolerInletModelObject()->handle(), ch.oilCoolerInletNode()->handle());
      ASSERT_TRUE(ch.oilCoolerOutletNode());
      EXPECT_EQ(ch.oilCoolerOutletModelObject()->handle(), ch.oilCoolerOutletNode()->handle());

      EXPECT_FALSE(ch.auxiliaryLoop());
      EXPECT_FALSE(ch.auxiliaryInletNode());
      EXPECT_FALSE(ch.auxiliaryOutletNode());
      EXPECT_EQ(5, auxLoop.demandComponents().size());
    }

    {
      // Try addDemandBranchOnLoop
      EXPECT_TRUE(ch.addDemandBranchOnAuxiliaryLoop(auxLoop));
      EXPECT_EQ(7, auxLoop.demandComponents().size());

      ASSERT_TRUE(ch.chilledWaterLoop());
      ASSERT_EQ(chwLoop, ch.chilledWaterLoop().get());
      ASSERT_TRUE(ch.plantLoop());
      ASSERT_EQ(chwLoop, ch.plantLoop().get());
      EXPECT_TRUE(ch.chilledWaterInletNode());
      EXPECT_EQ(ch.supplyInletModelObject()->handle(), ch.chilledWaterInletNode()->handle());
      EXPECT_TRUE(ch.chilledWaterOutletNode());
      EXPECT_EQ(ch.supplyOutletModelObject()->handle(), ch.chilledWaterOutletNode()->handle());

      ASSERT_TRUE(ch.condenserWaterLoop());
      EXPECT_EQ(cndLoop, ch.condenserWaterLoop().get());
      ASSERT_TRUE(ch.secondaryPlantLoop());
      ASSERT_EQ(cndLoop, ch.secondaryPlantLoop().get());
      ASSERT_TRUE(ch.condenserInletNode());
      EXPECT_EQ(ch.demandInletModelObject()->handle(), ch.condenserInletNode()->handle());
      ASSERT_TRUE(ch.condenserOutletNode());
      EXPECT_EQ(ch.demandOutletModelObject()->handle(), ch.condenserOutletNode()->handle());

      checkHeatRecoveryLoop();

      ASSERT_TRUE(ch.oilCoolerLoop());
      EXPECT_EQ(ocLoop, ch.oilCoolerLoop().get());
      ASSERT_TRUE(ch.oilCoolerInletNode());
      EXPECT_EQ(ch.oilCoolerInletModelObject()->handle(), ch.oilCoolerInletNode()->handle());
      ASSERT_TRUE(ch.oilCoolerOutletNode());
      EXPECT_EQ(ch.oilCoolerOutletModelObject()->handle(), ch.oilCoolerOutletNode()->handle());
    }
  }

  // Removing the Chiller doesn't remove the ExternalFile
  openstudio::path dstPath = representationFile->filePath();
  EXPECT_TRUE(exists(p));
  EXPECT_TRUE(exists(dstPath));

  ch.remove();
  EXPECT_EQ(1, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());

  EXPECT_TRUE(exists(p));
  EXPECT_TRUE(exists(dstPath));
}

TEST_F(ModelFixture, ChillerElectricASHRAE205_NotCBORFile) {
  Model model;

  openstudio::path p = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> representationFile = ExternalFile::getExternalFile(model, openstudio::toString(p));
  ASSERT_TRUE(representationFile);

  ASSERT_THROW(ChillerElectricASHRAE205{representationFile.get()}, openstudio::Exception);
}

TEST_F(ModelFixture, ChillerElectricASHRAE205_Clone) {
  Model m;

  openstudio::path p = resourcesPath() / toPath("model/A205ExampleChiller.RS0001.a205.cbor");
  EXPECT_TRUE(exists(p));

  openstudio::path expectedDestDir;
  std::vector<openstudio::path> absoluteFilePaths = m.workflowJSON().absoluteFilePaths();
  if (absoluteFilePaths.empty()) {
    expectedDestDir = m.workflowJSON().absoluteRootDir();
  } else {
    expectedDestDir = absoluteFilePaths[0];
  }

  if (exists(expectedDestDir)) {
    removeDirectory(expectedDestDir);
  }
  ASSERT_FALSE(exists(expectedDestDir));
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> representationFile = ExternalFile::getExternalFile(m, openstudio::toString(p));
  ASSERT_TRUE(representationFile);

  ChillerElectricASHRAE205 ch(representationFile.get());

  EXPECT_EQ(1, m.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());
  EXPECT_EQ(representationFile->handle(), ch.representationFile().handle());
  EXPECT_EQ(1, representationFile->chillerElectricASHRAE205s().size());

  {
    // Another model first, so we don't mess the original one
    Model m2;
    auto ch2 = ch.clone(m2).cast<ChillerElectricASHRAE205>();

    ASSERT_EQ(1, m2.getConcreteModelObjects<ExternalFile>().size());
    auto representationFile2 = m2.getConcreteModelObjects<ExternalFile>().front();
    EXPECT_EQ(1, m2.getConcreteModelObjects<ChillerElectricASHRAE205>().size());
    EXPECT_EQ(representationFile2.handle(), ch2.representationFile().handle());
    EXPECT_NE(representationFile->handle(), ch2.representationFile().handle());
    EXPECT_EQ(1, representationFile2.chillerElectricASHRAE205s().size());
  }

  {
    // Same Model, should point to the same ExternaFile
    auto ch2 = ch.clone(m).cast<ChillerElectricASHRAE205>();
    EXPECT_EQ(1, m.getConcreteModelObjects<ExternalFile>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<ChillerElectricASHRAE205>().size());
    EXPECT_EQ(representationFile->handle(), ch.representationFile().handle());
    EXPECT_EQ(representationFile->handle(), ch2.representationFile().handle());
    EXPECT_EQ(2, representationFile->chillerElectricASHRAE205s().size());
  }
}
