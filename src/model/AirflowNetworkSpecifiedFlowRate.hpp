/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSpecifiedFlowRate_HPP
#define MODEL_AIRFLOWNETWORKSpecifiedFlowRate_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkSpecifiedFlowRate_Impl;

  }  // namespace detail

  /** AirflowNetworkSpecifiedFlowRate is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkSpecifiedFlowRate'. */
  class MODEL_API AirflowNetworkSpecifiedFlowRate : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a specified flow rate object with a zero airflow value. */
    AirflowNetworkSpecifiedFlowRate(const Model& model);
    /** Construct a specified flow rate object with a specified airflow value. */
    AirflowNetworkSpecifiedFlowRate(const Model& model, double airFlowValue);

    virtual ~AirflowNetworkSpecifiedFlowRate() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkSpecifiedFlowRate(const AirflowNetworkSpecifiedFlowRate& other) = default;
    AirflowNetworkSpecifiedFlowRate(AirflowNetworkSpecifiedFlowRate&& other) = default;
    AirflowNetworkSpecifiedFlowRate& operator=(const AirflowNetworkSpecifiedFlowRate&) = default;
    AirflowNetworkSpecifiedFlowRate& operator=(AirflowNetworkSpecifiedFlowRate&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> airFlowUnitsValues();

    static std::vector<std::string> validAirFlowUnitsValues();

    /** @name Getters */
    //@{
    /** Returns the air flow value. */
    double airFlowValue() const;
    /** Returns the air flow units. */
    std::string airFlowUnits() const;
    /** Returns true if the air flow units is defaulted. */
    bool isAirFlowUnitsDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the air flow value. */
    bool setAirFlowValue(double airFlowValue);
    /** Sets the air flow units. */
    bool setAirFlowUnits(const std::string& airFlowUnits);
    /** Returns the air flow units to the default value. */
    void resetAirFlowUnits();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkSpecifiedFlowRate_Impl;

    explicit AirflowNetworkSpecifiedFlowRate(std::shared_ptr<detail::AirflowNetworkSpecifiedFlowRate_Impl> impl);

    friend class detail::AirflowNetworkSpecifiedFlowRate_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkSpecifiedFlowRate");
  };

  /** \relates AirflowNetworkSpecifiedFlowRate*/
  using OptionalAirflowNetworkSpecifiedFlowRate = boost::optional<AirflowNetworkSpecifiedFlowRate>;

  /** \relates AirflowNetworkSpecifiedFlowRate*/
  using AirflowNetworkSpecifiedFlowRateVector = std::vector<AirflowNetworkSpecifiedFlowRate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSpecifiedFlowRate_HPP
