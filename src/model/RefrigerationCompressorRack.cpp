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

#include "RefrigerationCompressorRack.hpp"
#include "RefrigerationCompressorRack_Impl.hpp"

#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"
#include "CurveLinear.hpp"
#include "CurveLinear_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_CompressorRack_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    RefrigerationCompressorRack_Impl::RefrigerationCompressorRack_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RefrigerationCompressorRack::iddObjectType());
    }

    RefrigerationCompressorRack_Impl::RefrigerationCompressorRack_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RefrigerationCompressorRack::iddObjectType());
    }

    RefrigerationCompressorRack_Impl::RefrigerationCompressorRack_Impl(const RefrigerationCompressorRack_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& RefrigerationCompressorRack_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // TODO: implement checks
        // FOR CONDENSERS COOLING SYSTEMS SERVING CASES AND/OR WALKINS:
        "Refrigeration System Condenser Fan Electricity Rate", "Refrigeration System Condenser Fan Electricity Energy",
        "Refrigeration System Condenser Heat Transfer Rate", "Refrigeration System Condenser Heat Transfer Energy",
        "Refrigeration System Condenser Total Recovered Heat Transfer Rate",
        "Refrigeration System Condenser Non Refrigeration Recovered Heat Transfer Rate",
        "Refrigeration System Condenser Heat Recovered for Non-Refrigeration Purposes Energy",
        "Refrigeration System Condenser Defrost Recovered Heat Transfer Rate",
        "Refrigeration System Condenser Defrost Recovered Heat Transfer Energy",
        // FOR CONDENSERS COOLING SYSTEMS SERVING AIR CHILLERS:
        "Refrigeration System Condenser Fan Electricity Rate", "Refrigeration System Condenser Fan Electric Consumption",
        "Refrigeration Air Chiller System Condenser Heat Transfer Rate", "Refrigeration Air Chiller System Condenser Heat Transfer Energy",
        "Refrigeration Air Chiller System Condenser Total Recovered Heat Transfer Rate",
        "Refrigeration Air Chiller System Condenser Non Refrigeration Recovered Heat Transfer Rate",
        "Refrigeration Air Chiller System Condenser Non Refrigeration Recovered Heat Transfer Energy",
        "Refrigeration Air Chiller System Condenser Defrost Recovered Heat Transfer Rate",
        "Refrigeration Air Chiller System Condenser Defrost Recovered Heat Transfer Energy"};
      return result;
    }

    IddObjectType RefrigerationCompressorRack_Impl::iddObjectType() const {
      return RefrigerationCompressorRack::iddObjectType();
    }

    ModelObject RefrigerationCompressorRack_Impl::clone(Model model) const {
      RefrigerationCompressorRack modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationCompressorRack>();

      modelObjectClone.resetAirInletZone();

      return modelObjectClone;
    }

    std::vector<IddObjectType> RefrigerationCompressorRack_Impl::allowableChildTypes() const {
      std::vector<IddObjectType> result;
      result.push_back(IddObjectType::OS_Curve_Linear);
      return result;
    }

    std::vector<ModelObject> RefrigerationCompressorRack_Impl::children() const {
      std::vector<ModelObject> result;
      if (boost::optional<CurveLinear> intermediate = ratedEffectiveTotalHeatRejectionRateCurve()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    boost::optional<CurveLinear> RefrigerationCompressorRack_Impl::ratedEffectiveTotalHeatRejectionRateCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(
        OS_Refrigeration_CompressorRackFields::RatedEffectiveTotalHeatRejectionRateCurveName);
    }

    double RefrigerationCompressorRack_Impl::ratedSubcoolingTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::RatedSubcoolingTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::RatedSubcoolingTemperatureDifference);
    }

    std::string RefrigerationCompressorRack_Impl::condenserFanSpeedControlType() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::CondenserFanSpeedControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isCondenserFanSpeedControlTypeDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::CondenserFanSpeedControlType);
    }

    double RefrigerationCompressorRack_Impl::ratedFanPower() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::RatedFanPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isRatedFanPowerDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::RatedFanPower);
    }

    double RefrigerationCompressorRack_Impl::minimumFanAirFlowRatio() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::MinimumFanAirFlowRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isMinimumFanAirFlowRatioDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::MinimumFanAirFlowRatio);
    }

    boost::optional<ThermalZone> RefrigerationCompressorRack_Impl::airInletZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_CompressorRackFields::AirInletZoneName);
    }

    std::string RefrigerationCompressorRack_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Refrigeration_CompressorRackFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::EndUseSubcategory);
    }

    double RefrigerationCompressorRack_Impl::condenserRefrigerantOperatingChargeInventory() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::CondenserRefrigerantOperatingChargeInventory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isCondenserRefrigerantOperatingChargeInventoryDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::CondenserRefrigerantOperatingChargeInventory);
    }

    double RefrigerationCompressorRack_Impl::condensateReceiverRefrigerantInventory() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::CondensateReceiverRefrigerantInventory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isCondensateReceiverRefrigerantInventoryDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::CondensateReceiverRefrigerantInventory);
    }

    double RefrigerationCompressorRack_Impl::condensatePipingRefrigerantInventory() const {
      boost::optional<double> value = getDouble(OS_Refrigeration_CompressorRackFields::CondensatePipingRefrigerantInventory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RefrigerationCompressorRack_Impl::isCondensatePipingRefrigerantInventoryDefaulted() const {
      return isEmpty(OS_Refrigeration_CompressorRackFields::CondensatePipingRefrigerantInventory);
    }

    bool RefrigerationCompressorRack_Impl::setRatedEffectiveTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear) {
      bool result(false);
      if (curveLinear) {
        result = setPointer(OS_Refrigeration_CompressorRackFields::RatedEffectiveTotalHeatRejectionRateCurveName, curveLinear.get().handle());
      } else {
        resetRatedEffectiveTotalHeatRejectionRateCurve();
        result = true;
      }
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetRatedEffectiveTotalHeatRejectionRateCurve() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::RatedEffectiveTotalHeatRejectionRateCurveName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::RatedSubcoolingTemperatureDifference, ratedSubcoolingTemperatureDifference);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetRatedSubcoolingTemperatureDifference() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::RatedSubcoolingTemperatureDifference, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setCondenserFanSpeedControlType(std::string condenserFanSpeedControlType) {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondenserFanSpeedControlType, condenserFanSpeedControlType);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetCondenserFanSpeedControlType() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondenserFanSpeedControlType, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setRatedFanPower(double ratedFanPower) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::RatedFanPower, ratedFanPower);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetRatedFanPower() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::RatedFanPower, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::MinimumFanAirFlowRatio, minimumFanAirFlowRatio);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetMinimumFanAirFlowRatio() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::MinimumFanAirFlowRatio, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setAirInletZone(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);
      if (thermalZone) {
        result = setPointer(OS_Refrigeration_CompressorRackFields::AirInletZoneName, thermalZone.get().handle());
      } else {
        resetAirInletZone();
        result = true;
      }
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetAirInletZone() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::AirInletZoneName, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
      bool result = setString(OS_Refrigeration_CompressorRackFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::CondenserRefrigerantOperatingChargeInventory,
                              condenserRefrigerantOperatingChargeInventory);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetCondenserRefrigerantOperatingChargeInventory() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondenserRefrigerantOperatingChargeInventory, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
      bool result =
        setDouble(OS_Refrigeration_CompressorRackFields::CondensateReceiverRefrigerantInventory, condensateReceiverRefrigerantInventory);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetCondensateReceiverRefrigerantInventory() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondensateReceiverRefrigerantInventory, "");
      OS_ASSERT(result);
    }

    bool RefrigerationCompressorRack_Impl::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
      bool result = setDouble(OS_Refrigeration_CompressorRackFields::CondensatePipingRefrigerantInventory, condensatePipingRefrigerantInventory);
      OS_ASSERT(result);
      return result;
    }

    void RefrigerationCompressorRack_Impl::resetCondensatePipingRefrigerantInventory() {
      bool result = setString(OS_Refrigeration_CompressorRackFields::CondensatePipingRefrigerantInventory, "");
      OS_ASSERT(result);
    }

    boost::optional<RefrigerationSystem> RefrigerationCompressorRack_Impl::system() const {

      boost::optional<RefrigerationSystem> system;
      // We use getModelObjectSources to check if more than one
      std::vector<RefrigerationSystem> systems =
        getObject<ModelObject>().getModelObjectSources<RefrigerationSystem>(RefrigerationSystem::iddObjectType());

      if (systems.size() > 0u) {
        if (systems.size() > 1u) {
          LOG(Error, briefDescription() << " is referenced by more than one RefrigerationSystem, returning the first");
        }
        system = systems[0];
      }
      return system;
    }

  }  // namespace detail

  RefrigerationCompressorRack::RefrigerationCompressorRack(const Model& model)
    : ParentObject(RefrigerationCompressorRack::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RefrigerationCompressorRack_Impl>());

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

  IddObjectType RefrigerationCompressorRack::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Refrigeration_CompressorRack);
  }

  std::vector<std::string> RefrigerationCompressorRack::condenserFanSpeedControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Refrigeration_CompressorRackFields::CondenserFanSpeedControlType);
  }

  boost::optional<CurveLinear> RefrigerationCompressorRack::ratedEffectiveTotalHeatRejectionRateCurve() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->ratedEffectiveTotalHeatRejectionRateCurve();
  }

  double RefrigerationCompressorRack::ratedSubcoolingTemperatureDifference() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->ratedSubcoolingTemperatureDifference();
  }

  bool RefrigerationCompressorRack::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isRatedSubcoolingTemperatureDifferenceDefaulted();
  }

  std::string RefrigerationCompressorRack::condenserFanSpeedControlType() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->condenserFanSpeedControlType();
  }

  bool RefrigerationCompressorRack::isCondenserFanSpeedControlTypeDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isCondenserFanSpeedControlTypeDefaulted();
  }

  double RefrigerationCompressorRack::ratedFanPower() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->ratedFanPower();
  }

  bool RefrigerationCompressorRack::isRatedFanPowerDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isRatedFanPowerDefaulted();
  }

  double RefrigerationCompressorRack::minimumFanAirFlowRatio() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->minimumFanAirFlowRatio();
  }

  bool RefrigerationCompressorRack::isMinimumFanAirFlowRatioDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isMinimumFanAirFlowRatioDefaulted();
  }

  boost::optional<ThermalZone> RefrigerationCompressorRack::airInletZone() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->airInletZone();
  }

  std::string RefrigerationCompressorRack::endUseSubcategory() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->endUseSubcategory();
  }

  bool RefrigerationCompressorRack::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isEndUseSubcategoryDefaulted();
  }

  double RefrigerationCompressorRack::condenserRefrigerantOperatingChargeInventory() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->condenserRefrigerantOperatingChargeInventory();
  }

  bool RefrigerationCompressorRack::isCondenserRefrigerantOperatingChargeInventoryDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isCondenserRefrigerantOperatingChargeInventoryDefaulted();
  }

  double RefrigerationCompressorRack::condensateReceiverRefrigerantInventory() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->condensateReceiverRefrigerantInventory();
  }

  bool RefrigerationCompressorRack::isCondensateReceiverRefrigerantInventoryDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isCondensateReceiverRefrigerantInventoryDefaulted();
  }

  double RefrigerationCompressorRack::condensatePipingRefrigerantInventory() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->condensatePipingRefrigerantInventory();
  }

  bool RefrigerationCompressorRack::isCondensatePipingRefrigerantInventoryDefaulted() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->isCondensatePipingRefrigerantInventoryDefaulted();
  }

  bool RefrigerationCompressorRack::setRatedEffectiveTotalHeatRejectionRateCurve(const CurveLinear& curveLinear) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setRatedEffectiveTotalHeatRejectionRateCurve(curveLinear);
  }

  void RefrigerationCompressorRack::resetRatedEffectiveTotalHeatRejectionRateCurve() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetRatedEffectiveTotalHeatRejectionRateCurve();
  }

  bool RefrigerationCompressorRack::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setRatedSubcoolingTemperatureDifference(ratedSubcoolingTemperatureDifference);
  }

  void RefrigerationCompressorRack::resetRatedSubcoolingTemperatureDifference() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetRatedSubcoolingTemperatureDifference();
  }

  bool RefrigerationCompressorRack::setCondenserFanSpeedControlType(std::string condenserFanSpeedControlType) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondenserFanSpeedControlType(condenserFanSpeedControlType);
  }

  void RefrigerationCompressorRack::resetCondenserFanSpeedControlType() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetCondenserFanSpeedControlType();
  }

  bool RefrigerationCompressorRack::setRatedFanPower(double ratedFanPower) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setRatedFanPower(ratedFanPower);
  }

  void RefrigerationCompressorRack::resetRatedFanPower() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetRatedFanPower();
  }

  bool RefrigerationCompressorRack::setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setMinimumFanAirFlowRatio(minimumFanAirFlowRatio);
  }

  void RefrigerationCompressorRack::resetMinimumFanAirFlowRatio() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetMinimumFanAirFlowRatio();
  }

  bool RefrigerationCompressorRack::setAirInletZone(const ThermalZone& thermalZone) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setAirInletZone(thermalZone);
  }

  void RefrigerationCompressorRack::resetAirInletZone() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetAirInletZone();
  }

  bool RefrigerationCompressorRack::setEndUseSubcategory(std::string endUseSubcategory) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void RefrigerationCompressorRack::resetEndUseSubcategory() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetEndUseSubcategory();
  }

  bool RefrigerationCompressorRack::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondenserRefrigerantOperatingChargeInventory(
      condenserRefrigerantOperatingChargeInventory);
  }

  void RefrigerationCompressorRack::resetCondenserRefrigerantOperatingChargeInventory() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetCondenserRefrigerantOperatingChargeInventory();
  }

  bool RefrigerationCompressorRack::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondensateReceiverRefrigerantInventory(condensateReceiverRefrigerantInventory);
  }

  void RefrigerationCompressorRack::resetCondensateReceiverRefrigerantInventory() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetCondensateReceiverRefrigerantInventory();
  }

  bool RefrigerationCompressorRack::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->setCondensatePipingRefrigerantInventory(condensatePipingRefrigerantInventory);
  }

  void RefrigerationCompressorRack::resetCondensatePipingRefrigerantInventory() {
    getImpl<detail::RefrigerationCompressorRack_Impl>()->resetCondensatePipingRefrigerantInventory();
  }

  boost::optional<RefrigerationSystem> RefrigerationCompressorRack::system() const {
    return getImpl<detail::RefrigerationCompressorRack_Impl>()->system();
  }

  /// @cond
  RefrigerationCompressorRack::RefrigerationCompressorRack(std::shared_ptr<detail::RefrigerationCompressorRack_Impl> impl)
    : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
