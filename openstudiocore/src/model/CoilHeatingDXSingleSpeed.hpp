/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGDXSINGLESPEED_HPP
#define MODEL_COILHEATINGDXSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

class Curve;

// class CurveBiquadratic;

// class CurveCubic

// class CurveQuadratic;

namespace detail {

class CoilHeatingDXSingleSpeed_Impl;

} // detail

/** CoilHeatingDXSingleSpeed is a StraightComponent that wraps the
 *  OpenStudio IDD object 'OS:Coil:Heating:DX:SingleSpeed'. */
class MODEL_API CoilHeatingDXSingleSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXSingleSpeed( const Model& model,
                                       Schedule & availabilitySchedule, 
                                       Curve& totalHeatingCapacityFunctionofTemperatureCurve,
                                       Curve& totalHeatingCapacityFunctionofFlowFractionCurve,
                                       Curve& energyInputRatioFunctionofTemperatureCurve,
                                       Curve& energyInputRatioFunctionofFlowFractionCurve,
                                       Curve& partLoadFractionCorrelationCurve
                                     );

  virtual ~CoilHeatingDXSingleSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validDefrostStrategyValues();

  static std::vector<std::string> validDefrostControlValues();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  boost::optional<double> ratedTotalHeatingCapacity() const;

  bool isRatedTotalHeatingCapacityAutosized() const;

  double ratedCOP() const;

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

  bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

  double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

  bool isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const;

  double crankcaseHeaterCapacity() const;

  bool isCrankcaseHeaterCapacityDefaulted() const;

  double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

  bool isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const;

  std::string defrostStrategy() const;

  bool isDefrostStrategyDefaulted() const;

  std::string defrostControl() const;

  bool isDefrostControlDefaulted() const;

  double defrostTimePeriodFraction() const;

  bool isDefrostTimePeriodFractionDefaulted() const;

  boost::optional<double> resistiveDefrostHeaterCapacity() const;

  bool isResistiveDefrostHeaterCapacityDefaulted() const;

  bool isResistiveDefrostHeaterCapacityAutosized() const;

  bool setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);

  void autosizeRatedTotalHeatingCapacity();

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  bool setRatedCOP(double ratedCOP);

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

  void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

  bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

  void resetMaximumOutdoorDryBulbTemperatureforDefrostOperation();

  bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

  void resetCrankcaseHeaterCapacity();

  bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

  void resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();

  bool setDefrostStrategy(std::string defrostStrategy);

  void resetDefrostStrategy();

  bool setDefrostControl(std::string defrostControl);

  void resetDefrostControl();

  bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

  void resetDefrostTimePeriodFraction();

  bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

  void resetResistiveDefrostHeaterCapacity();

  void autosizeResistiveDefrostHeaterCapacity();

  //@}
  /** @name Curves */
  //@{

  // A5 , \field Total Heating Capacity Function of Temperature Curve Name
  // \object-list BiquadraticQuadraticCubicCurves
  Curve totalHeatingCapacityFunctionofTemperatureCurve() const;
  bool setTotalHeatingCapacityFunctionofTemperatureCurve( const Curve& curve );

  // A6 , \field Total Heating Capacity Function of Flow Fraction Curve Name
  // \object-list QuadraticCubicCurves
  Curve totalHeatingCapacityFunctionofFlowFractionCurve() const;
  bool setTotalHeatingCapacityFunctionofFlowFractionCurve( const Curve& curve );  

  // A7 ,  \field Energy Input Ratio Function of Temperature Curve Name
  // \object-list BiquadraticQuadraticCubicCurves
  Curve energyInputRatioFunctionofTemperatureCurve() const;
  bool setEnergyInputRatioFunctionofTemperatureCurve( const Curve& curve );

  // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
  // \object-list QuadraticCubicCurves
  Curve energyInputRatioFunctionofFlowFractionCurve() const;
  bool setEnergyInputRatioFunctionofFlowFractionCurve( const Curve& curve );

  // A9 , \field Part Load Fraction Correlation Curve Name
  // \object-list QuadraticCubicCurves
  Curve partLoadFractionCorrelationCurve() const;
  bool setPartLoadFractionCorrelationCurve( const Curve& curve );  

  // A10, \field Defrost Energy Input Ratio Function of Temperature Curve Name
  // \object-list BiquadraticCurves
  // not a required curve so it needs a reset
  boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;
  bool setDefrostEnergyInputRatioFunctionofTemperatureCurve( const Curve& curve );
  void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingDXSingleSpeed_Impl ImplType;

  friend class detail::CoilHeatingDXSingleSpeed_Impl;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit CoilHeatingDXSingleSpeed(std::shared_ptr<detail::CoilHeatingDXSingleSpeed_Impl> impl);
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXSingleSpeed");
};

/** \relates CoilHeatingDXSingleSpeed*/
typedef boost::optional<CoilHeatingDXSingleSpeed> OptionalCoilHeatingDXSingleSpeed;

/** \relates CoilHeatingDXSingleSpeed*/
typedef std::vector<CoilHeatingDXSingleSpeed> CoilHeatingDXSingleSpeedVector;

} // model

} // openstudio

#endif // MODEL_COILHEATINGDXSINGLESPEED_HPP

