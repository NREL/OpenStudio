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

#ifndef MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilCoolingDXVariableSpeedSpeedData_Impl;

} // detail

/** CoilCoolingDXVariableSpeedSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableSpeed:SpeedData'. */
class MODEL_API CoilCoolingDXVariableSpeedSpeedData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingDXVariableSpeedSpeedData(const Model& model);

  explicit CoilCoolingDXVariableSpeedSpeedData(const Model& model,
                                               const Curve& totalCoolingCapacityFunctionofTemperatureCurve,
                                               const Curve& totalCoolingCapacityFunctionofAirFlowFractionCurve,
                                               const Curve& energyInputRatioFunctionofTemperatureCurve,
                                               const Curve& energyInputRatioFunctionofAirFlowFractionCurve);

  virtual ~CoilCoolingDXVariableSpeedSpeedData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double referenceUnitGrossRatedTotalCoolingCapacity() const;

  double referenceUnitGrossRatedSensibleHeatRatio() const;

  double referenceUnitGrossRatedCoolingCOP() const;

  double referenceUnitRatedAirFlowRate() const;

  boost::optional<double> referenceUnitRatedCondenserAirFlowRate() const;

  boost::optional<double> referenceUnitRatedPadEffectivenessofEvapPrecooling() const;

  Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

  Curve totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

  Curve energyInputRatioFunctionofTemperatureCurve() const;

  Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

  bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

  bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

  bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

  bool setReferenceUnitRatedCondenserAirFlowRate(double referenceUnitRatedCondenserAirFlowRate);

  void resetReferenceUnitRatedCondenserAirFlowRate();

  bool setReferenceUnitRatedPadEffectivenessofEvapPrecooling(double referenceUnitRatedPadEffectivenessofEvapPrecooling);

  void resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();

  bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingDXVariableSpeedSpeedData_Impl ImplType;

  explicit CoilCoolingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilCoolingDXVariableSpeedSpeedData_Impl> impl);

  friend class detail::CoilCoolingDXVariableSpeedSpeedData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeedSpeedData");
};

/** \relates CoilCoolingDXVariableSpeedSpeedData*/
typedef boost::optional<CoilCoolingDXVariableSpeedSpeedData> OptionalCoilCoolingDXVariableSpeedSpeedData;

/** \relates CoilCoolingDXVariableSpeedSpeedData*/
typedef std::vector<CoilCoolingDXVariableSpeedSpeedData> CoilCoolingDXVariableSpeedSpeedDataVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_HPP

