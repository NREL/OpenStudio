/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP

#include <model/ModelAPI.hpp>
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class UnivariateFunctions;

namespace detail {

  class CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;

} // detail

/** CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl is a HVACComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableRefrigerantFlow:FluidTemperatureControl'. */
class MODEL_API CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl : public HVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(const Model& model);

  virtual ~CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl() = default;

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  Schedule availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> coilAirInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> coilAirOutletNode() const;

  boost::optional<double> ratedTotalCoolingCapacity() const;

  bool isRatedTotalCoolingCapacityAutosized() const;

  boost::optional <double> autosizedRatedTotalCoolingCapacity();

  boost::optional<double> ratedSensibleHeatRatio() const;

  bool isRatedSensibleHeatRatioAutosized() const;

  boost::optional <double> autosizedRatedSensibleHeatRatio();

  double indoorUnitReferenceSuperheating() const;

  // TODO: Check return type. From object lists, some candidates are: UnivariateFunctions.
  UnivariateFunctions indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
  bool setAvailabilitySchedule(Schedule& schedule);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setCoilAirInletNode(const Connection& connection);

  void resetCoilAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setCoilAirOutletNode(const Connection& connection);

  void resetCoilAirOutletNode();

  bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

  void autosizeRatedTotalCoolingCapacity();

  bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

  void autosizeRatedSensibleHeatRatio();

  bool setIndoorUnitReferenceSuperheating(double indoorUnitReferenceSuperheating);

  // TODO: Check argument type. From object lists, some candidates are: UnivariateFunctions.
  bool setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const UnivariateFunctions& univariateFunctions);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  using ImplType = detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;

  explicit CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(std::shared_ptr<detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl> impl);

  friend class detail::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl");
};

/** \relates CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl*/
using OptionalCoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl = boost::optional<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>;

/** \relates CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl*/
using CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControlVector = std::vector<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP

