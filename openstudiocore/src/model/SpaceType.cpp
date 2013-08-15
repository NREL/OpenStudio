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

#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>

#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/DefaultScheduleSet_Impl.hpp>
#include <model/RenderingColor.hpp>
#include <model/RenderingColor_Impl.hpp>

#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/InternalMass.hpp>
#include <model/InternalMass_Impl.hpp>
#include <model/People.hpp>
#include <model/People_Impl.hpp>
#include <model/PeopleDefinition.hpp>
#include <model/PeopleDefinition_Impl.hpp>
#include <model/Lights.hpp>
#include <model/Lights_Impl.hpp>
#include <model/LightsDefinition.hpp>
#include <model/LightsDefinition_Impl.hpp>
#include <model/Luminaire.hpp>
#include <model/Luminaire_Impl.hpp>
#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipment_Impl.hpp>
#include <model/ElectricEquipmentDefinition.hpp>
#include <model/ElectricEquipmentDefinition_Impl.hpp>
#include <model/GasEquipment.hpp>
#include <model/GasEquipment_Impl.hpp>
#include <model/GasEquipmentDefinition.hpp>
#include <model/GasEquipmentDefinition_Impl.hpp>
#include <model/HotWaterEquipment.hpp>
#include <model/HotWaterEquipment_Impl.hpp>
#include <model/SteamEquipment.hpp>
#include <model/SteamEquipment_Impl.hpp>
#include <model/OtherEquipment.hpp>
#include <model/OtherEquipment_Impl.hpp>
#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/DesignSpecificationOutdoorAir_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Space.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SpaceType_FieldEnums.hxx>

#include <utilities/math/FloatCompare.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SpaceType_Impl::SpaceType_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SpaceType::iddObjectType());
  }

  SpaceType_Impl::SpaceType_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SpaceType::iddObjectType());
  }

  SpaceType_Impl::SpaceType_Impl(const SpaceType_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SpaceType_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SpaceType_Impl::iddObjectType() const {
    return SpaceType::iddObjectType();
  }

  std::vector<ModelObject> SpaceType_Impl::children() const
  {
    std::vector<ModelObject> result;

    // internal mass
    InternalMassVector internalMass = this->internalMass();
    result.insert(result.end(),internalMass.begin(),internalMass.end());

    // people
    PeopleVector people = this->people();
    result.insert(result.end(), people.begin(), people.end());

    // lights
    LightsVector lights = this->lights();
    result.insert(result.end(), lights.begin(), lights.end());

    // luminaires
    LuminaireVector luminaires = this->luminaires();
    result.insert(result.end(), luminaires.begin(), luminaires.end());

    // electric equipment
    ElectricEquipmentVector electricEquipment = this->electricEquipment();
    result.insert(result.end(), electricEquipment.begin(), electricEquipment.end());

    // gas equipment
    GasEquipmentVector gasEquipment = this->gasEquipment();
    result.insert(result.end(), gasEquipment.begin(), gasEquipment.end());

    // hot water equipment
    HotWaterEquipmentVector hotWaterEquipment = this->hotWaterEquipment();
    result.insert(result.end(), hotWaterEquipment.begin(), hotWaterEquipment.end());

    // steam equipment
    SteamEquipmentVector steamEquipment = this->steamEquipment();
    result.insert(result.end(), steamEquipment.begin(), steamEquipment.end());

    // other equipment
    OtherEquipmentVector otherEquipment = this->otherEquipment();
    result.insert(result.end(), otherEquipment.begin(), otherEquipment.end());

    // SpaceInfiltration_DesignFlowRate
    SpaceInfiltrationDesignFlowRateVector spaceInfiltrationDesignFlowRates = this->spaceInfiltrationDesignFlowRates();
    result.insert(result.end(), spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end());

    // SpaceInfiltration_EffectiveLeakageArea
    SpaceInfiltrationEffectiveLeakageAreaVector spaceInfiltrationEffectiveLeakageAreas = this->spaceInfiltrationEffectiveLeakageAreas();
    result.insert(result.end(), spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end());

    return result;
  }

  boost::optional<DefaultConstructionSet> SpaceType_Impl::defaultConstructionSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultConstructionSet>(OS_SpaceTypeFields::DefaultConstructionSetName);
  }

  bool SpaceType_Impl::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
  {
    return setPointer(OS_SpaceTypeFields::DefaultConstructionSetName, defaultConstructionSet.handle());
  }

  void SpaceType_Impl::resetDefaultConstructionSet()
  {
    setString(OS_SpaceTypeFields::DefaultConstructionSetName, "");
  }

  boost::optional<DefaultScheduleSet> SpaceType_Impl::defaultScheduleSet() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DefaultScheduleSet>(OS_SpaceTypeFields::DefaultScheduleSetName);
  }

  boost::optional<Schedule> SpaceType_Impl::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const
  {
    boost::optional<Schedule> result;
    boost::optional<DefaultScheduleSet> defaultScheduleSet;
    boost::optional<Building> building;
    boost::optional<SpaceType> spaceType;

    // first check this object
    defaultScheduleSet = this->defaultScheduleSet();
    if (defaultScheduleSet){
      result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
      if (result){
        return result;
      }
    }

    // then check building
    building = this->model().building();
    if (building){
      defaultScheduleSet = building->defaultScheduleSet();
      if (defaultScheduleSet){
        result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
        if (result){
          return result;
        }
      }

      // then check building's space type
      spaceType = building->spaceType();
      if (spaceType){
        defaultScheduleSet = spaceType->defaultScheduleSet();
        if (defaultScheduleSet){
          result = defaultScheduleSet->getDefaultSchedule(defaultScheduleType);
          if (result){
            return result;
          }
        }
      }
    }

    return boost::none;
  }

  bool SpaceType_Impl::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
  {
    return setPointer(OS_SpaceTypeFields::DefaultScheduleSetName, defaultScheduleSet.handle());
  }

  void SpaceType_Impl::resetDefaultScheduleSet()
  {
    setString(OS_SpaceTypeFields::DefaultScheduleSetName, "");
  }
 
  boost::optional<RenderingColor> SpaceType_Impl::renderingColor() const
  {
    return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(OS_SpaceTypeFields::GroupRenderingName);
  }

  bool SpaceType_Impl::setRenderingColor(const RenderingColor& renderingColor)
  {
    return setPointer(OS_SpaceTypeFields::GroupRenderingName, renderingColor.handle());
  }

  void SpaceType_Impl::resetRenderingColor()
  {
    setString(OS_SpaceTypeFields::GroupRenderingName, "");
  }

  std::vector<Space> SpaceType_Impl::spaces() const
  {
    std::vector<Space> result;

    Handle handle = this->handle();
    BOOST_FOREACH(const Space& space, this->model().getModelObjects<Space>()){
      OptionalSpaceType spaceType = space.spaceType();
      if (spaceType){
        if (spaceType->handle() == handle){
          result.push_back(space);
        }
      }
    }

    return result;
  }

  InternalMassVector SpaceType_Impl::internalMass() const
  {
    return getObject<ModelObject>().getModelObjectSources<InternalMass>(
      InternalMass::iddObjectType());
  }

  PeopleVector SpaceType_Impl::people() const
  {
    return getObject<ModelObject>().getModelObjectSources<People>(People::iddObjectType());
  }

  LightsVector SpaceType_Impl::lights() const
  {
    return getObject<ModelObject>().getModelObjectSources<Lights>(Lights::iddObjectType());
  }

  LuminaireVector SpaceType_Impl::luminaires() const
  {
    return getObject<ModelObject>().getModelObjectSources<Luminaire>(Luminaire::iddObjectType());
  }

  ElectricEquipmentVector SpaceType_Impl::electricEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<ElectricEquipment>(
      ElectricEquipment::iddObjectType());
  }

  GasEquipmentVector SpaceType_Impl::gasEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<GasEquipment>(
      GasEquipment::iddObjectType());
  }

  HotWaterEquipmentVector SpaceType_Impl::hotWaterEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<HotWaterEquipment>(
      HotWaterEquipment::iddObjectType());
  }

  SteamEquipmentVector SpaceType_Impl::steamEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<SteamEquipment>(
      SteamEquipment::iddObjectType());
  }

  OtherEquipmentVector SpaceType_Impl::otherEquipment() const
  {
    return getObject<ModelObject>().getModelObjectSources<OtherEquipment>(
      OtherEquipment::iddObjectType());
  }

  SpaceInfiltrationDesignFlowRateVector SpaceType_Impl::spaceInfiltrationDesignFlowRates() const
  {
    return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationDesignFlowRate>(
      SpaceInfiltrationDesignFlowRate::iddObjectType());
  }

  SpaceInfiltrationEffectiveLeakageAreaVector SpaceType_Impl::spaceInfiltrationEffectiveLeakageAreas() const
  {
    return getObject<ModelObject>().getModelObjectSources<SpaceInfiltrationEffectiveLeakageArea>(
      SpaceInfiltrationEffectiveLeakageArea::iddObjectType());
  }

  boost::optional<DesignSpecificationOutdoorAir> SpaceType_Impl::designSpecificationOutdoorAir() const
  {
    return getObject<ModelObject>().getModelObjectTarget<DesignSpecificationOutdoorAir>(
      OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName);
  }

  bool SpaceType_Impl::isDesignSpecificationOutdoorAirDefaulted() const
  {
    return isEmpty(OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName);
  }

  bool SpaceType_Impl::setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir)
  {
    return setPointer(OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir.handle());
  }

  void SpaceType_Impl::resetDesignSpecificationOutdoorAir()
  {
    bool test = setString(OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName, "");
    OS_ASSERT(test);
  }

  void SpaceType_Impl::hardApplySpaceLoadSchedules()
  {
    BOOST_FOREACH(ModelObject child, this->children()){
      if (child.optionalCast<SpaceLoad>()){
        child.cast<SpaceLoad>().hardApplySchedules();
      }
    }
  }

  boost::optional<double> SpaceType_Impl::peoplePerFloorArea() const {
    double result(0.0);
    BOOST_FOREACH(const People& peopleObject,people()) {
      OptionalDouble temp = peopleObject.peoplePerFloorArea();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setPeoplePerFloorArea(boost::optional<double> peoplePerFloorArea)
  {
    if (!peoplePerFloorArea) {
      return false;
    }
    OptionalPeople templatePeople;
    PeopleVector myPeople = people();
    if (!myPeople.empty()) {
      templatePeople = myPeople[0];
    }
    return setPeoplePerFloorArea(*peoplePerFloorArea,templatePeople);
  }

  bool SpaceType_Impl::setPeoplePerFloorArea(double peoplePerFloorArea, 
                                                  const boost::optional<People>& templatePeople)
  {
    if (peoplePerFloorArea < 0.0) {
      LOG(Error,"SpaceType cannot set peoplePerFloorArea to " << peoplePerFloorArea 
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalPeople myPeople = getMySpaceLoadInstance<People,PeopleDefinition>(templatePeople);
    if (!myPeople) {
      LOG(Error,"The templatePeople object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and people perSpaceFloorArea
    bool ok(true);
    OS_ASSERT(myPeople);
    myPeople->makeUnique();
    ok = myPeople->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myPeople->peopleDefinition().setPeopleperSpaceFloorArea(peoplePerFloorArea);
    OS_ASSERT(ok);
    ok = myPeople->setMultiplier(1); OS_ASSERT(ok);

    // remove all other people
    PeopleVector allMyPeople = people();
    removeAllButOneSpaceLoadInstance<People>(allMyPeople,*myPeople);

    return true;
  }

  boost::optional<double> SpaceType_Impl::spaceFloorAreaPerPerson() const {
    OptionalDouble peoplePerArea = peoplePerFloorArea();
    if (peoplePerArea) {
      return 1.0 / peoplePerArea.get();
    }
    return boost::none;
  }

  bool SpaceType_Impl::setSpaceFloorAreaPerPerson(boost::optional<double> spaceFloorAreaPerPerson) {
    if (!spaceFloorAreaPerPerson) {
      return false;
    }
    OptionalPeople templatePeople;
    PeopleVector myPeople = people();
    if (!myPeople.empty()) {
      templatePeople = myPeople[0];
    }
    return setSpaceFloorAreaPerPerson(*spaceFloorAreaPerPerson,templatePeople);
  }

  bool SpaceType_Impl::setSpaceFloorAreaPerPerson(double spaceFloorAreaPerPerson, 
                                                  const boost::optional<People>& templatePeople) 
  {
    if (lessThanOrEqual(spaceFloorAreaPerPerson,0.0)) {
      LOG(Error,"SpaceType cannot set spaceFloorAreaPerPerson to " << spaceFloorAreaPerPerson 
          << ", the value must be > 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalPeople myPeople = getMySpaceLoadInstance<People,PeopleDefinition>(templatePeople);
    if (!myPeople) {
      LOG(Error,"The templatePeople object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and people perSpaceFloorArea
    bool ok(true);
    OS_ASSERT(myPeople);
    myPeople->makeUnique();
    ok = myPeople->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myPeople->peopleDefinition().setSpaceFloorAreaperPerson(spaceFloorAreaPerPerson);
    OS_ASSERT(ok);
    ok = myPeople->setMultiplier(1); OS_ASSERT(ok);

    // remove all other people
    PeopleVector allMyPeople = people();
    removeAllButOneSpaceLoadInstance<People>(allMyPeople,*myPeople);

    return true;
  }

  double SpaceType_Impl::getNumberOfPeople(double floorArea) const {
    double result(0.0);
    BOOST_FOREACH(const People& peopleObject, people()) {
      result += peopleObject.getNumberOfPeople(floorArea);
    }
    return result;
  }

  double SpaceType_Impl::getPeoplePerFloorArea(double floorArea) const {
    double result(0.0);
    BOOST_FOREACH(const People& peopleObject,people()) {
      result += peopleObject.getPeoplePerFloorArea(floorArea);
    }
    return result;
  }

  double SpaceType_Impl::getFloorAreaPerPerson(double floorArea) const {
    return 1.0 / getPeoplePerFloorArea(floorArea);
  }

  boost::optional<double> SpaceType_Impl::lightingPowerPerFloorArea() const {
    if (!luminaires().empty()) {
      return boost::none;
    }

    double result(0.0);
    BOOST_FOREACH(const Lights& lightsObject, lights()) {
      OptionalDouble temp = lightsObject.powerPerFloorArea();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setLightingPowerPerFloorArea(
      boost::optional<double> lightingPowerPerFloorArea)
  {
    if (!lightingPowerPerFloorArea) {
      return false;
    }
    OptionalLights templateLights;
    LightsVector myLights = lights();
    if (!myLights.empty()) {
      templateLights = myLights[0];
    }
    return setLightingPowerPerFloorArea(*lightingPowerPerFloorArea,templateLights);
  }

  bool SpaceType_Impl::setLightingPowerPerFloorArea(
      double lightingPowerPerFloorArea,const boost::optional<Lights>& templateLights)
  {
    if (lightingPowerPerFloorArea < 0.0) {
      LOG(Error,"SpaceType cannot set lightingPowerPerFloorArea to " 
          << lightingPowerPerFloorArea << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalLights myLights = getMySpaceLoadInstance<Lights,LightsDefinition>(templateLights);
    if (!myLights) {
      LOG(Error,"The templateLights object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and lighting power per space floor area
    bool ok(true);
    OS_ASSERT(myLights);
    myLights->makeUnique();
    ok = myLights->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myLights->lightsDefinition().setWattsperSpaceFloorArea(lightingPowerPerFloorArea);
    OS_ASSERT(ok);
    ok = myLights->setMultiplier(1); OS_ASSERT(ok);

    // remove all other lights
    LightsVector allMyLights = lights();
    removeAllButOneSpaceLoadInstance<Lights>(allMyLights,*myLights);

    // remove all luminaires
    LuminaireVector myLuminaires = luminaires();
    BOOST_FOREACH(Luminaire& luminaire,myLuminaires) {
      luminaire.remove();
    }

    return true;
  }

  boost::optional<double> SpaceType_Impl::lightingPowerPerPerson() const {
    if (!luminaires().empty()) {
      return boost::none;
    }

    double result(0.0);
    BOOST_FOREACH(const Lights& lightsObject, lights()) {
      OptionalDouble temp = lightsObject.powerPerPerson();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setLightingPowerPerPerson(boost::optional<double> lightingPowerPerPerson) {
    if (!lightingPowerPerPerson) {
      return false;
    }
    OptionalLights templateLights;
    LightsVector myLights = lights();
    if (!myLights.empty()) {
      templateLights = myLights[0];
    }
    return setLightingPowerPerPerson(*lightingPowerPerPerson,templateLights);
  }

  bool SpaceType_Impl::setLightingPowerPerPerson(double lightingPowerPerPerson,
                                                 const boost::optional<Lights>& templateLights)
  {
    if (lightingPowerPerPerson < 0.0) {
      LOG(Error,"SpaceType cannot set lightingPowerPerPerson to " << lightingPowerPerPerson
          << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify People and PeopleDefinition object
    OptionalLights myLights = getMySpaceLoadInstance<Lights,LightsDefinition>(templateLights);
    if (!myLights) {
      LOG(Error,"The templateLights object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and lighting power per person
    bool ok(true);
    OS_ASSERT(myLights);
    myLights->makeUnique();
    ok = myLights->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myLights->lightsDefinition().setWattsperPerson(lightingPowerPerPerson);
    OS_ASSERT(ok);
    ok = myLights->setMultiplier(1); OS_ASSERT(ok);

    // remove all other lights
    LightsVector allMyLights = lights();
    removeAllButOneSpaceLoadInstance<Lights>(allMyLights,*myLights);

    // remove all luminaires
    LuminaireVector myLuminaires = luminaires();
    BOOST_FOREACH(Luminaire& luminaire,myLuminaires) {
      luminaire.remove();
    }

    return true;
  }

  double SpaceType_Impl::getLightingPower(double floorArea, double numPeople) const {
    double result(0.0);
    BOOST_FOREACH(const Lights& lightsObject, lights()) {
      result += lightsObject.getLightingPower(floorArea,numPeople);
    }
    BOOST_FOREACH(const Luminaire& luminaire, luminaires()) {
      result += luminaire.lightingPower();
    }
    return result;
  }

  double SpaceType_Impl::getLightingPowerPerFloorArea(double floorArea, 
                                                           double numPeople) const 
  {
    double result(0.0);
    BOOST_FOREACH(const Lights& lightsObject, lights()) {
      result += lightsObject.getPowerPerFloorArea(floorArea,numPeople);
    }
    BOOST_FOREACH(const Luminaire& luminaire, luminaires()) {
      result += luminaire.getPowerPerFloorArea(floorArea);
    }
    return result;
  }

  double SpaceType_Impl::getLightingPowerPerPerson(double floorArea, double numPeople) const {
    double result(0.0); 
    BOOST_FOREACH(const Lights& lightsObject, lights()) {
      result += lightsObject.getPowerPerPerson(floorArea,numPeople);
    }
    BOOST_FOREACH(const Luminaire& luminaire, luminaires()) {
      result += luminaire.getPowerPerPerson(numPeople);
    }
    return result;
  }

  boost::optional<double> SpaceType_Impl::electricEquipmentPowerPerFloorArea() const {
    double result(0.0);
    BOOST_FOREACH(const ElectricEquipment& equipment, electricEquipment()) {
      OptionalDouble temp = equipment.powerPerFloorArea();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setElectricEquipmentPowerPerFloorArea(
      boost::optional<double> electricEquipmentPowerPerFloorArea)
  {
    if (!electricEquipmentPowerPerFloorArea) {
      return false;
    }
    OptionalElectricEquipment templateEquipment;
    ElectricEquipmentVector myEquipment = electricEquipment();
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setElectricEquipmentPowerPerFloorArea(*electricEquipmentPowerPerFloorArea,
                                                      templateEquipment);
  }

  bool SpaceType_Impl::setElectricEquipmentPowerPerFloorArea(
      double electricEquipmentPowerPerFloorArea,
      const boost::optional<ElectricEquipment>& templateElectricEquipment)
  {
    if (electricEquipmentPowerPerFloorArea < 0.0) {
      LOG(Error,"SpaceType cannot set electricEquipmentPowerPerFloorArea " 
          << electricEquipmentPowerPerFloorArea << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify ElectricEquipment and ElectricEquipmentDefinition object
    OptionalElectricEquipment myEquipment = 
        getMySpaceLoadInstance<ElectricEquipment,ElectricEquipmentDefinition>(templateElectricEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateElectricEquipment object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and electric equipment WattsperSpaceFloorArea
    bool ok(true);
    OS_ASSERT(myEquipment);
    myEquipment->makeUnique();
    ok = myEquipment->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myEquipment->electricEquipmentDefinition().setWattsperSpaceFloorArea(electricEquipmentPowerPerFloorArea);
    OS_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); OS_ASSERT(ok);

    // remove all other electric equipment
    ElectricEquipmentVector allMyEquipment = electricEquipment();
    removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment,*myEquipment);

    return true;
  }

  boost::optional<double> SpaceType_Impl::electricEquipmentPowerPerPerson() const {
    double result(0.0);
    BOOST_FOREACH(const ElectricEquipment& equipment, electricEquipment()) {
      OptionalDouble temp = equipment.powerPerPerson();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setElectricEquipmentPowerPerPerson(
      boost::optional<double> electricEquipmentPowerPerPerson)
  {
    if (!electricEquipmentPowerPerPerson) {
      return false;
    }
    OptionalElectricEquipment templateEquipment;
    ElectricEquipmentVector myEquipment = electricEquipment();
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setElectricEquipmentPowerPerPerson(*electricEquipmentPowerPerPerson,
                                              templateEquipment);
  }

  bool SpaceType_Impl::setElectricEquipmentPowerPerPerson(
      double electricEquipmentPowerPerPerson,
      const boost::optional<ElectricEquipment>& templateElectricEquipment)
  {
    if (electricEquipmentPowerPerPerson < 0.0) {
      LOG(Error,"SpaceType cannot set electricEquipmentPowerPerPerson " 
          << electricEquipmentPowerPerPerson << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify ElectricEquipment and ElectricEquipmentDefinition object
    OptionalElectricEquipment myEquipment = 
        getMySpaceLoadInstance<ElectricEquipment,ElectricEquipmentDefinition>(templateElectricEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateElectricEquipment object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and electric equipment WattsperSpaceFloorArea
    bool ok(true);
    OS_ASSERT(myEquipment);
    myEquipment->makeUnique();
    ok = myEquipment->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myEquipment->electricEquipmentDefinition().setWattsperPerson(electricEquipmentPowerPerPerson);
    OS_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); OS_ASSERT(ok);

    // remove all other electric equipment
    ElectricEquipmentVector allMyEquipment = electricEquipment();
    removeAllButOneSpaceLoadInstance<ElectricEquipment>(allMyEquipment,*myEquipment);

    return true;
  }

  double SpaceType_Impl::getElectricEquipmentDesignLevel(double floorArea, double numPeople) const {
    double result(0.0);
    BOOST_FOREACH(const ElectricEquipment& equipment,electricEquipment()) {
      result += equipment.getDesignLevel(floorArea,numPeople);
    }
    return result;
  }

  double SpaceType_Impl::getElectricEquipmentPowerPerFloorArea(double floorArea, 
                                                                    double numPeople) const
  {
    double result(0.0);
    BOOST_FOREACH(const ElectricEquipment& equipment,electricEquipment()) {
      result += equipment.getPowerPerFloorArea(floorArea,numPeople);
    }
    return result;
  }

  double SpaceType_Impl::getElectricEquipmentPowerPerPerson(double floorArea, 
                                                            double numPeople) const
  {
    double result(0.0);
    BOOST_FOREACH(const ElectricEquipment& equipment,electricEquipment()) {
      result += equipment.getPowerPerPerson(floorArea,numPeople);
    }
    return result;
  }

  boost::optional<double> SpaceType_Impl::gasEquipmentPowerPerFloorArea() const {
    double result(0.0);
    BOOST_FOREACH(const GasEquipment& equipment, gasEquipment()) {
      OptionalDouble temp = equipment.powerPerFloorArea();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setGasEquipmentPowerPerFloorArea(
      boost::optional<double> gasEquipmentPowerPerFloorArea)
  {
    if (!gasEquipmentPowerPerFloorArea) {
      return false;
    }
    OptionalGasEquipment templateEquipment;
    GasEquipmentVector myEquipment = gasEquipment();
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setGasEquipmentPowerPerFloorArea(*gasEquipmentPowerPerFloorArea,
                                                 templateEquipment);
  }

  bool SpaceType_Impl::setGasEquipmentPowerPerFloorArea(
      double gasEquipmentPowerPerFloorArea,
      const boost::optional<GasEquipment>& templateGasEquipment)
  {
    if (gasEquipmentPowerPerFloorArea < 0.0) {
      LOG(Error,"SpaceType cannot set gasEquipmentPowerPerFloorArea " 
          << gasEquipmentPowerPerFloorArea << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify GasEquipment and GasEquipmentDefinition object
    OptionalGasEquipment myEquipment = 
        getMySpaceLoadInstance<GasEquipment,GasEquipmentDefinition>(templateGasEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateGasEquipment object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and gas equipment WattsperSpaceFloorArea
    bool ok(true);
    OS_ASSERT(myEquipment);
    myEquipment->makeUnique();
    ok = myEquipment->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myEquipment->gasEquipmentDefinition().setWattsperSpaceFloorArea(gasEquipmentPowerPerFloorArea);
    OS_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); OS_ASSERT(ok);

    // remove all other gas equipment
    GasEquipmentVector allMyEquipment = gasEquipment();
    removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment,*myEquipment);

    return true;
  }

  boost::optional<double> SpaceType_Impl::gasEquipmentPowerPerPerson() const {
    double result(0.0);
    BOOST_FOREACH(const GasEquipment& equipment, gasEquipment()) {
      OptionalDouble temp = equipment.powerPerPerson();
      if (temp) {
        result += temp.get();
      }
      else {
        return boost::none;
      }
    }
    return result;
  }

  bool SpaceType_Impl::setGasEquipmentPowerPerPerson(
      boost::optional<double> gasEquipmentPowerPerPerson)
  {
    if (!gasEquipmentPowerPerPerson) {
      return false;
    }
    OptionalGasEquipment templateEquipment;
    GasEquipmentVector myEquipment = gasEquipment();
    if (!myEquipment.empty()) {
      templateEquipment = myEquipment[0];
    }
    return setGasEquipmentPowerPerPerson(*gasEquipmentPowerPerPerson,templateEquipment);
  }

  bool SpaceType_Impl::setGasEquipmentPowerPerPerson(
      double gasEquipmentPowerPerPerson,
      const boost::optional<GasEquipment>& templateGasEquipment)
  {
    if (gasEquipmentPowerPerPerson < 0.0) {
      LOG(Error,"SpaceType cannot set gasEquipmentPowerPerPerson " 
          << gasEquipmentPowerPerPerson << ", the value must be >= 0.0.");
      return false;
    }

    // create or modify GasEquipment and GasEquipmentDefinition object
    OptionalGasEquipment myEquipment = 
        getMySpaceLoadInstance<GasEquipment,GasEquipmentDefinition>(templateGasEquipment);
    if (!myEquipment) {
      LOG(Error,"The templateGasEquipment object must be in the same Model as this SpaceType.");
      return false;
    }

    // set space type and gas equipment WattsperSpaceFloorArea
    bool ok(true);
    OS_ASSERT(myEquipment);
    myEquipment->makeUnique();
    ok = myEquipment->setSpaceType(getObject<SpaceType>()); OS_ASSERT(ok);
    ok = myEquipment->gasEquipmentDefinition().setWattsperPerson(gasEquipmentPowerPerPerson);
    OS_ASSERT(ok);
    ok = myEquipment->setMultiplier(1); OS_ASSERT(ok);

    // remove all other gas equipment
    GasEquipmentVector allMyEquipment = gasEquipment();
    removeAllButOneSpaceLoadInstance<GasEquipment>(allMyEquipment,*myEquipment);

    return true;
  }

  double SpaceType_Impl::getGasEquipmentDesignLevel(double floorArea, double numPeople) const {
    double result(0.0);
    BOOST_FOREACH(const GasEquipment& equipment,gasEquipment()) {
      result += equipment.getDesignLevel(floorArea,numPeople);
    }
    return result;
  }

  double SpaceType_Impl::getGasEquipmentPowerPerFloorArea(double floorArea, 
                                                               double numPeople) const
  {
    double result(0.0);
    BOOST_FOREACH(const GasEquipment& equipment,gasEquipment()) {
      result += equipment.getPowerPerFloorArea(floorArea,numPeople);
    }
    return result;
  }

  double SpaceType_Impl::getGasEquipmentPowerPerPerson(double floorArea, double numPeople) const {
    double result(0.0);
    BOOST_FOREACH(const GasEquipment& equipment,gasEquipment()) {
      result += equipment.getPowerPerPerson(floorArea,numPeople);
    }
    return result;
  }

  double SpaceType_Impl::floorArea() const
  {
    double result = 0;
    BOOST_FOREACH(const Space& space, this->model().getModelObjects<Space>()){
      boost::optional<SpaceType> spaceType = space.spaceType();
      if (spaceType && spaceType->handle() == this->handle()){
        result += space.multiplier()*space.floorArea();
      }
    }
    return result;
  }

  boost::optional<ModelObject> SpaceType_Impl::defaultConstructionSetAsModelObject() const {
    OptionalModelObject result;
    OptionalDefaultConstructionSet intermediate = defaultConstructionSet();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> SpaceType_Impl::defaultScheduleSetAsModelObject() const {
    OptionalModelObject result;
    OptionalDefaultScheduleSet intermediate = defaultScheduleSet();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> SpaceType_Impl::renderingColorAsModelObject() const {
    OptionalModelObject result;
    OptionalRenderingColor intermediate = renderingColor();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::internalMassAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(internalMass());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::peopleAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(people());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::lightsAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(lights());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::luminairesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(luminaires());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::electricEquipmentAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(electricEquipment());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::gasEquipmentAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(gasEquipment());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::hotWaterEquipmentAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(hotWaterEquipment());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::steamEquipmentAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(steamEquipment());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::otherEquipmentAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(otherEquipment());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::spaceInfiltrationDesignFlowRatesAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(spaceInfiltrationDesignFlowRates());
    return result;
  }

  std::vector<ModelObject> SpaceType_Impl::spaceInfiltrationEffectiveLeakageAreasAsModelObjects() const {
    ModelObjectVector result = castVector<ModelObject>(spaceInfiltrationEffectiveLeakageAreas());
    return result;
  }

  bool SpaceType_Impl::setDefaultConstructionSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDefaultConstructionSet intermediate = modelObject->optionalCast<DefaultConstructionSet>();
      if (intermediate) {
        return setDefaultConstructionSet(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetDefaultConstructionSet();
    }
    return true;
  }

  bool SpaceType_Impl::setDefaultScheduleSetAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDefaultScheduleSet intermediate = modelObject->optionalCast<DefaultScheduleSet>();
      if (intermediate) {
        return setDefaultScheduleSet(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetDefaultScheduleSet();
    }
    return true;
  }

  bool SpaceType_Impl::setRenderingColorAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalRenderingColor intermediate = modelObject->optionalCast<RenderingColor>();
      if (intermediate) {
        return setRenderingColor(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetRenderingColor();
    }
    return true;
  }

  template <typename T, typename TDef>
  boost::optional<T> SpaceType_Impl::getMySpaceLoadInstance(const boost::optional<T>& templateSpaceLoadInstance) {
    boost::optional<T> result;
    if (!templateSpaceLoadInstance) {
      TDef myDefinition(model());
      T myInstance(myDefinition);
      result = myInstance;
    }
    else {
      boost::optional<SpaceType> st = templateSpaceLoadInstance->spaceType();
      if (st && (st.get() == getObject<SpaceType>()))
      {
        result = templateSpaceLoadInstance;
      }
      else if (templateSpaceLoadInstance->model() != model()) {
        return boost::none;
      }
      else if (!st && !templateSpaceLoadInstance->space()) {
        result = templateSpaceLoadInstance;
      }
      else {
        T myInstance = templateSpaceLoadInstance->clone().template cast<T>();
        result = myInstance;
      }
    }
    return result;
  }

  template <typename T>
  void SpaceType_Impl::removeAllButOneSpaceLoadInstance(std::vector<T>& instances, const T& instanceToKeep) {
    int count(0);
    for (typename std::vector<T>::iterator it = instances.begin(), itEnd = instances.end();
         it != itEnd; ++it)
    {
      if (*it == instanceToKeep) {
        ++count;
        continue;
      }
      it->remove();
    }
    OS_ASSERT(count == 1);
  }

} // detail

SpaceType::SpaceType(const Model& model)
  : ResourceObject(SpaceType::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SpaceType_Impl>());
}

IddObjectType SpaceType::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SpaceType);
  return result;
}

boost::optional<DefaultConstructionSet> SpaceType::defaultConstructionSet() const
{
  return getImpl<detail::SpaceType_Impl>()->defaultConstructionSet();
}

bool SpaceType::setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet)
{
  return getImpl<detail::SpaceType_Impl>()->setDefaultConstructionSet(defaultConstructionSet);
}

void SpaceType::resetDefaultConstructionSet()
{
  getImpl<detail::SpaceType_Impl>()->resetDefaultConstructionSet();
}

boost::optional<DefaultScheduleSet> SpaceType::defaultScheduleSet() const
{
  return getImpl<detail::SpaceType_Impl>()->defaultScheduleSet();
}

boost::optional<Schedule> SpaceType::getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const
{
  return getImpl<detail::SpaceType_Impl>()->getDefaultSchedule(defaultScheduleType);
}

bool SpaceType::setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet)
{
  return getImpl<detail::SpaceType_Impl>()->setDefaultScheduleSet(defaultScheduleSet);
}

void SpaceType::resetDefaultScheduleSet()
{
  getImpl<detail::SpaceType_Impl>()->resetDefaultScheduleSet();
}

boost::optional<RenderingColor> SpaceType::renderingColor() const
{
  return getImpl<detail::SpaceType_Impl>()->renderingColor();
}

bool SpaceType::setRenderingColor(const RenderingColor& renderingColor)
{
  return getImpl<detail::SpaceType_Impl>()->setRenderingColor(renderingColor);
}

void SpaceType::resetRenderingColor()
{
  getImpl<detail::SpaceType_Impl>()->resetRenderingColor();
}

std::vector<Space> SpaceType::spaces() const
{
  return getImpl<detail::SpaceType_Impl>()->spaces();
}

std::vector<InternalMass> SpaceType::internalMass() const {
  return getImpl<detail::SpaceType_Impl>()->internalMass();
}

std::vector<People> SpaceType::people() const {
  return getImpl<detail::SpaceType_Impl>()->people();
}

std::vector<Lights> SpaceType::lights() const {
  return getImpl<detail::SpaceType_Impl>()->lights();
}

std::vector<Luminaire> SpaceType::luminaires() const {
  return getImpl<detail::SpaceType_Impl>()->luminaires();
}

std::vector<ElectricEquipment> SpaceType::electricEquipment() const {
  return getImpl<detail::SpaceType_Impl>()->electricEquipment();
}

std::vector<GasEquipment> SpaceType::gasEquipment() const {
  return getImpl<detail::SpaceType_Impl>()->gasEquipment();
}

std::vector<HotWaterEquipment> SpaceType::hotWaterEquipment() const {
  return getImpl<detail::SpaceType_Impl>()->hotWaterEquipment();
}

std::vector<SteamEquipment> SpaceType::steamEquipment() const {
  return getImpl<detail::SpaceType_Impl>()->steamEquipment();
}

std::vector<OtherEquipment> SpaceType::otherEquipment() const {
  return getImpl<detail::SpaceType_Impl>()->otherEquipment();
}

std::vector<SpaceInfiltrationDesignFlowRate> SpaceType::spaceInfiltrationDesignFlowRates() const {
  return getImpl<detail::SpaceType_Impl>()->spaceInfiltrationDesignFlowRates();
}

std::vector<SpaceInfiltrationEffectiveLeakageArea> SpaceType::spaceInfiltrationEffectiveLeakageAreas() const {
  return getImpl<detail::SpaceType_Impl>()->spaceInfiltrationEffectiveLeakageAreas();
}

boost::optional<DesignSpecificationOutdoorAir> SpaceType::designSpecificationOutdoorAir() const
{
  return getImpl<detail::SpaceType_Impl>()->designSpecificationOutdoorAir();
}

bool SpaceType::isDesignSpecificationOutdoorAirDefaulted() const
{
  return getImpl<detail::SpaceType_Impl>()->isDesignSpecificationOutdoorAirDefaulted();
}

bool SpaceType::setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir)
{
  return getImpl<detail::SpaceType_Impl>()->setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
}

void SpaceType::resetDesignSpecificationOutdoorAir()
{
  return getImpl<detail::SpaceType_Impl>()->resetDesignSpecificationOutdoorAir();
}

void SpaceType::hardApplySpaceLoadSchedules()
{
  getImpl<detail::SpaceType_Impl>()->hardApplySpaceLoadSchedules();
}

boost::optional<double> SpaceType::peoplePerFloorArea() const {
  return getImpl<detail::SpaceType_Impl>()->peoplePerFloorArea();
}

bool SpaceType::setPeoplePerFloorArea(double peoplePerFloorArea) {
  return getImpl<detail::SpaceType_Impl>()->setPeoplePerFloorArea(peoplePerFloorArea);
}

bool SpaceType::setPeoplePerFloorArea(double peoplePerFloorArea, const People& templatePeople) {
  return getImpl<detail::SpaceType_Impl>()->setPeoplePerFloorArea(peoplePerFloorArea,templatePeople);
}

boost::optional<double> SpaceType::spaceFloorAreaPerPerson() const {
  return getImpl<detail::SpaceType_Impl>()->spaceFloorAreaPerPerson();
}

bool SpaceType::setSpaceFloorAreaPerPerson(double spaceFloorAreaPerPerson) {
  return getImpl<detail::SpaceType_Impl>()->setSpaceFloorAreaPerPerson(spaceFloorAreaPerPerson);
}

bool SpaceType::setSpaceFloorAreaPerPerson(double spaceFloorAreaPerPerson, const People& templatePeople) {
  return getImpl<detail::SpaceType_Impl>()->setSpaceFloorAreaPerPerson(spaceFloorAreaPerPerson,templatePeople);
}

double SpaceType::getNumberOfPeople(double floorArea) const {
  return getImpl<detail::SpaceType_Impl>()->getNumberOfPeople(floorArea);
}

double SpaceType::getPeoplePerFloorArea(double floorArea) const {
  return getImpl<detail::SpaceType_Impl>()->getPeoplePerFloorArea(floorArea);
}

double SpaceType::getFloorAreaPerPerson(double floorArea) const {
  return getImpl<detail::SpaceType_Impl>()->getFloorAreaPerPerson(floorArea);
}

boost::optional<double> SpaceType::lightingPowerPerFloorArea() const {
  return getImpl<detail::SpaceType_Impl>()->lightingPowerPerFloorArea();
}

bool SpaceType::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea) {
  return getImpl<detail::SpaceType_Impl>()->setLightingPowerPerFloorArea(lightingPowerPerFloorArea);
}

bool SpaceType::setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, 
                                       const Lights& templateLights)
{
  return getImpl<detail::SpaceType_Impl>()->setLightingPowerPerFloorArea(lightingPowerPerFloorArea,templateLights);
}

boost::optional<double> SpaceType::lightingPowerPerPerson() const {
  return getImpl<detail::SpaceType_Impl>()->lightingPowerPerPerson();
}

bool SpaceType::setLightingPowerPerPerson(double lightingPowerPerPerson) {
  return getImpl<detail::SpaceType_Impl>()->setLightingPowerPerPerson(lightingPowerPerPerson);
}

bool SpaceType::setLightingPowerPerPerson(double lightingPowerPerPerson,const Lights& templateLights) {
  return getImpl<detail::SpaceType_Impl>()->setLightingPowerPerPerson(lightingPowerPerPerson,templateLights);
}

double SpaceType::getLightingPower(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getLightingPower(floorArea,numPeople);
}

double SpaceType::getLightingPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getLightingPowerPerFloorArea(floorArea,numPeople);
}

double SpaceType::getLightingPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getLightingPowerPerPerson(floorArea,numPeople);
}

boost::optional<double> SpaceType::electricEquipmentPowerPerFloorArea() const {
  return getImpl<detail::SpaceType_Impl>()->electricEquipmentPowerPerFloorArea();
}

bool SpaceType::setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea) {
  return getImpl<detail::SpaceType_Impl>()->setElectricEquipmentPowerPerFloorArea(electricEquipmentPowerPerFloorArea);
}

bool SpaceType::setElectricEquipmentPowerPerFloorArea(
    double electricEquipmentPowerPerFloorArea,
    const ElectricEquipment& templateElectricEquipment)
{
  return getImpl<detail::SpaceType_Impl>()->setElectricEquipmentPowerPerFloorArea(
      electricEquipmentPowerPerFloorArea,
      templateElectricEquipment);
}

boost::optional<double> SpaceType::electricEquipmentPowerPerPerson() const {
  return getImpl<detail::SpaceType_Impl>()->electricEquipmentPowerPerPerson();
}

bool SpaceType::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson) {
  return getImpl<detail::SpaceType_Impl>()->setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson);
}

bool SpaceType::setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson,
                                        const ElectricEquipment& templateElectricEquipment)
{
  return getImpl<detail::SpaceType_Impl>()->setElectricEquipmentPowerPerPerson(electricEquipmentPowerPerPerson,templateElectricEquipment);
}

double SpaceType::getElectricEquipmentDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getElectricEquipmentDesignLevel(floorArea,numPeople);
}

double SpaceType::getElectricEquipmentPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getElectricEquipmentPowerPerFloorArea(floorArea,numPeople);
}

double SpaceType::getElectricEquipmentPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getElectricEquipmentPowerPerPerson(floorArea,numPeople);
}

boost::optional<double> SpaceType::gasEquipmentPowerPerFloorArea() const {
  return getImpl<detail::SpaceType_Impl>()->gasEquipmentPowerPerFloorArea();
}

bool SpaceType::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea) {
  return getImpl<detail::SpaceType_Impl>()->setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea);
}

bool SpaceType::setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea,
                                           const GasEquipment& templateGasEquipment)
{
  return getImpl<detail::SpaceType_Impl>()->setGasEquipmentPowerPerFloorArea(gasEquipmentPowerPerFloorArea,templateGasEquipment);
}

boost::optional<double> SpaceType::gasEquipmentPowerPerPerson() const {
  return getImpl<detail::SpaceType_Impl>()->gasEquipmentPowerPerPerson();
}

bool SpaceType::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson) {
  return getImpl<detail::SpaceType_Impl>()->setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson);
}

bool SpaceType::setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson,
                                   const GasEquipment& templateGasEquipment)
{
  return getImpl<detail::SpaceType_Impl>()->setGasEquipmentPowerPerPerson(gasEquipmentPowerPerPerson,templateGasEquipment);
}

double SpaceType::getGasEquipmentDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getGasEquipmentDesignLevel(floorArea,numPeople);
}

double SpaceType::getGasEquipmentPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getGasEquipmentPowerPerFloorArea(floorArea,numPeople);
}

double SpaceType::getGasEquipmentPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::SpaceType_Impl>()->getGasEquipmentPowerPerPerson(floorArea,numPeople);
}

double SpaceType::floorArea() const
{
  return getImpl<detail::SpaceType_Impl>()->floorArea();
}

/// @cond
SpaceType::SpaceType(boost::shared_ptr<detail::SpaceType_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond


} // model
} // openstudio

