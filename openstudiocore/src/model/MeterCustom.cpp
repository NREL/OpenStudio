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

#include "MeterCustom.hpp"
#include "MeterCustom_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Meter_Custom_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  MeterCustom_Impl::MeterCustom_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : _Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == MeterCustom::iddObjectType());
  }

  MeterCustom_Impl::MeterCustom_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : _Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == MeterCustom::iddObjectType());
  }

  MeterCustom_Impl::MeterCustom_Impl(const MeterCustom_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : _Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& MeterCustom_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType MeterCustom_Impl::iddObjectType() const {
    return MeterCustom::iddObjectType();
  }

  boost::optional<std::string> MeterCustom_Impl::fuelType() const {
    return getString(OS_Meter_CustomFields::FuelType,true);
  }

  bool MeterCustom_Impl::setFuelType(boost::optional<std::string> fuelType) {
    bool result(false);
    if (fuelType) {
      result = setString(OS_Meter_CustomFields::FuelType, fuelType.get());
    }
    else {
      resetFuelType();
      result = true;
    }
    return result;
  }

  void MeterCustom_Impl::resetFuelType() {
    bool result = setString(OS_Meter_CustomFields::FuelType, "");
    OS_ASSERT(result);
  }

} // detail

MeterCustom::MeterCustom(const Model& model)
  : (MeterCustom::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::MeterCustom_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType MeterCustom::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Meter_Custom);
}

std::vector<std::string> MeterCustom::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Meter_CustomFields::FuelType);
}

boost::optional<std::string> MeterCustom::fuelType() const {
  return getImpl<detail::MeterCustom_Impl>()->fuelType();
}

bool MeterCustom::setFuelType(std::string fuelType) {
  return getImpl<detail::MeterCustom_Impl>()->setFuelType(fuelType);
}

void MeterCustom::resetFuelType() {
  getImpl<detail::MeterCustom_Impl>()->resetFuelType();
}

/// @cond
MeterCustom::MeterCustom(std::shared_ptr<detail::MeterCustom_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // model
} // openstudio

