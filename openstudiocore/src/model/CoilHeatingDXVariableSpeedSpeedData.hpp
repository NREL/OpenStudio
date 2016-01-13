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

#ifndef MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_HPP
#define MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilHeatingDXVariableSpeedSpeedData_Impl;

} // detail

/** CoilHeatingDXVariableSpeedSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:VariableSpeed:SpeedData'. */
class MODEL_API CoilHeatingDXVariableSpeedSpeedData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXVariableSpeedSpeedData(const Model& model);

  explicit CoilHeatingDXVariableSpeedSpeedData(const Model& model,
                                               const Curve& heatingCapacityFunctionofTemperatureCurve,
                                               const Curve& totalHeatingCapacityFunctionofAirFlowFractionCurve,
                                               const Curve& energyInputRatioFunctionofTemperatureCurve,
                                               const Curve& energyInputRatioFunctionofAirFlowFractionCurve);

  virtual ~CoilHeatingDXVariableSpeedSpeedData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double referenceUnitGrossRatedHeatingCapacity() const;

  double referenceUnitGrossRatedHeatingCOP() const;

  double referenceUnitRatedAirFlowRate() const;

  Curve heatingCapacityFunctionofTemperatureCurve() const;

  Curve totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

  Curve energyInputRatioFunctionofTemperatureCurve() const;

  Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

  bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

  bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

  bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingDXVariableSpeedSpeedData_Impl ImplType;

  explicit CoilHeatingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilHeatingDXVariableSpeedSpeedData_Impl> impl);

  friend class detail::CoilHeatingDXVariableSpeedSpeedData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeedSpeedData");
};

/** \relates CoilHeatingDXVariableSpeedSpeedData*/
typedef boost::optional<CoilHeatingDXVariableSpeedSpeedData> OptionalCoilHeatingDXVariableSpeedSpeedData;

/** \relates CoilHeatingDXVariableSpeedSpeedData*/
typedef std::vector<CoilHeatingDXVariableSpeedSpeedData> CoilHeatingDXVariableSpeedSpeedDataVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_HPP

