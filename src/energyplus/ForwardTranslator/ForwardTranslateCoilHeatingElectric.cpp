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
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingElectric_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingElectric( CoilHeatingElectric & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Coil_Heating_Electric);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(Coil_Heating_ElectricFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Efficiency

  if( (value = modelObject.efficiency()) )
  {
    idfObject.setDouble(Coil_Heating_ElectricFields::Efficiency,value.get());
  }

  // Nominal Capacity

  if( modelObject.isNominalCapacityAutosized() )
  {
    idfObject.setString(Coil_Heating_ElectricFields::NominalCapacity,"Autosize");
  }
  else if( (value = modelObject.nominalCapacity()) )
  {
    idfObject.setDouble(Coil_Heating_ElectricFields::NominalCapacity,value.get());
  }

  // Air Inlet Node Name

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_ElectricFields::AirInletNodeName,node->name().get());
    }
  }

  // Air Outlet Node Name

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_ElectricFields::AirOutletNodeName,node->name().get());
      // For now we write the temp setpoint node as the Coil Outlet Node
      idfObject.setString(Coil_Heating_ElectricFields::TemperatureSetpointNodeName,node->name().get());
    }
  }

  // Temperature Setpoint Node Name
  // If it was hardset we actually use that, otherwise keep above default (coil outlet)
  if( boost::optional<Node> node = modelObject.temperatureSetpointNode() )
  {
    idfObject.setString(Coil_Heating_ElectricFields::TemperatureSetpointNodeName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

