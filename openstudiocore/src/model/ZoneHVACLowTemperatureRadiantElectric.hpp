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

#ifndef MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP
#define MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACLowTemperatureRadiantElectric_Impl;

} // detail

/** ZoneHVACLowTemperatureRadiantElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:Electric'. */
class MODEL_API ZoneHVACLowTemperatureRadiantElectric : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACLowTemperatureRadiantElectric(const Model& model, Schedule & availabilitySchedule, Schedule & heatingTemperatureSchedule);

  virtual ~ZoneHVACLowTemperatureRadiantElectric() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> radiantSurfaceTypeValues();

  static std::vector<std::string> temperatureControlTypeValues();

  /** @name Getters */

  Schedule availabilitySchedule() const;

  Schedule heatingSetpointTemperatureSchedule() const;

  boost::optional<std::string> radiantSurfaceType() const;

  std::vector<Surface> surfaces() const;

  /** In EnergyPlus 8.2.0 and above this property maps to the EnergyPlus field "Heating Design Capacity" **/
  boost::optional<double> maximumElectricalPowertoPanel() const;

  bool isMaximumElectricalPowertoPanelDefaulted() const;

  bool isMaximumElectricalPowertoPanelAutosized() const;

  std::string temperatureControlType() const;

  bool isTemperatureControlTypeDefaulted() const;

  double heatingThrottlingRange() const;

  bool isHeatingThrottlingRangeDefaulted() const;

  /** @name Setters */

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

  bool setRadiantSurfaceType(std::string radiantSurfaceType);

  void resetRadiantSurfaceType();

  bool setMaximumElectricalPowertoPanel(double maximumElectricalPowertoPanel);

  void resetMaximumElectricalPowertoPanel();

  void autosizeMaximumElectricalPowertoPanel();

  bool setTemperatureControlType(std::string temperatureControlType);

  void resetTemperatureControlType();

  bool setHeatingThrottlingRange(double heatingThrottlingRange);

  void resetHeatingThrottlingRange();

  boost::optional<ThermalZone> thermalZone() const;

  bool addToThermalZone(ThermalZone & thermalZone);

  void removeFromThermalZone();


  /** @name Other */

  boost::optional<double> autosizedMaximumElectricalPowertoPanel() const ;



  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACLowTemperatureRadiantElectric_Impl ImplType;

  explicit ZoneHVACLowTemperatureRadiantElectric(std::shared_ptr<detail::ZoneHVACLowTemperatureRadiantElectric_Impl> impl);

  friend class detail::ZoneHVACLowTemperatureRadiantElectric_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACLowTemperatureRadiantElectric");
};

/** \relates ZoneHVACLowTemperatureRadiantElectric*/
typedef boost::optional<ZoneHVACLowTemperatureRadiantElectric> OptionalZoneHVACLowTemperatureRadiantElectric;

/** \relates ZoneHVACLowTemperatureRadiantElectric*/
typedef std::vector<ZoneHVACLowTemperatureRadiantElectric> ZoneHVACLowTemperatureRadiantElectricVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_HPP

