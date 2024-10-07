/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKZONE_HPP
#define MODEL_AIRFLOWNETWORKZONE_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class ThermalZone;
  class Schedule;
  class Schedule;
  class AirflowNetworkOccupantVentilationControl;

  namespace detail {

    class ThermalZone_Impl;
    class AirflowNetworkZone_Impl;

  }  // namespace detail

  /** AirflowNetworkZone is a AirflowNetworkNode that wraps the OpenStudio IDD object 'OS:AirflowNetworkZone'. */
  class MODEL_API AirflowNetworkZone : public AirflowNetworkNode
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AirflowNetworkZone() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkZone(const AirflowNetworkZone& other) = default;
    AirflowNetworkZone(AirflowNetworkZone&& other) = default;
    AirflowNetworkZone& operator=(const AirflowNetworkZone&) = default;
    AirflowNetworkZone& operator=(AirflowNetworkZone&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> ventilationControlModeValues();

    static std::vector<std::string> singleSidedWindPressureCoefficientAlgorithmValues();

    /** @name Getters */
    //@{

    /** Returns the associated thermal zone. */
    ThermalZone thermalZone() const;
    /** Returns the ventilation control mode. */
    std::string ventilationControlMode() const;
    /** Returns true if the ventilation control mode is defaulted. */
    bool isVentilationControlModeDefaulted() const;

    /** Returns the ventilation control zone temperature setpoint schedule, if any. */
    boost::optional<Schedule> ventilationControlZoneTemperatureSetpointSchedule() const;
    /** Returns the minimum venting open factor. */
    double minimumVentingOpenFactor() const;
    /** Returns true if the minimum venting open factor is defaulted. */
    bool isMinimumVentingOpenFactorDefaulted() const;
    /** Returns the temperature difference lower limit for maximum venting factor. */
    double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const;
    /** Returns true if the temperature difference lower limit for maximum venting factor is defaulted. */
    bool isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;
    /** Returns the temperature difference upper limit for minimum venting factor. */
    double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const;
    /** Returns true if the temperature difference upper limit for minimum venting factor is defaulted. */
    bool isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;
    /** Returns the enthalpy difference lower limit for maximum venting factor. */
    double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const;
    /** Returns true if the enthalpy difference lower limit for maximum venting factor is defaulted. */
    bool isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;
    /** Returns the enthalpy difference upper limit for minimum venting factor. */
    double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const;
    /** Returns true if the enthalpy difference upper limit for minimum venting factor is defaulted. */
    bool isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

    /** Returns the venting availability schedule, if any. */
    boost::optional<Schedule> ventingAvailabilitySchedule() const;
    /** Returns the single sided wind pressure algorithm. */
    std::string singleSidedWindPressureCoefficientAlgorithm() const;
    /** Returns true if the single sided wind pressure algorithm is defaulted. */
    bool isSingleSidedWindPressureCoefficientAlgorithmDefaulted() const;
    /** Returns the single sided wind pressure algorithm facade width. */
    double facadeWidth() const;
    /** Returns true if the single sided wind pressure algorithm facade width is defaulted. */
    bool isFacadeWidthDefaulted() const;

    /** Returns the occupant ventilation control object for this zone, if any. */
    boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the ventilation control mode. */
    bool setVentilationControlMode(const std::string& ventilationControlMode);
    /** Resets the ventilation control mode. */
    void resetVentilationControlMode();

    /** Sets the ventilation control zone temperature setpoint schedule. */
    bool setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule);
    /** Resets the ventilation control zone temperature setpoint schedule. */
    void resetVentilationControlZoneTemperatureSetpointSchedule();
    /** Sets the minimum venting open factor. */
    bool setMinimumVentingOpenFactor(double minimumVentingOpenFactor);
    /** Resets the minimum venting open factor. */
    void resetMinimumVentingOpenFactor();
    /** Sets the temperature difference lower limit for maximum venting factor. */
    bool setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
    /** Resets the temperature difference lower limit for maximum venting factor. */
    void resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
    /** Sets the temperature difference upper limit for minimum venting factor. */
    bool setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
    /** Resets the temperature difference upper limit for minimum venting factor. */
    void resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
    /** Sets the enthalpy difference lower limit for maximum venting factor. */
    bool setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
    /** Resets the enthalpy difference lower limit for maximum venting factor. */
    void resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
    /** Sets the enthalphy difference upper limit for minimum venting factor. */
    bool setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
    /** Resets the enthalphy difference upper limit for minimum venting factor. */
    void resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();

    /** Sets the venting availability schedule. */
    bool setVentingAvailabilitySchedule(Schedule& schedule);
    /** Resets the venting availability schedule. */
    void resetVentingAvailabilitySchedule();
    /** Sets the single sided wind pressure algorithm. */
    bool setSingleSidedWindPressureCoefficientAlgorithm(const std::string& singleSidedWindPressureCoefficientAlgorithm);
    /** Resets the single sided wind pressure algorithm. */
    void resetSingleSidedWindPressureCoefficientAlgorithm();
    /** Sets the single sided wind pressure algorithm facade width. */
    bool setFacadeWidth(double facadeWidth);
    /** Resets the single sided wind pressure algorithm facade width. */
    void resetFacadeWidth();

    /** Sets the occupant ventilation control object for this zone. */
    bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);
    /** Resets the occupant ventilation control object. */
    void resetOccupantVentilationControl();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkZone_Impl;

    explicit AirflowNetworkZone(std::shared_ptr<detail::AirflowNetworkZone_Impl> impl);
    AirflowNetworkZone(const Model& model, const Handle& handle);

    bool setThermalZone(const ThermalZone& thermalZone);
    void resetThermalZone();

    friend class detail::ThermalZone_Impl;
    friend class detail::AirflowNetworkZone_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkZone");
  };

  /** \relates AirflowNetworkZone*/
  using OptionalAirflowNetworkZone = boost::optional<AirflowNetworkZone>;

  /** \relates AirflowNetworkZone*/
  using AirflowNetworkZoneVector = std::vector<AirflowNetworkZone>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKZONE_HPP
