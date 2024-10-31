/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACEQUIPMENTLIST_HPP
#define MODEL_ZONEHVACEQUIPMENTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ZoneHVACEquipmentList_Impl;

  }  // namespace detail

  class ThermalZone;

  /** ZoneHVACEquipmentList is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EquipmentList'. */
  class MODEL_API ZoneHVACEquipmentList : public ModelObject
  {
   public:
    explicit ZoneHVACEquipmentList(const ThermalZone& thermalZone);

    virtual ~ZoneHVACEquipmentList() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACEquipmentList(const ZoneHVACEquipmentList& other) = default;
    ZoneHVACEquipmentList(ZoneHVACEquipmentList&& other) = default;
    ZoneHVACEquipmentList& operator=(const ZoneHVACEquipmentList&) = default;
    ZoneHVACEquipmentList& operator=(ZoneHVACEquipmentList&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> loadDistributionSchemeValues();

    static std::vector<std::string> validLoadDistributionSchemeValues();

    std::string loadDistributionScheme() const;

    bool setLoadDistributionScheme(const std::string& scheme);

    /** Add new equipment setting the heating and cooling priorities
   *  to the next available priority level.
   *  Air terminals associated with AirLoopHVAC will be moved to first priority.
   *  This method is relatively dumb.  It will add any model object to the list
   *  even if it is not hvac equipment.  That might change in the future.
   */
    bool addEquipment(const ModelObject& equipment);

    /** Remove equipment from the EquipmentList.
    * This will not remove the equipment from the model or
    * disconnect any node connections.  Use only if you know what you are doing.
    */
    bool removeEquipment(const ModelObject& equipment);

    /** Set cooling priority of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList
   */
    bool setCoolingPriority(const ModelObject& equipment, unsigned priority);

    /** Set heating or no-load priority of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList
   */
    bool setHeatingPriority(const ModelObject& equipment, unsigned priority);

    /** Return all equipment.  Order is undetermined. */
    std::vector<ModelObject> equipment() const;

    /** Return all equipment.  Order is determined by heating priority.
   *  Equipment having a Heating Priority of 0 is filtered out (unavailable)
   */
    std::vector<ModelObject> equipmentInHeatingOrder() const;

    /** Return all equipment.  Order is determined by cooling priority
   *  Equipment having a Cooling Priority of 0 is filtered out (unavailable)
   */
    std::vector<ModelObject> equipmentInCoolingOrder() const;

    ThermalZone thermalZone() const;

    // TODO: JM 2019-02-25 These should actually return a boost::optional<unsigned> in case the equipment isn't actually part of the list...
    /** Return the heating or no-load priority for equipment */
    unsigned heatingPriority(const ModelObject& equipment) const;

    /** Return the cooling priority for equipment */
    unsigned coolingPriority(const ModelObject& equipment) const;

    /** Return the Sequential Cooling Fraction of equipment.
   *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    OS_DEPRECATED(2, 9, 0) boost::optional<double> sequentialCoolingFraction(const ModelObject& equipment) const;

    /** Return the Sequential Cooling Fraction Schedule of equipment.
   *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    boost::optional<Schedule> sequentialCoolingFractionSchedule(const ModelObject& equipment) const;

    /** Return the Sequential Heating Fraction of equipment.
   *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its cooling priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    OS_DEPRECATED(2, 9, 0) boost::optional<double> sequentialHeatingFraction(const ModelObject& equipment) const;

    /** Return the Sequential Heating Fraction Schedule of equipment.
   *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its cooling priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    boost::optional<Schedule> sequentialHeatingFractionSchedule(const ModelObject& equipment) const;

    /** Set the Sequential Cooling Fraction Schedule Constant of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList, its cooling priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    OS_DEPRECATED(2, 9, 0) bool setSequentialCoolingFraction(const ModelObject& equipment, double fraction);

    /** Set the Sequential Cooling Fraction Schedule of equipment.
   *  Returns false when equipement is not in the ZoneHVACEquipmentList, its cooling priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    bool setSequentialCoolingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

    /** Set the Sequential Heating Fraction Schedule Constant of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    OS_DEPRECATED(2, 9, 0) bool setSequentialHeatingFraction(const ModelObject& equipment, double fraction);

    /** Set the Sequential Heating Fraction Schedule of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
   *  or the loadDistributionScheme isn't 'Sequential'
   */
    bool setSequentialHeatingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

   protected:
    /// @cond

    using ImplType = detail::ZoneHVACEquipmentList_Impl;

    explicit ZoneHVACEquipmentList(std::shared_ptr<detail::ZoneHVACEquipmentList_Impl> impl);

    friend class detail::ZoneHVACEquipmentList_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEquipmentList");
  };

  /** \relates ZoneHVACEquipmentList*/
  using OptionalZoneHVACEquipmentList = boost::optional<ZoneHVACEquipmentList>;

  /** \relates ZoneHVACEquipmentList*/
  using ZoneHVACEquipmentListVector = std::vector<ZoneHVACEquipmentList>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACEQUIPMENTLIST_HPP
