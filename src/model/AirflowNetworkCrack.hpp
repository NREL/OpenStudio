/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCRACK_HPP
#define MODEL_AIRFLOWNETWORKCRACK_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkReferenceCrackConditions;

  namespace detail {

    class AirflowNetworkCrack_Impl;

  }  // namespace detail

  /** AirflowNetworkCrack is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkCrack'. */
  class MODEL_API AirflowNetworkCrack : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a surface crack object with default values and a specified mass flow coefficient. */
    AirflowNetworkCrack(const Model& model, double massFlowCoefficient);
    /** Construct a surface crack object without a reference conditions object. */
    AirflowNetworkCrack(const Model& model, double massFlowCoefficient, double massFlowExponent);
    /** Construct a surface crack object. */
    AirflowNetworkCrack(const Model& model, double massFlowCoefficient, double massFlowExponent,
                        const AirflowNetworkReferenceCrackConditions& referenceCrackConditions);

    virtual ~AirflowNetworkCrack() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkCrack(const AirflowNetworkCrack& other) = default;
    AirflowNetworkCrack(AirflowNetworkCrack&& other) = default;
    AirflowNetworkCrack& operator=(const AirflowNetworkCrack&) = default;
    AirflowNetworkCrack& operator=(AirflowNetworkCrack&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the mass flow coefficient in kg/s at 1 Pa. */
    double airMassFlowCoefficient() const;
    /** Returns the mass flow exponent. */
    double airMassFlowExponent() const;
    /** Returns true if the mass flow exponent is defaulted. */
    bool isAirMassFlowExponentDefaulted() const;
    /** Returns a reference crack conditions object if available. */
    boost::optional<AirflowNetworkReferenceCrackConditions> referenceCrackConditions() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the mass flow coefficient in kg/s at 1 Pa.  */
    bool setAirMassFlowCoefficient(double airMassFlowCoefficientatReferenceConditions);
    /** Sets the mass flow exponent. */
    bool setAirMassFlowExponent(double airMassFlowExponent);
    /** Returns the mass flow exponent to the default value. */
    void resetAirMassFlowExponent();
    /** Sets the the reference crack conditions. */
    bool setReferenceCrackConditions(const AirflowNetworkReferenceCrackConditions& referenceCrackConditions);
    /** Resets the the reference crack conditions. */
    void resetReferenceCrackConditions();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkCrack_Impl;

    explicit AirflowNetworkCrack(std::shared_ptr<detail::AirflowNetworkCrack_Impl> impl);

    friend class detail::AirflowNetworkCrack_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkCrack");
  };

  /** \relates AirflowNetworkCrack*/
  using OptionalAirflowNetworkCrack = boost::optional<AirflowNetworkCrack>;

  /** \relates AirflowNetworkCrack*/
  using AirflowNetworkCrackVector = std::vector<AirflowNetworkCrack>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCRACK_HPP
