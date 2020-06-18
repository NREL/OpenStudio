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

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include "../utilities/units/Unit.hpp"
#include "../utilities/units/UnitFactory.hpp"

#include <string_view>
#include <array>
#include <algorithm>
#include <pugixml.hpp>

namespace openstudio {
namespace gbxml {

  Unit ReverseTranslator::lengthUnitFromEnum(std::string_view lengthUnitEnumString) const {

    // {'Kilometers'|'Meters'|'Centimeters'|'Millimeters'|'Miles'|'Yards'|'Feet'|'Inches'}

    if (lengthUnitEnumString.empty()) {
      LOG(Warn, "No length unit specified, using default " << m_lengthUnit);
      return m_temperatureUnit;
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 8> enumToUnitStrings = {{
      {"Meters", "m"},
        {"Kilometers", "km"},
        {"Centimeters", "cm"},
        {"Millimeters", "mm"},
        {"Miles", "mi"},
        {"Yards", "yd"},
        {"Feet", "ft"},
        {"Inches", "in"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&lengthUnitEnumString](const auto& s) {
        return istringEqual(s.first, lengthUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized length unit specified: " << lengthUnitEnumString << ", using default " << m_lengthUnit);
      return m_lengthUnit;
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::areaUnitFromEnum(std::string_view areaUnitEnumString) const {

    // {'SquareKilometers'|'SquareMeters'|'SquareCentimeters'|'SquareMillimeters'|'SquareMiles'|'SquareYards'|'SquareFeet'|'SquareInches'}

    if (areaUnitEnumString.empty()) {
      LOG(Warn, "No area unit specified, using default " << m_areaUnit);
      return m_temperatureUnit;
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 8> enumToUnitStrings = {{
      {"SquareMeters", "m^2"},
        {"SquareKilometers", "km^2"},
        {"SquareCentimeters", "cm^2"},
        {"SquareMillimeters", "mm^2"},
        {"SquareMiles", "mi^2"},
        {"SquareYards", "yd^2"},
        {"SquareFeet", "ft^2"},
        {"SquareInches", "in^2"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&areaUnitEnumString](const auto& s) {
        return istringEqual(s.first == areaUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized area unit specified: " << areaUnitEnumString << ", using default " << m_areaUnit);
      return m_areaUnit;
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::volumeUnitFromEnum(std::string_view volumeUnitEnumString) const {

    // {'CubicKilometers'|'CubicMeters'|'CubicCentimeters'|'CubicMillimeters'|'CubicMiles'|'CubicYards'|'CubicFeet'|'CubicInches'}

    if (volumeUnitEnumString.empty()) {
      LOG(Warn, "No volume unit specified, using default " << m_volumeUnit);
      return m_temperatureUnit;
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 8> enumToUnitStrings = {{
      {"CubicMeters", "m^3"},
        {"CubicKilometers", "km^3"},
        {"CubicCentimeters", "cm^3"},
        {"CubicMillimeters", "mm^3"},
        {"CubicMiles", "mi^3"},
        {"CubicYards", "yd^3"},
        {"CubicFeet", "ft^3"},
        {"CubicInches", "in^3"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&volumeUnitEnumString](const auto& s) {
        return istringEqual(s.first == volumeUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized volume unit specified: " << volumeUnitEnumString << ", using default " << m_volumeUnit);
      return m_volumeUnit;
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::temperatureUnitFromEnum(std::string_view temperatureUnitEnumString) const {

    //  {'F'|'C'|'K'|'R'}

    if (temperatureUnitEnumString.empty()) {
      LOG(Warn, "No temperature unit specified, using default " << m_temperatureUnit);
      return m_temperatureUnit;
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 4> enumToUnitStrings = {{
      {"F", "F"},
      {"C", "C"},
      {"K", "K"},
      {"R", "R"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&temperatureUnitEnumString](const auto& s) {
        return istringEqual(s.first == temperatureUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized temperature unit specified: " << temperatureUnitEnumString << ", using default " << m_temperatureUnit);
      return m_temperatureUnit;
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }



  Unit ReverseTranslator::uValueUnitFromEnum(std::string_view uValueUnitEnumString) const {

    std::string defaultUnit = "W/m^2*K";

    if (uValueUnitEnumString.empty()) {
      LOG(Warn, "No U-Value unit specified, using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 2> enumToUnitStrings = {{
      {"WPerSquareMeterK", "W/m^2*K"},
      {"BtuPerHourSquareFtF", "Btu/hr*ft^2*R"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&uValueUnitEnumString](const auto& s) {
        return istringEqual(s.first == uValueUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized U-Value unit specified: " << uValueUnitEnumString << ", using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }


  Unit ReverseTranslator::rValueUnitFromEnum(std::string_view rValueUnitEnumString) const {

    std::string defaultUnit = "m^2*K/W";

    if (rValueUnitEnumString.empty()) {
      LOG(Warn, "No R-Value unit specified, using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 2> enumToUnitStrings = {{
      {"SquareMeterKPerW", "m^2*K/W"},
      {"HrSquareFtFPerBTU", "hr*ft^2*R/W"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&rValueUnitEnumString](const auto& s) {
        return istringEqual(s.first == rValueUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized R-Value unit specified: " << rValueUnitEnumString << ", using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::specificHeatUnitFromEnum(std::string_view specificHeatUnitEnumString) const {

    std::string defaultUnit = "J/kg";

    if (specificHeatUnitEnumString.empty()) {
      LOG(Warn, "No specificHeat unit specified, using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 2> enumToUnitStrings = {{
      {"JPerKgK", "J/kg"},
        {"BTUPerLbF", "Btu/lb"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&specificHeatUnitEnumString](const auto& s) {
        return istringEqual(s.first == specificHeatUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized specificHeat unit specified: " << specificHeatUnitEnumString << ", using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::densityUnitFromEnum(std::string_view densityUnitEnumString) const {

    std::string defaultUnit = "kg/m^3";

    if (densityUnitEnumString.empty()) {
      LOG(Warn, "No density unit specified, using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 4> enumToUnitStrings = {{
      {"GramsPerCubicCm", "g/cm^3"},
      {"LbsPerCubicIn", "lb/ft^3"},
      {"LbsPerCubicFt", "lb/ft^3"},
      {"KgPerCubicM", "kg/m^3"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&densityUnitEnumString](const auto& s) {
        return istringEqual(s.first == densityUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized density unit specified: " << densityUnitEnumString << ", using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::conductivityUnitFromEnum(std::string_view conductivityUnitEnumString) const {

    std::string defaultUnit = "W/m*K";

    if (conductivityUnitEnumString.empty()) {
      LOG(Warn, "No conductivity unit specified, using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 4> enumToUnitStrings = {{
      // {"WPerCmC", "g/cm^3"},
      {"WPerMeterK", "W/m*K"},
      {"BtuPerHourFtF", "Btu/hr*ft*R"},
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&conductivityUnitEnumString](const auto& s) {
        return istringEqual(s.first == conductivityUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized conductivity unit specified: " << conductivityUnitEnumString << ", using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }

  Unit ReverseTranslator::windSpeedUnitFromEnum(std::string_view windSpeedUnitEnumString) const {

    std::string defaultUnit = "m/s";

    if (windSpeedUnitEnumString.empty()) {
      LOG(Warn, "No windSpeed unit specified, using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    }

    static constexpr std::array<std::pair<std::string_view, std::string_view>, 2> enumToUnitStrings = {{
      {"MetersPerSecond", "m/s"},
      {"MilesPerHour", "mi/h"},
      // {"Knots", "lb/ft^3"}, // TODO: not handled, not in the UnitFactory...
    }};

    auto it = std::find_if(enumToUnitStrings.begin(), enumToUnitStrings.end(), [&windSpeedUnitEnumString](const auto& s) {
        return istringEqual(s.first == windSpeedUnitEnumString);
        });
    if (it == enumToUnitStrings.end()) {
      LOG(Error, "Unrecognized windSpeed unit specified: " << windSpeedUnitEnumString << ", using default of " << defaultUnit);
      return UnitFactory::instance().createUnit(defaultUnit).get();
    } else {
      return UnitFactory::instance().createUnit(it->second.data()).get();
    }
  }


} // gbxml
} // openstudio
