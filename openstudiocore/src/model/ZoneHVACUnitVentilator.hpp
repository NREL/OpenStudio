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

#ifndef MODEL_ZONEHVACUNITVENTILATOR_HPP
#define MODEL_ZONEHVACUNITVENTILATOR_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class ZoneHVACUnitVentilator_Impl;

} // detail

/** ZoneHVACUnitVentilator is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:UnitVentilator'. */
class MODEL_API ZoneHVACUnitVentilator : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ZoneHVACUnitVentilator(const Model& model);

  explicit ZoneHVACUnitVentilator(
    const Model& model,
    const HVACComponent& supplyAirFan
  );

  virtual ~ZoneHVACUnitVentilator() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> outdoorAirControlTypeValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> maximumSupplyAirFlowRate() const;

  bool isMaximumSupplyAirFlowRateAutosized() const;

  std::string outdoorAirControlType() const;

  boost::optional<double> minimumOutdoorAirFlowRate() const;

  bool isMinimumOutdoorAirFlowRateAutosized() const;

  Schedule minimumOutdoorAirSchedule() const;

  boost::optional<double> maximumOutdoorAirFlowRate() const;

  bool isMaximumOutdoorAirFlowRateAutosized() const;

  Schedule maximumOutdoorAirFractionorTemperatureSchedule() const;

  HVACComponent supplyAirFan() const;

  boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

  boost::optional<HVACComponent> heatingCoil() const;

  double heatingConvergenceTolerance() const;

  boost::optional<HVACComponent> coolingCoil() const;

  double coolingConvergenceTolerance() const;

  // boost::optional<ModelObject> availabilityManagerList() const;

  // boost::optional<ModelObject> designSpecificationZoneHVACSizingObject() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate);

  void autosizeMaximumSupplyAirFlowRate();

  bool setOutdoorAirControlType(std::string outdoorAirControlType);

  bool setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate);

  void autosizeMinimumOutdoorAirFlowRate();

  bool setMinimumOutdoorAirSchedule(Schedule& schedule);

  bool setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate);

  void autosizeMaximumOutdoorAirFlowRate();

  bool setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule);

  bool setSupplyAirFan(const HVACComponent& supplyAirFan);

  bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

  void resetSupplyAirFanOperatingModeSchedule();

  bool setHeatingCoil(const HVACComponent& heatingCoil);

  void resetHeatingCoil();

  bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

  bool setCoolingCoil(const HVACComponent& coolingCoil);

  void resetCoolingCoil();

  bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

  // bool setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists);

  // void resetAvailabilityManagerList();

  // bool setDesignSpecificationZoneHVACSizingObject(const ModelObject& designSpecificationZoneHVACSizingName);

  // void resetDesignSpecificationZoneHVACSizingObject();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedMaximumSupplyAirFlowRate() const ;

  boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const ;

  boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::ZoneHVACUnitVentilator_Impl ImplType;

  explicit ZoneHVACUnitVentilator(std::shared_ptr<detail::ZoneHVACUnitVentilator_Impl> impl);

  friend class detail::ZoneHVACUnitVentilator_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneHVACUnitVentilator");
};

/** \relates ZoneHVACUnitVentilator*/
typedef boost::optional<ZoneHVACUnitVentilator> OptionalZoneHVACUnitVentilator;

/** \relates ZoneHVACUnitVentilator*/
typedef std::vector<ZoneHVACUnitVentilator> ZoneHVACUnitVentilatorVector;

} // model
} // openstudio

#endif // MODEL_ZONEHVACUNITVENTILATOR_HPP

