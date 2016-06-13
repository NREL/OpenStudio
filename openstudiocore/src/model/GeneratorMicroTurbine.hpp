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
#include "GeneratorMicroTurbineHeatRecovery.hpp"

namespace openstudio {

namespace model {

class Curve;
// For the optional Generator:MicroTurbine:HeatRecovery
// It was broken out because that part needs to connect to a plant loop
class StraightComponent;

// TODO: add the tables class if they get added to OS later?
//class DataTables // UniVariateTables and BiVariateTables

//class Connection;

namespace detail {

  class GeneratorMicroTurbine_Impl;

} // detail

/** GeneratorMicroTurbine is a Generator that wraps the OpenStudio IDD object 'OS:Generator:MicroTurbine'. */
class MODEL_API GeneratorMicroTurbine : public Generator {
 public:
  /** @name Constructors and Destructors */
  //@{

  // TODO: Could add a constructor with all the required inputs, but that's a lot of them

  explicit GeneratorMicroTurbine(const Model& model);

  virtual ~GeneratorMicroTurbine() {}
  
  virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const;

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validFuelTypeValues();

  /** @name Getters */
  //@{
    
  // TODO: In E+ there's ElectricLoadCenter:GeneratorList where you'll specify the order of generators and the availability Schedule
  // here it's been moved to the Generator themselves
  boost::optional<Schedule> availabilitySchedule() const;

  double referenceElectricalPowerOutput() const;

  double minimumFullLoadElectricalPowerOutput() const;
  bool isMinimumFullLoadElectricalPowerOutputDefaulted() const;

  // This will default to referenceElectricalPowerOutput if not defined, like E+ does
  double maximumFullLoadElectricalPowerOutput() const;
  bool isMaximumFullLoadElectricalPowerOutputDefaulted() const;

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

  // Optional Generator:MicroTurbine:HeatRecovery
  boost::optional<GeneratorMicroTurbineHeatRecovery> generatorMicroTurbineHeatRecovery() const;
  
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
    
  bool setAvailabilitySchedule(Schedule& schedule);
  void resetAvailabilitySchedule();

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

  bool setFuelType(const std::string& fuelType);
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
  
  // Optional Generator:MicroTurbine:HeatRecovery
  //bool setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& generatorMicroTurbineHeatRecovery);
  //void resetGeneratorMicroTurbineHeatRecovery();

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

