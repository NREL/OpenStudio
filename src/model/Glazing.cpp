/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Glazing.hpp"
#include "Glazing_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    Glazing_Impl::Glazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(idfObject, model, keepHandle) {}

    Glazing_Impl::Glazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : FenestrationMaterial_Impl(other, model, keepHandle) {}

    Glazing_Impl::Glazing_Impl(const Glazing_Impl& other, Model_Impl* model, bool keepHandle) : FenestrationMaterial_Impl(other, model, keepHandle) {}

  }  // namespace detail

  Glazing::Glazing(IddObjectType type, const Model& model) : FenestrationMaterial(type, model) {
    OS_ASSERT(getImpl<detail::Glazing_Impl>());
  }

  /// @cond
  Glazing::Glazing(std::shared_ptr<detail::Glazing_Impl> impl) : FenestrationMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
