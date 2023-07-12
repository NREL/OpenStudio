/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Thermostat.hpp"
#include "Thermostat_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    Thermostat_Impl::Thermostat_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    Thermostat_Impl::Thermostat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    Thermostat_Impl::Thermostat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    Thermostat_Impl::Thermostat_Impl(const Thermostat_Impl& other, Model_Impl* model, bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

    boost::optional<ThermalZone> Thermostat_Impl::thermalZone() const {
      Handle h = handle();
      auto zones = model().getConcreteModelObjects<model::ThermalZone>();
      for (const auto& zone : zones) {
        if (auto thermostat = zone.thermostat()) {
          if (thermostat->handle() == h) {
            return zone;
          }
        }
      }
      return boost::none;
    }

  }  // namespace detail

  Thermostat::Thermostat(std::shared_ptr<detail::Thermostat_Impl> p) : ModelObject(std::move(p)) {}

  Thermostat::Thermostat(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::Thermostat_Impl>());
  }

  boost::optional<ThermalZone> Thermostat::thermalZone() const {
    return getImpl<detail::Thermostat_Impl>()->thermalZone();
  }

}  // namespace model

}  // namespace openstudio
