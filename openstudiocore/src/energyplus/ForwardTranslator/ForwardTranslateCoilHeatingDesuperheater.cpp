/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilHeatingDesuperheater.hpp"

#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDesuperheater(CoilHeatingDesuperheater & modelObject)
{
  OptionalModelObject omo;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> d;

// Name
  IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_Desuperheater, modelObject);

// Availability Schedule Name
  boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

  if( availabilitySchedule )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      object.setString(Coil_Heating_DesuperheaterFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

// Heat Recovery Efficiency
  d = modelObject.heatReclaimRecoveryEfficiency();
  if (d) {
      object.setDouble(Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency,d.get());
  }

// Air Inlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      object.setString(Coil_Heating_DesuperheaterFields::AirInletNodeName,node->name().get());
    }
  }

// Air Outlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      object.setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName,node->name().get());
      object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName,node->name().get());
    }
  }

// HeatingSourceObjectType
// Heating Source Name
  boost::optional<ModelObject> heatingSource = modelObject.heatingSource();

  if ( heatingSource ) {
      boost::optional<IdfObject> _heatingSource = translateAndMapModelObject(heatingSource.get());

      if ( _heatingSource && _heatingSource->name() )
      {
        object.setString(Coil_Heating_DesuperheaterFields::HeatingSourceObjectType,_heatingSource->iddObject().name());
        object.setString(Coil_Heating_DesuperheaterFields::HeatingSourceName,_heatingSource->name().get());
      }
  }

// Temperature Setpoint Node Name
  // if( boost::optional<Node> node = modelObject.temperatureSetpointNode() )
  // {
  //   object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName,node->name().get());
  // }
  // object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName,"");

// Parasitic Electric Load
  d = modelObject.parasiticElectricLoad();
  if (d) {
      object.setDouble(Coil_Heating_DesuperheaterFields::ParasiticElectricLoad,d.get());
  }

  return object;

}
}
}
