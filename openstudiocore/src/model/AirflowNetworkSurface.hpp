/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKSURFACE_HPP
#define MODEL_AIRFLOWNETWORKSURFACE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
//class SurfAndSubSurf;
class PlanarSurface;
//class SurfaceAirflowLeakage;
//class ExternalNode;
class Schedule;
//class Schedule;
//class AirflowNetworkOccupantVentilationControl;

namespace detail {

class AirflowNetworkSurface_Impl;

} // detail

/** AirflowNetworkSurface is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:Surface'. */
class MODEL_API AirflowNetworkSurface : public ModelObject {
public:
  /** @name Constructors and Destructors */
  //@{

  AirflowNetworkSurface(const Model& model, const PlanarSurface &surface);

  virtual ~AirflowNetworkSurface() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> ventilationControlModeValues();

  /** @name Getters */
  //@{

  /** Returns the planar surface associated with this object. */
  boost::optional<PlanarSurface> surface() const;

  // TODO: Check return type. From object lists, some candidates are: SurfaceAirflowLeakage.
  //SurfaceAirflowLeakage leakageComponent() const;

  // TODO: Check return type. From object lists, some candidates are: ExternalNode.
  //boost::optional<ExternalNode> externalNode() const;
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

  double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const;

  bool isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;

  double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const;

  bool isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

  double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const;

  bool isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;

  double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const;

  bool isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> ventingAvailabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: AirflowNetworkOccupantVentilationControl.
  //boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: SurfAndSubSurf.
  bool setSurface(const PlanarSurface& surface);

  // TODO: Check argument type. From object lists, some candidates are: SurfaceAirflowLeakage.
  //bool setLeakageComponent(const SurfaceAirflowLeakage& surfaceAirflowLeakage);

  // TODO: Check argument type. From object lists, some candidates are: ExternalNode.
  //bool setExternalNode(const ExternalNode& externalNode);

  //void resetExternalNode();

  bool setWindowDoorOpeningFactorOrCrackFactor(double windowDoorOpeningFactorOrCrackFactor);

  void resetWindowDoorOpeningFactorOrCrackFactor();

  bool setVentilationControlMode(std::string ventilationControlMode);

  void resetVentilationControlMode();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule);

  void resetVentilationControlZoneTemperatureSetpointSchedule();

  bool setMinimumVentingOpenFactor(double minimumVentingOpenFactor);

  void resetMinimumVentingOpenFactor();

  bool setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);

  void resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();

  bool setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);

  void resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();

  bool setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);

  void resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();

  bool setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);

  void resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setVentingAvailabilitySchedule(Schedule& schedule);

  void resetVentingAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: AirflowNetworkOccupantVentilationControl.
  //bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);

  //void resetOccupantVentilationControl();

  //@}
  /** @name Other */
  //@{

  //@}
protected:
  /// @cond
  typedef detail::AirflowNetworkSurface_Impl ImplType;

  explicit AirflowNetworkSurface(std::shared_ptr<detail::AirflowNetworkSurface_Impl> impl);

  friend class detail::AirflowNetworkSurface_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
private:
  REGISTER_LOGGER("openstudio.model.AirflowNetworkSurface");
};

/** \relates AirflowNetworkSurface*/
typedef boost::optional<AirflowNetworkSurface> OptionalAirflowNetworkSurface;

/** \relates AirflowNetworkSurface*/
typedef std::vector<AirflowNetworkSurface> AirflowNetworkSurfaceVector;

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKSURFACE_HPP

