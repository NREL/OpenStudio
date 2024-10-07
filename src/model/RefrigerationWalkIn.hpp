/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONWALKIN_HPP
#define MODEL_REFRIGERATIONWALKIN_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "RefrigerationSystem.hpp"
#include "RefrigerationSecondarySystem.hpp"
#include "RefrigerationCompressorRack.hpp"
#include "ThermalZone.hpp"

namespace openstudio {

class Time;

namespace model {

  class Schedule;
  class RefrigerationWalkInZoneBoundary;
  class RefrigerationDefrostCycleParameters;

  namespace detail {

    class RefrigerationWalkIn_Impl;

  }  // namespace detail

  /** RefrigerationWalkIn is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:WalkIn'. */
  class MODEL_API RefrigerationWalkIn : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationWalkIn(const Model& model, Schedule& walkinDefrostSchedule);

    virtual ~RefrigerationWalkIn() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationWalkIn(const RefrigerationWalkIn& other) = default;
    RefrigerationWalkIn(RefrigerationWalkIn&& other) = default;
    RefrigerationWalkIn& operator=(const RefrigerationWalkIn&) = default;
    RefrigerationWalkIn& operator=(RefrigerationWalkIn&&) = default;

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

    // Returns the parent RefrigerationSystem if any
    boost::optional<RefrigerationSystem> system() const;

    // Returns the parent RefrigerationSecondarySystem if any
    boost::optional<RefrigerationSecondarySystem> secondarySystem() const;

    // Returns the parent RefrigerationCompresorRack if any
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

    bool setDefrostPower(double defrostPower);

    void resetDefrostPower();

    bool setTemperatureTerminationDefrostFractiontoIce(double temperatureTerminationDefrostFractiontoIce);

    void resetTemperatureTerminationDefrostFractiontoIce();

    bool setRestockingSchedule(Schedule& schedule);

    void resetRestockingSchedule();

    bool setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

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

    bool addToSecondarySystem(RefrigerationSecondarySystem& secondarySystem);

    bool addToCompressorRack(RefrigerationCompressorRack& compressorRack);

    // Remove from parent system if any
    void removeFromSystem();

    // Remove from parent secondary system if any
    void removeFromSecondarySystem();

    // Remove from parent compressor rack if any
    void removeFromCompressorRack();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationWalkIn_Impl;

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
  using OptionalRefrigerationWalkIn = boost::optional<RefrigerationWalkIn>;

  /** \relates RefrigerationWalkIn*/
  using RefrigerationWalkInVector = std::vector<RefrigerationWalkIn>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONWALKIN_HPP
