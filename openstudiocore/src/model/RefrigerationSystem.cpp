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

// TODO: Check the following class names against object getters and setters.
#include <model/ModelObjectLists.hpp>
#include <model/ModelObjectLists_Impl.hpp>
#include <model/RefrigerationAllTypesCondenser.hpp>
#include <model/RefrigerationAllTypesCondenser_Impl.hpp>
#include <model/Fluid.hpp>
#include <model/Fluid_Impl.hpp>
#include <model/RefrigerationSubcooler.hpp>
#include <model/RefrigerationSubcooler_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

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
    BOOST_ASSERT(idfObject.iddObject().type() == RefrigerationSystem::iddObjectType());
  }

  RefrigerationSystem_Impl::RefrigerationSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == RefrigerationSystem::iddObjectType());
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

  boost::optional<ModelObjectLists> RefrigerationSystem_Impl::refrigeratedCaseAndWalkInList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectLists>(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName);
  }

  boost::optional<ModelObjectLists> RefrigerationSystem_Impl::refrigerationTransferLoadList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectLists>(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName);
  }

  RefrigerationAllTypesCondenser RefrigerationSystem_Impl::refrigerationCondenser() const {
    boost::optional<RefrigerationAllTypesCondenser> value = optionalRefrigerationCondenser();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Refrigeration Condenser attached.");
    }
    return value.get();
  }

  ModelObjectLists RefrigerationSystem_Impl::compressorList() const {
    boost::optional<ModelObjectLists> value = optionalCompressorList();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Compressor List attached.");
    }
    return value.get();
  }

  double RefrigerationSystem_Impl::minimumCondensingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  Fluid RefrigerationSystem_Impl::refrigerationSystemWorkingFluidType() const {
    boost::optional<Fluid> value = optionalRefrigerationSystemWorkingFluidType();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Refrigeration System Working Fluid Type attached.");
    }
    return value.get();
  }

  std::string RefrigerationSystem_Impl::suctionTemperatureControlType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isSuctionTemperatureControlTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::SuctionTemperatureControlType);
  }

  boost::optional<RefrigerationSubcooler> RefrigerationSystem_Impl::mechanicalSubcooler() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationSubcooler>(OS_Refrigeration_SystemFields::MechanicalSubcoolerName);
  }

  boost::optional<RefrigerationSubcooler> RefrigerationSystem_Impl::liquidSuctionHeatExchangerSubcooler() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationSubcooler>(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName);
  }

  double RefrigerationSystem_Impl::sumUASuctionPiping() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping,true);
    BOOST_ASSERT(value);
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
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::EndUseSubcategory);
  }

  std::string RefrigerationSystem_Impl::numberofCompressorStages() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::NumberofCompressorStages,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isNumberofCompressorStagesDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::NumberofCompressorStages);
  }

  std::string RefrigerationSystem_Impl::intercoolerType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_SystemFields::IntercoolerType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isIntercoolerTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::IntercoolerType);
  }

  double RefrigerationSystem_Impl::shellandCoilIntercoolerEffectiveness() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool RefrigerationSystem_Impl::isShellandCoilIntercoolerEffectivenessDefaulted() const {
    return isEmpty(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness);
  }

  boost::optional<ModelObjectLists> RefrigerationSystem_Impl::highStageCompressorList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectLists>(OS_Refrigeration_SystemFields::HighStageCompressorListName);
  }

  bool RefrigerationSystem_Impl::setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectLists>& modelObjectLists) {
    bool result(false);
    if (modelObjectLists) {
      result = setPointer(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, modelObjectLists.get().handle());
    }
    else {
      resetRefrigeratedCaseAndWalkInList();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetRefrigeratedCaseAndWalkInList() {
    bool result = setString(OS_Refrigeration_SystemFields::RefrigeratedCaseAndWalkInListName, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setRefrigerationTransferLoadList(const boost::optional<ModelObjectLists>& modelObjectLists) {
    bool result(false);
    if (modelObjectLists) {
      result = setPointer(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName, modelObjectLists.get().handle());
    }
    else {
      resetRefrigerationTransferLoadList();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetRefrigerationTransferLoadList() {
    bool result = setString(OS_Refrigeration_SystemFields::RefrigerationTransferLoadListName, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setRefrigerationCondenser(const RefrigerationAllTypesCondenser& refrigerationAllTypesCondenser) {
    bool result = setPointer(OS_Refrigeration_SystemFields::RefrigerationCondenserName, refrigerationAllTypesCondenser.handle());
    return result;
  }

  bool RefrigerationSystem_Impl::setCompressorList(const ModelObjectLists& modelObjectLists) {
    bool result = setPointer(OS_Refrigeration_SystemFields::CompressorListName, modelObjectLists.handle());
    return result;
  }

  void RefrigerationSystem_Impl::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
    bool result = setDouble(OS_Refrigeration_SystemFields::MinimumCondensingTemperature, minimumCondensingTemperature);
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setRefrigerationSystemWorkingFluidType(const Fluid& fluid) {
    bool result = setPointer(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType, fluid.handle());
    return result;
  }

  bool RefrigerationSystem_Impl::setSuctionTemperatureControlType(std::string suctionTemperatureControlType) {
    bool result = setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, suctionTemperatureControlType);
    return result;
  }

  void RefrigerationSystem_Impl::resetSuctionTemperatureControlType() {
    bool result = setString(OS_Refrigeration_SystemFields::SuctionTemperatureControlType, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setMechanicalSubcooler(const boost::optional<RefrigerationSubcooler>& refrigerationSubcooler) {
    bool result(false);
    if (refrigerationSubcooler) {
      result = setPointer(OS_Refrigeration_SystemFields::MechanicalSubcoolerName, refrigerationSubcooler.get().handle());
    }
    else {
      resetMechanicalSubcooler();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetMechanicalSubcooler() {
    bool result = setString(OS_Refrigeration_SystemFields::MechanicalSubcoolerName, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setLiquidSuctionHeatExchangerSubcooler(const boost::optional<RefrigerationSubcooler>& refrigerationSubcooler) {
    bool result(false);
    if (refrigerationSubcooler) {
      result = setPointer(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName, refrigerationSubcooler.get().handle());
    }
    else {
      resetLiquidSuctionHeatExchangerSubcooler();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetLiquidSuctionHeatExchangerSubcooler() {
    bool result = setString(OS_Refrigeration_SystemFields::LiquidSuctionHeatExchangerSubcoolerName, "");
    BOOST_ASSERT(result);
  }

  void RefrigerationSystem_Impl::setSumUASuctionPiping(double sumUASuctionPiping) {
    bool result = setDouble(OS_Refrigeration_SystemFields::SumUASuctionPiping, sumUASuctionPiping);
    BOOST_ASSERT(result);
  }

  void RefrigerationSystem_Impl::resetSumUASuctionPiping() {
    bool result = setString(OS_Refrigeration_SystemFields::SumUASuctionPiping, "");
    BOOST_ASSERT(result);
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
    BOOST_ASSERT(result);
  }

  void RefrigerationSystem_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_SystemFields::EndUseSubcategory, endUseSubcategory);
    BOOST_ASSERT(result);
  }

  void RefrigerationSystem_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_SystemFields::EndUseSubcategory, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setNumberofCompressorStages(std::string numberofCompressorStages) {
    bool result = setString(OS_Refrigeration_SystemFields::NumberofCompressorStages, numberofCompressorStages);
    return result;
  }

  void RefrigerationSystem_Impl::resetNumberofCompressorStages() {
    bool result = setString(OS_Refrigeration_SystemFields::NumberofCompressorStages, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setIntercoolerType(std::string intercoolerType) {
    bool result = setString(OS_Refrigeration_SystemFields::IntercoolerType, intercoolerType);
    return result;
  }

  void RefrigerationSystem_Impl::resetIntercoolerType() {
    bool result = setString(OS_Refrigeration_SystemFields::IntercoolerType, "");
    BOOST_ASSERT(result);
  }

  void RefrigerationSystem_Impl::setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness) {
    bool result = setDouble(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, shellandCoilIntercoolerEffectiveness);
    BOOST_ASSERT(result);
  }

  void RefrigerationSystem_Impl::resetShellandCoilIntercoolerEffectiveness() {
    bool result = setString(OS_Refrigeration_SystemFields::ShellandCoilIntercoolerEffectiveness, "");
    BOOST_ASSERT(result);
  }

  bool RefrigerationSystem_Impl::setHighStageCompressorList(const boost::optional<ModelObjectLists>& modelObjectLists) {
    bool result(false);
    if (modelObjectLists) {
      result = setPointer(OS_Refrigeration_SystemFields::HighStageCompressorListName, modelObjectLists.get().handle());
    }
    else {
      resetHighStageCompressorList();
      result = true;
    }
    return result;
  }

  void RefrigerationSystem_Impl::resetHighStageCompressorList() {
    bool result = setString(OS_Refrigeration_SystemFields::HighStageCompressorListName, "");
    BOOST_ASSERT(result);
  }

  boost::optional<RefrigerationAllTypesCondenser> RefrigerationSystem_Impl::optionalRefrigerationCondenser() const {
    return getObject<ModelObject>().getModelObjectTarget<RefrigerationAllTypesCondenser>(OS_Refrigeration_SystemFields::RefrigerationCondenserName);
  }

  boost::optional<ModelObjectLists> RefrigerationSystem_Impl::optionalCompressorList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectLists>(OS_Refrigeration_SystemFields::CompressorListName);
  }

  boost::optional<Fluid> RefrigerationSystem_Impl::optionalRefrigerationSystemWorkingFluidType() const {
    return getObject<ModelObject>().getModelObjectTarget<Fluid>(OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType);
  }

} // detail

RefrigerationSystem::RefrigerationSystem(const Model& model)
  : ModelObject(RefrigerationSystem::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::RefrigerationSystem_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Refrigeration_SystemFields::RefrigerationCondenserName
  //     OS_Refrigeration_SystemFields::CompressorListName
  //     OS_Refrigeration_SystemFields::RefrigerationSystemWorkingFluidType
  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);
  // ok = setRefrigerationCondenser();
  BOOST_ASSERT(ok);
  // ok = setCompressorList();
  BOOST_ASSERT(ok);
  // setMinimumCondensingTemperature();
  // ok = setRefrigerationSystemWorkingFluidType();
  BOOST_ASSERT(ok);
}

IddObjectType RefrigerationSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_System);
}

std::vector<std::string> RefrigerationSystem::suctionTemperatureControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SystemFields::SuctionTemperatureControlType);
}

std::vector<std::string> RefrigerationSystem::numberofCompressorStagesValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SystemFields::NumberofCompressorStages);
}

std::vector<std::string> RefrigerationSystem::intercoolerTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_SystemFields::IntercoolerType);
}

boost::optional<ModelObjectLists> RefrigerationSystem::refrigeratedCaseAndWalkInList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigeratedCaseAndWalkInList();
}

boost::optional<ModelObjectLists> RefrigerationSystem::refrigerationTransferLoadList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationTransferLoadList();
}

RefrigerationAllTypesCondenser RefrigerationSystem::refrigerationCondenser() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationCondenser();
}

ModelObjectLists RefrigerationSystem::compressorList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->compressorList();
}

double RefrigerationSystem::minimumCondensingTemperature() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->minimumCondensingTemperature();
}

Fluid RefrigerationSystem::refrigerationSystemWorkingFluidType() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->refrigerationSystemWorkingFluidType();
}

std::string RefrigerationSystem::suctionTemperatureControlType() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->suctionTemperatureControlType();
}

bool RefrigerationSystem::isSuctionTemperatureControlTypeDefaulted() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->isSuctionTemperatureControlTypeDefaulted();
}

boost::optional<RefrigerationSubcooler> RefrigerationSystem::mechanicalSubcooler() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->mechanicalSubcooler();
}

boost::optional<RefrigerationSubcooler> RefrigerationSystem::liquidSuctionHeatExchangerSubcooler() const {
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

boost::optional<ModelObjectLists> RefrigerationSystem::highStageCompressorList() const {
  return getImpl<detail::RefrigerationSystem_Impl>()->highStageCompressorList();
}

bool RefrigerationSystem::setRefrigeratedCaseAndWalkInList(const ModelObjectLists& modelObjectLists) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigeratedCaseAndWalkInList(modelObjectLists);
}

void RefrigerationSystem::resetRefrigeratedCaseAndWalkInList() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetRefrigeratedCaseAndWalkInList();
}

bool RefrigerationSystem::setRefrigerationTransferLoadList(const ModelObjectLists& modelObjectLists) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationTransferLoadList(modelObjectLists);
}

void RefrigerationSystem::resetRefrigerationTransferLoadList() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetRefrigerationTransferLoadList();
}

bool RefrigerationSystem::setRefrigerationCondenser(const RefrigerationAllTypesCondenser& refrigerationAllTypesCondenser) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationCondenser(refrigerationAllTypesCondenser);
}

bool RefrigerationSystem::setCompressorList(const ModelObjectLists& modelObjectLists) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setCompressorList(modelObjectLists);
}

void RefrigerationSystem::setMinimumCondensingTemperature(double minimumCondensingTemperature) {
  getImpl<detail::RefrigerationSystem_Impl>()->setMinimumCondensingTemperature(minimumCondensingTemperature);
}

bool RefrigerationSystem::setRefrigerationSystemWorkingFluidType(const Fluid& fluid) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setRefrigerationSystemWorkingFluidType(fluid);
}

bool RefrigerationSystem::setSuctionTemperatureControlType(std::string suctionTemperatureControlType) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setSuctionTemperatureControlType(suctionTemperatureControlType);
}

void RefrigerationSystem::resetSuctionTemperatureControlType() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetSuctionTemperatureControlType();
}

bool RefrigerationSystem::setMechanicalSubcooler(const RefrigerationSubcooler& refrigerationSubcooler) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setMechanicalSubcooler(refrigerationSubcooler);
}

void RefrigerationSystem::resetMechanicalSubcooler() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetMechanicalSubcooler();
}

bool RefrigerationSystem::setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcooler& refrigerationSubcooler) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setLiquidSuctionHeatExchangerSubcooler(refrigerationSubcooler);
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

bool RefrigerationSystem::setNumberofCompressorStages(std::string numberofCompressorStages) {
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

bool RefrigerationSystem::setHighStageCompressorList(const ModelObjectLists& modelObjectLists) {
  return getImpl<detail::RefrigerationSystem_Impl>()->setHighStageCompressorList(modelObjectLists);
}

void RefrigerationSystem::resetHighStageCompressorList() {
  getImpl<detail::RefrigerationSystem_Impl>()->resetHighStageCompressorList();
}

/// @cond
RefrigerationSystem::RefrigerationSystem(boost::shared_ptr<detail::RefrigerationSystem_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

