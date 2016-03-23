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
#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/HeatExchangerFluidToFluid_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/HeatExchanger_FluidToFluid_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatExchangerFluidToFluid( HeatExchangerFluidToFluid & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject mo;
  boost::optional<IdfObject> idfo;

  IdfObject idfObject(IddObjectType::HeatExchanger_FluidToFluid);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName
  if( boost::optional<Schedule> sched = modelObject.availabilitySchedule() )
  {
    if( (idfo = translateAndMapModelObject(sched.get())) )
    {
      idfObject.setString(HeatExchanger_FluidToFluidFields::AvailabilityScheduleName,idfo->name().get());
    }
  }

  // LoopDemandSideInletNode
  mo = modelObject.demandInletModelObject();
  if( mo )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::LoopDemandSideInletNodeName,mo->name().get());
  }

  // LoopDemandSideOutletNode
  mo = modelObject.demandOutletModelObject();
  if( mo )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::LoopDemandSideOutletNodeName,mo->name().get());
  }

  // LoopDemandSideDesignFlowRate
  if( modelObject.isLoopDemandSideDesignFlowRateAutosized() )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate,"Autosize");
  }
  else
  {
    if( (d = modelObject.loopDemandSideDesignFlowRate()) )
    {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate,d.get());
    }
  }

  // LoopSupplySideInletNode
  mo = modelObject.supplyInletModelObject();
  if( mo )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::LoopSupplySideInletNodeName,mo->name().get());
  }

  // LoopSupplySideOutletNode
  mo = modelObject.supplyOutletModelObject();
  if( mo )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::LoopSupplySideOutletNodeName,mo->name().get());
  }

  // LoopSupplySideDesignFlowRate
  if( modelObject.isLoopSupplySideDesignFlowRateAutosized() )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate,"Autosize");
  }
  else
  {
    if( (d = modelObject.loopSupplySideDesignFlowRate()) )
    {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate,d.get());
    }
  }

  // HeatExchangeModelType
  if( (s = modelObject.heatExchangeModelType()) )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangeModelType,s.get());
  }

  // HeatExchangerUFactorTimesAreaValue
  if( modelObject.isHeatExchangerUFactorTimesAreaValueAutosized() )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue,"Autosize");
  }
  else
  {
    if( (d = modelObject.heatExchangerUFactorTimesAreaValue()) )
    {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue,d.get());
    }
  }

  // ControlType
  if( (s = modelObject.controlType()) )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::ControlType,s.get());
  }

  // HeatExchangerSetpointNodeName
  if( (mo = modelObject.supplyOutletModelObject()) )
  {
    auto node = mo->optionalCast<Node>();
    OS_ASSERT(node);
    if( ! node->setpointManagers().empty() ) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangerSetpointNodeName,mo->name().get());
    } else {
      auto plant = node->plantLoop();
      OS_ASSERT(plant);
      auto supplyOutletModelObject = plant->supplyOutletNode();
      idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangerSetpointNodeName,supplyOutletModelObject.name().get());
    }
  }

  // MinimumTemperatureDifferencetoActivateHeatExchanger
  if(  (d = modelObject.minimumTemperatureDifferencetoActivateHeatExchanger()) )
  {
    idfObject.setDouble(HeatExchanger_FluidToFluidFields::MinimumTemperatureDifferencetoActivateHeatExchanger,d.get());
  }

  // HeatTransferMeteringEndUseType
  if( (s = modelObject.heatTransferMeteringEndUseType()) )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType,s.get());
  }

  // ComponentOverrideLoopSupplySideInletNode
  if( (mo = modelObject.componentOverrideLoopSupplySideInletNode()) )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::ComponentOverrideLoopSupplySideInletNodeName,mo->name().get());
  }

  // ComponentOverrideLoopDemandSideInletNode
  if( (mo = modelObject.componentOverrideLoopDemandSideInletNode()) )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::ComponentOverrideLoopDemandSideInletNodeName,mo->name().get());
  }

  // ComponentOverrideCoolingControlTemperatureMode
  if( (s = modelObject.componentOverrideCoolingControlTemperatureMode()) )
  {
    idfObject.setString(HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode,s.get());
  }

  // SizingFactor
  if( (d = modelObject.sizingFactor()) )
  {
    idfObject.setDouble(HeatExchanger_FluidToFluidFields::SizingFactor,d.get());
  }

  // OperationMinimumTemperatureLimit
  if( (d = modelObject.operationMinimumTemperatureLimit()) )
  {
    idfObject.setDouble(HeatExchanger_FluidToFluidFields::OperationMinimumTemperatureLimit,d.get());
  }

  // OperationMaximumTemperatureLimit
  if( (d = modelObject.operationMaximumTemperatureLimit()) )
  {
    idfObject.setDouble(HeatExchanger_FluidToFluidFields::OperationMaximumTemperatureLimit,d.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

