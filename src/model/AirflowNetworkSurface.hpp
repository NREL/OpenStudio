/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSURFACE_HPP
#define MODEL_AIRFLOWNETWORKSURFACE_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkLinkage.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class PlanarSurface;
  class Surface;
  class SubSurface;
  class AirflowNetworkComponent;
  class AirflowNetworkExternalNode;
  class Schedule;
  class AirflowNetworkOccupantVentilationControl;

  namespace detail {

    class Surface_Impl;
    class SubSurface_Impl;
    class AirflowNetworkSurface_Impl;

  }  // namespace detail

  /** AirflowNetworkSurface is a AirflowNetworkLinkage that wraps the OpenStudio IDD object 'OS:AirflowNetwork:Surface'. */
  class MODEL_API AirflowNetworkSurface : public AirflowNetworkLinkage
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Construct a surface with a model surface to be linked to. */
    ///AirflowNetworkSurface(const Model& model, const Surface &surface);
    /** Construct a surface with a model subsurface to be linked to. */
    //AirflowNetworkSurface(const Model& model, const SubSurface &surface);
    /** Construct a surface with a model surface to be linked to and a leakage component. */
    //AirflowNetworkSurface(const Model& model, const Surface &surface, const AirflowNetworkComponent &component);
    /** Construct a surface with a model subsurface to be linked to and a leakage component. */
    //AirflowNetworkSurface(const Model& model, const SubSurface &surface, const AirflowNetworkComponent &component);

    virtual ~AirflowNetworkSurface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkSurface(const AirflowNetworkSurface& other) = default;
    AirflowNetworkSurface(AirflowNetworkSurface&& other) = default;
    AirflowNetworkSurface& operator=(const AirflowNetworkSurface&) = default;
    AirflowNetworkSurface& operator=(AirflowNetworkSurface&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> ventilationControlModeValues();

    /** @name Getters */
    //@{

    /** Returns the planar surface associated with this object. */
    PlanarSurface surface() const;

    /** Returns the leakage component associated with the surface, if any. */
    boost::optional<AirflowNetworkComponent> leakageComponent() const;

    /** Returns the external node associated with this surface, if any. */
    boost::optional<AirflowNetworkExternalNode> externalNode() const;
    /** Returns the window opening, door opening, or crack factor for this object. */
    double windowDoorOpeningFactorOrCrackFactor() const;
    /** Returns true if the window opening, door opening, or crack factor for this object is defaulted. */
    bool isWindowDoorOpeningFactorOrCrackFactorDefaulted() const;
    /** Returns the ventilation control mode for this object. */
    std::string ventilationControlMode() const;
    /** Returns true if the ventilation control mode for this object is defaulted. */
    bool isVentilationControlModeDefaulted() const;

    /** Returns the ventilation control schedule if this object has one. */
    boost::optional<Schedule> ventilationControlZoneTemperatureSetpointSchedule() const;
    /** Returns the minimum venting open factor for this object. */
    double minimumVentingOpenFactor() const;
    /** Returns true if the minimum venting open factor for this object is defaulted. */
    bool isMinimumVentingOpenFactorDefaulted() const;
    /** Returns the lower limit indoor/outdoor temperature difference. */
    double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const;
    /** Returns true if the lower limit indoor/outdoor temperature difference is defaulted. */
    bool isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;
    /** Returns the upper limit indoor/outdoor temperature difference. */
    double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const;
    /** Returns true if the upper limit indoor/outdoor temperature difference is defaulted. */
    bool isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;
    /** Returns the lower limit indoor/outdoor enthalpy difference. */
    double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const;
    /** Returns true if the lower limit indoor/outdoor enthalpy difference is defaulted. */
    bool isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;
    /** Returns the upper limit indoor/outdoor enthalpy difference. */
    double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const;
    /** Returns true if the upper limit indoor/outdoor enthalpy difference is defaulted. */
    bool isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

    /** Returns the venting availability for this surface, if any. */
    boost::optional<Schedule> ventingAvailabilitySchedule() const;

    /** Returns the ventilation control object associated with this surface, if any.*/
    boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

    //@}
    /** @name Setters */
    //@{

    /** Set the component associated with this surface. */
    bool setLeakageComponent(const AirflowNetworkComponent& surfaceAirflowLeakage);

    /** Set the external node associated with this surface. */
    bool setExternalNode(const AirflowNetworkExternalNode& externalNode);

    /** Remove the external node currently associated with this surface. */
    void resetExternalNode();
    /** Sets the opening factor/crack factor. */
    bool setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor);
    /** Resets the opening factor/crack factor. */
    void resetWindowDoorOpeningFactorOrCrackFactor();
    /** Sets the ventilation control mode. */
    bool setVentilationControlMode(const std::string& ventilationControlMode);
    /** Resets the ventilation control mode. */
    void resetVentilationControlMode();

    /** Sets the control setpoint schedule. */
    bool setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule);
    /** Resets the control setpoint schedule. */
    void resetVentilationControlZoneTemperatureSetpointSchedule();
    /** Sets the minimum venting open schedule. */
    bool setMinimumVentingOpenFactor(double minimumVentingOpenFactor);
    /** Reets the minimum venting open schedule. */
    void resetMinimumVentingOpenFactor();
    /** Sets the lower limit indoor/outdoor temperature difference. */
    bool setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);
    /** Resets the lower limit indoor/outdoor temperature difference. */
    void resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();
    /** Sets the upper limit indoor/outdoor temperature difference. */
    bool setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);
    /** Resets the upper limit indoor/outdoor temperature difference. */
    void resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();
    /** Sets the lower limit indoor/outdoor enthalpy difference. */
    bool setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);
    /** Resets the upper limit indoor/outdoor temperature difference. */
    void resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();
    /** Sets the upper limit indoor/outdoor enthalpy difference. */
    bool setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
      double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);
    /** Resets the upper limit indoor/outdoor enthalpy difference. */
    void resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();

    /** Sets the venting availabilty schedule. */
    bool setVentingAvailabilitySchedule(Schedule& schedule);

    /** Resets the venting availabilty schedule. */
    void resetVentingAvailabilitySchedule();

    /** Set the occupant ventilation control object for this surface. */
    bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);

    /** Reset the occupant ventilation control object. */
    void resetOccupantVentilationControl();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkSurface_Impl;

    explicit AirflowNetworkSurface(std::shared_ptr<detail::AirflowNetworkSurface_Impl> impl);

    AirflowNetworkSurface(const Model& model, const Handle& compHandle, const Handle& surfHandle);

    bool setSurface(const Surface& surfAndSubSurf);
    bool setSurface(const SubSurface& surfAndSubSurf);
    void resetSurface();

    friend class detail::Surface_Impl;
    friend class detail::SubSurface_Impl;
    friend class detail::AirflowNetworkSurface_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkSurface");
  };

  /** \relates AirflowNetworkSurface*/
  using OptionalAirflowNetworkSurface = boost::optional<AirflowNetworkSurface>;

  /** \relates AirflowNetworkSurface*/
  using AirflowNetworkSurfaceVector = std::vector<AirflowNetworkSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSURFACE_HPP
