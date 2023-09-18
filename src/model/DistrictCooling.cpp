/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DistrictCooling.hpp"
#include "DistrictCooling_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_DistrictCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    DistrictCooling_Impl::DistrictCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DistrictCooling::iddObjectType());
    }

    DistrictCooling_Impl::DistrictCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DistrictCooling::iddObjectType());
    }

    DistrictCooling_Impl::DistrictCooling_Impl(const DistrictCooling_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DistrictCooling_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "District Cooling Water Rate",
        "District Cooling Water Energy",
        "District Cooling Water Inlet Temperature",
        "District Cooling Water Outlet Temperature",
        "District Cooling Water Mass Flow Rate",
      };
      return result;
    }

    IddObjectType DistrictCooling_Impl::iddObjectType() const {
      return DistrictCooling::iddObjectType();
    }

    boost::optional<double> DistrictCooling_Impl::nominalCapacity() const {
      return getDouble(OS_DistrictCoolingFields::NominalCapacity, true);
    }

    bool DistrictCooling_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_DistrictCoolingFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<Schedule> DistrictCooling_Impl::capacityFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DistrictCoolingFields::CapacityFractionSchedule);
    }

    bool DistrictCooling_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
      bool result(false);
      if (nominalCapacity) {
        result = setDouble(OS_DistrictCoolingFields::NominalCapacity, nominalCapacity.get());
      }
      return result;
    }

    void DistrictCooling_Impl::autosizeNominalCapacity() {
      bool result = setString(OS_DistrictCoolingFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    unsigned DistrictCooling_Impl::inletPort() const {
      return OS_DistrictCoolingFields::ChilledWaterInletNodeName;
    }

    unsigned DistrictCooling_Impl::outletPort() const {
      return OS_DistrictCoolingFields::ChilledWaterOutletNodeName;
    }

    bool DistrictCooling_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    bool DistrictCooling_Impl::setCapacityFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_DistrictCoolingFields::CapacityFractionSchedule, "DistrictCooling", "Capacity Fraction", schedule);
      return result;
    }

    void DistrictCooling_Impl::resetCapacityFractionSchedule() {
      bool result = setString(OS_DistrictCoolingFields::CapacityFractionSchedule, "");
      OS_ASSERT(result);
    }

    boost::optional<double> DistrictCooling_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void DistrictCooling_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void DistrictCooling_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    ComponentType DistrictCooling_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> DistrictCooling_Impl::coolingFuelTypes() const {
      return {FuelType::DistrictCooling};
    }

    std::vector<FuelType> DistrictCooling_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> DistrictCooling_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  DistrictCooling::DistrictCooling(const Model& model) : StraightComponent(DistrictCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DistrictCooling_Impl>());
    autosizeNominalCapacity();
  }

  IddObjectType DistrictCooling::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DistrictCooling);
    return result;
  }

  boost::optional<double> DistrictCooling::nominalCapacity() const {
    return getImpl<detail::DistrictCooling_Impl>()->nominalCapacity();
  }

  bool DistrictCooling::isNominalCapacityAutosized() const {
    return getImpl<detail::DistrictCooling_Impl>()->isNominalCapacityAutosized();
  }

  boost::optional<Schedule> DistrictCooling::capacityFractionSchedule() const {
    return getImpl<detail::DistrictCooling_Impl>()->capacityFractionSchedule();
  }

  bool DistrictCooling::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::DistrictCooling_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void DistrictCooling::autosizeNominalCapacity() {
    getImpl<detail::DistrictCooling_Impl>()->autosizeNominalCapacity();
  }

  bool DistrictCooling::setCapacityFractionSchedule(Schedule& schedule) {
    return getImpl<detail::DistrictCooling_Impl>()->setCapacityFractionSchedule(schedule);
  }

  void DistrictCooling::resetCapacityFractionSchedule() {
    getImpl<detail::DistrictCooling_Impl>()->resetCapacityFractionSchedule();
  }

  /// @cond

  DistrictCooling::DistrictCooling(std::shared_ptr<detail::DistrictCooling_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> DistrictCooling::autosizedNominalCapacity() const {
    return getImpl<detail::DistrictCooling_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model

}  // namespace openstudio
