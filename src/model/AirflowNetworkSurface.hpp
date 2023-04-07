/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~AirflowNetworkSurface() = default;
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
