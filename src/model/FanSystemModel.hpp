/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_FANSYSTEMMODEL_HPP
#define MODEL_FANSYSTEMMODEL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class UnivariateFunctions;
class ThermalZone;

namespace detail {

  class FanSystemModel_Impl;

} // detail

/** FanSystemModel is a StraightComponent that wraps the OpenStudio IDD object 'OS:Fan:SystemModel'. */
class MODEL_API FanSystemModel : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FanSystemModel(const Model& model);

  virtual ~FanSystemModel() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> speedControlMethodValues();

  static std::vector<std::string> designPowerSizingMethodValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> airOutletNode() const;

  boost::optional<double> designMaximumAirFlowRate() const;

  bool isDesignMaximumAirFlowRateAutosized() const;

  boost::optional <double> autosizedDesignMaximumAirFlowRate();

  std::string speedControlMethod() const;

  double electricPowerMinimumFlowRateFraction() const;

  double designPressureRise() const;

  double motorEfficiency() const;

  double motorInAirStreamFraction() const;

  boost::optional<double> designElectricPowerConsumption() const;

  bool isDesignElectricPowerConsumptionAutosized() const;

  boost::optional <double> autosizedDesignElectricPowerConsumption();

  std::string designPowerSizingMethod() const;

  boost::optional<double> electricPowerPerUnitFlowRate() const;

  boost::optional<double> electricPowerPerUnitFlowRatePerUnitPressure() const;

  double fanTotalEfficiency() const;

  // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
  boost::optional<UnivariateFunctions> electricPowerFunctionofFlowFractionCurve() const;

  boost::optional<double> nightVentilationModePressureRise() const;

  boost::optional<double> nightVentilationModeFlowFraction() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> motorLossZone() const;

  boost::optional<double> motorLossRadiativeFraction() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  boost::optional<int> numberofSpeeds() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
  bool setAvailabilitySchedule(Schedule& schedule);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirInletNode(const Connection& connection);

  void resetAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirOutletNode(const Connection& connection);

  void resetAirOutletNode();

  bool setDesignMaximumAirFlowRate(double designMaximumAirFlowRate);

  void autosizeDesignMaximumAirFlowRate();

  bool setSpeedControlMethod(const std::string& speedControlMethod);

  bool setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction);

  bool setDesignPressureRise(double designPressureRise);

  bool setMotorEfficiency(double motorEfficiency);

  bool setMotorInAirStreamFraction(double motorInAirStreamFraction);

  bool setDesignElectricPowerConsumption(double designElectricPowerConsumption);

  void autosizeDesignElectricPowerConsumption();

  bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

  bool setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate);

  void resetElectricPowerPerUnitFlowRate();

  bool setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure);

  void resetElectricPowerPerUnitFlowRatePerUnitPressure();

  bool setFanTotalEfficiency(double fanTotalEfficiency);

  // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
  bool setElectricPowerFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions);

  void resetElectricPowerFunctionofFlowFractionCurve();

  bool setNightVentilationModePressureRise(double nightVentilationModePressureRise);

  void resetNightVentilationModePressureRise();

  bool setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction);

  void resetNightVentilationModeFlowFraction();

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setMotorLossZone(const ThermalZone& thermalZone);

  void resetMotorLossZone();

  bool setMotorLossRadiativeFraction(double motorLossRadiativeFraction);

  void resetMotorLossRadiativeFraction();

  bool setEndUseSubcategory(const std::string& endUseSubcategory);

  void resetEndUseSubcategory();

  bool setNumberofSpeeds(int numberofSpeeds);

  void resetNumberofSpeeds();

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::FanSystemModel_Impl ImplType;

  explicit FanSystemModel(std::shared_ptr<detail::FanSystemModel_Impl> impl);

  friend class detail::FanSystemModel_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FanSystemModel");
};

/** \relates FanSystemModel*/
typedef boost::optional<FanSystemModel> OptionalFanSystemModel;

/** \relates FanSystemModel*/
typedef std::vector<FanSystemModel> FanSystemModelVector;

} // model
} // openstudio

#endif // MODEL_FANSYSTEMMODEL_HPP

