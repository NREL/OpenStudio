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

#include "RefrigerationSystem.hpp"
#include "RefrigerationSystem_Impl.hpp"

#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "RefrigerationAirChiller.hpp"
#include "RefrigerationAirChiller_Impl.hpp"
#include "RefrigerationCondenserCascade.hpp"
#include "RefrigerationCondenserCascade_Impl.hpp"
#include "RefrigerationCase.hpp"
#include "RefrigerationCase_Impl.hpp"
#include "RefrigerationCompressor.hpp"
#include "RefrigerationCompressor_Impl.hpp"
#include "RefrigerationSecondarySystem.hpp"
#include "RefrigerationSecondarySystem_Impl.hpp"
#include "RefrigerationWalkIn.hpp"
#include "RefrigerationWalkIn_Impl.hpp"
#include "RefrigerationSubcoolerLiquidSuction.hpp"
#include "RefrigerationSubcoolerLiquidSuction_Impl.hpp"
#include "RefrigerationSubcoolerMechanical.hpp"
#include "RefrigerationSubcoolerMechanical_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_System_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationSystem_Impl::RefrigerationSystem_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationSystem::iddObjectType());
  }

  RefrigerationSystem_Impl::RefrigerationSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationSystem::iddObjectType());
  }

  RefrigerationSystem_Impl::RefrigerationSystem_Impl(const RefrigerationSystem_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationSystem_Impl::iddObjectType() const {
    return RefrigerationSystem::iddObjectType();
  }

  std::vector<IdfObject> RefrigerationSystem_Impl::remove()
  {
    std::vector<IdfObject> result;

    if ( boost::optional<ModelObject> condenser = this->optionalRefrigerationCondenser() ) {
      std::vector<IdfObject> removedCondenser = condenser->remove();
      result.insert(result.end(), removedCondenser.begin(), removedCondenser.end());
    }

    if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
      std::vector<IdfObject> removedCasesAndWalkins = caseAndWalkinList->remove();
      result.insert(result.end(), removedCasesAndWalkins.begin(), removedCasesAndWalkins.end());
    }

    if (boost::optional<ModelObjectList> transferLoadList = this->refrigerationTransferLoadList()) {
      std::vector<IdfObject> removedTransferLoads = transferLoadList->remove();
      result.insert(result.end(), removedTransferLoads.begin(), removedTransferLoads.end());
    }

    if (boost::optional<ModelObjectList> compressorList = this->compressorList()) {
      std::vector<IdfObject> removedCompressors = compressorList->remove();
      result.insert(result.end(), removedCompressors.begin(), removedCompressors.end());
    }

    if ( boost::optional<RefrigerationSubcoolerMechanical> mechSubcooler = this->mechanicalSubcooler() ) {
      std::vector<IdfObject> removedMechSubcooler = mechSubcooler->remove();
      result.insert(result.end(), removedMechSubcooler.begin(), removedMechSubcooler.end());
    }

    if ( boost::optional<RefrigerationSubcoolerLiquidSuction> liqSuctionSubcooler = this->liquidSuctionHeatExchangerSubcooler() ) {
      std::vector<IdfObject> removedLiqSuctionSubcooler = liqSuctionSubcooler->remove();
      result.insert(result.end(), removedLiqSuctionSubcooler.begin(), removedLiqSuctionSubcooler.end());
    }

    if (boost::optional<ModelObjectList> highStageCompressorList = this->highStageCompressorList()) {
      std::vector<IdfObject> removedHighStageCompressors = highStageCompressorList->remove();
      result.insert(result.end(), removedHighStageCompressors.begin(), removedHighStageCompressors.end());
    }

    std::vector<IdfObject> removedRefrigerationSystem = ModelObject_Impl::remove();
    result.insert(result.end(), removedRefrigerationSystem.begin(), removedRefrigerationSystem.end());

    return result;
  }

  ModelObject RefrigerationSystem_Impl::clone(Model model) const
  {
    RefrigerationSystem modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationSystem>();

    if ( boost::optional<ModelObject> condenser = this->optionalRefrigerationCondenser() ) {
      ModelObject condenserClone = condenser->clone(model);
      modelObjectClone.setRefrigerationCondenser(condenserClone);
    }

    if (boost::optional<ModelObjectList> caseAndWalkinList = this->refrigeratedCaseAndWalkInList()) {
      ModelObjectList caseAndWalkinListClone = caseAndWalkinList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinListClone);
    }

    if (boost::optional<ModelObjectList> transferLoadList = this->refrigerationTransferLoadList()) {
      ModelObjectList transferLoadListClone = transferLoadList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(transferLoadListClone);
    }

    if (boost::optional<ModelObjectList> compressorList = this->compressorList()) {
      ModelObjectList compressorListClone = compressorList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(compressorListClone);
    }

    if (boost::optional<RefrigerationSubcoolerMechanical> mechSubcooler = this->mechanicalSubcooler()) {
      RefrigerationSubcoolerMechanical mechSubClone = mechSubcooler->clone(model).cast<RefrigerationSubcoolerMechanical>();
      modelObjectClone.setMechanicalSubcooler(mechSubClone);
    }

    if (boost::optional<RefrigerationSubcoolerLiquidSuction> liqSuctionSubcooler = this->liquidSuctionHeatExchangerSubcooler()) {
      RefrigerationSubcoolerLiquidSuction liqSuctionSubClone = liqSuctionSubcooler->clone(model).cast<RefrigerationSubcoolerLiquidSuction>();
      modelObjectClone.setLiquidSuctionHeatExchangerSubcooler(liqSuctionSubClone);
    }

    if (boost::optional<ModelObjectList> highStageCompressorList = this->highStageCompressorList()) {
      ModelObjectList highStageCompressorListClone = highStageCompressorList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(highStageCompressorListClone);
    }

    modelObjectClone.resetSuctionPipingZone();

    return modelObjectClone;
  }

  template <class T>
  std::vector<T> RefrigerationSystem_Impl::listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const {
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

  std::vector<RefrigerationCase> RefrigerationSystem_Impl::cases() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationCase>( refrigeratedCaseAndWalkInList() );
  }

  std::vector<RefrigerationWalkIn> RefrigerationSystem_Impl::walkins() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationWalkIn>( refrigeratedCaseAndWalkInList() );
  }

  std::vector<RefrigerationCompressor> RefrigerationSystem_Impl::compressors() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationCompressor>( compressorList() );
  }

  std::vector<RefrigerationCompressor> RefrigerationSystem_Impl::highStageCompressors() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationCompressor>( highStageCompressorList() );
  }

  std::vector<RefrigerationSecondarySystem> RefrigerationSystem_Impl::secondarySystemLoads() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationSecondarySystem>( refrigerationTransferLoadList() );
  }

  std::vector<RefrigerationCondenserCascade> RefrigerationSystem_Impl::cascadeCondenserLoads() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationCondenserCascade>(refrigerationTransferLoadList());
  }

  std::vector<RefrigerationAirChiller> RefrigerationSystem_Impl::airChillers() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationAirChiller>(refrigeratedCaseAndWalkInList());
  }

  boost::optional<ModelObjectList> RefrigerationSystem_Impl::refrigeratedCaseAndWalkInList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName);
  }

  boost::optional<ModelObjectList> RefrigerationSystem_Impl::refrigerationTransferLoadList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName);
  }

  boost::optional<ModelObject> RefrigerationSystem_Impl::refrigerationCondenser() const {
    return optionalRefrigerationCondenser();
  }

  ModelObjectList RefrigerationSystem_Impl::compressorList() const {
    boost::optional<ModelObjectList> value = optionalCompressorList();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Compressor List attached.");
    }
    return value.get();
  }

  double RefrigerationSystem_Impl::minimumCondensingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string RefrigerationSystem_Impl::refrigerationSystemWorkingFluidType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string RefrigerationSystem_Impl::suctionTemperatureControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isSuctionTemperatureControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::SuctionTemperatureControlType);
  }

  boost::optional<RefrigerationSubcoolerMechanical> RefrigerationSystem_Impl::mechanicalSubcooler() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationSubcoolerMechanical>(OS_Refrigeration_SystemFields::MechanicalSubcoolerName);
  }

  boost::optional<RefrigerationSubcoolerLiquidSuction> RefrigerationSystem_Impl::liquidSuctionHeatExchangerSubcooler() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationSubcoolerLiquidSuction>(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName);
  }

  double RefrigerationSystem_Impl::sumUASuctionPiping() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isSumUASuctionPipingDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::SumUASuctionPiping);
  }

  boost::optional<ThermalZone> RefrigerationSystem_Impl::suctionPipingZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_SystemFields::SuctionPipingZoneName);
  }

  std::string RefrigerationSystem_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::EndUseSubcategory);
  }

  std::string RefrigerationSystem_Impl::numberofCompressorStages() const {
    if ( this->highStageCompressors().empty() ) {
      return "1";
    } else {
      return "2";
    }
  }

  std::string RefrigerationSystem_Impl::intercoolerType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::IntercoolerType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isIntercoolerTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::IntercoolerType);
  }

  double RefrigerationSystem_Impl::shellandCoilIntercoolerEffectiveness() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isShellandCoilIntercoolerEffectivenessDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness);
  }

  boost::optional<ModelObjectList> RefrigerationSystem_Impl::highStageCompressorList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::HighStageCompressorListName);
  }

  template <class T>
  void RefrigerationSystem_Impl::removeAllTemplate( boost::optional<ModelObjectList>& modelObjectList ) {
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
  void RefrigerationSystem_Impl::removeTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      modelObjectList->removeModelObject(modelObject);
    }
  }

  template <class T>
  bool RefrigerationSystem_Impl::addTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      return modelObjectList->addModelObject(modelObject);
    }
    return false;
  }

  bool RefrigerationSystem_Impl::addCase( const RefrigerationCase& refrigerationCase) {
    if( boost::optional<RefrigerationSystem> currentSystem = refrigerationCase.system() )
    {
      currentSystem->removeCase(refrigerationCase);
    }
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    return addTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeCase( const RefrigerationCase& refrigerationCase) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAllCases() {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeAllTemplate<RefrigerationCase>(modelObjectList);
  }

  bool RefrigerationSystem_Impl::addWalkin( const RefrigerationWalkIn& refrigerationWalkin) {
    if( boost::optional<RefrigerationSystem> currentSystem = refrigerationWalkin.system() )
    {
      currentSystem->removeWalkin(refrigerationWalkin);
    }
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    return addTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeWalkin( const RefrigerationWalkIn& refrigerationWalkin) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAllWalkins() {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeAllTemplate<RefrigerationWalkIn>(modelObjectList);
  }

  bool RefrigerationSystem_Impl::addCompressor(const RefrigerationCompressor& refrigerationCompressor) {
    return compressorList().addModelObject(refrigerationCompressor);
  }

  void RefrigerationSystem_Impl::removeCompressor(const RefrigerationCompressor& refrigerationCompressor) {
    compressorList().removeModelObject(refrigerationCompressor);
  }

  void RefrigerationSystem_Impl::removeAllCompressors() {
    compressorList().removeAllModelObjects();
  }

  bool RefrigerationSystem_Impl::addHighStageCompressor( const RefrigerationCompressor& refrigerationHighStageCompressor ) {
    boost::optional<ModelObjectList> modelObjectList = highStageCompressorList();
    return addTemplate<RefrigerationCompressor>(refrigerationHighStageCompressor, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeHighStageCompressor( const RefrigerationCompressor& refrigerationHighStageCompressor ) {
    boost::optional<ModelObjectList> modelObjectList = highStageCompressorList();
    removeTemplate<RefrigerationCompressor>(refrigerationHighStageCompressor, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAllHighStageCompressors() {
    boost::optional<ModelObjectList> modelObjectList = highStageCompressorList();
    removeAllTemplate<RefrigerationCompressor>(modelObjectList);
  }

  bool RefrigerationSystem_Impl::addSecondarySystemLoad( const RefrigerationSecondarySystem& refrigerationSecondarySystem) {
    boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
    return addTemplate<RefrigerationSecondarySystem>(refrigerationSecondarySystem, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeSecondarySystemLoad( const RefrigerationSecondarySystem& refrigerationSecondarySystem) {
    boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
    removeTemplate<RefrigerationSecondarySystem>(refrigerationSecondarySystem, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAllSecondarySystemLoads() {
    boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
    removeAllTemplate<RefrigerationSecondarySystem>(modelObjectList);
  }

  bool RefrigerationSystem_Impl::addCascadeCondenserLoad( const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
    boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
    return addTemplate<RefrigerationCondenserCascade>(refrigerationCondenserCascade, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeCascadeCondenserLoad( const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
    boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
    removeTemplate<RefrigerationCondenserCascade>(refrigerationCondenserCascade, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAllCascadeCondenserLoads() {
    boost::optional<ModelObjectList> modelObjectList = refrigerationTransferLoadList();
    removeAllTemplate<RefrigerationCondenserCascade>(modelObjectList);
  }

  bool RefrigerationSystem_Impl::addAirChiller( const RefrigerationAirChiller& refrigerationAirChiller) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    return addTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAirChiller( const RefrigerationAirChiller& refrigerationAirChiller) {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeTemplate<RefrigerationAirChiller>(refrigerationAirChiller, modelObjectList);
  }

  void RefrigerationSystem_Impl::removeAllAirChillers() {
    boost::optional<ModelObjectList> modelObjectList = refrigeratedCaseAndWalkInList();
    removeAllTemplate<RefrigerationAirChiller>(modelObjectList);
  }

  bool RefrigerationSystem_Impl::setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, modelObjectList.get().handle());
    }
    else {
      resetRefrigeratedCaseAndWalkInList();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetRefrigeratedCaseAndWalkInList() {
    bool result = setString(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setRefrigerationTransferLoadList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName, modelObjectList.get().handle());
    }
    else {
      resetRefrigerationTransferLoadList();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetRefrigerationTransferLoadList() {
    bool result = setString(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setRefrigerationCondenser(const ModelObject& refrigerationCondenser) {
    bool result = setPointer(OS_Refrigeration_SystemFields::RefrigerationCondenserName, refrigerationCondenser.handle());
    return result;
  }

  bool RefrigerationSystem_Impl::setCompressorList(const ModelObjectList& modelObjectList) {
    bool result = setPointer(OS_Refrigeration_SystemFields::CompressorListName, modelObjectList.handle());
    return result;
  }

  void RefrigerationSystem_Impl::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
    bool result = setDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature, minimumCondensingTemperature);
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType) {
    bool result = setString(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType, refrigerationSystemWorkingFluidType);
    return result;
  }

  bool RefrigerationSystem_Impl::setSuctionTemperatureControlType(std::string suctionTemperatureControlType) {
    bool result = setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, suctionTemperatureControlType);
    return result;
  }

  void RefrigerationSystem_Impl::resetSuctionTemperatureControlType() {
    bool result = setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setMechanicalSubcooler(const boost::optional<RefrigerationSubcoolerMechanical>& refrigerationSubcoolerMechanical) {
    bool result(false);
    if (refrigerationSubcoolerMechanical) {
      result = setPointer(OS_Refrigeration_SystemFields::MechanicalSubcoolerName, refrigerationSubcoolerMechanical.get().handle());
    }
    else {
      resetMechanicalSubcooler();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetMechanicalSubcooler() {
    bool result = setString(OS_Refrigeration_SystemFields::MechanicalSubcoolerName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setLiquidSuctionHeatExchangerSubcooler(const boost::optional<RefrigerationSubcoolerLiquidSuction>& refrigerationSubcoolerLiquidSuction) {
    bool result(false);
    if (refrigerationSubcoolerLiquidSuction) {
      result = setPointer(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName, refrigerationSubcoolerLiquidSuction.get().handle());
    }
    else {
      resetLiquidSuctionHeatExchangerSubcooler();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetLiquidSuctionHeatExchangerSubcooler() {
    bool result = setString(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName, "");
    OS_ASSERT(result);
  }

  void RefrigerationSystem_Impl::setSumUASuctionPiping(double sumUASuctionPiping) {
    bool result = setDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping, sumUASuctionPiping);
    OS_ASSERT(result);
  }

  void RefrigerationSystem_Impl::resetSumUASuctionPiping() {
    bool result = setString(OS_Refrigeration_SystemFields::SumUASuctionPiping, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Refrigeration_SystemFields::SuctionPipingZoneName, thermalZone.get().handle());
    }
    else {
      resetSuctionPipingZone();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetSuctionPipingZone() {
    bool result = setString(OS_Refrigeration_SystemFields::SuctionPipingZoneName, "");
    OS_ASSERT(result);
  }

  void RefrigerationSystem_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_SystemFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationSystem_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_SystemFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setIntercoolerType(std::string intercoolerType) {
    bool result = setString(OS_Refrigeration_SystemFields::IntercoolerType, intercoolerType);
    return result;
  }

  void RefrigerationSystem_Impl::resetIntercoolerType() {
    bool result = setString(OS_Refrigeration_SystemFields::IntercoolerType, "");
    OS_ASSERT(result);
  }

  void RefrigerationSystem_Impl::setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness) {
    bool result = setDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, shellandCoilIntercoolerEffectiveness);
    OS_ASSERT(result);
  }

  void RefrigerationSystem_Impl::resetShellandCoilIntercoolerEffectiveness() {
    bool result = setString(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, "");
    OS_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setHighStageCompressorList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Refrigeration_SystemFields::HighStageCompressorListName, modelObjectList.get().handle());
    }
    else {
      resetHighStageCompressorList();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetHighStageCompressorList() {
    bool result = setString(OS_Refrigeration_SystemFields::HighStageCompressorListName, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> RefrigerationSystem_Impl::optionalRefrigerationCondenser() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_Refrigeration_SystemFields::RefrigerationCondenserName);
  }

  boost::optional<ModelObjectList> RefrigerationSystem_Impl::optionalCompressorList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::CompressorListName);
  }

} // detail

RefrigerationSystem::RefrigerationSystem(const Model& model)
  : ModelObject(RefrigerationSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationSystem_Impl>());

  ModelObjectList caseAndWalkinList = ModelObjectList(model);
  caseAndWalkinList.setName(this->name().get() + " Case and Walkin List");
  bool ok = getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinList);
  OS_ASSERT(ok);
  ModelObjectList transferLoadList = ModelObjectList(model);
  transferLoadList.setName(this->name().get() + " Transfer Load List");
  ok = getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(transferLoadList);
  OS_ASSERT(ok);
  ModelObjectList compressorlist = ModelObjectList(model);
  compressorlist.setName(this->name().get() + " Compressor List");
  ok = getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(compressorlist);
  OS_ASSERT(ok);
  ModelObjectList highStageCompressorlist = ModelObjectList(model);
  highStageCompressorlist.setName(this->name().get() + " High Stage Compressor List");
  ok = getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(highStageCompressorlist);
  OS_ASSERT(ok);
  setMinimumCondensingTemperature(21.0);
  ok = setRefrigerationSystemWorkingFluidType("R404a");
  OS_ASSERT(ok);
  ok = setSuctionTemperatureControlType("ConstantSuctionTemperature");
  OS_ASSERT(ok);
}

IddObjectType RefrigerationSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_System);
}

std::vector<std::string> RefrigerationSystem::refrigerationSystemWorkingFluidTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType);
}

std::vector<std::string> RefrigerationSystem::suctionTemperatureControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SystemFields::SuctionTemperatureControlType);
}

std::vector<std::string> RefrigerationSystem::intercoolerTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SystemFields::IntercoolerType);
}

std::vector<IdfObject> RefrigerationSystem::remove() {
  return getImpl<detail::RefrigerationSystem_Impl>()->remove();
}

ModelObject RefrigerationSystem::clone(Model model) const {
  return getImpl<detail::RefrigerationSystem_Impl>()->clone(model);
}

std::vector<RefrigerationCase> RefrigerationSystem::cases() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->cases();
}

std::vector<RefrigerationWalkIn> RefrigerationSystem::walkins() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->walkins();
}

std::vector<RefrigerationCompressor> RefrigerationSystem::compressors() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->compressors();
}

std::vector<RefrigerationCompressor> RefrigerationSystem::highStageCompressors() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->highStageCompressors();
}

std::vector<RefrigerationSecondarySystem> RefrigerationSystem::secondarySystemLoads() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->secondarySystemLoads();
}

std::vector<RefrigerationCondenserCascade> RefrigerationSystem::cascadeCondenserLoads() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->cascadeCondenserLoads();
}

std::vector<RefrigerationAirChiller> RefrigerationSystem::airChillers() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->airChillers();
}

boost::optional<ModelObject> RefrigerationSystem::refrigerationCondenser() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationCondenser();
}

double RefrigerationSystem::minimumCondensingTemperature() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->minimumCondensingTemperature();
}

std::string RefrigerationSystem::refrigerationSystemWorkingFluidType() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationSystemWorkingFluidType();
}

std::string RefrigerationSystem::suctionTemperatureControlType() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->suctionTemperatureControlType();
}

bool RefrigerationSystem::isSuctionTemperatureControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isSuctionTemperatureControlTypeDefaulted();
}

boost::optional<RefrigerationSubcoolerMechanical> RefrigerationSystem::mechanicalSubcooler() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->mechanicalSubcooler();
}

boost::optional<RefrigerationSubcoolerLiquidSuction> RefrigerationSystem::liquidSuctionHeatExchangerSubcooler() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->liquidSuctionHeatExchangerSubcooler();
}

double RefrigerationSystem::sumUASuctionPiping() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->sumUASuctionPiping();
}

bool RefrigerationSystem::isSumUASuctionPipingDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isSumUASuctionPipingDefaulted();
}

boost::optional<ThermalZone> RefrigerationSystem::suctionPipingZone() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->suctionPipingZone();
}

std::string RefrigerationSystem::endUseSubcategory() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->endUseSubcategory();
}

bool RefrigerationSystem::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isEndUseSubcategoryDefaulted();
}

std::string RefrigerationSystem::numberofCompressorStages() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->numberofCompressorStages();
}

std::string RefrigerationSystem::intercoolerType() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->intercoolerType();
}

bool RefrigerationSystem::isIntercoolerTypeDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isIntercoolerTypeDefaulted();
}

double RefrigerationSystem::shellandCoilIntercoolerEffectiveness() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->shellandCoilIntercoolerEffectiveness();
}

bool RefrigerationSystem::isShellandCoilIntercoolerEffectivenessDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isShellandCoilIntercoolerEffectivenessDefaulted();
}

bool RefrigerationSystem::addCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addCase(refrigerationCase);
}

void RefrigerationSystem::removeCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeCase(refrigerationCase);
}

void RefrigerationSystem::removeAllCases() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllCases();
}

bool RefrigerationSystem::addWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addWalkin(refrigerationWalkin);
}

void RefrigerationSystem::removeWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeWalkin(refrigerationWalkin);
}

void RefrigerationSystem::removeAllWalkins() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllWalkins();
}

bool RefrigerationSystem::addCompressor(const RefrigerationCompressor& refrigerationCompressor) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addCompressor(refrigerationCompressor);
}

void RefrigerationSystem::removeCompressor(const RefrigerationCompressor& refrigerationCompressor) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeCompressor(refrigerationCompressor);
}

void RefrigerationSystem::removeAllCompressors() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllCompressors();
}

bool RefrigerationSystem::addHighStageCompressor(const RefrigerationCompressor& refrigerationHighStageCompressor) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addHighStageCompressor(refrigerationHighStageCompressor);
}

void RefrigerationSystem::removeHighStageCompressor(const RefrigerationCompressor& refrigerationHighStageCompressor) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeHighStageCompressor(refrigerationHighStageCompressor);
}

void RefrigerationSystem::removeAllHighStageCompressors() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllHighStageCompressors();
}

bool RefrigerationSystem::addSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystemLoad) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addSecondarySystemLoad(refrigerationSecondarySystemLoad);
}

void RefrigerationSystem::removeSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystemLoad) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeSecondarySystemLoad(refrigerationSecondarySystemLoad);
}

void RefrigerationSystem::removeAllSecondarySystemLoads() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllSecondarySystemLoads();
}

bool RefrigerationSystem::addCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addCascadeCondenserLoad(refrigerationCondenserCascade);
}

void RefrigerationSystem::removeCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeCascadeCondenserLoad(refrigerationCondenserCascade);
}

void RefrigerationSystem::removeAllCascadeCondenserLoads() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllCascadeCondenserLoads();
}

bool RefrigerationSystem::addAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
  return getImpl<detail::RefrigerationSystem_Impl>()->addAirChiller(refrigerationAirChiller);
}

void RefrigerationSystem::removeAirChiller(const RefrigerationAirChiller& refrigerationAirChiller) {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAirChiller(refrigerationAirChiller);
}

void RefrigerationSystem::removeAllAirChillers() {
  return getImpl<detail::RefrigerationSystem_Impl>()->removeAllAirChillers();
}

bool RefrigerationSystem::setRefrigerationCondenser(const ModelObject& refrigerationCondenser) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationCondenser(refrigerationCondenser);
}

void RefrigerationSystem::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
  getImpl<detail::RefrigerationSystem_Impl>()->setMinimumCondensingTemperature(minimumCondensingTemperature);
}

bool RefrigerationSystem::setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationSystemWorkingFluidType(refrigerationSystemWorkingFluidType);
}

bool RefrigerationSystem::setSuctionTemperatureControlType(std::string suctionTemperatureControlType) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setSuctionTemperatureControlType(suctionTemperatureControlType);
}

void RefrigerationSystem::resetSuctionTemperatureControlType() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetSuctionTemperatureControlType();
}

bool RefrigerationSystem::setMechanicalSubcooler(const RefrigerationSubcoolerMechanical& refrigerationSubcoolerMechanical) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setMechanicalSubcooler(refrigerationSubcoolerMechanical);
}

void RefrigerationSystem::resetMechanicalSubcooler() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetMechanicalSubcooler();
}

bool RefrigerationSystem::setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcoolerLiquidSuction& refrigerationSubcoolerLiquidSuction) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setLiquidSuctionHeatExchangerSubcooler(refrigerationSubcoolerLiquidSuction);
}

void RefrigerationSystem::resetLiquidSuctionHeatExchangerSubcooler() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetLiquidSuctionHeatExchangerSubcooler();
}

void RefrigerationSystem::setSumUASuctionPiping(double sumUASuctionPiping) {
  getImpl<detail::RefrigerationSystem_Impl>()->setSumUASuctionPiping(sumUASuctionPiping);
}

void RefrigerationSystem::resetSumUASuctionPiping() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetSumUASuctionPiping();
}

bool RefrigerationSystem::setSuctionPipingZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setSuctionPipingZone(thermalZone);
}

void RefrigerationSystem::resetSuctionPipingZone() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetSuctionPipingZone();
}

void RefrigerationSystem::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationSystem_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationSystem::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetEndUseSubcategory();
}

bool RefrigerationSystem::setIntercoolerType(std::string intercoolerType) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setIntercoolerType(intercoolerType);
}

void RefrigerationSystem::resetIntercoolerType() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetIntercoolerType();
}

void RefrigerationSystem::setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness) {
  getImpl<detail::RefrigerationSystem_Impl>()->setShellandCoilIntercoolerEffectiveness(shellandCoilIntercoolerEffectiveness);
}

void RefrigerationSystem::resetShellandCoilIntercoolerEffectiveness() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetShellandCoilIntercoolerEffectiveness();
}

/// @cond
RefrigerationSystem::RefrigerationSystem(std::shared_ptr<detail::RefrigerationSystem_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

