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
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

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
  // Fan Total Efficiency ///////////////////////////////////////////////////
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

  // End Use Subcategory
  idfObject.setString(Fan_ConstantVolumeFields::EndUseSubcategory, modelObject.endUseSubcategory());

  m_idfObjects.push_back(idfObject);
  return idfObject;
}

} // energyplus

} // openstudio
