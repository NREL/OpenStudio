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

    virtual ~AvailabilityManagerAssignmentList() = default;
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
