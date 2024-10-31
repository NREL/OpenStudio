/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LUMINAIREDEFINITION_HPP
#define MODEL_LUMINAIREDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class LuminaireDefinition_Impl;

  }  // namespace detail

  /** LuminaireDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object
 *  'OS_Luminaire_Definition'. \sa Luminaire */
  class MODEL_API LuminaireDefinition : public SpaceLoadDefinition
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    explicit LuminaireDefinition(const Model& model);

    virtual ~LuminaireDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LuminaireDefinition(const LuminaireDefinition& other) = default;
    LuminaireDefinition(LuminaireDefinition&& other) = default;
    LuminaireDefinition& operator=(const LuminaireDefinition&) = default;
    LuminaireDefinition& operator=(LuminaireDefinition&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    // TODO: Handle Non-Extensible IddField IES File Path.

    double lightingPower() const;
    bool isLightingPowerDefaulted() const;

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

    // TODO: Handle Non-Extensible IddField IES File Path.

    bool setLightingPower(double lightingPower);
    void resetLightingPower();

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
   protected:
    /// @cond
    using ImplType = detail::LuminaireDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit LuminaireDefinition(std::shared_ptr<detail::LuminaireDefinition_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LuminaireDefinition");
  };

  /** \relates LuminaireDefinition*/
  using OptionalLuminaireDefinition = boost::optional<LuminaireDefinition>;

  /** \relates LuminaireDefinition*/
  using LuminaireDefinitionVector = std::vector<LuminaireDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LUMINAIREDEFINITION_HPP
