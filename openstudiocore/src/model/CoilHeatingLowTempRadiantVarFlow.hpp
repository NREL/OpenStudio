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

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class CoilHeatingLowTempRadiantVarFlow_Impl;

} // detail

/** CoilHeatingLowTempRadiantVarFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:LowTemperatureRadiant:VariableFlow'. */
class MODEL_API CoilHeatingLowTempRadiantVarFlow : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingLowTempRadiantVarFlow(const Model& model,
                                            Schedule& heatingControlTemperature);

  virtual ~CoilHeatingLowTempRadiantVarFlow() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> maximumHotWaterFlow() const;

  //OSOptionalQuantity getMaximumHotWaterFlow(bool returnIP=false) const;

  bool isMaximumHotWaterFlowDefaulted() const;

  bool isMaximumHotWaterFlowAutosized() const;

  double heatingControlThrottlingRange() const;

  //Quantity getHeatingControlThrottlingRange(bool returnIP=false) const;

  bool isHeatingControlThrottlingRangeDefaulted() const;

  boost::optional<Schedule> heatingControlTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setMaximumHotWaterFlow(double maximumHotWaterFlow);

  //bool setMaximumHotWaterFlow(const Quantity& maximumHotWaterFlow);

  void resetMaximumHotWaterFlow();

  void autosizeMaximumHotWaterFlow();

  bool setHeatingControlThrottlingRange(double heatingControlThrottlingRange);

  //bool setHeatingControlThrottlingRange(const Quantity& heatingControlThrottlingRange);

  void resetHeatingControlThrottlingRange();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setHeatingControlTemperatureSchedule(Schedule& schedule);

  void resetHeatingControlTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingLowTempRadiantVarFlow_Impl ImplType;

  explicit CoilHeatingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantVarFlow_Impl> impl);

  friend class detail::CoilHeatingLowTempRadiantVarFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantVarFlow");
};

/** \relates CoilHeatingLowTempRadiantVarFlow*/
typedef boost::optional<CoilHeatingLowTempRadiantVarFlow> OptionalCoilHeatingLowTempRadiantVarFlow;

/** \relates CoilHeatingLowTempRadiantVarFlow*/
typedef std::vector<CoilHeatingLowTempRadiantVarFlow> CoilHeatingLowTempRadiantVarFlowVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP

