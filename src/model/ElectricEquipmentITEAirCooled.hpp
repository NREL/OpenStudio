/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_HPP
#define MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {

namespace model {

  class ElectricEquipmentITEAirCooledDefinition;
  class Curve;
  class Schedule;

  namespace detail {

    class ElectricEquipmentITEAirCooled_Impl;

  }  // namespace detail

  /** ElectricEquipmentITEAirCooled is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS:ElectricEquipment:ITE:AirCooled'. */
  /* Its fields are derived from the EnergyPlus IDD object 'ElectricEquipmentITEAirCooled'. \sa ElectricEquipmentITEAirCooledDefinition */
  class MODEL_API ElectricEquipmentITEAirCooled : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricEquipmentITEAirCooled(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition);

    virtual ~ElectricEquipmentITEAirCooled() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricEquipmentITEAirCooled(const ElectricEquipmentITEAirCooled& other) = default;
    ElectricEquipmentITEAirCooled(ElectricEquipmentITEAirCooled&& other) = default;
    ElectricEquipmentITEAirCooled& operator=(const ElectricEquipmentITEAirCooled&) = default;
    ElectricEquipmentITEAirCooled& operator=(ElectricEquipmentITEAirCooled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ElectricEquipmentITEAirCooledDefinition electricEquipmentITEAirCooledDefinition() const;

    // Return the Design Power Input Schedule. If not specified, default to always 1.
    boost::optional<Schedule> designPowerInputSchedule() const;

    bool isDesignPowerInputScheduleDefaulted() const;

    // Return the CPU Loading Schedule. If not specified, default to always 1.
    boost::optional<Schedule> cPULoadingSchedule() const;

    bool isCPULoadingScheduleDefaulted() const;

    std::string cPUEndUseSubcategory() const;

    bool isCPUEndUseSubcategoryDefaulted() const;

    std::string fanEndUseSubcategory() const;

    bool isFanEndUseSubcategoryDefaulted() const;

    std::string electricPowerSupplyEndUseSubcategory() const;

    bool isElectricPowerSupplyEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setElectricEquipmentITEAirCooledDefinition(const ElectricEquipmentITEAirCooledDefinition& electricEquipmentITEAirCooledDefinition);

    // Note Schedules are passed by reference, not const reference.
    bool setDesignPowerInputSchedule(Schedule& schedule);

    void resetDesignPowerInputSchedule();

    // Note Schedules are passed by reference, not const reference.
    bool setCPULoadingSchedule(Schedule& schedule);

    void resetCPULoadingSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setCPUEndUseSubcategory(const std::string& cPUEndUseSubcategory);

    void resetCPUEndUseSubcategory();

    bool setFanEndUseSubcategory(const std::string& fanEndUseSubcategory);

    void resetFanEndUseSubcategory();

    bool setElectricPowerSupplyEndUseSubcategory(const std::string& electricPowerSupplyEndUseSubcategory);

    void resetElectricPowerSupplyEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> wattsperUnit() const;

    boost::optional<double> wattsperZoneFloorArea() const;

    double getWattsperUnit(double floorArea) const;

    double getWattsperZoneFloorArea(double floorArea) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricEquipmentITEAirCooled_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ElectricEquipmentITEAirCooled(std::shared_ptr<detail::ElectricEquipmentITEAirCooled_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricEquipmentITEAirCooled");
  };

  /** \relates ElectricEquipmentITEAirCooled*/
  using OptionalElectricEquipmentITEAirCooled = boost::optional<ElectricEquipmentITEAirCooled>;

  /** \relates ElectricEquipmentITEAirCooled*/
  using ElectricEquipmentITEAirCooledVector = std::vector<ElectricEquipmentITEAirCooled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICEQUIPMENTITEAIRCOOLED_HPP
