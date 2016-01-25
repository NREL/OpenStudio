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

#ifndef MODEL_COILHEATINGDXMULTISPEED_HPP
#define MODEL_COILHEATINGDXMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;
class CoilHeatingDXMultiSpeedStageData;

namespace detail {

  class CoilHeatingDXMultiSpeed_Impl;

} // detail

/** CoilHeatingDXMultiSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:MultiSpeed'. */
class MODEL_API CoilHeatingDXMultiSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXMultiSpeed(const Model& model);

  virtual ~CoilHeatingDXMultiSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> defrostStrategyValues();

  static std::vector<std::string> defrostControlValues();

  static std::vector<std::string> fuelTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

  boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

  double crankcaseHeaterCapacity() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

  double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

  std::string defrostStrategy() const;

  std::string defrostControl() const;

  double defrostTimePeriodFraction() const;

  boost::optional<double> resistiveDefrostHeaterCapacity() const;

  bool isResistiveDefrostHeaterCapacityAutosized() const;

  bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

  std::string fuelType() const;

  int regionnumberforCalculatingHSPF() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  void setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

  void setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor);

  void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

  bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

  bool setDefrostStrategy(std::string defrostStrategy);

  bool setDefrostControl(std::string defrostControl);

  bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

  bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

  void autosizeResistiveDefrostHeaterCapacity();

  void setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

  bool setFuelType(std::string fuelType);

  bool setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF);

  //@}
  /** @name Other */
  //@{

  /** Return the performance data for each stage. **/
  std::vector<CoilHeatingDXMultiSpeedStageData> stages() const;

  /** Add a new stage after all of the existing stages **/
  bool addStage(const CoilHeatingDXMultiSpeedStageData& stage);

  /** Remove a stage **/
  void removeStage(const CoilHeatingDXMultiSpeedStageData& stage);

  /** Remove all stages **/
  void removeAllStages();

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingDXMultiSpeed_Impl ImplType;

  explicit CoilHeatingDXMultiSpeed(std::shared_ptr<detail::CoilHeatingDXMultiSpeed_Impl> impl);

  friend class detail::CoilHeatingDXMultiSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeed");
};

/** \relates CoilHeatingDXMultiSpeed*/
typedef boost::optional<CoilHeatingDXMultiSpeed> OptionalCoilHeatingDXMultiSpeed;

/** \relates CoilHeatingDXMultiSpeed*/
typedef std::vector<CoilHeatingDXMultiSpeed> CoilHeatingDXMultiSpeedVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXMULTISPEED_HPP

