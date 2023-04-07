/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_REFRIGERATIONWALKIN_IMPL_HPP
#define MODEL_REFRIGERATIONWALKIN_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "RefrigerationWalkInZoneBoundary.hpp"
#include "ThermalZone.hpp"

namespace openstudio {

class Time;

namespace model {

  class Schedule;
  class RefrigerationSystem;
  class RefrigerationSecondarySystem;
  class RefrigerationCompressorRack;
  class RefrigerationWalkInZoneBoundary;
  class RefrigerationDefrostCycleParameters;

  namespace detail {

    /** RefrigerationWalkIn_Impl is a ModelObject_Impl that is the implementation class for RefrigerationWalkIn.*/
    class MODEL_API RefrigerationWalkIn_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationWalkIn_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationWalkIn_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationWalkIn_Impl(const RefrigerationWalkIn_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationWalkIn_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      bool addZoneBoundary(const RefrigerationWalkInZoneBoundary& refrigerationWalkInZoneBoundary);

      void removeZoneBoundary(unsigned index);

      void removeAllZoneBoundaries();

      std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries() const;

      //@}
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

      boost::optional<RefrigerationDefrostCycleParameters> optionalWalkinDefrostCycleParameters() const;

      RefrigerationDefrostCycleParameters walkinDefrostCycleParameters();

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

      boost::optional<RefrigerationSystem> system() const;

      boost::optional<RefrigerationSecondarySystem> secondarySystem() const;

      boost::optional<RefrigerationCompressorRack> compressorRack() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setRatedCoilCoolingCapacity(double ratedCoilCoolingCapacity);

      bool setOperatingTemperature(double operatingTemperature);

      bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

      bool setRatedTotalHeatingPower(double ratedTotalHeatingPower);

      bool setHeatingPowerSchedule(Schedule& schedule);

      void resetHeatingPowerSchedule();

      bool setRatedCoolingCoilFanPower(double ratedCoolingCoilFanPower);

      void resetRatedCoolingCoilFanPower();

      bool setRatedCirculationFanPower(double ratedCirculationFanPower);

      void resetRatedCirculationFanPower();

      bool setRatedTotalLightingPower(double ratedTotalLightingPower);

      bool setLightingSchedule(Schedule& schedule);

      void resetLightingSchedule();

      bool setDefrostType(const std::string& defrostType);

      void resetDefrostType();

      bool setDefrostControlType(const std::string& defrostControlType);

      void resetDefrostControlType();

      bool setDefrostSchedule(Schedule& schedule);

      bool setDefrostDripDownSchedule(Schedule& schedule);

      void resetDefrostDripDownSchedule();

      bool setDefrostPower(boost::optional<double> defrostPower);

      void resetDefrostPower();

      bool setTemperatureTerminationDefrostFractiontoIce(boost::optional<double> temperatureTerminationDefrostFractiontoIce);

      void resetTemperatureTerminationDefrostFractiontoIce();

      bool setRestockingSchedule(Schedule& schedule);

      void resetRestockingSchedule();

      bool setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

      void resetAverageRefrigerantChargeInventory();

      bool setInsulatedFloorSurfaceArea(double insulatedFloorSurfaceArea);

      bool setInsulatedFloorUValue(double insulatedFloorUValue);

      void resetInsulatedFloorUValue();

      bool setWalkinDefrostCycleParameters(const RefrigerationDefrostCycleParameters& walkinDefrostCycleParameters);

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

      bool setZoneBoundaryThermalZone(const ThermalZone& zoneBoundaryThermalZone);

      void resetZoneBoundaryThermalZone();

      bool setZoneBoundaryTotalInsulatedSurfaceAreaFacingZone(double zoneBoundaryTotalInsulatedSurfaceAreaFacingZone);

      bool setZoneBoundaryAreaofGlassReachInDoorsFacingZone(double zoneBoundaryAreaofGlassReachInDoorsFacingZone);

      bool setZoneBoundaryHeightofGlassReachInDoorsFacingZone(double zoneBoundaryHeightofGlassReachInDoorsFacingZone);

      bool setZoneBoundaryAreaofStockingDoorsFacingZone(double zoneBoundaryAreaofStockingDoorsFacingZone);

      bool setZoneBoundaryHeightofStockingDoorsFacingZone(double zoneBoundaryHeightofStockingDoorsFacingZone);

      bool setZoneBoundaryInsulatedSurfaceUValueFacingZone(double zoneBoundaryInsulatedSurfaceUValueFacingZone);

      bool setZoneBoundaryGlassReachInDoorUValueFacingZone(double zoneBoundaryGlassReachInDoorUValueFacingZone);

      bool setZoneBoundaryStockingDoorUValueFacingZone(double zoneBoundaryStockingDoorUValueFacingZone);

      bool setZoneBoundaryStockingDoorOpeningScheduleFacingZone(Schedule& zoneBoundaryStockingDoorOpeningScheduleFacingZone);

      void resetZoneBoundaryStockingDoorOpeningScheduleFacingZone();

      bool addToSystem(RefrigerationSystem& system);

      void removeFromSystem();

      bool addToSecondarySystem(RefrigerationSecondarySystem& secondarySystem);

      void removeFromSecondarySystem();

      bool addToCompressorRack(RefrigerationCompressorRack& compressorRack);

      void removeFromCompressorRack();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationWalkIn");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalDefrostSchedule() const;

      RefrigerationWalkInZoneBoundary frontZoneBoundary();

      boost::optional<RefrigerationWalkInZoneBoundary> zoneBoundary() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONWALKIN_IMPL_HPP
