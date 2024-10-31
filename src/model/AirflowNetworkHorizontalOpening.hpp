/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKHORIZONTALOPENING_HPP
#define MODEL_AIRFLOWNETWORKHORIZONTALOPENING_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkHorizontalOpening_Impl;

  }  // namespace detail

  /** AirflowNetworkHorizontalOpening is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkHorizontalOpening'. */
  class MODEL_API AirflowNetworkHorizontalOpening : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a horizontal opening object with default mass flow exponent and sloping plane angle. */
    AirflowNetworkHorizontalOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double dischargeCoefficient);
    /** Construct a horizontal opening object. */
    AirflowNetworkHorizontalOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double massFlowExponentWhenOpeningisClosed,
                                    double slopingPlaneAngle, double dischargeCoefficient);

    virtual ~AirflowNetworkHorizontalOpening() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkHorizontalOpening(const AirflowNetworkHorizontalOpening& other) = default;
    AirflowNetworkHorizontalOpening(AirflowNetworkHorizontalOpening&& other) = default;
    AirflowNetworkHorizontalOpening& operator=(const AirflowNetworkHorizontalOpening&) = default;
    AirflowNetworkHorizontalOpening& operator=(AirflowNetworkHorizontalOpening&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the mass flow coefficient when the opening is closed in kg/s-m at 1 Pa pressure difference. */
    double airMassFlowCoefficientWhenOpeningisClosed() const;
    /** Returns the mass flow exponent when the opening is closed. */
    double airMassFlowExponentWhenOpeningisClosed() const;
    /** Returns true if the  mass flow exponent is defaulted. */
    bool isAirMassFlowExponentWhenOpeningisClosedDefaulted() const;
    /** Returns the angle of the horizontal opening to a sloping plane beneath it (reducing the effective opening area)
      in degrees, 90 degrees corresponds to fully open. */
    double slopingPlaneAngle() const;
    /** Returns true if the sloping plane angle is defaulted. */
    bool isSlopingPlaneAngleDefaulted() const;
    /** Returns the discharge coefficient. */
    double dischargeCoefficient() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the mass flow coefficient when the opening is closed in kg/s-m at 1 Pa pressure difference. */
    bool setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed);
    /** Sets the mass flow exponent when the opening is closed. */
    bool setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed);
    /** Resets the mass flow exponent when the opening is closed to the default value. */
    void resetAirMassFlowExponentWhenOpeningisClosed();
    /** Sets the angle of the horizontal opening to a sloping plane beneath it (reducing the effective opening area)
  in degrees, 90 degrees corresponds to fully open. */
    bool setSlopingPlaneAngle(double slopingPlaneAngle);
    /** Resets the sloping plane to the default value. */
    void resetSlopingPlaneAngle();
    /** Sets the discharge coefficient. */
    bool setDischargeCoefficient(double dischargeCoefficient);

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkHorizontalOpening_Impl;

    explicit AirflowNetworkHorizontalOpening(std::shared_ptr<detail::AirflowNetworkHorizontalOpening_Impl> impl);

    friend class detail::AirflowNetworkHorizontalOpening_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkHorizontalOpening");
  };

  /** \relates AirflowNetworkHorizontalOpening*/
  using OptionalAirflowNetworkHorizontalOpening = boost::optional<AirflowNetworkHorizontalOpening>;

  /** \relates AirflowNetworkHorizontalOpening*/
  using AirflowNetworkHorizontalOpeningVector = std::vector<AirflowNetworkHorizontalOpening>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKHORIZONTALOPENING_HPP
