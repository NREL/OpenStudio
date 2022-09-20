/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ZoneMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneMixing(ZoneMixing& modelObject) {
    // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in
    // the final file. Also set's the idfObject's name.
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneMixing, modelObject);

    boost::optional<double> value;

    // ZoneorSpaceName
    auto zoneorSpace = modelObject.zoneOrSpace();
    translateAndMapModelObject(zoneorSpace);
    idfObject.setString(ZoneMixingFields::ZoneorSpaceName, zoneorSpace.nameString());

    // ScheduleName
    Schedule schedule = modelObject.schedule();
    translateAndMapModelObject(schedule);
    idfObject.setString(ZoneMixingFields::ScheduleName, schedule.nameString());

    // DesignFlowRateCalculationMethod
    idfObject.setString(ZoneMixingFields::DesignFlowRateCalculationMethod, modelObject.designFlowRateCalculationMethod());

    // DesignFlowRate
    value = modelObject.designFlowRate();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::DesignFlowRate, *value);
    }

    // FlowRateperFloorArea
    value = modelObject.flowRateperFloorArea();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::FlowRateperFloorArea, *value);
    }

    // FlowRateperPerson
    value = modelObject.flowRateperPerson();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::FlowRateperPerson, *value);
    }

    // AirChangesperHour
    value = modelObject.airChangesperHour();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::AirChangesperHour, *value);
    }

    // SourceZoneorSpaceName
    if (boost::optional<ModelObject> sourceZoneOrSpace = modelObject.sourceZoneOrSpace()) {
      // DLM: do not translate source zone now, it will be translated at the right time
      idfObject.setString(ZoneMixingFields::SourceZoneorSpaceName, sourceZoneOrSpace->nameString());
    }

    // DeltaTemperature
    value = modelObject.deltaTemperature();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::DeltaTemperature, *value);
    }

    // DeltaTemperatureScheduleName
    boost::optional<Schedule> optSchedule = modelObject.deltaTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::DeltaTemperatureScheduleName, optSchedule->nameString());
    }

    // MinimumReceivingTemperatureScheduleName
    optSchedule = modelObject.minimumReceivingTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MinimumReceivingTemperatureScheduleName, optSchedule->nameString());
    }

    // MaximumReceivingTemperatureScheduleName
    optSchedule = modelObject.maximumReceivingTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MaximumReceivingTemperatureScheduleName, optSchedule->nameString());
    }

    // MinimumSourceTemperatureScheduleName
    optSchedule = modelObject.minimumSourceTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MinimumSourceTemperatureScheduleName, optSchedule->nameString());
    }

    // MaximumSourceTemperatureScheduleName
    optSchedule = modelObject.maximumSourceTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MaximumSourceTemperatureScheduleName, optSchedule->nameString());
    }

    // MinimumOutdoorTemperatureScheduleName
    optSchedule = modelObject.minimumOutdoorTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName, optSchedule->nameString());
    }

    // MaximumOutdoorTemperatureScheduleName
    optSchedule = modelObject.maximumOutdoorTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, optSchedule->nameString());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
