/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PipeAdiabatic.hpp"
#include "PipeAdiabatic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Model.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_Pipe_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    PipeAdiabatic_Impl::PipeAdiabatic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PipeAdiabatic::iddObjectType());
    }

    PipeAdiabatic_Impl::PipeAdiabatic_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PipeAdiabatic::iddObjectType());
    }

    PipeAdiabatic_Impl::PipeAdiabatic_Impl(const PipeAdiabatic_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PipeAdiabatic_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output
      return result;
    }

    IddObjectType PipeAdiabatic_Impl::iddObjectType() const {
      return PipeAdiabatic::iddObjectType();
    }

    unsigned PipeAdiabatic_Impl::inletPort() const {
      return OS_Pipe_AdiabaticFields::InletNodeName;
    }

    unsigned PipeAdiabatic_Impl::outletPort() const {
      return OS_Pipe_AdiabaticFields::OutletNodeName;
    }

    bool PipeAdiabatic_Impl::addToNode(Node& node) {
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        return StraightComponent_Impl::addToNode(node);
      }

      return false;
    }

    ComponentType PipeAdiabatic_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> PipeAdiabatic_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> PipeAdiabatic_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> PipeAdiabatic_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  // create a new PipeAdiabatic object in the model's workspace
  PipeAdiabatic::PipeAdiabatic(const Model& model) : StraightComponent(PipeAdiabatic::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PipeAdiabatic_Impl>());
  }

  unsigned PipeAdiabatic::inletPort() const {
    return getImpl<detail::PipeAdiabatic_Impl>()->inletPort();
  }

  unsigned PipeAdiabatic::outletPort() const {
    return getImpl<detail::PipeAdiabatic_Impl>()->outletPort();
  }

  PipeAdiabatic::PipeAdiabatic(std::shared_ptr<detail::PipeAdiabatic_Impl> p) : StraightComponent(std::move(p)) {}

  IddObjectType PipeAdiabatic::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Pipe_Adiabatic);
    return result;
  }

}  // namespace model
}  // namespace openstudio
