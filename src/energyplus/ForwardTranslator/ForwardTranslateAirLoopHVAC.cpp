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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"

#include "../../model/AvailabilityManagerAssignmentList.hpp"
#include "../../model/AvailabilityManagerAssignmentList_Impl.hpp"
#include "../../model/AvailabilityManager.hpp"
#include "../../model/AvailabilityManager_Impl.hpp"
#include "../../model/AvailabilityManagerHybridVentilation.hpp"
#include "../../model/AvailabilityManagerHybridVentilation_Impl.hpp"
#include "../../model/SizingSystem.hpp"
#include "../../model/SizingSystem_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../model/CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include "../../model/ControllerWaterCoil.hpp"
#include "../../model/ControllerWaterCoil_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ReturnPath_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/NodeList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_ReturnAirBypassFlow_FieldEnums.hxx>
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include <utilities/idd/ConnectorList_FieldEnums.hxx>
#include <utilities/idd/Connector_Splitter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/core/Containers.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  // Attempt to add all required setpoint managers upstream of the supply outlet node.
  // This algorithm requires there to be a setpoint manager on the supply outlet node.
  // This is a good method to consider adding to Model, as a method of AirLoopHVAC class.
  void addRequiredSetpointManagers(const AirLoopHVAC& airLoopHVAC) {
    std::vector<SetpointManager> _supplyOutletSetpointManagers = airLoopHVAC.supplyOutletNode().setpointManagers();
    if (!_supplyOutletSetpointManagers.empty()) {
      auto t_model = airLoopHVAC.model();
      auto supplyComponents = airLoopHVAC.supplyComponents();

      auto fanOrUnitary = airLoopHVAC.supplyFan();

      if (!fanOrUnitary) {
        auto airLoopHVACUnitarySystems = subsetCastVector<AirLoopHVACUnitarySystem>(supplyComponents);
        if (!airLoopHVACUnitarySystems.empty()) {
          auto unitary = airLoopHVACUnitarySystems.back();
          if (unitary.supplyFan()) {
            fanOrUnitary = unitary;
          }
        }
      }

      if (!fanOrUnitary) {
        auto airLoopHVACUnitaryHeatCoolVAVChangeoverBypass = subsetCastVector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>(supplyComponents);
        if (!airLoopHVACUnitaryHeatCoolVAVChangeoverBypass.empty()) {
          fanOrUnitary = airLoopHVACUnitaryHeatCoolVAVChangeoverBypass.back();
        }
      }

      if (!fanOrUnitary) {
        auto airLoopHVACUnitaryHeatPumpAirToAir = subsetCastVector<AirLoopHVACUnitaryHeatPumpAirToAir>(supplyComponents);
        if (!airLoopHVACUnitaryHeatPumpAirToAir.empty()) {
          fanOrUnitary = airLoopHVACUnitaryHeatPumpAirToAir.back();
        }
      }

      if (!fanOrUnitary) {
        auto airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed = subsetCastVector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>(supplyComponents);
        if (!airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.empty()) {
          fanOrUnitary = airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.back();
        }
      }

      std::vector<Node> upperNodes;
      std::vector<Node> lowerNodes;
      if (fanOrUnitary) {
        upperNodes = subsetCastVector<Node>(airLoopHVAC.supplyComponents(airLoopHVAC.supplyInletNode(), fanOrUnitary.get()));
        upperNodes.erase(upperNodes.begin());
        lowerNodes = subsetCastVector<Node>(airLoopHVAC.supplyComponents(fanOrUnitary.get(), airLoopHVAC.supplyOutletNode()));
        lowerNodes.erase(lowerNodes.end() - 1);
      } else {
        // Note if we don't have a fan this is going to be a problem for EnergyPlus,
        // but at this point it will be allowed in OS
        lowerNodes = subsetCastVector<Node>(supplyComponents);
        // We should at least have a supply inlet and outlet node
        OS_ASSERT(lowerNodes.size() >= 2);
        lowerNodes.erase(lowerNodes.begin());
        lowerNodes.erase(lowerNodes.end() - 1);
      }

      auto isTemperatureControl = [](SetpointManager& spm) -> bool { return istringEqual("Temperature", spm.controlVariable()); };

      for (auto& upperNode : upperNodes) {
        std::vector<SetpointManager> _setpointManagers = upperNode.setpointManagers();
        if (std::find_if(_setpointManagers.begin(), _setpointManagers.end(), isTemperatureControl) == _setpointManagers.end()) {
          SetpointManagerMixedAir spm(t_model);
          spm.addToNode(upperNode);
          spm.setName(upperNode.name().get() + " OS Default SPM");
        }
      }

      for (auto& lowerNode : lowerNodes) {
        std::vector<SetpointManager> _setpointManagers = lowerNode.setpointManagers();
        if (std::find_if(_setpointManagers.begin(), _setpointManagers.end(), isTemperatureControl) == _setpointManagers.end()) {
          for (auto _setpointManager : _supplyOutletSetpointManagers) {
            SetpointManager spmClone = _setpointManager.clone(t_model).cast<SetpointManager>();
            spmClone.addToNode(lowerNode);
            spmClone.setName(lowerNode.name().get() + " OS Default SPM");
          }
        }
      }

      if (boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem()) {
        boost::optional<Node> outboardOANode = oaSystem->outboardOANode();
        std::vector<Node> oaNodes = subsetCastVector<Node>(oaSystem->oaComponents());
        if (outboardOANode) {
          for (auto& oaNode : oaNodes) {
            if (oaNode != outboardOANode.get()) {
              std::vector<SetpointManager> _setpointManagers = oaNode.setpointManagers();
              if (std::find_if(_setpointManagers.begin(), _setpointManagers.end(), isTemperatureControl) == _setpointManagers.end()) {
                SetpointManagerMixedAir spm(t_model);
                spm.addToNode(oaNode);
                spm.setName(oaNode.name().get() + " OS Default SPM");
              }
            }
          }
        }
      }
    }
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVAC(AirLoopHVAC& airLoopHVAC) {
    Model t_model = airLoopHVAC.model();

    addRequiredSetpointManagers(airLoopHVAC);

    // Create a new IddObjectType::AirLoopHVAC
    IdfObject idfObject(IddObjectType::AirLoopHVAC);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : airLoopHVAC.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<double> val;
    boost::optional<IdfObject> nightCycleIdf;
    std::string s;
    std::string airLoopHVACName = airLoopHVAC.name().get();

    // Name
    idfObject.setName(airLoopHVACName);

    // Supply Side Inlet Node Name
    idfObject.setString(openstudio::AirLoopHVACFields::SupplySideInletNodeName, airLoopHVAC.supplyInletNode().name().get());

    // Supply Side Outlet Node Names
    IdfObject supplyOutletNodeList(IddObjectType::NodeList);
    supplyOutletNodeList.setName(airLoopHVACName + " Supply Outlet Nodes");
    m_idfObjects.push_back(supplyOutletNodeList);
    idfObject.setString(openstudio::AirLoopHVACFields::SupplySideOutletNodeNames, supplyOutletNodeList.name().get());

    for (const auto& node : airLoopHVAC.supplyOutletNodes()) {
      auto eg = supplyOutletNodeList.pushExtensibleGroup();
      eg.setString(NodeListExtensibleFields::NodeName, node.name().get());
    }

    // Demand Side Inlet Node Names
    IdfObject demandInletNodeList(IddObjectType::NodeList);
    demandInletNodeList.setName(airLoopHVACName + " Demand Inlet Nodes");
    m_idfObjects.push_back(demandInletNodeList);
    idfObject.setString(openstudio::AirLoopHVACFields::DemandSideInletNodeNames, demandInletNodeList.name().get());

    for (const auto& node : airLoopHVAC.demandInletNodes()) {
      auto eg = demandInletNodeList.pushExtensibleGroup();
      eg.setString(NodeListExtensibleFields::NodeName, node.name().get());
    }

    // Demand Side Outlet Node Name
    idfObject.setString(openstudio::AirLoopHVACFields::DemandSideOutletNodeName, airLoopHVAC.demandOutletNode().name().get());

    // Always On Schedule
    Schedule alwaysOn = airLoopHVAC.model().alwaysOnDiscreteSchedule();
    IdfObject alwaysOnIdf = translateAndMapModelObject(alwaysOn).get();

    // Sizing System
    SizingSystem sizingSystem = airLoopHVAC.sizingSystem();
    translateAndMapModelObject(sizingSystem);

    // Controllers
    std::vector<ModelObject> controllers;
    auto supplyComponents = airLoopHVAC.supplyComponents();

    for (const auto& supplyComponent : supplyComponents) {
      boost::optional<ControllerWaterCoil> controller;

      switch (supplyComponent.iddObject().type().value()) {
        case openstudio::IddObjectType::OS_Coil_Cooling_Water: {
          controller = supplyComponent.cast<CoilCoolingWater>().controllerWaterCoil();
          break;
        }
        case openstudio::IddObjectType::OS_Coil_Heating_Water: {
          controller = supplyComponent.cast<CoilHeatingWater>().controllerWaterCoil();
          break;
        }
        case openstudio::IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted: {
          auto coolingCoil = supplyComponent.cast<CoilSystemCoolingWaterHeatExchangerAssisted>().coolingCoil();
          if (auto coilCoolingWater = coolingCoil.optionalCast<CoilCoolingWater>()) {
            controller = coilCoolingWater->controllerWaterCoil();
          }
          break;
        }
        default: {
          break;
        }
      }

      if (controller) {
        controllers.push_back(controller.get());
      }
    }

    if (controllers.size() > 0) {
      IdfObject _controllerList(IddObjectType::AirLoopHVAC_ControllerList);
      _controllerList.clearExtensibleGroups();
      _controllerList.setName(airLoopHVAC.name().get() + " Controllers");
      m_idfObjects.push_back(_controllerList);

      idfObject.setString(AirLoopHVACFields::ControllerListName, _controllerList.name().get());

      int i = 1;

      for (auto& elem : controllers) {
        boost::optional<IdfObject> _controller = translateAndMapModelObject(elem);

        if (_controller) {
          _controllerList.setString(i, _controller->iddObject().name());
          _controllerList.setString(i + 1, _controller->name().get());

          i++;
          i++;
        }
      }
    }

    // AvailabilityManagerAssignmentList
    {
      // The AvailabilityManagerAssignment list is translated by itself, we just need to set its name on the right IDF field
      AvailabilityManagerAssignmentList avmList =
        airLoopHVAC.getImpl<openstudio::model::detail::AirLoopHVAC_Impl>()->availabilityManagerAssignmentList();

      // If the avmList isn't empty of just with an HybridVentilation, it should have been translated
      if (boost::optional<IdfObject> _avmList = this->translateAndMapModelObject(avmList)) {
        idfObject.setString(AirLoopHVACFields::AvailabilityManagerListName, _avmList->name().get());
      }
      // Otherwise, we default to the old behavior
      else {
        IdfObject availabilityManagerAssignmentListIdf(openstudio::IddObjectType::AvailabilityManagerAssignmentList);
        availabilityManagerAssignmentListIdf.setName(airLoopHVACName + "Availability Manager List");
        m_idfObjects.push_back(availabilityManagerAssignmentListIdf);

        idfObject.setString(openstudio::AirLoopHVACFields::AvailabilityManagerListName, availabilityManagerAssignmentListIdf.name().get());

        IdfObject idf(IddObjectType::AvailabilityManager_Scheduled);
        idf.setName(airLoopHVACName + " Availability Manager");
        m_idfObjects.push_back(idf);

        idf.setString(openstudio::AvailabilityManager_ScheduledFields::ScheduleName, airLoopHVAC.availabilitySchedule().name().get());

        auto eg = availabilityManagerAssignmentListIdf.pushExtensibleGroup();
        eg.setString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType, idf.iddObject().name());
        eg.setString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName, idf.name().get());
      }
    }

    // Design Supply Air Flow Rate
    if (airLoopHVAC.isDesignSupplyAirFlowRateAutosized()) {
      idfObject.setString(openstudio::AirLoopHVACFields::DesignSupplyAirFlowRate, "AutoSize");
    } else if ((val = airLoopHVAC.designSupplyAirFlowRate())) {
      idfObject.setDouble(openstudio::AirLoopHVACFields::DesignSupplyAirFlowRate, val.get());
    }

    // Design Return Air Flow Fraction of Supply Air Flow
    idfObject.setDouble(openstudio::AirLoopHVACFields::DesignReturnAirFlowFractionofSupplyAirFlow,
                        airLoopHVAC.designReturnAirFlowFractionofSupplyAirFlow());

    // Branch List Name
    IdfObject branchList(openstudio::IddObjectType::BranchList);
    branchList.setName(airLoopHVACName + " Supply Branches");
    m_idfObjects.push_back(branchList);

    auto supplyInletNode = airLoopHVAC.supplyInletNode();
    auto supplyOutletNodes = airLoopHVAC.supplyOutletNodes();
    auto splitter = airLoopHVAC.supplySplitter();
    if (splitter) {
      // Dual Duct - There will be three branches, and two outlet nodes
      OS_ASSERT(supplyOutletNodes.size() == 2u);

      IdfObject _connectorList(IddObjectType::ConnectorList);
      _connectorList.setName(airLoopHVACName + " Connector List");
      m_idfObjects.push_back(_connectorList);
      idfObject.setString(AirLoopHVACFields::ConnectorListName, _connectorList.name().get());

      IdfObject _supplySplitter(IddObjectType::Connector_Splitter);
      _supplySplitter.setName(airLoopHVACName + " Supply Splitter");
      m_idfObjects.push_back(_supplySplitter);

      {
        auto eg = _connectorList.pushExtensibleGroup();
        eg.setString(ConnectorListExtensibleFields::ConnectorObjectType, _supplySplitter.iddObject().name());
        eg.setString(ConnectorListExtensibleFields::ConnectorName, _supplySplitter.name().get());
      }

      {
        auto comps = airLoopHVAC.supplyComponents(supplyInletNode, splitter.get());
        IdfObject branch(openstudio::IddObjectType::Branch);
        m_idfObjects.push_back(branch);
        branch.setName(airLoopHVACName + " Main Branch");
        populateBranch(branch, comps, airLoopHVAC, true);

        auto eg = branchList.pushExtensibleGroup();
        eg.setString(BranchListExtensibleFields::BranchName, branch.name().get());

        _supplySplitter.setString(Connector_SplitterFields::InletBranchName, branch.name().get());
      }

      {
        auto comps = airLoopHVAC.supplyComponents(splitter.get(), supplyOutletNodes[0]);
        IdfObject branch(openstudio::IddObjectType::Branch);
        m_idfObjects.push_back(branch);
        branch.setName(airLoopHVACName + " Dual Duct Branch 1");
        populateBranch(branch, comps, airLoopHVAC, true);

        auto eg = branchList.pushExtensibleGroup();
        eg.setString(BranchListExtensibleFields::BranchName, branch.name().get());

        auto splitterEg = _supplySplitter.pushExtensibleGroup();
        splitterEg.setString(Connector_SplitterExtensibleFields::OutletBranchName, branch.name().get());
      }

      {
        auto comps = airLoopHVAC.supplyComponents(splitter.get(), supplyOutletNodes[1]);
        IdfObject branch(openstudio::IddObjectType::Branch);
        m_idfObjects.push_back(branch);
        branch.setName(airLoopHVACName + " Dual Duct Branch 2");
        populateBranch(branch, comps, airLoopHVAC, true);

        auto branchListEg = branchList.pushExtensibleGroup();
        branchListEg.setString(BranchListExtensibleFields::BranchName, branch.name().get());

        auto splitterEg = _supplySplitter.pushExtensibleGroup();
        splitterEg.setString(Connector_SplitterExtensibleFields::OutletBranchName, branch.name().get());
      }
    } else {
      OS_ASSERT(supplyOutletNodes.size() == 1u);
      // Single Duct - Everything goes on one branch
      auto comps = airLoopHVAC.supplyComponents(supplyInletNode, supplyOutletNodes[0]);

      IdfObject branch(openstudio::IddObjectType::Branch);
      m_idfObjects.push_back(branch);
      branch.setName(airLoopHVACName + " Main Branch");
      populateBranch(branch, comps, airLoopHVAC, true);

      auto eg = branchList.pushExtensibleGroup();
      eg.setString(BranchListExtensibleFields::BranchName, branch.name().get());
    }

    idfObject.setString(openstudio::AirLoopHVACFields::BranchListName, branchList.name().get());

    // Convert demand side components
    for (const auto& demandInletNode : airLoopHVAC.demandInletNodes()) {
      createAirLoopHVACSupplyPath(demandInletNode);
    }
    createAirLoopHVACReturnPath(airLoopHVAC);

    std::vector<ModelObject> demandComponents = airLoopHVAC.demandComponents();
    for (auto& demandComponent : demandComponents) {
      translateAndMapModelObject(demandComponent);
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
