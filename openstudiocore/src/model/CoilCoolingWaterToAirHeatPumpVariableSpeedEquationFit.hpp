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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

class Curve;
class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;

namespace detail {

  class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;

} // detail

/** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit'. */
class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit : public WaterToAirComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model);

  explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model, const Curve& partLoadFraction);

  virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  int nominalSpeedLevel() const;

  boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

  bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

  boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

  bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

  boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

  bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

  double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

  double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

  bool useHotGasReheat() const;

  Curve energyPartLoadFractionCurve() const;

  //@}
  /** @name Setters */
  //@{

  void setNominalSpeedLevel(int nominalSpeedLevel);

  void setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

  void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

  void setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

  void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

  void setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel);

  void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

  bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

  bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

  void setUseHotGasReheat(bool useHotGasReheat);

  bool setEnergyPartLoadFractionCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

  bool addSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

  void removeSpeed(const CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

  void removeAllSpeeds();

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl ImplType;

  explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl);

  friend class detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit");
};

/** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit*/
typedef boost::optional<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit> OptionalCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit;

/** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit*/
typedef std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP

