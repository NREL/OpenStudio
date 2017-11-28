/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "DaylightRedirectionDevice.hpp"
#include "DaylightRedirectionDevice_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WindowMaterial_DaylightRedirectionDevice_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  DaylightRedirectionDevice_Impl::DaylightRedirectionDevice_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ShadingMaterial_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == DaylightRedirectionDevice::iddObjectType());
  }

  DaylightRedirectionDevice_Impl::DaylightRedirectionDevice_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == DaylightRedirectionDevice::iddObjectType());
  }

  DaylightRedirectionDevice_Impl::DaylightRedirectionDevice_Impl(const DaylightRedirectionDevice_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ShadingMaterial_Impl(other,model,keepHandle)
  {}

  OptionalDouble DaylightRedirectionDevice_Impl::getVisibleTransmittance() const {
    // DLM: should this really throw?
    LOG_AND_THROW("Visible transmittance not supported for blinds.");
    return boost::none;
  }

  const std::vector<std::string>& DaylightRedirectionDevice_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType DaylightRedirectionDevice_Impl::iddObjectType() const {
    return DaylightRedirectionDevice::iddObjectType();
  }

  std::string DaylightRedirectionDevice_Impl::daylightRedirectionDeviceType() const {
    OptionalString os = getString(OS_WindowMaterial_DaylightRedirectionDeviceFields::DaylightRedirectionDeviceType,true);
    OS_ASSERT(os);
    return *os;
  }

  bool DaylightRedirectionDevice_Impl::isDaylightRedirectionDeviceTypeDefaulted() const {
    return isEmpty(OS_WindowMaterial_DaylightRedirectionDeviceFields::DaylightRedirectionDeviceType);
  }

  bool DaylightRedirectionDevice_Impl::setDaylightRedirectionDeviceType(const std::string& daylightRedirectionDeviceType) {
    bool result = setString(OS_WindowMaterial_DaylightRedirectionDeviceFields::DaylightRedirectionDeviceType, daylightRedirectionDeviceType);
    return result;
  }

  void DaylightRedirectionDevice_Impl::resetDaylightRedirectionDeviceType() {
    bool result = setString(OS_WindowMaterial_DaylightRedirectionDeviceFields::DaylightRedirectionDeviceType, "");
    OS_ASSERT(result);
  }

} // detail

DaylightRedirectionDevice::DaylightRedirectionDevice(const Model& model)
  : ShadingMaterial(DaylightRedirectionDevice::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::DaylightRedirectionDevice_Impl>());
}

IddObjectType DaylightRedirectionDevice::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice);
}

std::vector<std::string> DaylightRedirectionDevice::daylightRedirectionDeviceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WindowMaterial_DaylightRedirectionDeviceFields::DaylightRedirectionDeviceType);
}

std::string DaylightRedirectionDevice::daylightRedirectionDeviceType() const {
  return getImpl<detail::DaylightRedirectionDevice_Impl>()->daylightRedirectionDeviceType();
}

bool DaylightRedirectionDevice::isDaylightRedirectionDeviceTypeDefaulted() const {
  return getImpl<detail::DaylightRedirectionDevice_Impl>()->isDaylightRedirectionDeviceTypeDefaulted();
}

bool DaylightRedirectionDevice::setDaylightRedirectionDeviceType(const std::string& daylightRedirectionDeviceType) {
  return getImpl<detail::DaylightRedirectionDevice_Impl>()->setDaylightRedirectionDeviceType(daylightRedirectionDeviceType);
}

void DaylightRedirectionDevice::resetDaylightRedirectionDeviceType() {
  getImpl<detail::DaylightRedirectionDevice_Impl>()->resetDaylightRedirectionDeviceType();
}

/// @cond
DaylightRedirectionDevice::DaylightRedirectionDevice(std::shared_ptr<detail::DaylightRedirectionDevice_Impl> impl)
  : ShadingMaterial(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

