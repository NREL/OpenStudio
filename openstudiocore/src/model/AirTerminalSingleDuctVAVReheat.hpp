/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_HPP

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "ModelAPI.hpp"

namespace openstudio {

namespace model {

class Schedule;

class HVACComponent;

namespace detail {
  class AirTerminalSingleDuctVAVReheat_Impl;
} // detail

class MODEL_API AirTerminalSingleDuctVAVReheat : public StraightComponent {

  public:

  explicit AirTerminalSingleDuctVAVReheat( const Model& model, 
                                           Schedule & availabilitySchedule,
                                           HVACComponent & coil );

  virtual ~AirTerminalSingleDuctVAVReheat() {}

  static IddObjectType iddObjectType();

  /** Returns the reheat coil */
  HVACComponent reheatCoil() const;

  /** Sets the reheat coil.  Returns true if the supplied coil was accecpted */
  bool setReheatCoil(HVACComponent & coil);

  /** Returns the Schedule referred to by the AvailabilityScheduleName field. */
  Schedule availabilitySchedule() const;

  /** Sets the Schedule referred to by the AvailabilityScheduleName field. */
  bool setAvailabilitySchedule(Schedule& schedule);

  /** Returns the value of the MaximumAirFlowRate field. */
  boost::optional<double> maximumAirFlowRate() const ;

  /** Sets the value of the MaximumAirFlowRate field. */
  void setMaximumAirFlowRate( double value );

  /** Sets the value of the MaximumAirFlowRate field to AutoSize */
  void autosizeMaximumAirFlowRate();

  /** Returns true of the MaximumAirFlowRate field is set to AutoSize */
  bool isMaximumAirFlowRateAutosized() const;

  /** Returns the value of the MaximumAirFlowRate field. */
  std::string zoneMinimumAirFlowMethod();

  /** Sets the value of the MaximumAirFlowRate field.
   *  Options are FixedFlowRate and Scheduled.
   */
  void setZoneMinimumAirFlowMethod( std::string value );

  /** Returns the value of the ConstantMinimumAirFlowFraction field. */
  double constantMinimumAirFlowFraction();

  /** Sets the value of the ConstantMinimumAirFlowFraction field. */
  void setConstantMinimumAirFlowFraction( double value );

  /** Returns the value of the FixedMinimumAirFlowRate field. */
  double fixedMinimumAirFlowRate();

  /** Sets the value of the FixedMinimumAirFlowRate field. */
  void setFixedMinimumAirFlowRate( double value );

  /** Returns the Schedule referred to by the MinimumAirFlowFractionScheduleName field. */
  boost::optional<Schedule> minimumAirFlowFractionSchedule() const;

  /** Sets the Schedule referred to by the MinimumAirFlowFractionScheduleName field. */
  bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

  void resetMinimumAirFlowFractionSchedule();

  /** Returns the value of the MaximumHotWaterOrSteamFlowRate field. */
  boost::optional<double> maximumHotWaterOrSteamFlowRate();

  /** Sets the value of the MaximumHotWaterOrSteamFlowRate field. */
  void setMaximumHotWaterOrSteamFlowRate( double value );

  /** Sets the value of the MaximumHotWaterOrSteamFlowRate field to AutoSize */
  void autosizeMaximumHotWaterOrSteamFlowRate();

  /** Returns true of the MaximumHotWaterOrSteamFlowRate field is set to AutoSize */
  bool isMaximumHotWaterOrSteamFlowRateAutosized() const;

  /** Returns the value of the MinimumHotWaterOrSteamFlowRate field. */
  double minimumHotWaterOrSteamFlowRate();

  /** Sets the value of the MinimumHotWaterOrSteamFlowRate field. */
  void setMinimumHotWaterOrStreamFlowRate( double value );

  /** Returns the value of the ConvergenceTolerance field. */
  double convergenceTolerance();

  /** Sets the value of the ConvergenceTolerance field. */
  void setConvergenceTolerance( double value );

  /** Returns the value of the DamperHeatingAction field. */
  std::string damperHeatingAction();

  /** Sets the value of the DamperHeatingAction field.
   *  Options are Normal and Reverse.
   */
  void setDamperHeatingAction( std::string value );

  /** Returns the value of the MaximumFlowPerZoneFloorAreaDuringReheat field. */
  boost::optional<double> maximumFlowPerZoneFloorAreaDuringReheat();

  /** Sets the value of the MaximumFlowPerZoneFloorAreaDuringReheat field. */
  void setMaximumFlowPerZoneFloorAreaDuringReheat( double value );

  /** Sets the value of the MaximumFlowPerZoneFloorAreaDuringReheat field to AutoSize */
  void autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

  /** Returns true of the MaximumFlowPerZoneFloorAreaDuringReheat field is set to AutoSize */
  bool isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const;

  /** Resets the MaximumFlowPerZoneFloorAreaDuringReheat field to no value */
  void resetMaximumFlowPerZoneFloorAreaDuringReheat();

  /** Returns the value of the MaximumFlowFractionDuringReheat field. */
  boost::optional<double> maximumFlowFractionDuringReheat();

  /** Sets the value of the MaximumFlowFractionDuringReheat field. */
  void setMaximumFlowFractionDuringReheat( double value );  

  /** Sets the value of the MaximumFlowFractionDuringReheat field to AutoSize */
  void autosizeMaximumFlowFractionDuringReheat();

  /** Returns true of the MaximumFlowFractionDuringReheat field is set to AutoSize */
  bool isMaximumFlowFractionDuringReheatAutosized() const;

  /** Reset MaximumFlowFractionDuringReheat to no value */
  void resetMaximumFlowFractionDuringReheat();

  /** Returns the value of the MaximumReheatAirTemperature field. */
  double maximumReheatAirTemperature();

  /** Sets the value of the MaximumReheatAirTemperature field. */
  void setMaximumReheatAirTemperature( double value );

 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  /// @cond 

  typedef detail::AirTerminalSingleDuctVAVReheat_Impl ImplType;

  explicit AirTerminalSingleDuctVAVReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVReheat_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVReheat");

  /// @endcond 

};

typedef boost::optional<AirTerminalSingleDuctVAVReheat> OptionalAirTerminalSingleDuctVAVReheat;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVREHEAT_HPP

