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

#include "RefrigerationCondenserAirCooled.hpp"
#include "RefrigerationCondenserAirCooled_Impl.hpp"

#include "CurveLinear.hpp"
#include "CurveLinear_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_Condenser_AirCooled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationCondenserAirCooled_Impl::RefrigerationCondenserAirCooled_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationCondenserAirCooled::iddObjectType());
  }

  RefrigerationCondenserAirCooled_Impl::RefrigerationCondenserAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationCondenserAirCooled::iddObjectType());
  }

  RefrigerationCondenserAirCooled_Impl::RefrigerationCondenserAirCooled_Impl(const RefrigerationCondenserAirCooled_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationCondenserAirCooled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationCondenserAirCooled_Impl::iddObjectType() const {
    return RefrigerationCondenserAirCooled::iddObjectType();
  }

  ModelObject RefrigerationCondenserAirCooled_Impl::clone(Model model) const
  {
    RefrigerationCondenserAirCooled modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationCondenserAirCooled>();

    modelObjectClone.resetAirInletZone();

    return modelObjectClone;
  }

  std::vector<IddObjectType> RefrigerationCondenserAirCooled_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Linear);
    return result;
  }

  std::vector<ModelObject> RefrigerationCondenserAirCooled_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<CurveLinear> intermediate = ratedEffectiveTotalHeatRejectionRateCurve()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  boost::optional<CurveLinear> RefrigerationCondenserAirCooled_Impl::ratedEffectiveTotalHeatRejectionRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_Refrigeration_Condenser_AirCooledFields::RatedEffectiveTotalHeatRejectionRateCurveName);
  }

  double RefrigerationCondenserAirCooled_Impl::ratedSubcoolingTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_AirCooledFields::RatedSubcoolingTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::RatedSubcoolingTemperatureDifference);
  }

  std::string RefrigerationCondenserAirCooled_Impl::condenserFanSpeedControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isCondenserFanSpeedControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType);
  }

  double RefrigerationCondenserAirCooled_Impl::ratedFanPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_AirCooledFields::RatedFanPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isRatedFanPowerDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::RatedFanPower);
  }

  double RefrigerationCondenserAirCooled_Impl::minimumFanAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_AirCooledFields::MinimumFanAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isMinimumFanAirFlowRatioDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::MinimumFanAirFlowRatio);
  }

  boost::optional<ThermalZone> RefrigerationCondenserAirCooled_Impl::airInletZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_Condenser_AirCooledFields::AirInletZoneName);
  }

  std::string RefrigerationCondenserAirCooled_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_AirCooledFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::EndUseSubcategory);
  }

  double RefrigerationCondenserAirCooled_Impl::condenserRefrigerantOperatingChargeInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_AirCooledFields::CondenserRefrigerantOperatingChargeInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isCondenserRefrigerantOperatingChargeInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::CondenserRefrigerantOperatingChargeInventory);
  }

  double RefrigerationCondenserAirCooled_Impl::condensateReceiverRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_AirCooledFields::CondensateReceiverRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isCondensateReceiverRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::CondensateReceiverRefrigerantInventory);
  }

  double RefrigerationCondenserAirCooled_Impl::condensatePipingRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_AirCooledFields::CondensatePipingRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserAirCooled_Impl::isCondensatePipingRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_AirCooledFields::CondensatePipingRefrigerantInventory);
  }

  bool RefrigerationCondenserAirCooled_Impl::setRatedEffectiveTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear) {
    bool result(false);
    if (curveLinear) {
      result = setPointer(OS_Refrigeration_Condenser_AirCooledFields::RatedEffectiveTotalHeatRejectionRateCurveName, curveLinear.get().handle());
    }
    else {
      resetRatedEffectiveTotalHeatRejectionRateCurve();
      result = true;
    }
    return result;
  }

  void RefrigerationCondenserAirCooled_Impl::resetRatedEffectiveTotalHeatRejectionRateCurve() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::RatedEffectiveTotalHeatRejectionRateCurveName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserAirCooled_Impl::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
    bool result = setDouble(OS_Refrigeration_Condenser_AirCooledFields::RatedSubcoolingTemperatureDifference, ratedSubcoolingTemperatureDifference);
    return result;
  }

  void RefrigerationCondenserAirCooled_Impl::resetRatedSubcoolingTemperatureDifference() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::RatedSubcoolingTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserAirCooled_Impl::setCondenserFanSpeedControlType(std::string condenserFanSpeedControlType) {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType, condenserFanSpeedControlType);
    return result;
  }

  void RefrigerationCondenserAirCooled_Impl::resetCondenserFanSpeedControlType() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserAirCooled_Impl::setRatedFanPower(double ratedFanPower) {
    bool result = setDouble(OS_Refrigeration_Condenser_AirCooledFields::RatedFanPower, ratedFanPower);
    return result;
  }

  void RefrigerationCondenserAirCooled_Impl::resetRatedFanPower() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::RatedFanPower, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserAirCooled_Impl::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
    bool result = setDouble(OS_Refrigeration_Condenser_AirCooledFields::MinimumFanAirFlowRatio, minimumFanAirFlowRatio);
    return result;
  }

  void RefrigerationCondenserAirCooled_Impl::resetMinimumFanAirFlowRatio() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::MinimumFanAirFlowRatio, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserAirCooled_Impl::setAirInletZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Refrigeration_Condenser_AirCooledFields::AirInletZoneName, thermalZone.get().handle());
    }
    else {
      resetAirInletZone();
      result = true;
    }
    return result;
  }

  void RefrigerationCondenserAirCooled_Impl::resetAirInletZone() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::AirInletZoneName, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
    bool result = setDouble(OS_Refrigeration_Condenser_AirCooledFields::CondenserRefrigerantOperatingChargeInventory, condenserRefrigerantOperatingChargeInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::resetCondenserRefrigerantOperatingChargeInventory() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::CondenserRefrigerantOperatingChargeInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_Condenser_AirCooledFields::CondensateReceiverRefrigerantInventory, condensateReceiverRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::resetCondensateReceiverRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::CondensateReceiverRefrigerantInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_Condenser_AirCooledFields::CondensatePipingRefrigerantInventory, condensatePipingRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserAirCooled_Impl::resetCondensatePipingRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_Condenser_AirCooledFields::CondensatePipingRefrigerantInventory, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationCondenserAirCooled::RefrigerationCondenserAirCooled(const Model& model)
  : ParentObject(RefrigerationCondenserAirCooled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationCondenserAirCooled_Impl>());

  bool ok = true;

  CurveLinear heatRejectionCurve = CurveLinear(model);
  heatRejectionCurve.setName("Refrigeration Condenser Air Cooled HR Curve");
  heatRejectionCurve.setCoefficient1Constant(0.0);
  heatRejectionCurve.setCoefficient2x(22000.0);
  heatRejectionCurve.setMinimumValueofx(5.0);
  heatRejectionCurve.setMaximumValueofx(22.2);

  setRatedEffectiveTotalHeatRejectionRateCurve(heatRejectionCurve);
  OS_ASSERT(ok);

  setRatedSubcoolingTemperatureDifference(0.0);
  setCondenserFanSpeedControlType("VariableSpeed");
  setRatedFanPower(7000.0);
  setMinimumFanAirFlowRatio(0.25);
}

IddObjectType RefrigerationCondenserAirCooled::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Condenser_AirCooled);
}

std::vector<std::string> RefrigerationCondenserAirCooled::condenserFanSpeedControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_Condenser_AirCooledFields::CondenserFanSpeedControlType);
}

boost::optional<CurveLinear> RefrigerationCondenserAirCooled::ratedEffectiveTotalHeatRejectionRateCurve() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->ratedEffectiveTotalHeatRejectionRateCurve();
}

double RefrigerationCondenserAirCooled::ratedSubcoolingTemperatureDifference() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->ratedSubcoolingTemperatureDifference();
}

bool RefrigerationCondenserAirCooled::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isRatedSubcoolingTemperatureDifferenceDefaulted();
}

std::string RefrigerationCondenserAirCooled::condenserFanSpeedControlType() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->condenserFanSpeedControlType();
}

bool RefrigerationCondenserAirCooled::isCondenserFanSpeedControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isCondenserFanSpeedControlTypeDefaulted();
}

double RefrigerationCondenserAirCooled::ratedFanPower() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->ratedFanPower();
}

bool RefrigerationCondenserAirCooled::isRatedFanPowerDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isRatedFanPowerDefaulted();
}

double RefrigerationCondenserAirCooled::minimumFanAirFlowRatio() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->minimumFanAirFlowRatio();
}

bool RefrigerationCondenserAirCooled::isMinimumFanAirFlowRatioDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isMinimumFanAirFlowRatioDefaulted();
}

boost::optional<ThermalZone> RefrigerationCondenserAirCooled::airInletZone() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->airInletZone();
}

std::string RefrigerationCondenserAirCooled::endUseSubcategory() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->endUseSubcategory();
}

bool RefrigerationCondenserAirCooled::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isEndUseSubcategoryDefaulted();
}

double RefrigerationCondenserAirCooled::condenserRefrigerantOperatingChargeInventory() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->condenserRefrigerantOperatingChargeInventory();
}

bool RefrigerationCondenserAirCooled::isCondenserRefrigerantOperatingChargeInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isCondenserRefrigerantOperatingChargeInventoryDefaulted();
}

double RefrigerationCondenserAirCooled::condensateReceiverRefrigerantInventory() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->condensateReceiverRefrigerantInventory();
}

bool RefrigerationCondenserAirCooled::isCondensateReceiverRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isCondensateReceiverRefrigerantInventoryDefaulted();
}

double RefrigerationCondenserAirCooled::condensatePipingRefrigerantInventory() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->condensatePipingRefrigerantInventory();
}

bool RefrigerationCondenserAirCooled::isCondensatePipingRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->isCondensatePipingRefrigerantInventoryDefaulted();
}

bool RefrigerationCondenserAirCooled::setRatedEffectiveTotalHeatRejectionRateCurve(const CurveLinear& curveLinear) {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setRatedEffectiveTotalHeatRejectionRateCurve(curveLinear);
}

void RefrigerationCondenserAirCooled::resetRatedEffectiveTotalHeatRejectionRateCurve() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetRatedEffectiveTotalHeatRejectionRateCurve();
}

bool RefrigerationCondenserAirCooled::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setRatedSubcoolingTemperatureDifference(ratedSubcoolingTemperatureDifference);
}

void RefrigerationCondenserAirCooled::resetRatedSubcoolingTemperatureDifference() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetRatedSubcoolingTemperatureDifference();
}

bool RefrigerationCondenserAirCooled::setCondenserFanSpeedControlType(std::string condenserFanSpeedControlType) {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setCondenserFanSpeedControlType(condenserFanSpeedControlType);
}

void RefrigerationCondenserAirCooled::resetCondenserFanSpeedControlType() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetCondenserFanSpeedControlType();
}

bool RefrigerationCondenserAirCooled::setRatedFanPower(double ratedFanPower) {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setRatedFanPower(ratedFanPower);
}

void RefrigerationCondenserAirCooled::resetRatedFanPower() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetRatedFanPower();
}

bool RefrigerationCondenserAirCooled::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setMinimumFanAirFlowRatio(minimumFanAirFlowRatio);
}

void RefrigerationCondenserAirCooled::resetMinimumFanAirFlowRatio() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetMinimumFanAirFlowRatio();
}

bool RefrigerationCondenserAirCooled::setAirInletZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setAirInletZone(thermalZone);
}

void RefrigerationCondenserAirCooled::resetAirInletZone() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetAirInletZone();
}

void RefrigerationCondenserAirCooled::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationCondenserAirCooled::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetEndUseSubcategory();
}

void RefrigerationCondenserAirCooled::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setCondenserRefrigerantOperatingChargeInventory(condenserRefrigerantOperatingChargeInventory);
}

void RefrigerationCondenserAirCooled::resetCondenserRefrigerantOperatingChargeInventory() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetCondenserRefrigerantOperatingChargeInventory();
}

void RefrigerationCondenserAirCooled::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setCondensateReceiverRefrigerantInventory(condensateReceiverRefrigerantInventory);
}

void RefrigerationCondenserAirCooled::resetCondensateReceiverRefrigerantInventory() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetCondensateReceiverRefrigerantInventory();
}

void RefrigerationCondenserAirCooled::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->setCondensatePipingRefrigerantInventory(condensatePipingRefrigerantInventory);
}

void RefrigerationCondenserAirCooled::resetCondensatePipingRefrigerantInventory() {
  getImpl<detail::RefrigerationCondenserAirCooled_Impl>()->resetCondensatePipingRefrigerantInventory();
}

/// @cond
RefrigerationCondenserAirCooled::RefrigerationCondenserAirCooled(std::shared_ptr<detail::RefrigerationCondenserAirCooled_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

