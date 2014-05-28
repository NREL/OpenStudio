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

#ifndef MODEL_REFRIGERATIONCONDENSERWATERCOOLED_HPP
#define MODEL_REFRIGERATIONCONDENSERWATERCOOLED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class RefrigerationCondenserWaterCooled_Impl;

} // detail

/** RefrigerationCondenserWaterCooled is a StraightComponent that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:WaterCooled'. */
class MODEL_API RefrigerationCondenserWaterCooled : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationCondenserWaterCooled(const Model& model);

  virtual ~RefrigerationCondenserWaterCooled() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> waterCooledLoopFlowTypeValues();

  /** @name Getters */
  //@{

  boost::optional<double> ratedEffectiveTotalHeatRejectionRate() const;

  double ratedCondensingTemperature() const;

  double ratedSubcoolingTemperatureDifference() const;

  bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

  double ratedWaterInletTemperature() const;

  std::string waterCooledLoopFlowType() const;

  bool isWaterCooledLoopFlowTypeDefaulted() const;

  boost::optional<Schedule> waterOutletTemperatureSchedule() const;

  boost::optional<double> waterDesignFlowRate() const;

  boost::optional<double> waterMaximumFlowRate() const;

  double waterMaximumWaterOutletTemperature() const;

  bool isWaterMaximumWaterOutletTemperatureDefaulted() const;

  double waterMinimumWaterInletTemperature() const;

  bool isWaterMinimumWaterInletTemperatureDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

  boost::optional<double> condensateReceiverRefrigerantInventory() const;

  boost::optional<double> condensatePipingRefrigerantInventory() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

  void resetRatedEffectiveTotalHeatRejectionRate();

  bool setRatedCondensingTemperature(double ratedCondensingTemperature);

  bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

  void resetRatedSubcoolingTemperatureDifference();

  bool setRatedWaterInletTemperature(double ratedWaterInletTemperature);

  bool setWaterCooledLoopFlowType(std::string waterCooledLoopFlowType);

  void resetWaterCooledLoopFlowType();

  bool setWaterOutletTemperatureSchedule(Schedule& schedule);

  void resetWaterOutletTemperatureSchedule();

  bool setWaterDesignFlowRate(double waterDesignFlowRate);

  void resetWaterDesignFlowRate();

  bool setWaterMaximumFlowRate(double waterMaximumFlowRate);

  void resetWaterMaximumFlowRate();

  bool setWaterMaximumWaterOutletTemperature(double waterMaximumWaterOutletTemperature);

  void resetWaterMaximumWaterOutletTemperature();

  bool setWaterMinimumWaterInletTemperature(double waterMinimumWaterInletTemperature);

  void resetWaterMinimumWaterInletTemperature();

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
  typedef detail::RefrigerationCondenserWaterCooled_Impl ImplType;

  explicit RefrigerationCondenserWaterCooled(std::shared_ptr<detail::RefrigerationCondenserWaterCooled_Impl> impl);

  friend class detail::RefrigerationCondenserWaterCooled_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationCondenserWaterCooled");
};

/** \relates RefrigerationCondenserWaterCooled*/
typedef boost::optional<RefrigerationCondenserWaterCooled> OptionalRefrigerationCondenserWaterCooled;

/** \relates RefrigerationCondenserWaterCooled*/
typedef std::vector<RefrigerationCondenserWaterCooled> RefrigerationCondenserWaterCooledVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSERWATERCOOLED_HPP

