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

#ifndef MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_HPP
#define MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class CurveLinear;
// class ThermalZone;

namespace detail {

  class RefrigerationGasCoolerAirCooled_Impl;

} // detail

/** RefrigerationGasCoolerAirCooled is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:GasCooler:AirCooled'. */
class MODEL_API RefrigerationGasCoolerAirCooled : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationGasCoolerAirCooled(const Model& model);

  virtual ~RefrigerationGasCoolerAirCooled() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> gasCoolerFanSpeedControlTypeValues();

  /** @name Getters */
  //@{

  boost::optional<CurveLinear> ratedTotalHeatRejectionRateCurve() const;

  std::string gasCoolerFanSpeedControlType() const;

  bool isGasCoolerFanSpeedControlTypeDefaulted() const;

  double ratedFanPower() const;

  bool isRatedFanPowerDefaulted() const;

  double minimumFanAirFlowRatio() const;

  bool isMinimumFanAirFlowRatioDefaulted() const;

  double transitionTemperature() const;

  bool isTransitionTemperatureDefaulted() const;

  double transcriticalApproachTemperature() const;

  bool isTranscriticalApproachTemperatureDefaulted() const;

  double subcriticalTemperatureDifference() const;

  bool isSubcriticalTemperatureDifferenceDefaulted() const;

  double minimumCondensingTemperature() const;

  bool isMinimumCondensingTemperatureDefaulted() const;

  // boost::optional<ThermalZone> airInletNode() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  double gasCoolerRefrigerantOperatingChargeInventory() const;

  bool isGasCoolerRefrigerantOperatingChargeInventoryDefaulted() const;

  double gasCoolerReceiverRefrigerantInventory() const;

  bool isGasCoolerReceiverRefrigerantInventoryDefaulted() const;

  double gasCoolerOutletPipingRefrigerantInventory() const;

  bool isGasCoolerOutletPipingRefrigerantInventoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedTotalHeatRejectionRateCurve(const CurveLinear& curveLinear);

  void resetRatedTotalHeatRejectionRateCurve();

  bool setGasCoolerFanSpeedControlType(std::string gasCoolerFanSpeedControlType);

  void resetGasCoolerFanSpeedControlType();

  bool setRatedFanPower(double ratedFanPower);

  void resetRatedFanPower();

  bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

  void resetMinimumFanAirFlowRatio();

  void setTransitionTemperature(double transitionTemperature);

  void resetTransitionTemperature();

  void setTranscriticalApproachTemperature(double transcriticalApproachTemperature);

  void resetTranscriticalApproachTemperature();

  void setSubcriticalTemperatureDifference(double subcriticalTemperatureDifference);

  void resetSubcriticalTemperatureDifference();

  void setMinimumCondensingTemperature(double minimumCondensingTemperature);

  void resetMinimumCondensingTemperature();

  // bool setAirInletNode(const ThermalZone& thermalZone);

  // void resetAirInletNode();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  void setGasCoolerRefrigerantOperatingChargeInventory(double gasCoolerRefrigerantOperatingChargeInventory);

  void resetGasCoolerRefrigerantOperatingChargeInventory();

  void setGasCoolerReceiverRefrigerantInventory(double gasCoolerReceiverRefrigerantInventory);

  void resetGasCoolerReceiverRefrigerantInventory();

  void setGasCoolerOutletPipingRefrigerantInventory(double gasCoolerOutletPipingRefrigerantInventory);

  void resetGasCoolerOutletPipingRefrigerantInventory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationGasCoolerAirCooled_Impl ImplType;

  explicit RefrigerationGasCoolerAirCooled(std::shared_ptr<detail::RefrigerationGasCoolerAirCooled_Impl> impl);

  friend class detail::RefrigerationGasCoolerAirCooled_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationGasCoolerAirCooled");
};

/** \relates RefrigerationGasCoolerAirCooled*/
typedef boost::optional<RefrigerationGasCoolerAirCooled> OptionalRefrigerationGasCoolerAirCooled;

/** \relates RefrigerationGasCoolerAirCooled*/
typedef std::vector<RefrigerationGasCoolerAirCooled> RefrigerationGasCoolerAirCooledVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_HPP

