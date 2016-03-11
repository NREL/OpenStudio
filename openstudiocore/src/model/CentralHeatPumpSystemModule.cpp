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

#include "CentralHeatPumpSystemModule.hpp"
#include "CentralHeatPumpSystemModule_Impl.hpp"

#include "ChillerHeaterPerformanceElectricEIR.hpp"
#include "ChillerHeaterPerformanceElectricEIR_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_CentralHeatPumpSystem_Module_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CentralHeatPumpSystemModule_Impl::CentralHeatPumpSystemModule_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CentralHeatPumpSystemModule::iddObjectType());
  }

  CentralHeatPumpSystemModule_Impl::CentralHeatPumpSystemModule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CentralHeatPumpSystemModule::iddObjectType());
  }

  CentralHeatPumpSystemModule_Impl::CentralHeatPumpSystemModule_Impl(const CentralHeatPumpSystemModule_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CentralHeatPumpSystemModule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CentralHeatPumpSystemModule_Impl::iddObjectType() const {
    return CentralHeatPumpSystemModule::iddObjectType();
  }

  std::vector<ScheduleTypeKey> CentralHeatPumpSystemModule_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_CentralHeatPumpSystem_ModuleFields::ChillerHeaterModulesControlScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CentralHeatPumpSystemModule","Chiller Heater Modules Control"));
    }
    return result;
  }

  ChillerHeaterPerformanceElectricEIR CentralHeatPumpSystemModule_Impl::chillerHeaterModulesPerformanceComponent() const {
    boost::optional<ChillerHeaterPerformanceElectricEIR> value = optionalChillerHeaterModulesPerformanceComponent();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Chiller Heater Modules Performance Component attached.");
    }
    return value.get();
  }

  Schedule CentralHeatPumpSystemModule_Impl::chillerHeaterModulesControlSchedule() const {
    boost::optional<Schedule> value = optionalChillerHeaterModulesControlSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Chiller Heater Modules Control Schedule attached.");
    }
    return value.get();
  }

  int CentralHeatPumpSystemModule_Impl::numberofChillerHeaterModules() const {
    boost::optional<int> value = getInt(OS_CentralHeatPumpSystem_ModuleFields::NumberofChillerHeaterModules,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CentralHeatPumpSystemModule_Impl::setChillerHeaterModulesPerformanceComponent(const ChillerHeaterPerformanceElectricEIR& chillerHeaterPerformanceElectricEIR) {
    bool result = setPointer(OS_CentralHeatPumpSystem_ModuleFields::ChillerHeaterModulesPerformanceComponentName, chillerHeaterPerformanceElectricEIR.handle());
    return result;
  }

  bool CentralHeatPumpSystemModule_Impl::setChillerHeaterModulesControlSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_CentralHeatPumpSystem_ModuleFields::ChillerHeaterModulesControlScheduleName,
                              "CentralHeatPumpSystemModule",
                              "Chiller Heater Modules Control",
                              schedule);
    return result;
  }

  bool CentralHeatPumpSystemModule_Impl::setNumberofChillerHeaterModules(int numberofChillerHeaterModules) {
    bool result = setInt(OS_CentralHeatPumpSystem_ModuleFields::NumberofChillerHeaterModules, numberofChillerHeaterModules);
    return result;
  }

  boost::optional<ChillerHeaterPerformanceElectricEIR> CentralHeatPumpSystemModule_Impl::optionalChillerHeaterModulesPerformanceComponent() const {
    return getObject<ModelObject>().getModelObjectTarget<ChillerHeaterPerformanceElectricEIR>(OS_CentralHeatPumpSystem_ModuleFields::ChillerHeaterModulesPerformanceComponentName);
  }

  boost::optional<Schedule> CentralHeatPumpSystemModule_Impl::optionalChillerHeaterModulesControlSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_CentralHeatPumpSystem_ModuleFields::ChillerHeaterModulesControlScheduleName);
  }

} // detail

CentralHeatPumpSystemModule::CentralHeatPumpSystemModule(const Model& model)
  : ParentObject(CentralHeatPumpSystemModule::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CentralHeatPumpSystemModule_Impl>());

  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  auto const module = ChillerHeaterPerformanceElectricEIR(model);

  bool ok = true;
  ok = setChillerHeaterModulesPerformanceComponent( module );
  OS_ASSERT(ok);
  ok = setChillerHeaterModulesControlSchedule( alwaysOn );
  OS_ASSERT(ok);
  ok = setNumberofChillerHeaterModules( 1 );
  OS_ASSERT(ok);
}

IddObjectType CentralHeatPumpSystemModule::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CentralHeatPumpSystem_Module);
}

ChillerHeaterPerformanceElectricEIR CentralHeatPumpSystemModule::chillerHeaterModulesPerformanceComponent() const {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->chillerHeaterModulesPerformanceComponent();
}

Schedule CentralHeatPumpSystemModule::chillerHeaterModulesControlSchedule() const {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->chillerHeaterModulesControlSchedule();
}

int CentralHeatPumpSystemModule::numberofChillerHeaterModules() const {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->numberofChillerHeaterModules();
}

bool CentralHeatPumpSystemModule::setChillerHeaterModulesPerformanceComponent(const ChillerHeaterPerformanceElectricEIR& chillerHeaterPerformanceElectricEIR) {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->setChillerHeaterModulesPerformanceComponent(chillerHeaterPerformanceElectricEIR);
}

bool CentralHeatPumpSystemModule::setChillerHeaterModulesControlSchedule(Schedule& schedule) {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->setChillerHeaterModulesControlSchedule(schedule);
}

bool CentralHeatPumpSystemModule::setNumberofChillerHeaterModules(int numberofChillerHeaterModules) {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->setNumberofChillerHeaterModules(numberofChillerHeaterModules);
}

/// @cond
CentralHeatPumpSystemModule::CentralHeatPumpSystemModule(std::shared_ptr<detail::CentralHeatPumpSystemModule_Impl> impl)
  : ParentObject(impl)
{}
/// @endcond

} // model
} // openstudio

