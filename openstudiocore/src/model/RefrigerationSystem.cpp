/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/RefrigerationSystem.hpp>
#include <model/RefrigerationSystem_Impl.hpp>

#include <model/ModelObjectList.hpp>
#include <model/ModelObjectList_Impl.hpp>
#include <model/RefrigerationCondenserCascade.hpp>
#include <model/RefrigerationCondenserCascade_Impl.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCase_Impl.hpp>
#include <model/RefrigerationCompressor.hpp>
#include <model/RefrigerationCompressor_Impl.hpp>
#include <model/RefrigerationSecondarySystem.hpp>
#include <model/RefrigerationSecondarySystem_Impl.hpp>
#include <model/RefrigerationWalkIn.hpp>
#include <model/RefrigerationWalkIn_Impl.hpp>
#include <model/RefrigerationSubcoolerLiquidSuction.hpp>
#include <model/RefrigerationSubcoolerLiquidSuction_Impl.hpp>
#include <model/RefrigerationSubcoolerMechanical.hpp>
#include <model/RefrigerationSubcoolerMechanical_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Refrigeration_System_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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

    // if (boost::optional<ModelObjectList> highStageCompressorList = this->highStageCompressorList()) {
    //   std::vector<IdfObject> removedHighStageCompressors = highStageCompressorList->remove();
    //   result.insert(result.end(), removedHighStageCompressors.begin(), removedHighStageCompressors.end());
    // }

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

    // if (boost::optional<ModelObjectList> highStageCompressorList = this->highStageCompressorList()) {
    //   ModelObjectList highStageCompressorListClone = highStageCompressorList->clone(model).cast<ModelObjectList>();
    //   modelObjectClone.getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(highStageCompressorListClone);
    // }

    modelObjectClone.resetSuctionPipingZone();

    return modelObjectClone;
  }

  template <class T>
  std::vector<T> RefrigerationSystem_Impl::listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const {
    std::vector<T> result;

    if( modelObjectList ) {
      std::vector<ModelObject> modelObjects = modelObjectList->modelObjects();

      for(std::vector<ModelObject>::iterator it = modelObjects.begin(); it != modelObjects.end(); ++it) {
          boost::optional<T> caseOrWalkin = it->optionalCast<T>();
          if (caseOrWalkin) {
            result.push_back(caseOrWalkin.get());
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
    std::vector<RefrigerationCompressor> result;

      std::vector<ModelObject> modelObjects = compressorList().modelObjects();

      if(!modelObjects.empty()) {
        for(std::vector<ModelObject>::iterator it = modelObjects.begin(); it != modelObjects.end(); ++it) {
            result.push_back(it->cast<RefrigerationCompressor>());
        }
      }

    return result;
  }

  std::vector<RefrigerationSecondarySystem> RefrigerationSystem_Impl::secondarySystemLoads() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationSecondarySystem>( refrigerationTransferLoadList() );
  }

  std::vector<RefrigerationCondenserCascade> RefrigerationSystem_Impl::cascadeCondenserLoads() const {
    return RefrigerationSystem_Impl::listTemplate<RefrigerationCondenserCascade>(refrigerationTransferLoadList());
  }

  boost::optional<ModelObjectList> RefrigerationSystem_Impl::refrigeratedCaseAndWalkInList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName);
  }

  boost::optional<ModelObjectList> RefrigerationSystem_Impl::refrigerationTransferLoadList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName);
  }

  ModelObject RefrigerationSystem_Impl::refrigerationCondenser() const {
    boost::optional<ModelObject> value = optionalRefrigerationCondenser();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Refrigeration Condenser attached.");
    }
    return value.get();
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

  // std::string RefrigerationSystem_Impl::numberofCompressorStages() const {
  //   boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::NumberofCompressorStages,true);
  //   OS_ASSERT(value);
  //   return value.get();
  // }

  // bool RefrigerationSystem_Impl::isNumberofCompressorStagesDefaulted() const {
  //   return isEmpty(OS_Refrigeration_SystemFields::NumberofCompressorStages);
  // }

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

      for(std::vector<ModelObject>::iterator it = modelObjects.begin(); it != modelObjects.end(); ++it) {
          boost::optional<T> modelObject = it->optionalCast<T>();
          if (modelObject) {
            modelObjectList->removeModelObject(*it);
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

  // bool RefrigerationSystem_Impl::setNumberofCompressorStages(std::string numberofCompressorStages) {
  //   bool result = setString(OS_Refrigeration_SystemFields::NumberofCompressorStages, numberofCompressorStages);
  //   return result;
  // }

  // void RefrigerationSystem_Impl::resetNumberofCompressorStages() {
  //   bool result = setString(OS_Refrigeration_SystemFields::NumberofCompressorStages, "");
  //   OS_ASSERT(result);
  // }

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

  bool ok = true;
  ModelObjectList caseAndWalkinList = ModelObjectList(model);
  caseAndWalkinList.setName(this->name().get() + " Case and Walkin List");
  ok = getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(caseAndWalkinList);
  OS_ASSERT(ok);
  ModelObjectList transferLoadList = ModelObjectList(model);
  transferLoadList.setName(this->name().get() + " Transfer Load List");
  ok = getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(transferLoadList);
  OS_ASSERT(ok);
  ModelObjectList compressorlist = ModelObjectList(model);
  compressorlist.setName(this->name().get() + " Compressor List");
  ok = getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(compressorlist);
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

// std::vector<std::string> RefrigerationSystem::numberofCompressorStagesValues() {
//   return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
//                         OS_Refrigeration_SystemFields::NumberofCompressorStages);
// }

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

std::vector<RefrigerationSecondarySystem> RefrigerationSystem::secondarySystemLoads() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->secondarySystemLoads();
}

std::vector<RefrigerationCondenserCascade> RefrigerationSystem::cascadeCondenserLoads() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->cascadeCondenserLoads();
}

/*boost::optional<ModelObjectList> RefrigerationSystem::refrigeratedCaseAndWalkInList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
}

boost::optional<ModelObjectList> RefrigerationSystem::refrigerationTransferLoadList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
}*/

ModelObject RefrigerationSystem::refrigerationCondenser() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationCondenser();
}

/*ModelObjectList RefrigerationSystem::compressorList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->compressorList();
}*/

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

/*std::string RefrigerationSystem::numberofCompressorStages() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->numberofCompressorStages();
}

bool RefrigerationSystem::isNumberofCompressorStagesDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isNumberofCompressorStagesDefaulted();
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

boost::optional<ModelObjectList> RefrigerationSystem::highStageCompressorList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->highStageCompressorList();
}*/

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

/*bool RefrigerationSystem::setRefrigeratedCaseAndWalkInList(const ModelObjectList& modelObjectList) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(modelObjectList);
}

void RefrigerationSystem::resetRefrigeratedCaseAndWalkInList() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetRefrigeratedCaseAndWalkInList();
}

bool RefrigerationSystem::setRefrigerationTransferLoadList(const ModelObjectList& modelObjectList) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(modelObjectList);
}

void RefrigerationSystem::resetRefrigerationTransferLoadList() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetRefrigerationTransferLoadList();
}*/

bool RefrigerationSystem::setRefrigerationCondenser(const ModelObject& refrigerationCondenser) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationCondenser(refrigerationCondenser);
}

/*bool RefrigerationSystem::setCompressorList(const ModelObjectList& modelObjectList) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(modelObjectList);
}*/

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

/*bool RefrigerationSystem::setNumberofCompressorStages(std::string numberofCompressorStages) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setNumberofCompressorStages(numberofCompressorStages);
}

void RefrigerationSystem::resetNumberofCompressorStages() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetNumberofCompressorStages();
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

bool RefrigerationSystem::setHighStageCompressorList(const ModelObjectList& modelObjectList) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(modelObjectList);
}

void RefrigerationSystem::resetHighStageCompressorList() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetHighStageCompressorList();
}*/

/// @cond
RefrigerationSystem::RefrigerationSystem(boost::shared_ptr<detail::RefrigerationSystem_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

