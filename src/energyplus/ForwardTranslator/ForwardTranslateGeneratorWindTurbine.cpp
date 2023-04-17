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
