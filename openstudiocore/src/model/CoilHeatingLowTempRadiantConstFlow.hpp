/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"


namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  class CoilHeatingLowTempRadiantConstFlow_Impl;

} // detail

/** CoilHeatingLowTempRadiantConstFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:LowTemperatureRadiant:ConstantFlow'.
   The object has following four schedules: heating high and low water temperature schedules, heating high and low control temperature schedules. These schedules
   define the high and low limits of a zone radiant system's desired inlet/supply heating water temperatures and the high and low limits of a zone's heating control temperature setpoints.
   If the space radiant heating control temperature drops below the heating control temperature setpoint low limit, the inlet heating water temperature is increased to its high limit. If the space
   radiant heating control temperature exceeds its heating control temperature high limit, the radiant heating loop is shut off. If the space radiant heating control temperature varies between
   the heating control temperature high and low limits, the desired inlet heating water temperature is linearly interpolated between the high and low heating water temperature limits. The desired water
   temperature is achieved by the zone 3-way valve for mixing/bypassing controls.
  */

class MODEL_API CoilHeatingLowTempRadiantConstFlow : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  CoilHeatingLowTempRadiantConstFlow(const Model& model,
                                     Schedule& heatingHighWaterTemperatureSchedule,
                                     Schedule& heatingLowWaterTemperatureSchedule,
                                     Schedule& heatingHighControlTemperatureSchedule,
                                     Schedule& heatingLowControlTemperatureSchedule);

  virtual ~CoilHeatingLowTempRadiantConstFlow() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{


  boost::optional<Schedule> heatingHighWaterTemperatureSchedule() const;

  boost::optional<Schedule> heatingLowWaterTemperatureSchedule() const;

  boost::optional<Schedule> heatingHighControlTemperatureSchedule() const;

  boost::optional<Schedule> heatingLowControlTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{


  bool setHeatingHighWaterTemperatureSchedule(Schedule& schedule);

  void resetHeatingHighWaterTemperatureSchedule();

  bool setHeatingLowWaterTemperatureSchedule(Schedule& schedule);

  void resetHeatingLowWaterTemperatureSchedule();

  bool setHeatingHighControlTemperatureSchedule(Schedule& schedule);

  void resetHeatingHighControlTemperatureSchedule();

  bool setHeatingLowControlTemperatureSchedule(Schedule& schedule);

  void resetHeatingLowControlTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingLowTempRadiantConstFlow_Impl ImplType;

  explicit CoilHeatingLowTempRadiantConstFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantConstFlow_Impl> impl);

  friend class detail::CoilHeatingLowTempRadiantConstFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantConstFlow");
};

/** \relates CoilHeatingLowTempRadiantConstFlow*/
typedef boost::optional<CoilHeatingLowTempRadiantConstFlow> OptionalCoilHeatingLowTempRadiantConstFlow;

/** \relates CoilHeatingLowTempRadiantConstFlow*/
typedef std::vector<CoilHeatingLowTempRadiantConstFlow> CoilHeatingLowTempRadiantConstFlowVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_HPP

