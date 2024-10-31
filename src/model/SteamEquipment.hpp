/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STEAMEQUIPMENT_HPP
#define MODEL_STEAMEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class SteamEquipmentDefinition;
  class Schedule;

  namespace detail {

    class SteamEquipment_Impl;

  }  // namespace detail

  /** SteamEquipment is a SpaceLoadInstance that wraps the OpenStudio IDD object
 *  'OS:SteamEquipment'. \sa SteamEquipmentDefinition
 */
  class MODEL_API SteamEquipment : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SteamEquipment(const SteamEquipmentDefinition& definition);

    virtual ~SteamEquipment() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SteamEquipment(const SteamEquipment& other) = default;
    SteamEquipment(SteamEquipment&& other) = default;
    SteamEquipment& operator=(const SteamEquipment&) = default;
    SteamEquipment& operator=(SteamEquipment&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    SteamEquipmentDefinition steamEquipmentDefinition() const;

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

    bool setSteamEquipmentDefinition(const SteamEquipmentDefinition& definition);

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
    using ImplType = detail::SteamEquipment_Impl;

    friend class detail::SteamEquipment_Impl;
    friend class openstudio::detail::IdfObject_Impl;
    friend class IdfObject;
    friend class Model;

    explicit SteamEquipment(std::shared_ptr<detail::SteamEquipment_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SteamEquipment");
  };

  /** \relates SteamEquipment*/
  using OptionalSteamEquipment = boost::optional<SteamEquipment>;

  /** \relates SteamEquipment*/
  using SteamEquipmentVector = std::vector<SteamEquipment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STEAMEQUIPMENT_HPP
