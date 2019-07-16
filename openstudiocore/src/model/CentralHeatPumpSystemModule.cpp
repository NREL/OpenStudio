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

#include "CentralHeatPumpSystemModule.hpp"
#include "CentralHeatPumpSystemModule_Impl.hpp"

#include "ChillerHeaterPerformanceElectricEIR.hpp"
#include "ChillerHeaterPerformanceElectricEIR_Impl.hpp"
#include "CentralHeatPumpSystem.hpp"
#include "CentralHeatPumpSystem_Impl.hpp"

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

  /** This method clones the CentralHeatPumpSystemModyle but sets the chillerHeaterModulesPerformanceComponent to the same as the original
   * By using the "children" method and listing the chillerHeaterModulesPerformanceComponent there ModelObject_Impl::clone will automatically do
   * the right thing -> NO IT DOESN'T */
  ModelObject CentralHeatPumpSystemModule_Impl::clone(Model model) const
  {
    CentralHeatPumpSystemModule newCentralHPMod = ModelObject_Impl::clone(model).cast<CentralHeatPumpSystemModule>();

    // If not using "children", then expliclity do it:
    bool ok = true;
    Model t_model = this->model();

    // If it's the same model
    if (t_model == model) {
      // We don't want to clone the perf object, set it to the same ChillHeaterPerformanceElectricEIR
      ChillerHeaterPerformanceElectricEIR chillerHeaterPerf = this->chillerHeaterModulesPerformanceComponent();
      ok = newCentralHPMod.setChillerHeaterModulesPerformanceComponent(chillerHeaterPerf);

    // If it's a different model
    } else {
      // We clone the chillerHeaterPerformance into the target model
      ChillerHeaterPerformanceElectricEIR chillerHeaterPerf = this->chillerHeaterModulesPerformanceComponent().clone(model).cast<ChillerHeaterPerformanceElectricEIR>();
      ok = newCentralHPMod.setChillerHeaterModulesPerformanceComponent(chillerHeaterPerf);
    }

    // This better have worked
    OS_ASSERT(ok);

    return newCentralHPMod;
  }


  // Returns allowable child types: ChillerHeaterPerformanceElectricEIR
  std::vector<IddObjectType> CentralHeatPumpSystemModule_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    // result.push_back(IddObjectType::OS_ChillerHeaterPerformance_Electric_EIR);
    return result;
  }

  // Returns the single child object: the ChillerHeaterPerformanceElectricEIR
  std::vector<ModelObject> CentralHeatPumpSystemModule_Impl::children() const
  {
    std::vector<ModelObject> result;

    // ChillerHeaterPerformanceElectricEIR ch_heater = chillerHeaterModulesPerformanceComponent();
    // result.push_back(ch_heater);

    // The chillerHeaterModulesControlSchedule, being a schedule, is already a resource so no need here

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

  // Convenience function to return parent CentralHeatPumpSystem
  boost::optional<CentralHeatPumpSystem> CentralHeatPumpSystemModule_Impl::centralHeatPumpSystem() const {

    boost::optional<CentralHeatPumpSystem> result;

    // loop on all CentralHeatPumpSystems in the model
    for ( const CentralHeatPumpSystem& central_hp : this->model().getConcreteModelObjects<CentralHeatPumpSystem>() )
    {
      // Loop on each CentralHeatPumpSystemModules
      for (const CentralHeatPumpSystemModule& central_hp_module : central_hp.modules() )
      {
        if ( central_hp_module.handle() == this->handle() )
        {
          result = central_hp;
        }
      }
    }
    return result;
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

boost::optional<CentralHeatPumpSystem> CentralHeatPumpSystemModule::centralHeatPumpSystem() const {
  return getImpl<detail::CentralHeatPumpSystemModule_Impl>()->centralHeatPumpSystem();
}

/// @cond
CentralHeatPumpSystemModule::CentralHeatPumpSystemModule(std::shared_ptr<detail::CentralHeatPumpSystemModule_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

