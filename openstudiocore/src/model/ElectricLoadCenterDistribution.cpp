/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"

#include "Model.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Generator.hpp"
#include "Generator_Impl.hpp"
#include "Inverter.hpp"
#include "Inverter_Impl.hpp"
//#include "ElecricalStorage.hpp"
//#include "ElecricalStorage_Impl.hpp"
//#include "Transformer.hpp"
//#include "Transformer_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ElectricLoadCenter_Distribution_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ElectricLoadCenterDistribution_Impl::ElectricLoadCenterDistribution_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricLoadCenterDistribution::iddObjectType());
  }

  ElectricLoadCenterDistribution_Impl::ElectricLoadCenterDistribution_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricLoadCenterDistribution::iddObjectType());
  }

  ElectricLoadCenterDistribution_Impl::ElectricLoadCenterDistribution_Impl(const ElectricLoadCenterDistribution_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricLoadCenterDistribution_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Electric Load Center Requested Electric Power");
      result.push_back("Electric Load Center Produced Electric Power");
      result.push_back("Electric Load Center Produced Electric Energy");
      result.push_back("Electric Load Center Produced Thermal Rate");
      result.push_back("Electric Load Center Produced Thermal Energy");
    }
    return result;
  }

  IddObjectType ElectricLoadCenterDistribution_Impl::iddObjectType() const {
    return ElectricLoadCenterDistribution::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ElectricLoadCenterDistribution_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ElectricLoadCenter_DistributionFields::TrackScheduleNameSchemeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ElectricLoadCenterDistribution","Track  Scheme"));
    }
    return result;
  }

  std::vector<ModelObject> ElectricLoadCenterDistribution_Impl::children() const
  {
    std::vector<ModelObject> result;
    result.push_back(generatorModelObjectList());
    return result;
  }

  std::vector<openstudio::IdfObject> ElectricLoadCenterDistribution_Impl::remove()
  {
    return ParentObject_Impl::remove();
  }

  std::vector<IddObjectType> ElectricLoadCenterDistribution_Impl::allowableChildTypes() const
  {
    return std::vector<IddObjectType>();
  }

  ModelObject ElectricLoadCenterDistribution_Impl::clone(Model model) const
  {
    // should this also clone the generators, inverters, and storage?
    return ParentObject_Impl::clone(model);
  }

  std::vector<Generator> ElectricLoadCenterDistribution_Impl::generators() const {
    std::vector<Generator> result;
    for (auto modelObject : generatorModelObjectList().modelObjects()){
      if (modelObject.optionalCast<Generator>()){
        result.push_back(modelObject.cast<Generator>());
      }
    }
    return result;
  }

  std::string ElectricLoadCenterDistribution_Impl::generatorOperationSchemeType() const {
    boost::optional<std::string> value = getString(OS_ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterDistribution_Impl::isGeneratorOperationSchemeTypeDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType);
  }

  //boost::optional<double> ElectricLoadCenterDistribution_Impl::demandLimitSchemePurchasedElectricDemandLimit() const {
  //  return getDouble(OS_ElectricLoadCenter_DistributionFields::DemandLimitSchemePurchasedElectricDemandLimit,true);
  //}

  //boost::optional<Schedule> ElectricLoadCenterDistribution_Impl::trackScheduleSchemeSchedule() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ElectricLoadCenter_DistributionFields::TrackScheduleNameSchemeScheduleName);
  //}

  //boost::optional<std::string> ElectricLoadCenterDistribution_Impl::trackMeterSchemeMeterName() const {
  //  return getString(OS_ElectricLoadCenter_DistributionFields::TrackMeterSchemeMeterName,true);
  //}

  std::string ElectricLoadCenterDistribution_Impl::electricalBussType() const {
    boost::optional<std::string> value = getString(OS_ElectricLoadCenter_DistributionFields::ElectricalBussType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ElectricLoadCenterDistribution_Impl::isElectricalBussTypeDefaulted() const {
    return isEmpty(OS_ElectricLoadCenter_DistributionFields::ElectricalBussType);
  }

  boost::optional<Inverter> ElectricLoadCenterDistribution_Impl::inverter() const {
    return getObject<ModelObject>().getModelObjectTarget<Inverter>(OS_ElectricLoadCenter_DistributionFields::InverterObjectName);
  }

  //boost::optional<ElecricalStorage> ElectricLoadCenterDistribution_Impl::electricalStorage() const {
  //  return getObject<ModelObject>().getModelObjectTarget<ElecricalStorage>(OS_ElectricLoadCenter_DistributionFields::ElectricalStorageObjectName);
  //}

  //boost::optional<Transformer> ElectricLoadCenterDistribution_Impl::transformer() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Transformer>(OS_ElectricLoadCenter_DistributionFields::TransformerObjectName);
  //}

  bool ElectricLoadCenterDistribution_Impl::addGenerator(const Generator& generator)
  {
    if (!isElectricalBussTypeDefaulted()){
      // todo: check that generator is ok on this buss type
    }

    ModelObjectList list = generatorModelObjectList();
    if (list.hasModelObject(generator)){
      return false;
    }

    // previous load center
    boost::optional<ElectricLoadCenterDistribution> previous = generator.electricLoadCenterDistribution();
    if (previous){
      previous->removeGenerator(generator);
    }

    // DLM: may have problems with signals here if inverter is temporarily on two load centers
    bool result = generatorModelObjectList().addModelObject(generator);

    if (result){
      if (isElectricalBussTypeDefaulted()){
        // TODO: update bus type
      }
    } else{
      if (previous){
        bool test = previous->addGenerator(generator);
        OS_ASSERT(test);
      }
    }

    return result;
  }

  bool ElectricLoadCenterDistribution_Impl::removeGenerator(const Generator& generator)
  {
    ModelObjectList list = generatorModelObjectList();
    if (!list.hasModelObject(generator)){
      return false;
    }

    generatorModelObjectList().removeModelObject(generator);

    if (list.size() == 0){
      resetElectricalBussType();
    }

    return true;
  }

  void ElectricLoadCenterDistribution_Impl::resetGenerators()
  {
    generatorModelObjectList().removeAllModelObjects();
    resetElectricalBussType();
  }

  bool ElectricLoadCenterDistribution_Impl::setGeneratorOperationSchemeType(const std::string& generatorOperationSchemeType) {
    return setString(OS_ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType, generatorOperationSchemeType);
  }

  void ElectricLoadCenterDistribution_Impl::resetGeneratorOperationSchemeType() {
    bool result = setString(OS_ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType, "");
    OS_ASSERT(result);
  }

  //void ElectricLoadCenterDistribution_Impl::setDemandLimitSchemePurchasedElectricDemandLimit(double demandLimitSchemePurchasedElectricDemandLimit) {
  //  bool result = setDouble(OS_ElectricLoadCenter_DistributionFields::DemandLimitSchemePurchasedElectricDemandLimit, demandLimitSchemePurchasedElectricDemandLimit);
  //  OS_ASSERT(result);
  //}

  //void ElectricLoadCenterDistribution_Impl::resetDemandLimitSchemePurchasedElectricDemandLimit() {
  //  bool result = setString(OS_ElectricLoadCenter_DistributionFields::DemandLimitSchemePurchasedElectricDemandLimit, "");
  // OS_ASSERT(result);
  //}

  //bool ElectricLoadCenterDistribution_Impl::setTrackScheduleSchemeSchedule(Schedule& schedule) {
  //  bool result = setSchedule(OS_ElectricLoadCenter_DistributionFields::TrackScheduleNameSchemeScheduleName,
  //                            "ElectricLoadCenterDistribution",
  //                            "Track  Scheme",
  //                            schedule);
  //  return result;
  //}

  //void ElectricLoadCenterDistribution_Impl::resetTrackScheduleSchemeSchedule() {
  //  bool result = setString(OS_ElectricLoadCenter_DistributionFields::TrackScheduleNameSchemeScheduleName, "");
  //  OS_ASSERT(result);
  //}

  //bool ElectricLoadCenterDistribution_Impl::setTrackMeterSchemeMeterName(const std::string& trackMeterSchemeMeterName) {
  //  return setString(OS_ElectricLoadCenter_DistributionFields::TrackMeterSchemeMeterName, trackMeterSchemeMeterName);
  //}

  //void ElectricLoadCenterDistribution_Impl::resetTrackMeterSchemeMeterName() {
  //  bool result = setString(OS_ElectricLoadCenter_DistributionFields::TrackMeterSchemeMeterName, "");
  //  OS_ASSERT(result);
  //}

  bool ElectricLoadCenterDistribution_Impl::setElectricalBussType(const std::string& electricalBussType) {
    bool result = setString(OS_ElectricLoadCenter_DistributionFields::ElectricalBussType, electricalBussType);
    return result;
  }

  void ElectricLoadCenterDistribution_Impl::resetElectricalBussType() {
    bool result = setString(OS_ElectricLoadCenter_DistributionFields::ElectricalBussType, "");
    OS_ASSERT(result);
  }

  bool ElectricLoadCenterDistribution_Impl::setInverter(const Inverter& inverter) {

    if (!isElectricalBussTypeDefaulted()){
      // todo: check that inverter is ok on this buss type
    }

    // previous load center
    boost::optional<ElectricLoadCenterDistribution> previous = inverter.electricLoadCenterDistribution();
    if (previous){
      previous->resetInverter();
    }

    // DLM: may have problems with signals here if inverter is temporarily on two load centers
    bool result = setPointer(OS_ElectricLoadCenter_DistributionFields::InverterObjectName, inverter.handle());
 
    if (result){
      
      // TODO: update bus type

    } else{

      if (previous){
        bool test = previous->setInverter(inverter);
        OS_ASSERT(test);
      }

    }
    
    return result;
  }

  void ElectricLoadCenterDistribution_Impl::resetInverter() {
    bool result = setString(OS_ElectricLoadCenter_DistributionFields::InverterObjectName, "");

    // TODO: update bus type
    OS_ASSERT(result);
  }

  //bool ElectricLoadCenterDistribution_Impl::setElectricalStorage(const ElectricalStorage& electricalStorage) {
  //  return setPointer(OS_ElectricLoadCenter_DistributionFields::ElectricalStorageObjectName, electricalStorage.handle());
  //}

  //void ElectricLoadCenterDistribution_Impl::resetElectricalStorage() {
  //  bool result = setString(OS_ElectricLoadCenter_DistributionFields::ElectricalStorageObjectName, "");
  //  OS_ASSERT(result);
  //}

  //bool ElectricLoadCenterDistribution_Impl::setTransformer(const Transformer& transformer) {
  //  return setPointer(OS_ElectricLoadCenter_DistributionFields::TransformerObjectName, transformer.handle());
  //}

  //void ElectricLoadCenterDistribution_Impl::resetTransformer() {
  //  bool result = setString(OS_ElectricLoadCenter_DistributionFields::TransformerObjectName, "");
  //  OS_ASSERT(result);
  //}

  ModelObjectList ElectricLoadCenterDistribution_Impl::generatorModelObjectList()  const {
    boost::optional<ModelObjectList> result = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_ElectricLoadCenter_DistributionFields::GeneratorListName);
    OS_ASSERT(result);
    return result.get();
  }

} // detail

ElectricLoadCenterDistribution::ElectricLoadCenterDistribution(const Model& model)
  : ParentObject(ElectricLoadCenterDistribution::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricLoadCenterDistribution_Impl>());

  ModelObjectList generatorList(model);
  bool ok = setPointer(OS_ElectricLoadCenter_DistributionFields::GeneratorListName, generatorList.handle());
  OS_ASSERT(ok);
}

IddObjectType ElectricLoadCenterDistribution::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ElectricLoadCenter_Distribution);
}

std::vector<std::string> ElectricLoadCenterDistribution::generatorOperationSchemeTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType);
}

std::vector<std::string> ElectricLoadCenterDistribution::electricalBussTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricLoadCenter_DistributionFields::ElectricalBussType);
}

std::vector<Generator> ElectricLoadCenterDistribution::generators() const {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->generators();
}

std::string ElectricLoadCenterDistribution::generatorOperationSchemeType() const {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->generatorOperationSchemeType();
}

bool ElectricLoadCenterDistribution::isGeneratorOperationSchemeTypeDefaulted() const {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->isGeneratorOperationSchemeTypeDefaulted();
}

//boost::optional<double> ElectricLoadCenterDistribution::demandLimitSchemePurchasedElectricDemandLimit() const {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->demandLimitSchemePurchasedElectricDemandLimit();
//}

//boost::optional<Schedule> ElectricLoadCenterDistribution::trackScheduleSchemeSchedule() const {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->trackScheduleSchemeSchedule();
//}

//boost::optional<std::string> ElectricLoadCenterDistribution::trackMeterSchemeMeterName() const {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->trackMeterSchemeMeterName();
//}

std::string ElectricLoadCenterDistribution::electricalBussType() const {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->electricalBussType();
}

bool ElectricLoadCenterDistribution::isElectricalBussTypeDefaulted() const {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->isElectricalBussTypeDefaulted();
}

boost::optional<Inverter> ElectricLoadCenterDistribution::inverter() const {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->inverter();
}

//boost::optional<ElecricalStorage> ElectricLoadCenterDistribution::elecricalStorage() const {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->elecricalStorage();
//}

//boost::optional<Transformer> ElectricLoadCenterDistribution::transformer() const {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->transformer();
//}

bool ElectricLoadCenterDistribution::addGenerator(const Generator& generator){
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->addGenerator(generator);
}

bool ElectricLoadCenterDistribution::removeGenerator(const Generator& generator){
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->removeGenerator(generator);
}

void ElectricLoadCenterDistribution::resetGenerators(){
  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetGenerators();
}

bool ElectricLoadCenterDistribution::setGeneratorOperationSchemeType(const std::string& generatorOperationSchemeType) {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setGeneratorOperationSchemeType(generatorOperationSchemeType);
}

void ElectricLoadCenterDistribution::resetGeneratorOperationSchemeType() {
  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetGeneratorOperationSchemeType();
}

//void ElectricLoadCenterDistribution::setDemandLimitSchemePurchasedElectricDemandLimit(double demandLimitSchemePurchasedElectricDemandLimit) {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setDemandLimitSchemePurchasedElectricDemandLimit(demandLimitSchemePurchasedElectricDemandLimit);
//}

//void ElectricLoadCenterDistribution::resetDemandLimitSchemePurchasedElectricDemandLimit() {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetDemandLimitSchemePurchasedElectricDemandLimit();
//}

//bool ElectricLoadCenterDistribution::setTrackScheduleSchemeSchedule(Schedule& schedule) {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setTrackScheduleSchemeSchedule(schedule);
//}

//void ElectricLoadCenterDistribution::resetTrackScheduleSchemeSchedule() {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetTrackScheduleSchemeSchedule();
//}

//bool ElectricLoadCenterDistribution::setTrackMeterSchemeMeterName(const std::string& trackMeterSchemeMeterName) {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setTrackMeterSchemeMeterName(trackMeterSchemeMeterName);
//}

//void ElectricLoadCenterDistribution::resetTrackMeterSchemeMeterName() {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetTrackMeterSchemeMeterName();
//}

//bool ElectricLoadCenterDistribution::setElectricalBussType(const std::string& electricalBussType) {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setElectricalBussType(electricalBussType);
//}

//void ElectricLoadCenterDistribution::resetElectricalBussType() {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetElectricalBussType();
//}

bool ElectricLoadCenterDistribution::setInverter(const Inverter& inverter) {
  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setInverter(inverter);
}

void ElectricLoadCenterDistribution::resetInverter() {
  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetInverter();
}

//bool ElectricLoadCenterDistribution::setElecricalStorage(const ElecricalStorage& elecricalStorage) {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setElectricalStorageObject(elecricalStorage);
//}

//void ElectricLoadCenterDistribution::resetElecricalStorage() {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetElecricalStorage();
//}

//bool ElectricLoadCenterDistribution::setTransformer(const Transformer& transformer) {
//  return getImpl<detail::ElectricLoadCenterDistribution_Impl>()->setTransformer(transformer);
//}

//void ElectricLoadCenterDistribution::resetTransformer() {
//  getImpl<detail::ElectricLoadCenterDistribution_Impl>()->resetTransformer();
//}

/// @cond
ElectricLoadCenterDistribution::ElectricLoadCenterDistribution(std::shared_ptr<detail::ElectricLoadCenterDistribution_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

