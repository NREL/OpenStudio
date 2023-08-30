/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ShadingMaterial.hpp"
#include "ShadingMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ShadingMaterial_Impl::ShadingMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(idfObject, model, keepHandle) {}

    ShadingMaterial_Impl::ShadingMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(other, model, keepHandle) {}

    ShadingMaterial_Impl::ShadingMaterial_Impl(const ShadingMaterial_Impl& other, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(other, model, keepHandle) {}

  }  // namespace detail

  ShadingMaterial::ShadingMaterial(IddObjectType type, const Model& model) : FenestrationMaterial(type, model) {
    OS_ASSERT(getImpl<detail::ShadingMaterial_Impl>());
  }

  /// @cond
  ShadingMaterial::ShadingMaterial(std::shared_ptr<detail::ShadingMaterial_Impl> impl) : FenestrationMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
