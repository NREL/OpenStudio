/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

