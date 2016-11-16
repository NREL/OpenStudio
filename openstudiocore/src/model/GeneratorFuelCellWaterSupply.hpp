/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_GENERATORFUELCELLWATERSUPPLY_HPP
#define MODEL_GENERATORFUELCELLWATERSUPPLY_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class CurveQuadratic;
class CurveCubic;
class Connection;
class Schedule;

namespace detail {

  class GeneratorFuelCellWaterSupply_Impl;

} // detail

/** GeneratorFuelCellWaterSupply is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:WaterSupply'. */
class MODEL_API GeneratorFuelCellWaterSupply : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorFuelCellWaterSupply(const Model& model);

  virtual ~GeneratorFuelCellWaterSupply() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> waterTemperatureModelingModeValues();

  /** @name Getters */
  //@{

  CurveQuadratic reformerWaterFlowRateFunctionofFuelRateCurve() const;

  CurveCubic reformerWaterPumpPowerFunctionofFuelRateCurve() const;

  double pumpHeatLossFactor() const;

  std::string waterTemperatureModelingMode() const;

  boost::optional<Connection> waterTemperatureReferenceNode() const;

  boost::optional<Schedule> waterTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setReformerWaterFlowRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

  void resetReformerWaterFlowRateFunctionofFuelRateCurve();

  bool setReformerWaterPumpPowerFunctionofFuelRateCurve(const CurveCubic& cubicCurves);

  void resetReformerWaterPumpPowerFunctionofFuelRateCurve();

  void setPumpHeatLossFactor(double pumpHeatLossFactor);

  void resetPumpHeatLossFactor();

  bool setWaterTemperatureModelingMode(const std::string& waterTemperatureModelingMode);

  void resetWaterTemperatureModelingMode();

  bool setWaterTemperatureReferenceNode(const Connection& connection);

  void resetWaterTemperatureReferenceNode();

  // Note Schedules are passed by reference, not const reference.
  bool setWaterTemperatureSchedule(Schedule& schedule);

  void resetWaterTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorFuelCellWaterSupply_Impl ImplType;

  explicit GeneratorFuelCellWaterSupply(std::shared_ptr<detail::GeneratorFuelCellWaterSupply_Impl> impl);

  friend class detail::GeneratorFuelCellWaterSupply_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorFuelCellWaterSupply");
};

/** \relates GeneratorFuelCellWaterSupply*/
typedef boost::optional<GeneratorFuelCellWaterSupply> OptionalGeneratorFuelCellWaterSupply;

/** \relates GeneratorFuelCellWaterSupply*/
typedef std::vector<GeneratorFuelCellWaterSupply> GeneratorFuelCellWaterSupplyVector;

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELLWATERSUPPLY_HPP

