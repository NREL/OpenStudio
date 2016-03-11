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

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class HeatPumpWaterToWaterEquationFitHeating_Impl;

} // detail

/** HeatPumpWaterToWaterEquationFitHeating is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:WaterToWater:EquationFit:Heating'. */
class MODEL_API HeatPumpWaterToWaterEquationFitHeating : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HeatPumpWaterToWaterEquationFitHeating(const Model& model);

  virtual ~HeatPumpWaterToWaterEquationFitHeating() {}

  //@}

  static IddObjectType iddObjectType();

  double ratedLoadSideFlowRate() const;

  double ratedSourceSideFlowRate() const;

  double ratedHeatingCapacity() const;

  double ratedHeatingPowerConsumption() const;

  double heatingCapacityCoefficient1() const;

  double heatingCapacityCoefficient2() const;

  double heatingCapacityCoefficient3() const;

  double heatingCapacityCoefficient4() const;

  double heatingCapacityCoefficient5() const;

  double heatingCompressorPowerCoefficient1() const;

  double heatingCompressorPowerCoefficient2() const;

  double heatingCompressorPowerCoefficient3() const;

  double heatingCompressorPowerCoefficient4() const;

  double heatingCompressorPowerCoefficient5() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

  bool setRatedSourceSideFlowRate(double ratedSourceSideFlowRate);

  bool setRatedHeatingCapacity(double ratedHeatingCapacity);

  bool setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption);

  void setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1);

  void setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2);

  void setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3);

  void setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4);

  void setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5);

  void setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1);

  void setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2);

  void setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3);

  void setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4);

  void setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::HeatPumpWaterToWaterEquationFitHeating_Impl ImplType;

  explicit HeatPumpWaterToWaterEquationFitHeating(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitHeating_Impl> impl);

  friend class detail::HeatPumpWaterToWaterEquationFitHeating_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitHeating");
};

/** \relates HeatPumpWaterToWaterEquationFitHeating*/
typedef boost::optional<HeatPumpWaterToWaterEquationFitHeating> OptionalHeatPumpWaterToWaterEquationFitHeating;

/** \relates HeatPumpWaterToWaterEquationFitHeating*/
typedef std::vector<HeatPumpWaterToWaterEquationFitHeating> HeatPumpWaterToWaterEquationFitHeatingVector;

} // model
} // openstudio

#endif // MODEL_HEATPUMPWATERTOWATEREQUATIONFITHEATING_HPP

