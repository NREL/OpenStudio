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
#include "../../model/ZoneControlHumidistat.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/ZoneControl_Humidistat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneControlHumidistat( ZoneControlHumidistat& modelObject )
{
  boost::optional<IdfObject> result;

  auto humidifyingSchedule = modelObject.humidifyingRelativeHumiditySetpointSchedule();
  auto dehumidifyingSchedule = modelObject.dehumidifyingRelativeHumiditySetpointSchedule();

  if( humidifyingSchedule || dehumidifyingSchedule ) {
    // Name
    result = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneControl_Humidistat, modelObject);

    // Zone Name
    // set by ThermalZone

    // Humidifying Relative Humidity Setpoint Schedule Name
    if( humidifyingSchedule ) {
      boost::optional<IdfObject> _humidifyingSchedule = translateAndMapModelObject(humidifyingSchedule.get());

      if( _humidifyingSchedule && _humidifyingSchedule->name() ) {
        result->setString(ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName,_humidifyingSchedule->name().get());
      }
    }

    // Dehumidifying Relative Humidity Setpoint Schedule Name
    if( dehumidifyingSchedule ) {
      boost::optional<IdfObject> _dehumidifyingSchedule = translateAndMapModelObject(dehumidifyingSchedule.get());

      if( _dehumidifyingSchedule && _dehumidifyingSchedule->name() ) {
        result->setString(ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName,_dehumidifyingSchedule->name().get());
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio
