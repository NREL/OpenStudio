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

#include "RefrigerationSecondarySystem.hpp"
#include "RefrigerationSecondarySystem_Impl.hpp"

#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "RefrigerationAirChiller.hpp"
#include "RefrigerationAirChiller_Impl.hpp"
#include "RefrigerationCase.hpp"
#include "RefrigerationCase_Impl.hpp"
#include "RefrigerationWalkIn.hpp"
#include "RefrigerationWalkIn_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_SecondarySystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationSecondarySystem_Impl::RefrigerationSecondarySystem_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationSecondarySystem::iddObjectType());
  }

  RefrigerationSecondarySystem_Impl::RefrigerationSecondarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationSecondarySystem::iddObjectType());
  }

  RefrigerationSecondarySystem_Impl::RefrigerationSecondarySystem_Impl(const RefrigerationSecondarySystem_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationSecondarySystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationSecondarySystem_Impl::iddObjectType() const {
    return RefrigerationSecondarySystem::iddObjectType();
  }

  std::vector<IdfObject> RefrigerationSecondarySystem_Impl::remove()
  {
    std::vector<IdfObject> result;

    if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
      std::vector<IdfObject> removedCasesAndWalkins = caseAndWalkinList->remove();
      result.insert(result.end(), removedCasesAndWalkins.begin(), removedCasesAndWalkins.end());
    }

    std::vector<IdfObject> removedRefrigerationSecondarySystem = ParentObject_Impl::remove();
    result.insert(result.end(), removedRefrigerationSecondarySystem.begin(), removedRefrigerationSecondarySystem.end());

    return result;
  }

  ModelObject RefrigerationSecondarySystem_Impl::clone(Model model) const
  {
    RefrigerationSecondarySystem modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationSecondarySystem>();

    if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
      ModelObjectList caseAndWalkinListClone = caseAndWalkinList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationSecondarySystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinListClone);
    }

    modelObjectClone.resetDistributionPipingZone();
    modelObjectClone.resetReceiverSeparatorZone();

    return modelObjectClone;
  }

  std::vector<IddObjectType> RefrigerationSecondarySystem_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Cubic);
    return result;
  }

  std::vector<ModelObject> RefrigerationSecondarySystem_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<CurveCubic> intermediate = variableSpeedPumpCubicCurve()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  template <class T>
  std::vector<T> RefrigerationSecondarySystem_Impl::listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const {
    std::vector<T> result;

    if( modelObjectList ) {
      std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          boost::optional<T> modelObject = elem.optionalCast<T>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }

    return result;
  }

  std::vector<RefrigerationCase> RefrigerationSecondarySystem_Impl::cases() const {
    return RefrigerationSecondarySystem_Impl::listTemplate<RefrigerationCase>(refrigeratedCaseAndWalkInList());
  }

  std::vector<RefrigerationWalkIn> RefrigerationSecondarySystem_Impl::walkins() const {
    return RefrigerationSecondarySystem_Impl::listTemplate<RefrigerationWalkIn>(refrigeratedCaseAndWalkInList());
  }

  std::vector<RefrigerationAirChiller> RefrigerationSecondarySystem_Impl::airChillers() const {
    return RefrigerationSecondarySystem_Impl::listTemplate<RefrigerationAirChiller>(refrigeratedCaseAndWalkInList());
  }

  boost::optional<ModelObjectList> RefrigerationSecondarySystem_Impl::refrigeratedCaseAndWalkInList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SecondarySystemFields::RefrigeratedCaseAndWalkInListName);
  }

  std::string RefrigerationSecondarySystem_Impl::circulatingFluidName() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SecondarySystemFields::CirculatingFluidName,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<int> RefrigerationSecondarySystem_Impl::glycolConcentration() const {
    return getInt(OS_Refrigeration_SecondarySystemFields::GlycolConcentration,true);
  }

  boost::optional<double> RefrigerationSecondarySystem_Impl::evaporatorCapacity() const {
    return getDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorCapacity,true);
  }

  boost::optional<double> RefrigerationSecondarySystem_Impl::evaporatorFlowRateforSecondaryFluid() const {
    return getDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorFlowRateforSecondaryFluid,true);
  }

  double RefrigerationSecondarySystem_Impl::evaporatorEvaporatingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorEvaporatingTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationSecondarySystem_Impl::evaporatorApproachTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorApproachTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> RefrigerationSecondarySystem_Impl::evaporatorRangeTemperatureDifference() const {
    return getDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorRangeTemperatureDifference,true);
  }

  int RefrigerationSecondarySystem_Impl::numberofPumpsinLoop() const {
    boost::optional<int> value = getInt(OS_Refrigeration_SecondarySystemFields::NumberofPumpsinLoop,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isNumberofPumpsinLoopDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::NumberofPumpsinLoop);
  }

  boost::optional<double> RefrigerationSecondarySystem_Impl::totalPumpFlowRate() const {
    return getDouble(OS_Refrigeration_SecondarySystemFields::TotalPumpFlowRate,true);
  }

  boost::optional<double> RefrigerationSecondarySystem_Impl::totalPumpPower() const {
    return getDouble(OS_Refrigeration_SecondarySystemFields::TotalPumpPower,true);
  }

  boost::optional<double> RefrigerationSecondarySystem_Impl::totalPumpHead() const {
    return getDouble(OS_Refrigeration_SecondarySystemFields::TotalPumpHead,true);
  }

  double RefrigerationSecondarySystem_Impl::phaseChangeCirculatingRate() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::PhaseChangeCirculatingRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isPhaseChangeCirculatingRateDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::PhaseChangeCirculatingRate);
  }

  std::string RefrigerationSecondarySystem_Impl::pumpDriveType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SecondarySystemFields::PumpDriveType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isPumpDriveTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::PumpDriveType);
  }

  boost::optional<CurveCubic> RefrigerationSecondarySystem_Impl::variableSpeedPumpCubicCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_Refrigeration_SecondarySystemFields::VariableSpeedPumpCubicCurveName);
  }

  double RefrigerationSecondarySystem_Impl::pumpMotorHeattoFluid() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::PumpMotorHeattoFluid,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isPumpMotorHeattoFluidDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::PumpMotorHeattoFluid);
  }

  double RefrigerationSecondarySystem_Impl::sumUADistributionPiping() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::SumUADistributionPiping,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isSumUADistributionPipingDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::SumUADistributionPiping);
  }

  boost::optional<ThermalZone> RefrigerationSecondarySystem_Impl::distributionPipingZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_SecondarySystemFields::DistributionPipingZoneName);
  }

  double RefrigerationSecondarySystem_Impl::sumUAReceiverSeparatorShell() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::SumUAReceiver_SeparatorShell,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isSumUAReceiverSeparatorShellDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::SumUAReceiver_SeparatorShell);
  }

  boost::optional<ThermalZone> RefrigerationSecondarySystem_Impl::receiverSeparatorZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_SecondarySystemFields::Receiver_SeparatorZoneName);
  }

  double RefrigerationSecondarySystem_Impl::evaporatorRefrigerantInventory() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorRefrigerantInventory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isEvaporatorRefrigerantInventoryDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::EvaporatorRefrigerantInventory);
  }

  std::string RefrigerationSecondarySystem_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SecondarySystemFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSecondarySystem_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_SecondarySystemFields::EndUseSubcategory);
  }

  template <class T>
  void RefrigerationSecondarySystem_Impl::removeAllTemplate( boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          boost::optional<T> modelObject = elem.optionalCast<T>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  template <class T>
  void RefrigerationSecondarySystem_Impl::removeTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      modelObjectList->removeModelObject(modelObject);
    }
  }

  template <class T>
  bool RefrigerationSecondarySystem_Impl::addTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      return modelObjectList->addModelObject(modelObject);
    }
    return false;
  }

  bool RefrigerationSecondarySystem_Impl::addCase( const RefrigerationCase& refrigerationCase) {
    if( boost::optional<RefrigerationSystem> currentSystem = refrigerationCase.system() )
    {
      currentSystem->removeCase(refrigerationCase);
    }
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    return addTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationSecondarySystem_Impl::removeCase( const RefrigerationCase& refrigerationCase) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationSecondarySystem_Impl::removeAllCases() {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeAllTemplate<RefrigerationCase>(modelObjectList);
  }

  bool RefrigerationSecondarySystem_Impl::addWalkin( const RefrigerationWalkIn& refrigerationWalkin) {
    if( boost::optional<RefrigerationSystem> currentSystem = refrigerationWalkin.system() )
    {
      currentSystem->removeWalkin(refrigerationWalkin);
    }
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    return addTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationSecondarySystem_Impl::removeWalkin( const RefrigerationWalkIn& refrigerationWalkin) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationSecondarySystem_Impl::removeAllWalkins() {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeAllTemplate<RefrigerationWalkIn>(modelObjectList);
  }

  bool RefrigerationSecondarySystem_Impl::addAirChiller( const RefrigerationAirChiller& refrigerationAirChiller) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    return addTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
  }

  void RefrigerationSecondarySystem_Impl::removeAirChiller( const RefrigerationAirChiller& refrigerationAirChiller) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
  }

  void RefrigerationSecondarySystem_Impl::removeAllAirChillers() {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeAllTemplate<RefrigerationAirChiller>(modelObjectList);
  }

  bool RefrigerationSecondarySystem_Impl::setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Refrigeration_SecondarySystemFields::RefrigeratedCaseAndWalkInListName, modelObjectList.get().handle());
    }
    else {
      resetRefrigeratedCaseAndWalkInList();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetRefrigeratedCaseAndWalkInList() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::RefrigeratedCaseAndWalkInListName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setCirculatingFluidName(std::string circulatingFluidName) {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::CirculatingFluidName, circulatingFluidName);
    return result;
  }

  bool RefrigerationSecondarySystem_Impl::setGlycolConcentration(boost::optional<int> glycolConcentration) {
    bool result(false);
    if (glycolConcentration) {
      result = setInt(OS_Refrigeration_SecondarySystemFields::GlycolConcentration, glycolConcentration.get());
    }
    else {
      resetGlycolConcentration();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetGlycolConcentration() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::GlycolConcentration, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setEvaporatorCapacity(boost::optional<double> evaporatorCapacity) {
    bool result(false);
    if (evaporatorCapacity) {
      result = setDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorCapacity, evaporatorCapacity.get());
    }
    else {
      resetEvaporatorCapacity();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetEvaporatorCapacity() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::EvaporatorCapacity, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setEvaporatorFlowRateforSecondaryFluid(boost::optional<double> evaporatorFlowRateforSecondaryFluid) {
    bool result(false);
    if (evaporatorFlowRateforSecondaryFluid) {
      result = setDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorFlowRateforSecondaryFluid, evaporatorFlowRateforSecondaryFluid.get());
    }
    else {
      resetEvaporatorFlowRateforSecondaryFluid();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetEvaporatorFlowRateforSecondaryFluid() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::EvaporatorFlowRateforSecondaryFluid, "");
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setEvaporatorEvaporatingTemperature(double evaporatorEvaporatingTemperature) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorEvaporatingTemperature, evaporatorEvaporatingTemperature);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setEvaporatorApproachTemperatureDifference(double evaporatorApproachTemperatureDifference) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorApproachTemperatureDifference, evaporatorApproachTemperatureDifference);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setEvaporatorRangeTemperatureDifference(boost::optional<double> evaporatorRangeTemperatureDifference) {
    bool result(false);
    if (evaporatorRangeTemperatureDifference) {
      result = setDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorRangeTemperatureDifference, evaporatorRangeTemperatureDifference.get());
    }
    else {
      resetEvaporatorRangeTemperatureDifference();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::resetEvaporatorRangeTemperatureDifference() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::EvaporatorRangeTemperatureDifference, "");
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setNumberofPumpsinLoop(int numberofPumpsinLoop) {
    bool result = setInt(OS_Refrigeration_SecondarySystemFields::NumberofPumpsinLoop, numberofPumpsinLoop);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::resetNumberofPumpsinLoop() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::NumberofPumpsinLoop, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setTotalPumpFlowRate(boost::optional<double> totalPumpFlowRate) {
    bool result(false);
    if (totalPumpFlowRate) {
      result = setDouble(OS_Refrigeration_SecondarySystemFields::TotalPumpFlowRate, totalPumpFlowRate.get());
    }
    else {
      resetTotalPumpFlowRate();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetTotalPumpFlowRate() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::TotalPumpFlowRate, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setTotalPumpPower(boost::optional<double> totalPumpPower) {
    bool result(false);
    if (totalPumpPower) {
      result = setDouble(OS_Refrigeration_SecondarySystemFields::TotalPumpPower, totalPumpPower.get());
    }
    else {
      resetTotalPumpPower();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetTotalPumpPower() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::TotalPumpPower, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setTotalPumpHead(boost::optional<double> totalPumpHead) {
    bool result(false);
    if (totalPumpHead) {
      result = setDouble(OS_Refrigeration_SecondarySystemFields::TotalPumpHead, totalPumpHead.get());
    }
    else {
      resetTotalPumpHead();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetTotalPumpHead() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::TotalPumpHead, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setPhaseChangeCirculatingRate(double phaseChangeCirculatingRate) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::PhaseChangeCirculatingRate, phaseChangeCirculatingRate);
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetPhaseChangeCirculatingRate() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::PhaseChangeCirculatingRate, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setPumpDriveType(std::string pumpDriveType) {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::PumpDriveType, pumpDriveType);
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetPumpDriveType() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::PumpDriveType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setVariableSpeedPumpCubicCurve(const boost::optional<CurveCubic>& curveCubic) {
    bool result(false);
    if (curveCubic) {
      result = setPointer(OS_Refrigeration_SecondarySystemFields::VariableSpeedPumpCubicCurveName, curveCubic.get().handle());
    }
    else {
      resetVariableSpeedPumpCubicCurve();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetVariableSpeedPumpCubicCurve() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::VariableSpeedPumpCubicCurveName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setPumpMotorHeattoFluid(double pumpMotorHeattoFluid) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::PumpMotorHeattoFluid, pumpMotorHeattoFluid);
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetPumpMotorHeattoFluid() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::PumpMotorHeattoFluid, "");
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setSumUADistributionPiping(double sumUADistributionPiping) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::SumUADistributionPiping, sumUADistributionPiping);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::resetSumUADistributionPiping() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::SumUADistributionPiping, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setDistributionPipingZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Refrigeration_SecondarySystemFields::DistributionPipingZoneName, thermalZone.get().handle());
    }
    else {
      resetDistributionPipingZone();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetDistributionPipingZone() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::DistributionPipingZoneName, "");
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setSumUAReceiverSeparatorShell(double sumUAReceiverSeparatorShell) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::SumUAReceiver_SeparatorShell, sumUAReceiverSeparatorShell);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::resetSumUAReceiverSeparatorShell() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::SumUAReceiver_SeparatorShell, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSecondarySystem_Impl::setReceiverSeparatorZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Refrigeration_SecondarySystemFields::Receiver_SeparatorZoneName, thermalZone.get().handle());
    }
    else {
      resetReceiverSeparatorZone();
      result = true;
    }
    return result;
  }

  void RefrigerationSecondarySystem_Impl::resetReceiverSeparatorZone() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::Receiver_SeparatorZoneName, "");
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setEvaporatorRefrigerantInventory(double evaporatorRefrigerantInventory) {
    bool result = setDouble(OS_Refrigeration_SecondarySystemFields::EvaporatorRefrigerantInventory, evaporatorRefrigerantInventory);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::resetEvaporatorRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::EvaporatorRefrigerantInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationSecondarySystem_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_SecondarySystemFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationSecondarySystem::RefrigerationSecondarySystem(const Model& model)
  : ParentObject(RefrigerationSecondarySystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationSecondarySystem_Impl>());

  ModelObjectList caseAndWalkinList = ModelObjectList(model);
  caseAndWalkinList.setName(this->name().get() + " Case and Walkin List");
  bool ok = getImpl<detail::RefrigerationSecondarySystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinList);
  OS_ASSERT(ok);
  ok = setCirculatingFluidName("PropyleneGlycol");
  OS_ASSERT(ok);
  ok = setGlycolConcentration(30);
  OS_ASSERT(ok);
  ok = setEvaporatorCapacity(3.5E4);
  OS_ASSERT(ok);
  ok = setEvaporatorFlowRateforSecondaryFluid(0.0021);
  OS_ASSERT(ok);
  setEvaporatorEvaporatingTemperature(-12.6);
  setEvaporatorApproachTemperatureDifference(2.7);
  setEvaporatorRangeTemperatureDifference(4.0);
  setNumberofPumpsinLoop(3);
  ok = setTotalPumpFlowRate(0.0023);
  OS_ASSERT(ok);
  ok = setTotalPumpHead(2.09E5);
  OS_ASSERT(ok);
  ok = setPumpMotorHeattoFluid(1.0);
  OS_ASSERT(ok);
}

IddObjectType RefrigerationSecondarySystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_SecondarySystem);
}

std::vector<std::string> RefrigerationSecondarySystem::circulatingFluidNameValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SecondarySystemFields::CirculatingFluidName);
}

std::vector<std::string> RefrigerationSecondarySystem::pumpDriveTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SecondarySystemFields::PumpDriveType);
}

std::vector<RefrigerationCase> RefrigerationSecondarySystem::cases() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->cases();
}

std::vector<RefrigerationWalkIn> RefrigerationSecondarySystem::walkins() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->walkins();
}

std::vector<RefrigerationAirChiller> RefrigerationSecondarySystem::airChillers() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->airChillers();
}

std::string RefrigerationSecondarySystem::circulatingFluidName() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->circulatingFluidName();
}

boost::optional<int> RefrigerationSecondarySystem::glycolConcentration() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->glycolConcentration();
}

boost::optional<double> RefrigerationSecondarySystem::evaporatorCapacity() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->evaporatorCapacity();
}

boost::optional<double> RefrigerationSecondarySystem::evaporatorFlowRateforSecondaryFluid() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->evaporatorFlowRateforSecondaryFluid();
}

double RefrigerationSecondarySystem::evaporatorEvaporatingTemperature() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->evaporatorEvaporatingTemperature();
}

double RefrigerationSecondarySystem::evaporatorApproachTemperatureDifference() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->evaporatorApproachTemperatureDifference();
}

boost::optional<double> RefrigerationSecondarySystem::evaporatorRangeTemperatureDifference() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->evaporatorRangeTemperatureDifference();
}

int RefrigerationSecondarySystem::numberofPumpsinLoop() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->numberofPumpsinLoop();
}

bool RefrigerationSecondarySystem::isNumberofPumpsinLoopDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isNumberofPumpsinLoopDefaulted();
}

boost::optional<double> RefrigerationSecondarySystem::totalPumpFlowRate() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->totalPumpFlowRate();
}

boost::optional<double> RefrigerationSecondarySystem::totalPumpPower() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->totalPumpPower();
}

boost::optional<double> RefrigerationSecondarySystem::totalPumpHead() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->totalPumpHead();
}

double RefrigerationSecondarySystem::phaseChangeCirculatingRate() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->phaseChangeCirculatingRate();
}

bool RefrigerationSecondarySystem::isPhaseChangeCirculatingRateDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isPhaseChangeCirculatingRateDefaulted();
}

std::string RefrigerationSecondarySystem::pumpDriveType() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->pumpDriveType();
}

bool RefrigerationSecondarySystem::isPumpDriveTypeDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isPumpDriveTypeDefaulted();
}

boost::optional<CurveCubic> RefrigerationSecondarySystem::variableSpeedPumpCubicCurve() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->variableSpeedPumpCubicCurve();
}

double RefrigerationSecondarySystem::pumpMotorHeattoFluid() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->pumpMotorHeattoFluid();
}

bool RefrigerationSecondarySystem::isPumpMotorHeattoFluidDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isPumpMotorHeattoFluidDefaulted();
}

double RefrigerationSecondarySystem::sumUADistributionPiping() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->sumUADistributionPiping();
}

bool RefrigerationSecondarySystem::isSumUADistributionPipingDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isSumUADistributionPipingDefaulted();
}

boost::optional<ThermalZone> RefrigerationSecondarySystem::distributionPipingZone() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->distributionPipingZone();
}

double RefrigerationSecondarySystem::sumUAReceiverSeparatorShell() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->sumUAReceiverSeparatorShell();
}

bool RefrigerationSecondarySystem::isSumUAReceiverSeparatorShellDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isSumUAReceiverSeparatorShellDefaulted();
}

boost::optional<ThermalZone> RefrigerationSecondarySystem::receiverSeparatorZone() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->receiverSeparatorZone();
}

double RefrigerationSecondarySystem::evaporatorRefrigerantInventory() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->evaporatorRefrigerantInventory();
}

bool RefrigerationSecondarySystem::isEvaporatorRefrigerantInventoryDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isEvaporatorRefrigerantInventoryDefaulted();
}

std::string RefrigerationSecondarySystem::endUseSubcategory() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->endUseSubcategory();
}

bool RefrigerationSecondarySystem::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->isEndUseSubcategoryDefaulted();
}

bool RefrigerationSecondarySystem::addCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->addCase(refrigerationCase);
}

void RefrigerationSecondarySystem::removeCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->removeCase(refrigerationCase);
}

void RefrigerationSecondarySystem::removeAllCases() {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->removeAllCases();
}

bool RefrigerationSecondarySystem::addWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->addWalkin(refrigerationWalkin);
}

void RefrigerationSecondarySystem::removeWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->removeWalkin(refrigerationWalkin);
}

void RefrigerationSecondarySystem::removeAllWalkins() {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->removeAllWalkins();
}

bool RefrigerationSecondarySystem::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->addAirChiller(refrigerationAirChiller);
}

void RefrigerationSecondarySystem::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->removeAirChiller(refrigerationAirChiller);
}

void RefrigerationSecondarySystem::removeAllAirChillers() {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->removeAllAirChillers();
}

void RefrigerationSecondarySystem::resetRefrigeratedCaseAndWalkInList() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetRefrigeratedCaseAndWalkInList();
}

bool RefrigerationSecondarySystem::setCirculatingFluidName(std::string circulatingFluidName) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setCirculatingFluidName(circulatingFluidName);
}

bool RefrigerationSecondarySystem::setGlycolConcentration(int glycolConcentration) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setGlycolConcentration(glycolConcentration);
}

void RefrigerationSecondarySystem::resetGlycolConcentration() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetGlycolConcentration();
}

bool RefrigerationSecondarySystem::setEvaporatorCapacity(double evaporatorCapacity) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEvaporatorCapacity(evaporatorCapacity);
}

void RefrigerationSecondarySystem::resetEvaporatorCapacity() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetEvaporatorCapacity();
}

bool RefrigerationSecondarySystem::setEvaporatorFlowRateforSecondaryFluid(double evaporatorFlowRateforSecondaryFluid) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEvaporatorFlowRateforSecondaryFluid(evaporatorFlowRateforSecondaryFluid);
}

void RefrigerationSecondarySystem::resetEvaporatorFlowRateforSecondaryFluid() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetEvaporatorFlowRateforSecondaryFluid();
}

void RefrigerationSecondarySystem::setEvaporatorEvaporatingTemperature(double evaporatorEvaporatingTemperature) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEvaporatorEvaporatingTemperature(evaporatorEvaporatingTemperature);
}

void RefrigerationSecondarySystem::setEvaporatorApproachTemperatureDifference(double evaporatorApproachTemperatureDifference) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEvaporatorApproachTemperatureDifference(evaporatorApproachTemperatureDifference);
}

void RefrigerationSecondarySystem::setEvaporatorRangeTemperatureDifference(double evaporatorRangeTemperatureDifference) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEvaporatorRangeTemperatureDifference(evaporatorRangeTemperatureDifference);
}

void RefrigerationSecondarySystem::resetEvaporatorRangeTemperatureDifference() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetEvaporatorRangeTemperatureDifference();
}

void RefrigerationSecondarySystem::setNumberofPumpsinLoop(int numberofPumpsinLoop) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setNumberofPumpsinLoop(numberofPumpsinLoop);
}

void RefrigerationSecondarySystem::resetNumberofPumpsinLoop() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetNumberofPumpsinLoop();
}

bool RefrigerationSecondarySystem::setTotalPumpFlowRate(double totalPumpFlowRate) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setTotalPumpFlowRate(totalPumpFlowRate);
}

void RefrigerationSecondarySystem::resetTotalPumpFlowRate() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetTotalPumpFlowRate();
}

bool RefrigerationSecondarySystem::setTotalPumpPower(double totalPumpPower) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setTotalPumpPower(totalPumpPower);
}

void RefrigerationSecondarySystem::resetTotalPumpPower() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetTotalPumpPower();
}

bool RefrigerationSecondarySystem::setTotalPumpHead(double totalPumpHead) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setTotalPumpHead(totalPumpHead);
}

void RefrigerationSecondarySystem::resetTotalPumpHead() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetTotalPumpHead();
}

bool RefrigerationSecondarySystem::setPhaseChangeCirculatingRate(double phaseChangeCirculatingRate) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setPhaseChangeCirculatingRate(phaseChangeCirculatingRate);
}

void RefrigerationSecondarySystem::resetPhaseChangeCirculatingRate() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetPhaseChangeCirculatingRate();
}

bool RefrigerationSecondarySystem::setPumpDriveType(std::string pumpDriveType) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setPumpDriveType(pumpDriveType);
}

void RefrigerationSecondarySystem::resetPumpDriveType() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetPumpDriveType();
}

bool RefrigerationSecondarySystem::setVariableSpeedPumpCubicCurve(const CurveCubic& curveCubic) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setVariableSpeedPumpCubicCurve(curveCubic);
}

void RefrigerationSecondarySystem::resetVariableSpeedPumpCubicCurve() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetVariableSpeedPumpCubicCurve();
}

bool RefrigerationSecondarySystem::setPumpMotorHeattoFluid(double pumpMotorHeattoFluid) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setPumpMotorHeattoFluid(pumpMotorHeattoFluid);
}

void RefrigerationSecondarySystem::resetPumpMotorHeattoFluid() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetPumpMotorHeattoFluid();
}

void RefrigerationSecondarySystem::setSumUADistributionPiping(double sumUADistributionPiping) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setSumUADistributionPiping(sumUADistributionPiping);
}

void RefrigerationSecondarySystem::resetSumUADistributionPiping() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetSumUADistributionPiping();
}

bool RefrigerationSecondarySystem::setDistributionPipingZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setDistributionPipingZone(thermalZone);
}

void RefrigerationSecondarySystem::resetDistributionPipingZone() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetDistributionPipingZone();
}

void RefrigerationSecondarySystem::setSumUAReceiverSeparatorShell(double sumUAReceiverSeparatorShell) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setSumUAReceiverSeparatorShell(sumUAReceiverSeparatorShell);
}

void RefrigerationSecondarySystem::resetSumUAReceiverSeparatorShell() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetSumUAReceiverSeparatorShell();
}

bool RefrigerationSecondarySystem::setReceiverSeparatorZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationSecondarySystem_Impl>()->setReceiverSeparatorZone(thermalZone);
}

void RefrigerationSecondarySystem::resetReceiverSeparatorZone() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetReceiverSeparatorZone();
}

void RefrigerationSecondarySystem::setEvaporatorRefrigerantInventory(double evaporatorRefrigerantInventory) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEvaporatorRefrigerantInventory(evaporatorRefrigerantInventory);
}

void RefrigerationSecondarySystem::resetEvaporatorRefrigerantInventory() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetEvaporatorRefrigerantInventory();
}

void RefrigerationSecondarySystem::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationSecondarySystem::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationSecondarySystem_Impl>()->resetEndUseSubcategory();
}

/// @cond
RefrigerationSecondarySystem::RefrigerationSecondarySystem(std::shared_ptr<detail::RefrigerationSecondarySystem_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

