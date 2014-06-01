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

#ifndef MODEL_REFRIGERATIONCASE_HPP
#define MODEL_REFRIGERATIONCASE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "RefrigerationSystem.hpp"

namespace openstudio {

class Time;

namespace model {

class Schedule;
class ThermalZone;
class CurveCubic;

namespace detail {

  class RefrigerationCase_Impl;

} // detail

/** RefrigerationCase is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Case'. */
class MODEL_API RefrigerationCase : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationCase(const Model& model, Schedule& caseDefrostSchedule);

  virtual ~RefrigerationCase() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> latentCaseCreditCurveTypeValues();

  static std::vector<std::string> antiSweatHeaterControlTypeValues();

  static std::vector<std::string> caseDefrostTypeValues();

  static std::vector<std::string> defrostEnergyCorrectionCurveTypeValues();

  static std::vector<std::string> unitTypeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<ThermalZone> thermalZone() const;

  double ratedAmbientTemperature() const;

  bool isRatedAmbientTemperatureDefaulted() const;

  double ratedAmbientRelativeHumidity() const;

  bool isRatedAmbientRelativeHumidityDefaulted() const;

  double ratedTotalCoolingCapacityperUnitLength() const;

  bool isRatedTotalCoolingCapacityperUnitLengthDefaulted() const;

  double ratedLatentHeatRatio() const;

  bool isRatedLatentHeatRatioDefaulted() const;

  double ratedRuntimeFraction() const;

  bool isRatedRuntimeFractionDefaulted() const;

  double caseLength() const;

  bool isCaseLengthDefaulted() const;

  double caseOperatingTemperature() const;

  bool isCaseOperatingTemperatureDefaulted() const;

  std::string latentCaseCreditCurveType() const;

  bool isLatentCaseCreditCurveTypeDefaulted() const;

  CurveCubic latentCaseCreditCurve() const;

  double standardCaseFanPowerperUnitLength() const;

  bool isStandardCaseFanPowerperUnitLengthDefaulted() const;

  double operatingCaseFanPowerperUnitLength() const;

  bool isOperatingCaseFanPowerperUnitLengthDefaulted() const;

  double standardCaseLightingPowerperUnitLength() const;

  bool isStandardCaseLightingPowerperUnitLengthDefaulted() const;

  boost::optional<double> installedCaseLightingPowerperUnitLength() const;

  boost::optional<Schedule> caseLightingSchedule() const;

  double fractionofLightingEnergytoCase() const;

  bool isFractionofLightingEnergytoCaseDefaulted() const;

  double caseAntiSweatHeaterPowerperUnitLength() const;

  bool isCaseAntiSweatHeaterPowerperUnitLengthDefaulted() const;

  double minimumAntiSweatHeaterPowerperUnitLength() const;

  bool isMinimumAntiSweatHeaterPowerperUnitLengthDefaulted() const;

  std::string antiSweatHeaterControlType() const;

  bool isAntiSweatHeaterControlTypeDefaulted() const;

  double humidityatZeroAntiSweatHeaterEnergy() const;

  bool isHumidityatZeroAntiSweatHeaterEnergyDefaulted() const;

  double caseHeight() const;

  bool isCaseHeightDefaulted() const;

  double fractionofAntiSweatHeaterEnergytoCase() const;

  bool isFractionofAntiSweatHeaterEnergytoCaseDefaulted() const;

  double caseDefrostPowerperUnitLength() const;

  bool isCaseDefrostPowerperUnitLengthDefaulted() const;

  std::string caseDefrostType() const;

  bool isCaseDefrostTypeDefaulted() const;

  boost::optional<Schedule> caseDefrostSchedule() const;

  boost::optional<Schedule> caseDefrostDripDownSchedule() const;

  std::string defrostEnergyCorrectionCurveType() const;

  bool isDefrostEnergyCorrectionCurveTypeDefaulted() const;

  boost::optional<CurveCubic> defrostEnergyCorrectionCurve() const;

  double underCaseHVACReturnAirFraction() const;

  bool isUnderCaseHVACReturnAirFractionDefaulted() const;

  boost::optional<Schedule> refrigeratedCaseRestockingSchedule() const;

  boost::optional<Schedule> caseCreditFractionSchedule() const;

  boost::optional<double> designEvaporatorTemperatureorBrineInletTemperature() const;

  double averageRefrigerantChargeInventory() const;

  bool isAverageRefrigerantChargeInventoryDefaulted() const;

  boost::optional<int> numberOfDoors() const;

  boost::optional<double> ratedTotalCoolingCapacityperDoor() const;

  boost::optional<double> standardCaseFanPowerperDoor() const;

  boost::optional<double> operatingCaseFanPowerperDoor() const;

  boost::optional<double> standardCaseLightingPowerperDoor() const;

  boost::optional<double> installedCaseLightingPowerperDoor() const;

  boost::optional<double> caseAntiSweatHeaterPowerperDoor() const;

  boost::optional<double> minimumAntiSweatHeaterPowerperDoor() const;

  boost::optional<double> caseDefrostPowerperDoor() const;

  /** Read the method for defining loads.
  * Options are UnitLength and NumberOfDoors.  If UnitLength values will be used 
  * from the *perUnitLength methods.  If NumberOfDoors, the values will be used
  * from the *perDoor methods.
  */
  std::string unitType() const;

  bool isUnitTypeDefaulted() const;

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

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setThermalZone(const ThermalZone& thermalZone);

  void resetThermalZone();

  bool setRatedAmbientTemperature(double ratedAmbientTemperature);

  void resetRatedAmbientTemperature();

  bool setRatedAmbientRelativeHumidity(double ratedAmbientRelativeHumidity);

  void resetRatedAmbientRelativeHumidity();

  bool setRatedTotalCoolingCapacityperUnitLength(double ratedTotalCoolingCapacityperUnitLength);

  void resetRatedTotalCoolingCapacityperUnitLength();

  bool setRatedLatentHeatRatio(double ratedLatentHeatRatio);

  void resetRatedLatentHeatRatio();

  bool setRatedRuntimeFraction(double ratedRuntimeFraction);

  void resetRatedRuntimeFraction();

  bool setCaseLength(double caseLength);

  void resetCaseLength();

  bool setCaseOperatingTemperature(double caseOperatingTemperature);

  void resetCaseOperatingTemperature();

  bool setLatentCaseCreditCurveType(std::string latentCaseCreditCurveType);

  void resetLatentCaseCreditCurveType();

  bool setLatentCaseCreditCurve(const CurveCubic& curveCubic);

  bool setStandardCaseFanPowerperUnitLength(double standardCaseFanPowerperUnitLength);

  void resetStandardCaseFanPowerperUnitLength();

  bool setOperatingCaseFanPowerperUnitLength(double operatingCaseFanPowerperUnitLength);

  void resetOperatingCaseFanPowerperUnitLength();

  void setStandardCaseLightingPowerperUnitLength(double standardCaseLightingPowerperUnitLength);

  void resetStandardCaseLightingPowerperUnitLength();

  void setInstalledCaseLightingPowerperUnitLength(double installedCaseLightingPowerperUnitLength);

  void resetInstalledCaseLightingPowerperUnitLength();

  bool setCaseLightingSchedule(Schedule& schedule);

  void resetCaseLightingSchedule();

  bool setFractionofLightingEnergytoCase(double fractionofLightingEnergytoCase);

  void resetFractionofLightingEnergytoCase();

  bool setCaseAntiSweatHeaterPowerperUnitLength(double caseAntiSweatHeaterPowerperUnitLength);

  void resetCaseAntiSweatHeaterPowerperUnitLength();

  bool setMinimumAntiSweatHeaterPowerperUnitLength(double minimumAntiSweatHeaterPowerperUnitLength);

  void resetMinimumAntiSweatHeaterPowerperUnitLength();

  bool setAntiSweatHeaterControlType(std::string antiSweatHeaterControlType);

  void resetAntiSweatHeaterControlType();

  void setHumidityatZeroAntiSweatHeaterEnergy(double humidityatZeroAntiSweatHeaterEnergy);

  void resetHumidityatZeroAntiSweatHeaterEnergy();

  bool setCaseHeight(double caseHeight);

  void resetCaseHeight();

  bool setFractionofAntiSweatHeaterEnergytoCase(double fractionofAntiSweatHeaterEnergytoCase);

  void resetFractionofAntiSweatHeaterEnergytoCase();

  bool setCaseDefrostPowerperUnitLength(double caseDefrostPowerperUnitLength);

  void resetCaseDefrostPowerperUnitLength();

  bool setCaseDefrostType(std::string caseDefrostType);

  void resetCaseDefrostType();

  bool setCaseDefrostSchedule(Schedule& schedule);

  void resetCaseDefrostSchedule();

  bool setCaseDefrostDripDownSchedule(Schedule& schedule);

  void resetCaseDefrostDripDownSchedule();

  bool setDefrostEnergyCorrectionCurveType(std::string defrostEnergyCorrectionCurveType);

  void resetDefrostEnergyCorrectionCurveType();

  bool setDefrostEnergyCorrectionCurve(const CurveCubic& curveCubic);

  void resetDefrostEnergyCorrectionCurve();

  bool setUnderCaseHVACReturnAirFraction(double underCaseHVACReturnAirFraction);

  void resetUnderCaseHVACReturnAirFraction();

  bool setRefrigeratedCaseRestockingSchedule(Schedule& schedule);

  void resetRefrigeratedCaseRestockingSchedule();

  bool setCaseCreditFractionSchedule(Schedule& schedule);

  void resetCaseCreditFractionSchedule();

  bool setDesignEvaporatorTemperatureorBrineInletTemperature(double designEvaporatorTemperatureorBrineInletTemperature);

  void resetDesignEvaporatorTemperatureorBrineInletTemperature();

  void setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

  void resetAverageRefrigerantChargeInventory();

  bool setNumberOfDoors(int numberOfDoors);

  void resetNumberOfDoors();

  bool setRatedTotalCoolingCapacityperDoor(double ratedTotalCoolingCapacityperDoor);

  void resetRatedTotalCoolingCapacityperDoor();

  bool setStandardCaseFanPowerperDoor(double standardCaseFanPowerperDoor);

  void resetStandardCaseFanPowerperDoor();

  bool setOperatingCaseFanPowerperDoor(double operatingCaseFanPowerperDoor);

  void resetOperatingCaseFanPowerperDoor();

  bool setStandardCaseLightingPowerperDoor(double standardCaseLightingPowerperDoor);

  void resetStandardCaseLightingPowerperDoor();

  bool setInstalledCaseLightingPowerperDoor(double installedCaseLightingPowerperDoor);

  void resetInstalledCaseLightingPowerperDoor();

  bool setCaseAntiSweatHeaterPowerperDoor(double caseAntiSweatHeaterPowerperDoor);

  void resetCaseAntiSweatHeaterPowerperDoor();

  bool setMinimumAntiSweatHeaterPowerperDoor(double minimumAntiSweatHeaterPowerperDoor);

  void resetMinimumAntiSweatHeaterPowerperDoor();

  bool setCaseDefrostPowerperDoor(double caseDefrostPowerperDoor);

  void resetCaseDefrostPowerperDoor();

  /** Set the method for defining loads.
  * Options are UnitLength and NumberOfDoors.  If UnitLength values will be used 
  * from the *perUnitLength methods.  If NumberOfDoors, the values will be used
  * from the *perDoor methods.
  */
  bool setUnitType(std::string unitType);

  /** Reset unit type to the default, "UnitLength" **/
  void resetUnitType();

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

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationCase_Impl ImplType;

  explicit RefrigerationCase(std::shared_ptr<detail::RefrigerationCase_Impl> impl);

  friend class detail::RefrigerationCase_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationCase");
};

/** \relates RefrigerationCase*/
typedef boost::optional<RefrigerationCase> OptionalRefrigerationCase;

/** \relates RefrigerationCase*/
typedef std::vector<RefrigerationCase> RefrigerationCaseVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCASE_HPP

