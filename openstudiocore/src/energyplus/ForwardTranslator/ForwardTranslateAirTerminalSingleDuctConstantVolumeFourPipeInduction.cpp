/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeFourPipeInduction( AirTerminalSingleDuctConstantVolumeFourPipeInduction & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction, modelObject);

  // Availability Schedule Name
  if( (temp = modelObject.availabilitySchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Maximum Total Air Flow Rate
  if( (d = modelObject.maximumTotalAirFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate,d.get());
  }
  else if( modelObject.isMaximumTotalAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate,"Autosize");
  }

  // Induction Ratio
  if( (d = modelObject.inductionRatio()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio,d.get());
  }

  // Supply Air Inlet Node Name
  // Induced Air Inlet Node Name
  // Air Outlet Node Name

  // Hot Water Inlet Node Name
  // deprecated

  // Cold Water Inlet Node Name
  //deprecated

  // Heating Coil Object Type
  // Heating Coil Name

  // Maximum Hot Water Flow Rate
  if( (d = modelObject.maximumHotWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate,d.get());
  }
  else if( modelObject.isMaximumHotWaterFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate,"Autosize");
  }

  // Minimum Hot Water Flow Rate
  if( (d = modelObject.minimumHotWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate,d.get());
  }

  // Heating Convergence Tolerance
  if( (d = modelObject.heatingConvergenceTolerance()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance,d.get());
  }

  // Cooling Coil Object Type
  // Cooling Coil Name
  
  // Maximum Cold Water Flow Rate
  if( (d = modelObject.maximumColdWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate,d.get());
  }
  else if( modelObject.isMaximumColdWaterFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate,"Autosize");
  }

  // Minimum Cold Water Flow Rate
  if( (d = modelObject.minimumColdWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate,d.get());
  }

  // Cooling Convergence Tolerance
  if( (d = modelObject.coolingConvergenceTolerance()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance,d.get());
  }

  // Zone Mixer Name

  return idfObject;
}

} // energyplus

} // openstudio
