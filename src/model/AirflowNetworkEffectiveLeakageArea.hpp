/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_HPP
#define MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkEffectiveLeakageArea_Impl;

  }  // namespace detail

  /** AirflowNetworkEffectiveLeakageArea is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkEffectiveLeakageArea'. */
  class MODEL_API AirflowNetworkEffectiveLeakageArea : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct an effective leakage area object with default values and a specified effective leakage area. */
    AirflowNetworkEffectiveLeakageArea(const Model& model, double effectiveLeakageArea);
    /** Construct an effective leakage area object. */
    AirflowNetworkEffectiveLeakageArea(const Model& model, double effectiveLeakageArea, double dischargeCoefficient,
                                       double referencePressureDifference, double massFlowExponent);

    virtual ~AirflowNetworkEffectiveLeakageArea() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkEffectiveLeakageArea(const AirflowNetworkEffectiveLeakageArea& other) = default;
    AirflowNetworkEffectiveLeakageArea(AirflowNetworkEffectiveLeakageArea&& other) = default;
    AirflowNetworkEffectiveLeakageArea& operator=(const AirflowNetworkEffectiveLeakageArea&) = default;
    AirflowNetworkEffectiveLeakageArea& operator=(AirflowNetworkEffectiveLeakageArea&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the effective leakage area in square meters. */
    double effectiveLeakageArea() const;
    /** Returns the discharge coefficient. */
    double dischargeCoefficient() const;
    /** Returns true if the discharge coefficient is defaulted. */
    bool isDischargeCoefficientDefaulted() const;
    /** Returns the reference pressure difference in Pa. */
    double referencePressureDifference() const;
    /** Returns true if the reference pressure difference is defaulted. */
    bool isReferencePressureDifferenceDefaulted() const;
    /** Returns the mass flow exponent. */
    double airMassFlowExponent() const;
    /** Returns true if the mass flow exponent. */
    bool isAirMassFlowExponentDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the effective leakage area in square meters. */
    bool setEffectiveLeakageArea(double effectiveLeakageArea);
    /** Sets the discharge coefficient. */
    bool setDischargeCoefficient(double dischargeCoefficient);
    /** Returns the discharge coefficient to the default value. */
    void resetDischargeCoefficient();
    /** Sets the reference pressure difference in Pa. */
    bool setReferencePressureDifference(double referencePressureDifference);
    /** Returns the reference pressure difference to the default value. */
    void resetReferencePressureDifference();
    /** Sets the mass flow exponent. */
    bool setAirMassFlowExponent(double airMassFlowExponent);
    /** Returns the mass flow exponent to the default value. */
    void resetAirMassFlowExponent();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkEffectiveLeakageArea_Impl;

    explicit AirflowNetworkEffectiveLeakageArea(std::shared_ptr<detail::AirflowNetworkEffectiveLeakageArea_Impl> impl);

    friend class detail::AirflowNetworkEffectiveLeakageArea_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkEffectiveLeakageArea");
  };

  /** \relates AirflowNetworkEffectiveLeakageArea*/
  using OptionalAirflowNetworkEffectiveLeakageArea = boost::optional<AirflowNetworkEffectiveLeakageArea>;

  /** \relates AirflowNetworkEffectiveLeakageArea*/
  using AirflowNetworkEffectiveLeakageAreaVector = std::vector<AirflowNetworkEffectiveLeakageArea>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEFFECTIVELEAKAGEAREA_HPP
