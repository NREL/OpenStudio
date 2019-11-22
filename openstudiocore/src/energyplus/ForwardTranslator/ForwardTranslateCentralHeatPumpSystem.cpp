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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CentralHeatPumpSystem.hpp"
#include "../../model/CentralHeatPumpSystem_Impl.hpp"
#include "../../model/CentralHeatPumpSystemModule.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/CentralHeatPumpSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCentralHeatPumpSystem( CentralHeatPumpSystem& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  auto const& modules = modelObject.getImpl<model::detail::CentralHeatPumpSystem_Impl>()->modules();

  // If the CentralHeatPumpSystem doesn't have at least one CentralHeatPumpSystemModule, then it shouldn't be translated
  if (modules.empty()) {
    LOG(Warn, "CentralHeatPumpSystem " << modelObject.name().get() << " has no CentralHeatPumpSystemModules, it will not be translated");
    return boost::none;
  }

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::CentralHeatPumpSystem, modelObject);

  // ControlMethod
  if( (s = modelObject.controlMethod()) ) {
    idfObject.setString(CentralHeatPumpSystemFields::ControlMethod,s.get());
  }

  // AncillaryPower
  if( (d = modelObject.ancillaryPower()) ) {
    idfObject.setDouble(CentralHeatPumpSystemFields::AncillaryPower,d.get());
  }

  // AncillaryOperationScheduleName
  {
   if( auto schedule = modelObject.ancillaryOperationSchedule() ) {
     if( auto _schedule = translateAndMapModelObject(schedule.get()) ) {
       idfObject.setString(CentralHeatPumpSystemFields::AncillaryOperationScheduleName,_schedule->name().get());
     }
   }
  }

  // supply = Cooling Loop, demand=Source Loop, tertiary = Heating Loop

  // CoolingLoopInletNodeName
  if( auto mo = modelObject.supplyInletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::CoolingLoopInletNodeName,node->name().get());
    }
  }

  // CoolingLoopOutletNodeName
  if( auto mo = modelObject.supplyOutletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::CoolingLoopOutletNodeName,node->name().get());
    }
  }

  // SourceLoopInletNodeName
  if( auto mo = modelObject.demandInletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::SourceLoopInletNodeName,node->name().get());
    }
  }

  // SourceLoopOutletNodeName
  if( auto mo = modelObject.demandOutletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::SourceLoopOutletNodeName,node->name().get());
    }
  }


  // HeatingLoopInletNodeName
  if ( auto mo = modelObject.tertiaryInletModelObject() ) {
    if ( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::HeatingLoopInletNodeName, node->name().get());
    }
  }
  // HeatingLoopOutletNodeName
  if ( auto mo = modelObject.tertiaryOutletModelObject() ) {
    if ( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::HeatingLoopOutletNodeName, node->name().get());
    }
  }

  // ChillerHeaterModulesPerformanceComponentObjectType1
  // ChillerHeaterModulesPerformanceComponentName1
  // ChillerHeaterModulesControlScheduleName1
  // NumberofChillerHeaterModules1

  for ( auto const& module : modules ) {
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

    auto performanceComponent = module.chillerHeaterModulesPerformanceComponent();
    if( auto _performanceComponent = translateAndMapModelObject(performanceComponent) ) {
      group.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesPerformanceComponentObjectType, _performanceComponent->iddObject().name());
      group.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesPerformanceComponentName, _performanceComponent->name().get());
    }
    {
      auto schedule = module.chillerHeaterModulesControlSchedule();
      if( auto _schedule = translateAndMapModelObject(schedule) ) {
        group.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesControlScheduleName,_schedule->name().get());
     }
    }
    group.setInt(CentralHeatPumpSystemExtensibleFields::NumberofChillerHeaterModules, module.numberofChillerHeaterModules() );
  }

  return idfObject;
}

} // energyplus

} // openstudio

