/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_COILHEATINGDXVARIABLESPEED_HPP
#define MODEL_COILHEATINGDXVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Curve;
class CoilHeatingDXVariableSpeedSpeedData;

namespace detail {

  class CoilHeatingDXVariableSpeed_Impl;

} // detail

/** CoilHeatingDXVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:VariableSpeed'. */
class MODEL_API CoilHeatingDXVariableSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXVariableSpeed(const Model& model);

  explicit CoilHeatingDXVariableSpeed(const Model& model, const Curve& partLoadFraction);

  virtual ~CoilHeatingDXVariableSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> defrostStrategyValues();

  static std::vector<std::string> defrostControlValues();

  /** @name Getters */
  //@{

  int nominalSpeedLevel() const;

  boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel() const;

  bool isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const;

  boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

  bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

  Curve energyPartLoadFractionCurve() const;

  boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

  double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

  boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

  double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

  double crankcaseHeaterCapacity() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  std::string defrostStrategy() const;

  std::string defrostControl() const;

  double defrostTimePeriodFraction() const;

  boost::optional<double> resistiveDefrostHeaterCapacity() const;

  bool isResistiveDefrostHeaterCapacityAutosized() const;

  //@}
  /** @name Setters */
  //@{

  void setNominalSpeedLevel(int nominalSpeedLevel);

  void setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel);

  void autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();

  void setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

  void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

  bool setEnergyPartLoadFractionCurve(const Curve& curve);

  bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

  bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

  void setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor);

  void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

  bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  bool setDefrostStrategy(std::string defrostStrategy);

  bool setDefrostControl(std::string defrostControl);

  bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

  bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

  void autosizeResistiveDefrostHeaterCapacity();

  //@}
  /** @name Other */
  //@{

  std::vector<CoilHeatingDXVariableSpeedSpeedData> speeds() const;

  bool addSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

  void removeSpeed(const CoilHeatingDXVariableSpeedSpeedData& speed);

  void removeAllSpeeds();

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingDXVariableSpeed_Impl ImplType;

  explicit CoilHeatingDXVariableSpeed(std::shared_ptr<detail::CoilHeatingDXVariableSpeed_Impl> impl);

  friend class detail::CoilHeatingDXVariableSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeed");
};

/** \relates CoilHeatingDXVariableSpeed*/
typedef boost::optional<CoilHeatingDXVariableSpeed> OptionalCoilHeatingDXVariableSpeed;

/** \relates CoilHeatingDXVariableSpeed*/
typedef std::vector<CoilHeatingDXVariableSpeed> CoilHeatingDXVariableSpeedVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXVARIABLESPEED_HPP

