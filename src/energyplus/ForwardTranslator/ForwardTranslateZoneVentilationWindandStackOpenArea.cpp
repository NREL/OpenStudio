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
#include "../../model/ZoneVentilationWindandStackOpenArea.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include <utilities/idd/ZoneVentilation_WindandStackOpenArea_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneVentilationWindandStackOpenArea(ZoneVentilationWindandStackOpenArea& modelObject) {
    auto idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneVentilation_WindandStackOpenArea, modelObject);

    // Note: no need to really handle the case where it doesn't exist since ThermalZone is the one calling this translation to begin with
    if (boost::optional<ThermalZone> value = modelObject.thermalZone()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::ZoneName, value->name().get());
    }

    {
      double value = modelObject.openingArea();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningArea, value);
    }

    {
      Schedule schedule = modelObject.openingAreaFractionSchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::OpeningAreaFractionScheduleName, _schedule->name().get());
      }
    }

    if (modelObject.isOpeningEffectivenessAutocalculated()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, "Autocalculate");
    } else {
      if (boost::optional<double> value = modelObject.openingEffectiveness()) {
        idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::OpeningEffectiveness, value.get());
      }
    }

    {
      double value = modelObject.effectiveAngle();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::EffectiveAngle, value);
    }

    {
      double value = modelObject.heightDifference();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::HeightDifference, value);
    }

    if (modelObject.isDischargeCoefficientforOpeningAutocalculated()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, "Autocalculate");
    } else {
      if (boost::optional<double> value = modelObject.dischargeCoefficientforOpening()) {
        idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::DischargeCoefficientforOpening, value.get());
      }
    }

    {
      auto value = modelObject.minimumIndoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperature, value);
    }

    if (auto value = modelObject.minimumIndoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MinimumIndoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumIndoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperature, value);
    }

    if (auto value = modelObject.maximumIndoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MaximumIndoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.deltaTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperature, value);
    }

    if (auto value = modelObject.deltaTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::DeltaTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.minimumOutdoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperature, value);
    }

    if (auto value = modelObject.minimumOutdoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MinimumOutdoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumOutdoorTemperature();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperature, value);
    }

    if (auto value = modelObject.maximumOutdoorTemperatureSchedule()) {
      idfObject.setString(ZoneVentilation_WindandStackOpenAreaFields::MaximumOutdoorTemperatureScheduleName, value->name().get());
    }

    {
      auto value = modelObject.maximumWindSpeed();
      idfObject.setDouble(ZoneVentilation_WindandStackOpenAreaFields::MaximumWindSpeed, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
