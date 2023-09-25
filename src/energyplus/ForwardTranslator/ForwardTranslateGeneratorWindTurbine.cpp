/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorWindTurbine.hpp"
#include "../../model/GeneratorWindTurbine_Impl.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Generator_WindTurbine_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  /** This method forward translates the OS:Generator:WindTurbine as well the optional attached OS:Generator:WindTurbine:HeatRecovery
 to the EnergyPlus Generator:WindTurbine object */
  boost::optional<IdfObject> ForwardTranslator::translateGeneratorWindTurbine(GeneratorWindTurbine& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<int> i;

    // We are going to check for valid rotor type and blade fields before we register the object
    IdfObject idfObject(openstudio::IddObjectType::Generator_WindTurbine);

    // Name
    idfObject.setName(modelObject.nameString());

    // Rotor Type
    std::string rotorType = modelObject.rotorType();
    idfObject.setString(Generator_WindTurbineFields::RotorType, rotorType);

    // Blade Chord Area
    double bladeChordArea = modelObject.bladeChordArea();
    idfObject.setDouble(Generator_WindTurbineFields::BladeChordArea, bladeChordArea);

    // Blade Drag Coefficient
    double bladeDragCoefficient = modelObject.bladeDragCoefficient();
    idfObject.setDouble(Generator_WindTurbineFields::BladeDragCoefficient, bladeDragCoefficient);

    // Blade Lift Coefficient
    double bladeLiftCoefficient = modelObject.bladeLiftCoefficient();
    idfObject.setDouble(Generator_WindTurbineFields::BladeLiftCoefficient, bladeLiftCoefficient);

    // Hard check instead of letting E+ crash for required fields depending on rotor type
    if (openstudio::istringEqual("VerticalAxisWindTurbine", rotorType)
        && ((bladeChordArea == 0) || (bladeDragCoefficient == 0) || (bladeLiftCoefficient == 0))) {
      LOG(Error, modelObject.briefDescription() << ": When 'Rotor Type' == 'VerticalAxisWindTurbine',"
                                                << "'Blade Chord Area', 'Blade Drag Coefficient' and 'Blade Lift Coefficient' cannot be zero."
                                                << " It will not be translated'");
      return boost::none;
    }

    // at this point, we can register the new object
    m_idfObjects.push_back(idfObject);

    // Availability Schedule Name
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        idfObject.setString(Generator_WindTurbineFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    // Power Control
    s = modelObject.powerControl();
    if (s) {
      idfObject.setString(Generator_WindTurbineFields::PowerControl, s.get());
    }

    // Rated Rotor Speed
    d = modelObject.ratedRotorSpeed();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::RatedRotorSpeed, d.get());
    }

    // Rotor Diameter
    d = modelObject.rotorDiameter();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::RotorDiameter, d.get());
    }

    // Overall Height
    d = modelObject.overallHeight();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::OverallHeight, d.get());
    }

    // Number of Blades
    i = modelObject.numberofBlades();
    if (i) {
      idfObject.setInt(Generator_WindTurbineFields::NumberofBlades, i.get());
    }

    // Rated Power
    d = modelObject.ratedPower();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::RatedPower, d.get());
    }

    // Rated Wind Speed
    d = modelObject.ratedWindSpeed();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::RatedWindSpeed, d.get());
    }

    // Cut In Wind Speed
    d = modelObject.cutInWindSpeed();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::CutInWindSpeed, d.get());
    }

    // Cut Out Wind Speed
    d = modelObject.cutOutWindSpeed();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::CutOutWindSpeed, d.get());
    }

    // Fraction system Efficiency
    d = modelObject.fractionSystemEfficiency();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::FractionsystemEfficiency, d.get());
    }

    // Maximum Tip Speed Ratio
    d = modelObject.maximumTipSpeedRatio();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::MaximumTipSpeedRatio, d.get());
    }

    // Maximum Power Coefficient
    d = modelObject.maximumPowerCoefficient();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::MaximumPowerCoefficient, d.get());
    }

    // Annual Local Average Wind Speed
    d = modelObject.annualLocalAverageWindSpeed();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::AnnualLocalAverageWindSpeed, d.get());
    }

    // Height for Local Average Wind Speed
    d = modelObject.heightforLocalAverageWindSpeed();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::HeightforLocalAverageWindSpeed, d.get());
    }

    // Power Coefficient C1
    d = modelObject.powerCoefficientC1();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::PowerCoefficientC1, d.get());
    }

    // Power Coefficient C2
    d = modelObject.powerCoefficientC2();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::PowerCoefficientC2, d.get());
    }

    // Power Coefficient C3
    d = modelObject.powerCoefficientC3();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::PowerCoefficientC3, d.get());
    }

    // Power Coefficient C4
    d = modelObject.powerCoefficientC4();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::PowerCoefficientC4, d.get());
    }

    // Power Coefficient C5
    d = modelObject.powerCoefficientC5();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::PowerCoefficientC5, d.get());
    }

    // Power Coefficient C6
    d = modelObject.powerCoefficientC6();
    if (d) {
      idfObject.setDouble(Generator_WindTurbineFields::PowerCoefficientC6, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
