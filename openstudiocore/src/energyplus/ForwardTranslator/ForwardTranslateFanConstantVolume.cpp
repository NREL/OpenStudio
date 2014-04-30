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
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Schedule.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateFanConstantVolume( FanConstantVolume& modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  // Create a new IddObjectType::Fan_ConstantVolume
  IdfObject idfObject(IddObjectType::Fan_ConstantVolume);

  ///////////////////////////////////////////////////////////////////////////
  // Field: Name ////////////////////////////////////////////////////////////
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }
  ///////////////////////////////////////////////////////////////////////////


  // hook up required objects
  try {
    if( boost::optional<model::AirLoopHVAC> airLoopHVAC = modelObject.airLoopHVAC() )
    {
      Schedule sched = airLoopHVAC->availabilitySchedule();
      boost::optional<IdfObject> schedIdf = translateAndMapModelObject(sched);
      if( schedIdf )
      {
        idfObject.setString(Fan_ConstantVolumeFields::AvailabilityScheduleName,schedIdf->name().get());
      }
    }
    else
    {
      Schedule sched = modelObject.availabilitySchedule();
      translateAndMapModelObject(sched);
      idfObject.setString(Fan_ConstantVolumeFields::AvailabilityScheduleName,sched.name().get());
    }
  }
  catch (std::exception& e) {
    LOG(Error,"Could not translate " << modelObject.briefDescription() << ", because " 
        << e.what() << ".");
    return boost::none;
  }


  ///////////////////////////////////////////////////////////////////////////
  // Fan Efficiency /////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::FanTotalEfficiency,modelObject.fanEfficiency());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Pressure Rise //////////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::PressureRise,modelObject.pressureRise());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Maximum Flow Rate //////////////////////////////////////////////////////
  d = modelObject.maximumFlowRate();
  if(d)
  {
    idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::MaximumFlowRate,*d);
  }
  else
  {
    idfObject.setString(openstudio::Fan_ConstantVolumeFields::MaximumFlowRate,"AutoSize");
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Motor Efficiency ///////////////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::MotorEfficiency,modelObject.motorEfficiency());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Motor In Airstream Fraction ////////////////////////////////////////////
  idfObject.setDouble(openstudio::Fan_ConstantVolumeFields::MotorInAirstreamFraction,modelObject.motorInAirstreamFraction());
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Air Inlet Node Name ////////////////////////////////////////////////////
  temp = modelObject.inletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(openstudio::Fan_ConstantVolumeFields::AirInletNodeName,*s);
    }
  }
  ///////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////
  // Air Outlet Node Name ///////////////////////////////////////////////////
  temp = modelObject.outletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(openstudio::Fan_ConstantVolumeFields::AirOutletNodeName,*s);
    }
  }
  ///
  ////////////////////////////////////////////////////////////////////////

  m_idfObjects.push_back(idfObject);
  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

