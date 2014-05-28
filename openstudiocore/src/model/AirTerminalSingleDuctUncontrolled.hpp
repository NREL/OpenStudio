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

#ifndef MODEL_AIRTERMINALSINGLEDUCTUNCONTROLLED_HPP
#define MODEL_AIRTERMINALSINGLEDUCTUNCONTROLLED_HPP

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "ModelAPI.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class AirTerminalSingleDuctUncontrolled_Impl;

} // detail

/** AirTerminalSingleDuctUncontrolled is an interface to the EnergyPlus IDD object
 *  named "AirTerminal:SingleDuct:Uncontrolled"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  AirTerminal:SingleDuct:Uncontrolled objects in energyplus.  
 */
class MODEL_API AirTerminalSingleDuctUncontrolled : public StraightComponent {

 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new AirTerminalSingleDuctUncontrolled object and places it inside the 
   *  model.  The object is fully initialized with all companion objects. */
  AirTerminalSingleDuctUncontrolled(const Model& model, Schedule & availabilitySchedule);

  virtual ~AirTerminalSingleDuctUncontrolled() {}

  //@}

  static IddObjectType iddObjectType();

  /** Returns the Schedule referred to by the AvailabilityScheduleName field. **/
  Schedule availabilitySchedule() const;

  /** Sets the Schedule referred to by the AvailabilityScheduleName field. **/
  bool setAvailabilitySchedule(Schedule& schedule);


  boost::optional<double> maximumAirFlowRate() const;

  OSOptionalQuantity getMaximumAirFlowRate(bool returnIP=false) const;

  bool isMaximumAirFlowRateAutosized() const;

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  bool setMaximumAirFlowRate(const Quantity& maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  //@}
 protected:
  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @cond 
  typedef detail::AirTerminalSingleDuctUncontrolled_Impl ImplType;

  explicit AirTerminalSingleDuctUncontrolled(std::shared_ptr<detail::AirTerminalSingleDuctUncontrolled_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctUncontrolled");
  /// @endcond 
};

typedef boost::optional<AirTerminalSingleDuctUncontrolled> OptionalAirTerminalSingleDuctUncontrolled;

typedef std::vector<AirTerminalSingleDuctUncontrolled> AirTerminalSingleDuctUncontrolledVector;

} // model

} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTUNCONTROLLED_HPP

