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

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class CoilCoolingLowTempRadiantVarFlow_Impl;

} // detail

/** CoilCoolingLowTempRadiantVarFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:LowTemperatureRadiant:VariableFlow'. */
class MODEL_API CoilCoolingLowTempRadiantVarFlow : public StraightComponent {
 public:
 
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingLowTempRadiantVarFlow(const Model& model,
                                            Schedule& coolingControlTemperatureSchedule);

  virtual ~CoilCoolingLowTempRadiantVarFlow() {}

  //@}

  static IddObjectType iddObjectType();
  
  //unsigned inletPort();

  //unsigned outletPort();

  static std::vector<std::string> condensationControlTypeValues();

  /** @name Getters */
  //@{

  boost::optional<double> maximumColdWaterFlow() const;

  bool isMaximumColdWaterFlowDefaulted() const;

  bool isMaximumColdWaterFlowAutosized() const;

  double coolingControlThrottlingRange() const;

  bool isCoolingControlThrottlingRangeDefaulted() const;

  boost::optional<Schedule> coolingControlTemperatureSchedule() const;

  std::string condensationControlType() const;

  bool isCondensationControlTypeDefaulted() const;

  double condensationControlDewpointOffset() const;

  bool isCondensationControlDewpointOffsetDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setMaximumColdWaterFlow(double maximumColdWaterFlow);

  void resetMaximumColdWaterFlow();

  void autosizeMaximumColdWaterFlow();

  bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

  void resetCoolingControlThrottlingRange();

  bool setCoolingControlTemperatureSchedule(Schedule& schedule);

  void resetCoolingControlTemperatureSchedule();

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
  typedef detail::CoilCoolingLowTempRadiantVarFlow_Impl ImplType;

  explicit CoilCoolingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilCoolingLowTempRadiantVarFlow_Impl> impl);

  friend class detail::CoilCoolingLowTempRadiantVarFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantVarFlow");
};

/** \relates CoilCoolingLowTempRadiantVarFlow*/
typedef boost::optional<CoilCoolingLowTempRadiantVarFlow> OptionalCoilCoolingLowTempRadiantVarFlow;

/** \relates CoilCoolingLowTempRadiantVarFlow*/
typedef std::vector<CoilCoolingLowTempRadiantVarFlow> CoilCoolingLowTempRadiantVarFlowVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP

