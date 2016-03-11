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

#include "CentralHeatPumpSystem.hpp"
#include "CentralHeatPumpSystem_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "CentralHeatPumpSystemModule.hpp"
#include "CentralHeatPumpSystemModule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_CentralHeatPumpSystem_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CentralHeatPumpSystem::iddObjectType());
  }

  CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CentralHeatPumpSystem::iddObjectType());
  }

  CentralHeatPumpSystem_Impl::CentralHeatPumpSystem_Impl(const CentralHeatPumpSystem_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CentralHeatPumpSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CentralHeatPumpSystem_Impl::iddObjectType() const {
    return CentralHeatPumpSystem::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CentralHeatPumpSystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CentralHeatPumpSystem","Ancillary Operation"));
    }
    return result;
  }

  unsigned CentralHeatPumpSystem_Impl::supplyInletPort()
  {
    return OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName;
  }

  unsigned CentralHeatPumpSystem_Impl::supplyOutletPort()
  {
    return OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName;
  }

  unsigned CentralHeatPumpSystem_Impl::demandInletPort()
  {
    return OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName;
  }

  unsigned CentralHeatPumpSystem_Impl::demandOutletPort()
  {
    return OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName;
  }

// OS_CentralHeatPumpSystemFields::CoolingLoopInletNodeName
// OS_CentralHeatPumpSystemFields::CoolingLoopOutletNodeName
// OS_CentralHeatPumpSystemFields::SourceLoopInletNodeName
// OS_CentralHeatPumpSystemFields::SourceLoopOutletNodeName
// OS_CentralHeatPumpSystemFields::HeatingLoopInletNodeName
// OS_CentralHeatPumpSystemFields::HeatingLoopOutletNodeName

  boost::optional<ModelObjectList> CentralHeatPumpSystem_Impl::chillerHeaterModuleList() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_CentralHeatPumpSystemFields::ChillerHeaterModuleListName);
  }

  bool CentralHeatPumpSystem_Impl::addModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule) {
    auto modelObjectList = chillerHeaterModuleList();
    if( modelObjectList ) {
      modelObjectList->addModelObject(centralHeatPumpSystemModule);
    }
    return false;
  }

  void CentralHeatPumpSystem_Impl::removeModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule) {
    auto modelObjectList = chillerHeaterModuleList();
    if( modelObjectList ) {
      modelObjectList->removeModelObject(centralHeatPumpSystemModule);
    }
  }

  void CentralHeatPumpSystem_Impl::removeAllModules() {
    auto modelObjectList = chillerHeaterModuleList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CentralHeatPumpSystemModule>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
      }
    }
  }

  std::vector<CentralHeatPumpSystemModule> CentralHeatPumpSystem_Impl::modules() const {
    std::vector<CentralHeatPumpSystemModule> result;
    auto const modelObjectList = chillerHeaterModuleList();
    if( modelObjectList ) {
      auto const modelObjects = modelObjectList->modelObjects();

      for(const auto & elem : modelObjects) {
          auto const modelObject = elem.optionalCast<CentralHeatPumpSystemModule>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
      }
    }
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setChillerHeaterModuleList(const boost::optional<ModelObjectList>& modelObjectList) {
    bool result(false);
    if (modelObjectList) {
      result = setPointer(OS_CentralHeatPumpSystemFields::ChillerHeaterModuleListName, modelObjectList.get().handle());
    }
    else {
      resetChillerHeaterModuleList();
      result = true;
    }
    return result;
  }

  void CentralHeatPumpSystem_Impl::resetChillerHeaterModuleList() {
    bool result = setString(OS_CentralHeatPumpSystemFields::ChillerHeaterModuleListName, "");
    OS_ASSERT(result);
  }

  std::string CentralHeatPumpSystem_Impl::controlMethod() const {
    boost::optional<std::string> value = getString(OS_CentralHeatPumpSystemFields::ControlMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CentralHeatPumpSystem_Impl::ancillaryPower() const {
    boost::optional<double> value = getDouble(OS_CentralHeatPumpSystemFields::AncillaryPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> CentralHeatPumpSystem_Impl::ancillaryOperationSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName);
  }

  bool CentralHeatPumpSystem_Impl::setControlMethod(std::string controlMethod) {
    bool result = setString(OS_CentralHeatPumpSystemFields::ControlMethod, controlMethod);
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setAncillaryPower(double ancillaryPower) {
    bool result = setDouble(OS_CentralHeatPumpSystemFields::AncillaryPower, ancillaryPower);
    return result;
  }

  bool CentralHeatPumpSystem_Impl::setAncillaryOperationSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName,
                              "CentralHeatPumpSystem",
                              "Ancillary Operation",
                              schedule);
    return result;
  }

  void CentralHeatPumpSystem_Impl::resetAncillaryOperationSchedule() {
    bool result = setString(OS_CentralHeatPumpSystemFields::AncillaryOperationScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

CentralHeatPumpSystem::CentralHeatPumpSystem(const Model& model)
  : WaterToWaterComponent(CentralHeatPumpSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CentralHeatPumpSystem_Impl>());

  bool ok = true;
  ok = setControlMethod( "SmartMixing" );
  OS_ASSERT(ok);
  ok = setAncillaryPower( 0.0 );
  OS_ASSERT(ok);

  auto chillerHeaterModuleList = ModelObjectList(model);
  chillerHeaterModuleList.setName(this->name().get() + " Chiller Heater Module List");
  ok = getImpl<detail::CentralHeatPumpSystem_Impl>()->setChillerHeaterModuleList(chillerHeaterModuleList);
  OS_ASSERT(ok);

}

IddObjectType CentralHeatPumpSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CentralHeatPumpSystem);
}

std::vector<std::string> CentralHeatPumpSystem::controlMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_CentralHeatPumpSystemFields::ControlMethod);
}

std::string CentralHeatPumpSystem::controlMethod() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->controlMethod();
}

double CentralHeatPumpSystem::ancillaryPower() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->ancillaryPower();
}

boost::optional<Schedule> CentralHeatPumpSystem::ancillaryOperationSchedule() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->ancillaryOperationSchedule();
}

bool CentralHeatPumpSystem::setControlMethod(std::string controlMethod) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setControlMethod(controlMethod);
}

bool CentralHeatPumpSystem::setAncillaryPower(double ancillaryPower) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setAncillaryPower(ancillaryPower);
}

bool CentralHeatPumpSystem::setAncillaryOperationSchedule(Schedule& schedule) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->setAncillaryOperationSchedule(schedule);
}

void CentralHeatPumpSystem::resetAncillaryOperationSchedule() {
  getImpl<detail::CentralHeatPumpSystem_Impl>()->resetAncillaryOperationSchedule();
}

bool CentralHeatPumpSystem::addModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule) {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->addModule( centralHeatPumpSystemModule );
}

void CentralHeatPumpSystem::removeModule( const CentralHeatPumpSystemModule & centralHeatPumpSystemModule) {
  getImpl<detail::CentralHeatPumpSystem_Impl>()->removeModule( centralHeatPumpSystemModule );
}

void CentralHeatPumpSystem::removeAllModules() {
  getImpl<detail::CentralHeatPumpSystem_Impl>()->removeAllModules();
}

std::vector<CentralHeatPumpSystemModule> CentralHeatPumpSystem::modules() const {
  return getImpl<detail::CentralHeatPumpSystem_Impl>()->modules();
}

/// @cond
CentralHeatPumpSystem::CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl)
  : WaterToWaterComponent(impl)
{}
/// @endcond

} // model
} // openstudio

