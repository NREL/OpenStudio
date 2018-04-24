/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCASE_IMPL_HPP
#define MODEL_REFRIGERATIONCASE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {

class Time;

namespace model {

class Schedule;
class ThermalZone;
class CurveCubic;
class RefrigerationDefrostCycleParameters;

namespace detail {

  /** RefrigerationCase_Impl is a ParentObject_Impl that is the implementation class for RefrigerationCase.*/
  class MODEL_API RefrigerationCase_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationCase_Impl(const IdfObject& idfObject,
                           Model_Impl* model,
                           bool keepHandle);

    RefrigerationCase_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    RefrigerationCase_Impl(const RefrigerationCase_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~RefrigerationCase_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<IdfObject> remove() override;

    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    virtual std::vector<ModelObject> children() const override;

    //@}
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

    std::string unitType() const;

    bool isUnitTypeDefaulted() const;

    boost::optional<RefrigerationDefrostCycleParameters> optionalCaseDefrostCycleParameters() const;

    RefrigerationDefrostCycleParameters caseDefrostCycleParameters();

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

    std::vector<openstudio::Time> defrostStartTimes() const;

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

    bool setStandardCaseLightingPowerperUnitLength(double standardCaseLightingPowerperUnitLength);

    void resetStandardCaseLightingPowerperUnitLength();

    bool setInstalledCaseLightingPowerperUnitLength(boost::optional<double> installedCaseLightingPowerperUnitLength);

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

    bool setHumidityatZeroAntiSweatHeaterEnergy(double humidityatZeroAntiSweatHeaterEnergy);

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

    bool setDefrostEnergyCorrectionCurve(const boost::optional<CurveCubic>& curveCubic);

    void resetDefrostEnergyCorrectionCurve();

    bool setUnderCaseHVACReturnAirFraction(double underCaseHVACReturnAirFraction);

    void resetUnderCaseHVACReturnAirFraction();

    bool setRefrigeratedCaseRestockingSchedule(Schedule& schedule);

    void resetRefrigeratedCaseRestockingSchedule();

    bool setCaseCreditFractionSchedule(Schedule& schedule);

    void resetCaseCreditFractionSchedule();

    bool setDesignEvaporatorTemperatureorBrineInletTemperature(boost::optional<double> designEvaporatorTemperatureorBrineInletTemperature);

    void resetDesignEvaporatorTemperatureorBrineInletTemperature();

    bool setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

    void resetAverageRefrigerantChargeInventory();

    bool setNumberOfDoors(boost::optional<int> numberOfDoors);

    void resetNumberOfDoors();

    bool setRatedTotalCoolingCapacityperDoor(boost::optional<double> ratedTotalCoolingCapacityperDoor);

    void resetRatedTotalCoolingCapacityperDoor();

    bool setStandardCaseFanPowerperDoor(boost::optional<double> standardCaseFanPowerperDoor);

    void resetStandardCaseFanPowerperDoor();

    bool setOperatingCaseFanPowerperDoor(boost::optional<double> operatingCaseFanPowerperDoor);

    void resetOperatingCaseFanPowerperDoor();

    bool setStandardCaseLightingPowerperDoor(boost::optional<double> standardCaseLightingPowerperDoor);

    void resetStandardCaseLightingPowerperDoor();

    bool setInstalledCaseLightingPowerperDoor(boost::optional<double> installedCaseLightingPowerperDoor);

    void resetInstalledCaseLightingPowerperDoor();

    bool setCaseAntiSweatHeaterPowerperDoor(boost::optional<double> caseAntiSweatHeaterPowerperDoor);

    void resetCaseAntiSweatHeaterPowerperDoor();

    bool setMinimumAntiSweatHeaterPowerperDoor(boost::optional<double> minimumAntiSweatHeaterPowerperDoor);

    void resetMinimumAntiSweatHeaterPowerperDoor();

    bool setCaseDefrostPowerperDoor(boost::optional<double> caseDefrostPowerperDoor);

    void resetCaseDefrostPowerperDoor();

    bool setUnitType(std::string unitType);

    void resetUnitType();

    bool setCaseDefrostCycleParameters(const RefrigerationDefrostCycleParameters& caseDefrostCycleParameters);

    bool setDurationofDefrostCycle(boost::optional<int> durationofDefrostCycle);

    void resetDurationofDefrostCycle();

    bool setDripDownTime(boost::optional<int> dripDownTime);

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
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCase");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveCubic> optionalLatentCaseCreditCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCASE_IMPL_HPP
