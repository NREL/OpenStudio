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

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

//class Quantity;
//class OSOptionalQuantity;

namespace model {

class Schedule;

namespace detail {

  class CoilCoolingLowTempRadiantConstFlow_Impl;

} // detail

/** CoilCoolingLowTempRadiantConstFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:LowTemperatureRadiant:ConstantFlow'. */
class MODEL_API CoilCoolingLowTempRadiantConstFlow : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  CoilCoolingLowTempRadiantConstFlow(const Model& model, 
                                     Schedule& coolingHighWaterTemperatureSchedule,
                                     Schedule& coolingLowWaterTemperatureSchedule,
                                     Schedule& coolingHighControlTemperatureSchedule,
                                     Schedule& coolingLowControlTemperatureSchedule);

  virtual ~CoilCoolingLowTempRadiantConstFlow() {}
 
  //virtual unsigned inletPort();

  //virtual unsigned outletPort();
  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> condensationControlTypeValues();



  /** @name Getters */
  //@{

   /** The following four schedules: cooling high and low water temperature schedules, cooling high and low control temperature schedules
   define the high and low limits of a zone radiant system's desired inlet/supply cooling water temperatures and the high and low limits of a zone's cooling control temperature setpoints. 
   If the space radiant cooling control temperature exceeds the cooling control temperature setpoint high limit, the inlet cooling water temperature is reduced to its low limit. If the space 
   radiant cooling control temperature drops below its cooling control temperature low limit, the radiant cooling loop is shut off. If the space radiant cooling control temperature varies between
   the cooling control temperature high and low limits, the desired inlet cooling water temperature is linearly interpolated between the high and low cooling water temperature limits. The desired water
   temperature is achieved by the zone 3-way valve for mixing/bypassing controls.  
  */

  boost::optional<Schedule> coolingHighWaterTemperatureSchedule() const;

  boost::optional<Schedule> coolingLowWaterTemperatureSchedule() const;

  boost::optional<Schedule> coolingHighControlTemperatureSchedule() const;

  boost::optional<Schedule> coolingLowControlTemperatureSchedule() const;

  std::string condensationControlType() const;

  bool isCondensationControlTypeDefaulted() const;

  double condensationControlDewpointOffset() const;

  bool isCondensationControlDewpointOffsetDefaulted() const;

  //@}
  /** @name Setters */
  //@{


  bool setCoolingHighWaterTemperatureSchedule(Schedule& schedule);

  void resetCoolingHighWaterTemperatureSchedule();

  bool setCoolingLowWaterTemperatureSchedule(Schedule& schedule);

  void resetCoolingLowWaterTemperatureSchedule();

  bool setCoolingHighControlTemperatureSchedule(Schedule& schedule);

  void resetCoolingHighControlTemperatureSchedule();

  bool setCoolingLowControlTemperatureSchedule(Schedule& schedule);

  void resetCoolingLowControlTemperatureSchedule();

  bool setCondensationControlType(std::string condensationControlType);

  void resetCondensationControlType();

  void setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

  void resetCondensationControlDewpointOffset();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingLowTempRadiantConstFlow_Impl ImplType;

  explicit CoilCoolingLowTempRadiantConstFlow(std::shared_ptr<detail::CoilCoolingLowTempRadiantConstFlow_Impl> impl);

  friend class detail::CoilCoolingLowTempRadiantConstFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantConstFlow");
};

/** \relates CoilCoolingLowTempRadiantConstFlow*/
typedef boost::optional<CoilCoolingLowTempRadiantConstFlow> OptionalCoilCoolingLowTempRadiantConstFlow;

/** \relates CoilCoolingLowTempRadiantConstFlow*/
typedef std::vector<CoilCoolingLowTempRadiantConstFlow> CoilCoolingLowTempRadiantConstFlowVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_HPP

