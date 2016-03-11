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

  // SourceLoopInletNodeName
  if( auto mo = modelObject.supplyInletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::SourceLoopInletNodeName,node->name().get());
    }
  }

  // SourceLoopOutletNodeName
  if( auto mo = modelObject.supplyOutletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::SourceLoopOutletNodeName,node->name().get());
    }
  }

  // CoolingLoopInletNodeName
  if( auto mo = modelObject.demandInletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::CoolingLoopInletNodeName,node->name().get());
    }
  }

  // CoolingLoopOutletNodeName
  if( auto mo = modelObject.demandOutletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(CentralHeatPumpSystemFields::CoolingLoopOutletNodeName,node->name().get());
    }
  }

  // HeatingLoopInletNodeName
  // HeatingLoopOutletNodeName

  // ChillerHeaterModulesPerformanceComponentObjectType1
  // ChillerHeaterModulesPerformanceComponentName1
  // ChillerHeaterModulesControlScheduleName1
  // NumberofChillerHeaterModules1
  auto const& modules = modelObject.getImpl<model::detail::CentralHeatPumpSystem_Impl>()->modules();
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
        idfObject.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesControlScheduleName,_schedule->name().get());
     }
    }
    group.setInt(CentralHeatPumpSystemExtensibleFields::NumberofChillerHeaterModules, module.numberofChillerHeaterModules() );
  }

  return idfObject;
}

} // energyplus

} // openstudio

