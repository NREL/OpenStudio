/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterface.hpp"
#include "ExternalInterface_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterface_Impl::ExternalInterface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterface::iddObjectType());
    }

    ExternalInterface_Impl::ExternalInterface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterface::iddObjectType());
    }

    ExternalInterface_Impl::ExternalInterface_Impl(const ExternalInterface_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterface_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterface_Impl::iddObjectType() const {
      return ExternalInterface::iddObjectType();
    }

    std::string ExternalInterface_Impl::nameofExternalInterface() const {
      boost::optional<std::string> value = getString(OS_ExternalInterfaceFields::NameofExternalInterface, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    bool ExternalInterface_Impl::setNameofExternalInterface(const std::string& nameofExternalInterface) {
      bool result = setString(OS_ExternalInterfaceFields::NameofExternalInterface, nameofExternalInterface);
      return result;
    }

  }  // namespace detail

  IddObjectType ExternalInterface::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface};
  }

  std::vector<std::string> ExternalInterface::nameofExternalInterfaceValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ExternalInterfaceFields::NameofExternalInterface);
  }

  std::string ExternalInterface::nameofExternalInterface() const {
    return getImpl<detail::ExternalInterface_Impl>()->nameofExternalInterface();
  }

  bool ExternalInterface::setNameofExternalInterface(const std::string& nameofExternalInterface) {
    return getImpl<detail::ExternalInterface_Impl>()->setNameofExternalInterface(nameofExternalInterface);
  }

  /// @cond
  ExternalInterface::ExternalInterface(std::shared_ptr<detail::ExternalInterface_Impl> impl) : ModelObject(impl) {}
  ExternalInterface::ExternalInterface(Model& model) : ModelObject(ExternalInterface::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
