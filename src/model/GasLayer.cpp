/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GasLayer.hpp"
#include "GasLayer_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    GasLayer_Impl::GasLayer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(idfObject, model, keepHandle) {}

    GasLayer_Impl::GasLayer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(other, model, keepHandle) {}

    GasLayer_Impl::GasLayer_Impl(const GasLayer_Impl& other, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(other, model, keepHandle) {}

    OptionalDouble GasLayer_Impl::getVisibleTransmittance() const {
      OptionalDouble od(1.0);
      return od;
    }

    boost::optional<double> GasLayer_Impl::interiorVisibleAbsorptance() const {
      return 0.0;
    }

    boost::optional<double> GasLayer_Impl::exteriorVisibleAbsorptance() const {
      return 0.0;
    }

  }  // namespace detail

  /// @cond
  GasLayer::GasLayer(IddObjectType type, const Model& model) : FenestrationMaterial(type, model) {
    OS_ASSERT(getImpl<detail::GasLayer_Impl>());
  }

  GasLayer::GasLayer(std::shared_ptr<detail::GasLayer_Impl> impl) : FenestrationMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
