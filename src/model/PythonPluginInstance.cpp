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

#include "PythonPluginInstance.hpp"
#include "PythonPluginInstance_Impl.hpp"

#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_Instance_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

#include <boost/regex.hpp>

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace openstudio {
namespace model {

  namespace detail {

    PythonPluginInstance_Impl::PythonPluginInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PythonPluginInstance::iddObjectType());
    }

    PythonPluginInstance_Impl::PythonPluginInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PythonPluginInstance::iddObjectType());
    }

    PythonPluginInstance_Impl::PythonPluginInstance_Impl(const PythonPluginInstance_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PythonPluginInstance_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType PythonPluginInstance_Impl::iddObjectType() const {
      return PythonPluginInstance::iddObjectType();
    }

    std::vector<ResourceObject> PythonPluginInstance_Impl::resources() const {
      return {externalFile()};
    }

    ExternalFile PythonPluginInstance_Impl::externalFile() const {
      auto value = getObject<ModelObject>().getModelObjectTarget<ExternalFile>(OS_PythonPlugin_InstanceFields::ExternalFileName);
      OS_ASSERT(value);
      return value.get();
    }

    bool PythonPluginInstance_Impl::runDuringWarmupDays() const {
      boost::optional<std::string> value = getString(OS_PythonPlugin_InstanceFields::RunDuringWarmupDays, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool PythonPluginInstance_Impl::isRunDuringWarmupDaysDefaulted() const {
      return isEmpty(OS_PythonPlugin_InstanceFields::RunDuringWarmupDays);
    }

    std::string PythonPluginInstance_Impl::pluginClassName() const {
      boost::optional<std::string> value = getString(OS_PythonPlugin_InstanceFields::PluginClassName, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PythonPluginInstance_Impl::setRunDuringWarmupDays(bool runDuringWarmupDays) {
      bool result = false;
      if (runDuringWarmupDays) {
        result = setString(OS_PythonPlugin_InstanceFields::RunDuringWarmupDays, "Yes");
      } else {
        result = setString(OS_PythonPlugin_InstanceFields::RunDuringWarmupDays, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    void PythonPluginInstance_Impl::resetRunDuringWarmupDays() {
      bool result = setString(OS_PythonPlugin_InstanceFields::RunDuringWarmupDays, "");
      OS_ASSERT(result);
    }

    bool PythonPluginInstance_Impl::setPluginClassName(const std::string& pluginClassName) {
      if (!findPluginClassNameInFile(pluginClassName)) {
        return false;
      }

      bool result = setString(OS_PythonPlugin_InstanceFields::PluginClassName, pluginClassName);
      return result;
    }

    bool PythonPluginInstance_Impl::findPluginClassNameInFile(const std::string& pluginClassName) const {
      auto validClassNames = validPluginClassNamesInFile();
      auto it = std::find(validClassNames.cbegin(), validClassNames.cend(), pluginClassName);
      if (it == validClassNames.cend()) {
        std::stringstream ss;
        ss << "[";
        bool firstTime = true;
        for (const auto& className : validClassNames) {
          if (firstTime) {
            ss << "\"" << className << "\"";
            firstTime = false;
          } else {
            ss << ", \"" << className << "\"";
          }
        }
        ss << "]";
        LOG(Warn, pluginClassName << " not found. validClassNames=" << ss.str());
        return false;
      }

      return true;
    }

    std::vector<std::string> PythonPluginInstance_Impl::validPluginClassNamesInFile() const {
      std::vector<std::string> result;

      // Regexes are expensive, so static duration makes sense
      static const boost::regex re(R"(^class\s+(\w+)\s*\(\s*EnergyPlusPlugin)");
      boost::smatch matches;

      auto filePath = openstudio::filesystem::system_complete(externalFile().filePath());
      openstudio::filesystem::ifstream ifs(filePath);
      std::string line;
      while (std::getline(ifs, line)) {
        if (boost::regex_search(line, matches, re)) {
          result.emplace_back(matches[1].first, matches[1].second);
        }
      }
      return result;
    }

  }  // namespace detail

  PythonPluginInstance::PythonPluginInstance(const ExternalFile& externalfile, const std::string& pluginClassName)
    : ResourceObject(PythonPluginInstance::iddObjectType(), externalfile.model()) {
    OS_ASSERT(getImpl<detail::PythonPluginInstance_Impl>());

    auto filePath = externalfile.filePath();
    bool ok = (toString(filePath.extension()) == ".py");
    if (!ok) {
      remove();
      LOG_AND_THROW("External file must have a .py extension, got externalfile='" << filePath << "'.");
    }

    ok = setPointer(OS_PythonPlugin_InstanceFields::ExternalFileName, externalfile.handle());
    OS_ASSERT(ok);

    // This must occur after setPointer externalfile
    ok = setPluginClassName(pluginClassName);
    if (!ok) {
      remove();
      LOG_AND_THROW("Cannot find the Plugin Class Name '" << pluginClassName << "' in external file '" << filePath << "'.");
    }
  }

  IddObjectType PythonPluginInstance::iddObjectType() {
    return {IddObjectType::OS_PythonPlugin_Instance};
  }

  ExternalFile PythonPluginInstance::externalFile() const {
    return getImpl<detail::PythonPluginInstance_Impl>()->externalFile();
  }

  bool PythonPluginInstance::runDuringWarmupDays() const {
    return getImpl<detail::PythonPluginInstance_Impl>()->runDuringWarmupDays();
  }

  bool PythonPluginInstance::isRunDuringWarmupDaysDefaulted() const {
    return getImpl<detail::PythonPluginInstance_Impl>()->isRunDuringWarmupDaysDefaulted();
  }

  std::string PythonPluginInstance::pluginClassName() const {
    return getImpl<detail::PythonPluginInstance_Impl>()->pluginClassName();
  }

  bool PythonPluginInstance::setRunDuringWarmupDays(bool runDuringWarmupDays) {
    return getImpl<detail::PythonPluginInstance_Impl>()->setRunDuringWarmupDays(runDuringWarmupDays);
  }

  void PythonPluginInstance::resetRunDuringWarmupDays() {
    getImpl<detail::PythonPluginInstance_Impl>()->resetRunDuringWarmupDays();
  }

  bool PythonPluginInstance::setPluginClassName(const std::string& pluginClassName) {
    return getImpl<detail::PythonPluginInstance_Impl>()->setPluginClassName(pluginClassName);
  }

  bool PythonPluginInstance::findPluginClassNameInFile(const std::string& pluginClassName) const {
    return getImpl<detail::PythonPluginInstance_Impl>()->findPluginClassNameInFile(pluginClassName);
  }

  std::vector<std::string> PythonPluginInstance::validPluginClassNamesInFile() const {
    return getImpl<detail::PythonPluginInstance_Impl>()->validPluginClassNamesInFile();
  }

  /// @cond
  PythonPluginInstance::PythonPluginInstance(std::shared_ptr<detail::PythonPluginInstance_Impl> impl) : ResourceObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
