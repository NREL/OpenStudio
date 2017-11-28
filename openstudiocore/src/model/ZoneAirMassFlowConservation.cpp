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
  : ModelObject(std::move(impl))
{}
ZoneAirMassFlowConservation::ZoneAirMassFlowConservation(Model& model)
  : ModelObject(ZoneAirMassFlowConservation::iddObjectType(),model)
{}

/// @endcond

} // model
} // openstudio

