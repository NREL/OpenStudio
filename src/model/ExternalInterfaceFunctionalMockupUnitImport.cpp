/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitImport_Impl::ExternalInterfaceFunctionalMockupUnitImport_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                       bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImport::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImport_Impl::ExternalInterfaceFunctionalMockupUnitImport_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImport::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImport_Impl::ExternalInterfaceFunctionalMockupUnitImport_Impl(
      const ExternalInterfaceFunctionalMockupUnitImport_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImport_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitImport_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitImport::iddObjectType();
    }

    double ExternalInterfaceFunctionalMockupUnitImport_Impl::fMUTimeout() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitImport_Impl::isFMUTimeoutDefaulted() const {
      return isEmpty(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout);
    }

    int ExternalInterfaceFunctionalMockupUnitImport_Impl::fMULoggingOn() const {
      boost::optional<int> value = getInt(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitImport_Impl::isFMULoggingOnDefaulted() const {
      return isEmpty(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn);
    }

    bool ExternalInterfaceFunctionalMockupUnitImport_Impl::setFMUTimeout(double fMUTimeout) {
      bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout, fMUTimeout);
      OS_ASSERT(result);
      return result;
    }

    void ExternalInterfaceFunctionalMockupUnitImport_Impl::resetFMUTimeout() {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout, "");
      OS_ASSERT(result);
    }

    bool ExternalInterfaceFunctionalMockupUnitImport_Impl::setFMULoggingOn(int fMULoggingOn) {
      bool result = setInt(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn, fMULoggingOn);
      OS_ASSERT(result);
      return result;
    }

    void ExternalInterfaceFunctionalMockupUnitImport_Impl::resetFMULoggingOn() {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn, "");
      OS_ASSERT(result);
    }

    bool ExternalInterfaceFunctionalMockupUnitImport_Impl::setFMUFileName(const std::string& fMUName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMUFileName, fMUName);
      return result;
    }

    std::string ExternalInterfaceFunctionalMockupUnitImport_Impl::fMUFileName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImportFields::FMUFileName);
      if (value) {
        return value.get();
      }
      return "";
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitImport::ExternalInterfaceFunctionalMockupUnitImport(const Model& model, const std::string& fmuName)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitImport::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>());
    //bool ok1 = getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->setName(fmuName);
    bool ok = setFMUFileName(fmuName);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to " << fmuName << ".");
    }
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImport::iddObjectType() {
    return IddObjectType(IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport);
  }

  double ExternalInterfaceFunctionalMockupUnitImport::fMUTimeout() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->fMUTimeout();
  }

  bool ExternalInterfaceFunctionalMockupUnitImport::isFMUTimeoutDefaulted() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->isFMUTimeoutDefaulted();
  }

  int ExternalInterfaceFunctionalMockupUnitImport::fMULoggingOn() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->fMULoggingOn();
  }

  bool ExternalInterfaceFunctionalMockupUnitImport::isFMULoggingOnDefaulted() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->isFMULoggingOnDefaulted();
  }

  bool ExternalInterfaceFunctionalMockupUnitImport::setFMUTimeout(double fMUTimeout) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->setFMUTimeout(fMUTimeout);
  }

  void ExternalInterfaceFunctionalMockupUnitImport::resetFMUTimeout() {
    getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->resetFMUTimeout();
  }

  bool ExternalInterfaceFunctionalMockupUnitImport::setFMULoggingOn(int fMULoggingOn) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->setFMULoggingOn(fMULoggingOn);
  }

  void ExternalInterfaceFunctionalMockupUnitImport::resetFMULoggingOn() {
    getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->resetFMULoggingOn();
  }

  bool ExternalInterfaceFunctionalMockupUnitImport::setFMUFileName(const std::string& fMUName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->setFMUFileName(fMUName);
  }

  std::string ExternalInterfaceFunctionalMockupUnitImport::fMUFileName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl>()->fMUFileName();
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitImport::ExternalInterfaceFunctionalMockupUnitImport(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImport_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
