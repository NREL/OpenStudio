/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GenericModelObject.hpp"
#include "GenericModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    // constructor
    GenericModelObject_Impl::GenericModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    GenericModelObject_Impl::GenericModelObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    GenericModelObject_Impl::GenericModelObject_Impl(const GenericModelObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& GenericModelObject_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType GenericModelObject_Impl::iddObjectType() const {
      IddObjectType retVal(iddObject().type());
      return retVal;
    }

  }  // namespace detail

  GenericModelObject::GenericModelObject(std::shared_ptr<detail::GenericModelObject_Impl> impl) : ModelObject(std::move(impl)) {}

}  // namespace model
}  // namespace openstudio
