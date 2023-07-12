/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Duct.hpp"
#include "Duct_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Duct_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    Duct_Impl::Duct_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Duct::iddObjectType());
    }

    Duct_Impl::Duct_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Duct::iddObjectType());
    }

    Duct_Impl::Duct_Impl(const Duct_Impl& other, Model_Impl* model, bool keepHandle) : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& Duct_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output variables
      return result;
    }

    IddObjectType Duct_Impl::iddObjectType() const {
      return Duct::iddObjectType();
    }

    unsigned Duct_Impl::inletPort() const {
      return OS_DuctFields::InletNode;
    }

    unsigned Duct_Impl::outletPort() const {
      return OS_DuctFields::OutletNode;
    }

    bool Duct_Impl::addToNode(Node& node) {
      if (node.airLoopHVAC()) {
        return StraightComponent_Impl::addToNode(node);
      }

      if (node.airLoopHVACOutdoorAirSystem()) {
        return StraightComponent_Impl::addToNode(node);
      }

      return false;
    }

    ComponentType Duct_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> Duct_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> Duct_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> Duct_Impl::appGHeatingFuelTypes() const {
      return {};
    }
  }  // namespace detail

  Duct::Duct(const Model& model) : StraightComponent(Duct::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Duct_Impl>());

    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
  }

  IddObjectType Duct::iddObjectType() {
    return {IddObjectType::OS_Duct};
  }

  /// @cond
  Duct::Duct(std::shared_ptr<detail::Duct_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
