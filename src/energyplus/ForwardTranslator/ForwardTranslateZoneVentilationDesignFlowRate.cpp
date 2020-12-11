/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"
#include "../../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneVentilationDesignFlowRate(ZoneVentilationDesignFlowRate& modelObject) {
    auto idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneVentilation_DesignFlowRate, modelObject);

    if (auto value = modelObject.thermalZone()) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName, value->name().get());
    }

    {
      auto mo = modelObject.schedule();
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, mo.name().get());
    }

    {
      auto value = modelObject.designFlowRateCalculationMethod();
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, value);
    }

    {
      auto value = modelObject.designFlowRate();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::DesignFlowRate, value);
    }

    {
      auto value = modelObject.flowRateperZoneFloorArea();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea, value);
    }

    {
      auto value = modelObject.flowRateperPerson();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperPerson, value);
    }

    {
      auto value = modelObject.airChangesperHour();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::AirChangesperHour, value);
    }

    {
      auto value = modelObject.ventilationType();
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::VentilationType, value);
    }

    {
      auto value = modelObject.fanPressureRise();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FanPressureRise, value);
    }

    {
      auto value = modelObject.fanTotalEfficiency();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FanTotalEfficiency, value);
    }

    {
      auto value = modelObject.constantTermCoefficient();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient, value);
    }

    {
      auto value = modelObject.temperatureTermCoefficient();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient, value);
    }

    {
      auto value = modelObject.velocityTermCoefficient();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient, value);
    }

    {
      auto value = modelObject.velocitySquaredTermCoefficient();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient, value);
    }

    {
      auto value = modelObject.minimumIndoorTemperature();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperature, value);
    }

    if (auto value = modelObject.minimumIndoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumIndoorTemperature();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperature, value);
    }

    if (auto value = modelObject.maximumIndoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.deltaTemperature();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::DeltaTemperature, value);
    }

    if (auto value = modelObject.deltaTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.minimumOutdoorTemperature();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperature, value);
    }

    if (auto value = modelObject.minimumOutdoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumOutdoorTemperature();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperature, value);
    }

    if (auto value = modelObject.maximumOutdoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumWindSpeed();
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MaximumWindSpeed, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
