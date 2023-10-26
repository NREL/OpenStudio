/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GeneratorWindTurbine.hpp"
#include "GeneratorWindTurbine_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_WindTurbine_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    GeneratorWindTurbine_Impl::GeneratorWindTurbine_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Generator_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == GeneratorWindTurbine::iddObjectType());
    }

    GeneratorWindTurbine_Impl::GeneratorWindTurbine_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Generator_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == GeneratorWindTurbine::iddObjectType());
    }

    GeneratorWindTurbine_Impl::GeneratorWindTurbine_Impl(const GeneratorWindTurbine_Impl& other, Model_Impl* model, bool keepHandle)
      : Generator_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& GeneratorWindTurbine_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Generator Produced AC Electricity Rate",       "Generator Produced AC Electricity Energy",
                                                   "Generator Turbine Local Wind Speed",           "Generator Turbine Local Air Density",
                                                   "Generator Turbine Power Coefficient",          "Generator Turbine Tip Speed Ratio",
                                                   "Generator Turbine Chordal Component Velocity", "Generator Turbine Normal Component Velocity",
                                                   "Generator Turbine Relative Flow Velocity",     "Generator Turbine Attack Angle"};
      return result;
    }

    IddObjectType GeneratorWindTurbine_Impl::iddObjectType() const {
      return GeneratorWindTurbine::iddObjectType();
    }

    std::string GeneratorWindTurbine_Impl::generatorObjectType() const {
      return "Generator:WindTurbine";
    }

    std::vector<ScheduleTypeKey> GeneratorWindTurbine_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Generator_WindTurbineFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("GeneratorWindTurbine", "Availability"));
      }
      return result;
    }

    // translated to ElectricLoadCenter:Generators 'Generator Rated Electric Power Output'
    boost::optional<double> GeneratorWindTurbine_Impl::ratedElectricPowerOutput() const {
      return getDouble(OS_Generator_WindTurbineFields::RatedPower, true);
    }

    boost::optional<Schedule> GeneratorWindTurbine_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Generator_WindTurbineFields::AvailabilityScheduleName);
    }

    // Convenience method to go fetch the connected GeneratorWindTurbine's 'Rated Thermal to Electrical Power Ratio'
    boost::optional<double> GeneratorWindTurbine_Impl::ratedThermaltoElectricalPowerRatio() const {
      return boost::none;
    }

    std::string GeneratorWindTurbine_Impl::rotorType() const {
      boost::optional<std::string> value = getString(OS_Generator_WindTurbineFields::RotorType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string GeneratorWindTurbine_Impl::powerControl() const {
      boost::optional<std::string> value = getString(OS_Generator_WindTurbineFields::PowerControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::ratedRotorSpeed() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::RatedRotorSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::rotorDiameter() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::RotorDiameter, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::overallHeight() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::OverallHeight, true);
      OS_ASSERT(value);
      return value.get();
    }

    int GeneratorWindTurbine_Impl::numberofBlades() const {
      boost::optional<int> value = getInt(OS_Generator_WindTurbineFields::NumberofBlades, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::ratedPower() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::RatedPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::ratedWindSpeed() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::RatedWindSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::cutInWindSpeed() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::CutInWindSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::cutOutWindSpeed() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::CutOutWindSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::fractionSystemEfficiency() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::FractionsystemEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::maximumTipSpeedRatio() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::MaximumTipSpeedRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::maximumPowerCoefficient() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::MaximumPowerCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> GeneratorWindTurbine_Impl::annualLocalAverageWindSpeed() const {
      return getDouble(OS_Generator_WindTurbineFields::AnnualLocalAverageWindSpeed, true);
    }

    double GeneratorWindTurbine_Impl::heightforLocalAverageWindSpeed() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::HeightforLocalAverageWindSpeed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::bladeChordArea() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::BladeChordArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::bladeDragCoefficient() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::BladeDragCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::bladeLiftCoefficient() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::BladeLiftCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::powerCoefficientC1() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::PowerCoefficientC1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::powerCoefficientC2() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::PowerCoefficientC2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::powerCoefficientC3() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::PowerCoefficientC3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::powerCoefficientC4() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::PowerCoefficientC4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::powerCoefficientC5() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::PowerCoefficientC5, true);
      OS_ASSERT(value);
      return value.get();
    }

    double GeneratorWindTurbine_Impl::powerCoefficientC6() const {
      boost::optional<double> value = getDouble(OS_Generator_WindTurbineFields::PowerCoefficientC6, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool GeneratorWindTurbine_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Generator_WindTurbineFields::AvailabilityScheduleName, "GeneratorWindTurbine", "Availability", schedule);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setRotorType(const std::string& rotorType) {
      bool result = setString(OS_Generator_WindTurbineFields::RotorType, rotorType);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerControl(const std::string& powerControl) {
      bool result = setString(OS_Generator_WindTurbineFields::PowerControl, powerControl);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setRatedRotorSpeed(double ratedRotorSpeed) {
      bool result = setDouble(OS_Generator_WindTurbineFields::RatedRotorSpeed, ratedRotorSpeed);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setRotorDiameter(double rotorDiameter) {
      bool result = setDouble(OS_Generator_WindTurbineFields::RotorDiameter, rotorDiameter);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setOverallHeight(double overallHeight) {
      bool result = setDouble(OS_Generator_WindTurbineFields::OverallHeight, overallHeight);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setNumberofBlades(int numberofBlades) {
      bool result = setInt(OS_Generator_WindTurbineFields::NumberofBlades, numberofBlades);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setRatedPower(double ratedPower) {
      bool result = setDouble(OS_Generator_WindTurbineFields::RatedPower, ratedPower);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setRatedWindSpeed(double ratedWindSpeed) {
      bool result = setDouble(OS_Generator_WindTurbineFields::RatedWindSpeed, ratedWindSpeed);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setCutInWindSpeed(double cutInWindSpeed) {
      bool result = setDouble(OS_Generator_WindTurbineFields::CutInWindSpeed, cutInWindSpeed);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setCutOutWindSpeed(double cutOutWindSpeed) {
      bool result = setDouble(OS_Generator_WindTurbineFields::CutOutWindSpeed, cutOutWindSpeed);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setFractionSystemEfficiency(double fractionSystemEfficiency) {
      bool result = setDouble(OS_Generator_WindTurbineFields::FractionsystemEfficiency, fractionSystemEfficiency);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setMaximumTipSpeedRatio(double maximumTipSpeedRatio) {
      bool result = setDouble(OS_Generator_WindTurbineFields::MaximumTipSpeedRatio, maximumTipSpeedRatio);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setMaximumPowerCoefficient(double maximumPowerCoefficient) {
      bool result = setDouble(OS_Generator_WindTurbineFields::MaximumPowerCoefficient, maximumPowerCoefficient);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setAnnualLocalAverageWindSpeed(double annualLocalAverageWindSpeed) {
      bool result = setDouble(OS_Generator_WindTurbineFields::AnnualLocalAverageWindSpeed, annualLocalAverageWindSpeed);
      return result;
    }

    void GeneratorWindTurbine_Impl::resetAnnualLocalAverageWindSpeed() {
      bool result = setString(OS_Generator_WindTurbineFields::AnnualLocalAverageWindSpeed, "");
      OS_ASSERT(result);
    }

    bool GeneratorWindTurbine_Impl::setHeightforLocalAverageWindSpeed(double heightforLocalAverageWindSpeed) {
      bool result = setDouble(OS_Generator_WindTurbineFields::HeightforLocalAverageWindSpeed, heightforLocalAverageWindSpeed);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setBladeChordArea(double bladeChordArea) {
      bool result = setDouble(OS_Generator_WindTurbineFields::BladeChordArea, bladeChordArea);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setBladeDragCoefficient(double bladeDragCoefficient) {
      bool result = setDouble(OS_Generator_WindTurbineFields::BladeDragCoefficient, bladeDragCoefficient);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setBladeLiftCoefficient(double bladeLiftCoefficient) {
      bool result = setDouble(OS_Generator_WindTurbineFields::BladeLiftCoefficient, bladeLiftCoefficient);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerCoefficientC1(double powerCoefficientC1) {
      bool result = setDouble(OS_Generator_WindTurbineFields::PowerCoefficientC1, powerCoefficientC1);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerCoefficientC2(double powerCoefficientC2) {
      bool result = setDouble(OS_Generator_WindTurbineFields::PowerCoefficientC2, powerCoefficientC2);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerCoefficientC3(double powerCoefficientC3) {
      bool result = setDouble(OS_Generator_WindTurbineFields::PowerCoefficientC3, powerCoefficientC3);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerCoefficientC4(double powerCoefficientC4) {
      bool result = setDouble(OS_Generator_WindTurbineFields::PowerCoefficientC4, powerCoefficientC4);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerCoefficientC5(double powerCoefficientC5) {
      bool result = setDouble(OS_Generator_WindTurbineFields::PowerCoefficientC5, powerCoefficientC5);
      return result;
    }

    bool GeneratorWindTurbine_Impl::setPowerCoefficientC6(double powerCoefficientC6) {
      bool result = setDouble(OS_Generator_WindTurbineFields::PowerCoefficientC6, powerCoefficientC6);
      return result;
    }

  }  // namespace detail

  GeneratorWindTurbine::GeneratorWindTurbine(const Model& model) : Generator(GeneratorWindTurbine::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GeneratorWindTurbine_Impl>());

    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setAvailabilitySchedule(schedule);
    }
    setRotorType("HorizontalAxisWindTurbine");
    setPowerControl("VariableSpeedVariablePitch");
    setRatedRotorSpeed(41.0);  // from docs
    setRotorDiameter(19.2);    // from docs
    setOverallHeight(30.5);    // from docs
    setNumberofBlades(3);
    setRatedPower(55000.0);    // from docs
    setRatedWindSpeed(11.0);   // from docs
    setCutInWindSpeed(3.5);    // from docs
    setCutOutWindSpeed(25.0);  // from docs
    setFractionSystemEfficiency(0.835);
    setMaximumTipSpeedRatio(5.0);
    setMaximumPowerCoefficient(0.25);
    setHeightforLocalAverageWindSpeed(50.0);
    setBladeChordArea(2.08);  // from docs
    setBladeDragCoefficient(0.9);
    setBladeLiftCoefficient(0.05);
    setPowerCoefficientC1(0.5176);
    setPowerCoefficientC2(116.0);
    setPowerCoefficientC3(0.4);
    setPowerCoefficientC4(0.0);
    setPowerCoefficientC5(5.0);
    setPowerCoefficientC6(21.0);
  }

  IddObjectType GeneratorWindTurbine::iddObjectType() {
    return {IddObjectType::OS_Generator_WindTurbine};
  }

  std::string GeneratorWindTurbine::rotorType() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->rotorType();
  }

  std::string GeneratorWindTurbine::powerControl() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerControl();
  }

  double GeneratorWindTurbine::ratedRotorSpeed() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->ratedRotorSpeed();
  }

  double GeneratorWindTurbine::rotorDiameter() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->rotorDiameter();
  }

  double GeneratorWindTurbine::overallHeight() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->overallHeight();
  }

  int GeneratorWindTurbine::numberofBlades() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->numberofBlades();
  }

  double GeneratorWindTurbine::ratedPower() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->ratedPower();
  }

  double GeneratorWindTurbine::ratedWindSpeed() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->ratedWindSpeed();
  }

  double GeneratorWindTurbine::cutInWindSpeed() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->cutInWindSpeed();
  }

  double GeneratorWindTurbine::cutOutWindSpeed() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->cutOutWindSpeed();
  }

  double GeneratorWindTurbine::fractionSystemEfficiency() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->fractionSystemEfficiency();
  }

  double GeneratorWindTurbine::maximumTipSpeedRatio() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->maximumTipSpeedRatio();
  }

  double GeneratorWindTurbine::maximumPowerCoefficient() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->maximumPowerCoefficient();
  }

  boost::optional<double> GeneratorWindTurbine::annualLocalAverageWindSpeed() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->annualLocalAverageWindSpeed();
  }

  double GeneratorWindTurbine::heightforLocalAverageWindSpeed() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->heightforLocalAverageWindSpeed();
  }

  double GeneratorWindTurbine::bladeChordArea() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->bladeChordArea();
  }

  double GeneratorWindTurbine::bladeDragCoefficient() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->bladeDragCoefficient();
  }

  double GeneratorWindTurbine::bladeLiftCoefficient() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->bladeLiftCoefficient();
  }

  double GeneratorWindTurbine::powerCoefficientC1() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerCoefficientC1();
  }

  double GeneratorWindTurbine::powerCoefficientC2() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerCoefficientC2();
  }

  double GeneratorWindTurbine::powerCoefficientC3() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerCoefficientC3();
  }

  double GeneratorWindTurbine::powerCoefficientC4() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerCoefficientC4();
  }

  double GeneratorWindTurbine::powerCoefficientC5() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerCoefficientC5();
  }

  double GeneratorWindTurbine::powerCoefficientC6() const {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->powerCoefficientC6();
  }

  bool GeneratorWindTurbine::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool GeneratorWindTurbine::setRotorType(const std::string& rotorType) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setRotorType(rotorType);
  }

  bool GeneratorWindTurbine::setPowerControl(const std::string& powerControl) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerControl(powerControl);
  }

  bool GeneratorWindTurbine::setRatedRotorSpeed(double ratedRotorSpeed) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setRatedRotorSpeed(ratedRotorSpeed);
  }

  bool GeneratorWindTurbine::setRotorDiameter(double rotorDiameter) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setRotorDiameter(rotorDiameter);
  }

  bool GeneratorWindTurbine::setOverallHeight(double overallHeight) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setOverallHeight(overallHeight);
  }

  bool GeneratorWindTurbine::setNumberofBlades(int numberofBlades) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setNumberofBlades(numberofBlades);
  }

  bool GeneratorWindTurbine::setRatedPower(double ratedPower) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setRatedPower(ratedPower);
  }

  bool GeneratorWindTurbine::setRatedWindSpeed(double ratedWindSpeed) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setRatedWindSpeed(ratedWindSpeed);
  }

  bool GeneratorWindTurbine::setCutInWindSpeed(double cutInWindSpeed) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setCutInWindSpeed(cutInWindSpeed);
  }

  bool GeneratorWindTurbine::setCutOutWindSpeed(double cutOutWindSpeed) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setCutOutWindSpeed(cutOutWindSpeed);
  }

  bool GeneratorWindTurbine::setFractionSystemEfficiency(double fractionSystemEfficiency) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setFractionSystemEfficiency(fractionSystemEfficiency);
  }

  bool GeneratorWindTurbine::setMaximumTipSpeedRatio(double maximumTipSpeedRatio) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setMaximumTipSpeedRatio(maximumTipSpeedRatio);
  }

  bool GeneratorWindTurbine::setMaximumPowerCoefficient(double maximumPowerCoefficient) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setMaximumPowerCoefficient(maximumPowerCoefficient);
  }

  bool GeneratorWindTurbine::setAnnualLocalAverageWindSpeed(double annualLocalAverageWindSpeed) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setAnnualLocalAverageWindSpeed(annualLocalAverageWindSpeed);
  }

  void GeneratorWindTurbine::resetAnnualLocalAverageWindSpeed() {
    getImpl<detail::GeneratorWindTurbine_Impl>()->resetAnnualLocalAverageWindSpeed();
  }

  bool GeneratorWindTurbine::setHeightforLocalAverageWindSpeed(double heightforLocalAverageWindSpeed) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setHeightforLocalAverageWindSpeed(heightforLocalAverageWindSpeed);
  }

  bool GeneratorWindTurbine::setBladeChordArea(double bladeChordArea) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setBladeChordArea(bladeChordArea);
  }

  bool GeneratorWindTurbine::setBladeDragCoefficient(double bladeDragCoefficient) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setBladeDragCoefficient(bladeDragCoefficient);
  }

  bool GeneratorWindTurbine::setBladeLiftCoefficient(double bladeLiftCoefficient) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setBladeLiftCoefficient(bladeLiftCoefficient);
  }

  bool GeneratorWindTurbine::setPowerCoefficientC1(double powerCoefficientC1) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerCoefficientC1(powerCoefficientC1);
  }

  bool GeneratorWindTurbine::setPowerCoefficientC2(double powerCoefficientC2) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerCoefficientC2(powerCoefficientC2);
  }

  bool GeneratorWindTurbine::setPowerCoefficientC3(double powerCoefficientC3) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerCoefficientC3(powerCoefficientC3);
  }

  bool GeneratorWindTurbine::setPowerCoefficientC4(double powerCoefficientC4) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerCoefficientC4(powerCoefficientC4);
  }

  bool GeneratorWindTurbine::setPowerCoefficientC5(double powerCoefficientC5) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerCoefficientC5(powerCoefficientC5);
  }

  bool GeneratorWindTurbine::setPowerCoefficientC6(double powerCoefficientC6) {
    return getImpl<detail::GeneratorWindTurbine_Impl>()->setPowerCoefficientC6(powerCoefficientC6);
  }

  /// @cond
  GeneratorWindTurbine::GeneratorWindTurbine(std::shared_ptr<detail::GeneratorWindTurbine_Impl> impl) : Generator(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
