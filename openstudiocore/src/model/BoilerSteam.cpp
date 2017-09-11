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

#include "BoilerSteam.hpp"
#include "BoilerSteam_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Boiler_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  BoilerSteam_Impl::BoilerSteam_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == BoilerSteam::iddObjectType());
  }

  BoilerSteam_Impl::BoilerSteam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == BoilerSteam::iddObjectType());
  }

  BoilerSteam_Impl::BoilerSteam_Impl(const BoilerSteam_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& BoilerSteam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType BoilerSteam_Impl::iddObjectType() const {
    return BoilerSteam::iddObjectType();
  }

  std::string BoilerSteam_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_Boiler_SteamFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> BoilerSteam_Impl::maximumOperatingPressure() const {
    return getDouble(OS_Boiler_SteamFields::MaximumOperatingPressure,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getMaximumOperatingPressure(bool returnIP) const {
    OptionalDouble value = maximumOperatingPressure();
    return getQuantityFromDouble(OS_Boiler_SteamFields::MaximumOperatingPressure, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::theoreticalEfficiency() const {
    return getDouble(OS_Boiler_SteamFields::TheoreticalEfficiency,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getTheoreticalEfficiency(bool returnIP) const {
    OptionalDouble value = theoreticalEfficiency();
    return getQuantityFromDouble(OS_Boiler_SteamFields::TheoreticalEfficiency, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::designOutletSteamTemperature() const {
    return getDouble(OS_Boiler_SteamFields::DesignOutletSteamTemperature,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getDesignOutletSteamTemperature(bool returnIP) const {
    OptionalDouble value = designOutletSteamTemperature();
    return getQuantityFromDouble(OS_Boiler_SteamFields::DesignOutletSteamTemperature, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::nominalCapacity() const {
    return getDouble(OS_Boiler_SteamFields::NominalCapacity,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getNominalCapacity(bool returnIP) const {
    OptionalDouble value = nominalCapacity();
    return getQuantityFromDouble(OS_Boiler_SteamFields::NominalCapacity, value, returnIP);
  }

  bool BoilerSteam_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Boiler_SteamFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> BoilerSteam_Impl::minimumPartLoadRatio() const {
    return getDouble(OS_Boiler_SteamFields::MinimumPartLoadRatio,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getMinimumPartLoadRatio(bool returnIP) const {
    OptionalDouble value = minimumPartLoadRatio();
    return getQuantityFromDouble(OS_Boiler_SteamFields::MinimumPartLoadRatio, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::maximumPartLoadRatio() const {
    return getDouble(OS_Boiler_SteamFields::MaximumPartLoadRatio,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getMaximumPartLoadRatio(bool returnIP) const {
    OptionalDouble value = maximumPartLoadRatio();
    return getQuantityFromDouble(OS_Boiler_SteamFields::MaximumPartLoadRatio, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::optimumPartLoadRatio() const {
    return getDouble(OS_Boiler_SteamFields::OptimumPartLoadRatio,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getOptimumPartLoadRatio(bool returnIP) const {
    OptionalDouble value = optimumPartLoadRatio();
    return getQuantityFromDouble(OS_Boiler_SteamFields::OptimumPartLoadRatio, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const {
    return getDouble(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP) const {
    OptionalDouble value = coefficient1ofFuelUseFunctionofPartLoadRatioCurve();
    return getQuantityFromDouble(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const {
    return getDouble(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP) const {
    OptionalDouble value = coefficient2ofFuelUseFunctionofPartLoadRatioCurve();
    return getQuantityFromDouble(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, value, returnIP);
  }

  boost::optional<double> BoilerSteam_Impl::coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const {
    return getDouble(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve,true);
  }

  OSOptionalQuantity BoilerSteam_Impl::getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP) const {
    OptionalDouble value = coefficient3ofFuelUseFunctionofPartLoadRatioCurve();
    return getQuantityFromDouble(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, value, returnIP);
  }

  double BoilerSteam_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_Boiler_SteamFields::SizingFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity BoilerSteam_Impl::getSizingFactor(bool returnIP) const {
    OptionalDouble value = sizingFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Boiler_SteamFields::SizingFactor, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool BoilerSteam_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_Boiler_SteamFields::SizingFactor);
  }

  bool BoilerSteam_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_Boiler_SteamFields::FuelType, fuelType);
    return result;
  }

  void BoilerSteam_Impl::setMaximumOperatingPressure(boost::optional<double> maximumOperatingPressure) {
    bool result(false);
    if (maximumOperatingPressure) {
      result = setDouble(OS_Boiler_SteamFields::MaximumOperatingPressure, maximumOperatingPressure.get());
    }
    else {
      resetMaximumOperatingPressure();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setMaximumOperatingPressure(const OSOptionalQuantity& maximumOperatingPressure) {
    bool result(false);
    OptionalDouble value;
    if (maximumOperatingPressure.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::MaximumOperatingPressure,maximumOperatingPressure.get());
      if (value) {
        setMaximumOperatingPressure(value);

        result = true;
      }
    }
    else {
      setMaximumOperatingPressure(value);
      result = true;
    }
    return result;
  }

  void BoilerSteam_Impl::resetMaximumOperatingPressure() {
    bool result = setString(OS_Boiler_SteamFields::MaximumOperatingPressure, "");
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setTheoreticalEfficiency(boost::optional<double> theoreticalEfficiency) {
    bool result(false);
    if (theoreticalEfficiency) {
      result = setDouble(OS_Boiler_SteamFields::TheoreticalEfficiency, theoreticalEfficiency.get());
    }
    else {
      resetTheoreticalEfficiency();
      result = true;
    }
    return result;
  }

  bool BoilerSteam_Impl::setTheoreticalEfficiency(const OSOptionalQuantity& theoreticalEfficiency) {
    bool result(false);
    OptionalDouble value;
    if (theoreticalEfficiency.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::TheoreticalEfficiency,theoreticalEfficiency.get());
      if (value) {
        result = setTheoreticalEfficiency(value);
      }
    }
    else {
      result = setTheoreticalEfficiency(value);
    }
    return result;
  }

  void BoilerSteam_Impl::resetTheoreticalEfficiency() {
    bool result = setString(OS_Boiler_SteamFields::TheoreticalEfficiency, "");
    OS_ASSERT(result);
  }

  void BoilerSteam_Impl::setDesignOutletSteamTemperature(boost::optional<double> designOutletSteamTemperature) {
    bool result(false);
    if (designOutletSteamTemperature) {
      result = setDouble(OS_Boiler_SteamFields::DesignOutletSteamTemperature, designOutletSteamTemperature.get());
    }
    else {
      resetDesignOutletSteamTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setDesignOutletSteamTemperature(const OSOptionalQuantity& designOutletSteamTemperature) {
    bool result(false);
    OptionalDouble value;
    if (designOutletSteamTemperature.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::DesignOutletSteamTemperature,designOutletSteamTemperature.get());
      if (value) {
        setDesignOutletSteamTemperature(value);

        result = true;
      }
    }
    else {
      setDesignOutletSteamTemperature(value);
      result = true;
    }
    return result;
  }

  void BoilerSteam_Impl::resetDesignOutletSteamTemperature() {
    bool result = setString(OS_Boiler_SteamFields::DesignOutletSteamTemperature, "");
    OS_ASSERT(result);
  }

  void BoilerSteam_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_Boiler_SteamFields::NominalCapacity, nominalCapacity.get());
    }
    else {
      resetNominalCapacity();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setNominalCapacity(const OSOptionalQuantity& nominalCapacity) {
    bool result(false);
    OptionalDouble value;
    if (nominalCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::NominalCapacity,nominalCapacity.get());
      if (value) {
        setNominalCapacity(value);

        result = true;
      }
    }
    else {
      setNominalCapacity(value);
      result = true;
    }
    return result;
  }

  void BoilerSteam_Impl::resetNominalCapacity() {
    bool result = setString(OS_Boiler_SteamFields::NominalCapacity, "");
    OS_ASSERT(result);
  }

  void BoilerSteam_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Boiler_SteamFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setMinimumPartLoadRatio(boost::optional<double> minimumPartLoadRatio) {
    bool result(false);
    if (minimumPartLoadRatio) {
      result = setDouble(OS_Boiler_SteamFields::MinimumPartLoadRatio, minimumPartLoadRatio.get());
    }
    else {
      resetMinimumPartLoadRatio();
      result = true;
    }
    return result;
  }

  bool BoilerSteam_Impl::setMinimumPartLoadRatio(const OSOptionalQuantity& minimumPartLoadRatio) {
    bool result(false);
    OptionalDouble value;
    if (minimumPartLoadRatio.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::MinimumPartLoadRatio,minimumPartLoadRatio.get());
      if (value) {
        result = setMinimumPartLoadRatio(value);
      }
    }
    else {
      result = setMinimumPartLoadRatio(value);
    }
    return result;
  }

  void BoilerSteam_Impl::resetMinimumPartLoadRatio() {
    bool result = setString(OS_Boiler_SteamFields::MinimumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setMaximumPartLoadRatio(boost::optional<double> maximumPartLoadRatio) {
    bool result(false);
    if (maximumPartLoadRatio) {
      result = setDouble(OS_Boiler_SteamFields::MaximumPartLoadRatio, maximumPartLoadRatio.get());
    }
    else {
      resetMaximumPartLoadRatio();
      result = true;
    }
    return result;
  }

  bool BoilerSteam_Impl::setMaximumPartLoadRatio(const OSOptionalQuantity& maximumPartLoadRatio) {
    bool result(false);
    OptionalDouble value;
    if (maximumPartLoadRatio.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::MaximumPartLoadRatio,maximumPartLoadRatio.get());
      if (value) {
        result = setMaximumPartLoadRatio(value);
      }
    }
    else {
      result = setMaximumPartLoadRatio(value);
    }
    return result;
  }

  void BoilerSteam_Impl::resetMaximumPartLoadRatio() {
    bool result = setString(OS_Boiler_SteamFields::MaximumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setOptimumPartLoadRatio(boost::optional<double> optimumPartLoadRatio) {
    bool result(false);
    if (optimumPartLoadRatio) {
      result = setDouble(OS_Boiler_SteamFields::OptimumPartLoadRatio, optimumPartLoadRatio.get());
    }
    else {
      resetOptimumPartLoadRatio();
      result = true;
    }
    return result;
  }

  bool BoilerSteam_Impl::setOptimumPartLoadRatio(const OSOptionalQuantity& optimumPartLoadRatio) {
    bool result(false);
    OptionalDouble value;
    if (optimumPartLoadRatio.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::OptimumPartLoadRatio,optimumPartLoadRatio.get());
      if (value) {
        result = setOptimumPartLoadRatio(value);
      }
    }
    else {
      result = setOptimumPartLoadRatio(value);
    }
    return result;
  }

  void BoilerSteam_Impl::resetOptimumPartLoadRatio() {
    bool result = setString(OS_Boiler_SteamFields::OptimumPartLoadRatio, "");
    OS_ASSERT(result);
  }

  void BoilerSteam_Impl::setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
    bool result(false);
    if (coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
      result = setDouble(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, coefficient1ofFuelUseFunctionofPartLoadRatioCurve.get());
    }
    else {
      resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
    bool result(false);
    OptionalDouble value;
    if (coefficient1ofFuelUseFunctionofPartLoadRatioCurve.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve,coefficient1ofFuelUseFunctionofPartLoadRatioCurve.get());
      if (value) {
        setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(value);

        result = true;
      }
    }
    else {
      setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(value);
      result = true;
    }
    return result;
  }

  void BoilerSteam_Impl::resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, "");
    OS_ASSERT(result);
  }

  void BoilerSteam_Impl::setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
    bool result(false);
    if (coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
      result = setDouble(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, coefficient2ofFuelUseFunctionofPartLoadRatioCurve.get());
    }
    else {
      resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
    bool result(false);
    OptionalDouble value;
    if (coefficient2ofFuelUseFunctionofPartLoadRatioCurve.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve,coefficient2ofFuelUseFunctionofPartLoadRatioCurve.get());
      if (value) {
        setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(value);

        result = true;
      }
    }
    else {
      setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(value);
      result = true;
    }
    return result;
  }

  void BoilerSteam_Impl::resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, "");
    OS_ASSERT(result);
  }

  void BoilerSteam_Impl::setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
    bool result(false);
    if (coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
      result = setDouble(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, coefficient3ofFuelUseFunctionofPartLoadRatioCurve.get());
    }
    else {
      resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(const OSOptionalQuantity& coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
    bool result(false);
    OptionalDouble value;
    if (coefficient3ofFuelUseFunctionofPartLoadRatioCurve.isSet()) {
      value = getDoubleFromQuantity(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve,coefficient3ofFuelUseFunctionofPartLoadRatioCurve.get());
      if (value) {
        setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(value);

        result = true;
      }
    }
    else {
      setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(value);
      result = true;
    }
    return result;
  }

  void BoilerSteam_Impl::resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve() {
    bool result = setString(OS_Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, "");
    OS_ASSERT(result);
  }

  bool BoilerSteam_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_Boiler_SteamFields::SizingFactor, sizingFactor);
    return result;
  }

  bool BoilerSteam_Impl::setSizingFactor(const Quantity& sizingFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_Boiler_SteamFields::SizingFactor,sizingFactor);
    if (!value) {
      return false;
    }
    return setSizingFactor(value.get());
  }

  void BoilerSteam_Impl::resetSizingFactor() {
    bool result = setString(OS_Boiler_SteamFields::SizingFactor, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> BoilerSteam_Impl::fuelTypeValues() const {
    return BoilerSteam::fuelTypeValues();
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::maximumOperatingPressure_SI() const {
    return getMaximumOperatingPressure(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::maximumOperatingPressure_IP() const {
    return getMaximumOperatingPressure(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::theoreticalEfficiency_SI() const {
    return getTheoreticalEfficiency(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::theoreticalEfficiency_IP() const {
    return getTheoreticalEfficiency(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::designOutletSteamTemperature_SI() const {
    return getDesignOutletSteamTemperature(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::designOutletSteamTemperature_IP() const {
    return getDesignOutletSteamTemperature(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::nominalCapacity_SI() const {
    return getNominalCapacity(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::nominalCapacity_IP() const {
    return getNominalCapacity(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::minimumPartLoadRatio_SI() const {
    return getMinimumPartLoadRatio(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::minimumPartLoadRatio_IP() const {
    return getMinimumPartLoadRatio(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::maximumPartLoadRatio_SI() const {
    return getMaximumPartLoadRatio(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::maximumPartLoadRatio_IP() const {
    return getMaximumPartLoadRatio(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::optimumPartLoadRatio_SI() const {
    return getOptimumPartLoadRatio(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::optimumPartLoadRatio_IP() const {
    return getOptimumPartLoadRatio(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::coefficient1ofFuelUseFunctionofPartLoadRatioCurve_SI() const {
    return getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::coefficient1ofFuelUseFunctionofPartLoadRatioCurve_IP() const {
    return getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::coefficient2ofFuelUseFunctionofPartLoadRatioCurve_SI() const {
    return getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::coefficient2ofFuelUseFunctionofPartLoadRatioCurve_IP() const {
    return getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(true);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::coefficient3ofFuelUseFunctionofPartLoadRatioCurve_SI() const {
    return getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(false);
  }

  openstudio::OSOptionalQuantity BoilerSteam_Impl::coefficient3ofFuelUseFunctionofPartLoadRatioCurve_IP() const {
    return getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(true);
  }

  openstudio::Quantity BoilerSteam_Impl::sizingFactor_SI() const {
    return getSizingFactor(false);
  }

  openstudio::Quantity BoilerSteam_Impl::sizingFactor_IP() const {
    return getSizingFactor(true);
  }

  unsigned BoilerSteam_Impl::inletPort()
  {
    return OS_Boiler_SteamFields::WaterInletNodeName;
  }

  unsigned BoilerSteam_Impl::outletPort()
  {
    return OS_Boiler_SteamFields::SteamOutletNodeName;
  }

  bool BoilerSteam_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->supplyComponent(node.handle()) )
      {
        if( StraightComponent_Impl::addToNode(node) )
        {
          plant->setFluidType("Steam");
          return true;
        }
      }
    }

    return false;
  }

} // detail

BoilerSteam::BoilerSteam(const Model& model)
  : StraightComponent(BoilerSteam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::BoilerSteam_Impl>());

  setFuelType("NaturalGas");
  setMaximumOperatingPressure(160000);
  setTheoreticalEfficiency(0.8);
  setDesignOutletSteamTemperature(115);
  autosizeNominalCapacity();
  setMinimumPartLoadRatio(0.00001);
  setMaximumPartLoadRatio(1.0);
  setOptimumPartLoadRatio(0.2);
  setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(0.8);
  setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(0.1);
  setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(0.1);
  setSizingFactor(1.0);
}

IddObjectType BoilerSteam::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Boiler_Steam);
}

std::vector<std::string> BoilerSteam::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Boiler_SteamFields::FuelType);
}

std::string BoilerSteam::fuelType() const {
  return getImpl<detail::BoilerSteam_Impl>()->fuelType();
}

boost::optional<double> BoilerSteam::maximumOperatingPressure() const {
  return getImpl<detail::BoilerSteam_Impl>()->maximumOperatingPressure();
}

OSOptionalQuantity BoilerSteam::getMaximumOperatingPressure(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getMaximumOperatingPressure(returnIP);
}

boost::optional<double> BoilerSteam::theoreticalEfficiency() const {
  return getImpl<detail::BoilerSteam_Impl>()->theoreticalEfficiency();
}

OSOptionalQuantity BoilerSteam::getTheoreticalEfficiency(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getTheoreticalEfficiency(returnIP);
}

boost::optional<double> BoilerSteam::designOutletSteamTemperature() const {
  return getImpl<detail::BoilerSteam_Impl>()->designOutletSteamTemperature();
}

OSOptionalQuantity BoilerSteam::getDesignOutletSteamTemperature(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getDesignOutletSteamTemperature(returnIP);
}

boost::optional<double> BoilerSteam::nominalCapacity() const {
  return getImpl<detail::BoilerSteam_Impl>()->nominalCapacity();
}

OSOptionalQuantity BoilerSteam::getNominalCapacity(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getNominalCapacity(returnIP);
}

bool BoilerSteam::isNominalCapacityAutosized() const {
  return getImpl<detail::BoilerSteam_Impl>()->isNominalCapacityAutosized();
}

boost::optional<double> BoilerSteam::minimumPartLoadRatio() const {
  return getImpl<detail::BoilerSteam_Impl>()->minimumPartLoadRatio();
}

OSOptionalQuantity BoilerSteam::getMinimumPartLoadRatio(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getMinimumPartLoadRatio(returnIP);
}

boost::optional<double> BoilerSteam::maximumPartLoadRatio() const {
  return getImpl<detail::BoilerSteam_Impl>()->maximumPartLoadRatio();
}

OSOptionalQuantity BoilerSteam::getMaximumPartLoadRatio(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getMaximumPartLoadRatio(returnIP);
}

boost::optional<double> BoilerSteam::optimumPartLoadRatio() const {
  return getImpl<detail::BoilerSteam_Impl>()->optimumPartLoadRatio();
}

OSOptionalQuantity BoilerSteam::getOptimumPartLoadRatio(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getOptimumPartLoadRatio(returnIP);
}

boost::optional<double> BoilerSteam::coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::BoilerSteam_Impl>()->coefficient1ofFuelUseFunctionofPartLoadRatioCurve();
}

OSOptionalQuantity BoilerSteam::getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(returnIP);
}

boost::optional<double> BoilerSteam::coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::BoilerSteam_Impl>()->coefficient2ofFuelUseFunctionofPartLoadRatioCurve();
}

OSOptionalQuantity BoilerSteam::getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(returnIP);
}

boost::optional<double> BoilerSteam::coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const {
  return getImpl<detail::BoilerSteam_Impl>()->coefficient3ofFuelUseFunctionofPartLoadRatioCurve();
}

OSOptionalQuantity BoilerSteam::getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(returnIP);
}

double BoilerSteam::sizingFactor() const {
  return getImpl<detail::BoilerSteam_Impl>()->sizingFactor();
}

Quantity BoilerSteam::getSizingFactor(bool returnIP) const {
  return getImpl<detail::BoilerSteam_Impl>()->getSizingFactor(returnIP);
}

bool BoilerSteam::isSizingFactorDefaulted() const {
  return getImpl<detail::BoilerSteam_Impl>()->isSizingFactorDefaulted();
}

bool BoilerSteam::setFuelType(std::string fuelType) {
  return getImpl<detail::BoilerSteam_Impl>()->setFuelType(fuelType);
}

void BoilerSteam::setMaximumOperatingPressure(double maximumOperatingPressure) {
  getImpl<detail::BoilerSteam_Impl>()->setMaximumOperatingPressure(maximumOperatingPressure);
}

bool BoilerSteam::setMaximumOperatingPressure(const Quantity& maximumOperatingPressure) {
  return getImpl<detail::BoilerSteam_Impl>()->setMaximumOperatingPressure(maximumOperatingPressure);
}

void BoilerSteam::resetMaximumOperatingPressure() {
  getImpl<detail::BoilerSteam_Impl>()->resetMaximumOperatingPressure();
}

bool BoilerSteam::setTheoreticalEfficiency(double theoreticalEfficiency) {
  return getImpl<detail::BoilerSteam_Impl>()->setTheoreticalEfficiency(theoreticalEfficiency);
}

bool BoilerSteam::setTheoreticalEfficiency(const Quantity& theoreticalEfficiency) {
  return getImpl<detail::BoilerSteam_Impl>()->setTheoreticalEfficiency(theoreticalEfficiency);
}

void BoilerSteam::resetTheoreticalEfficiency() {
  getImpl<detail::BoilerSteam_Impl>()->resetTheoreticalEfficiency();
}

void BoilerSteam::setDesignOutletSteamTemperature(double designOutletSteamTemperature) {
  getImpl<detail::BoilerSteam_Impl>()->setDesignOutletSteamTemperature(designOutletSteamTemperature);
}

bool BoilerSteam::setDesignOutletSteamTemperature(const Quantity& designOutletSteamTemperature) {
  return getImpl<detail::BoilerSteam_Impl>()->setDesignOutletSteamTemperature(designOutletSteamTemperature);
}

void BoilerSteam::resetDesignOutletSteamTemperature() {
  getImpl<detail::BoilerSteam_Impl>()->resetDesignOutletSteamTemperature();
}

void BoilerSteam::setNominalCapacity(double nominalCapacity) {
  getImpl<detail::BoilerSteam_Impl>()->setNominalCapacity(nominalCapacity);
}

bool BoilerSteam::setNominalCapacity(const Quantity& nominalCapacity) {
  return getImpl<detail::BoilerSteam_Impl>()->setNominalCapacity(nominalCapacity);
}

void BoilerSteam::resetNominalCapacity() {
  getImpl<detail::BoilerSteam_Impl>()->resetNominalCapacity();
}

void BoilerSteam::autosizeNominalCapacity() {
  getImpl<detail::BoilerSteam_Impl>()->autosizeNominalCapacity();
}

bool BoilerSteam::setMinimumPartLoadRatio(double minimumPartLoadRatio) {
  return getImpl<detail::BoilerSteam_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

bool BoilerSteam::setMinimumPartLoadRatio(const Quantity& minimumPartLoadRatio) {
  return getImpl<detail::BoilerSteam_Impl>()->setMinimumPartLoadRatio(minimumPartLoadRatio);
}

void BoilerSteam::resetMinimumPartLoadRatio() {
  getImpl<detail::BoilerSteam_Impl>()->resetMinimumPartLoadRatio();
}

bool BoilerSteam::setMaximumPartLoadRatio(double maximumPartLoadRatio) {
  return getImpl<detail::BoilerSteam_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

bool BoilerSteam::setMaximumPartLoadRatio(const Quantity& maximumPartLoadRatio) {
  return getImpl<detail::BoilerSteam_Impl>()->setMaximumPartLoadRatio(maximumPartLoadRatio);
}

void BoilerSteam::resetMaximumPartLoadRatio() {
  getImpl<detail::BoilerSteam_Impl>()->resetMaximumPartLoadRatio();
}

bool BoilerSteam::setOptimumPartLoadRatio(double optimumPartLoadRatio) {
  return getImpl<detail::BoilerSteam_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
}

bool BoilerSteam::setOptimumPartLoadRatio(const Quantity& optimumPartLoadRatio) {
  return getImpl<detail::BoilerSteam_Impl>()->setOptimumPartLoadRatio(optimumPartLoadRatio);
}

void BoilerSteam::resetOptimumPartLoadRatio() {
  getImpl<detail::BoilerSteam_Impl>()->resetOptimumPartLoadRatio();
}

void BoilerSteam::setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(double coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
  getImpl<detail::BoilerSteam_Impl>()->setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
}

bool BoilerSteam::setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient1ofFuelUseFunctionofPartLoadRatioCurve) {
  return getImpl<detail::BoilerSteam_Impl>()->setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
}

void BoilerSteam::resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve() {
  getImpl<detail::BoilerSteam_Impl>()->resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();
}

void BoilerSteam::setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(double coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
  getImpl<detail::BoilerSteam_Impl>()->setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
}

bool BoilerSteam::setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient2ofFuelUseFunctionofPartLoadRatioCurve) {
  return getImpl<detail::BoilerSteam_Impl>()->setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
}

void BoilerSteam::resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve() {
  getImpl<detail::BoilerSteam_Impl>()->resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();
}

void BoilerSteam::setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(double coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
  getImpl<detail::BoilerSteam_Impl>()->setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
}

bool BoilerSteam::setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient3ofFuelUseFunctionofPartLoadRatioCurve) {
  return getImpl<detail::BoilerSteam_Impl>()->setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
}

void BoilerSteam::resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve() {
  getImpl<detail::BoilerSteam_Impl>()->resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();
}

bool BoilerSteam::setSizingFactor(double sizingFactor) {
  return getImpl<detail::BoilerSteam_Impl>()->setSizingFactor(sizingFactor);
}

bool BoilerSteam::setSizingFactor(const Quantity& sizingFactor) {
  return getImpl<detail::BoilerSteam_Impl>()->setSizingFactor(sizingFactor);
}

void BoilerSteam::resetSizingFactor() {
  getImpl<detail::BoilerSteam_Impl>()->resetSizingFactor();
}

/// @cond
BoilerSteam::BoilerSteam(std::shared_ptr<detail::BoilerSteam_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

