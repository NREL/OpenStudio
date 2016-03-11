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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilWaterHeatingAirToWaterHeatPump_Impl;

} // detail

/** CoilWaterHeatingAirToWaterHeatPump is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump'. 
 *  As of EnergyPlus version 8.4.0 this object maps to Coil:WaterHeating:AirToWaterHeatPump:Pumped in idf format.
 */
class MODEL_API CoilWaterHeatingAirToWaterHeatPump : public HVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilWaterHeatingAirToWaterHeatPump(const Model& model);

  explicit CoilWaterHeatingAirToWaterHeatPump(const Model& model,
    Curve & heatingCapacityFunctionofTemperatureCurve,
    Curve & heatingCapacityFunctionofAirFlowFractionCurve,
    Curve & heatingCapacityFunctionofWaterFlowFractionCurve,
    Curve & heatingCOPFunctionofTemperatureCurve,
    Curve & heatingCOPFunctionofAirFlowFractionCurve,
    Curve & heatingCOPFunctionofWaterFlowFractionCurve,
    Curve & partLoadFractionCorrelationCurve);

  virtual ~CoilWaterHeatingAirToWaterHeatPump() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> evaporatorAirTemperatureTypeforCurveObjectsValues();

  /** @name Getters */
  //@{

  double ratedHeatingCapacity() const;

  double ratedCOP() const;

  double ratedSensibleHeatRatio() const;

  double ratedEvaporatorInletAirDryBulbTemperature() const;

  double ratedEvaporatorInletAirWetBulbTemperature() const;

  double ratedCondenserInletWaterTemperature() const;

  boost::optional<double> ratedEvaporatorAirFlowRate() const;

  bool isRatedEvaporatorAirFlowRateAutosized() const;

  boost::optional<double> ratedCondenserWaterFlowRate() const;

  bool isRatedCondenserWaterFlowRateAutosized() const;

  bool evaporatorFanPowerIncludedinRatedCOP() const;

  bool condenserPumpPowerIncludedinRatedCOP() const;

  bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

  double condenserWaterPumpPower() const;

  double fractionofCondenserPumpHeattoWater() const;

  double crankcaseHeaterCapacity() const;

  double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

  std::string evaporatorAirTemperatureTypeforCurveObjects() const;

  Curve heatingCapacityFunctionofTemperatureCurve() const;

  Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

  Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

  Curve heatingCOPFunctionofTemperatureCurve() const;

  Curve heatingCOPFunctionofAirFlowFractionCurve() const;

  Curve heatingCOPFunctionofWaterFlowFractionCurve() const;

  Curve partLoadFractionCorrelationCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedHeatingCapacity(double ratedHeatingCapacity);

  bool setRatedCOP(double ratedCOP);

  bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

  bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

  bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

  bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

  bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

  void autosizeRatedEvaporatorAirFlowRate();

  bool setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate);

  void autosizeRatedCondenserWaterFlowRate();

  void setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

  void setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP);

  void setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

  bool setCondenserWaterPumpPower(double condenserWaterPumpPower);

  bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

  bool setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects);

  bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

  bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

  bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

  bool setPartLoadFractionCorrelationCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilWaterHeatingAirToWaterHeatPump_Impl ImplType;

  explicit CoilWaterHeatingAirToWaterHeatPump(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPump_Impl> impl);

  friend class detail::CoilWaterHeatingAirToWaterHeatPump_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPump");
};

/** \relates CoilWaterHeatingAirToWaterHeatPump*/
typedef boost::optional<CoilWaterHeatingAirToWaterHeatPump> OptionalCoilWaterHeatingAirToWaterHeatPump;

/** \relates CoilWaterHeatingAirToWaterHeatPump*/
typedef std::vector<CoilWaterHeatingAirToWaterHeatPump> CoilWaterHeatingAirToWaterHeatPumpVector;

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP

