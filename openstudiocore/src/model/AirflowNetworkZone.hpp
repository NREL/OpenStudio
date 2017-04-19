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

#ifndef MODEL_AIRFLOWNETWORKZONE_HPP
#define MODEL_AIRFLOWNETWORKZONE_HPP

#include <model/ModelAPI.hpp>

#include "AirflowNetworkNode.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class ThermalZone;
class Schedule;
class Schedule;
class AirflowNetworkOccupantVentilationControl;

namespace detail {

  class AirflowNetworkZone_Impl;

} // detail

/** AirflowNetworkZone is a AirflowNetworkNode that wraps the OpenStudio IDD object 'OS:AirflowNetworkZone'. */
class MODEL_API AirflowNetworkZone : public AirflowNetworkNode {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirflowNetworkZone(const Model& model);

  virtual ~AirflowNetworkZone() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> ventilationControlModeValues();

  static std::vector<std::string> singleSidedWindPressureCoefficientAlgorithmValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  ThermalZone thermalZone() const;

  std::string ventilationControlMode() const;

  bool isVentilationControlModeDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> ventilationControlZoneTemperatureSetpointSchedule() const;

  double minimumVentingOpenFactor() const;

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

  std::string singleSidedWindPressureCoefficientAlgorithm() const;

  bool isSingleSidedWindPressureCoefficientAlgorithmDefaulted() const;

  double facadeWidth() const;

  bool isFacadeWidthDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: AirflowNetworkOccupantVentilationControl.
  boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setThermalZone(const ThermalZone& thermalZone);

  bool setVentilationControlMode(const std::string& ventilationControlMode);

  void resetVentilationControlMode();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
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
  // Note Schedules are passed by reference, not const reference.
  bool setVentingAvailabilitySchedule(Schedule& schedule);

  void resetVentingAvailabilitySchedule();

  bool setSingleSidedWindPressureCoefficientAlgorithm(const std::string& singleSidedWindPressureCoefficientAlgorithm);

  void resetSingleSidedWindPressureCoefficientAlgorithm();

  bool setFacadeWidth(double facadeWidth);

  void resetFacadeWidth();

  // TODO: Check argument type. From object lists, some candidates are: AirflowNetworkOccupantVentilationControl.
  bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);

  void resetOccupantVentilationControl();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirflowNetworkZone_Impl ImplType;

  explicit AirflowNetworkZone(std::shared_ptr<detail::AirflowNetworkZone_Impl> impl);

  friend class detail::AirflowNetworkZone_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirflowNetworkZone");
};

/** \relates AirflowNetworkZone*/
typedef boost::optional<AirflowNetworkZone> OptionalAirflowNetworkZone;

/** \relates AirflowNetworkZone*/
typedef std::vector<AirflowNetworkZone> AirflowNetworkZoneVector;

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKZONE_HPP

