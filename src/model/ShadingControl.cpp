/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ShadingControl.hpp"
#include "ShadingControl_Impl.hpp"
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "ShadingMaterial.hpp"
#include "ShadingMaterial_Impl.hpp"
#include "Blind.hpp"
#include "Blind_Impl.hpp"
#include "DaylightRedirectionDevice.hpp"
#include "DaylightRedirectionDevice_Impl.hpp"
#include "Screen.hpp"
#include "Screen_Impl.hpp"
#include "Shade.hpp"
#include "Shade_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ShadingControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ShadingControl_Impl::ShadingControl_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ShadingControl::iddObjectType());
  }

  ShadingControl_Impl::ShadingControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ShadingControl::iddObjectType());
  }

  ShadingControl_Impl::ShadingControl_Impl(const ShadingControl_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ShadingControl_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result;
    return result;
  }

  IddObjectType ShadingControl_Impl::iddObjectType() const
  {
    return ShadingControl::iddObjectType();
  }

  ModelObject ShadingControl_Impl::clone(Model model) const {
    ShadingControl scClone = ResourceObject_Impl::clone(model).cast<ShadingControl>();

    if (model != this->model()) {
      scClone.removeAllSubSurfaces();
    }

    return scClone;
  }

  boost::optional<Construction> ShadingControl_Impl::construction() const
  {
    return getObject<ShadingControl>().getModelObjectTarget<Construction>(OS_ShadingControlFields::ConstructionwithShadingName);
  }

  boost::optional<ShadingMaterial> ShadingControl_Impl::shadingMaterial() const
  {
    return getObject<ShadingControl>().getModelObjectTarget<ShadingMaterial>(OS_ShadingControlFields::ShadingDeviceMaterialName);
  }

  std::string ShadingControl_Impl::shadingType() const
  {
    boost::optional<std::string> result = getString(OS_ShadingControlFields::ShadingType, true);
    OS_ASSERT(result);
    return result.get();
  }

  std::string ShadingControl_Impl::shadingControlType() const
  {
    boost::optional<std::string> result = getString(OS_ShadingControlFields::ShadingControlType, true);
    OS_ASSERT(result);
    return result.get();
  }

  bool ShadingControl_Impl::isShadingControlTypeDefaulted() const
  {
    return isEmpty(OS_ShadingControlFields::ShadingControlType);
  }

  boost::optional<Schedule> ShadingControl_Impl::schedule() const
  {
    return getObject<ShadingControl>().getModelObjectTarget<Schedule>(OS_ShadingControlFields::ScheduleName);
  }

  boost::optional<double> ShadingControl_Impl::setpoint() const
  {
    boost::optional<double> result = getDouble(OS_ShadingControlFields::Setpoint);
    if (!result){
      std::string shadingControlType = this->shadingControlType();
      if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)){
        result = 27; // w/m^2
      }
    }
    return result;
  }

  bool ShadingControl_Impl::isSetpointDefaulted() const
  {
    return isEmpty(OS_ShadingControlFields::Setpoint);
  }

  bool ShadingControl_Impl::setShadingType(const std::string& shadingType)
  {
    return setString(OS_ShadingControlFields::ShadingType, shadingType);
  }

  bool ShadingControl_Impl::setShadingControlType(const std::string& shadingControlType)
  {
    std::string oldControlType = this->shadingControlType();
    bool result = setString(OS_ShadingControlFields::ShadingControlType, shadingControlType);
    if (result){
      if (oldControlType != shadingControlType){
        resetSetpoint();
      } else if (istringEqual("AlwaysOn", shadingControlType) ||
                 istringEqual("AlwaysOff", shadingControlType) ||
                 istringEqual("OnIfScheduleAllows", shadingControlType)){
        resetSetpoint();
      }
    }
    return result;
  }

  void ShadingControl_Impl::resetShadingControlType()
  {
    bool test = setString(OS_ShadingControlFields::ShadingControlType, "");
    OS_ASSERT(test);

    resetSetpoint();
  }

  bool ShadingControl_Impl::setSchedule(const Schedule& schedule)
  {
    bool result = setPointer(OS_ShadingControlFields::ScheduleName, schedule.handle());
    if (result){
      bool test = setString(OS_ShadingControlFields::ShadingControlIsScheduled, "Yes");
      OS_ASSERT(test);
    }
    return result;
  }

  void ShadingControl_Impl::resetSchedule()
  {
    bool test = setString(OS_ShadingControlFields::ScheduleName, "");
    OS_ASSERT(test);

    test = setString(OS_ShadingControlFields::ShadingControlIsScheduled, "No");
    OS_ASSERT(test);
  }

  bool ShadingControl_Impl::setSetpoint(double setpoint)
  {
    bool result = false;
    std::string shadingControlType = this->shadingControlType();
    if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)){
      result = setDouble(OS_ShadingControlFields::Setpoint, setpoint);
    }
    return result;
  }

  void ShadingControl_Impl::resetSetpoint()
  {
    bool test = setString(OS_ShadingControlFields::Setpoint, "");
    OS_ASSERT(test);
  }

  std::string ShadingControl_Impl::multipleSurfaceControlType() const
  {
    boost::optional<std::string> result = getString(OS_ShadingControlFields::MultipleSurfaceControlType, true);
    OS_ASSERT(result);
    return result.get();
  }

  bool ShadingControl_Impl::setMultipleSurfaceControlType(const std::string& multipleSurfaceControlType)
  {
    return setString(OS_ShadingControlFields::MultipleSurfaceControlType, multipleSurfaceControlType);
  }

  unsigned int ShadingControl_Impl::numberofSubSurfaces() const
  {
    return numExtensibleGroups();
  }

  std::vector<SubSurface> ShadingControl_Impl::subSurfaces() const
  {
    std::vector<SubSurface> result;
    auto groups = extensibleGroups();
    for (const auto & group : groups) {
      boost::optional<WorkspaceObject> wo = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ShadingControlExtensibleFields::SubSurfaceName);
      if (wo) {
        if (auto subsurface = wo->optionalCast<SubSurface>()) {
          result.push_back(subsurface.get());
         }
      }
    }
    return result;
  }

  boost::optional<unsigned> ShadingControl_Impl::subSurfaceIndex(const SubSurface& subSurface) const {

    boost::optional<unsigned> result;

    auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
    auto h = openstudio::toString(subSurface.handle());
    auto it = std::find_if(egs.begin(), egs.end(),
      [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_ShadingControlExtensibleFields::SubSurfaceName).get() == h);
      });

    // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
    if (it != egs.end()) {
      result = std::distance(egs.begin(), it) + 1;
    }

    return result;
  }


  bool ShadingControl_Impl::addSubSurface(const SubSurface& subSurface)
  {
    // Check if subSurface already exists
    boost::optional<unsigned> _existingIndex = subSurfaceIndex(subSurface);
    if (_existingIndex) {
      LOG(Warn, "For " << briefDescription() << ", SubSurface already exists.");
      return true;
    }

    bool result;

    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    bool subsurface = eg.setPointer(OS_ShadingControlExtensibleFields::SubSurfaceName, subSurface.handle());
    if (subsurface) {
      result = true;
    } else {
      // Something went wrong
      // So erase the new extensible group
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      result = false;
    }
    return result;
  }


  bool ShadingControl_Impl::setSubSurfaceIndex(const SubSurface& subSurface, unsigned index)
  {
    boost::optional<unsigned> idx = subSurfaceIndex(subSurface);
    if (!idx) {
      LOG(Warn, "For " << briefDescription() << " cannot set the index of SubSurface " << subSurface.nameString() << " since it is not part of it.");
      return false;
    }

    // TODO: we could just set via string instead of doing a ton of typechecking below...
    std::vector<SubSurface> subSurfaceVector = subSurfaces();

    if (index > subSurfaceVector.size()) {
      LOG(Warn, "Requested a subSurface index of " << index << " to be assigned to " << subSurface.nameString() << ", but "
          << briefDescription() << " only has " << subSurfaceVector.size() << " SubSurfaces, resetting to that.");
      index = subSurfaceVector.size();
    } else if (index < 1) {
      LOG(Warn, "Requested a subSurface index of " << index << " < 1 to be assigned to " << subSurface.briefDescription() << ", resetting to 1");
      index = 1;
    }

    subSurfaceVector.erase(subSurfaceVector.begin() + idx.get() - 1); // subSurfaceIndex is 1-indexed, and vector is 0-indexed

    subSurfaceVector.insert(subSurfaceVector.begin() + (index - 1), subSurface);

    return setSubSurfaces(subSurfaceVector);
  }

  bool ShadingControl_Impl::addSubSurface(const SubSurface& subSurface, unsigned index) {
    bool ok = addSubSurface(subSurface);
    if (!ok) {
      return false;
    }
    ok = setSubSurfaceIndex(subSurface, index);
    return ok;
  }

  bool ShadingControl_Impl::removeSubSurface(const SubSurface& subSurface)
  {
    boost::optional<unsigned> idx = subSurfaceIndex(subSurface);
    if (!idx) {
      LOG(Warn, "For " << briefDescription() << " cannot remove SubSurface " << subSurface.nameString() << " since it is not part of it.");
      return false;
    }

    return removeSubSurface(idx.get());
  }

  bool ShadingControl_Impl::removeSubSurface(unsigned index) {
    bool result = false;
    if ((index > 0) && (index <= numberofSubSurfaces())) {
      getObject<ModelObject>().eraseExtensibleGroup(index-1);
      result = true;
    }
    return result;
  }


  bool ShadingControl_Impl::addSubSurfaces(const std::vector<SubSurface> &subSurfaces)
  {
    bool ok = true;
    for (const SubSurface& subSurface : subSurfaces) {
      ok &= addSubSurface(subSurface);
    }
    return ok;
  }

  void ShadingControl_Impl::removeAllSubSurfaces()
  {
    clearExtensibleGroups();
  }


  bool ShadingControl_Impl::setSubSurfaces(const std::vector<SubSurface> &subSurfaces)
  {
    removeAllSubSurfaces();
    return addSubSurfaces(subSurfaces);
  }

} // detail


ShadingControl::ShadingControl(const Construction& construction)
  : ResourceObject(ShadingControl::iddObjectType(),construction.model())
{
  OS_ASSERT(getImpl<detail::ShadingControl_Impl>());

  std::vector<Material> layers = construction.layers();

  std::string type;

  // check layers from inside out
  int i = layers.size() - 1;
  for (; i >=0; --i){
    if (layers[i].optionalCast<Shade>()){
      type = "Shade";
      LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
      break;
    }else if( layers[i].optionalCast<Blind>()){
      type = "Blind";
      LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
      break;
    }else if( layers[i].optionalCast<Screen>()){
      type = "Screen";
      setShadingControlType("AlwaysOn");
      LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
      break;
    } else if (layers[i].optionalCast<DaylightRedirectionDevice>()){
      type = "DaylightRedirectionDevice";
      setShadingControlType("AlwaysOn");
      LOG(Warn, "ShadingMaterial detected in shaded construction, prefer to use the ShadingControl constructor from ShadingMaterial.");
      break;
    }
  }

  std::string position;
  if (i == int(layers.size()) - 1){
    position = "Interior";
  }else if (i == 0){
    position = "Exterior";
  }else{
    position = "BetweenGlass";
  }

  if (type.empty()){
    type = "SwitchableGlazing";
    position = "";
  }

  if (type == "Screen" && position != "Exterior"){
    this->remove();
    LOG_AND_THROW(position << type << " is not an allowable configuration for ShadingControl");
  }

  if (type == "DaylightRedirectionDevice" && position != "Interior"){
    this->remove();
    LOG_AND_THROW(position << type << " is not an allowable configuration for ShadingControl");
  }

  bool test = this->setShadingType(position + type);
  OS_ASSERT(test);

  test = setMultipleSurfaceControlType("Sequential"); // E+ IDD Default
  OS_ASSERT(test);

  test = this->setPointer(OS_ShadingControlFields::ConstructionwithShadingName, construction.handle());
  OS_ASSERT(test);
}

ShadingControl::ShadingControl(const ShadingMaterial& shadingMaterial)
  : ResourceObject(ShadingControl::iddObjectType(),shadingMaterial.model())
{
  OS_ASSERT(getImpl<detail::ShadingControl_Impl>());

  std::string type;
  if (shadingMaterial.optionalCast<Shade>()){
    type = "InteriorShade";
  }else if(shadingMaterial.optionalCast<Blind>()){
    type = "InteriorBlind";
  }else if(shadingMaterial.optionalCast<Screen>()){
    type = "ExteriorScreen";
    setShadingControlType("AlwaysOn");
  } else if (shadingMaterial.optionalCast<DaylightRedirectionDevice>()){
    type = "InteriorDaylightRedirectionDevice";
    setShadingControlType("AlwaysOn");
  }

  bool test = this->setShadingType(type);
  OS_ASSERT(test);

  test = setMultipleSurfaceControlType("Sequential"); // E+ IDD Default
  OS_ASSERT(test);

  test = this->setPointer(OS_ShadingControlFields::ShadingDeviceMaterialName, shadingMaterial.handle());
  OS_ASSERT(test);
}

IddObjectType ShadingControl::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ShadingControl);
}

std::vector<std::string> ShadingControl::shadingTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ShadingControlFields::ShadingType);
}

std::vector<std::string> ShadingControl::shadingControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ShadingControlFields::ShadingControlType);
}

std::vector<std::string> ShadingControl::multipleSurfaceControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ShadingControlFields::MultipleSurfaceControlType);
}

boost::optional<Construction> ShadingControl::construction() const {
  return getImpl<detail::ShadingControl_Impl>()->construction();
}

boost::optional<ShadingMaterial> ShadingControl::shadingMaterial() const {
  return getImpl<detail::ShadingControl_Impl>()->shadingMaterial();
}

std::string ShadingControl::shadingType() const {
  return getImpl<detail::ShadingControl_Impl>()->shadingType();
}

std::string ShadingControl::shadingControlType() const {
  return getImpl<detail::ShadingControl_Impl>()->shadingControlType();
}

bool ShadingControl::isShadingControlTypeDefaulted() const {
  return getImpl<detail::ShadingControl_Impl>()->isShadingControlTypeDefaulted();
}

boost::optional<Schedule> ShadingControl::schedule() const {
  return getImpl<detail::ShadingControl_Impl>()->schedule();
}

boost::optional<double> ShadingControl::setpoint() const {
  return getImpl<detail::ShadingControl_Impl>()->setpoint();
}

bool ShadingControl::isSetpointDefaulted() const{
  return getImpl<detail::ShadingControl_Impl>()->isSetpointDefaulted();
}

bool ShadingControl::setShadingType(const std::string& shadingType){
  return getImpl<detail::ShadingControl_Impl>()->setShadingType(shadingType);
}

bool ShadingControl::setShadingControlType(const std::string& shadingControlType){
  return getImpl<detail::ShadingControl_Impl>()->setShadingControlType(shadingControlType);
}

void ShadingControl::resetShadingControlType(){
  return getImpl<detail::ShadingControl_Impl>()->resetShadingControlType();
}

bool ShadingControl::setSchedule(const Schedule& schedule){
  return getImpl<detail::ShadingControl_Impl>()->setSchedule(schedule);
}

void ShadingControl::resetSchedule(){
  getImpl<detail::ShadingControl_Impl>()->resetSchedule();
}

bool ShadingControl::setSetpoint(double setpoint){
  return getImpl<detail::ShadingControl_Impl>()->setSetpoint(setpoint);
}

void ShadingControl::resetSetpoint(){
  getImpl<detail::ShadingControl_Impl>()->resetSetpoint();
}

std::string ShadingControl::multipleSurfaceControlType() const {
  return getImpl<detail::ShadingControl_Impl>()->multipleSurfaceControlType();
}


bool ShadingControl::setMultipleSurfaceControlType(const std::string& multipleSurfaceControlType){
  return getImpl<detail::ShadingControl_Impl>()->setMultipleSurfaceControlType(multipleSurfaceControlType);
}



// Extensible Fields

std::vector<SubSurface> ShadingControl::subSurfaces() const {
  return getImpl<detail::ShadingControl_Impl>()->subSurfaces();
}

unsigned int ShadingControl::numberofSubSurfaces() const {
  return getImpl<detail::ShadingControl_Impl>()->numberofSubSurfaces();
}

boost::optional<unsigned> ShadingControl::subSurfaceIndex(const SubSurface& subSurface) const {
  return getImpl<detail::ShadingControl_Impl>()->subSurfaceIndex(subSurface);
}

bool ShadingControl::addSubSurface(const SubSurface& subSurface) {
  return getImpl<detail::ShadingControl_Impl>()->addSubSurface(subSurface);
}

bool ShadingControl::addSubSurface(const SubSurface& subSurface, unsigned index) {
  return getImpl<detail::ShadingControl_Impl>()->addSubSurface(subSurface, index);
}

bool ShadingControl::setSubSurfaceIndex(const SubSurface& subSurface, unsigned index) {
  return getImpl<detail::ShadingControl_Impl>()->setSubSurfaceIndex(subSurface, index);
}

bool ShadingControl::removeSubSurface(unsigned index) {
  return getImpl<detail::ShadingControl_Impl>()->removeSubSurface(index);
}

bool ShadingControl::removeSubSurface(const SubSurface& subSurface) {
  return getImpl<detail::ShadingControl_Impl>()->removeSubSurface(subSurface);
}

bool ShadingControl::addSubSurfaces(const std::vector<SubSurface> &subSurfaces) {
  return getImpl<detail::ShadingControl_Impl>()->addSubSurfaces(subSurfaces);
}

bool ShadingControl::setSubSurfaces(const std::vector<SubSurface> &subSurfaces) {
  return getImpl<detail::ShadingControl_Impl>()->setSubSurfaces(subSurfaces);
}

void ShadingControl::removeAllSubSurfaces() {
  getImpl<detail::ShadingControl_Impl>()->removeAllSubSurfaces();
}

/// @cond
ShadingControl::ShadingControl(std::shared_ptr<detail::ShadingControl_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

