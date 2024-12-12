/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "PythonPluginSearchPaths.hpp"
#include "PythonPluginSearchPaths_Impl.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_SearchPaths_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    PythonPluginSearchPaths_Impl::PythonPluginSearchPaths_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PythonPluginSearchPaths::iddObjectType());
    }

    PythonPluginSearchPaths_Impl::PythonPluginSearchPaths_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PythonPluginSearchPaths::iddObjectType());
    }

    PythonPluginSearchPaths_Impl::PythonPluginSearchPaths_Impl(const PythonPluginSearchPaths_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PythonPluginSearchPaths_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
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

    std::vector<std::string> PythonPluginSearchPaths_Impl::searchPaths() const {
      std::vector<std::string> result;
      for (const auto& eg : extensibleGroups()) {
        auto _s = eg.getString(OS_PythonPlugin_SearchPathsExtensibleFields::SearchPath);
        OS_ASSERT(_s);
        result.push_back(_s.get());
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

    bool PythonPluginSearchPaths_Impl::addSearchPath(const std::string& searchPath) {
      std::vector<std::string> existingSearchPaths = this->searchPaths();
      if (std::find_if(existingSearchPaths.begin(), existingSearchPaths.end(),
                       [&searchPath](const std::string& s) { return openstudio::istringEqual(s, searchPath); })
          != existingSearchPaths.end()) {
        LOG(Info, "Not adding search path '" << searchPath << "' to PythonPlugin:SearchPaths since it is already present");
        // Return true anyways, it's a success
        return true;
      }

      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool result = eg.setString(OS_PythonPlugin_SearchPathsExtensibleFields::SearchPath, searchPath);
      if (!result) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }

      return result;
    }

    bool PythonPluginSearchPaths_Impl::setSearchPaths(const std::vector<std::string>& searchPaths) {
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

  std::vector<std::string> PythonPluginSearchPaths::searchPaths() const {
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

  bool PythonPluginSearchPaths::addSearchPath(const std::string& searchPath) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->addSearchPath(searchPath);
  }

  bool PythonPluginSearchPaths::setSearchPaths(const std::vector<std::string>& searchPaths) {
    return getImpl<detail::PythonPluginSearchPaths_Impl>()->setSearchPaths(searchPaths);
  }

  void PythonPluginSearchPaths::clearSearchPaths() {
    getImpl<detail::PythonPluginSearchPaths_Impl>()->clearSearchPaths();
  }

  /// @cond
  PythonPluginSearchPaths::PythonPluginSearchPaths(std::shared_ptr<detail::PythonPluginSearchPaths_Impl> impl) : ParentObject(std::move(impl)) {}
  PythonPluginSearchPaths::PythonPluginSearchPaths(Model& model) : ParentObject(PythonPluginSearchPaths::iddObjectType(), model) {
    setAddCurrentWorkingDirectorytoSearchPath(true);
    setAddInputFileDirectorytoSearchPath(true);
    setAddepinEnvironmentVariabletoSearchPath(true);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
