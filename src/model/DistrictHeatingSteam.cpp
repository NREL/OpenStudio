/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DistrictHeatingSteam.hpp"
#include "DistrictHeatingSteam_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_DistrictHeating_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    DistrictHeatingSteam_Impl::DistrictHeatingSteam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DistrictHeatingSteam::iddObjectType());
    }

    DistrictHeatingSteam_Impl::DistrictHeatingSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DistrictHeatingSteam::iddObjectType());
    }

    DistrictHeatingSteam_Impl::DistrictHeatingSteam_Impl(const DistrictHeatingSteam_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DistrictHeatingSteam_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "District Heating Hot Water Rate",    "District Heating Hot Water Energy",   "District Heating Rate",
        "District Heating Inlet Temperature", "District Heating Outlet Temperature", "District Heating Mass Flow Rate"};
      return result;
    }

    IddObjectType DistrictHeatingSteam_Impl::iddObjectType() const {
      return DistrictHeatingSteam::iddObjectType();
    }

    std::vector<ScheduleTypeKey> DistrictHeatingSteam_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_DistrictHeating_SteamFields::CapacityFractionSchedule) != fieldIndices.cend()) {
        result.emplace_back("DistrictHeatingSteam", "Capacity Fraction Schedule");
      }
      return result;
    }

    unsigned DistrictHeatingSteam_Impl::inletPort() const {
      return OS_DistrictHeating_SteamFields::SteamInletNodeName;
    }

    unsigned DistrictHeatingSteam_Impl::outletPort() const {
      return OS_DistrictHeating_SteamFields::SteamOutletNodeName;
    }

    bool DistrictHeatingSteam_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    boost::optional<double> DistrictHeatingSteam_Impl::nominalCapacity() const {
      return getDouble(OS_DistrictHeating_SteamFields::NominalCapacity, true);
    }

    bool DistrictHeatingSteam_Impl::isNominalCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_DistrictHeating_SteamFields::NominalCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool DistrictHeatingSteam_Impl::setNominalCapacity(double nominalCapacity) {
      return setDouble(OS_DistrictHeating_SteamFields::NominalCapacity, nominalCapacity);
    }

    void DistrictHeatingSteam_Impl::autosizeNominalCapacity() {
      const bool result = setString(OS_DistrictHeating_SteamFields::NominalCapacity, "autosize");
      OS_ASSERT(result);
    }

    Schedule DistrictHeatingSteam_Impl::capacityFractionSchedule() const {
      auto sch_ = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_DistrictHeating_SteamFields::CapacityFractionSchedule);
      OS_ASSERT(sch_);
      return *sch_;
    }

    bool DistrictHeatingSteam_Impl::setCapacityFractionSchedule(Schedule& schedule) {
      const bool result =
        setSchedule(OS_DistrictHeating_SteamFields::CapacityFractionSchedule, "DistrictHeatingSteam", "Capacity Fraction", schedule);
      return result;
    }

    boost::optional<double> DistrictHeatingSteam_Impl::autosizedNominalCapacity() const {
      return getAutosizedValue("Design Size Nominal Capacity", "W");
    }

    void DistrictHeatingSteam_Impl::autosize() {
      autosizeNominalCapacity();
    }

    void DistrictHeatingSteam_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalCapacity();
      if (val) {
        setNominalCapacity(val.get());
      }
    }

    ComponentType DistrictHeatingSteam_Impl::componentType() const {
      return ComponentType::Heating;
    }

    std::vector<FuelType> DistrictHeatingSteam_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> DistrictHeatingSteam_Impl::heatingFuelTypes() const {
      return {FuelType::Steam};
    }

    std::vector<AppGFuelType> DistrictHeatingSteam_Impl::appGHeatingFuelTypes() const {
      return {AppGFuelType::District};
    }

  }  // namespace detail

  DistrictHeatingSteam::DistrictHeatingSteam(const Model& model) : StraightComponent(DistrictHeatingSteam::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::DistrictHeatingSteam_Impl>());
    autosizeNominalCapacity();

    {
      auto schedule = model.alwaysOnContinuousSchedule();
      const bool ok = setCapacityFractionSchedule(schedule);
      OS_ASSERT(ok);
    }
  }

  IddObjectType DistrictHeatingSteam::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DistrictHeating_Steam);
    return result;
  }

  boost::optional<double> DistrictHeatingSteam::nominalCapacity() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->nominalCapacity();
  }

  bool DistrictHeatingSteam::isNominalCapacityAutosized() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->isNominalCapacityAutosized();
  }

  Schedule DistrictHeatingSteam::capacityFractionSchedule() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->capacityFractionSchedule();
  }

  bool DistrictHeatingSteam::setNominalCapacity(double nominalCapacity) {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->setNominalCapacity(nominalCapacity);
  }

  void DistrictHeatingSteam::autosizeNominalCapacity() {
    getImpl<detail::DistrictHeatingSteam_Impl>()->autosizeNominalCapacity();
  }

  bool DistrictHeatingSteam::setCapacityFractionSchedule(Schedule& schedule) {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->setCapacityFractionSchedule(schedule);
  }

  /// @cond

  DistrictHeatingSteam::DistrictHeatingSteam(std::shared_ptr<detail::DistrictHeatingSteam_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> DistrictHeatingSteam::autosizedNominalCapacity() const {
    return getImpl<detail::DistrictHeatingSteam_Impl>()->autosizedNominalCapacity();
  }

}  // namespace model

}  // namespace openstudio
