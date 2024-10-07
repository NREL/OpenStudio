/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HOTWATEREQUIPMENT_HPP
#define MODEL_HOTWATEREQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HotWaterEquipmentDefinition;

  namespace detail {

    class HotWaterEquipment_Impl;

  }  // namespace detail

  /** HotWaterEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:HotWaterEquipment'. Its fields are derived from the EnergyPlus IDD object
 *  'HotWaterEquipment'. \sa HotWaterEquipmentDefinition
 */
  class MODEL_API HotWaterEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HotWaterEquipment(const HotWaterEquipmentDefinition& hotWaterEquipmentDefinition);

    virtual ~HotWaterEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HotWaterEquipment(const HotWaterEquipment& other) = default;
    HotWaterEquipment(HotWaterEquipment&& other) = default;
    HotWaterEquipment& operator=(const HotWaterEquipment&) = default;
    HotWaterEquipment& operator=(HotWaterEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the HotWaterEquipmentDefinition object. */
    HotWaterEquipmentDefinition hotWaterEquipmentDefinition() const;

    /** Returns the (fractional) equipment schedule.  If this object does not
   *  specify a schedule this function will search the hierarchy. */
    boost::optional<Schedule> schedule() const;

    /** Returns true if this object does not specify a schedule directly. */
    bool isScheduleDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the HotWaterEquipmentDefinition object. */
    bool setHotWaterEquipmentDefinition(const HotWaterEquipmentDefinition& definition);

    /** Sets the (fractional) Schedule. */
    bool setSchedule(Schedule& schedule);

    /** Resets the (fractional) Schedule. */
    void resetSchedule();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    /** Returns the design level represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getDesignLevel(double floorArea, double numPeople) const;

    /** Returns the watts/m^2 represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the watts/person represented by this instance, assuming floorArea (m^2) and
   *  numPeople. */
    double getPowerPerPerson(double floorArea, double numPeople) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HotWaterEquipment_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit HotWaterEquipment(std::shared_ptr<detail::HotWaterEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HotWaterEquipment");
  };

  /** \relates HotWaterEquipment*/
  using OptionalHotWaterEquipment = boost::optional<HotWaterEquipment>;

  /** \relates HotWaterEquipment*/
  using HotWaterEquipmentVector = std::vector<HotWaterEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HOTWATEREQUIPMENT_HPP
