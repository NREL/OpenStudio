/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WaterUseEquipment.hpp"
#include "WaterUseEquipment_Impl.hpp"
#include "WaterUseEquipmentDefinition.hpp"
#include "WaterUseEquipmentDefinition_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "WaterUseConnections.hpp"
#include "WaterUseConnections_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include <utilities/idd/OS_WaterUse_Equipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    WaterUseEquipment_Impl::WaterUseEquipment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WaterUseEquipment::iddObjectType());
    }

    WaterUseEquipment_Impl::WaterUseEquipment_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WaterUseEquipment::iddObjectType());
    }

    WaterUseEquipment_Impl::WaterUseEquipment_Impl(const WaterUseEquipment_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& WaterUseEquipment_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Water Use Equipment Hot Water Mass Flow Rate", "Water Use Equipment Cold Water Mass Flow Rate", "Water Use Equipment Total Mass Flow Rate",
        "Water Use Equipment Hot Water Volume Flow Rate", "Water Use Equipment Cold Water Volume Flow Rate",
        "Water Use Equipment Total Volume Flow Rate", "Water Use Equipment Hot Water Volume", "Water Use Equipment Cold Water Volume",
        "Water Use Equipment Total Volume", "Water Use Equipment Mains Water Volume", "Water Use Equipment Hot Water Temperature",
        "Water Use Equipment Cold Water Temperature", "Water Use Equipment Target Water Temperature", "Water Use Equipment Mixed Water Temperature",
        "Water Use Equipment Drain Water Temperature", "Water Use Equipment Heating Rate", "Water Use Equipment Heating Energy",

        // The Key is the name of the Water Use Equipment, not the zone,
        // so it's appropriate to report it here rather than the ThermalZone
        // cf EnergyPlus/WaterUse.cc
        "Water Use Equipment Zone Sensible Heat Gain Rate", "Water Use Equipment Zone Sensible Heat Gain Energy",
        "Water Use Equipment Zone Latent Gain Rate", "Water Use Equipment Zone Latent Gain Energy",
        "Water Use Equipment Zone Moisture Gain Mass Flow Rate", "Water Use Equipment Zone Moisture Gain Mass"

      };
      return result;
    }

    IddObjectType WaterUseEquipment_Impl::iddObjectType() const {
      return WaterUseEquipment::iddObjectType();
    }

    std::vector<ScheduleTypeKey> WaterUseEquipment_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_WaterUse_EquipmentFields::FlowRateFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterUseEquipment", "Flow Rate Fraction"));
      }
      return result;
    }

    boost::optional<Schedule> WaterUseEquipment_Impl::flowRateFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterUse_EquipmentFields::FlowRateFractionScheduleName);
    }

    bool WaterUseEquipment_Impl::setFlowRateFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterUse_EquipmentFields::FlowRateFractionScheduleName, "WaterUseEquipment", "Flow Rate Fraction", schedule);
      return result;
    }

    void WaterUseEquipment_Impl::resetFlowRateFractionSchedule() {
      bool result = setString(OS_WaterUse_EquipmentFields::FlowRateFractionScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> WaterUseEquipment_Impl::flowRateFractionScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = flowRateFractionSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool WaterUseEquipment_Impl::setFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          return setFlowRateFractionSchedule(*intermediate);
        } else {
          return false;
        }
      } else {
        resetFlowRateFractionSchedule();
      }
      return true;
    }

    boost::optional<WaterUseConnections> WaterUseEquipment_Impl::waterUseConnections() const {
      std::vector<WaterUseConnections> connections = model().getConcreteModelObjects<WaterUseConnections>();

      for (const auto& connection : connections) {
        std::vector<WaterUseEquipment> equipment = connection.waterUseEquipment();

        for (const auto& elem : equipment) {
          if (elem.handle() == handle()) {
            return connection;
          }
        }
      }

      return boost::none;
    }

    std::vector<IdfObject> WaterUseEquipment_Impl::remove() {
      if (boost::optional<WaterUseConnections> c = waterUseConnections()) {
        auto e = getObject<WaterUseEquipment>();

        c->removeWaterUseEquipment(e);
      }

      return ModelObject_Impl::remove();
    }

    WaterUseEquipmentDefinition WaterUseEquipment_Impl::waterUseEquipmentDefinition() const {
      return definition().cast<WaterUseEquipmentDefinition>();
    }

    bool WaterUseEquipment_Impl::setWaterUseEquipmentDefinition(const WaterUseEquipmentDefinition& definition) {
      return setPointer(OS_WaterUse_EquipmentFields::WaterUseEquipmentDefinitionName, definition.handle());
    }

    int WaterUseEquipment_Impl::spaceIndex() const {
      return OS_WaterUse_EquipmentFields::SpaceName;
    }

    int WaterUseEquipment_Impl::definitionIndex() const {
      return OS_WaterUse_EquipmentFields::WaterUseEquipmentDefinitionName;
    }

    bool WaterUseEquipment_Impl::setDefinition(const SpaceLoadDefinition& definition) {
      bool result = false;
      boost::optional<WaterUseEquipmentDefinition> waterUseEquipmentDefinition = definition.optionalCast<WaterUseEquipmentDefinition>();
      if (waterUseEquipmentDefinition) {
        result = setWaterUseEquipmentDefinition(*waterUseEquipmentDefinition);
      }
      return result;
    }

    bool WaterUseEquipment_Impl::hardSize() {
      return false;
    }

    bool WaterUseEquipment_Impl::hardApplySchedules() {
      return false;
    }

    double WaterUseEquipment_Impl::multiplier() const {
      return 1;
    }

    bool WaterUseEquipment_Impl::isMultiplierDefaulted() const {
      return true;
    }

    bool WaterUseEquipment_Impl::isAbsolute() const {
      return true;
    }

  }  // namespace detail

  WaterUseEquipment::WaterUseEquipment(const WaterUseEquipmentDefinition& waterUseEquipmentDefinition)
    : SpaceLoadInstance(WaterUseEquipment::iddObjectType(), waterUseEquipmentDefinition) {
    OS_ASSERT(getImpl<detail::WaterUseEquipment_Impl>());

    Schedule sch = this->model().alwaysOnDiscreteSchedule();
    bool test = this->setFlowRateFractionSchedule(sch);
    OS_ASSERT(test);
  }

  IddObjectType WaterUseEquipment::iddObjectType() {
    return {IddObjectType::OS_WaterUse_Equipment};
  }

  boost::optional<Schedule> WaterUseEquipment::flowRateFractionSchedule() const {
    return getImpl<detail::WaterUseEquipment_Impl>()->flowRateFractionSchedule();
  }

  bool WaterUseEquipment::setFlowRateFractionSchedule(Schedule& flowRateFractionSchedule) {
    return getImpl<detail::WaterUseEquipment_Impl>()->setFlowRateFractionSchedule(flowRateFractionSchedule);
  }

  void WaterUseEquipment::resetFlowRateFractionSchedule() {
    getImpl<detail::WaterUseEquipment_Impl>()->resetFlowRateFractionSchedule();
  }

  boost::optional<WaterUseConnections> WaterUseEquipment::waterUseConnections() const {
    return getImpl<detail::WaterUseEquipment_Impl>()->waterUseConnections();
  }

  WaterUseEquipmentDefinition WaterUseEquipment::waterUseEquipmentDefinition() const {
    return getImpl<detail::WaterUseEquipment_Impl>()->waterUseEquipmentDefinition();
  }

  bool WaterUseEquipment::setWaterUseEquipmentDefinition(const WaterUseEquipmentDefinition& definition) {
    return getImpl<detail::WaterUseEquipment_Impl>()->setWaterUseEquipmentDefinition(definition);
  }

  /// @cond
  WaterUseEquipment::WaterUseEquipment(std::shared_ptr<detail::WaterUseEquipment_Impl> impl) : SpaceLoadInstance(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
