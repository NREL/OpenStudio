/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include <model/ParentObject.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
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

  explicit RefrigerationCase(const Model& model, Schedule& caseLightingSchedule, Schedule& caseDefrostSchedule, Schedule& caseDefrostDripDownSchedule);

  virtual ~RefrigerationCase() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> latentCaseCreditCurveTypeValues();

  static std::vector<std::string> antiSweatHeaterControlTypeValues();

  static std::vector<std::string> caseDefrostTypeValues();

  static std::vector<std::string> defrostEnergyCorrectionCurveTypeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> availabilitySchedule() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
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

  // TODO: Check return type. From object lists, some candidates are: CurveCubic.
  CurveCubic latentCaseCreditCurve() const;

  double standardCaseFanPowerperUnitLength() const;

  bool isStandardCaseFanPowerperUnitLengthDefaulted() const;

  double operatingCaseFanPowerperUnitLength() const;

  bool isOperatingCaseFanPowerperUnitLengthDefaulted() const;

  double standardCaseLightingPowerperUnitLength() const;

  bool isStandardCaseLightingPowerperUnitLengthDefaulted() const;

  boost::optional<double> installedCaseLightingPowerperUnitLength() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
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

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> caseDefrostSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> caseDefrostDripDownSchedule() const;

  std::string defrostEnergyCorrectionCurveType() const;

  bool isDefrostEnergyCorrectionCurveTypeDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: CurveCubic.
  boost::optional<CurveCubic> defrostEnergyCorrectionCurve() const;

  double underCaseHVACReturnAirFraction() const;

  bool isUnderCaseHVACReturnAirFractionDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> refrigeratedCaseRestockingSchedule() const;

  // TODO: Check return type. From object lists, some candidates are: Schedule.
  boost::optional<Schedule> caseCreditFractionSchedule() const;

  boost::optional<double> designEvaporatorTemperatureorBrineInletTemperature() const;

  double averageRefrigerantChargeInventory() const;

  bool isAverageRefrigerantChargeInventoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
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

  // TODO: Check argument type. From object lists, some candidates are: CurveCubic.
  bool setLatentCaseCreditCurve(const CurveCubic& curveCubic);

  bool setStandardCaseFanPowerperUnitLength(double standardCaseFanPowerperUnitLength);

  void resetStandardCaseFanPowerperUnitLength();

  bool setOperatingCaseFanPowerperUnitLength(double operatingCaseFanPowerperUnitLength);

  void resetOperatingCaseFanPowerperUnitLength();

  void setStandardCaseLightingPowerperUnitLength(double standardCaseLightingPowerperUnitLength);

  void resetStandardCaseLightingPowerperUnitLength();

  void setInstalledCaseLightingPowerperUnitLength(double installedCaseLightingPowerperUnitLength);

  void resetInstalledCaseLightingPowerperUnitLength();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
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

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setCaseDefrostSchedule(Schedule& schedule);

  void resetCaseDefrostSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setCaseDefrostDripDownSchedule(Schedule& schedule);

  void resetCaseDefrostDripDownSchedule();

  bool setDefrostEnergyCorrectionCurveType(std::string defrostEnergyCorrectionCurveType);

  void resetDefrostEnergyCorrectionCurveType();

  // TODO: Check argument type. From object lists, some candidates are: CurveCubic.
  bool setDefrostEnergyCorrectionCurve(const CurveCubic& curveCubic);

  void resetDefrostEnergyCorrectionCurve();

  bool setUnderCaseHVACReturnAirFraction(double underCaseHVACReturnAirFraction);

  void resetUnderCaseHVACReturnAirFraction();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setRefrigeratedCaseRestockingSchedule(Schedule& schedule);

  void resetRefrigeratedCaseRestockingSchedule();

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setCaseCreditFractionSchedule(Schedule& schedule);

  void resetCaseCreditFractionSchedule();

  bool setDesignEvaporatorTemperatureorBrineInletTemperature(double designEvaporatorTemperatureorBrineInletTemperature);

  void resetDesignEvaporatorTemperatureorBrineInletTemperature();

  void setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

  void resetAverageRefrigerantChargeInventory();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationCase_Impl ImplType;

  explicit RefrigerationCase(boost::shared_ptr<detail::RefrigerationCase_Impl> impl);

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

