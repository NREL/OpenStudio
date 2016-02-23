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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl;

} // detail

/** CoilWaterHeatingAirToWaterHeatPumpWrapped is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump:Wrapped'. */
class MODEL_API CoilWaterHeatingAirToWaterHeatPumpWrapped : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilWaterHeatingAirToWaterHeatPumpWrapped(const Model& model);

  virtual ~CoilWaterHeatingAirToWaterHeatPumpWrapped() {}

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

  double ratedCondenserWaterTemperature() const;

  boost::optional<double> ratedEvaporatorAirFlowRate() const;

  bool isRatedEvaporatorAirFlowRateAutocalculated() const;

  bool evaporatorFanPowerIncludedinRatedCOP() const;

  double crankcaseHeaterCapacity() const;

  double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

  std::string evaporatorAirTemperatureTypeforCurveObjects() const;

  Curve heatingCapacityFunctionofTemperatureCurve() const;

  Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

  Curve heatingCOPFunctionofTemperatureCurve() const;

  Curve heatingCOPFunctionofAirFlowFractionCurve() const;

  Curve partLoadFractionCorrelationCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedHeatingCapacity(double ratedHeatingCapacity);

  bool setRatedCOP(double ratedCOP);

  bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

  bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

  bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

  bool setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature);

  bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

  void autocalculateRatedEvaporatorAirFlowRate();

  void setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

  bool setEvaporatorAirTemperatureTypeforCurveObjects(std::string evaporatorAirTemperatureTypeforCurveObjects);

  bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

  bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setPartLoadFractionCorrelationCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl ImplType;

  explicit CoilWaterHeatingAirToWaterHeatPumpWrapped(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl> impl);

  friend class detail::CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpWrapped");
};

/** \relates CoilWaterHeatingAirToWaterHeatPumpWrapped*/
typedef boost::optional<CoilWaterHeatingAirToWaterHeatPumpWrapped> OptionalCoilWaterHeatingAirToWaterHeatPumpWrapped;

/** \relates CoilWaterHeatingAirToWaterHeatPumpWrapped*/
typedef std::vector<CoilWaterHeatingAirToWaterHeatPumpWrapped> CoilWaterHeatingAirToWaterHeatPumpWrappedVector;

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_HPP

