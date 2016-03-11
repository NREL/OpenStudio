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

#ifndef MODEL_COILCOOLINGDXVARIABLESPEED_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Curve;
// class WaterStorageTank;
class Schedule;
class CoilCoolingDXVariableSpeedSpeedData;

namespace detail {

  class CoilCoolingDXVariableSpeed_Impl;

} // detail

/** CoilCoolingDXVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableSpeed'. */
class MODEL_API CoilCoolingDXVariableSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingDXVariableSpeed(const Model& model);

  explicit CoilCoolingDXVariableSpeed(const Model& model, const Curve& partLoadFraction);

  virtual ~CoilCoolingDXVariableSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> condenserTypeValues();

  /** @name Getters */
  //@{

  int nominalSpeedLevel() const;

  boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

  bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

  boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

  bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

  double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

  double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

  Curve energyPartLoadFractionCurve() const;

  // boost::optional<std::string> condenserAirInletNodeName() const;

  std::string condenserType() const;

  boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

  bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

  double crankcaseHeaterCapacity() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

  // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

  double basinHeaterCapacity() const;

  double basinHeaterSetpointTemperature() const;

  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  //@}
  /** @name Setters */
  //@{

  void setNominalSpeedLevel(int nominalSpeedLevel);

  void setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

  void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

  void setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

  void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

  bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

  bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

  bool setEnergyPartLoadFractionCurve(const Curve& curve);

  // void setCondenserAirInletNodeName(std::string condenserAirInletNodeName);

  // void resetCondenserAirInletNodeName();

  bool setCondenserType(std::string condenserType);

  bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

  void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  // bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

  // void resetSupplyWaterStorageTank();

  // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

  // void resetCondensateCollectionWaterStorageTank();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  bool setBasinHeaterOperatingSchedule(Schedule& schedule);

  void resetBasinHeaterOperatingSchedule();

  //@}
  /** @name Other */
  //@{

  std::vector<CoilCoolingDXVariableSpeedSpeedData> speeds() const;

  bool addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

  void removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

  void removeAllSpeeds();

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingDXVariableSpeed_Impl ImplType;

  explicit CoilCoolingDXVariableSpeed(std::shared_ptr<detail::CoilCoolingDXVariableSpeed_Impl> impl);

  friend class detail::CoilCoolingDXVariableSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeed");
};

/** \relates CoilCoolingDXVariableSpeed*/
typedef boost::optional<CoilCoolingDXVariableSpeed> OptionalCoilCoolingDXVariableSpeed;

/** \relates CoilCoolingDXVariableSpeed*/
typedef std::vector<CoilCoolingDXVariableSpeed> CoilCoolingDXVariableSpeedVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLESPEED_HPP

