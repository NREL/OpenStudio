/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ElectricalStorage.hpp"
#include "ElectricalStorage_Impl.hpp"

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    ElectricalStorage_Impl::ElectricalStorage_Impl(IddObjectType type, Model_Impl* model) : ParentObject_Impl(type, model) {}

    ElectricalStorage_Impl::ElectricalStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {}

    ElectricalStorage_Impl::ElectricalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    ElectricalStorage_Impl::ElectricalStorage_Impl(const ElectricalStorage_Impl& other, Model_Impl* model, bool keepHandles)
      : ParentObject_Impl(other, model, keepHandles) {}

    // Convenience method to find the ELCD linked to this storage device
    boost::optional<ElectricLoadCenterDistribution> ElectricalStorage_Impl::electricLoadCenterDistribution() const {
      auto elcds = getObject<ModelObject>().getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
      if (elcds.empty()) {
        // no error
      } else if (elcds.size() == 1u) {
        return elcds[0];
      } else {
        // error
      }

      return boost::none;
    }

  }  // namespace detail

  ElectricalStorage::ElectricalStorage(IddObjectType type, const Model& model) : ParentObject(type, model) {
    OS_ASSERT(getImpl<detail::ElectricalStorage_Impl>());
  }

  ElectricalStorage::ElectricalStorage(std::shared_ptr<detail::ElectricalStorage_Impl> p) : ParentObject(std::move(p)) {}

  boost::optional<ElectricLoadCenterDistribution> ElectricalStorage::electricLoadCenterDistribution() const {
    return getImpl<detail::ElectricalStorage_Impl>()->electricLoadCenterDistribution();
  }

  boost::optional<ThermalZone> ElectricalStorage::thermalZone() const {
    return getImpl<detail::ElectricalStorage_Impl>()->thermalZone();
  }

  bool ElectricalStorage::setThermalZone(const ThermalZone& thermalZone) {
    return getImpl<detail::ElectricalStorage_Impl>()->setThermalZone(thermalZone);
  }

  void ElectricalStorage::resetThermalZone() {
    getImpl<detail::ElectricalStorage_Impl>()->resetThermalZone();
  }

}  // namespace model

}  // namespace openstudio
