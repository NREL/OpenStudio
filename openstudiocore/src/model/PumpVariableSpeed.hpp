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

#ifndef MODEL_PUMPVARIABLESPEED_HPP
#define MODEL_PUMPVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;

namespace detail {

  class PumpVariableSpeed_Impl;

} // detail

/** PumpVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Pump:VariableSpeed'. */
class MODEL_API PumpVariableSpeed : public StraightComponent 
{
  public:

  /** @name Constructors and Destructors */
  //@{

  explicit PumpVariableSpeed(const Model& model);

  virtual ~PumpVariableSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> pumpControlTypeValues();

  /** \deprecated */
  static std::vector<std::string> validPumpControlTypeValues();

  static std::vector<std::string> vfdControlTypeValues();

  /** @name Getters */
  //@{

  boost::optional<double> ratedFlowRate() const;

  OSOptionalQuantity getRatedFlowRate(bool returnIP=false) const;

  bool isRatedFlowRateDefaulted() const;

  bool isRatedFlowRateAutosized() const;

  double ratedPumpHead() const;

  Quantity getRatedPumpHead(bool returnIP=false) const;

  bool isRatedPumpHeadDefaulted() const;

  boost::optional<double> ratedPowerConsumption() const;

  OSOptionalQuantity getRatedPowerConsumption(bool returnIP=false) const;

  bool isRatedPowerConsumptionDefaulted() const;

  bool isRatedPowerConsumptionAutosized() const;

  double motorEfficiency() const;

  Quantity getMotorEfficiency(bool returnIP=false) const;

  bool isMotorEfficiencyDefaulted() const;

  double fractionofMotorInefficienciestoFluidStream() const;

  Quantity getFractionofMotorInefficienciestoFluidStream(bool returnIP=false) const;

  bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

  double coefficient1ofthePartLoadPerformanceCurve() const;

  Quantity getCoefficient1ofthePartLoadPerformanceCurve(bool returnIP=false) const;

  bool isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const;

  double coefficient2ofthePartLoadPerformanceCurve() const;

  Quantity getCoefficient2ofthePartLoadPerformanceCurve(bool returnIP=false) const;

  bool isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const;

  double coefficient3ofthePartLoadPerformanceCurve() const;

  Quantity getCoefficient3ofthePartLoadPerformanceCurve(bool returnIP=false) const;

  bool isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const;

  double coefficient4ofthePartLoadPerformanceCurve() const;

  Quantity getCoefficient4ofthePartLoadPerformanceCurve(bool returnIP=false) const;

  bool isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const;

  double minimumFlowRate() const;

  Quantity getMinimumFlowRate(bool returnIP=false) const;

  bool isMinimumFlowRateDefaulted() const;

  std::string pumpControlType() const;

  bool isPumpControlTypeDefaulted() const;

  boost::optional<Schedule> pumpFlowRateSchedule() const;

  boost::optional<Curve> pumpCurve() const;

  boost::optional<double> impellerDiameter() const;

  OSOptionalQuantity getImpellerDiameter(bool returnIP=false) const;

  boost::optional<std::string> vFDControlType() const;

  boost::optional<Schedule> pumpRPMSchedule() const;

  boost::optional<Schedule> minimumPressureSchedule() const;

  boost::optional<Schedule> maximumPressureSchedule() const;

  boost::optional<Schedule> minimumRPMSchedule() const;

  boost::optional<Schedule> maximumRPMSchedule() const;

  //@}
  /** @name Setters */
  //@{

  void setRatedFlowRate(double ratedFlowRate);

  bool setRatedFlowRate(const Quantity& ratedFlowRate);

  void resetRatedFlowRate();

  void autosizeRatedFlowRate();

  void setRatedPumpHead(double ratedPumpHead);

  bool setRatedPumpHead(const Quantity& ratedPumpHead);

  void resetRatedPumpHead();

  void setRatedPowerConsumption(double ratedPowerConsumption);

  bool setRatedPowerConsumption(const Quantity& ratedPowerConsumption);

  void resetRatedPowerConsumption();

  void autosizeRatedPowerConsumption();

  bool setMotorEfficiency(double motorEfficiency);

  bool setMotorEfficiency(const Quantity& motorEfficiency);

  void resetMotorEfficiency();

  bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

  bool setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream);

  void resetFractionofMotorInefficienciestoFluidStream();

  void setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

  bool setCoefficient1ofthePartLoadPerformanceCurve(const Quantity& coefficient1ofthePartLoadPerformanceCurve);

  void resetCoefficient1ofthePartLoadPerformanceCurve();

  void setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

  bool setCoefficient2ofthePartLoadPerformanceCurve(const Quantity& coefficient2ofthePartLoadPerformanceCurve);

  void resetCoefficient2ofthePartLoadPerformanceCurve();

  void setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

  bool setCoefficient3ofthePartLoadPerformanceCurve(const Quantity& coefficient3ofthePartLoadPerformanceCurve);

  void resetCoefficient3ofthePartLoadPerformanceCurve();

  void setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

  bool setCoefficient4ofthePartLoadPerformanceCurve(const Quantity& coefficient4ofthePartLoadPerformanceCurve);

  void resetCoefficient4ofthePartLoadPerformanceCurve();

  void setMinimumFlowRate(double minimumFlowRate);

  bool setMinimumFlowRate(const Quantity& minimumFlowRate);

  void resetMinimumFlowRate();

  bool setPumpControlType(std::string pumpControlType);

  void resetPumpControlType();

  bool setPumpFlowRateSchedule(Schedule& schedule);

  void resetPumpFlowRateSchedule();

  /** Returns false if curve is not a CurveLinear, CurveQuadratic, CurveCubic or CurveQuartic. */
  bool setPumpCurve(const Curve& curve);

  void resetPumpCurve();

  void setImpellerDiameter(double impellerDiameter);

  bool setImpellerDiameter(const Quantity& impellerDiameter);

  void resetImpellerDiameter();

  bool setVFDControlType(std::string vFDControlType);

  void resetVFDControlType();

  bool setPumpRPMSchedule(Schedule& schedule);

  void resetPumpRPMSchedule();

  bool setMinimumPressureSchedule(Schedule& schedule);

  void resetMinimumPressureSchedule();

  bool setMaximumPressureSchedule(Schedule& schedule);

  void resetMaximumPressureSchedule();

  bool setMinimumRPMSchedule(Schedule& schedule);

  void resetMinimumRPMSchedule();

  bool setMaximumRPMSchedule(Schedule& schedule);

  void resetMaximumRPMSchedule();

  //@}
 protected:
  /// @cond
  typedef detail::PumpVariableSpeed_Impl ImplType;

  friend class detail::PumpVariableSpeed_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit PumpVariableSpeed(std::shared_ptr<detail::PumpVariableSpeed_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.PumpVariableSpeed");
};

/** \relates PumpVariableSpeed*/
typedef boost::optional<PumpVariableSpeed> OptionalPumpVariableSpeed;

/** \relates PumpVariableSpeed*/
typedef std::vector<PumpVariableSpeed> PumpVariableSpeedVector;

} // model
} // openstudio

#endif // MODEL_PUMPVARIABLESPEED_HPP

