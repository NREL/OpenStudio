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

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"

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

  bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

  void resetCondensationControlDewpointOffset();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedMaximumColdWaterFlow() const ;



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

  // Used to find the ZoneHVAC that contains this coil
  boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;

};

/** \relates CoilCoolingLowTempRadiantVarFlow*/
typedef boost::optional<CoilCoolingLowTempRadiantVarFlow> OptionalCoilCoolingLowTempRadiantVarFlow;

/** \relates CoilCoolingLowTempRadiantVarFlow*/
typedef std::vector<CoilCoolingLowTempRadiantVarFlow> CoilCoolingLowTempRadiantVarFlowVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP
