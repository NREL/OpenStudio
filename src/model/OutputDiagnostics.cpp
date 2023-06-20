/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OutputDiagnostics.hpp"
#include "OutputDiagnostics_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_Diagnostics_FieldEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputDiagnostics_Impl::OutputDiagnostics_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputDiagnostics::iddObjectType());
    }

    OutputDiagnostics_Impl::OutputDiagnostics_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputDiagnostics::iddObjectType());
    }

    OutputDiagnostics_Impl::OutputDiagnostics_Impl(const OutputDiagnostics_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputDiagnostics_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType OutputDiagnostics_Impl::iddObjectType() const {
      return OutputDiagnostics::iddObjectType();
    }

    std::vector<std::string> OutputDiagnostics_Impl::keys() const {
      std::vector<std::string> result;
      for (const auto& eg : extensibleGroups()) {
        auto _s = eg.getString(OS_Output_DiagnosticsExtensibleFields::Key);
        OS_ASSERT(_s);
        result.push_back(_s.get());
      }
      return result;
    }

    bool OutputDiagnostics_Impl::addKey(const std::string& key) {
      std::vector<std::string> existingKeys = this->keys();
      if (std::find_if(existingKeys.begin(), existingKeys.end(), [&key](const std::string& k) { return openstudio::istringEqual(k, key); })
          != existingKeys.end()) {
        LOG(Info, "Not adding key '" << key << "' to Output:Diagnostics since it is already present");
        // Return true anyways, it's a success
        return true;
      }

      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool result = eg.setString(OS_Output_DiagnosticsExtensibleFields::Key, key);
      if (!result) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }

      return result;
    }

    bool OutputDiagnostics_Impl::setKeys(const std::vector<std::string>& keys) {
      bool result = true;

      clearKeys();

      for (const auto& k : keys) {
        bool thisResult = addKey(k);
        if (!thisResult) {
          LOG(Warn, "Couldn't add key " << k << " to OutputDiagnostics, skipping and continuing.");
          result = false;
        }
      }

      return result;
    }

    bool OutputDiagnostics_Impl::enableDisplayExtraWarnings() {
      return addKey("DisplayExtraWarnings");
    }

    void OutputDiagnostics_Impl::clearKeys() {
      clearExtensibleGroups();
    }

  }  // namespace detail

  IddObjectType OutputDiagnostics::iddObjectType() {
    return {IddObjectType::OS_Output_Diagnostics};
  }

  std::vector<std::string> OutputDiagnostics::keyValues() {
    IddObject obj = IddFactory::instance().getObject(iddObjectType()).get();
    // Return IddKeyNames in extensible portion
    return getIddKeyNames(obj, obj.numFields() + OS_Output_DiagnosticsExtensibleFields::Key);
  }

  std::vector<std::string> OutputDiagnostics::validKeyValues() {
    return keyValues();
  }

  std::vector<std::string> OutputDiagnostics::keys() const {
    return getImpl<detail::OutputDiagnostics_Impl>()->keys();
  }

  bool OutputDiagnostics::addKey(const std::string& key) {
    return getImpl<detail::OutputDiagnostics_Impl>()->addKey(key);
  }

  bool OutputDiagnostics::setKeys(const std::vector<std::string>& keys) {
    return getImpl<detail::OutputDiagnostics_Impl>()->setKeys(keys);
  }

  bool OutputDiagnostics::enableDisplayExtraWarnings() {
    return getImpl<detail::OutputDiagnostics_Impl>()->enableDisplayExtraWarnings();
  }

  void OutputDiagnostics::clearKeys() {
    getImpl<detail::OutputDiagnostics_Impl>()->clearKeys();
  }

  /// @cond
  OutputDiagnostics::OutputDiagnostics(std::shared_ptr<detail::OutputDiagnostics_Impl> impl) : ModelObject(impl) {}
  OutputDiagnostics::OutputDiagnostics(Model& model) : ModelObject(OutputDiagnostics::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
