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

#ifndef MODEL_GENERATORMICROTURBINEHEATRECOVERY_HPP
#define MODEL_GENERATORMICROTURBINEHEATRECOVERY_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"


namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Curve;
class GeneratorMicroTurbine;

namespace detail {

  class GeneratorMicroTurbineHeatRecovery_Impl;

} // detail

/** GeneratorMicroTurbineHeatRecovery is a StraightComponent that wraps the OpenStudio IDD object 'OS:Generator:MicroTurbine:HeatRecovery'. */
class MODEL_API GeneratorMicroTurbineHeatRecovery : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{
    
 // Constructs a new GeneratorMicroTurbineHeatRecovery object in the model, given a GeneratorMicroTurbine
  explicit GeneratorMicroTurbineHeatRecovery( const Model& model, 
                                              GeneratorMicroTurbine & mchp );
  
  virtual ~GeneratorMicroTurbineHeatRecovery() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validHeatRecoveryWaterFlowOperatingModeValues();

  /** @name Getters */
  //@{
      
  // TODO: Check return type. From object lists, some candidates are: Connection.
  // boost::optional<Connection> heatRecoveryWaterInletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  // boost::optional<Connection> heatRecoveryWaterOutletNode() const;

  double referenceThermalEfficiencyUsingLowerHeatValue() const;
  bool isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const;

  double referenceInletWaterTemperature() const;

  std::string heatRecoveryWaterFlowOperatingMode() const;
  bool isHeatRecoveryWaterFlowOperatingModeDefaulted() const;

  double referenceHeatRecoveryWaterFlowRate() const;

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
  
  double ratedThermaltoElectricalPowerRatio() const;
  bool isRatedThermaltoElectricalPowerRatioDefaulted() const;
  
  // Return optional parent generator
  GeneratorMicroTurbine generatorMicroTurbine() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setHeatRecoveryWaterInletNode(const Connection& connection);
  //void resetHeatRecoveryWaterInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  //bool setHeatRecoveryWaterOutletNode(const Connection& connection);
  //void resetHeatRecoveryWaterOutletNode();

  bool setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue);
  void resetReferenceThermalEfficiencyUsingLowerHeatValue();

  bool setReferenceInletWaterTemperature(double referenceInletWaterTemperature);

  bool setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode);
  void resetHeatRecoveryWaterFlowOperatingMode();

  bool setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate);
  //void resetReferenceHeatRecoveryWaterFlowRate();

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
  bool setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& heatRecoveryRateFunctionofWaterFlowRateCurve);
  void resetHeatRecoveryRateFunctionofWaterFlowRateCurve();

  bool setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate);
  void resetMinimumHeatRecoveryWaterFlowRate();

  bool setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate);
  void resetMaximumHeatRecoveryWaterFlowRate();

  bool setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature);
  void resetMaximumHeatRecoveryWaterTemperature();

  bool setRatedThermaltoElectricalPowerRatio(double ratedThermaltoElectricalPowerRatio);
  void resetRatedThermaltoElectricalPowerRatio();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GeneratorMicroTurbineHeatRecovery_Impl ImplType;

  explicit GeneratorMicroTurbineHeatRecovery(std::shared_ptr<detail::GeneratorMicroTurbineHeatRecovery_Impl> impl);

  friend class detail::GeneratorMicroTurbineHeatRecovery_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbineHeatRecovery");
};

/** \relates GeneratorMicroTurbineHeatRecovery*/
typedef boost::optional<GeneratorMicroTurbineHeatRecovery> OptionalGeneratorMicroTurbineHeatRecovery;

/** \relates GeneratorMicroTurbineHeatRecovery*/
typedef std::vector<GeneratorMicroTurbineHeatRecovery> GeneratorMicroTurbineHeatRecoveryVector;

} // model
} // openstudio

#endif // MODEL_GENERATORMICROTURBINEHEATRECOVERY_HPP

