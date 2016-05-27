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

#ifndef MODEL_GENERATORMICROTURBINE_HPP
#define MODEL_GENERATORMICROTURBINE_HPP

#include <model/ModelAPI.hpp>
#include "Generator.hpp"

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class Curve;

// TODO: Do I need the child classes of Curve too? It seems that I can use the generic Curve for everything here, I'll only use in .cpp when setting
//class CurveBicubic;
//class CurveBiquadratic;
//class CurveCubic;
//class CurveQuadratic;

// TODO: add the tables class if they get added to OS later?
//class UniVariateTables
//class BiVariateTables

//class Connection;

namespace detail {

  class GeneratorMicroTurbine_Impl;

} // detail

/** GeneratorMicroTurbine is a Generator that wraps the OpenStudio IDD object 'OS:Generator:MicroTurbine'. */
class MODEL_API GeneratorMicroTurbine : public Generator {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorMicroTurbine(const Model& model);
  
  // TODO: Could add a constructor with the curve but that'd be nuts, there are 13 of them

  virtual ~GeneratorMicroTurbine() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validFuelTypeValues();

  static std::vector<std::string> validHeatRecoveryWaterFlowOperatingModeValues();

  /** @name Getters */
  //@{

  double referenceElectricalPowerOutput() const;

  double minimumFullLoadElectricalPowerOutput() const;
  bool isMinimumFullLoadElectricalPowerOutputDefaulted() const;

  boost::optional<double> maximumFullLoadElectricalPowerOutput() const;

  double referenceElectricalEfficiencyUsingLowerHeatingValue() const;

  double referenceCombustionAirInletTemperature() const;
  bool isReferenceCombustionAirInletTemperatureDefaulted() const;

  double referenceCombustionAirInletHumidityRatio() const;
  bool isReferenceCombustionAirInletHumidityRatioDefaulted() const;

  double referenceElevation() const;
  bool isReferenceElevationDefaulted() const;

  Curve electricalPowerFunctionofTemperatureandElevationCurve() const;
  
  Curve electricalEfficiencyFunctionofTemperatureCurve() const;
  
  Curve electricalEfficiencyFunctionofPartLoadRatioCurve() const;

  std::string fuelType() const;
  bool isFuelTypeDefaulted() const;

  double fuelHigherHeatingValue() const;
  bool isFuelHigherHeatingValueDefaulted() const;

  double fuelLowerHeatingValue() const;
  bool isFuelLowerHeatingValueDefaulted() const;

  double standbyPower() const;
  bool isStandbyPowerDefaulted() const;

  double ancillaryPower() const;
  bool isAncillaryPowerDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  boost::optional<Curve> ancillaryPowerFunctionofFuelInputCurve() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // boost::optional<Connection> heatRecoveryWaterInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // boost::optional<Connection> heatRecoveryWaterOutletNode() const;

  double referenceThermalEfficiencyUsingLowerHeatValue() const;
  bool isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const;

  boost::optional<double> referenceInletWaterTemperature() const;

  std::string heatRecoveryWaterFlowOperatingMode() const;

  bool isHeatRecoveryWaterFlowOperatingModeDefaulted() const;

  boost::optional<double> referenceHeatRecoveryWaterFlowRate() const;

  // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  boost::optional<Curve> heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const;

  // TODO: Check return type. From object lists, some candidates are: BicubicBiquadraticCurves, BiVariateTables.
  boost::optional<Curve> thermalEfficiencyFunctionofTemperatureandElevationCurve() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  boost::optional<Curve> heatRecoveryRateFunctionofPartLoadRatioCurve() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  boost::optional<Curve> heatRecoveryRateFunctionofInletWaterTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  boost::optional<Curve> heatRecoveryRateFunctionofWaterFlowRateCurve() const;

  double minimumHeatRecoveryWaterFlowRate() const;
  bool isMinimumHeatRecoveryWaterFlowRateDefaulted() const;

  double maximumHeatRecoveryWaterFlowRate() const;
  bool isMaximumHeatRecoveryWaterFlowRateDefaulted() const;

  boost::optional<double> maximumHeatRecoveryWaterTemperature() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  //boost::optional<Connection> combustionAirInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // boost::optional<Connection> combustionAirOutletNode() const;

  boost::optional<double> referenceExhaustAirMassFlowRate() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  boost::optional<Curve> exhaustAirFlowRateFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  boost::optional<Curve> exhaustAirFlowRateFunctionofPartLoadRatioCurve() const;

  boost::optional<double> nominalExhaustAirOutletTemperature() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  boost::optional<Curve> exhaustAirTemperatureFunctionofTemperatureCurve() const;

  // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  boost::optional<Curve> exhaustAirTemperatureFunctionofPartLoadRatioCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceElectricalPowerOutput(double referenceElectricalPowerOutput);

  bool setMinimumFullLoadElectricalPowerOutput(double minimumFullLoadElectricalPowerOutput);
  void resetMinimumFullLoadElectricalPowerOutput();

  bool setMaximumFullLoadElectricalPowerOutput(double maximumFullLoadElectricalPowerOutput);
  void resetMaximumFullLoadElectricalPowerOutput();

  bool setReferenceElectricalEfficiencyUsingLowerHeatingValue(double referenceElectricalEfficiencyUsingLowerHeatingValue);

  void setReferenceCombustionAirInletTemperature(double referenceCombustionAirInletTemperature);
  void resetReferenceCombustionAirInletTemperature();

  bool setReferenceCombustionAirInletHumidityRatio(double referenceCombustionAirInletHumidityRatio);
  void resetReferenceCombustionAirInletHumidityRatio();

  bool setReferenceElevation(double referenceElevation);
  void resetReferenceElevation();

  // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  bool setElectricalPowerFunctionofTemperatureandElevationCurve(const Curve& electricalPowerFunctionofTemperatureandElevationCurve);

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
  bool setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& electricalEfficiencyFunctionofTemperatureCurve);

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
  bool setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& electricalEfficiencyFunctionofPartLoadRatioCurve);

  bool setFuelType(std::string fuelType);
  void resetFuelType();

  bool setFuelHigherHeatingValue(double fuelHigherHeatingValue);
  void resetFuelHigherHeatingValue();

  bool setFuelLowerHeatingValue(double fuelLowerHeatingValue);
  void resetFuelLowerHeatingValue();

  bool setStandbyPower(double standbyPower);
  void resetStandbyPower();

  bool setAncillaryPower(double ancillaryPower);
  void resetAncillaryPower();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  bool setAncillaryPowerFunctionofFuelInputCurve(const Curve& ancillaryPowerFunctionofFuelInputCurve);
  void resetAncillaryPowerFunctionofFuelInputCurve();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setHeatRecoveryWaterInletNode(const Connection& connection);
  //void resetHeatRecoveryWaterInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setHeatRecoveryWaterOutletNode(const Connection& connection);
  //void resetHeatRecoveryWaterOutletNode();

  bool setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue);
  void resetReferenceThermalEfficiencyUsingLowerHeatValue();

  void setReferenceInletWaterTemperature(double referenceInletWaterTemperature);
  void resetReferenceInletWaterTemperature();

  bool setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode);
  void resetHeatRecoveryWaterFlowOperatingMode();

  bool setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate);
  void resetReferenceHeatRecoveryWaterFlowRate();

  // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
  bool setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve);
  void resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();

  // TODO: Check argument type. From object lists, some candidates are: BicubicBiquadraticCurves, BiVariateTables.
  bool setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& thermalEfficiencyFunctionofTemperatureandElevationCurve);
  void resetThermalEfficiencyFunctionofTemperatureandElevationCurve();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  bool setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& heatRecoveryRateFunctionofPartLoadRatioCurve);
  void resetHeatRecoveryRateFunctionofPartLoadRatioCurve();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  bool setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& heatRecoveryRateFunctionofInletWaterTemperatureCurve);
  void resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
  bool setHeatRecoveryRateFunctionofWaterFlowRateCurve(const curve& heatRecoveryRateFunctionofWaterFlowRateCurve);
  void resetHeatRecoveryRateFunctionofWaterFlowRateCurve();

  bool setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate);
  void resetMinimumHeatRecoveryWaterFlowRate();

  bool setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate);
  void resetMaximumHeatRecoveryWaterFlowRate();

  void setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature);
  void resetMaximumHeatRecoveryWaterTemperature();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setCombustionAirInletNode(const Connection& connection);
  //void resetCombustionAirInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setCombustionAirOutletNode(const Connection& connection);
  //void resetCombustionAirOutletNode();

  bool setReferenceExhaustAirMassFlowRate(double referenceExhaustAirMassFlowRate);
  void resetReferenceExhaustAirMassFlowRate();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  bool setExhaustAirFlowRateFunctionofTemperatureCurve(const Curve& exhaustAirFlowRateFunctionofTemperatureCurve);
  void resetExhaustAirFlowRateFunctionofTemperatureCurve();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  bool setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const Curve& exhaustAirFlowRateFunctionofPartLoadRatioCurve);
  void resetExhaustAirFlowRateFunctionofPartLoadRatioCurve();

  void setNominalExhaustAirOutletTemperature(double nominalExhaustAirOutletTemperature);
  void resetNominalExhaustAirOutletTemperature();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  bool setExhaustAirTemperatureFunctionofTemperatureCurve(const Curve& exhaustAirTemperatureFunctionofTemperatureCurve);
  void resetExhaustAirTemperatureFunctionofTemperatureCurve();

  // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
  bool setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const Curve& exhaustAirTemperatureFunctionofPartLoadRatioCurve);
  void resetExhaustAirTemperatureFunctionofPartLoadRatioCurve();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorMicroTurbine_Impl ImplType;

  explicit GeneratorMicroTurbine(std::shared_ptr<detail::GeneratorMicroTurbine_Impl> impl);

  friend class detail::GeneratorMicroTurbine_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbine");
};

/** \relates GeneratorMicroTurbine*/
typedef boost::optional<GeneratorMicroTurbine> OptionalGeneratorMicroTurbine;

/** \relates GeneratorMicroTurbine*/
typedef std::vector<GeneratorMicroTurbine> GeneratorMicroTurbineVector;

} // model
} // openstudio

#endif // MODEL_GENERATORMICROTURBINE_HPP

