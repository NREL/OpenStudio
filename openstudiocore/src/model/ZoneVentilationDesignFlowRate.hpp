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

#ifndef MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP
#define MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class ZoneVentilationDesignFlowRate_Impl;

} // detail

/** ZoneVentilationDesignFlowRate is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneVentilation:DesignFlowRate'. */
class MODEL_API ZoneVentilationDesignFlowRate : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneVentilationDesignFlowRate(const Model& model);

  virtual ~ZoneVentilationDesignFlowRate() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> designFlowRateCalculationMethodValues();

  static std::vector<std::string> ventilationTypeValues();

  /** @name Getters */
  //@{

  Schedule schedule() const;

  std::string designFlowRateCalculationMethod() const;

  double designFlowRate() const;

  double flowRateperZoneFloorArea() const;

  double flowRateperPerson() const;

  double airChangesperHour() const;

  std::string ventilationType() const;

  double fanPressureRise() const;

  double fanTotalEfficiency() const;

  double constantTermCoefficient() const;

  double temperatureTermCoefficient() const;

  double velocityTermCoefficient() const;

  double velocitySquaredTermCoefficient() const;

  double minimumIndoorTemperature() const;

  boost::optional<Schedule> minimumIndoorTemperatureSchedule() const;

  double maximumIndoorTemperature() const;

  boost::optional<Schedule> maximumIndoorTemperatureSchedule() const;

  double deltaTemperature() const;

  boost::optional<Schedule> deltaTemperatureSchedule() const;

  double minimumOutdoorTemperature() const;

  boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

  double maximumOutdoorTemperature() const;

  boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

  double maximumWindSpeed() const;

  //@}
  /** @name Setters */
  //@{

  bool setSchedule(Schedule& schedule);

  /** \deprecated ZoneVentilationDesignFlowRate::setDesignFlowRateCalculationMethod has been deprecated and will be removed in a future release, \n
   *  the design flow rate calculation method is set during the call to setDesignFlowRate, setFlowRateperZoneFloorArea, setAirChangesperHour, etc
   **/
  OS_DEPRECATED bool setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod);

  bool setDesignFlowRate(double designFlowRate);

  bool setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea);

  bool setFlowRateperPerson(double flowRateperPerson);

  bool setAirChangesperHour(double airChangesperHour);

  bool setVentilationType(std::string ventilationType);

  bool setFanPressureRise(double fanPressureRise);

  bool setFanTotalEfficiency(double fanTotalEfficiency);

  void setConstantTermCoefficient(double constantTermCoefficient);

  void setTemperatureTermCoefficient(double temperatureTermCoefficient);

  void setVelocityTermCoefficient(double velocityTermCoefficient);

  void setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient);

  bool setMinimumIndoorTemperature(double minimumIndoorTemperature);

  bool setMinimumIndoorTemperatureSchedule(Schedule& schedule);

  void resetMinimumIndoorTemperatureSchedule();

  bool setMaximumIndoorTemperature(double maximumIndoorTemperature);

  bool setMaximumIndoorTemperatureSchedule(Schedule& schedule);

  void resetMaximumIndoorTemperatureSchedule();

  bool setDeltaTemperature(double deltaTemperature);

  bool setDeltaTemperatureSchedule(Schedule& schedule);

  void resetDeltaTemperatureSchedule();

  bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

  bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMinimumOutdoorTemperatureSchedule();

  bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

  bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

  void resetMaximumOutdoorTemperatureSchedule();

  bool setMaximumWindSpeed(double maximumWindSpeed);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneVentilationDesignFlowRate_Impl ImplType;

  explicit ZoneVentilationDesignFlowRate(std::shared_ptr<detail::ZoneVentilationDesignFlowRate_Impl> impl);

  friend class detail::ZoneVentilationDesignFlowRate_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneVentilationDesignFlowRate");
};

/** \relates ZoneVentilationDesignFlowRate*/
typedef boost::optional<ZoneVentilationDesignFlowRate> OptionalZoneVentilationDesignFlowRate;

/** \relates ZoneVentilationDesignFlowRate*/
typedef std::vector<ZoneVentilationDesignFlowRate> ZoneVentilationDesignFlowRateVector;

} // model
} // openstudio

#endif // MODEL_ZONEVENTILATIONDESIGNFLOWRATE_HPP

