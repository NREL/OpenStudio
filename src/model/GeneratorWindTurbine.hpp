/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORWINDTURBINE_HPP
#define MODEL_GENERATORWINDTURBINE_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class GeneratorWindTurbine_Impl;

  }  // namespace detail

  /** GeneratorWindTurbine is a Generator that wraps the OpenStudio IDD object 'OS:Generator:WindTurbine'. */
  class MODEL_API GeneratorWindTurbine : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorWindTurbine(const Model& model);

    virtual ~GeneratorWindTurbine() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorWindTurbine(const GeneratorWindTurbine& other) = default;
    GeneratorWindTurbine(GeneratorWindTurbine&& other) = default;
    GeneratorWindTurbine& operator=(const GeneratorWindTurbine&) = default;
    GeneratorWindTurbine& operator=(GeneratorWindTurbine&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string rotorType() const;

    std::string powerControl() const;

    double ratedRotorSpeed() const;

    double rotorDiameter() const;

    double overallHeight() const;

    int numberofBlades() const;

    double ratedPower() const;

    double ratedWindSpeed() const;

    double cutInWindSpeed() const;

    double cutOutWindSpeed() const;

    double fractionSystemEfficiency() const;

    double maximumTipSpeedRatio() const;

    double maximumPowerCoefficient() const;

    boost::optional<double> annualLocalAverageWindSpeed() const;

    double heightforLocalAverageWindSpeed() const;

    double bladeChordArea() const;

    double bladeDragCoefficient() const;

    double bladeLiftCoefficient() const;

    double powerCoefficientC1() const;

    double powerCoefficientC2() const;

    double powerCoefficientC3() const;

    double powerCoefficientC4() const;

    double powerCoefficientC5() const;

    double powerCoefficientC6() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRotorType(const std::string& rotorType);

    bool setPowerControl(const std::string& powerControl);

    bool setRatedRotorSpeed(double ratedRotorSpeed);

    bool setRotorDiameter(double rotorDiameter);

    bool setOverallHeight(double overallHeight);

    bool setNumberofBlades(int numberofBlades);

    bool setRatedPower(double ratedPower);

    bool setRatedWindSpeed(double ratedWindSpeed);

    bool setCutInWindSpeed(double cutInWindSpeed);

    bool setCutOutWindSpeed(double cutOutWindSpeed);

    bool setFractionSystemEfficiency(double fractionSystemEfficiency);

    bool setMaximumTipSpeedRatio(double maximumTipSpeedRatio);

    bool setMaximumPowerCoefficient(double maximumPowerCoefficient);

    bool setAnnualLocalAverageWindSpeed(double annualLocalAverageWindSpeed);

    void resetAnnualLocalAverageWindSpeed();

    bool setHeightforLocalAverageWindSpeed(double heightforLocalAverageWindSpeed);

    bool setBladeChordArea(double bladeChordArea);

    bool setBladeDragCoefficient(double bladeDragCoefficient);

    bool setBladeLiftCoefficient(double bladeLiftCoefficient);

    bool setPowerCoefficientC1(double powerCoefficientC1);

    bool setPowerCoefficientC2(double powerCoefficientC2);

    bool setPowerCoefficientC3(double powerCoefficientC3);

    bool setPowerCoefficientC4(double powerCoefficientC4);

    bool setPowerCoefficientC5(double powerCoefficientC5);

    bool setPowerCoefficientC6(double powerCoefficientC6);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorWindTurbine_Impl;

    explicit GeneratorWindTurbine(std::shared_ptr<detail::GeneratorWindTurbine_Impl> impl);

    friend class detail::GeneratorWindTurbine_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorWindTurbine");
  };

  /** \relates GeneratorWindTurbine*/
  using OptionalGeneratorWindTurbine = boost::optional<GeneratorWindTurbine>;

  /** \relates GeneratorWindTurbine*/
  using GeneratorWindTurbineVector = std::vector<GeneratorWindTurbine>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORWINDTURBINE_HPP
