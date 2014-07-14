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

#ifndef MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class Schedule;
class DesignSpecificationOutdoorAir;

namespace detail {

  class AirTerminalSingleDuctVAVNoReheat_Impl;

} // detail

/** AirTerminalSingleDuctVAVNoReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:VAV:NoReheat'. */
class MODEL_API AirTerminalSingleDuctVAVNoReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{


  explicit AirTerminalSingleDuctVAVNoReheat(const Model& model, Schedule& schedule);

  virtual ~AirTerminalSingleDuctVAVNoReheat() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> zoneMinimumAirFlowInputMethodValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule availabilitySchedule() const;

  boost::optional<double> maximumAirFlowRate() const;

  bool isMaximumAirFlowRateAutosized() const;

  boost::optional<std::string> zoneMinimumAirFlowInputMethod() const;

  boost::optional<double> constantMinimumAirFlowFraction() const;

  bool isConstantMinimumAirFlowFractionDefaulted() const;

  boost::optional<double> fixedMinimumAirFlowRate() const;

  bool isFixedMinimumAirFlowRateDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> minimumAirFlowFractionSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
  boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAirObject() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  void resetMaximumAirFlowRate();

  void autosizeMaximumAirFlowRate();

  bool setZoneMinimumAirFlowInputMethod(std::string zoneMinimumAirFlowInputMethod);

  void resetZoneMinimumAirFlowInputMethod();

  bool setConstantMinimumAirFlowFraction(double constantMinimumAirFlowFraction);

  void resetConstantMinimumAirFlowFraction();

  bool setFixedMinimumAirFlowRate(double fixedMinimumAirFlowRate);

  void resetFixedMinimumAirFlowRate();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setMinimumAirFlowFractionSchedule(Schedule& schedule);

  void resetMinimumAirFlowFractionSchedule();

  // TODO: Check argument type. From object lists, some candidates are: DesignSpecificationOutdoorAir.
  bool setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir);

  void resetDesignSpecificationOutdoorAirObject();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctVAVNoReheat_Impl ImplType;

  explicit AirTerminalSingleDuctVAVNoReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVNoReheat_Impl> impl);

  friend class detail::AirTerminalSingleDuctVAVNoReheat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctVAVNoReheat");
};

/** \relates AirTerminalSingleDuctVAVNoReheat*/
typedef boost::optional<AirTerminalSingleDuctVAVNoReheat> OptionalAirTerminalSingleDuctVAVNoReheat;

/** \relates AirTerminalSingleDuctVAVNoReheat*/
typedef std::vector<AirTerminalSingleDuctVAVNoReheat> AirTerminalSingleDuctVAVNoReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTVAVNOREHEAT_HPP

