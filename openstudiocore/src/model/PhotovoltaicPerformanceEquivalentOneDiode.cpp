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

#include "PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "PhotovoltaicPerformanceEquivalentOneDiode_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PhotovoltaicPerformance_EquivalentOneDiode_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PhotovoltaicPerformanceEquivalentOneDiode_Impl::PhotovoltaicPerformanceEquivalentOneDiode_Impl(const IdfObject& idfObject,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : PhotovoltaicPerformance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType());
  }

  PhotovoltaicPerformanceEquivalentOneDiode_Impl::PhotovoltaicPerformanceEquivalentOneDiode_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : PhotovoltaicPerformance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType());
  }

  PhotovoltaicPerformanceEquivalentOneDiode_Impl::PhotovoltaicPerformanceEquivalentOneDiode_Impl(const PhotovoltaicPerformanceEquivalentOneDiode_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : PhotovoltaicPerformance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PhotovoltaicPerformanceEquivalentOneDiode_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PhotovoltaicPerformanceEquivalentOneDiode_Impl::iddObjectType() const {
    return PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType();
  }

  std::string PhotovoltaicPerformanceEquivalentOneDiode_Impl::celltype() const {
    boost::optional<std::string> value = getString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isCelltypeDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype);
  }

  int PhotovoltaicPerformanceEquivalentOneDiode_Impl::numberOfCellsInSeries() const {
    boost::optional<int> value = getInt(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NumberofCellsinSeries,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isNumberOfCellsInSeriesDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NumberofCellsinSeries);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::activeArea() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ActiveArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isActiveAreaDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ActiveArea);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::transmittanceAbsorptanceProduct() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TransmittanceAbsorptanceProduct,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isTransmittanceAbsorptanceProductDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TransmittanceAbsorptanceProduct);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::semiconductorBandgap() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::SemiconductorBandgap,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isSemiconductorBandgapDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::SemiconductorBandgap);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::shuntResistance() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShuntResistance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isShuntResistanceDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShuntResistance);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::shortCircuitCurrent() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShortCircuitCurrent,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isShortCircuitCurrentDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShortCircuitCurrent);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::openCircuitVoltage() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::OpenCircuitVoltage,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isOpenCircuitVoltageDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::OpenCircuitVoltage);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::referenceTemperature() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isReferenceTemperatureDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceTemperature);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::referenceInsolation() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceInsolation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isReferenceInsolationDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceInsolation);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::moduleCurrentatMaximumPower() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleCurrentatMaximumPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isModuleCurrentatMaximumPowerDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleCurrentatMaximumPower);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::moduleVoltageatMaximumPower() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleVoltageatMaximumPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isModuleVoltageatMaximumPowerDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleVoltageatMaximumPower);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::temperatureCoefficientofShortCircuitCurrent() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofShortCircuitCurrent,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isTemperatureCoefficientofShortCircuitCurrentDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofShortCircuitCurrent);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::temperatureCoefficientofOpenCircuitVoltage() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofOpenCircuitVoltage,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isTemperatureCoefficientofOpenCircuitVoltageDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofOpenCircuitVoltage);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::nominalOperatingCellTemperatureTestAmbientTemperature() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestAmbientTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isNominalOperatingCellTemperatureTestAmbientTemperatureDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestAmbientTemperature);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::nominalOperatingCellTemperatureTestCellTemperature() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestCellTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isNominalOperatingCellTemperatureTestCellTemperatureDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestCellTemperature);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::nominalOperatingCellTemperatureTestInsolation() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestInsolation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isNominalOperatingCellTemperatureTestInsolationDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestInsolation);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::moduleHeatLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleHeatLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isModuleHeatLossCoefficientDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleHeatLossCoefficient);
  }

  double PhotovoltaicPerformanceEquivalentOneDiode_Impl::totalHeatCapacity() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TotalHeatCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::isTotalHeatCapacityDefaulted() const {
    return isEmpty(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TotalHeatCapacity);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setCelltype(std::string celltype) {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype, celltype);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetCelltype() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setNumberofCellsinSeries(int numberOfCellsInSeries) {
    bool result = setInt(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NumberofCellsinSeries, numberOfCellsInSeries);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetNumberofCellsinSeries() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NumberofCellsinSeries, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setActiveArea(double activeArea) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ActiveArea, activeArea);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetActiveArea() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ActiveArea, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setTransmittanceAbsorptanceProduct(double transmittanceAbsorptanceProduct) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TransmittanceAbsorptanceProduct, transmittanceAbsorptanceProduct);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetTransmittanceAbsorptanceProduct() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TransmittanceAbsorptanceProduct, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setSemiconductorBandgap(double semiconductorBandgap) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::SemiconductorBandgap, semiconductorBandgap);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetSemiconductorBandgap() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::SemiconductorBandgap, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setShuntResistance(double shuntResistance) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShuntResistance, shuntResistance);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetShuntResistance() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShuntResistance, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setShortCircuitCurrent(double shortCircuitCurrent) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShortCircuitCurrent, shortCircuitCurrent);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetShortCircuitCurrent() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ShortCircuitCurrent, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setOpenCircuitVoltage(double openCircuitVoltage) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::OpenCircuitVoltage, openCircuitVoltage);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetOpenCircuitVoltage() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::OpenCircuitVoltage, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setReferenceTemperature(double referenceTemperature) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceTemperature, referenceTemperature);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetReferenceTemperature() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceTemperature, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setReferenceInsolation(double referenceInsolation) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceInsolation, referenceInsolation);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetReferenceInsolation() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceInsolation, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setModuleCurrentatMaximumPower(double moduleCurrentatMaximumPower) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleCurrentatMaximumPower, moduleCurrentatMaximumPower);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetModuleCurrentatMaximumPower() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleCurrentatMaximumPower, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setModuleVoltageatMaximumPower(double moduleVoltageatMaximumPower) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleVoltageatMaximumPower, moduleVoltageatMaximumPower);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetModuleVoltageatMaximumPower() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleVoltageatMaximumPower, "");
    OS_ASSERT(result);
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::setTemperatureCoefficientofShortCircuitCurrent(double temperatureCoefficientofShortCircuitCurrent) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofShortCircuitCurrent, temperatureCoefficientofShortCircuitCurrent);
    OS_ASSERT(result);
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetTemperatureCoefficientofShortCircuitCurrent() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofShortCircuitCurrent, "");
    OS_ASSERT(result);
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::setTemperatureCoefficientofOpenCircuitVoltage(double temperatureCoefficientofOpenCircuitVoltage) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofOpenCircuitVoltage, temperatureCoefficientofOpenCircuitVoltage);
    OS_ASSERT(result);
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetTemperatureCoefficientofOpenCircuitVoltage() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofOpenCircuitVoltage, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setNominalOperatingCellTemperatureTestAmbientTemperature(double nominalOperatingCellTemperatureTestAmbientTemperature) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestAmbientTemperature, nominalOperatingCellTemperatureTestAmbientTemperature);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetNominalOperatingCellTemperatureTestAmbientTemperature() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestAmbientTemperature, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setNominalOperatingCellTemperatureTestCellTemperature(double nominalOperatingCellTemperatureTestCellTemperature) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestCellTemperature, nominalOperatingCellTemperatureTestCellTemperature);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetNominalOperatingCellTemperatureTestCellTemperature() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestCellTemperature, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setNominalOperatingCellTemperatureTestInsolation(double nominalOperatingCellTemperatureTestInsolation) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestInsolation, nominalOperatingCellTemperatureTestInsolation);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetNominalOperatingCellTemperatureTestInsolation() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestInsolation, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setModuleHeatLossCoefficient(double moduleHeatLossCoefficient) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleHeatLossCoefficient, moduleHeatLossCoefficient);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetModuleHeatLossCoefficient() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleHeatLossCoefficient, "");
    OS_ASSERT(result);
  }

  bool PhotovoltaicPerformanceEquivalentOneDiode_Impl::setTotalHeatCapacity(double totalHeatCapacity) {
    bool result = setDouble(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TotalHeatCapacity, totalHeatCapacity);
    return result;
  }

  void PhotovoltaicPerformanceEquivalentOneDiode_Impl::resetTotalHeatCapacity() {
    bool result = setString(OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::TotalHeatCapacity, "");
    OS_ASSERT(result);
  }

} // detail

PhotovoltaicPerformanceEquivalentOneDiode::PhotovoltaicPerformanceEquivalentOneDiode(const Model& model)
  : PhotovoltaicPerformance(PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType PhotovoltaicPerformanceEquivalentOneDiode::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PhotovoltaicPerformance_EquivalentOneDiode);
}

std::vector<std::string> PhotovoltaicPerformanceEquivalentOneDiode::celltypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype);
}

std::string PhotovoltaicPerformanceEquivalentOneDiode::celltype() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->celltype();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isCelltypeDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isCelltypeDefaulted();
}

int PhotovoltaicPerformanceEquivalentOneDiode::numberOfCellsInSeries() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->numberOfCellsInSeries();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isNumberOfCellsInSeriesDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isNumberOfCellsInSeriesDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::activeArea() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->activeArea();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isActiveAreaDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isActiveAreaDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::transmittanceAbsorptanceProduct() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->transmittanceAbsorptanceProduct();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isTransmittanceAbsorptanceProductDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isTransmittanceAbsorptanceProductDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::semiconductorBandgap() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->semiconductorBandgap();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isSemiconductorBandgapDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isSemiconductorBandgapDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::shuntResistance() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->shuntResistance();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isShuntResistanceDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isShuntResistanceDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::shortCircuitCurrent() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->shortCircuitCurrent();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isShortCircuitCurrentDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isShortCircuitCurrentDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::openCircuitVoltage() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->openCircuitVoltage();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isOpenCircuitVoltageDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isOpenCircuitVoltageDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::referenceTemperature() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->referenceTemperature();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isReferenceTemperatureDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isReferenceTemperatureDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::referenceInsolation() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->referenceInsolation();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isReferenceInsolationDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isReferenceInsolationDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::moduleCurrentatMaximumPower() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->moduleCurrentatMaximumPower();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isModuleCurrentatMaximumPowerDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isModuleCurrentatMaximumPowerDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::moduleVoltageatMaximumPower() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->moduleVoltageatMaximumPower();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isModuleVoltageatMaximumPowerDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isModuleVoltageatMaximumPowerDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::temperatureCoefficientofShortCircuitCurrent() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->temperatureCoefficientofShortCircuitCurrent();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isTemperatureCoefficientofShortCircuitCurrentDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isTemperatureCoefficientofShortCircuitCurrentDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::temperatureCoefficientofOpenCircuitVoltage() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->temperatureCoefficientofOpenCircuitVoltage();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isTemperatureCoefficientofOpenCircuitVoltageDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isTemperatureCoefficientofOpenCircuitVoltageDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::nominalOperatingCellTemperatureTestAmbientTemperature() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->nominalOperatingCellTemperatureTestAmbientTemperature();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isNominalOperatingCellTemperatureTestAmbientTemperatureDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isNominalOperatingCellTemperatureTestAmbientTemperatureDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::nominalOperatingCellTemperatureTestCellTemperature() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->nominalOperatingCellTemperatureTestCellTemperature();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isNominalOperatingCellTemperatureTestCellTemperatureDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isNominalOperatingCellTemperatureTestCellTemperatureDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::nominalOperatingCellTemperatureTestInsolation() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->nominalOperatingCellTemperatureTestInsolation();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isNominalOperatingCellTemperatureTestInsolationDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isNominalOperatingCellTemperatureTestInsolationDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::moduleHeatLossCoefficient() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->moduleHeatLossCoefficient();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isModuleHeatLossCoefficientDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isModuleHeatLossCoefficientDefaulted();
}

double PhotovoltaicPerformanceEquivalentOneDiode::totalHeatCapacity() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->totalHeatCapacity();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::isTotalHeatCapacityDefaulted() const {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->isTotalHeatCapacityDefaulted();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setCelltype(std::string celltype) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setCelltype(celltype);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetCelltype() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetCelltype();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setNumberofCellsinSeries(int numberOfCellsInSeries) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setNumberofCellsinSeries(numberOfCellsInSeries);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetNumberofCellsinSeries() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetNumberofCellsinSeries();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setActiveArea(double activeArea) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setActiveArea(activeArea);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetActiveArea() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetActiveArea();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setTransmittanceAbsorptanceProduct(double transmittanceAbsorptanceProduct) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setTransmittanceAbsorptanceProduct(transmittanceAbsorptanceProduct);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetTransmittanceAbsorptanceProduct() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetTransmittanceAbsorptanceProduct();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setSemiconductorBandgap(double semiconductorBandgap) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setSemiconductorBandgap(semiconductorBandgap);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetSemiconductorBandgap() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetSemiconductorBandgap();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setShuntResistance(double shuntResistance) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setShuntResistance(shuntResistance);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetShuntResistance() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetShuntResistance();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setShortCircuitCurrent(double shortCircuitCurrent) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setShortCircuitCurrent(shortCircuitCurrent);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetShortCircuitCurrent() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetShortCircuitCurrent();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setOpenCircuitVoltage(double openCircuitVoltage) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setOpenCircuitVoltage(openCircuitVoltage);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetOpenCircuitVoltage() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetOpenCircuitVoltage();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setReferenceTemperature(double referenceTemperature) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setReferenceTemperature(referenceTemperature);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetReferenceTemperature() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetReferenceTemperature();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setReferenceInsolation(double referenceInsolation) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setReferenceInsolation(referenceInsolation);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetReferenceInsolation() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetReferenceInsolation();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setModuleCurrentatMaximumPower(double moduleCurrentatMaximumPower) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setModuleCurrentatMaximumPower(moduleCurrentatMaximumPower);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetModuleCurrentatMaximumPower() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetModuleCurrentatMaximumPower();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setModuleVoltageatMaximumPower(double moduleVoltageatMaximumPower) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setModuleVoltageatMaximumPower(moduleVoltageatMaximumPower);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetModuleVoltageatMaximumPower() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetModuleVoltageatMaximumPower();
}

void PhotovoltaicPerformanceEquivalentOneDiode::setTemperatureCoefficientofShortCircuitCurrent(double temperatureCoefficientofShortCircuitCurrent) {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setTemperatureCoefficientofShortCircuitCurrent(temperatureCoefficientofShortCircuitCurrent);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetTemperatureCoefficientofShortCircuitCurrent() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetTemperatureCoefficientofShortCircuitCurrent();
}

void PhotovoltaicPerformanceEquivalentOneDiode::setTemperatureCoefficientofOpenCircuitVoltage(double temperatureCoefficientofOpenCircuitVoltage) {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setTemperatureCoefficientofOpenCircuitVoltage(temperatureCoefficientofOpenCircuitVoltage);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetTemperatureCoefficientofOpenCircuitVoltage() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetTemperatureCoefficientofOpenCircuitVoltage();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setNominalOperatingCellTemperatureTestAmbientTemperature(double nominalOperatingCellTemperatureTestAmbientTemperature) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setNominalOperatingCellTemperatureTestAmbientTemperature(nominalOperatingCellTemperatureTestAmbientTemperature);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetNominalOperatingCellTemperatureTestAmbientTemperature() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetNominalOperatingCellTemperatureTestAmbientTemperature();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setNominalOperatingCellTemperatureTestCellTemperature(double nominalOperatingCellTemperatureTestCellTemperature) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setNominalOperatingCellTemperatureTestCellTemperature(nominalOperatingCellTemperatureTestCellTemperature);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetNominalOperatingCellTemperatureTestCellTemperature() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetNominalOperatingCellTemperatureTestCellTemperature();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setNominalOperatingCellTemperatureTestInsolation(double nominalOperatingCellTemperatureTestInsolation) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setNominalOperatingCellTemperatureTestInsolation(nominalOperatingCellTemperatureTestInsolation);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetNominalOperatingCellTemperatureTestInsolation() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetNominalOperatingCellTemperatureTestInsolation();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setModuleHeatLossCoefficient(double moduleHeatLossCoefficient) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setModuleHeatLossCoefficient(moduleHeatLossCoefficient);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetModuleHeatLossCoefficient() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetModuleHeatLossCoefficient();
}

bool PhotovoltaicPerformanceEquivalentOneDiode::setTotalHeatCapacity(double totalHeatCapacity) {
  return getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->setTotalHeatCapacity(totalHeatCapacity);
}

void PhotovoltaicPerformanceEquivalentOneDiode::resetTotalHeatCapacity() {
  getImpl<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl>()->resetTotalHeatCapacity();
}

/// @cond
PhotovoltaicPerformanceEquivalentOneDiode::PhotovoltaicPerformanceEquivalentOneDiode(std::shared_ptr<detail::PhotovoltaicPerformanceEquivalentOneDiode_Impl> impl)
  : PhotovoltaicPerformance(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

