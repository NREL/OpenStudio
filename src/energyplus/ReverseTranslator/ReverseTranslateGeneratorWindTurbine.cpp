/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ReverseTranslator.hpp"

#include "../../model/GeneratorWindTurbine.hpp"
#include "../../model/GeneratorWindTurbine_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Generator_WindTurbine_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateGeneratorWindTurbine(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Generator_WindTurbine) {
      LOG(Error, "WorkspaceObject is not IddObjectType: GeneratorWindTurbine");
      return boost::none;
    }

    OptionalDouble d;
    OptionalString s;
    OptionalInt i;
    OptionalWorkspaceObject target;

    openstudio::model::GeneratorWindTurbine wind(m_model);

    // Name
    s = workspaceObject.name();
    if (s) {
      wind.setName(*s);
    }

    // Availability Schedule Name
    if ((target = workspaceObject.getTarget(Generator_WindTurbineFields::AvailabilityScheduleName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optSch = modelObject->optionalCast<Schedule>()) {
          wind.setAvailabilitySchedule(optSch.get());
        }
      }
    }

    // Rotor Type
    s = workspaceObject.getString(Generator_WindTurbineFields::RotorType);
    if (s) {
      wind.setRotorType(*s);
    }

    // Power Control
    s = workspaceObject.getString(Generator_WindTurbineFields::PowerControl);
    if (s) {
      wind.setPowerControl(*s);
    }

    // Rated Rotor Speed
    d = workspaceObject.getDouble(Generator_WindTurbineFields::RatedRotorSpeed);
    if (d) {
      wind.setRatedRotorSpeed(*d);
    }

    // Rotor Diameter
    d = workspaceObject.getDouble(Generator_WindTurbineFields::RotorDiameter);
    if (d) {
      wind.setRotorDiameter(*d);
    }

    // Overall Height
    d = workspaceObject.getDouble(Generator_WindTurbineFields::OverallHeight);
    if (d) {
      wind.setOverallHeight(*d);
    }

    // Number of Blades
    i = workspaceObject.getInt(Generator_WindTurbineFields::NumberofBlades);
    if (i) {
      wind.setNumberofBlades(*i);
    }

    // Rated Power
    d = workspaceObject.getDouble(Generator_WindTurbineFields::RatedPower);
    if (d) {
      wind.setRatedPower(*d);
    }

    // Rated Wind Speed
    d = workspaceObject.getDouble(Generator_WindTurbineFields::RatedWindSpeed);
    if (d) {
      wind.setRatedWindSpeed(*d);
    }

    // Cut In Wind Speed
    d = workspaceObject.getDouble(Generator_WindTurbineFields::CutInWindSpeed);
    if (d) {
      wind.setCutInWindSpeed(*d);
    }

    // Cut Out Wind Speed
    d = workspaceObject.getDouble(Generator_WindTurbineFields::CutOutWindSpeed);
    if (d) {
      wind.setCutOutWindSpeed(*d);
    }

    // Fraction system Efficiency
    d = workspaceObject.getDouble(Generator_WindTurbineFields::FractionsystemEfficiency);
    if (d) {
      wind.setFractionSystemEfficiency(*d);
    }

    // Maximum Tip Speed Ratio
    d = workspaceObject.getDouble(Generator_WindTurbineFields::MaximumTipSpeedRatio);
    if (d) {
      wind.setMaximumTipSpeedRatio(*d);
    }

    // Maximum Power Coefficient
    d = workspaceObject.getDouble(Generator_WindTurbineFields::MaximumPowerCoefficient);
    if (d) {
      wind.setMaximumPowerCoefficient(*d);
    }

    // Annual Local Average Wind Speed
    d = workspaceObject.getDouble(Generator_WindTurbineFields::AnnualLocalAverageWindSpeed);
    if (d) {
      wind.setAnnualLocalAverageWindSpeed(*d);
    }

    // Height for Local Average Wind Speed
    d = workspaceObject.getDouble(Generator_WindTurbineFields::HeightforLocalAverageWindSpeed);
    if (d) {
      wind.setHeightforLocalAverageWindSpeed(*d);
    }

    // Blade Chord Area
    d = workspaceObject.getDouble(Generator_WindTurbineFields::BladeChordArea);
    if (d) {
      wind.setBladeChordArea(*d);
    }

    // Blade Drag Coefficient
    d = workspaceObject.getDouble(Generator_WindTurbineFields::BladeDragCoefficient);
    if (d) {
      wind.setBladeDragCoefficient(*d);
    }

    // Blade Lift Coefficient
    d = workspaceObject.getDouble(Generator_WindTurbineFields::BladeLiftCoefficient);
    if (d) {
      wind.setBladeLiftCoefficient(*d);
    }

    // Power Coefficient C1
    d = workspaceObject.getDouble(Generator_WindTurbineFields::PowerCoefficientC1);
    if (d) {
      wind.setPowerCoefficientC1(*d);
    }

    // Power Coefficient C2
    d = workspaceObject.getDouble(Generator_WindTurbineFields::PowerCoefficientC2);
    if (d) {
      wind.setPowerCoefficientC2(*d);
    }

    // Power Coefficient C3
    d = workspaceObject.getDouble(Generator_WindTurbineFields::PowerCoefficientC3);
    if (d) {
      wind.setPowerCoefficientC3(*d);
    }

    // Power Coefficient C4
    d = workspaceObject.getDouble(Generator_WindTurbineFields::PowerCoefficientC4);
    if (d) {
      wind.setPowerCoefficientC4(*d);
    }

    // Power Coefficient C5
    d = workspaceObject.getDouble(Generator_WindTurbineFields::PowerCoefficientC5);
    if (d) {
      wind.setPowerCoefficientC5(*d);
    }

    // Power Coefficient C6
    d = workspaceObject.getDouble(Generator_WindTurbineFields::PowerCoefficientC6);
    if (d) {
      wind.setPowerCoefficientC6(*d);
    }

    return wind;
  }

}  // namespace energyplus

}  // namespace openstudio
