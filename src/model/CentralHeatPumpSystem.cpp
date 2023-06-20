/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CentralHeatPumpSystem.hpp"
#include "CentralHeatPumpSystem_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "CentralHeatPumpSystemModule.hpp"
#include "CentralHeatPumpSystemModule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"

// Need for clone override
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_CentralHeatPumpSystem_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CentralHeatPumpSystem::iddObjectType());
    }

    CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CentralHeatPumpSystem::iddObjectType());
    }

    CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const CentralHeatPumpSystem_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CentralHeatPumpSystem_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Chiller Heater System Cooling Electricity Rate",
                                                   "Chiller Heater System Cooling Electricity Energy",
                                                   "Chiller Heater System Heating Electricity Rate",
                                                   "Chiller Heater System Heating Electricity Energy",
                                                   "Chiller Heater System Cooling Rate",
                                                   "Chiller Heater System Cooling Energy",
                                                   "Chiller Heater System Heating Rate",
                                                   "Chiller Heater System Heating Energy",
                                                   "Chiller Heater System Source Heat Transfer Rate",
                                                   "Chiller Heater System Source Heat Transfer Energy",
                                                   "Chiller Heater System Cooling Inlet Temperature",
                                                   "Chiller Heater System Cooling Outlet Temperature",
                                                   "Chiller Heater System Cooling Mass Flow Rate",
                                                   "Chiller Heater System Heating Inlet Temperature",
                                                   "Chiller Heater System Heating Outlet Temperature",
                                                   "Chiller Heater System Heating Mass Flow Rate",
                                                   "Chiller Heater System Source Inlet Temperature",
                                                   "Chiller Heater System Source Outlet Temperature",
                                                   "Chiller Heater System Source Mass Flow Rate"};
      return result;
    }

    IddObjectType CentralHeatPumpSystem_Impl::iddObjectType() const {
      return CentralHeatPumpSystem::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CentralHeatPumpSystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName) != e) {
        result.push_back(ScheduleTypeKey("CentralHeatPumpSystem", "Ancillary Operation"));
      }
      return result;
    }

    // This will clone the CentralHeatPumpSystem as well as the CentralHeatPumpSystemModules if there are some
    // and will return a reference to the new CentralHeatPumpSystem
    ModelObject CentralHeatPumpSystem_Impl::clone(Model model) const {

      // Call the WaterToWater clone method, for a clean one, and that will reset the connections to loops
      auto newCentralHP = WaterToWaterComponent_Impl::clone(model).cast<CentralHeatPumpSystem>();

      // In the CentralHeatPumpSystem Implementation, the actual important object is the chillerHeaterModuleList
      // Create a new (blank) ModelObjectList, and set it
      auto newChillerHeaterModuleList = ModelObjectList(model);
      newChillerHeaterModuleList.setName(newCentralHP.name().get() + " Chiller Heater Module List");
      bool ok = true;
      ok = newCentralHP.getImpl<detail::CentralHeatPumpSystem_Impl>()->setChillerHeaterModuleList(newChillerHeaterModuleList);
      OS_ASSERT(ok);

      // Now, the matter is to repopulate it.
      for (const CentralHeatPumpSystemModule& centralHPMod : this->modules()) {

        // The CentralHeatPumpSystemModule_Impl::clone method will set the reference to the same
        // ChillerHeaterPerformanceElectricEIR as the original one
        auto centralHPModClone = centralHPMod.clone(model).cast<CentralHeatPumpSystemModule>();

        // Add that to the new object
        newCentralHP.addModule(centralHPModClone);
      }

      return std::move(newCentralHP);
    }

    // CoolingLoop
    unsigned CentralHeatPumpSystem_Impl::supplyInletPort() const {
      return OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName;
    }

    unsigned CentralHeatPumpSystem_Impl::supplyOutletPort() const {
      return OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName;
    }

    // SourceLoop
    unsigned CentralHeatPumpSystem_Impl::demandInletPort() const {
      return OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName;
    }

    unsigned CentralHeatPumpSystem_Impl::demandOutletPort() const {
      return OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName;
    }

    // HeatingLoop
    unsigned CentralHeatPumpSystem_Impl::tertiaryInletPort() const {
      return OS_CentralHeatPumpSystemFields::HeatingLoopInletNodeName;
    }

    unsigned CentralHeatPumpSystem_Impl::tertiaryOutletPort() const {
      return OS_CentralHeatPumpSystemFields::HeatingLoopOutletNodeName;
    }

    bool CentralHeatPumpSystem_Impl::addToNode(Node& node) {
      boost::optional<PlantLoop> t_plantLoop = node.plantLoop();

      // If trying to add to a node that is on the supply side of a plant loop
      if (t_plantLoop) {
        if (t_plantLoop->supplyComponent(node.handle())) {
          // If there is already a cooling Plant Loop
          boost::optional<PlantLoop> coolingPlant = this->coolingPlantLoop();
          if (coolingPlant) {
            // And it's not the same as the node's loop
            if (t_plantLoop.get() != coolingPlant.get()) {
              // And if there is no heatingPlantLoop (tertiary)
              boost::optional<PlantLoop> heatingPlant = this->heatingPlantLoop();
              if (!heatingPlant) {
                // Then try to add it to the tertiary one
                LOG(Warn, "Calling addToTertiaryNode to connect it to the tertiary (=heating) loop for " << briefDescription());
                return this->addToTertiaryNode(node);
              }
            }
          }
        }
      }

      // All other cases, call the base class implementation
      return WaterToWaterComponent_Impl::addToNode(node);
    }

    bool CentralHeatPumpSystem_Impl::addToTertiaryNode(Node& node) {
      auto _model = node.model();
      auto t_plantLoop = node.plantLoop();

      // Only accept adding to a node that is on a supply side of a plant loop
      // Since tertiary here = heating loop
      if (t_plantLoop) {
        if (t_plantLoop->supplyComponent(node.handle())) {
          // Call base class method which accepts both supply and demand
          return WaterToWaterComponent_Impl::addToTertiaryNode(node);
        } else {
          LOG(Info, "Cannot connect the tertiary (=heating) loop to the demand side for " << briefDescription());
        }
      }
      return false;
    }

    boost::optional<ModelObjectList> CentralHeatPumpSystem_Impl::chillerHeaterModuleList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_CentralHeatPumpSystemFields::ChillerHeaterModuleListName);
    }

    bool CentralHeatPumpSystem_Impl::addModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule) {
      bool result = false;
      auto modelObjectList = chillerHeaterModuleList();
      if (modelObjectList) {
        result = modelObjectList->addModelObject(centralHeatPumpSystemModule);
      }
      return result;
    }

    void CentralHeatPumpSystem_Impl::removeModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule) {
      auto modelObjectList = chillerHeaterModuleList();
      if (modelObjectList) {
        modelObjectList->removeModelObject(centralHeatPumpSystemModule);
      }
    }

    void CentralHeatPumpSystem_Impl::removeAllModules() {
      auto modelObjectList = chillerHeaterModuleList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CentralHeatPumpSystemModule>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    std::vector<CentralHeatPumpSystemModule> CentralHeatPumpSystem_Impl::modules() const {
      std::vector<CentralHeatPumpSystemModule> result;
      auto const modelObjectList = chillerHeaterModuleList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CentralHeatPumpSystemModule>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }
      return result;
    }

    /** Convenience Function to return the Cooling Loop **/
    boost::optional<PlantLoop> CentralHeatPumpSystem_Impl::coolingPlantLoop() const {
      return WaterToWaterComponent_Impl::plantLoop();
    }

    /** Convenience Function to return the Source Loop **/
    boost::optional<PlantLoop> CentralHeatPumpSystem_Impl::sourcePlantLoop() const {
      return WaterToWaterComponent_Impl::secondaryPlantLoop();
    }

    /** Convenience Function to return the Heating Loop **/
    boost::optional<PlantLoop> CentralHeatPumpSystem_Impl::heatingPlantLoop() const {
      return WaterToWaterComponent_Impl::tertiaryPlantLoop();
    }

    bool CentralHeatPumpSystem_Impl::setChillerHeaterModuleList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_CentralHeatPumpSystemFields::ChillerHeaterModuleListName, modelObjectList.get().handle());
      } else {
        resetChillerHeaterModuleList();
        result = true;
      }
      return result;
    }

    void CentralHeatPumpSystem_Impl::resetChillerHeaterModuleList() {
      bool result = setString(OS_CentralHeatPumpSystemFields::ChillerHeaterModuleListName, "");
      OS_ASSERT(result);
    }

    std::string CentralHeatPumpSystem_Impl::controlMethod() const {
      boost::optional<std::string> value = getString(OS_CentralHeatPumpSystemFields::ControlMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CentralHeatPumpSystem_Impl::ancillaryPower() const {
      boost::optional<double> value = getDouble(OS_CentralHeatPumpSystemFields::AncillaryPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> CentralHeatPumpSystem_Impl::ancillaryOperationSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName);
    }

    bool CentralHeatPumpSystem_Impl::setControlMethod(const std::string& controlMethod) {
      bool result = setString(OS_CentralHeatPumpSystemFields::ControlMethod, controlMethod);
      return result;
    }

    bool CentralHeatPumpSystem_Impl::setAncillaryPower(double ancillaryPower) {
      bool result = setDouble(OS_CentralHeatPumpSystemFields::AncillaryPower, ancillaryPower);
      return result;
    }

    bool CentralHeatPumpSystem_Impl::setAncillaryOperationSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName, "CentralHeatPumpSystem", "Ancillary Operation", schedule);
      return result;
    }

    void CentralHeatPumpSystem_Impl::resetAncillaryOperationSchedule() {
      bool result = setString(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName, "");
      OS_ASSERT(result);
    }

    ComponentType CentralHeatPumpSystem_Impl::componentType() const {
      // Note (JM): This really depends on which loop is calling this...

      // This component has a tertiary loop, and is placed on the supply side of TWO loops: Heating and Cooling
      // If it's the supplyLoop (= cooling), we should probably have a PlantEquipmentOperation::CoolingLoad
      // If it's the tertiaryLoop (= heating), PlantEquipmentOperation::HeatingLoad
      // Returning BOTH will place it on a PlantEquipmentOperation::Uncontrolled

      // As a result, this is handled in coolingComponents() and heatingComponents() directly
      return ComponentType::None;
    }

    std::vector<FuelType> CentralHeatPumpSystem_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      result.insert(FuelType::Electricity);
      if (auto p_ = sourcePlantLoop()) {
        for (auto ft : p_->coolingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> CentralHeatPumpSystem_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      result.insert(FuelType::Electricity);
      if (auto p_ = sourcePlantLoop()) {
        for (auto ft : p_->heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> CentralHeatPumpSystem_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      result.insert(AppGFuelType::HeatPump);
      if (auto p_ = sourcePlantLoop()) {
        for (auto ft : p_->appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  CentralHeatPumpSystem::CentralHeatPumpSystem(const Model& model) : WaterToWaterComponent(CentralHeatPumpSystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CentralHeatPumpSystem_Impl>());

    bool ok = true;
    ok = setControlMethod("SmartMixing");
    OS_ASSERT(ok);
    ok = setAncillaryPower(0.0);
    OS_ASSERT(ok);

    auto chillerHeaterModuleList = ModelObjectList(model);
    chillerHeaterModuleList.setName(this->name().get() + " Chiller Heater Module List");
    ok = getImpl<detail::CentralHeatPumpSystem_Impl>()->setChillerHeaterModuleList(chillerHeaterModuleList);
    OS_ASSERT(ok);

    // Perhaps we should by default add one CentralHeatPumpSystemModule
    // But this is at least caught by the ForwardTranslator right now...
  }

  IddObjectType CentralHeatPumpSystem::iddObjectType() {
    return {IddObjectType::OS_CentralHeatPumpSystem};
  }

  std::vector<std::string> CentralHeatPumpSystem::controlMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_CentralHeatPumpSystemFields::ControlMethod);
  }

  std::string CentralHeatPumpSystem::controlMethod() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->controlMethod();
  }

  double CentralHeatPumpSystem::ancillaryPower() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->ancillaryPower();
  }

  boost::optional<Schedule> CentralHeatPumpSystem::ancillaryOperationSchedule() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->ancillaryOperationSchedule();
  }

  bool CentralHeatPumpSystem::setControlMethod(const std::string& controlMethod) {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->setControlMethod(controlMethod);
  }

  bool CentralHeatPumpSystem::setAncillaryPower(double ancillaryPower) {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->setAncillaryPower(ancillaryPower);
  }

  bool CentralHeatPumpSystem::setAncillaryOperationSchedule(Schedule& schedule) {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->setAncillaryOperationSchedule(schedule);
  }

  void CentralHeatPumpSystem::resetAncillaryOperationSchedule() {
    getImpl<detail::CentralHeatPumpSystem_Impl>()->resetAncillaryOperationSchedule();
  }

  bool CentralHeatPumpSystem::addModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule) {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->addModule(centralHeatPumpSystemModule);
  }

  void CentralHeatPumpSystem::removeModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule) {
    getImpl<detail::CentralHeatPumpSystem_Impl>()->removeModule(centralHeatPumpSystemModule);
  }

  void CentralHeatPumpSystem::removeAllModules() {
    getImpl<detail::CentralHeatPumpSystem_Impl>()->removeAllModules();
  }

  std::vector<CentralHeatPumpSystemModule> CentralHeatPumpSystem::modules() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->modules();
  }

  // Convenience functions
  boost::optional<PlantLoop> CentralHeatPumpSystem::coolingPlantLoop() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->coolingPlantLoop();
  }

  boost::optional<PlantLoop> CentralHeatPumpSystem::sourcePlantLoop() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->sourcePlantLoop();
  }

  boost::optional<PlantLoop> CentralHeatPumpSystem::heatingPlantLoop() const {
    return getImpl<detail::CentralHeatPumpSystem_Impl>()->heatingPlantLoop();
  }

  /// @cond
  CentralHeatPumpSystem::CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl) : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
