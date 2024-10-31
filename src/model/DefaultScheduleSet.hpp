/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DEFAULTSCHEDULESET_HPP
#define MODEL_DEFAULTSCHEDULESET_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

#include "../utilities/core/Enum.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class DefaultScheduleSet_Impl;

  }  // namespace detail

  // clang-format off

  OPENSTUDIO_ENUM(DefaultScheduleType,
    ((HoursofOperationSchedule)(Hours of Operation Schedule)(1))
    ((NumberofPeopleSchedule)(Number of People Schedule)(2))
    ((PeopleActivityLevelSchedule)(People Activity Level Schedule)(3))
    ((LightingSchedule)(Lighting Schedule)(4))
    ((ElectricEquipmentSchedule)(Electric Equipment Schedule)(5))
    ((GasEquipmentSchedule)(Gas Equipment Schedule)(6))
    ((HotWaterEquipmentSchedule)(Hot Water Equipment Schedule)(7))
    ((InfiltrationSchedule)(Infiltration Schedule)(8))
    ((SteamEquipmentSchedule)(Steam Equipment Schedule)(9))
    ((OtherEquipmentSchedule)(Other Equipment Schedule)(10))
    ((DesignPowerInputScheduleName)(Design Power Input Schedule)(11))
    ((CPULoadingScheduleName)(CPU Loading Schedule)(12))
  );

  // clang-format on

  /** DefaultScheduleSet is a ResourceObject that wraps the OpenStudio IDD object
 *  'OS:DefaultScheduleSet'. */
  class MODEL_API DefaultScheduleSet : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DefaultScheduleSet(const Model& model);

    virtual ~DefaultScheduleSet() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DefaultScheduleSet(const DefaultScheduleSet& other) = default;
    DefaultScheduleSet(DefaultScheduleSet&& other) = default;
    DefaultScheduleSet& operator=(const DefaultScheduleSet&) = default;
    DefaultScheduleSet& operator=(DefaultScheduleSet&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> hoursofOperationSchedule() const;

    boost::optional<Schedule> numberofPeopleSchedule() const;

    boost::optional<Schedule> peopleActivityLevelSchedule() const;

    boost::optional<Schedule> lightingSchedule() const;

    boost::optional<Schedule> electricEquipmentSchedule() const;

    boost::optional<Schedule> gasEquipmentSchedule() const;

    boost::optional<Schedule> hotWaterEquipmentSchedule() const;

    boost::optional<Schedule> infiltrationSchedule() const;

    boost::optional<Schedule> steamEquipmentSchedule() const;

    boost::optional<Schedule> otherEquipmentSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHoursofOperationSchedule(Schedule& schedule);

    void resetHoursofOperationSchedule();

    bool setNumberofPeopleSchedule(Schedule& schedule);

    void resetNumberofPeopleSchedule();

    bool setPeopleActivityLevelSchedule(Schedule& schedule);

    void resetPeopleActivityLevelSchedule();

    bool setLightingSchedule(Schedule& schedule);

    void resetLightingSchedule();

    bool setElectricEquipmentSchedule(Schedule& schedule);

    void resetElectricEquipmentSchedule();

    bool setGasEquipmentSchedule(Schedule& schedule);

    void resetGasEquipmentSchedule();

    bool setHotWaterEquipmentSchedule(Schedule& schedule);

    void resetHotWaterEquipmentSchedule();

    bool setInfiltrationSchedule(Schedule& schedule);

    void resetInfiltrationSchedule();

    bool setSteamEquipmentSchedule(Schedule& schedule);

    void resetSteamEquipmentSchedule();

    bool setOtherEquipmentSchedule(Schedule& schedule);

    void resetOtherEquipmentSchedule();

    //@}
    /** @name Other */
    //@{

    /// Returns the default schedule of a particular type.
    boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultScheduleSet& other);

    //@}
   protected:
    /// @cond
    using ImplType = detail::DefaultScheduleSet_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit DefaultScheduleSet(std::shared_ptr<detail::DefaultScheduleSet_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DefaultScheduleSet");
  };

  /** \relates DefaultScheduleSet*/
  using OptionalDefaultScheduleSet = boost::optional<DefaultScheduleSet>;

  /** \relates DefaultScheduleSet*/
  using DefaultScheduleSetVector = std::vector<DefaultScheduleSet>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSCHEDULESET_HPP
