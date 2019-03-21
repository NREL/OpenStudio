/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "Luminaire.hpp"
#include "Luminaire_Impl.hpp"

#include "LuminaireDefinition.hpp"
#include "LuminaireDefinition_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "DefaultScheduleSet.hpp"
#include "DefaultScheduleSet_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "Model.hpp"

#include <utilities/idd/OS_Luminaire_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Vector3d.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/EulerAngles.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Luminaire_Impl::Luminaire_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Luminaire::iddObjectType());
  }

  Luminaire_Impl::Luminaire_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Luminaire::iddObjectType());
  }

  Luminaire_Impl::Luminaire_Impl(const Luminaire_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Luminaire_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType Luminaire_Impl::iddObjectType() const {
    return Luminaire::iddObjectType();
  }

  std::vector<ScheduleTypeKey> Luminaire_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_LuminaireFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("Luminaire","Luminaire"));
    }
    return result;
  }

  bool Luminaire_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    LuminaireDefinition definition = this->luminaireDefinition();
    for (LifeCycleCost cost : definition.lifeCycleCosts()){
      // DLM: is this appropriate for a luminaire?
      cost.convertToCostPerEach();
    }

    return true;
  }

  bool Luminaire_Impl::hardApplySchedules()
  {
    bool result = false;
    boost::optional<Schedule> schedule = this->schedule();
    if (schedule){
      result = this->setSchedule(*schedule);
    }
    return result;
  }

  double Luminaire_Impl::positionXcoordinate() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::PositionXcoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double Luminaire_Impl::positionYcoordinate() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::PositionYcoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double Luminaire_Impl::positionZcoordinate() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::PositionZcoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double Luminaire_Impl::psiRotationAroundXaxis() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::PsiRotationAroundXaxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Luminaire_Impl::isPsiRotationAroundXaxisDefaulted() const {
    return isEmpty(OS_LuminaireFields::PsiRotationAroundXaxis);
  }

  double Luminaire_Impl::thetaRotationAroundYaxis() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::ThetaRotationAroundYaxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Luminaire_Impl::isThetaRotationAroundYaxisDefaulted() const {
    return isEmpty(OS_LuminaireFields::ThetaRotationAroundYaxis);
  }

  double Luminaire_Impl::phiRotationAroundZaxis() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::PhiRotationAroundZaxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Luminaire_Impl::isPhiRotationAroundZaxisDefaulted() const {
    return isEmpty(OS_LuminaireFields::PhiRotationAroundZaxis);
  }

  double Luminaire_Impl::fractionReplaceable() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::FractionReplaceable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Luminaire_Impl::isFractionReplaceableDefaulted() const {
    return isEmpty(OS_LuminaireFields::FractionReplaceable);
  }

  double Luminaire_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_LuminaireFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Luminaire_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_LuminaireFields::Multiplier);
  }

  bool Luminaire_Impl::isAbsolute() const {
    return true;
  }

  std::string Luminaire_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_LuminaireFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Luminaire_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_LuminaireFields::EndUseSubcategory);
  }

  bool Luminaire_Impl::setPositionXcoordinate(double positionXcoordinate) {
    bool result = setDouble(OS_LuminaireFields::PositionXcoordinate, positionXcoordinate);
    OS_ASSERT(result);
    return result;
  }

  bool Luminaire_Impl::setPositionYcoordinate(double positionYcoordinate) {
    bool result = setDouble(OS_LuminaireFields::PositionYcoordinate, positionYcoordinate);
    OS_ASSERT(result);
    return result;
  }

  bool Luminaire_Impl::setPositionZcoordinate(double positionZcoordinate) {
    bool result = setDouble(OS_LuminaireFields::PositionZcoordinate, positionZcoordinate);
    OS_ASSERT(result);
    return result;
  }

  bool Luminaire_Impl::setPsiRotationAroundXaxis(double psiRotationAroundXaxis) {
    bool result = setDouble(OS_LuminaireFields::PsiRotationAroundXaxis, psiRotationAroundXaxis);
    OS_ASSERT(result);
    return result;
  }

  void Luminaire_Impl::resetPsiRotationAroundXaxis() {
    bool result = setString(OS_LuminaireFields::PsiRotationAroundXaxis, "");
    OS_ASSERT(result);
  }

  bool Luminaire_Impl::setThetaRotationAroundYaxis(double thetaRotationAroundYaxis) {
    bool result = setDouble(OS_LuminaireFields::ThetaRotationAroundYaxis, thetaRotationAroundYaxis);
    OS_ASSERT(result);
    return result;
  }

  void Luminaire_Impl::resetThetaRotationAroundYaxis() {
    bool result = setString(OS_LuminaireFields::ThetaRotationAroundYaxis, "");
    OS_ASSERT(result);
  }

  bool Luminaire_Impl::setPhiRotationAroundZaxis(double phiRotationAroundZaxis) {
    bool result = setDouble(OS_LuminaireFields::PhiRotationAroundZaxis, phiRotationAroundZaxis);
    OS_ASSERT(result);
    return result;
  }

  void Luminaire_Impl::resetPhiRotationAroundZaxis() {
    bool result = setString(OS_LuminaireFields::PhiRotationAroundZaxis, "");
    OS_ASSERT(result);
  }

  bool Luminaire_Impl::setFractionReplaceable(double fractionReplaceable) {
    bool result = setDouble(OS_LuminaireFields::FractionReplaceable, fractionReplaceable);
    return result;
  }

  void Luminaire_Impl::resetFractionReplaceable() {
    bool result = setString(OS_LuminaireFields::FractionReplaceable, "");
    OS_ASSERT(result);
  }

  bool Luminaire_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_LuminaireFields::Multiplier, multiplier);
    return result;
  }

  void Luminaire_Impl::resetMultiplier() {
    bool result = setString(OS_LuminaireFields::Multiplier, "");
    OS_ASSERT(result);
  }

  bool Luminaire_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_LuminaireFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  void Luminaire_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_LuminaireFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  int Luminaire_Impl::spaceIndex() const {
    return OS_LuminaireFields::SpaceorSpaceTypeName;
  }

  int Luminaire_Impl::definitionIndex() const {
    return OS_LuminaireFields::LuminaireDefinitionName;
  }

  boost::optional<Schedule> Luminaire_Impl::schedule() const
  {
    boost::optional<Schedule> result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_LuminaireFields::ScheduleName);
    if (!result){
      // search upwards
      OptionalSpace space = this->space();
      OptionalSpaceType spaceType = this->spaceType();
      if (space){
        result = space->getDefaultSchedule(DefaultScheduleType::LightingSchedule);
      }else if (spaceType){
        result = spaceType->getDefaultSchedule(DefaultScheduleType::LightingSchedule);
      }
    }
    return result;
  }

  bool Luminaire_Impl::isScheduleDefaulted() const
  {
    return isEmpty(OS_LuminaireFields::ScheduleName);
  }

  bool Luminaire_Impl::setSchedule(Schedule& schedule)
  {
    bool result = ModelObject_Impl::setSchedule(OS_LuminaireFields::ScheduleName,
                                                "Luminaire",
                                                "Luminaire",
                                                schedule);
    return result;
  }

  void Luminaire_Impl::resetSchedule()
  {
    this->setString(OS_LuminaireFields::ScheduleName, "");
  }

  LuminaireDefinition Luminaire_Impl::luminaireDefinition() const
  {
    return this->definition().cast<LuminaireDefinition>();
  }

  bool Luminaire_Impl::setLuminaireDefinition(const LuminaireDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool Luminaire_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<LuminaireDefinition> luminaireDefinition = definition.optionalCast<LuminaireDefinition>();
    if (luminaireDefinition){
      result = setLuminaireDefinition(*luminaireDefinition);
    }
    return result;
  }

  Point3d Luminaire_Impl::position() const
  {
    double x = this->getDouble(OS_LuminaireFields::PositionXcoordinate, true).get();
    double y = this->getDouble(OS_LuminaireFields::PositionYcoordinate, true).get();
    double z = this->getDouble(OS_LuminaireFields::PositionZcoordinate, true).get();
    Point3d result(x,y,z);
    return result;
  }

  bool Luminaire_Impl::setPosition(const Point3d& position)
  {
    bool result = true;
    result = result && this->setDouble(OS_LuminaireFields::PositionXcoordinate, position.x());
    result = result && this->setDouble(OS_LuminaireFields::PositionYcoordinate, position.y());
    result = result && this->setDouble(OS_LuminaireFields::PositionZcoordinate, position.z());
    return result;
  }

  Transformation Luminaire_Impl::transformation() const
  {
    Vector3d origin(this->positionXcoordinate(),
                    this->positionYcoordinate(),
                    this->positionZcoordinate());

    EulerAngles angles(degToRad(this->psiRotationAroundXaxis()),
                       degToRad(this->thetaRotationAroundYaxis()),
                       degToRad(this->phiRotationAroundZaxis()));

    Transformation result = Transformation::translation(origin)*Transformation::rotation(angles);

    return result;
  }

  bool Luminaire_Impl::setTransformation(const Transformation& transformation)
  {
    Vector3d translation = transformation.translation();
    this->setPositionXcoordinate(translation.x());
    this->setPositionYcoordinate(translation.y());
    this->setPositionZcoordinate(translation.z());

    EulerAngles eulerAngles = transformation.eulerAngles();
    setPsiRotationAroundXaxis(radToDeg(eulerAngles.psi()));
    setThetaRotationAroundYaxis(radToDeg(eulerAngles.theta()));
    setPhiRotationAroundZaxis(radToDeg(eulerAngles.phi()));

    return true;
  }

  double Luminaire_Impl::lightingPower() const {
    return luminaireDefinition().lightingPower() * multiplier();
  }

  double Luminaire_Impl::getPowerPerFloorArea(double floorArea) const {
    return lightingPower() / floorArea;
  }

  double Luminaire_Impl::getPowerPerPerson(double numPeople) const {
    return lightingPower() / numPeople;
  }

  boost::optional<ModelObject> Luminaire_Impl::luminaireDefinitionAsModelObject() const {
    OptionalModelObject result = luminaireDefinition();
    return result;
  }

  boost::optional<ModelObject> Luminaire_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool Luminaire_Impl::setLuminaireDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalLuminaireDefinition intermediate = modelObject->optionalCast<LuminaireDefinition>();
      if (intermediate) {
        LuminaireDefinition luminaireDefinition(*intermediate);
        return setLuminaireDefinition(luminaireDefinition);
      }
    }
    return false;
  }

  bool Luminaire_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetSchedule();
    }
    return true;
  }

} // detail

Luminaire::Luminaire(const LuminaireDefinition& luminaireDefinition)
  : SpaceLoadInstance(Luminaire::iddObjectType(),luminaireDefinition)
{
  OS_ASSERT(getImpl<detail::Luminaire_Impl>());
  setPositionXcoordinate(0.0);
  setPositionYcoordinate(0.0);
  setPositionZcoordinate(0.0);

  setPsiRotationAroundXaxis(0.0);
  setThetaRotationAroundYaxis(0.0);
  setPhiRotationAroundZaxis(0.0);

  /*
   *Schedule sch = this->model().alwaysOnDiscreteSchedule();
   *bool test = this->setSchedule(sch);
   *OS_ASSERT(test);
   *test = this->setMultiplier(1.0);
   *OS_ASSERT(test);
   */
  bool test = this->setEndUseSubcategory("General");
  OS_ASSERT(test);
  test = this->setFractionReplaceable(1.0);
  OS_ASSERT(test);

}

IddObjectType Luminaire::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Luminaire);
  return result;
}

double Luminaire::positionXcoordinate() const {
  return getImpl<detail::Luminaire_Impl>()->positionXcoordinate();
}

double Luminaire::positionYcoordinate() const {
  return getImpl<detail::Luminaire_Impl>()->positionYcoordinate();
}

double Luminaire::positionZcoordinate() const {
  return getImpl<detail::Luminaire_Impl>()->positionZcoordinate();
}

double Luminaire::psiRotationAroundXaxis() const {
  return getImpl<detail::Luminaire_Impl>()->psiRotationAroundXaxis();
}

bool Luminaire::isPsiRotationAroundXaxisDefaulted() const {
  return getImpl<detail::Luminaire_Impl>()->isPsiRotationAroundXaxisDefaulted();
}

double Luminaire::thetaRotationAroundYaxis() const {
  return getImpl<detail::Luminaire_Impl>()->thetaRotationAroundYaxis();
}

bool Luminaire::isThetaRotationAroundYaxisDefaulted() const {
  return getImpl<detail::Luminaire_Impl>()->isThetaRotationAroundYaxisDefaulted();
}

double Luminaire::phiRotationAroundZaxis() const {
  return getImpl<detail::Luminaire_Impl>()->phiRotationAroundZaxis();
}

bool Luminaire::isPhiRotationAroundZaxisDefaulted() const {
  return getImpl<detail::Luminaire_Impl>()->isPhiRotationAroundZaxisDefaulted();
}

double Luminaire::fractionReplaceable() const {
  return getImpl<detail::Luminaire_Impl>()->fractionReplaceable();
}

bool Luminaire::isFractionReplaceableDefaulted() const {
  return getImpl<detail::Luminaire_Impl>()->isFractionReplaceableDefaulted();
}

std::string Luminaire::endUseSubcategory() const {
  return getImpl<detail::Luminaire_Impl>()->endUseSubcategory();
}

bool Luminaire::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::Luminaire_Impl>()->isEndUseSubcategoryDefaulted();
}

bool Luminaire::setPositionXcoordinate(double positionXcoordinate) {
  return getImpl<detail::Luminaire_Impl>()->setPositionXcoordinate(positionXcoordinate);
}

bool Luminaire::setPositionYcoordinate(double positionYcoordinate) {
  return getImpl<detail::Luminaire_Impl>()->setPositionYcoordinate(positionYcoordinate);
}

bool Luminaire::setPositionZcoordinate(double positionZcoordinate) {
  return getImpl<detail::Luminaire_Impl>()->setPositionZcoordinate(positionZcoordinate);
}

bool Luminaire::setPsiRotationAroundXaxis(double psiRotationAroundXaxis) {
  return getImpl<detail::Luminaire_Impl>()->setPsiRotationAroundXaxis(psiRotationAroundXaxis);
}

void Luminaire::resetPsiRotationAroundXaxis() {
  getImpl<detail::Luminaire_Impl>()->resetPsiRotationAroundXaxis();
}

bool Luminaire::setThetaRotationAroundYaxis(double thetaRotationAroundYaxis) {
  return getImpl<detail::Luminaire_Impl>()->setThetaRotationAroundYaxis(thetaRotationAroundYaxis);
}

void Luminaire::resetThetaRotationAroundYaxis() {
  getImpl<detail::Luminaire_Impl>()->resetThetaRotationAroundYaxis();
}

bool Luminaire::setPhiRotationAroundZaxis(double phiRotationAroundZaxis) {
  return getImpl<detail::Luminaire_Impl>()->setPhiRotationAroundZaxis(phiRotationAroundZaxis);
}

void Luminaire::resetPhiRotationAroundZaxis() {
  getImpl<detail::Luminaire_Impl>()->resetPhiRotationAroundZaxis();
}

bool Luminaire::setFractionReplaceable(double fractionReplaceable) {
  return getImpl<detail::Luminaire_Impl>()->setFractionReplaceable(fractionReplaceable);
}

void Luminaire::resetFractionReplaceable() {
  getImpl<detail::Luminaire_Impl>()->resetFractionReplaceable();
}

bool Luminaire::setMultiplier(double multiplier) {
  return getImpl<detail::Luminaire_Impl>()->setMultiplier(multiplier);
}

void Luminaire::resetMultiplier() {
  getImpl<detail::Luminaire_Impl>()->resetMultiplier();
}

bool Luminaire::setEndUseSubcategory(std::string endUseSubcategory) {
  return getImpl<detail::Luminaire_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void Luminaire::resetEndUseSubcategory() {
  getImpl<detail::Luminaire_Impl>()->resetEndUseSubcategory();
}

boost::optional<Schedule> Luminaire::schedule() const
{
  return getImpl<detail::Luminaire_Impl>()->schedule();
}

bool Luminaire::isScheduleDefaulted() const
{
  return getImpl<detail::Luminaire_Impl>()->isScheduleDefaulted();
}

bool Luminaire::setSchedule(Schedule& schedule)
{
  return getImpl<detail::Luminaire_Impl>()->setSchedule(schedule);
}

void Luminaire::resetSchedule()
{
  getImpl<detail::Luminaire_Impl>()->resetSchedule();
}

LuminaireDefinition Luminaire::luminaireDefinition() const
{
  return getImpl<detail::Luminaire_Impl>()->luminaireDefinition();
}

bool Luminaire::setLuminaireDefinition(const LuminaireDefinition& luminaireDefinition)
{
  return getImpl<detail::Luminaire_Impl>()->setLuminaireDefinition(luminaireDefinition);
}

Point3d Luminaire::position() const
{
  return getImpl<detail::Luminaire_Impl>()->position();
}

bool Luminaire::setPosition(const Point3d& position)
{
  return getImpl<detail::Luminaire_Impl>()->setPosition(position);
}

Transformation Luminaire::transformation() const
{
  return getImpl<detail::Luminaire_Impl>()->transformation();
}

bool Luminaire::setTransformation(const Transformation& transformation)
{
  return getImpl<detail::Luminaire_Impl>()->setTransformation(transformation);
}

double Luminaire::lightingPower() const {
  return getImpl<detail::Luminaire_Impl>()->lightingPower();
}

double Luminaire::getPowerPerFloorArea(double floorArea) const {
  return getImpl<detail::Luminaire_Impl>()->getPowerPerFloorArea(floorArea);
}

double Luminaire::getPowerPerPerson(double numPeople) const {
  return getImpl<detail::Luminaire_Impl>()->getPowerPerPerson(numPeople);
}

/// @cond
Luminaire::Luminaire(std::shared_ptr<detail::Luminaire_Impl> impl)
  : SpaceLoadInstance(std::move(impl))
{}
/// @endcond

} // model
} // openstudio