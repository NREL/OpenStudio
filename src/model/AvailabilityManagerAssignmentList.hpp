/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_HPP
#define MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class AvailabilityManagerAssignmentList_Impl;
    class AirLoopHVAC_Impl;
    class PlantLoop_Impl;

  }  // namespace detail

  class AvailabilityManager;
  class Loop;
  class AirLoopHVAC;
  class PlantLoop;
  class ZoneHVACComponent;

  /** AvailabilityManagerAssignmentList is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManagerAssignmentList'. */
  class MODEL_API AvailabilityManagerAssignmentList : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /*
   * Ctor needs a loop (AirLoopHVAC or PlantLoop). This shouldn't be used, the Loop Ctor will create one for you.
   * At least until we implement AVMLists for ZoneHVAC equipment
   */
    explicit AvailabilityManagerAssignmentList(const Loop& loop);

    virtual ~AvailabilityManagerAssignmentList() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerAssignmentList(const AvailabilityManagerAssignmentList& other) = default;
    AvailabilityManagerAssignmentList(AvailabilityManagerAssignmentList&& other) = default;
    AvailabilityManagerAssignmentList& operator=(const AvailabilityManagerAssignmentList&) = default;
    AvailabilityManagerAssignmentList& operator=(AvailabilityManagerAssignmentList&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /*
   * Get the priority of a given AvailabilityManager
   */
    unsigned availabilityManagerPriority(const AvailabilityManager& avm) const;

    /*
   * Return all AvailabilityManagers assigned to this list, in the priority order
   */
    std::vector<AvailabilityManager> availabilityManagers() const;

    /*
   * Returns an optional Loop (base class for AirLoopHVAC & PlantLoop) to which this availabilityManagerAssignementList is assigned
   */
    boost::optional<Loop> loop() const;

    /*
   * Returns an optional AirLoopHVAC to which this availabilityManagerAssignementList is assigned
   */
    boost::optional<AirLoopHVAC> airLoopHVAC() const;

    /*
   * Returns an optional PlantLoop to which this availabilityManagerAssignementList is assigned
   */
    boost::optional<PlantLoop> plantLoop() const;

    /*
   * Returns an optional HVAC Component  to which this availabilityManagerAssignementList is assigned
   * A number of other objects such as most of the ZoneHVAC stuff (ZoneHVAC:FourPipeFanCoil, ZoneHVAC:PackagedTerminalHeatPump, etc)
   * as well as the AirLoopHVAC:OutdoorAirSystem can have an AvailabilityManagerAssignmentList, but it isn't implemented in OS IDD yet
   */
    boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Setters */
    //@{

    /*
   * Add a new AvailabilityManager at the end of the list (priority = last).
   */
    bool addAvailabilityManager(const AvailabilityManager& avm);

    /*
   * Add a new AvailabilityManager to the list which a given priority (1 to x).
   * Internally calls addAvailabilityManager then setAvailabilityManagerPriority, see remarks there
   */
    bool addAvailabilityManager(const AvailabilityManager& avm, unsigned priority);

    /*
   * Set all availabilityManagers using a list of AvailabilityManagers
   */
    bool setAvailabilityManagers(const std::vector<AvailabilityManager>& avms);

    /*
   * Removes all AvailabilityManagers in this list
   */
    void resetAvailabilityManagers();

    /*
   * Remove the given AvailabilityManager from this AvailabilityManagerAssignmentList
   */
    bool removeAvailabilityManager(const AvailabilityManager& avm);

    /*
   * Remove the availabilityManager at the given priority
   */
    bool removeAvailabilityManager(unsigned priority);

    /*
   * You can shuffle the priority of a given AvailabilityManager after having added it
   * If priority is below 1, it's reset to 1.
   * If priority is greater than the number of availability managers, will reset to last
   */
    bool setAvailabilityManagerPriority(const AvailabilityManager& avm, unsigned priority);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerAssignmentList_Impl;

    explicit AvailabilityManagerAssignmentList(const Model& model);
    explicit AvailabilityManagerAssignmentList(std::shared_ptr<detail::AvailabilityManagerAssignmentList_Impl> impl);

    friend class detail::AvailabilityManagerAssignmentList_Impl;
    friend class Model;
    friend class IdfObject;
    friend class detail::AirLoopHVAC_Impl;
    friend class detail::PlantLoop_Impl;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerAssignmentList");
  };

  /** \relates AvailabilityManagerAssignmentList*/
  using OptionalAvailabilityManagerAssignmentList = boost::optional<AvailabilityManagerAssignmentList>;

  /** \relates AvailabilityManagerAssignmentList*/
  using AvailabilityManagerAssignmentListVector = std::vector<AvailabilityManagerAssignmentList>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERASSIGNMENTLIST_HPP
