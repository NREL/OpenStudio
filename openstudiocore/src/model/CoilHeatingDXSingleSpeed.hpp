/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

  explicit CoilHeatingDXSingleSpeed(const Model& model);

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

  double ratedSupplyFanPowerPerVolumeFlowRate() const;

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

  bool setRatedSupplyFanPowerPerVolumeFlowRate(double ratedSupplyFanPowerPerVolumeFlowRate);

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

  boost::optional<double> autosizedRatedTotalHeatingCapacity() const ;

  boost::optional<double> autosizedRatedAirFlowRate() const ;

  boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const ;



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

