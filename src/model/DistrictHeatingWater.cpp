/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DistrictHeatingWater.hpp"
#include "DistrictHeatingWater_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_DistrictHeating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    DistrictHeatingWater_Impl::DistrictHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DistrictHeatingWater::iddObjectType());
    }

    DistrictHeatingWater_Impl::DistrictHeatingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DistrictHeatingWater::iddObjectType());
    }

    DistrictHeatingWater_Impl::DistrictHeatingWater_Impl(const DistrictHeatingWater_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DistrictHeatingWater_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "District Heating Water Energy",
        "District Heating Water Rate",
        "District Heating Water Inlet Temperature",
        "District Heating Water Outlet Temperature",
        "District Heating Water Mass Flow Rate",
      };
      return result;
    }

    IddObjectType DistrictHeatingWater_Impl::iddObjectType() const {
      return DistrictHeatingWater::iddObjectType();
    }

    std::vector<ScheduleTypeKey> DistrictHeatingWater_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_DistrictHeating_WaterFields::CapacityFractionSchedule) != fieldIndices.cend()) {
        result.emplace_back("DistrictHeatingWater", "Capacity Fraction Schedule");
      }
      return result;
    }

    unsigned DistrictHeatingWater_Impl::inletPort() const {
      return OS_DistrictHeating_WaterFields::HotWaterInletNodeName;
    }

    unsigned DistrictHeatingWater_Impl::outletPort() const {
      return OS_DistrictHeating_WaterFields::HotWaterOutletNodeName;
    }

    bool DistrictHeatingWater_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    boost::optional<double> DistrictHeatingWater_Impl::nominalCapacity() const {
      return getDouble(OS_DistrictHeating_WaterFields::NominalCapacity, true);
    }

    bool DistrictHeatingWater_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_DistrictHeating_WaterFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool DistrictHeatingWater_Impl::setNominalCapacity(double nominalCapacity) {
      return setDouble(OS_DistrictHeating_WaterFields::NominalCapacity, nominalCapacity);
    }

    void DistrictHeatingWater_Impl::autosizeNominalCapacity() {
      const bool result = setString(OS_DistrictHeating_WaterFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    Schedule DistrictHeatingWater_Impl::capacityFractionSchedule() const {
      auto sch_ = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DistrictHeating_WaterFields::CapacityFractionSchedule);
      OS_ASSERT(sch_);
      return *sch_;
    }

    bool DistrictHeatingWater_Impl::setCapacityFractionSchedule(Schedule& schedule) {
      const bool result =
        setSchedule(OS_DistrictHeating_WaterFields::CapacityFractionSchedule, "DistrictHeatingWater", "Capacity Fraction", schedule);
      return result;
    }

    boost::optional<double> DistrictHeatingWater_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void DistrictHeatingWater_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void DistrictHeatingWater_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    ComponentType DistrictHeatingWater_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> DistrictHeatingWater_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> DistrictHeatingWater_Impl::heatingFuelTypes() const {
      return {FuelType::DistrictHeating};
    }

    std::vector<AppGFuelType> DistrictHeatingWater_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::District};
    }

  }  // namespace detail

  DistrictHeatingWater::DistrictHeatingWater(const Model& model) : StraightComponent(DistrictHeatingWater::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DistrictHeatingWater_Impl>());
    autosizeNominalCapacity();

    {
      auto schedule = model.alwaysOnContinuousSchedule();
      const bool ok = setCapacityFractionSchedule(schedule);
      OS_ASSERT(ok);
    }
  }

  IddObjectType DistrictHeatingWater::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DistrictHeating_Water);
    return result;
  }

  boost::optional<double> DistrictHeatingWater::nominalCapacity() const {
    return getImpl<detail::DistrictHeatingWater_Impl>()->nominalCapacity();
  }

  bool DistrictHeatingWater::isNominalCapacityAutosized() const {
    return getImpl<detail::DistrictHeatingWater_Impl>()->isNominalCapacityAutosized();
  }

  bool DistrictHeatingWater::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::DistrictHeatingWater_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void DistrictHeatingWater::autosizeNominalCapacity() {
    getImpl<detail::DistrictHeatingWater_Impl>()->autosizeNominalCapacity();
  }

  Schedule DistrictHeatingWater::capacityFractionSchedule() const {
    return getImpl<detail::DistrictHeatingWater_Impl>()->capacityFractionSchedule();
  }

  bool DistrictHeatingWater::setCapacityFractionSchedule(Schedule& schedule) {
    return getImpl<detail::DistrictHeatingWater_Impl>()->setCapacityFractionSchedule(schedule);
  }

  /// @cond

  DistrictHeatingWater::DistrictHeatingWater(std::shared_ptr<detail::DistrictHeatingWater_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> DistrictHeatingWater::autosizedNominalCapacity() const {
    return getImpl<detail::DistrictHeatingWater_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model

}  // namespace openstudio
