/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

#include <model/Connection.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
/*class Connection;*/
class HVACComponent;

namespace detail {

  class AirTerminalSingleDuctConstantVolumeReheat_Impl;

} // detail

/** AirTerminalSingleDuctConstantVolumeReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:Reheat'. */
class MODEL_API AirTerminalSingleDuctConstantVolumeReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctConstantVolumeReheat(const Model& model);

  virtual ~AirTerminalSingleDuctConstantVolumeReheat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection airOutletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  Connection airInletNode() const;

  boost::optional<double> maximumAirFlowRate() const;

  OSOptionalQuantity getMaximumAirFlowRate(bool returnIP=false) const;

  bool isMaximumAirFlowRateAutosized() const;

  // TODO: Check return type. From object lists, some candidates are: HeatingCoilName.
  HVACComponent reheatCoil() const;

  boost::optional<double> maximumHotWaterorSteamFlowRate() const;

  OSOptionalQuantity getMaximumHotWaterorSteamFlowRate(bool returnIP=false) const;

  bool isMaximumHotWaterorSteamFlowRateAutosized() const;

  double minimumHotWaterorSteamFlowRate() const;

  Quantity getMinimumHotWaterorSteamFlowRate(bool returnIP=false) const;

  bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

  double convergenceTolerance() const;

  Quantity getConvergenceTolerance(bool returnIP=false) const;

  bool isConvergenceToleranceDefaulted() const;

  double maximumReheatAirTemperature() const;

  Quantity getMaximumReheatAirTemperature(bool returnIP=false) const;

  bool isMaximumReheatAirTemperatureDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirOutletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setAirInletNode(const Connection& connection);

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  bool setMaximumAirFlowRate(const Quantity& maximumAirFlowRate);

  void resetMaximumAirFlowRate();

  void autosizeMaximumAirFlowRate();

  // TODO: Check argument type. From object lists, some candidates are: HeatingCoilName.
  bool setReheatCoil(const HVACComponent& heatingCoilName);

  void resetReheatCoil();

  bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

  bool setMaximumHotWaterorSteamFlowRate(const Quantity& maximumHotWaterorSteamFlowRate);

  void resetMaximumHotWaterorSteamFlowRate();

  void autosizeMaximumHotWaterorSteamFlowRate();

  bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

  bool setMinimumHotWaterorSteamFlowRate(const Quantity& minimumHotWaterorSteamFlowRate);

  void resetMinimumHotWaterorSteamFlowRate();

  bool setConvergenceTolerance(double convergenceTolerance);

  bool setConvergenceTolerance(const Quantity& convergenceTolerance);

  void resetConvergenceTolerance();

  bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

  bool setMaximumReheatAirTemperature(const Quantity& maximumReheatAirTemperature);

  void resetMaximumReheatAirTemperature();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctConstantVolumeReheat_Impl ImplType;

  explicit AirTerminalSingleDuctConstantVolumeReheat(boost::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeReheat_Impl> impl);

  friend class detail::AirTerminalSingleDuctConstantVolumeReheat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeReheat");
};

/** \relates AirTerminalSingleDuctConstantVolumeReheat*/
typedef boost::optional<AirTerminalSingleDuctConstantVolumeReheat> OptionalAirTerminalSingleDuctConstantVolumeReheat;

/** \relates AirTerminalSingleDuctConstantVolumeReheat*/
typedef std::vector<AirTerminalSingleDuctConstantVolumeReheat> AirTerminalSingleDuctConstantVolumeReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_HPP

