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

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/CFMUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/units/CelsiusUnit.hpp"
#include "../utilities/units/FahrenheitUnit.hpp"
#include "../utilities/units/TemperatureUnit_Impl.hpp"
#include "../utilities/units/UnitFactory.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleTypeLimits_Impl::ScheduleTypeLimits_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleTypeLimits::iddObjectType());
    }

    ScheduleTypeLimits_Impl::ScheduleTypeLimits_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleTypeLimits::iddObjectType());
    }

    ScheduleTypeLimits_Impl::ScheduleTypeLimits_Impl(const ScheduleTypeLimits_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ScheduleTypeLimits_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ScheduleTypeLimits_Impl::iddObjectType() const {
      return ScheduleTypeLimits::iddObjectType();
    }

    boost::optional<double> ScheduleTypeLimits_Impl::lowerLimitValue() const {
      return getDouble(OS_ScheduleTypeLimitsFields::LowerLimitValue, true);
    }

    boost::optional<double> ScheduleTypeLimits_Impl::upperLimitValue() const {
      return getDouble(OS_ScheduleTypeLimitsFields::UpperLimitValue, true);
    }

    boost::optional<std::string> ScheduleTypeLimits_Impl::numericType() const {
      return getString(OS_ScheduleTypeLimitsFields::NumericType, true, true);
    }

    std::string ScheduleTypeLimits_Impl::unitType() const {
      boost::optional<std::string> value = getString(OS_ScheduleTypeLimitsFields::UnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ScheduleTypeLimits_Impl::isUnitTypeDefaulted() const {
      return isEmpty(OS_ScheduleTypeLimitsFields::UnitType);
    }

    boost::optional<Unit> ScheduleTypeLimits_Impl::units(bool returnIP) const {
      return ScheduleTypeLimits::units(unitType(), returnIP);
    }

    bool ScheduleTypeLimits_Impl::setLowerLimitValue(double lowerLimitValue) {
      bool result = false;
      result = setDouble(OS_ScheduleTypeLimitsFields::LowerLimitValue, lowerLimitValue);
      return result;
    }

    void ScheduleTypeLimits_Impl::resetLowerLimitValue() {
      bool result = setString(OS_ScheduleTypeLimitsFields::LowerLimitValue, "");
      OS_ASSERT(result);
    }

    bool ScheduleTypeLimits_Impl::setUpperLimitValue(double upperLimitValue) {
      bool result = false;
      result = setDouble(OS_ScheduleTypeLimitsFields::UpperLimitValue, upperLimitValue);
      return result;
    }

    void ScheduleTypeLimits_Impl::resetUpperLimitValue() {
      bool result = setString(OS_ScheduleTypeLimitsFields::UpperLimitValue, "");
      OS_ASSERT(result);
    }

    bool ScheduleTypeLimits_Impl::setNumericType(const std::string& numericType) {
      bool result = false;
      result = setString(OS_ScheduleTypeLimitsFields::NumericType, numericType);
      return result;
    }

    void ScheduleTypeLimits_Impl::resetNumericType() {
      bool result = setString(OS_ScheduleTypeLimitsFields::NumericType, "");
      OS_ASSERT(result);
    }

    bool ScheduleTypeLimits_Impl::setUnitType(const std::string& unitType) {
      bool result = setString(OS_ScheduleTypeLimitsFields::UnitType, unitType);
      return result;
    }

    void ScheduleTypeLimits_Impl::resetUnitType() {
      bool result = setString(OS_ScheduleTypeLimitsFields::UnitType, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ScheduleTypeLimits::ScheduleTypeLimits(const Model& model) : ResourceObject(ScheduleTypeLimits::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ScheduleTypeLimits_Impl>());
  }

  IddObjectType ScheduleTypeLimits::iddObjectType() {
    IddObjectType result(IddObjectType::OS_ScheduleTypeLimits);
    return result;
  }

  std::vector<std::string> ScheduleTypeLimits::numericTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ScheduleTypeLimitsFields::NumericType);
  }

  std::vector<std::string> ScheduleTypeLimits::unitTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ScheduleTypeLimitsFields::UnitType);
  }

  boost::optional<Unit> ScheduleTypeLimits::units(std::string unitType, bool returnIP) {
    boost::to_lower(unitType);
    OptionalUnit result;

    if (unitType.empty() || (unitType == "dimensionless") || (unitType == "availability") || (unitType == "controlmode")) {
      if (returnIP) {
        result = IPUnit();
      } else {
        result = SIUnit();
      }
      return result;
    }

    char firstLetter = unitType[0];
    switch (firstLetter) {
      case 'a': {
        if (unitType == "activitylevel") {
          result = (createSIPower() / createSIPeople());
        } else if (unitType == "angle") {
          result = createIPAngle();
        }
        break;
      }
      case 'c': {
        if (unitType == "capacity") {
          if (returnIP) {
            result = BTUUnit(BTUExpnt(1, 0, -1));
          } else {
            result = createSIPower();
          }
        } else if (unitType == "clothinginsulation") {
          result = Unit();
          result->setBaseUnitExponent("clo", 1);
        } else if (unitType == "convectioncoefficient") {
          if (returnIP) {
            result = BTUUnit(BTUExpnt(1, -2, -1, -1));
          } else {
            result = createSIThermalConductance();
          }
        }
        break;
      }
      case 'd': {
        if (unitType == "deltatemperature") {
          if (returnIP) {
            result = createFahrenheitTemperature();
            result->cast<TemperatureUnit>().setAsRelative();
          } else {
            result = createCelsiusTemperature();
            result->cast<TemperatureUnit>().setAsRelative();
          }
        }
        break;
      }
      case 'l': {
        if (unitType == "linearpowerdensity") {
          if (returnIP) {
            result = (createIPPower() / createIPLength());
          } else {
            result = (createSIPower() / createSILength());
          }
        }
        break;
      }
      case 'm': {
        if (unitType == "massflowrate") {
          if (returnIP) {
            result = IPUnit(IPExpnt(1, 0, -1));
          } else {
            result = SIUnit(SIExpnt(1, 0, -1));
          }
        }
        break;
      }
      case 'p': {
        if (unitType == "percent") {
          result = Unit();
          result->setBaseUnitExponent("%", 1);
        } else if (unitType == "power") {
          result = createSIPower();
        } else if (unitType == "precipitationrate") {
          if (returnIP) {
            result = BTUUnit(BTUExpnt(0, 1, -1));
          } else {
            result = WhUnit(WhExpnt(0, -1, 1));
          }
        } else if (unitType == "pressure") {
          if (returnIP) {
            result = createIPPressure();
          } else {
            result = createSIPressure();
          }
        }
        break;
      }
      case 'r': {
        if (unitType == "rotationsperminute") {
          result = createCFMFrequency();
        }
        break;
      }
      case 's': {
        if (unitType == "solarenergy") {
          result = WhUnit(WhExpnt(1, 1, -2));
        }
        break;
      }
      case 't': {
        if (unitType == "temperature") {
          if (returnIP) {
            result = createFahrenheitTemperature();
          } else {
            result = createCelsiusTemperature();
          }
        }
        break;
      }
      case 'v': {
        if (unitType == "velocity") {
          if (returnIP) {
            result = CFMUnit(CFMExpnt(1, -1));
          } else {
            result = SIUnit(SIExpnt(0, 1, -1));
          }
        }
        if (unitType == "volumetricflowrate") {
          if (returnIP) {
            result = IPUnit(IPExpnt(0, 3, -1));
          } else {
            result = SIUnit(SIExpnt(0, 3, -1));
          }
        }
        break;
      }
    }

    return result;
  }

  boost::optional<double> ScheduleTypeLimits::lowerLimitValue() const {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->lowerLimitValue();
  }

  boost::optional<double> ScheduleTypeLimits::upperLimitValue() const {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->upperLimitValue();
  }

  boost::optional<std::string> ScheduleTypeLimits::numericType() const {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->numericType();
  }

  std::string ScheduleTypeLimits::unitType() const {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->unitType();
  }

  bool ScheduleTypeLimits::isUnitTypeDefaulted() const {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->isUnitTypeDefaulted();
  }

  boost::optional<Unit> ScheduleTypeLimits::units(bool returnIP) const {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->units(returnIP);
  }

  bool ScheduleTypeLimits::setLowerLimitValue(double lowerLimitValue) {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->setLowerLimitValue(lowerLimitValue);
  }

  void ScheduleTypeLimits::resetLowerLimitValue() {
    getImpl<detail::ScheduleTypeLimits_Impl>()->resetLowerLimitValue();
  }

  bool ScheduleTypeLimits::setUpperLimitValue(double upperLimitValue) {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->setUpperLimitValue(upperLimitValue);
  }

  void ScheduleTypeLimits::resetUpperLimitValue() {
    getImpl<detail::ScheduleTypeLimits_Impl>()->resetUpperLimitValue();
  }

  bool ScheduleTypeLimits::setNumericType(const std::string& numericType) {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->setNumericType(numericType);
  }

  void ScheduleTypeLimits::resetNumericType() {
    getImpl<detail::ScheduleTypeLimits_Impl>()->resetNumericType();
  }

  bool ScheduleTypeLimits::setUnitType(const std::string& unitType) {
    return getImpl<detail::ScheduleTypeLimits_Impl>()->setUnitType(unitType);
  }

  void ScheduleTypeLimits::resetUnitType() {
    getImpl<detail::ScheduleTypeLimits_Impl>()->resetUnitType();
  }

  /// @cond
  ScheduleTypeLimits::ScheduleTypeLimits(std::shared_ptr<detail::ScheduleTypeLimits_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

  bool isCompatible(const ScheduleTypeLimits& parentLimits, const ScheduleTypeLimits& candidate) {
    if (candidate == parentLimits) {
      return true;
    }

    // check continuous/discrete
    if (OptionalString numericType = parentLimits.numericType()) {
      if (!candidate.numericType() || !istringEqual(candidate.numericType().get(), *numericType)) {
        return false;
      }
    }

    // check unit type
    std::string unitType = parentLimits.unitType();
    if (parentLimits.isUnitTypeDefaulted() || istringEqual("Dimensionless", unitType)) {
      if (!(candidate.isUnitTypeDefaulted() || (istringEqual("Dimensionless", candidate.unitType())))) {
        return false;
      }
    } else {
      if (!istringEqual(candidate.unitType(), unitType)) {
        return false;
      }
    }

    // check lower limit
    if (parentLimits.lowerLimitValue()) {
      if (!candidate.lowerLimitValue() || (candidate.lowerLimitValue().get() < parentLimits.lowerLimitValue().get())) {
        return false;
      }
    }

    // check upper limit
    if (parentLimits.upperLimitValue()) {
      if (!candidate.upperLimitValue() || (candidate.upperLimitValue().get() > parentLimits.upperLimitValue().get())) {
        return false;
      }
    }

    // passed all checks
    return true;
  }

}  // namespace model
}  // namespace openstudio
