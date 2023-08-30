/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DistrictHeating.hpp"
#include "DistrictHeating_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_DistrictHeating_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    DistrictHeating_Impl::DistrictHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DistrictHeating::iddObjectType());
    }

    DistrictHeating_Impl::DistrictHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DistrictHeating::iddObjectType());
    }

    DistrictHeating_Impl::DistrictHeating_Impl(const DistrictHeating_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DistrictHeating_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "District Heating Hot Water Rate",    "District Heating Hot Water Energy",   "District Heating Rate",
        "District Heating Inlet Temperature", "District Heating Outlet Temperature", "District Heating Mass Flow Rate"};
      return result;
    }

    IddObjectType DistrictHeating_Impl::iddObjectType() const {
      return DistrictHeating::iddObjectType();
    }

    std::vector<ScheduleTypeKey> DistrictHeating_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_DistrictHeatingFields::CapacityFractionSchedule) != e) {
        result.push_back(ScheduleTypeKey("DistrictHeating", "Capacity Fraction Schedule"));
      }
      return result;
    }

    boost::optional<double> DistrictHeating_Impl::nominalCapacity() const {
      return getDouble(OS_DistrictHeatingFields::NominalCapacity, true);
    }

    bool DistrictHeating_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_DistrictHeatingFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<Schedule> DistrictHeating_Impl::capacityFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DistrictHeatingFields::CapacityFractionSchedule);
    }

    bool DistrictHeating_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
      bool result(false);
      if (nominalCapacity) {
        result = setDouble(OS_DistrictHeatingFields::NominalCapacity, nominalCapacity.get());
      }
      return result;
    }

    void DistrictHeating_Impl::autosizeNominalCapacity() {
      bool result = setString(OS_DistrictHeatingFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    unsigned DistrictHeating_Impl::inletPort() const {
      return OS_DistrictHeatingFields::HotWaterInletNodeName;
    }

    unsigned DistrictHeating_Impl::outletPort() const {
      return OS_DistrictHeatingFields::HotWaterOutletNodeName;
    }

    bool DistrictHeating_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    bool DistrictHeating_Impl::setCapacityFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_DistrictHeatingFields::CapacityFractionSchedule, "DistrictHeating", "Capacity Fraction", schedule);
      return result;
    }

    void DistrictHeating_Impl::resetCapacityFractionSchedule() {
      bool result = setString(OS_DistrictHeatingFields::CapacityFractionSchedule, "");
      OS_ASSERT(result);
    }

    boost::optional<double> DistrictHeating_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void DistrictHeating_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void DistrictHeating_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    ComponentType DistrictHeating_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> DistrictHeating_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> DistrictHeating_Impl::heatingFuelTypes() const {
      return {FuelType::DistrictHeating};
    }

    std::vector<AppGFuelType> DistrictHeating_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::District};
    }

  }  // namespace detail

  DistrictHeating::DistrictHeating(const Model& model) : StraightComponent(DistrictHeating::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DistrictHeating_Impl>());
    autosizeNominalCapacity();
  }

  IddObjectType DistrictHeating::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DistrictHeating);
    return result;
  }

  boost::optional<double> DistrictHeating::nominalCapacity() const {
    return getImpl<detail::DistrictHeating_Impl>()->nominalCapacity();
  }

  bool DistrictHeating::isNominalCapacityAutosized() const {
    return getImpl<detail::DistrictHeating_Impl>()->isNominalCapacityAutosized();
  }

  boost::optional<Schedule> DistrictHeating::capacityFractionSchedule() const {
    return getImpl<detail::DistrictHeating_Impl>()->capacityFractionSchedule();
  }

  bool DistrictHeating::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::DistrictHeating_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void DistrictHeating::autosizeNominalCapacity() {
    getImpl<detail::DistrictHeating_Impl>()->autosizeNominalCapacity();
  }

  bool DistrictHeating::setCapacityFractionSchedule(Schedule& schedule) {
    return getImpl<detail::DistrictHeating_Impl>()->setCapacityFractionSchedule(schedule);
  }

  void DistrictHeating::resetCapacityFractionSchedule() {
    getImpl<detail::DistrictHeating_Impl>()->resetCapacityFractionSchedule();
  }

  /// @cond

  DistrictHeating::DistrictHeating(std::shared_ptr<detail::DistrictHeating_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> DistrictHeating::autosizedNominalCapacity() const {
    return getImpl<detail::DistrictHeating_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model

}  // namespace openstudio
