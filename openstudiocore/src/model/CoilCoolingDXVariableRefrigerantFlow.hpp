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

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;

namespace detail {

  class CoilCoolingDXVariableRefrigerantFlow_Impl;

} // detail

/** CoilCoolingDXVariableRefrigerantFlow is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableRefrigerantFlow'. */
class MODEL_API CoilCoolingDXVariableRefrigerantFlow : public HVACComponent {

 public:

  explicit CoilCoolingDXVariableRefrigerantFlow(const Model& model);

  virtual ~CoilCoolingDXVariableRefrigerantFlow() {}

  static IddObjectType iddObjectType();

  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);

  boost::optional<double> ratedTotalCoolingCapacity() const;

  bool isRatedTotalCoolingCapacityAutosized() const;

  bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

  void autosizeRatedTotalCoolingCapacity();

  boost::optional<double> ratedSensibleHeatRatio() const;

  bool isRatedSensibleHeatRatioAutosized() const;

  bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

  void autosizeRatedSensibleHeatRatio();

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  Curve coolingCapacityRatioModifierFunctionofTemperatureCurve() const;

  bool setCoolingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& lcurve);

  Curve coolingCapacityModifierCurveFunctionofFlowFraction() const;

  bool setCoolingCapacityModifierCurveFunctionofFlowFraction(const Curve& lcurve);

 protected:

  /// @cond
  typedef detail::CoilCoolingDXVariableRefrigerantFlow_Impl ImplType;

  explicit CoilCoolingDXVariableRefrigerantFlow(std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlow_Impl> impl);

  friend class detail::CoilCoolingDXVariableRefrigerantFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlow");
};

/** \relates CoilCoolingDXVariableRefrigerantFlow*/
typedef boost::optional<CoilCoolingDXVariableRefrigerantFlow> OptionalCoilCoolingDXVariableRefrigerantFlow;

/** \relates CoilCoolingDXVariableRefrigerantFlow*/
typedef std::vector<CoilCoolingDXVariableRefrigerantFlow> CoilCoolingDXVariableRefrigerantFlowVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_HPP

