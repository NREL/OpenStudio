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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class HeatPumpWaterToWaterEquationFitCooling_Impl;

} // detail

/** HeatPumpWaterToWaterEquationFitCooling is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:WaterToWater:EquationFit:Cooling'. */
class MODEL_API HeatPumpWaterToWaterEquationFitCooling : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HeatPumpWaterToWaterEquationFitCooling(const Model& model);

  virtual ~HeatPumpWaterToWaterEquationFitCooling() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double ratedLoadSideFlowRate() const;

  double ratedSourceSideFlowRate() const;

  double ratedCoolingCapacity() const;

  double ratedCoolingPowerConsumption() const;

  double coolingCapacityCoefficient1() const;

  double coolingCapacityCoefficient2() const;

  double coolingCapacityCoefficient3() const;

  double coolingCapacityCoefficient4() const;

  double coolingCapacityCoefficient5() const;

  double coolingCompressorPowerCoefficient1() const;

  double coolingCompressorPowerCoefficient2() const;

  double coolingCompressorPowerCoefficient3() const;

  double coolingCompressorPowerCoefficient4() const;

  double coolingCompressorPowerCoefficient5() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

  bool setRatedSourceSideFlowRate(double ratedSourceSideFlowRate);

  bool setRatedCoolingCapacity(double ratedCoolingCapacity);

  bool setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption);

  void setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1);

  void setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2);

  void setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3);

  void setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4);

  void setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5);

  void setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1);

  void setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2);

  void setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3);

  void setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4);

  void setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::HeatPumpWaterToWaterEquationFitCooling_Impl ImplType;

  explicit HeatPumpWaterToWaterEquationFitCooling(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitCooling_Impl> impl);

  friend class detail::HeatPumpWaterToWaterEquationFitCooling_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitCooling");
};

/** \relates HeatPumpWaterToWaterEquationFitCooling*/
typedef boost::optional<HeatPumpWaterToWaterEquationFitCooling> OptionalHeatPumpWaterToWaterEquationFitCooling;

/** \relates HeatPumpWaterToWaterEquationFitCooling*/
typedef std::vector<HeatPumpWaterToWaterEquationFitCooling> HeatPumpWaterToWaterEquationFitCoolingVector;

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP

