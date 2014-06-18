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

#ifndef MODEL_REFRIGERATIONWALKIN_HPP
#define MODEL_REFRIGERATIONWALKIN_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "RefrigerationSystem.hpp"
#include "ThermalZone.hpp"

namespace openstudio {

class Time;

namespace model {

class Schedule;
class RefrigerationWalkInZoneBoundary;
class RefrigerationDefrostCycleParameters;

namespace detail {

  class RefrigerationWalkIn_Impl;

} // detail

/** RefrigerationWalkIn is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:WalkIn'. */
class MODEL_API RefrigerationWalkIn : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationWalkIn(const Model& model, Schedule& walkinDefrostSchedule);

  virtual ~RefrigerationWalkIn() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> defrostTypeValues();

  static std::vector<std::string> defrostControlTypeValues();

  bool addZoneBoundary(const RefrigerationWalkInZoneBoundary& refrigerationWalkInZoneBoundary);

  void removeZoneBoundary(unsigned index);

  void removeAllZoneBoundaries();

  std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries() const;

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double ratedCoilCoolingCapacity() const;

  double operatingTemperature() const;

  double ratedCoolingSourceTemperature() const;

  double ratedTotalHeatingPower() const;

  boost::optional<Schedule> heatingPowerSchedule() const;

  double ratedCoolingCoilFanPower() const;

  bool isRatedCoolingCoilFanPowerDefaulted() const;

  double ratedCirculationFanPower() const;

  bool isRatedCirculationFanPowerDefaulted() const;

  double ratedTotalLightingPower() const;

  boost::optional<Schedule> lightingSchedule() const;

  std::string defrostType() const;

  bool isDefrostTypeDefaulted() const;

  std::string defrostControlType() const;

  bool isDefrostControlTypeDefaulted() const;

  Schedule defrostSchedule() const;

  boost::optional<Schedule> defrostDripDownSchedule() const;

  boost::optional<double> defrostPower() const;

  boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;

  boost::optional<Schedule> restockingSchedule() const;

  double averageRefrigerantChargeInventory() const;

  bool isAverageRefrigerantChargeInventoryDefaulted() const;

  double insulatedFloorSurfaceArea() const;

  double insulatedFloorUValue() const;

  bool isInsulatedFloorUValueDefaulted() const;

  boost::optional<int> durationofDefrostCycle() const;

  boost::optional<int> dripDownTime() const;

  boost::optional<openstudio::Time> defrost1StartTime() const;

  boost::optional<openstudio::Time> defrost2StartTime() const;

  boost::optional<openstudio::Time> defrost3StartTime() const;

  boost::optional<openstudio::Time> defrost4StartTime() const;

  boost::optional<openstudio::Time> defrost5StartTime() const;

  boost::optional<openstudio::Time> defrost6StartTime() const;

  boost::optional<openstudio::Time> defrost7StartTime() const;

  boost::optional<openstudio::Time> defrost8StartTime() const;

  boost::optional<RefrigerationSystem> system() const;

  boost::optional<ThermalZone> zoneBoundaryThermalZone() const;

  boost::optional<double> zoneBoundaryTotalInsulatedSurfaceAreaFacingZone() const;

  boost::optional<double> zoneBoundaryAreaofGlassReachInDoorsFacingZone() const;

  boost::optional<double> zoneBoundaryHeightofGlassReachInDoorsFacingZone() const;

  boost::optional<double> zoneBoundaryAreaofStockingDoorsFacingZone() const;

  boost::optional<double> zoneBoundaryHeightofStockingDoorsFacingZone() const;

  boost::optional<double> zoneBoundaryInsulatedSurfaceUValueFacingZone() const;

  boost::optional<double> zoneBoundaryGlassReachInDoorUValueFacingZone() const;

  boost::optional<double> zoneBoundaryStockingDoorUValueFacingZone() const;

  boost::optional<Schedule> zoneBoundaryStockingDoorOpeningScheduleFacingZone() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  void setRatedCoilCoolingCapacity(double ratedCoilCoolingCapacity);

  bool setOperatingTemperature(double operatingTemperature);

  bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

  void setRatedTotalHeatingPower(double ratedTotalHeatingPower);

  bool setHeatingPowerSchedule(Schedule& schedule);

  void resetHeatingPowerSchedule();

  bool setRatedCoolingCoilFanPower(double ratedCoolingCoilFanPower);

  void resetRatedCoolingCoilFanPower();

  bool setRatedCirculationFanPower(double ratedCirculationFanPower);

  void resetRatedCirculationFanPower();

  void setRatedTotalLightingPower(double ratedTotalLightingPower);

  bool setLightingSchedule(Schedule& schedule);

  void resetLightingSchedule();

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

  bool setRestockingSchedule(Schedule& schedule);

  void resetRestockingSchedule();

  void setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

  void resetAverageRefrigerantChargeInventory();

  bool setInsulatedFloorSurfaceArea(double insulatedFloorSurfaceArea);

  bool setInsulatedFloorUValue(double insulatedFloorUValue);

  void resetInsulatedFloorUValue();

  bool setDurationofDefrostCycle(int durationofDefrostCycle);

  void resetDurationofDefrostCycle();

  bool setDripDownTime(int dripDownTime);

  void resetDripDownTime();

  bool setDefrost1StartTime(const openstudio::Time& defrost1StartTime);

  void resetDefrost1StartTime();

  bool setDefrost2StartTime(const openstudio::Time& defrost2StartTime);

  void resetDefrost2StartTime();

  bool setDefrost3StartTime(const openstudio::Time& defrost3StartTime);

  void resetDefrost3StartTime();

  bool setDefrost4StartTime(const openstudio::Time& defrost4StartTime);

  void resetDefrost4StartTime();

  bool setDefrost5StartTime(const openstudio::Time& defrost5StartTime);

  void resetDefrost5StartTime();

  bool setDefrost6StartTime(const openstudio::Time& defrost6StartTime);

  void resetDefrost6StartTime();

  bool setDefrost7StartTime(const openstudio::Time& defrost7StartTime);

  void resetDefrost7StartTime();

  bool setDefrost8StartTime(const openstudio::Time& defrost8StartTime);

  void resetDefrost8StartTime();

  bool addToSystem(RefrigerationSystem & system);

  void removeFromSystem();

  bool setZoneBoundaryThermalZone(const ThermalZone& zoneBoundaryThermalZone);

  void resetZoneBoundaryThermalZone();

  bool setZoneBoundaryTotalInsulatedSurfaceAreaFacingZone(double zoneBoundaryTotalInsulatedSurfaceAreaFacingZone);

  void setZoneBoundaryAreaofGlassReachInDoorsFacingZone(double zoneBoundaryAreaofGlassReachInDoorsFacingZone);

  void setZoneBoundaryHeightofGlassReachInDoorsFacingZone(double zoneBoundaryHeightofGlassReachInDoorsFacingZone);

  void setZoneBoundaryAreaofStockingDoorsFacingZone(double zoneBoundaryAreaofStockingDoorsFacingZone);

  void setZoneBoundaryHeightofStockingDoorsFacingZone(double zoneBoundaryHeightofStockingDoorsFacingZone);

  bool setZoneBoundaryInsulatedSurfaceUValueFacingZone(double zoneBoundaryInsulatedSurfaceUValueFacingZone);

  bool setZoneBoundaryGlassReachInDoorUValueFacingZone(double zoneBoundaryGlassReachInDoorUValueFacingZone);

  bool setZoneBoundaryStockingDoorUValueFacingZone(double zoneBoundaryStockingDoorUValueFacingZone);

  bool setZoneBoundaryStockingDoorOpeningScheduleFacingZone(Schedule& zoneBoundaryStockingDoorOpeningScheduleFacingZone);

  void resetZoneBoundaryStockingDoorOpeningScheduleFacingZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationWalkIn_Impl ImplType;

  explicit RefrigerationWalkIn(std::shared_ptr<detail::RefrigerationWalkIn_Impl> impl);

  friend class detail::RefrigerationWalkIn_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationWalkIn");
};

/** \relates RefrigerationWalkIn*/
typedef boost::optional<RefrigerationWalkIn> OptionalRefrigerationWalkIn;

/** \relates RefrigerationWalkIn*/
typedef std::vector<RefrigerationWalkIn> RefrigerationWalkInVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONWALKIN_HPP

