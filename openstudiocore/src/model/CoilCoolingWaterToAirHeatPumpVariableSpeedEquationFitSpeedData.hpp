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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;

} // detail

/** CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:VariableSpeedEquationFit:SpeedData'. */
class MODEL_API CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(const Model& model);

  explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(
    const Model& model,
    const Curve& totalCoolingCapacityFunctionofTemperature,
    const Curve& totalCoolingCapacityFunctionofAirFlowFraction,
    const Curve& totalCoolingCapacityFunctionofWaterFlowFraction,
    const Curve& energyInputRatioFunctionofTemperature,
    const Curve& energyInputRatioFunctionofAirFlowFraction,
    const Curve& energyInputRatioFunctionofWaterFlowFraction,
    const Curve& wasteHeatFunctionofTemperature
  );

  virtual ~CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double referenceUnitGrossRatedTotalCoolingCapacity() const;

  double referenceUnitGrossRatedSensibleHeatRatio() const;

  double referenceUnitGrossRatedCoolingCOP() const;

  double referenceUnitRatedAirFlowRate() const;

  double referenceUnitRatedWaterFlowRate() const;

  Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

  Curve totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

  Curve totalCoolingCapacityFunctionofWaterFlowFractionCurve() const;

  Curve energyInputRatioFunctionofTemperatureCurve() const;

  Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

  Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

  double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

  Curve wasteHeatFunctionofTemperatureCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

  bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

  bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

  bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

  bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

  bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setTotalCoolingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve);

  bool setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);

  bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl ImplType;

  explicit CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl> impl);

  friend class detail::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");
};

/** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
typedef boost::optional<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> OptionalCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData;

/** \relates CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData*/
typedef std::vector<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedDataVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_HPP

