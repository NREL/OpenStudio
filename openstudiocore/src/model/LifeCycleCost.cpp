/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/LifeCycleCost.hpp>
#include <model/LifeCycleCost_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/LightsDefinition.hpp>
#include <model/LightsDefinition_Impl.hpp>
#include <model/LuminaireDefinition.hpp>
#include <model/LuminaireDefinition_Impl.hpp>
#include <model/ElectricEquipmentDefinition.hpp>
#include <model/ElectricEquipmentDefinition_Impl.hpp>
#include <model/GasEquipmentDefinition.hpp>
#include <model/GasEquipmentDefinition_Impl.hpp>
#include <model/HotWaterEquipmentDefinition.hpp>
#include <model/HotWaterEquipmentDefinition_Impl.hpp>
#include <model/SteamEquipmentDefinition.hpp>
#include <model/SteamEquipmentDefinition_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_LifeCycleCost_FieldEnums.hxx>

#include <boost/algorithm/string.hpp>

#include <utilities/core/Assert.hpp>

#include <typeinfo>
#include <iostream>

using namespace std;

using std::string;
using std::type_info;

namespace openstudio {
namespace model  {
namespace detail {

LifeCycleCost_Impl::LifeCycleCost_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ModelObject_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == LifeCycleCost::iddObjectType());
}

LifeCycleCost_Impl::LifeCycleCost_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == LifeCycleCost::iddObjectType());
}

LifeCycleCost_Impl::LifeCycleCost_Impl(const LifeCycleCost_Impl& other,Model_Impl* model,bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
}

std::string LifeCycleCost_Impl::category() const
{
  boost::optional<std::string> value = getString(OS_LifeCycleCostFields::Category,true);
  OS_ASSERT(value);
  return value.get();
}

std::string LifeCycleCost_Impl::itemType() const
{
  boost::optional<std::string> value = getString(OS_LifeCycleCostFields::ItemType,true);
  OS_ASSERT(value);
  return value.get();
}

ModelObject LifeCycleCost_Impl::item() const
{
  boost::optional<ModelObject> value = getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_LifeCycleCostFields::ItemName);
  OS_ASSERT(value);
  return value.get();
}

double LifeCycleCost_Impl::cost() const
{
  boost::optional<double> value = getDouble(OS_LifeCycleCostFields::Cost,true);
  OS_ASSERT(value);
  return value.get();
}

std::vector<std::string> LifeCycleCost_Impl::validCostUnitsValues() const
{
  std::vector<std::string> result;
  std::string itemType = this->itemType();
  if (istringEqual("Construction", itemType)){
    result.push_back("CostPerArea");
  }else if (istringEqual("Building", itemType)){
    result.push_back("CostPerEach");
    result.push_back("CostPerArea");
    result.push_back("CostPerThermalZone");
  }else if (istringEqual("Space", itemType)){
    result.push_back("CostPerEach");
    result.push_back("CostPerArea");
  }else if (istringEqual("ThermalZone", itemType)){
    result.push_back("CostPerEach");
    result.push_back("CostPerArea");
  }else if (istringEqual("AirLoop", itemType)){
    result.push_back("CostPerEach");
    result.push_back("CostPerThermalZone");
  }else if (istringEqual("PlantLoop", itemType)){
    result.push_back("CostPerEach");
  }else if (istringEqual("ZoneHVAC", itemType)){
    result.push_back("CostPerEach");
  }else if (istringEqual("Lights", itemType)){
    result.push_back("CostPerEach");
    result.push_back("CostPerArea");
  }else if (istringEqual("Luminaire", itemType)){
    result.push_back("CostPerEach");
  }else if (istringEqual("Equipment", itemType)){
    result.push_back("CostPerEach");
    result.push_back("CostPerArea");
  }else if (istringEqual("HVACComponent", itemType)){
    result.push_back("CostPerEach");
  }else if (istringEqual("ZoneHVACComponent", itemType)){
    result.push_back("CostPerEach");
  }else{
    result.push_back("CostPerEach");
  }
  
  return result;
}

std::string LifeCycleCost_Impl::costUnits() const
{
  boost::optional<std::string> value = getString(OS_LifeCycleCostFields::CostUnits,true);
  OS_ASSERT(value);
  return value.get();
}

int LifeCycleCost_Impl::yearsFromStart() const
{
  boost::optional<int> value = getInt(OS_LifeCycleCostFields::YearsfromStart,true);
  OS_ASSERT(value);
  return value.get();
}

bool LifeCycleCost_Impl::isYearsFromStartDefaulted() const
{
  return isEmpty(OS_LifeCycleCostFields::YearsfromStart);
}

int LifeCycleCost_Impl::monthsFromStart() const
{
  boost::optional<int> value = getInt(OS_LifeCycleCostFields::MonthsfromStart,true);
  OS_ASSERT(value);
  return value.get();
}

bool LifeCycleCost_Impl::isMonthsFromStartDefaulted() const
{
  return isEmpty(OS_LifeCycleCostFields::MonthsfromStart);
}

int LifeCycleCost_Impl::repeatPeriodYears() const
{
  boost::optional<int> value = getInt(OS_LifeCycleCostFields::RepeatPeriodYears,true);
  OS_ASSERT(value);
  return value.get();
}

bool LifeCycleCost_Impl::isRepeatPeriodYearsDefaulted() const
{
  return isEmpty(OS_LifeCycleCostFields::RepeatPeriodYears);
}

int LifeCycleCost_Impl::repeatPeriodMonths() const
{
  boost::optional<int> value = getInt(OS_LifeCycleCostFields::RepeatPeriodMonths,true);
  OS_ASSERT(value);
  return value.get();
}

bool LifeCycleCost_Impl::isRepeatPeriodMonthsDefaulted() const
{
  return isEmpty(OS_LifeCycleCostFields::RepeatPeriodYears);
}

bool LifeCycleCost_Impl::setCategory(const std::string& category)
{
  return setString(OS_LifeCycleCostFields::Category, category);
}

bool LifeCycleCost_Impl::setCost(double cost)
{
  return setDouble(OS_LifeCycleCostFields::Cost, cost);
}

bool LifeCycleCost_Impl::setCostUnits(const std::string& costUnits)
{
  std::vector<std::string> validCostUnitsValues = this->validCostUnitsValues();
  if (std::find(validCostUnitsValues.begin(), validCostUnitsValues.end(), costUnits) == validCostUnitsValues.end()){
    return false;
  }

  return setString(OS_LifeCycleCostFields::CostUnits, costUnits);
}

bool LifeCycleCost_Impl::setYearsFromStart(int yearsFromStart)
{
  return setInt(OS_LifeCycleCostFields::YearsfromStart, yearsFromStart);
}

void LifeCycleCost_Impl::resetYearsFromStart()
{
  bool test = setString(OS_LifeCycleCostFields::YearsfromStart, "");
  OS_ASSERT(test);
}

bool LifeCycleCost_Impl::setMonthsFromStart(int monthsFromStart)
{
  return setInt(OS_LifeCycleCostFields::MonthsfromStart, monthsFromStart);
}

void LifeCycleCost_Impl::resetMonthsFromStart()
{
  bool test = setString(OS_LifeCycleCostFields::MonthsfromStart, "");
  OS_ASSERT(test);
}

bool LifeCycleCost_Impl::setRepeatPeriodYears(int repeatPeriodYears)
{
  return setInt(OS_LifeCycleCostFields::RepeatPeriodYears, repeatPeriodYears);
}

void LifeCycleCost_Impl::resetRepeatPeriodYears()
{
  bool test = setString(OS_LifeCycleCostFields::RepeatPeriodYears, "");
  OS_ASSERT(test);
}

bool LifeCycleCost_Impl::setRepeatPeriodMonths(int repeatPeriodMonths)
{
  return setInt(OS_LifeCycleCostFields::RepeatPeriodMonths, repeatPeriodMonths);
}

void LifeCycleCost_Impl::resetRepeatPeriodMonths()
{
  bool test = setString(OS_LifeCycleCostFields::RepeatPeriodMonths, "");
  OS_ASSERT(test);
}

double LifeCycleCost_Impl::totalCost() const
{
  double result = 0;
  std::string costUnits = this->costUnits();

  if (istringEqual("CostPerEach", costUnits)){
    boost::optional<int> costedQuantity = this->costedQuantity();
    if (costedQuantity){
      result = costedQuantity.get() * this->cost();
    }
  }else if (istringEqual("CostPerArea", costUnits)){
    boost::optional<double> costedArea = this->costedArea();
    if (costedArea){
      result = costedArea.get() * this->cost();
    }
  }else if (istringEqual("CostPerThermalZone", costUnits)){
    boost::optional<int> costedThermalZones = this->costedThermalZones();
    if (costedThermalZones){
      result = costedThermalZones.get() * this->cost();
    }
  }

  return result;
}

bool LifeCycleCost_Impl::convertToCostPerEach()
{
  std::string costUnits = this->costUnits();
  if (istringEqual("CostPerEach", costUnits)){
    return true;
  }else if (istringEqual("CostPerArea", costUnits)){
    boost::optional<double> costedArea = this->costedArea();
    if (!costedArea){
      return false;
    }
  }else if (istringEqual("CostPerThermalZone", costUnits)){
    boost::optional<int> costedThermalZones = this->costedThermalZones();
    if (!costedThermalZones){
      return false;
    }
  }

  double totalCost = this->totalCost();
  
  if (!this->setCostUnits("CostPerEach")){
    return false;
  }
  
  bool test = this->setCost(totalCost);
  OS_ASSERT(test);
  return true;
}

boost::optional<int> LifeCycleCost_Impl::costedQuantity() const
{ 
  boost::optional<int> result;

  std::string costUnits = this->costUnits();
  if (!istringEqual("CostPerEach", costUnits)){
    return boost::none;
  }

  ModelObject modelObject = item();

  if (modelObject.optionalCast<ConstructionBase>()){
    // no quantity defined

  }else if (modelObject.optionalCast<Building>()){
    result = 1;

  }else if (modelObject.optionalCast<Space>()){
    result = modelObject.cast<Space>().multiplier();

  }else if (modelObject.optionalCast<ThermalZone>()){
    result = modelObject.cast<ThermalZone>().multiplier();

  }else if (modelObject.optionalCast<AirLoopHVAC>()){
    result = 1;

  }else if (modelObject.optionalCast<PlantLoop>()){
    result = 1;

  }else if (modelObject.optionalCast<HVACComponent>()){
    result = 1;

  }else if (modelObject.optionalCast<ZoneHVACComponent>()){
    result = 1;

  }else if (modelObject.optionalCast<SpaceLoadDefinition>()){
    // covers lights, equipment, etc
    result = modelObject.cast<SpaceLoadDefinition>().quantity();

  }

  return result;
}

boost::optional<double> LifeCycleCost_Impl::costedArea() const
{
  boost::optional<double> result;

  std::string costUnits = this->costUnits();
  if (!istringEqual("CostPerArea", costUnits)){
    return boost::none;
  }

  ModelObject modelObject = item();

  if (modelObject.optionalCast<ConstructionBase>()){
    result = modelObject.cast<ConstructionBase>().getNetArea(); 

  }else if (modelObject.optionalCast<Building>()){
    result = modelObject.cast<Building>().floorArea();

  }else if (modelObject.optionalCast<Space>()){
    result = modelObject.cast<Space>().multiplier() * modelObject.cast<Space>().floorArea();

  }else if (modelObject.optionalCast<ThermalZone>()){
    result = modelObject.cast<ThermalZone>().multiplier() * modelObject.cast<ThermalZone>().floorArea();

  }else if (modelObject.optionalCast<AirLoopHVAC>()){
    // no area defined

  }else if (modelObject.optionalCast<PlantLoop>()){
    // no area defined

  }else if (modelObject.optionalCast<HVACComponent>()){
    // no area defined

  }else if (modelObject.optionalCast<ZoneHVACComponent>()){
    // no area defined

  }else if (modelObject.optionalCast<SpaceLoadDefinition>()){
    // covers lights, equipment, etc
    result = modelObject.cast<SpaceLoadDefinition>().floorArea();

  }

  return result;
}

boost::optional<int>  LifeCycleCost_Impl::costedThermalZones() const
{ 
  boost::optional<int> result;

  std::string costUnits = this->costUnits();
  if (!istringEqual("CostPerThermalZone", costUnits)){
    return boost::none;
  }

  ModelObject modelObject = item();

  if (modelObject.optionalCast<ConstructionBase>()){
    // number of thermal zones not defined

  }else if (modelObject.optionalCast<Building>()){
    result = 0;
    BOOST_FOREACH(const ThermalZone& thermalZone, this->model().getModelObjects<ThermalZone>()){
      result = result.get() + thermalZone.multiplier();
    }

  }else if (modelObject.optionalCast<Space>()){
    // number of thermal zones not defined

  }else if (modelObject.optionalCast<ThermalZone>()){
    // number of thermal zones not defined

  }else if (modelObject.optionalCast<AirLoopHVAC>()){
    result = 0;
    BOOST_FOREACH(const ThermalZone& thermalZone, modelObject.cast<AirLoopHVAC>().thermalZones()){
      result = result.get() + thermalZone.multiplier();
    }

  }else if (modelObject.optionalCast<PlantLoop>()){
    // number of thermal zones not defined

  }else if (modelObject.optionalCast<ZoneHVACComponent>()){
    // number of thermal zones not defined

  }else if (modelObject.optionalCast<SpaceLoadDefinition>()){
    // number of thermal zones not defined

  }

  return result;
}

// Get all output variable names that could be associated with this object.
const std::vector<std::string>& LifeCycleCost_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

} // detail


/// LifeCycleCost
LifeCycleCost::LifeCycleCost(const ModelObject& modelObject)
  : ModelObject(LifeCycleCost::iddObjectType(), modelObject.model())
{
  OS_ASSERT(getImpl<detail::LifeCycleCost_Impl>());
  bool test = getImpl<detail::LifeCycleCost_Impl>()->setPointer(OS_LifeCycleCostFields::ItemName, modelObject.handle());
  OS_ASSERT(test);

  if (modelObject.optionalCast<ConstructionBase>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Construction");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<Building>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Building");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<Space>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Space");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<ThermalZone>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "ThermalZone");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<AirLoopHVAC>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "AirLoop");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<PlantLoop>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "PlantLoop");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<ZoneHVACComponent>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "ZoneHVAC");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<LightsDefinition>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Lights");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<LuminaireDefinition>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Luminaire");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<ElectricEquipmentDefinition>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Equipment");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<GasEquipmentDefinition>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Equipment");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<HotWaterEquipmentDefinition>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Equipment");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<SteamEquipmentDefinition>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "Equipment");
    OS_ASSERT(test);
  }else if (modelObject.optionalCast<HVACComponent>()){
    test = setString(OS_LifeCycleCostFields::ItemType, "HVACComponent");
    OS_ASSERT(test);
  }else{
    this->remove();
    throw openstudio::Exception("Cannot add cost to unknown model object type '" + modelObject.iddObject().name() + "'");
  }

  test = this->setCost(0.0);
  OS_ASSERT(test);

  std::vector<std::string> validCostUnitsValues = this->validCostUnitsValues();
  OS_ASSERT(!validCostUnitsValues.empty());
  test = this->setCostUnits(validCostUnitsValues[0]);
  OS_ASSERT(test);

}

// constructor
LifeCycleCost::LifeCycleCost(boost::shared_ptr<detail::LifeCycleCost_Impl> impl)
  : ModelObject(impl)
{}

IddObjectType LifeCycleCost::iddObjectType() 
{
  IddObjectType result(IddObjectType::OS_LifeCycleCost);
  return result;
}

std::vector<std::string> LifeCycleCost::validCategoryValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_LifeCycleCostFields::Category);
}

std::vector<std::string> LifeCycleCost::validItemTypeValues() 
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_LifeCycleCostFields::ItemType);
}

boost::optional<LifeCycleCost> LifeCycleCost::createLifeCycleCost(const std::string& name, const ModelObject& modelObject, double cost, const std::string& costUnits, const std::string& category, int repeatPeriodYears, int yearsFromStart)
{
  boost::optional<LifeCycleCost> result;

  try{
    result = LifeCycleCost(modelObject);
  }catch(const std::exception&){
    return boost::none;
  }

  bool test = true;
  test = test && result->setName(name);
  test = test && result->setCost(cost);
  test = test && result->setCostUnits(costUnits);
  test = test && result->setCategory(category);
  if (repeatPeriodYears > 0){
    test = test && result->setRepeatPeriodYears(repeatPeriodYears);
  }
  if (yearsFromStart > 0){
    test = test && result->setYearsFromStart(yearsFromStart);
  }

  if (!test){
    result->remove();
    return boost::none;
  }

  return result;
}


std::string LifeCycleCost::category() const {
  return getImpl<detail::LifeCycleCost_Impl>()->category();
}

std::string LifeCycleCost::itemType() const {
  return getImpl<detail::LifeCycleCost_Impl>()->itemType();
}

ModelObject LifeCycleCost::item() const {
  return getImpl<detail::LifeCycleCost_Impl>()->item();
}

double LifeCycleCost::cost() const {
  return getImpl<detail::LifeCycleCost_Impl>()->cost();
}

std::vector<std::string> LifeCycleCost::validCostUnitsValues() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->validCostUnitsValues();
}

std::string LifeCycleCost::costUnits() const 
{
  return getImpl<detail::LifeCycleCost_Impl>()->costUnits();
}

int LifeCycleCost::yearsFromStart() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->yearsFromStart();
}

bool LifeCycleCost::isYearsFromStartDefaulted() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->isYearsFromStartDefaulted();
}

int LifeCycleCost::monthsFromStart() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->monthsFromStart();
}

bool LifeCycleCost::isMonthsFromStartDefaulted() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->isMonthsFromStartDefaulted();
}

int LifeCycleCost::repeatPeriodYears() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->repeatPeriodYears();
}

bool LifeCycleCost::isRepeatPeriodYearsDefaulted() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->isRepeatPeriodYearsDefaulted();
}

int LifeCycleCost::repeatPeriodMonths() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->repeatPeriodMonths();
}

bool LifeCycleCost::isRepeatPeriodMonthsDefaulted() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->isRepeatPeriodMonthsDefaulted();
}

bool LifeCycleCost::setCategory(const std::string& category){
  return getImpl<detail::LifeCycleCost_Impl>()->setCategory(category);
}

bool LifeCycleCost::setCost(double cost) {
  return getImpl<detail::LifeCycleCost_Impl>()->setCost(cost);
}

bool LifeCycleCost::setCostUnits(const std::string& costUnits) {
  return getImpl<detail::LifeCycleCost_Impl>()->setCostUnits(costUnits);
}

bool LifeCycleCost::setYearsFromStart(int yearsFromStart)
{
  return getImpl<detail::LifeCycleCost_Impl>()->setYearsFromStart(yearsFromStart);
}

void LifeCycleCost::resetYearsFromStart()
{
  getImpl<detail::LifeCycleCost_Impl>()->resetYearsFromStart();
}

bool LifeCycleCost::setMonthsFromStart(int monthsFromStart)
{
  return getImpl<detail::LifeCycleCost_Impl>()->setMonthsFromStart(monthsFromStart);
}

void LifeCycleCost::resetMonthsFromStart()
{
  getImpl<detail::LifeCycleCost_Impl>()->resetMonthsFromStart();
}

bool LifeCycleCost::setRepeatPeriodYears(int repeatPeriodYears)
{
  return getImpl<detail::LifeCycleCost_Impl>()->setRepeatPeriodYears(repeatPeriodYears);
}

void LifeCycleCost::resetRepeatPeriodYears()
{
  getImpl<detail::LifeCycleCost_Impl>()->resetRepeatPeriodYears();
}

bool LifeCycleCost::setRepeatPeriodMonths(int repeatPeriodMonths)
{
  return getImpl<detail::LifeCycleCost_Impl>()->setRepeatPeriodMonths(repeatPeriodMonths);
}

void LifeCycleCost::resetRepeatPeriodMonths()
{
  getImpl<detail::LifeCycleCost_Impl>()->resetRepeatPeriodMonths();
}

double LifeCycleCost::totalCost() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->totalCost();
}

bool LifeCycleCost::convertToCostPerEach() 
{
  return getImpl<detail::LifeCycleCost_Impl>()->convertToCostPerEach();
}

boost::optional<int> LifeCycleCost::costedQuantity() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->costedQuantity();
}

boost::optional<double> LifeCycleCost::costedArea() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->costedArea();
}

boost::optional<int> LifeCycleCost::costedThermalZones() const
{
  return getImpl<detail::LifeCycleCost_Impl>()->costedThermalZones();
}

} // model
} // openstudio
