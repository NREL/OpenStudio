/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  : ShadingMaterial(impl)
{}
/// @endcond

} // model
} // openstudio

