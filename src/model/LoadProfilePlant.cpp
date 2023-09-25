/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "LoadProfilePlant.hpp"
#include "LoadProfilePlant_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleDay.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/time/Time.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_LoadProfile_Plant_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    LoadProfilePlant_Impl::LoadProfilePlant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == LoadProfilePlant::iddObjectType());
    }

    LoadProfilePlant_Impl::LoadProfilePlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == LoadProfilePlant::iddObjectType());
    }

    LoadProfilePlant_Impl::LoadProfilePlant_Impl(const LoadProfilePlant_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& LoadProfilePlant_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Plant Load Profile Mass Flow Rate", "Plant Load Profile Heat Transfer Rate",
                                                   "Plant Load Profile Heat Transfer Energy", "Plant Load Profile Heating Energy",
                                                   "Plant Load Profile Cooling Energy"};
      return result;
    }

    IddObjectType LoadProfilePlant_Impl::iddObjectType() const {
      return LoadProfilePlant::iddObjectType();
    }

    std::vector<ScheduleTypeKey> LoadProfilePlant_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_LoadProfile_PlantFields::LoadScheduleName) != e) {
        result.push_back(ScheduleTypeKey("LoadProfilePlant", "Load"));
      }
      if (std::find(b, e, OS_LoadProfile_PlantFields::FlowRateFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("LoadProfilePlant", "Flow Rate Fraction"));
      }
      return result;
    }

    unsigned LoadProfilePlant_Impl::inletPort() const {
      return OS_LoadProfile_PlantFields::InletNodeName;
    }

    unsigned LoadProfilePlant_Impl::outletPort() const {
      return OS_LoadProfile_PlantFields::OutletNodeName;
    }

    Schedule LoadProfilePlant_Impl::loadSchedule() const {
      boost::optional<Schedule> value = optionalLoadSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Load Schedule attached.");
      }
      return value.get();
    }

    double LoadProfilePlant_Impl::peakFlowRate() const {
      boost::optional<double> value = getDouble(OS_LoadProfile_PlantFields::PeakFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    Schedule LoadProfilePlant_Impl::flowRateFractionSchedule() const {
      boost::optional<Schedule> value = optionalFlowRateFractionSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Flow Rate Fraction Schedule attached.");
      }
      return value.get();
    }

    bool LoadProfilePlant_Impl::setLoadSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_LoadProfile_PlantFields::LoadScheduleName, "LoadProfilePlant", "Load", schedule);
      return result;
    }

    bool LoadProfilePlant_Impl::setPeakFlowRate(double peakFlowRate) {
      bool result = setDouble(OS_LoadProfile_PlantFields::PeakFlowRate, peakFlowRate);
      OS_ASSERT(result);
      return result;
    }

    bool LoadProfilePlant_Impl::setFlowRateFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_LoadProfile_PlantFields::FlowRateFractionScheduleName, "LoadProfilePlant", "Flow Rate Fraction", schedule);
      return result;
    }

    boost::optional<Schedule> LoadProfilePlant_Impl::optionalLoadSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LoadProfile_PlantFields::LoadScheduleName);
    }

    boost::optional<Schedule> LoadProfilePlant_Impl::optionalFlowRateFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LoadProfile_PlantFields::FlowRateFractionScheduleName);
    }

    bool LoadProfilePlant_Impl::addToNode(Node& node) {
      if (auto plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }
      return false;
    }

    std::vector<EMSActuatorNames> LoadProfilePlant_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Plant Load Profile", "Mass Flow Rate"}, {"Plant Load Profile", "Power"}};
      return actuators;
    }

    std::vector<std::string> LoadProfilePlant_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    ComponentType LoadProfilePlant_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> LoadProfilePlant_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> LoadProfilePlant_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> LoadProfilePlant_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  LoadProfilePlant::LoadProfilePlant(const Model& model) : StraightComponent(LoadProfilePlant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::LoadProfilePlant_Impl>());

    auto alwaysOn = model.alwaysOnDiscreteSchedule();

    auto scheduleRuleset = ScheduleRuleset(model);
    auto scheduleDay = scheduleRuleset.defaultDaySchedule();
    scheduleDay.addValue(Time(0, 4), 8000);
    scheduleDay.addValue(Time(0, 8), 6000);
    scheduleDay.addValue(Time(0, 9), 0);
    scheduleDay.addValue(Time(0, 12), 6000);
    scheduleDay.addValue(Time(0, 24), 10000);

    bool ok = true;
    ok = setLoadSchedule(scheduleRuleset);
    OS_ASSERT(ok);
    setPeakFlowRate(0.002);
    ok = setFlowRateFractionSchedule(alwaysOn);
    OS_ASSERT(ok);
  }

  LoadProfilePlant::LoadProfilePlant(const Model& model, Schedule& loadSchedule, Schedule& flowRateFractionSchedule)
    : StraightComponent(LoadProfilePlant::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::LoadProfilePlant_Impl>());

    setLoadSchedule(loadSchedule);
    setPeakFlowRate(0.0);
    setFlowRateFractionSchedule(flowRateFractionSchedule);
  }

  IddObjectType LoadProfilePlant::iddObjectType() {
    return {IddObjectType::OS_LoadProfile_Plant};
  }

  Schedule LoadProfilePlant::loadSchedule() const {
    return getImpl<detail::LoadProfilePlant_Impl>()->loadSchedule();
  }

  double LoadProfilePlant::peakFlowRate() const {
    return getImpl<detail::LoadProfilePlant_Impl>()->peakFlowRate();
  }

  Schedule LoadProfilePlant::flowRateFractionSchedule() const {
    return getImpl<detail::LoadProfilePlant_Impl>()->flowRateFractionSchedule();
  }

  bool LoadProfilePlant::setLoadSchedule(Schedule& schedule) {
    return getImpl<detail::LoadProfilePlant_Impl>()->setLoadSchedule(schedule);
  }

  bool LoadProfilePlant::setPeakFlowRate(double peakFlowRate) {
    return getImpl<detail::LoadProfilePlant_Impl>()->setPeakFlowRate(peakFlowRate);
  }

  bool LoadProfilePlant::setFlowRateFractionSchedule(Schedule& schedule) {
    return getImpl<detail::LoadProfilePlant_Impl>()->setFlowRateFractionSchedule(schedule);
  }

  /// @cond
  LoadProfilePlant::LoadProfilePlant(std::shared_ptr<detail::LoadProfilePlant_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
