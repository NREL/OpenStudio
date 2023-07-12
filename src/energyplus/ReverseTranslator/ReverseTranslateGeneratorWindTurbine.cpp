/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
