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

#ifndef MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_HPP
#define MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

// WaterStorageTank object not used
//class WaterStorageTank;
class Schedule;

namespace detail {

  class RefrigerationCondenserEvaporativeCooled_Impl;

} // detail

/** RefrigerationCondenserEvaporativeCooled is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:EvaporativeCooled'. */
class MODEL_API RefrigerationCondenserEvaporativeCooled : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationCondenserEvaporativeCooled(const Model& model);

  virtual ~RefrigerationCondenserEvaporativeCooled() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fanSpeedControlTypeValues();

  /** @name Getters */
  //@{

  double ratedEffectiveTotalHeatRejectionRate() const;

  double ratedSubcoolingTemperatureDifference() const;

  bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

  std::string fanSpeedControlType() const;

  bool isFanSpeedControlTypeDefaulted() const;

  double ratedFanPower() const;

  double minimumFanAirFlowRatio() const;

  bool isMinimumFanAirFlowRatioDefaulted() const;

  double approachTemperatureConstantTerm() const;

  bool isApproachTemperatureConstantTermDefaulted() const;

  double approachTemperatureCoefficient2() const;

  bool isApproachTemperatureCoefficient2Defaulted() const;

  double approachTemperatureCoefficient3() const;

  bool isApproachTemperatureCoefficient3Defaulted() const;

  double approachTemperatureCoefficient4() const;

  bool isApproachTemperatureCoefficient4Defaulted() const;

  double minimumCapacityFactor() const;

  bool isMinimumCapacityFactorDefaulted() const;

  double maximumCapacityFactor() const;

  bool isMaximumCapacityFactorDefaulted() const;

  //std::string airInletNode() const;

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateDefaulted() const;

  bool isRatedAirFlowRateAutocalculated() const;

  double basinHeaterCapacity() const;

  bool isBasinHeaterCapacityDefaulted() const;

  double basinHeaterSetpointTemperature() const;

  bool isBasinHeaterSetpointTemperatureDefaulted() const;

  boost::optional<double> ratedWaterPumpPower() const;

  bool isRatedWaterPumpPowerDefaulted() const;

  bool isRatedWaterPumpPowerAutocalculated() const;

  //boost::optional<WaterStorageTank> evaporativeWaterSupplyTank() const;

  boost::optional<Schedule> evaporativeCondenserAvailabilitySchedule() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  double condenserRefrigerantOperatingChargeInventory() const;

  bool isCondenserRefrigerantOperatingChargeInventoryDefaulted() const;

  double condensateReceiverRefrigerantInventory() const;

  bool isCondensateReceiverRefrigerantInventoryDefaulted() const;

  double condensatePipingRefrigerantInventory() const;

  bool isCondensatePipingRefrigerantInventoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

  bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

  void resetRatedSubcoolingTemperatureDifference();

  bool setFanSpeedControlType(std::string fanSpeedControlType);

  void resetFanSpeedControlType();

  bool setRatedFanPower(double ratedFanPower);

  bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

  void resetMinimumFanAirFlowRatio();

  bool setApproachTemperatureConstantTerm(double approachTemperatureConstantTerm);

  void resetApproachTemperatureConstantTerm();

  bool setApproachTemperatureCoefficient2(double approachTemperatureCoefficient2);

  void resetApproachTemperatureCoefficient2();

  bool setApproachTemperatureCoefficient3(double approachTemperatureCoefficient3);

  void resetApproachTemperatureCoefficient3();

  bool setApproachTemperatureCoefficient4(double approachTemperatureCoefficient4);

  void resetApproachTemperatureCoefficient4();

  void setMinimumCapacityFactor(double minimumCapacityFactor);

  void resetMinimumCapacityFactor();

  void setMaximumCapacityFactor(double maximumCapacityFactor);

  void resetMaximumCapacityFactor();

  //bool setAirInletNode(std::string airInletNode);

  //void resetAirInletNode();

  void setRatedAirFlowRate(double ratedAirFlowRate);

  void resetRatedAirFlowRate();

  void autocalculateRatedAirFlowRate();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  void resetBasinHeaterCapacity();

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  void resetBasinHeaterSetpointTemperature();

  void setRatedWaterPumpPower(double ratedWaterPumpPower);

  void resetRatedWaterPumpPower();

  void autocalculateRatedWaterPumpPower();

  //bool setEvaporativeWaterSupplyTank(const WaterStorageTank& waterStorageTank);

  //void resetEvaporativeWaterSupplyTank();

  bool setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule);

  void resetEvaporativeCondenserAvailabilitySchedule();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  void setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

  void resetCondenserRefrigerantOperatingChargeInventory();

  void setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

  void resetCondensateReceiverRefrigerantInventory();

  void setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

  void resetCondensatePipingRefrigerantInventory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationCondenserEvaporativeCooled_Impl ImplType;

  explicit RefrigerationCondenserEvaporativeCooled(std::shared_ptr<detail::RefrigerationCondenserEvaporativeCooled_Impl> impl);

  friend class detail::RefrigerationCondenserEvaporativeCooled_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationCondenserEvaporativeCooled");
};

/** \relates RefrigerationCondenserEvaporativeCooled*/
typedef boost::optional<RefrigerationCondenserEvaporativeCooled> OptionalRefrigerationCondenserEvaporativeCooled;

/** \relates RefrigerationCondenserEvaporativeCooled*/
typedef std::vector<RefrigerationCondenserEvaporativeCooled> RefrigerationCondenserEvaporativeCooledVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_HPP

