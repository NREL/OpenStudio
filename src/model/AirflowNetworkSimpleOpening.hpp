/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSIMPLEOPENING_HPP
#define MODEL_AIRFLOWNETWORKSIMPLEOPENING_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkSimpleOpening_Impl;

  }  // namespace detail

  /** AirflowNetworkSimpleOpening is a AirflowNetworkComponent that wraps the OpenStudio IDD object 'OS:AirflowNetworkSimpleOpening'. */
  class MODEL_API AirflowNetworkSimpleOpening : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a simple opening object with default mass flow exponent. */
    AirflowNetworkSimpleOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double minimumDensityDifferenceforTwoWayFlow,
                                double dischargeCoefficient);
    /** Construct a simple opening object. */
    AirflowNetworkSimpleOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed, double massFlowExponentWhenOpeningisClosed,
                                double minimumDensityDifferenceforTwoWayFlow, double dischargeCoefficient);

    virtual ~AirflowNetworkSimpleOpening() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkSimpleOpening(const AirflowNetworkSimpleOpening& other) = default;
    AirflowNetworkSimpleOpening(AirflowNetworkSimpleOpening&& other) = default;
    AirflowNetworkSimpleOpening& operator=(const AirflowNetworkSimpleOpening&) = default;
    AirflowNetworkSimpleOpening& operator=(AirflowNetworkSimpleOpening&&) = default;

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
    /** Returns the minimum density difference above which two-way flow may occur due to stack effect in kg/m3. */
    double minimumDensityDifferenceforTwoWayFlow() const;
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
    /** Sets the minimum density difference above which two-way flow may occur due to stack effect in kg/m3. */
    bool setMinimumDensityDifferenceforTwoWayFlow(double minimumDensityDifferenceforTwoWayFlow);
    /** Sets the discharge coefficient. */
    bool setDischargeCoefficient(double dischargeCoefficient);

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkSimpleOpening_Impl;

    explicit AirflowNetworkSimpleOpening(std::shared_ptr<detail::AirflowNetworkSimpleOpening_Impl> impl);

    friend class detail::AirflowNetworkSimpleOpening_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkSimpleOpening");
  };

  /** \relates AirflowNetworkSimpleOpening*/
  using OptionalAirflowNetworkSimpleOpening = boost::optional<AirflowNetworkSimpleOpening>;

  /** \relates AirflowNetworkSimpleOpening*/
  using AirflowNetworkSimpleOpeningVector = std::vector<AirflowNetworkSimpleOpening>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSIMPLEOPENING_HPP
