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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

class Curve;
class CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;

namespace detail {

  class CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;

} // detail

/** CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:WaterToAirHeatPump:VariableSpeedEquationFit'. */
class MODEL_API CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit : public WaterToAirComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model);

  explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(const Model& model, const Curve& partLoadFraction);

  virtual ~CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  int nominalSpeedLevel() const;

  boost::optional<double> ratedHeatingCapacityAtSelectedNominalSpeedLevel() const;

  bool isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized() const;

  boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

  bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

  boost::optional<double> ratedWaterFlowRateAtSelectedNominalSpeedLevel() const;

  bool isRatedWaterFlowRateAtSelectedNominalSpeedLevelAutosized() const;

  Curve energyPartLoadFractionCurve() const;

  //@}
  /** @name Setters */
  //@{

  void setNominalSpeedLevel(int nominalSpeedLevel);

  void setRatedHeatingCapacityAtSelectedNominalSpeedLevel(double ratedHeatingCapacityAtSelectedNominalSpeedLevel);

  void autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();

  void setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

  void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

  void setRatedWaterFlowRateAtSelectedNominalSpeedLevel(double ratedWaterFlowRateAtSelectedNominalSpeedLevel);

  void autosizeRatedWaterFlowRateAtSelectedNominalSpeedLevel();

  bool setEnergyPartLoadFractionCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> speeds() const;

  bool addSpeed(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

  void removeSpeed(const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData& speed);

  void removeAllSpeeds();

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl ImplType;

  explicit CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(std::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl> impl);

  friend class detail::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit");
};

/** \relates CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit*/
typedef boost::optional<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit> OptionalCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit;

/** \relates CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit*/
typedef std::vector<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit> CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFIT_HPP

