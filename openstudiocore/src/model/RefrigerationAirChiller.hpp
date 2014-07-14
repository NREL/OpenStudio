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

#ifndef MODEL_REFRIGERATIONAIRCHILLER_HPP
#define MODEL_REFRIGERATIONAIRCHILLER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
// class CurveLinear;

namespace detail {

  class RefrigerationAirChiller_Impl;

} // detail

/** RefrigerationAirChiller is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Refrigeration:AirChiller'. */
class MODEL_API RefrigerationAirChiller : public ZoneHVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationAirChiller(const Model& model, Schedule& defrostSchedule);

  virtual ~RefrigerationAirChiller() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> capacityRatingTypeValues();

  // static std::vector<std::string> capacityCorrectionCurveTypeValues();

  static std::vector<std::string> fanSpeedControlTypeValues();

  static std::vector<std::string> defrostTypeValues();

  static std::vector<std::string> defrostControlTypeValues();

  static std::vector<std::string> verticalLocationValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  std::string capacityRatingType() const;

  boost::optional<double> ratedUnitLoadFactor() const;

  boost::optional<double> ratedCapacity() const;

  double ratedRelativeHumidity() const;

  bool isRatedRelativeHumidityDefaulted() const;

  double ratedCoolingSourceTemperature() const;

  double ratedTemperatureDifferenceDT1() const;

  boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() const;

  double coilMaterialCorrectionFactor() const;

  bool isCoilMaterialCorrectionFactorDefaulted() const;

  double refrigerantCorrectionFactor() const;

  bool isRefrigerantCorrectionFactorDefaulted() const;

  // std::string capacityCorrectionCurveType() const;

  // bool isCapacityCorrectionCurveTypeDefaulted() const;

  // boost::optional<CurveLinear> capacityCorrectionCurve() const;

  double sHR60CorrectionFactor() const;

  bool isSHR60CorrectionFactorDefaulted() const;

  double ratedTotalHeatingPower() const;

  boost::optional<Schedule> heatingPowerSchedule() const;

  std::string fanSpeedControlType() const;

  bool isFanSpeedControlTypeDefaulted() const;

  double ratedFanPower() const;

  bool isRatedFanPowerDefaulted() const;

  double ratedAirFlow() const;

  double minimumFanAirFlowRatio() const;

  bool isMinimumFanAirFlowRatioDefaulted() const;

  std::string defrostType() const;

  bool isDefrostTypeDefaulted() const;

  std::string defrostControlType() const;

  bool isDefrostControlTypeDefaulted() const;

  Schedule defrostSchedule() const;

  boost::optional<Schedule> defrostDripDownSchedule() const;

  boost::optional<double> defrostPower() const;

  boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;

  std::string verticalLocation() const;

  bool isVerticalLocationDefaulted() const;

  double averageRefrigerantChargeInventory() const;

  bool isAverageRefrigerantChargeInventoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setCapacityRatingType(std::string capacityRatingType);

  void setRatedUnitLoadFactor(double ratedUnitLoadFactor);

  void resetRatedUnitLoadFactor();

  void setRatedCapacity(double ratedCapacity);

  void resetRatedCapacity();

  bool setRatedRelativeHumidity(double ratedRelativeHumidity);

  void resetRatedRelativeHumidity();

  bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

  bool setRatedTemperatureDifferenceDT1(double ratedTemperatureDifferenceDT1);

  bool setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(double maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature);

  void resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();

  void setCoilMaterialCorrectionFactor(double coilMaterialCorrectionFactor);

  void resetCoilMaterialCorrectionFactor();

  void setRefrigerantCorrectionFactor(double refrigerantCorrectionFactor);

  void resetRefrigerantCorrectionFactor();

  // bool setCapacityCorrectionCurveType(std::string capacityCorrectionCurveType);

  // void resetCapacityCorrectionCurveType();

  // bool setCapacityCorrectionCurve(const CurveLinear& curveLinear);

  // void resetCapacityCorrectionCurve();

  bool setSHR60CorrectionFactor(double sHR60CorrectionFactor);

  void resetSHR60CorrectionFactor();

  void setRatedTotalHeatingPower(double ratedTotalHeatingPower);

  bool setHeatingPowerSchedule(Schedule& schedule);

  void resetHeatingPowerSchedule();

  bool setFanSpeedControlType(std::string fanSpeedControlType);

  void resetFanSpeedControlType();

  bool setRatedFanPower(double ratedFanPower);

  void resetRatedFanPower();

  void setRatedAirFlow(double ratedAirFlow);

  bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

  void resetMinimumFanAirFlowRatio();

  bool setDefrostType(std::string defrostType);

  void resetDefrostType();

  bool setDefrostControlType(std::string defrostControlType);

  void resetDefrostControlType();

  bool setDefrostSchedule(Schedule& schedule);

  bool setDefrostDripDownSchedule(Schedule& schedule);

  void resetDefrostDripDownSchedule();

  bool setDefrostPower(double defrostPower);

  void resetDefrostPower();

  bool setTemperatureTerminationDefrostFractiontoIce(double temperatureTerminationDefrostFractiontoIce);

  void resetTemperatureTerminationDefrostFractiontoIce();

  // bool setVerticalLocation(std::string verticalLocation);

  // void resetVerticalLocation();

  void setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

  void resetAverageRefrigerantChargeInventory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationAirChiller_Impl ImplType;

  explicit RefrigerationAirChiller(std::shared_ptr<detail::RefrigerationAirChiller_Impl> impl);

  friend class detail::RefrigerationAirChiller_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationAirChiller");
};

/** \relates RefrigerationAirChiller*/
typedef boost::optional<RefrigerationAirChiller> OptionalRefrigerationAirChiller;

/** \relates RefrigerationAirChiller*/
typedef std::vector<RefrigerationAirChiller> RefrigerationAirChillerVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONAIRCHILLER_HPP

