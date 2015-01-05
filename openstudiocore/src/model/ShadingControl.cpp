/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "ShadingControl.hpp"
#include "ShadingControl_Impl.hpp"
#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "ShadingMaterial.hpp"
#include "ShadingMaterial_Impl.hpp"
#include "Blind.hpp"
#include "Blind_Impl.hpp"
#include "Screen.hpp"
#include "Screen_Impl.hpp"
#include "Shade.hpp"
#include "Shade_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

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
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }
  
  IddObjectType ShadingControl_Impl::iddObjectType() const
  {
    return ShadingControl::iddObjectType();
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

  boost::optional<Schedule> ShadingControl_Impl::schedule() const
  {
    return getObject<ShadingControl>().getModelObjectTarget<Schedule>(OS_ShadingControlFields::ScheduleName);
  }
 

  bool ShadingControl_Impl::setShadingType(const std::string& shadingType)
  {
    return setString(OS_ShadingControlFields::ShadingType, shadingType);
  }
  
  bool ShadingControl_Impl::setShadingControlType(const std::string& shadingControlType)
  {
    return setString(OS_ShadingControlFields::ShadingControlType, shadingControlType);
  }
  
  bool ShadingControl_Impl::setSchedule(const Schedule& schedule)
  {
    return setPointer(OS_ShadingControlFields::ScheduleName, schedule.handle());
  }
  
  void ShadingControl_Impl::resetSchedule()
  {
    bool test = setString(OS_ShadingControlFields::ScheduleName, "");
    OS_ASSERT(test);
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
      break;
    }else if( layers[i].optionalCast<Blind>()){
      type = "Blind";
      break;
    }else if( layers[i].optionalCast<Screen>()){
      type = "Screen";
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

  bool test = this->setShadingType(position + type);
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
  }

  bool test = this->setShadingType(type);
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

boost::optional<Schedule> ShadingControl::schedule() const {
  return getImpl<detail::ShadingControl_Impl>()->schedule();
} 

bool ShadingControl::setShadingType(const std::string& shadingType){
  return getImpl<detail::ShadingControl_Impl>()->setShadingType(shadingType);
} 

bool ShadingControl::setShadingControlType(const std::string& shadingControlType){
  return getImpl<detail::ShadingControl_Impl>()->setShadingControlType(shadingControlType);
} 

bool ShadingControl::setSchedule(const Schedule& schedule){
  return getImpl<detail::ShadingControl_Impl>()->setSchedule(schedule);
} 

void ShadingControl::resetSchedule(){
  return getImpl<detail::ShadingControl_Impl>()->resetSchedule();
} 


/// @cond
ShadingControl::ShadingControl(std::shared_ptr<detail::ShadingControl_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

