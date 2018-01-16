/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP
#define MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

class ThermalZone;
class Schedule;
class Curve;

namespace detail {

  class AvailabilityManagerHybridVentilation_Impl;

} // detail

/** AvailabilityManagerHybridVentilation is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:HybridVentilation'. */
class MODEL_API AvailabilityManagerHybridVentilation : public AvailabilityManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  // Constructor for AvailabilityManagerHybridVentilation.
  // The required "ventilationControlModeSchedule" will be initialized with
  // a new schedule having a constant value of one, meaning the availability manager
  // will operate with temperature control.
  // The required "minimumOutdoorVentilationAirSchedule" will be initialized with a
  // schedule of constant value 0. This schedule is only applicable if control mode is 0,
  // but EnergyPlus requires it to operate.
  explicit AvailabilityManagerHybridVentilation(const Model& model);

  // Constructor for AvailabilityManagerHybridVentilation.
  // The required "ventilationControlModeSchedule" must be provided.
  // The required "minimumOutdoorVentilationAirSchedule" must be provided.
  explicit AvailabilityManagerHybridVentilation(const Model& model, Schedule& ventilationControlModeSchedule, Schedule& minimumOutdoorVentilationAirSchedule);

  virtual ~AvailabilityManagerHybridVentilation() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<ThermalZone> controlledZone() const;

  Schedule ventilationControlModeSchedule() const;

  bool useWeatherFileRainIndicators() const;

  double maximumWindSpeed() const;

  double minimumOutdoorTemperature() const;

  double maximumOutdoorTemperature() const;

  double minimumOutdoorEnthalpy() const;

  double maximumOutdoorEnthalpy() const;

  double minimumOutdoorDewpoint() const;

  double maximumOutdoorDewpoint() const;

  Schedule minimumOutdoorVentilationAirSchedule() const;

  boost::optional<Curve> openingFactorFunctionofWindSpeedCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlledZone(const ThermalZone& thermalZone);

  void resetControlledZone();

  bool setVentilationControlModeSchedule(Schedule& schedule);

  bool setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators);

  bool setMaximumWindSpeed(double maximumWindSpeed);

  bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

  bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

  bool setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy);

  bool setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy);

  bool setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint);

  bool setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint);

  bool setMinimumOutdoorVentilationAirSchedule(Schedule& schedule);

  bool setOpeningFactorFunctionofWindSpeedCurve(const Curve& curve);

  void resetOpeningFactorFunctionofWindSpeedCurve();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AvailabilityManagerHybridVentilation_Impl ImplType;

  explicit AvailabilityManagerHybridVentilation(std::shared_ptr<detail::AvailabilityManagerHybridVentilation_Impl> impl);

  friend class detail::AvailabilityManagerHybridVentilation_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AvailabilityManagerHybridVentilation");
};

/** \relates AvailabilityManagerHybridVentilation*/
typedef boost::optional<AvailabilityManagerHybridVentilation> OptionalAvailabilityManagerHybridVentilation;

/** \relates AvailabilityManagerHybridVentilation*/
typedef std::vector<AvailabilityManagerHybridVentilation> AvailabilityManagerHybridVentilationVector;

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_HPP

