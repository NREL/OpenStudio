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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/FanVariableVolume.hpp>
#include <model/FanVariableVolume_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Schedule.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFanVariableVolume( FanVariableVolume& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Create a new IddObjectType::Fan_VariableVolume
  IdfObject idfObject(IddObjectType::Fan_VariableVolume);

  m_idfObjects.push_back(idfObject);

  // Field: Name ////////////////////////////////////////////////////////////
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<model::AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC() )
  {
    Schedule sched = airLoopHVAC->availabilitySchedule();
    boost::optional<IdfObject> schedIdf = translateAndMapModelObject(sched);
    if( schedIdf )
    {
      idfObject.setString(Fan_VariableVolumeFields::AvailabilityScheduleName,schedIdf->name().get());
    }
  }
  else
  {
    Schedule sched = modelObject.availabilitySchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(Fan_VariableVolumeFields::AvailabilityScheduleName,sched.name().get());
  }

  // Fan Efficiency /////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_VariableVolumeFields::FanTotalEfficiency,modelObject.fanEfficiency());

  // Pressure Rise //////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_VariableVolumeFields::PressureRise,modelObject.pressureRise());

  // Maximum Flow Rate //////////////////////////////////////////////////////
  if( modelObject.isMaximumFlowRateAutosized() )
  {
    idfObject.setString(openstudio::Fan_VariableVolumeFields::MaximumFlowRate,"AutoSize");
  }
  else if( (d = modelObject.maximumFlowRate()) )
  {
    idfObject.setDouble(openstudio::Fan_VariableVolumeFields::MaximumFlowRate,d.get());
  }

  // FanPowerMinimumFlowRateInputMethod
  if( (s = modelObject.fanPowerMinimumFlowRateInputMethod()) )
  {
    idfObject.setString(Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod,s.get());
  }

  // FanPowerMinimumFlowFraction
  if( (d = modelObject.fanPowerMinimumFlowFraction()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerMinimumFlowFraction,d.get());
  }

  // FanPowerMinimumAirFlowRate

  if( (d = modelObject.fanPowerMinimumAirFlowRate()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate,d.get());
  }

  // Motor Efficiency ///////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_VariableVolumeFields::MotorEfficiency,modelObject.motorEfficiency());

  // FanPowerCoefficient1

  if( (d = modelObject.fanPowerCoefficient1()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerCoefficient1,d.get());
  }

  // FanPowerCoefficient2

  if( (d = modelObject.fanPowerCoefficient2()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerCoefficient2,d.get());
  }

  // FanPowerCoefficient3

  if( (d = modelObject.fanPowerCoefficient3()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerCoefficient3,d.get());
  }

  // FanPowerCoefficient4

  if( (d = modelObject.fanPowerCoefficient4()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerCoefficient4,d.get());
  }

  // FanPowerCoefficient5

  if( (d = modelObject.fanPowerCoefficient5()) )
  {
    idfObject.setDouble(Fan_VariableVolumeFields::FanPowerCoefficient5,d.get());
  }

  // Motor In Airstream Fraction ////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_VariableVolumeFields::MotorInAirstreamFraction,modelObject.motorInAirstreamFraction());

  // Air Inlet Node Name ////////////////////////////////////////////////////
  temp = modelObject.inletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(openstudio::Fan_VariableVolumeFields::AirInletNodeName,*s);
    }
  }

  // Air Outlet Node Name ///////////////////////////////////////////////////
  temp = modelObject.outletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(openstudio::Fan_VariableVolumeFields::AirOutletNodeName,*s);
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

