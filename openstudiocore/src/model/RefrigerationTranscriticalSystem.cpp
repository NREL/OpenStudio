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

#include "RefrigerationTranscriticalSystem.hpp"
#include "RefrigerationTranscriticalSystem_Impl.hpp"

#include "RefrigerationCase.hpp"
#include "RefrigerationCase_Impl.hpp"
#include "RefrigerationCompressor.hpp"
#include "RefrigerationCompressor_Impl.hpp"
#include "RefrigerationWalkIn.hpp"
#include "RefrigerationWalkIn_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "RefrigerationGasCoolerAirCooled.hpp"
#include "RefrigerationGasCoolerAirCooled_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_TranscriticalSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationTranscriticalSystem_Impl::RefrigerationTranscriticalSystem_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationTranscriticalSystem::iddObjectType());
  }

  RefrigerationTranscriticalSystem_Impl::RefrigerationTranscriticalSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationTranscriticalSystem::iddObjectType());
  }

  RefrigerationTranscriticalSystem_Impl::RefrigerationTranscriticalSystem_Impl(const RefrigerationTranscriticalSystem_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationTranscriticalSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationTranscriticalSystem_Impl::iddObjectType() const {
    return RefrigerationTranscriticalSystem::iddObjectType();
  }

  std::vector<IdfObject> RefrigerationTranscriticalSystem_Impl::remove()
  {
    std::vector<IdfObject> result;

    if (boost::optional<ModelObjectList> mediumTemperatureCaseAndWalkinList = this->mediumTemperatureRefrigeratedCaseAndWalkInList()) {
      std::vector<IdfObject> removedCasesAndWalkins = mediumTemperatureCaseAndWalkinList->remove();
      result.insert(result.end(), removedCasesAndWalkins.begin(), removedCasesAndWalkins.end());
    }

    if (boost::optional<ModelObjectList> lowTemperatureCaseAndWalkinList = this->lowTemperatureRefrigeratedCaseAndWalkInList()) {
      std::vector<IdfObject> removedCasesAndWalkins = lowTemperatureCaseAndWalkinList->remove();
      result.insert(result.end(), removedCasesAndWalkins.begin(), removedCasesAndWalkins.end());
    }

    if (boost::optional<ModelObjectList> highPressureCompressorList = this->highPressureCompressorList()) {
      std::vector<IdfObject> removedCompressors = highPressureCompressorList->remove();
      result.insert(result.end(), removedCompressors.begin(), removedCompressors.end());
    }

    if (boost::optional<ModelObjectList> lowPressureCompressorList = this->lowPressureCompressorList()) {
      std::vector<IdfObject> removedCompressors = lowPressureCompressorList->remove();
      result.insert(result.end(), removedCompressors.begin(), removedCompressors.end());
    }

    if (boost::optional<RefrigerationGasCoolerAirCooled> refrigerationGasCooler = this->refrigerationGasCooler()) {
      std::vector<IdfObject> removedGasCoolers = refrigerationGasCooler->remove();
      result.insert(result.end(), removedGasCoolers.begin(), removedGasCoolers.end());
    }

    std::vector<IdfObject> removedRefrigerationTranscriticalSystem = ModelObject_Impl::remove();
    result.insert(result.end(), removedRefrigerationTranscriticalSystem.begin(), removedRefrigerationTranscriticalSystem.end());

    return result;
  }

  ModelObject RefrigerationTranscriticalSystem_Impl::clone(Model model) const
  {
    RefrigerationTranscriticalSystem modelObjectClone = ModelObject_Impl::clone(model).cast<RefrigerationTranscriticalSystem>();

    if (boost::optional<ModelObjectList> mediumTemperatureCaseAndWalkinList = this->mediumTemperatureRefrigeratedCaseAndWalkInList()) {
      ModelObjectList caseAndWalkinListClone = mediumTemperatureCaseAndWalkinList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setMediumTemperatureRefrigeratedCaseAndWalkInList(caseAndWalkinListClone);
    }

    if (boost::optional<ModelObjectList> lowTemperatureCaseAndWalkinList = this->lowTemperatureRefrigeratedCaseAndWalkInList()) {
      ModelObjectList caseAndWalkinListClone = lowTemperatureCaseAndWalkinList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setLowTemperatureRefrigeratedCaseAndWalkInList(caseAndWalkinListClone);
    }

    if (boost::optional<ModelObjectList> highPressureCompressorList = this->highPressureCompressorList()) {
      ModelObjectList compressorListClone = highPressureCompressorList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setHighPressureCompressorList(compressorListClone);
    }

    if (boost::optional<ModelObjectList> lowPressureCompressorList = this->lowPressureCompressorList()) {
      ModelObjectList compressorListClone = lowPressureCompressorList->clone(model).cast<ModelObjectList>();
      modelObjectClone.getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setLowPressureCompressorList(compressorListClone);
    }

    if (boost::optional<RefrigerationGasCoolerAirCooled> refrigerationGasCooler = this->refrigerationGasCooler()) {
      RefrigerationGasCoolerAirCooled refrigerationGasCoolerClone = refrigerationGasCooler->clone(model).cast<RefrigerationGasCoolerAirCooled>();
      modelObjectClone.getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setRefrigerationGasCooler(refrigerationGasCoolerClone);
    }

    modelObjectClone.resetMediumTemperatureSuctionPipingZone();
    modelObjectClone.resetLowTemperatureSuctionPipingZone();

    return modelObjectClone;
  }

  template <class T>
  std::vector<T> RefrigerationTranscriticalSystem_Impl::listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const {
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

  template <class T>
  void RefrigerationTranscriticalSystem_Impl::removeAllTemplate( boost::optional<ModelObjectList>& modelObjectList ) {
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
  void RefrigerationTranscriticalSystem_Impl::removeTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      modelObjectList->removeModelObject(modelObject);
    }
  }

  template <class T>
  bool RefrigerationTranscriticalSystem_Impl::addTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList ) {
    if( modelObjectList ) {
      return modelObjectList->addModelObject(modelObject);
    }
    return false;
  }

  bool RefrigerationTranscriticalSystem_Impl::addMediumTemperatureCase( const RefrigerationCase & refrigerationCase) {
    boost::optional<ModelObjectList> modelObjectList = mediumTemperatureRefrigeratedCaseAndWalkInList();
    return RefrigerationTranscriticalSystem_Impl::addTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeMediumTemperatureCase( const RefrigerationCase & refrigerationCase) {
    boost::optional<ModelObjectList> modelObjectList = mediumTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeAllMediumTemperatureCases() {
    boost::optional<ModelObjectList> modelObjectList = mediumTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeAllTemplate<RefrigerationCase>(modelObjectList);
  }

  std::vector<RefrigerationCase> RefrigerationTranscriticalSystem_Impl::mediumTemperatureCases() const {
    return RefrigerationTranscriticalSystem_Impl::listTemplate<RefrigerationCase>( mediumTemperatureRefrigeratedCaseAndWalkInList() );
  }

  bool RefrigerationTranscriticalSystem_Impl::addLowTemperatureCase( const RefrigerationCase & refrigerationCase) {
    boost::optional<ModelObjectList> modelObjectList = lowTemperatureRefrigeratedCaseAndWalkInList();
    return RefrigerationTranscriticalSystem_Impl::addTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeLowTemperatureCase( const RefrigerationCase & refrigerationCase) {
    boost::optional<ModelObjectList> modelObjectList = lowTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeTemplate<RefrigerationCase>(refrigerationCase, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeAllLowTemperatureCases() {
    boost::optional<ModelObjectList> modelObjectList = lowTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeAllTemplate<RefrigerationCase>(modelObjectList);
  }

  std::vector<RefrigerationCase> RefrigerationTranscriticalSystem_Impl::lowTemperatureCases() const {
    return RefrigerationTranscriticalSystem_Impl::listTemplate<RefrigerationCase>( lowTemperatureRefrigeratedCaseAndWalkInList() );
  }

  bool RefrigerationTranscriticalSystem_Impl::addMediumTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin) {
    boost::optional<ModelObjectList> modelObjectList = mediumTemperatureRefrigeratedCaseAndWalkInList();
    return RefrigerationTranscriticalSystem_Impl::addTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeMediumTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin) {
    boost::optional<ModelObjectList> modelObjectList = mediumTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeAllMediumTemperatureWalkins() {
    boost::optional<ModelObjectList> modelObjectList = mediumTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeAllTemplate<RefrigerationWalkIn>(modelObjectList);
  }

  std::vector<RefrigerationWalkIn> RefrigerationTranscriticalSystem_Impl::mediumTemperatureWalkins() const {
    return RefrigerationTranscriticalSystem_Impl::listTemplate<RefrigerationWalkIn>( mediumTemperatureRefrigeratedCaseAndWalkInList() );
  }

  bool RefrigerationTranscriticalSystem_Impl::addLowTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin) {
    boost::optional<ModelObjectList> modelObjectList = lowTemperatureRefrigeratedCaseAndWalkInList();
    return RefrigerationTranscriticalSystem_Impl::addTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeLowTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin) {
    boost::optional<ModelObjectList> modelObjectList = lowTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeTemplate<RefrigerationWalkIn>(refrigerationWalkin, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeAllLowTemperatureWalkins() {
    boost::optional<ModelObjectList> modelObjectList = lowTemperatureRefrigeratedCaseAndWalkInList();
    RefrigerationTranscriticalSystem_Impl::removeAllTemplate<RefrigerationWalkIn>(modelObjectList);
  }

  std::vector<RefrigerationWalkIn> RefrigerationTranscriticalSystem_Impl::lowTemperatureWalkins() const {
    return RefrigerationTranscriticalSystem_Impl::listTemplate<RefrigerationWalkIn>( lowTemperatureRefrigeratedCaseAndWalkInList() );
  }

  bool RefrigerationTranscriticalSystem_Impl::addHighPressureCompressor( const RefrigerationCompressor & compressor) {
    boost::optional<ModelObjectList> modelObjectList = highPressureCompressorList();
    return RefrigerationTranscriticalSystem_Impl::addTemplate<RefrigerationCompressor>(compressor, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeHighPressureCompressor( const RefrigerationCompressor & compressor) {
    boost::optional<ModelObjectList> modelObjectList = highPressureCompressorList();
    RefrigerationTranscriticalSystem_Impl::removeTemplate<RefrigerationCompressor>(compressor, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeAllHighPressureCompressors() {
    boost::optional<ModelObjectList> modelObjectList = highPressureCompressorList();
    RefrigerationTranscriticalSystem_Impl::removeAllTemplate<RefrigerationCompressor>(modelObjectList);
  }

  std::vector<RefrigerationCompressor> RefrigerationTranscriticalSystem_Impl::highPressureCompressors() const {
    return RefrigerationTranscriticalSystem_Impl::listTemplate<RefrigerationCompressor>( highPressureCompressorList() );
  }

  bool RefrigerationTranscriticalSystem_Impl::addLowPressureCompressor( const RefrigerationCompressor & compressor) {
    boost::optional<ModelObjectList> modelObjectList = lowPressureCompressorList();
    return RefrigerationTranscriticalSystem_Impl::addTemplate<RefrigerationCompressor>(compressor, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeLowPressureCompressor( const RefrigerationCompressor & compressor) {
    boost::optional<ModelObjectList> modelObjectList = lowPressureCompressorList();
    RefrigerationTranscriticalSystem_Impl::removeTemplate<RefrigerationCompressor>(compressor, modelObjectList);
  }

  void RefrigerationTranscriticalSystem_Impl::removeAllLowPressureCompressors() {
    boost::optional<ModelObjectList> modelObjectList = lowPressureCompressorList();
    RefrigerationTranscriticalSystem_Impl::removeAllTemplate<RefrigerationCompressor>(modelObjectList);
  }

  std::vector<RefrigerationCompressor> RefrigerationTranscriticalSystem_Impl::lowPressureCompressors() const {
    return RefrigerationTranscriticalSystem_Impl::listTemplate<RefrigerationCompressor>( lowPressureCompressorList() );
  }

  boost::optional<ModelObjectList> RefrigerationTranscriticalSystem_Impl::mediumTemperatureRefrigeratedCaseAndWalkInList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_TranscriticalSystemFields::MediumTemperatureRefrigeratedCaseAndWalkInListName);
  }

  boost::optional<ModelObjectList> RefrigerationTranscriticalSystem_Impl::lowTemperatureRefrigeratedCaseAndWalkInList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_TranscriticalSystemFields::LowTemperatureRefrigeratedCaseAndWalkInListName);
  }

  boost::optional<RefrigerationGasCoolerAirCooled> RefrigerationTranscriticalSystem_Impl::refrigerationGasCooler() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationGasCoolerAirCooled>(OS_Refrigeration_TranscriticalSystemFields::RefrigerationGasCoolerName);
  }

  boost::optional<ModelObjectList> RefrigerationTranscriticalSystem_Impl::highPressureCompressorList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_TranscriticalSystemFields::HighPressureCompressorListName);
  }

  boost::optional<ModelObjectList> RefrigerationTranscriticalSystem_Impl::lowPressureCompressorList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Refrigeration_TranscriticalSystemFields::LowPressureCompressorListName);
  }

  double RefrigerationTranscriticalSystem_Impl::receiverPressure() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_TranscriticalSystemFields::ReceiverPressure,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationTranscriticalSystem_Impl::isReceiverPressureDefaulted() const {
    return isEmpty(OS_Refrigeration_TranscriticalSystemFields::ReceiverPressure);
  }

  double RefrigerationTranscriticalSystem_Impl::subcoolerEffectiveness() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_TranscriticalSystemFields::SubcoolerEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationTranscriticalSystem_Impl::isSubcoolerEffectivenessDefaulted() const {
    return isEmpty(OS_Refrigeration_TranscriticalSystemFields::SubcoolerEffectiveness);
  }

  std::string RefrigerationTranscriticalSystem_Impl::refrigerationSystemWorkingFluidType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_TranscriticalSystemFields::RefrigerationSystemWorkingFluidType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationTranscriticalSystem_Impl::sumUASuctionPipingforMediumTemperatureLoads() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforMediumTemperatureLoads,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationTranscriticalSystem_Impl::isSumUASuctionPipingforMediumTemperatureLoadsDefaulted() const {
    return isEmpty(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforMediumTemperatureLoads);
  }

  boost::optional<ThermalZone> RefrigerationTranscriticalSystem_Impl::mediumTemperatureSuctionPipingZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_TranscriticalSystemFields::MediumTemperatureSuctionPipingZoneName);
  }

  double RefrigerationTranscriticalSystem_Impl::sumUASuctionPipingforLowTemperatureLoads() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforLowTemperatureLoads,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationTranscriticalSystem_Impl::isSumUASuctionPipingforLowTemperatureLoadsDefaulted() const {
    return isEmpty(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforLowTemperatureLoads);
  }

  boost::optional<ThermalZone> RefrigerationTranscriticalSystem_Impl::lowTemperatureSuctionPipingZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Refrigeration_TranscriticalSystemFields::LowTemperatureSuctionPipingZoneName);
  }

  std::string RefrigerationTranscriticalSystem_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_TranscriticalSystemFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationTranscriticalSystem_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_TranscriticalSystemFields::EndUseSubcategory);
  }

  bool RefrigerationTranscriticalSystem_Impl::setMediumTemperatureRefrigeratedCaseAndWalkInList(const ModelObjectList& modelObjectList) {
    bool result = setPointer(OS_Refrigeration_TranscriticalSystemFields::MediumTemperatureRefrigeratedCaseAndWalkInListName, modelObjectList.handle());
    return result;
  }

  bool RefrigerationTranscriticalSystem_Impl::setLowTemperatureRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Refrigeration_TranscriticalSystemFields::LowTemperatureRefrigeratedCaseAndWalkInListName, modelObjectList.get().handle());
    }
    else {
      resetLowTemperatureRefrigeratedCaseAndWalkInList();
      result = true;
    }
    return result;
  }

  void RefrigerationTranscriticalSystem_Impl::resetLowTemperatureRefrigeratedCaseAndWalkInList() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::LowTemperatureRefrigeratedCaseAndWalkInListName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationTranscriticalSystem_Impl::setRefrigerationGasCooler(const RefrigerationGasCoolerAirCooled& refrigerationGasCoolerAirCooled) {
    bool result = setPointer(OS_Refrigeration_TranscriticalSystemFields::RefrigerationGasCoolerName, refrigerationGasCoolerAirCooled.handle());
    return result;
  }

  bool RefrigerationTranscriticalSystem_Impl::setHighPressureCompressorList(const ModelObjectList& modelObjectList) {
    bool result = setPointer(OS_Refrigeration_TranscriticalSystemFields::HighPressureCompressorListName, modelObjectList.handle());
    return result;
  }

  bool RefrigerationTranscriticalSystem_Impl::setLowPressureCompressorList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_Refrigeration_TranscriticalSystemFields::LowPressureCompressorListName, modelObjectList.get().handle());
    }
    else {
      resetLowPressureCompressorList();
      result = true;
    }
    return result;
  }

  void RefrigerationTranscriticalSystem_Impl::resetLowPressureCompressorList() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::LowPressureCompressorListName, "");
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::setReceiverPressure(double receiverPressure) {
    bool result = setDouble(OS_Refrigeration_TranscriticalSystemFields::ReceiverPressure, receiverPressure);
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::resetReceiverPressure() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::ReceiverPressure, "");
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::setSubcoolerEffectiveness(double subcoolerEffectiveness) {
    bool result = setDouble(OS_Refrigeration_TranscriticalSystemFields::SubcoolerEffectiveness, subcoolerEffectiveness);
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::resetSubcoolerEffectiveness() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::SubcoolerEffectiveness, "");
    OS_ASSERT(result);
  }

  bool RefrigerationTranscriticalSystem_Impl::setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType) {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::RefrigerationSystemWorkingFluidType, refrigerationSystemWorkingFluidType);
    return result;
  }

  void RefrigerationTranscriticalSystem_Impl::setSumUASuctionPipingforMediumTemperatureLoads(double sumUASuctionPipingforMediumTemperatureLoads) {
    bool result = setDouble(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforMediumTemperatureLoads, sumUASuctionPipingforMediumTemperatureLoads);
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::resetSumUASuctionPipingforMediumTemperatureLoads() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforMediumTemperatureLoads, "");
    OS_ASSERT(result);
  }

  bool RefrigerationTranscriticalSystem_Impl::setMediumTemperatureSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Refrigeration_TranscriticalSystemFields::MediumTemperatureSuctionPipingZoneName, thermalZone.get().handle());
    }
    else {
      resetMediumTemperatureSuctionPipingZone();
      result = true;
    }
    return result;
  }

  void RefrigerationTranscriticalSystem_Impl::resetMediumTemperatureSuctionPipingZone() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::MediumTemperatureSuctionPipingZoneName, "");
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::setSumUASuctionPipingforLowTemperatureLoads(double sumUASuctionPipingforLowTemperatureLoads) {
    bool result = setDouble(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforLowTemperatureLoads, sumUASuctionPipingforLowTemperatureLoads);
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::resetSumUASuctionPipingforLowTemperatureLoads() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::SumUASuctionPipingforLowTemperatureLoads, "");
    OS_ASSERT(result);
  }

  bool RefrigerationTranscriticalSystem_Impl::setLowTemperatureSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Refrigeration_TranscriticalSystemFields::LowTemperatureSuctionPipingZoneName, thermalZone.get().handle());
    }
    else {
      resetLowTemperatureSuctionPipingZone();
      result = true;
    }
    return result;
  }

  void RefrigerationTranscriticalSystem_Impl::resetLowTemperatureSuctionPipingZone() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::LowTemperatureSuctionPipingZoneName, "");
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationTranscriticalSystem_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_TranscriticalSystemFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationTranscriticalSystem::RefrigerationTranscriticalSystem(const Model& model)
  : ModelObject(RefrigerationTranscriticalSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationTranscriticalSystem_Impl>());

  ModelObjectList mediumTemperatureCaseAndWalkinList = ModelObjectList(model);
  mediumTemperatureCaseAndWalkinList.setName(this->name().get() + " Medium Temperature Case and Walkin List");
  bool ok = getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setMediumTemperatureRefrigeratedCaseAndWalkInList(mediumTemperatureCaseAndWalkinList);
  OS_ASSERT(ok);
  ModelObjectList lowTemperatureCaseAndWalkinList = ModelObjectList(model);
  lowTemperatureCaseAndWalkinList.setName(this->name().get() + " Low Temperature Case and Walkin List");
  ok = getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setLowTemperatureRefrigeratedCaseAndWalkInList(lowTemperatureCaseAndWalkinList);
  OS_ASSERT(ok);
  ModelObjectList highPressureCompressorlist = ModelObjectList(model);
  highPressureCompressorlist.setName(this->name().get() + " High Pressure Compressor List");
  ok = getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setHighPressureCompressorList(highPressureCompressorlist);
  OS_ASSERT(ok);
  ModelObjectList lowPressureCompressorlist = ModelObjectList(model);
  lowPressureCompressorlist.setName(this->name().get() + " Low Pressure Compressor List");
  ok = getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setLowPressureCompressorList(lowPressureCompressorlist);
  OS_ASSERT(ok);

  setReceiverPressure(3500000.0);
  setSubcoolerEffectiveness(0.4);
  ok = setRefrigerationSystemWorkingFluidType("R744");
  OS_ASSERT(ok);
  setSumUASuctionPipingforMediumTemperatureLoads(0.0);
  setSumUASuctionPipingforLowTemperatureLoads(0.0);
}

IddObjectType RefrigerationTranscriticalSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_TranscriticalSystem);
}

std::vector<std::string> RefrigerationTranscriticalSystem::refrigerationSystemWorkingFluidTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_TranscriticalSystemFields::RefrigerationSystemWorkingFluidType);
}

std::vector<IdfObject> RefrigerationTranscriticalSystem::remove() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->remove();
}

ModelObject RefrigerationTranscriticalSystem::clone(Model model) const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->clone(model);
}

std::vector<RefrigerationCase> RefrigerationTranscriticalSystem::mediumTemperatureCases() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureCases();
}

std::vector<RefrigerationCase> RefrigerationTranscriticalSystem::lowTemperatureCases() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureCases();
}

std::vector<RefrigerationWalkIn> RefrigerationTranscriticalSystem::mediumTemperatureWalkins() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureWalkins();
}

std::vector<RefrigerationWalkIn> RefrigerationTranscriticalSystem::lowTemperatureWalkins() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureWalkins();
}

std::vector<RefrigerationCompressor> RefrigerationTranscriticalSystem::highPressureCompressors() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->highPressureCompressors();
}

std::vector<RefrigerationCompressor> RefrigerationTranscriticalSystem::lowPressureCompressors() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->lowPressureCompressors();
}

bool RefrigerationTranscriticalSystem::addMediumTemperatureCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->addMediumTemperatureCase(refrigerationCase);
}

void RefrigerationTranscriticalSystem::removeMediumTemperatureCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeMediumTemperatureCase(refrigerationCase);
}

void RefrigerationTranscriticalSystem::removeAllMediumTemperatureCases() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeAllMediumTemperatureCases();
}

bool RefrigerationTranscriticalSystem::addMediumTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->addMediumTemperatureWalkin(refrigerationWalkin);
}

void RefrigerationTranscriticalSystem::removeMediumTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeMediumTemperatureWalkin(refrigerationWalkin);
}

void RefrigerationTranscriticalSystem::removeAllMediumTemperatureWalkins() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeAllMediumTemperatureWalkins();
}

bool RefrigerationTranscriticalSystem::addLowTemperatureCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->addLowTemperatureCase(refrigerationCase);
}

void RefrigerationTranscriticalSystem::removeLowTemperatureCase(const RefrigerationCase& refrigerationCase) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeLowTemperatureCase(refrigerationCase);
}

void RefrigerationTranscriticalSystem::removeAllLowTemperatureCases() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeAllLowTemperatureCases();
}

bool RefrigerationTranscriticalSystem::addLowTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->addLowTemperatureWalkin(refrigerationWalkin);
}

void RefrigerationTranscriticalSystem::removeLowTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeLowTemperatureWalkin(refrigerationWalkin);
}

void RefrigerationTranscriticalSystem::removeAllLowTemperatureWalkins() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeAllLowTemperatureWalkins();
}

bool RefrigerationTranscriticalSystem::addHighPressureCompressor(const RefrigerationCompressor& refrigerationCompressor) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->addHighPressureCompressor(refrigerationCompressor);
}

void RefrigerationTranscriticalSystem::removeHighPressureCompressor(const RefrigerationCompressor& refrigerationCompressor) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeHighPressureCompressor(refrigerationCompressor);
}

void RefrigerationTranscriticalSystem::removeAllHighPressureCompressors() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeAllHighPressureCompressors();
}

bool RefrigerationTranscriticalSystem::addLowPressureCompressor(const RefrigerationCompressor& refrigerationCompressor) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->addLowPressureCompressor(refrigerationCompressor);
}

void RefrigerationTranscriticalSystem::removeLowPressureCompressor(const RefrigerationCompressor& refrigerationCompressor) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeLowPressureCompressor(refrigerationCompressor);
}

void RefrigerationTranscriticalSystem::removeAllLowPressureCompressors() {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->removeAllLowPressureCompressors();
}

boost::optional<RefrigerationGasCoolerAirCooled> RefrigerationTranscriticalSystem::refrigerationGasCooler() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->refrigerationGasCooler();
}

double RefrigerationTranscriticalSystem::receiverPressure() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->receiverPressure();
}

bool RefrigerationTranscriticalSystem::isReceiverPressureDefaulted() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->isReceiverPressureDefaulted();
}

double RefrigerationTranscriticalSystem::subcoolerEffectiveness() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->subcoolerEffectiveness();
}

bool RefrigerationTranscriticalSystem::isSubcoolerEffectivenessDefaulted() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->isSubcoolerEffectivenessDefaulted();
}

std::string RefrigerationTranscriticalSystem::refrigerationSystemWorkingFluidType() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->refrigerationSystemWorkingFluidType();
}

double RefrigerationTranscriticalSystem::sumUASuctionPipingforMediumTemperatureLoads() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->sumUASuctionPipingforMediumTemperatureLoads();
}

bool RefrigerationTranscriticalSystem::isSumUASuctionPipingforMediumTemperatureLoadsDefaulted() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->isSumUASuctionPipingforMediumTemperatureLoadsDefaulted();
}

boost::optional<ThermalZone> RefrigerationTranscriticalSystem::mediumTemperatureSuctionPipingZone() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->mediumTemperatureSuctionPipingZone();
}

double RefrigerationTranscriticalSystem::sumUASuctionPipingforLowTemperatureLoads() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->sumUASuctionPipingforLowTemperatureLoads();
}

bool RefrigerationTranscriticalSystem::isSumUASuctionPipingforLowTemperatureLoadsDefaulted() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->isSumUASuctionPipingforLowTemperatureLoadsDefaulted();
}

boost::optional<ThermalZone> RefrigerationTranscriticalSystem::lowTemperatureSuctionPipingZone() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->lowTemperatureSuctionPipingZone();
}

std::string RefrigerationTranscriticalSystem::endUseSubcategory() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->endUseSubcategory();
}

bool RefrigerationTranscriticalSystem::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->isEndUseSubcategoryDefaulted();
}

bool RefrigerationTranscriticalSystem::setRefrigerationGasCooler(const RefrigerationGasCoolerAirCooled& refrigerationGasCoolerAirCooled) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setRefrigerationGasCooler(refrigerationGasCoolerAirCooled);
}

void RefrigerationTranscriticalSystem::setReceiverPressure(double receiverPressure) {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setReceiverPressure(receiverPressure);
}

void RefrigerationTranscriticalSystem::resetReceiverPressure() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetReceiverPressure();
}

void RefrigerationTranscriticalSystem::setSubcoolerEffectiveness(double subcoolerEffectiveness) {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setSubcoolerEffectiveness(subcoolerEffectiveness);
}

void RefrigerationTranscriticalSystem::resetSubcoolerEffectiveness() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetSubcoolerEffectiveness();
}

bool RefrigerationTranscriticalSystem::setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setRefrigerationSystemWorkingFluidType(refrigerationSystemWorkingFluidType);
}

void RefrigerationTranscriticalSystem::setSumUASuctionPipingforMediumTemperatureLoads(double sumUASuctionPipingforMediumTemperatureLoads) {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setSumUASuctionPipingforMediumTemperatureLoads(sumUASuctionPipingforMediumTemperatureLoads);
}

void RefrigerationTranscriticalSystem::resetSumUASuctionPipingforMediumTemperatureLoads() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetSumUASuctionPipingforMediumTemperatureLoads();
}

bool RefrigerationTranscriticalSystem::setMediumTemperatureSuctionPipingZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setMediumTemperatureSuctionPipingZone(thermalZone);
}

void RefrigerationTranscriticalSystem::resetMediumTemperatureSuctionPipingZone() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetMediumTemperatureSuctionPipingZone();
}

void RefrigerationTranscriticalSystem::setSumUASuctionPipingforLowTemperatureLoads(double sumUASuctionPipingforLowTemperatureLoads) {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setSumUASuctionPipingforLowTemperatureLoads(sumUASuctionPipingforLowTemperatureLoads);
}

void RefrigerationTranscriticalSystem::resetSumUASuctionPipingforLowTemperatureLoads() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetSumUASuctionPipingforLowTemperatureLoads();
}

bool RefrigerationTranscriticalSystem::setLowTemperatureSuctionPipingZone(const ThermalZone& thermalZone) {
  return getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setLowTemperatureSuctionPipingZone(thermalZone);
}

void RefrigerationTranscriticalSystem::resetLowTemperatureSuctionPipingZone() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetLowTemperatureSuctionPipingZone();
}

void RefrigerationTranscriticalSystem::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationTranscriticalSystem::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationTranscriticalSystem_Impl>()->resetEndUseSubcategory();
}

/// @cond
RefrigerationTranscriticalSystem::RefrigerationTranscriticalSystem(std::shared_ptr<detail::RefrigerationTranscriticalSystem_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

