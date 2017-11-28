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

#include "RefrigerationGasCoolerAirCooled.hpp"
#include "RefrigerationGasCoolerAirCooled_Impl.hpp"

#include "CurveLinear.hpp"
#include "CurveLinear_Impl.hpp"
// #include "ThermalZone.hpp"
// #include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_GasCooler_AirCooled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationGasCoolerAirCooled_Impl::RefrigerationGasCoolerAirCooled_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationGasCoolerAirCooled::iddObjectType());
  }

  RefrigerationGasCoolerAirCooled_Impl::RefrigerationGasCoolerAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationGasCoolerAirCooled::iddObjectType());
  }

  RefrigerationGasCoolerAirCooled_Impl::RefrigerationGasCoolerAirCooled_Impl(const RefrigerationGasCoolerAirCooled_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationGasCoolerAirCooled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationGasCoolerAirCooled_Impl::iddObjectType() const {
    return RefrigerationGasCoolerAirCooled::iddObjectType();
  }

  ModelObject RefrigerationGasCoolerAirCooled_Impl::clone(Model model) const
  {
    RefrigerationGasCoolerAirCooled modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationGasCoolerAirCooled>();

    // modelObjectClone.resetAirInletNode();

    return modelObjectClone;
  }

  std::vector<IddObjectType> RefrigerationGasCoolerAirCooled_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Linear);
    return result;
  }

  std::vector<ModelObject> RefrigerationGasCoolerAirCooled_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<CurveLinear> intermediate = ratedTotalHeatRejectionRateCurve()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  boost::optional<CurveLinear> RefrigerationGasCoolerAirCooled_Impl::ratedTotalHeatRejectionRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_Refrigeration_GasCooler_AirCooledFields::RatedTotalHeatRejectionRateCurveName);
  }

  std::string RefrigerationGasCoolerAirCooled_Impl::gasCoolerFanSpeedControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerFanSpeedControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isGasCoolerFanSpeedControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerFanSpeedControlType);
  }

  double RefrigerationGasCoolerAirCooled_Impl::ratedFanPower() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::RatedFanPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isRatedFanPowerDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::RatedFanPower);
  }

  double RefrigerationGasCoolerAirCooled_Impl::minimumFanAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::MinimumFanAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isMinimumFanAirFlowRatioDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::MinimumFanAirFlowRatio);
  }

  double RefrigerationGasCoolerAirCooled_Impl::transitionTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::TransitionTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isTransitionTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::TransitionTemperature);
  }

  double RefrigerationGasCoolerAirCooled_Impl::transcriticalApproachTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::TranscriticalApproachTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isTranscriticalApproachTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::TranscriticalApproachTemperature);
  }

  double RefrigerationGasCoolerAirCooled_Impl::subcriticalTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::SubcriticalTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isSubcriticalTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::SubcriticalTemperatureDifference);
  }

  double RefrigerationGasCoolerAirCooled_Impl::minimumCondensingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::MinimumCondensingTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isMinimumCondensingTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::MinimumCondensingTemperature);
  }

  // boost::optional<ThermalZone> RefrigerationGasCoolerAirCooled_Impl::airInletNode() const {
  //   return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_GasCooler_AirCooledFields::AirInletNodeName);
  // }

  std::string RefrigerationGasCoolerAirCooled_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_GasCooler_AirCooledFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::EndUseSubcategory);
  }

  double RefrigerationGasCoolerAirCooled_Impl::gasCoolerRefrigerantOperatingChargeInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerRefrigerantOperatingChargeInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isGasCoolerRefrigerantOperatingChargeInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerRefrigerantOperatingChargeInventory);
  }

  double RefrigerationGasCoolerAirCooled_Impl::gasCoolerReceiverRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerReceiverRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isGasCoolerReceiverRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerReceiverRefrigerantInventory);
  }

  double RefrigerationGasCoolerAirCooled_Impl::gasCoolerOutletPipingRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerOutletPipingRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationGasCoolerAirCooled_Impl::isGasCoolerOutletPipingRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerOutletPipingRefrigerantInventory);
  }

  bool RefrigerationGasCoolerAirCooled_Impl::setRatedTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear) {
    bool result(false);
    if (curveLinear) {
        result = setPointer(OS_Refrigeration_GasCooler_AirCooledFields::RatedTotalHeatRejectionRateCurveName, curveLinear.get().handle());
    }
    else {
      resetRatedTotalHeatRejectionRateCurve();
      result = true;
    }
    return result;
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetRatedTotalHeatRejectionRateCurve() {
      bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::RatedTotalHeatRejectionRateCurveName, "");
      OS_ASSERT(result);
  }

  bool RefrigerationGasCoolerAirCooled_Impl::setGasCoolerFanSpeedControlType(std::string gasCoolerFanSpeedControlType) {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerFanSpeedControlType, gasCoolerFanSpeedControlType);
    return result;
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetGasCoolerFanSpeedControlType() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerFanSpeedControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationGasCoolerAirCooled_Impl::setRatedFanPower(double ratedFanPower) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::RatedFanPower, ratedFanPower);
    return result;
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetRatedFanPower() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::RatedFanPower, "");
    OS_ASSERT(result);
  }

  bool RefrigerationGasCoolerAirCooled_Impl::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::MinimumFanAirFlowRatio, minimumFanAirFlowRatio);
    return result;
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetMinimumFanAirFlowRatio() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::MinimumFanAirFlowRatio, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setTransitionTemperature(double transitionTemperature) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::TransitionTemperature, transitionTemperature);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetTransitionTemperature() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::TransitionTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setTranscriticalApproachTemperature(double transcriticalApproachTemperature) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::TranscriticalApproachTemperature, transcriticalApproachTemperature);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetTranscriticalApproachTemperature() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::TranscriticalApproachTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setSubcriticalTemperatureDifference(double subcriticalTemperatureDifference) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::SubcriticalTemperatureDifference, subcriticalTemperatureDifference);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetSubcriticalTemperatureDifference() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::SubcriticalTemperatureDifference, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::MinimumCondensingTemperature, minimumCondensingTemperature);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetMinimumCondensingTemperature() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::MinimumCondensingTemperature, "");
    OS_ASSERT(result);
  }

  // bool RefrigerationGasCoolerAirCooled_Impl::setAirInletNode(const boost::optional<ThermalZone>& thermalZone) {
  //   bool result(false);
  //   if (thermalZone) {
  //     result = setPointer(OS_Refrigeration_GasCooler_AirCooledFields::AirInletNodeName, thermalZone.get().handle());
  //   }
  //   else {
  //     resetAirInletNode();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationGasCoolerAirCooled_Impl::resetAirInletNode() {
  //   bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::AirInletNodeName, "");
  //   OS_ASSERT(result);
  // }

  void RefrigerationGasCoolerAirCooled_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setGasCoolerRefrigerantOperatingChargeInventory(double gasCoolerRefrigerantOperatingChargeInventory) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerRefrigerantOperatingChargeInventory, gasCoolerRefrigerantOperatingChargeInventory);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetGasCoolerRefrigerantOperatingChargeInventory() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerRefrigerantOperatingChargeInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setGasCoolerReceiverRefrigerantInventory(double gasCoolerReceiverRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerReceiverRefrigerantInventory, gasCoolerReceiverRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetGasCoolerReceiverRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerReceiverRefrigerantInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::setGasCoolerOutletPipingRefrigerantInventory(double gasCoolerOutletPipingRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerOutletPipingRefrigerantInventory, gasCoolerOutletPipingRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationGasCoolerAirCooled_Impl::resetGasCoolerOutletPipingRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerOutletPipingRefrigerantInventory, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationGasCoolerAirCooled::RefrigerationGasCoolerAirCooled(const Model& model)
  : ParentObject(RefrigerationGasCoolerAirCooled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>());

  CurveLinear heatRejectionCurve = CurveLinear(model);
  heatRejectionCurve.setName("Refrigeration Gas Cooler Air Cooled HR Curve");
  heatRejectionCurve.setCoefficient1Constant(0.0);
  heatRejectionCurve.setCoefficient2x(42000.0);
  heatRejectionCurve.setMinimumValueofx(3.0);
  heatRejectionCurve.setMaximumValueofx(22.2);

  bool ok = setRatedTotalHeatRejectionRateCurve(heatRejectionCurve);
  OS_ASSERT(ok);
  ok = setGasCoolerFanSpeedControlType("VariableSpeed");
  OS_ASSERT(ok);
  ok = setRatedFanPower(12000.0);
  OS_ASSERT(ok);
}

IddObjectType RefrigerationGasCoolerAirCooled::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_GasCooler_AirCooled);
}

std::vector<std::string> RefrigerationGasCoolerAirCooled::gasCoolerFanSpeedControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_GasCooler_AirCooledFields::GasCoolerFanSpeedControlType);
}

boost::optional<CurveLinear> RefrigerationGasCoolerAirCooled::ratedTotalHeatRejectionRateCurve() const {
    return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->ratedTotalHeatRejectionRateCurve();
}

std::string RefrigerationGasCoolerAirCooled::gasCoolerFanSpeedControlType() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->gasCoolerFanSpeedControlType();
}

bool RefrigerationGasCoolerAirCooled::isGasCoolerFanSpeedControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isGasCoolerFanSpeedControlTypeDefaulted();
}

double RefrigerationGasCoolerAirCooled::ratedFanPower() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->ratedFanPower();
}

bool RefrigerationGasCoolerAirCooled::isRatedFanPowerDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isRatedFanPowerDefaulted();
}

double RefrigerationGasCoolerAirCooled::minimumFanAirFlowRatio() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->minimumFanAirFlowRatio();
}

bool RefrigerationGasCoolerAirCooled::isMinimumFanAirFlowRatioDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isMinimumFanAirFlowRatioDefaulted();
}

double RefrigerationGasCoolerAirCooled::transitionTemperature() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->transitionTemperature();
}

bool RefrigerationGasCoolerAirCooled::isTransitionTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isTransitionTemperatureDefaulted();
}

double RefrigerationGasCoolerAirCooled::transcriticalApproachTemperature() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->transcriticalApproachTemperature();
}

bool RefrigerationGasCoolerAirCooled::isTranscriticalApproachTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isTranscriticalApproachTemperatureDefaulted();
}

double RefrigerationGasCoolerAirCooled::subcriticalTemperatureDifference() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->subcriticalTemperatureDifference();
}

bool RefrigerationGasCoolerAirCooled::isSubcriticalTemperatureDifferenceDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isSubcriticalTemperatureDifferenceDefaulted();
}

double RefrigerationGasCoolerAirCooled::minimumCondensingTemperature() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->minimumCondensingTemperature();
}

bool RefrigerationGasCoolerAirCooled::isMinimumCondensingTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isMinimumCondensingTemperatureDefaulted();
}

// boost::optional<ThermalZone> RefrigerationGasCoolerAirCooled::airInletNode() const {
//   return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->airInletNode();
// }

std::string RefrigerationGasCoolerAirCooled::endUseSubcategory() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->endUseSubcategory();
}

bool RefrigerationGasCoolerAirCooled::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isEndUseSubcategoryDefaulted();
}

double RefrigerationGasCoolerAirCooled::gasCoolerRefrigerantOperatingChargeInventory() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->gasCoolerRefrigerantOperatingChargeInventory();
}

bool RefrigerationGasCoolerAirCooled::isGasCoolerRefrigerantOperatingChargeInventoryDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isGasCoolerRefrigerantOperatingChargeInventoryDefaulted();
}

double RefrigerationGasCoolerAirCooled::gasCoolerReceiverRefrigerantInventory() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->gasCoolerReceiverRefrigerantInventory();
}

bool RefrigerationGasCoolerAirCooled::isGasCoolerReceiverRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isGasCoolerReceiverRefrigerantInventoryDefaulted();
}

double RefrigerationGasCoolerAirCooled::gasCoolerOutletPipingRefrigerantInventory() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->gasCoolerOutletPipingRefrigerantInventory();
}

bool RefrigerationGasCoolerAirCooled::isGasCoolerOutletPipingRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->isGasCoolerOutletPipingRefrigerantInventoryDefaulted();
}

bool RefrigerationGasCoolerAirCooled::setRatedTotalHeatRejectionRateCurve(const CurveLinear& curveLinear) {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setRatedTotalHeatRejectionRateCurve(curveLinear);
}

void RefrigerationGasCoolerAirCooled::resetRatedTotalHeatRejectionRateCurve() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetRatedTotalHeatRejectionRateCurve();
}

bool RefrigerationGasCoolerAirCooled::setGasCoolerFanSpeedControlType(std::string gasCoolerFanSpeedControlType) {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setGasCoolerFanSpeedControlType(gasCoolerFanSpeedControlType);
}

void RefrigerationGasCoolerAirCooled::resetGasCoolerFanSpeedControlType() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetGasCoolerFanSpeedControlType();
}

bool RefrigerationGasCoolerAirCooled::setRatedFanPower(double ratedFanPower) {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setRatedFanPower(ratedFanPower);
}

void RefrigerationGasCoolerAirCooled::resetRatedFanPower() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetRatedFanPower();
}

bool RefrigerationGasCoolerAirCooled::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
  return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setMinimumFanAirFlowRatio(minimumFanAirFlowRatio);
}

void RefrigerationGasCoolerAirCooled::resetMinimumFanAirFlowRatio() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetMinimumFanAirFlowRatio();
}

void RefrigerationGasCoolerAirCooled::setTransitionTemperature(double transitionTemperature) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setTransitionTemperature(transitionTemperature);
}

void RefrigerationGasCoolerAirCooled::resetTransitionTemperature() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetTransitionTemperature();
}

void RefrigerationGasCoolerAirCooled::setTranscriticalApproachTemperature(double transcriticalApproachTemperature) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setTranscriticalApproachTemperature(transcriticalApproachTemperature);
}

void RefrigerationGasCoolerAirCooled::resetTranscriticalApproachTemperature() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetTranscriticalApproachTemperature();
}

void RefrigerationGasCoolerAirCooled::setSubcriticalTemperatureDifference(double subcriticalTemperatureDifference) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setSubcriticalTemperatureDifference(subcriticalTemperatureDifference);
}

void RefrigerationGasCoolerAirCooled::resetSubcriticalTemperatureDifference() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetSubcriticalTemperatureDifference();
}

void RefrigerationGasCoolerAirCooled::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setMinimumCondensingTemperature(minimumCondensingTemperature);
}

void RefrigerationGasCoolerAirCooled::resetMinimumCondensingTemperature() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetMinimumCondensingTemperature();
}

// bool RefrigerationGasCoolerAirCooled::setAirInletNode(const ThermalZone& thermalZone) {
//   return getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setAirInletNode(thermalZone);
// }

// void RefrigerationGasCoolerAirCooled::resetAirInletNode() {
//   getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetAirInletNode();
// }

void RefrigerationGasCoolerAirCooled::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationGasCoolerAirCooled::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetEndUseSubcategory();
}

void RefrigerationGasCoolerAirCooled::setGasCoolerRefrigerantOperatingChargeInventory(double gasCoolerRefrigerantOperatingChargeInventory) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setGasCoolerRefrigerantOperatingChargeInventory(gasCoolerRefrigerantOperatingChargeInventory);
}

void RefrigerationGasCoolerAirCooled::resetGasCoolerRefrigerantOperatingChargeInventory() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetGasCoolerRefrigerantOperatingChargeInventory();
}

void RefrigerationGasCoolerAirCooled::setGasCoolerReceiverRefrigerantInventory(double gasCoolerReceiverRefrigerantInventory) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setGasCoolerReceiverRefrigerantInventory(gasCoolerReceiverRefrigerantInventory);
}

void RefrigerationGasCoolerAirCooled::resetGasCoolerReceiverRefrigerantInventory() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetGasCoolerReceiverRefrigerantInventory();
}

void RefrigerationGasCoolerAirCooled::setGasCoolerOutletPipingRefrigerantInventory(double gasCoolerOutletPipingRefrigerantInventory) {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->setGasCoolerOutletPipingRefrigerantInventory(gasCoolerOutletPipingRefrigerantInventory);
}

void RefrigerationGasCoolerAirCooled::resetGasCoolerOutletPipingRefrigerantInventory() {
  getImpl<detail::RefrigerationGasCoolerAirCooled_Impl>()->resetGasCoolerOutletPipingRefrigerantInventory();
}

/// @cond
RefrigerationGasCoolerAirCooled::RefrigerationGasCoolerAirCooled(std::shared_ptr<detail::RefrigerationGasCoolerAirCooled_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

