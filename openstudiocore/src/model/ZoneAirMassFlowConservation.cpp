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

#include "ZoneAirMassFlowConservation.hpp"
#include "ZoneAirMassFlowConservation_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneAirMassFlowConservation_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneAirMassFlowConservation_Impl::ZoneAirMassFlowConservation_Impl(const IdfObject& idfObject,
                                                                      Model_Impl* model,
                                                                      bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneAirMassFlowConservation::iddObjectType());
  }

  ZoneAirMassFlowConservation_Impl::ZoneAirMassFlowConservation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                      Model_Impl* model,
                                                                      bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneAirMassFlowConservation::iddObjectType());
  }

  ZoneAirMassFlowConservation_Impl::ZoneAirMassFlowConservation_Impl(const ZoneAirMassFlowConservation_Impl& other,
                                                                      Model_Impl* model,
                                                                      bool keepHandle)
    : ModelObject_Impl(other, model, keepHandle)
  {}

  const std::vector<std::string>& ZoneAirMassFlowConservation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Zone Infiltration Air Mass Flow Balance Status");
      result.push_back("Zone Mass Balance Infiltration Air Mass Flow Rate");
    }
    return result;
  }

  IddObjectType ZoneAirMassFlowConservation_Impl::iddObjectType() const {
    return ZoneAirMassFlowConservation::iddObjectType();
  }

  bool ZoneAirMassFlowConservation_Impl::adjustZoneMixingForZoneAirMassFlowBalance() const {
    boost::optional<std::string> value = getString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ZoneAirMassFlowConservation_Impl::isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted() const {
    return isEmpty(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance);
  }

  std::string ZoneAirMassFlowConservation_Impl::sourceZoneInfiltrationTreatment() const {
    return "";
  }

  bool ZoneAirMassFlowConservation_Impl::isSourceZoneInfiltrationTreatmentDefaulted() const {
    return false;
  }

  std::string ZoneAirMassFlowConservation_Impl::infiltrationBalancingMethod() const {
    boost::optional<std::string> value = getString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneAirMassFlowConservation_Impl::isInfiltrationBalancingMethodDefaulted() const {
    return isEmpty(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod);
  }

  std::string ZoneAirMassFlowConservation_Impl::infiltrationBalancingZones() const {
    boost::optional<std::string> value = getString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneAirMassFlowConservation_Impl::isInfiltrationBalancingZonesDefaulted() const {
    return isEmpty(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones);
  }

  void ZoneAirMassFlowConservation_Impl::setAdjustZoneMixingForZoneAirMassFlowBalance(bool adjustZoneMixingForZoneAirMassFlowBalance) {
    bool result = false;
    if (adjustZoneMixingForZoneAirMassFlowBalance) {
      result = setString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, "Yes");
    } else {
      result = setString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, "No");
    }
    OS_ASSERT(result);
  }

  void ZoneAirMassFlowConservation_Impl::resetAdjustZoneMixingForZoneAirMassFlowBalance() {
    bool result = setString(OS_ZoneAirMassFlowConservationFields::AdjustZoneMixingForZoneAirMassFlowBalance, "");
    OS_ASSERT(result);
  }

  bool ZoneAirMassFlowConservation_Impl::setSourceZoneInfiltrationTreatment(const std::string& sourceZoneInfiltrationTreatment) {
    return false;
  }

  void ZoneAirMassFlowConservation_Impl::resetSourceZoneInfiltrationTreatment() {
  }

  bool ZoneAirMassFlowConservation_Impl::setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod)
  {
    bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, infiltrationBalancingMethod);
    return result;
  }

  void ZoneAirMassFlowConservation_Impl::resetInfiltrationBalancingMethod()
  {
    bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, "");
    OS_ASSERT(result);
  }

  bool ZoneAirMassFlowConservation_Impl::setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones)
  {
    bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, infiltrationBalancingZones);
    return result;
  }

  void ZoneAirMassFlowConservation_Impl::resetInfiltrationBalancingZones()
  {
    bool result = setString(OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType ZoneAirMassFlowConservation::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneAirMassFlowConservation);
}

std::vector<std::string> ZoneAirMassFlowConservation::sourceZoneInfiltrationTreatmentValues() {
  return std::vector<std::string>();
}

std::vector<std::string> ZoneAirMassFlowConservation::infiltrationBalancingMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod);
}

std::vector<std::string> ZoneAirMassFlowConservation::infiltrationBalancingZonesValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneAirMassFlowConservationFields::InfiltrationBalancingZones);
}

bool ZoneAirMassFlowConservation::adjustZoneMixingForZoneAirMassFlowBalance() const {
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->adjustZoneMixingForZoneAirMassFlowBalance();
}

bool ZoneAirMassFlowConservation::isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted() const {
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted();
}

std::string ZoneAirMassFlowConservation::sourceZoneInfiltrationTreatment() const {
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->sourceZoneInfiltrationTreatment();
}

bool ZoneAirMassFlowConservation::isSourceZoneInfiltrationTreatmentDefaulted() const {
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isSourceZoneInfiltrationTreatmentDefaulted();
}

std::string ZoneAirMassFlowConservation::infiltrationBalancingMethod() const{
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->infiltrationBalancingMethod();
}

bool ZoneAirMassFlowConservation::isInfiltrationBalancingMethodDefaulted() const{
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isInfiltrationBalancingMethodDefaulted();
}

std::string ZoneAirMassFlowConservation::infiltrationBalancingZones() const{
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->infiltrationBalancingZones();
}

bool ZoneAirMassFlowConservation::isInfiltrationBalancingZonesDefaulted() const{
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->isInfiltrationBalancingZonesDefaulted();
}

void ZoneAirMassFlowConservation::setAdjustZoneMixingForZoneAirMassFlowBalance(bool adjustZoneMixingForZoneAirMassFlowBalance) {
  getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setAdjustZoneMixingForZoneAirMassFlowBalance(adjustZoneMixingForZoneAirMassFlowBalance);
}

void ZoneAirMassFlowConservation::resetAdjustZoneMixingForZoneAirMassFlowBalance() {
  getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetAdjustZoneMixingForZoneAirMassFlowBalance();
}

bool ZoneAirMassFlowConservation::setSourceZoneInfiltrationTreatment(const std::string& sourceZoneInfiltrationTreatment) {
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setSourceZoneInfiltrationTreatment(sourceZoneInfiltrationTreatment);
}

void ZoneAirMassFlowConservation::resetSourceZoneInfiltrationTreatment() {
  getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetSourceZoneInfiltrationTreatment();
}

bool ZoneAirMassFlowConservation::setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod)
{
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setInfiltrationBalancingMethod(infiltrationBalancingMethod);
}

void ZoneAirMassFlowConservation::resetInfiltrationBalancingMethod()
{
  getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetInfiltrationBalancingMethod();
}

bool ZoneAirMassFlowConservation::setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones)
{
  return getImpl<detail::ZoneAirMassFlowConservation_Impl>()->setInfiltrationBalancingZones(infiltrationBalancingZones);
}

void ZoneAirMassFlowConservation::resetInfiltrationBalancingZones() 
{
  getImpl<detail::ZoneAirMassFlowConservation_Impl>()->resetInfiltrationBalancingZones();
}


/// @cond
ZoneAirMassFlowConservation::ZoneAirMassFlowConservation(std::shared_ptr<detail::ZoneAirMassFlowConservation_Impl> impl)
  : ModelObject(impl)
{}
ZoneAirMassFlowConservation::ZoneAirMassFlowConservation(Model& model)
  : ModelObject(ZoneAirMassFlowConservation::iddObjectType(),model)
{}

/// @endcond

} // model
} // openstudio

