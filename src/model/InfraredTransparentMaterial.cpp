/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "InfraredTransparentMaterial.hpp"
#include "InfraredTransparentMaterial_Impl.hpp"

#include <utilities/idd/OS_Material_InfraredTransparent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    InfraredTransparentMaterial_Impl::InfraredTransparentMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelPartitionMaterial_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == InfraredTransparentMaterial::iddObjectType());
    }

    InfraredTransparentMaterial_Impl::InfraredTransparentMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelPartitionMaterial_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == InfraredTransparentMaterial::iddObjectType());
    }

    InfraredTransparentMaterial_Impl::InfraredTransparentMaterial_Impl(const InfraredTransparentMaterial_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : ModelPartitionMaterial_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& InfraredTransparentMaterial_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType InfraredTransparentMaterial_Impl::iddObjectType() const {
      return InfraredTransparentMaterial::iddObjectType();
    }

  }  // namespace detail

  InfraredTransparentMaterial::InfraredTransparentMaterial(const Model& model)
    : ModelPartitionMaterial(InfraredTransparentMaterial::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::InfraredTransparentMaterial_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    bool ok = true;
    // ok = setHandle();
    OS_ASSERT(ok);
  }

  IddObjectType InfraredTransparentMaterial::iddObjectType() {
    return {IddObjectType::OS_Material_InfraredTransparent};
  }

  /// @cond
  InfraredTransparentMaterial::InfraredTransparentMaterial(std::shared_ptr<detail::InfraredTransparentMaterial_Impl> impl)
    : ModelPartitionMaterial(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
