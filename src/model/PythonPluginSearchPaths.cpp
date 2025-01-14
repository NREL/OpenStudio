/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PythonPluginSearchPaths.hpp"
#include "PythonPluginSearchPaths_Impl.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Path.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_SearchPaths_FieldEnums.hxx>

#include <iterator>

namespace openstudio {
namespace model {

  namespace detail {

    PythonPluginSearchPaths_Impl::PythonPluginSearchPaths_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PythonPluginSearchPaths::iddObjectType());
    }

    PythonPluginSearchPaths_Impl::PythonPluginSearchPaths_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PythonPluginSearchPaths::iddObjectType());
    }

    PythonPluginSearchPaths_Impl::PythonPluginSearchPaths_Impl(const PythonPluginSearchPaths_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PythonPluginSearchPaths_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      return result;
    }

    IddObjectType PythonPluginSearchPaths_Impl::iddObjectType() const {
      return PythonPluginSearchPaths::iddObjectType();
    }

    bool PythonPluginSearchPaths_Impl::addCurrentWorkingDirectorytoSearchPath() const {
      return getBooleanFieldValue(OS_PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath);
    }

    bool PythonPluginSearchPaths_Impl::addInputFileDirectorytoSearchPath() const {
      return getBooleanFieldValue(OS_PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath);
    }

    bool PythonPluginSearchPaths_Impl::addepinEnvironmentVariabletoSearchPath() const {
      return getBooleanFieldValue(OS_PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath);
    }

    std::vector<openstudio::path> PythonPluginSearchPaths_Impl::searchPaths() const {
      std::vector<openstudio::path> result;
      for (const auto& eg : extensibleGroups()) {
        auto _s = eg.getString(OS_PythonPlugin_SearchPathsExtensibleFields::SearchPath);
        OS_ASSERT(_s);
        result.push_back(openstudio::toPath(_s.get()));
      }
      return result;
    }

    bool PythonPluginSearchPaths_Impl::setAddCurrentWorkingDirectorytoSearchPath(bool addCurrentWorkingDirectorytoSearchPath) {
      const bool result =
        setBooleanFieldValue(OS_PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, addCurrentWorkingDirectorytoSearchPath);
      OS_ASSERT(result);
      return result;
    }

    bool PythonPluginSearchPaths_Impl::setAddInputFileDirectorytoSearchPath(bool addInputFileDirectorytoSearchPath) {
      const bool result =
        setBooleanFieldValue(OS_PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, addInputFileDirectorytoSearchPath);
      OS_ASSERT(result);
      return result;
    }

    bool PythonPluginSearchPaths_Impl::setAddepinEnvironmentVariabletoSearchPath(bool addepinEnvironmentVariabletoSearchPath) {
      const bool result =
        setBooleanFieldValue(OS_PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, addepinEnvironmentVariabletoSearchPath);
      OS_ASSERT(result);
      return result;
    }

    bool PythonPluginSearchPaths_Impl::addSearchPath(const openstudio::path& searchPath) {
      std::vector<openstudio::path> existingSearchPaths = this->searchPaths();
      if (std::find(existingSearchPaths.begin(), existingSearchPaths.end(), searchPath) != existingSearchPaths.end()) {
        LOG(Info, "Not adding search path '" << searchPath << "' to PythonPlugin:SearchPaths since it is already present");
        // Return true anyways, it's a success
        return true;
      }

      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool result = eg.setString(OS_PythonPlugin_SearchPathsExtensibleFields::SearchPath, searchPath.generic_string());
      if (!result) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }

      return result;
    }

    bool PythonPluginSearchPaths_Impl::setSearchPaths(const std::vector<openstudio::path>& searchPaths) {
      bool result = true;

      clearSearchPaths();

      for (const auto& s : searchPaths) {
        bool thisResult = addSearchPath(s);
        if (!thisResult) {
          LOG(Warn, "Couldn't add search path " << s << " to PythonPlugin:SearchPaths, skipping and continuing.");
          result = false;
        }
      }

      return result;
    }

    void PythonPluginSearchPaths_Impl::clearSearchPaths() {
      clearExtensibleGroups();
    }

  }  // namespace detail

  IddObjectType PythonPluginSearchPaths::iddObjectType() {
    return {IddObjectType::OS_PythonPlugin_SearchPaths};
  }

  bool PythonPluginSearchPaths::addCurrentWorkingDirectorytoSearchPath() const {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->addCurrentWorkingDirectorytoSearchPath();
  }

  bool PythonPluginSearchPaths::addInputFileDirectorytoSearchPath() const {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->addInputFileDirectorytoSearchPath();
  }

  bool PythonPluginSearchPaths::addepinEnvironmentVariabletoSearchPath() const {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->addepinEnvironmentVariabletoSearchPath();
  }

  std::vector<openstudio::path> PythonPluginSearchPaths::searchPaths() const {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->searchPaths();
  }

  bool PythonPluginSearchPaths::setAddCurrentWorkingDirectorytoSearchPath(bool addCurrentWorkingDirectorytoSearchPath) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->setAddCurrentWorkingDirectorytoSearchPath(addCurrentWorkingDirectorytoSearchPath);
  }

  bool PythonPluginSearchPaths::setAddInputFileDirectorytoSearchPath(bool addInputFileDirectorytoSearchPath) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->setAddInputFileDirectorytoSearchPath(addInputFileDirectorytoSearchPath);
  }

  bool PythonPluginSearchPaths::setAddepinEnvironmentVariabletoSearchPath(bool addepinEnvironmentVariabletoSearchPath) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->setAddepinEnvironmentVariabletoSearchPath(addepinEnvironmentVariabletoSearchPath);
  }

  bool PythonPluginSearchPaths::addSearchPath(const openstudio::path& searchPath) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->addSearchPath(searchPath);
  }

  bool PythonPluginSearchPaths::setSearchPaths(const std::vector<openstudio::path>& searchPaths) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->setSearchPaths(searchPaths);
  }

  bool PythonPluginSearchPaths::setSearchPaths(const std::vector<std::string>& searchPaths) {
    std::vector<openstudio::path> paths;
    paths.reserve(searchPaths.size());
    std::transform(searchPaths.cbegin(), searchPaths.cend(), std::back_inserter(paths), [](const std::string& s) { return openstudio::toPath(s); });
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->setSearchPaths(paths);
  }

  void PythonPluginSearchPaths::clearSearchPaths() {
    getImpl<detail::PythonPluginSearchPaths_Impl>()->clearSearchPaths();
  }

  /// @cond
  PythonPluginSearchPaths::PythonPluginSearchPaths(std::shared_ptr<detail::PythonPluginSearchPaths_Impl> impl) : ModelObject(std::move(impl)) {}
  PythonPluginSearchPaths::PythonPluginSearchPaths(Model& model) : ModelObject(PythonPluginSearchPaths::iddObjectType(), model) {
    setAddCurrentWorkingDirectorytoSearchPath(true);
    setAddInputFileDirectorytoSearchPath(true);
    setAddepinEnvironmentVariabletoSearchPath(true);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
