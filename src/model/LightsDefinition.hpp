/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTSDEFINITION_HPP
#define MODEL_LIGHTSDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class LightsDefinition_Impl;

  }  // namespace detail

  /** LightsDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object
 *  'OS_Lights_Definition'. Its fields are derived from the EnergyPlus IDD object
 *  'Lights'. \sa Lights
 */
  class MODEL_API LightsDefinition : public SpaceLoadDefinition
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    explicit LightsDefinition(const Model& model);

    virtual ~LightsDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LightsDefinition(const LightsDefinition& other) = default;
    LightsDefinition(LightsDefinition&& other) = default;
    LightsDefinition& operator=(const LightsDefinition&) = default;
    LightsDefinition& operator=(LightsDefinition&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDesignLevelCalculationMethodValues();

    /** @name Getters */
    //@{

    std::string designLevelCalculationMethod() const;
    boost::optional<double> lightingLevel() const;
    boost::optional<double> wattsperSpaceFloorArea() const;
    boost::optional<double> wattsperPerson() const;

    double fractionRadiant() const;
    bool isFractionRadiantDefaulted() const;

    double fractionVisible() const;
    bool isFractionVisibleDefaulted() const;

    double returnAirFraction() const;
    bool isReturnAirFractionDefaulted() const;

    bool returnAirFractionCalculatedfromPlenumTemperature() const;
    bool isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const;

    double returnAirFractionFunctionofPlenumTemperatureCoefficient1() const;
    bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const;

    double returnAirFractionFunctionofPlenumTemperatureCoefficient2() const;
    bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets designLevelCalculationMethod to 'LightingLevel', clears wattsperSpaceFloorArea
    /// and wattsperPerson, and sets lightingLevel.
    bool setLightingLevel(double lightingLevel);

    /// Sets designLevelCalculationMethod to 'Watts/Area', clears lightingLevel
    /// and wattsperPerson, and sets wattsperSpaceFloorArea.
    bool setWattsperSpaceFloorArea(double wattsperSpaceFloorArea);

    /// Sets designLevelCalculationMethod to 'Watts/Person', clears lightingLevel
    /// and wattsperSpaceFloorArea, and sets wattsperPerson.
    bool setWattsperPerson(double wattsperPerson);

    bool setFractionRadiant(double fractionRadiant);
    void resetFractionRadiant();

    bool setFractionVisible(double fractionVisible);
    void resetFractionVisible();

    bool setReturnAirFraction(double returnAirFraction);
    void resetReturnAirFraction();

    bool setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature);
    void resetReturnAirFractionCalculatedfromPlenumTemperature();

    bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1);
    void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();

    bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2);
    void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();

    //@}
    /** @name Other */
    //@{

    /** Returns the lighting level represented by this definition, assuming floorArea (m^2) and
   *  numPeople. */
    double getLightingPower(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    /** If method is a \link validDesignLevelCalculationMethodValues valid design level
   *  calculation method \endlink, changes this definition to an equivalent lighting level, under
   *  the assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople);

    //@}
   protected:
    /// @cond
    using ImplType = detail::LightsDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit LightsDefinition(std::shared_ptr<detail::LightsDefinition_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LightsDefinition");
  };

  /** \relates LightsDefinition*/
  using OptionalLightsDefinition = boost::optional<LightsDefinition>;

  /** \relates LightsDefinition*/
  using LightsDefinitionVector = std::vector<LightsDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTSDEFINITION_HPP
