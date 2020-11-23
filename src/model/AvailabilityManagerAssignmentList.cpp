/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"

#include "AvailabilityManager_Impl.hpp"
#include "AvailabilityManager.hpp"

#include "Loop_Impl.hpp"
#include "Loop.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "PlantLoop_Impl.hpp"
#include "PlantLoop.hpp"
#include "Model.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

// for the containingZoneHVACComponent method
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerAssignmentList_Impl::AvailabilityManagerAssignmentList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerAssignmentList::iddObjectType());
    }

    AvailabilityManagerAssignmentList_Impl::AvailabilityManagerAssignmentList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerAssignmentList::iddObjectType());
    }

    AvailabilityManagerAssignmentList_Impl::AvailabilityManagerAssignmentList_Impl(const AvailabilityManagerAssignmentList_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerAssignmentList_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output
      return result;
    }

    IddObjectType AvailabilityManagerAssignmentList_Impl::iddObjectType() const {
      return AvailabilityManagerAssignmentList::iddObjectType();
    }

    /*
   * TODO: Clone all the AVMs
   * The Ctor gets a loop though... Perhaps should have a clone(Loop loop) method instead?
   * It's meant to be called in the Loop.clone method anwyays...
   * TODO: This will need to change once ZOneHVAC equipment can have an AVMList too...
   */
    ModelObject AvailabilityManagerAssignmentList_Impl::clone(Model model) const {
      // Create a new, blank, one
      AvailabilityManagerAssignmentList avmListClone(model);

      // Clone all AVMs and populate the clone
      std::vector<AvailabilityManager> avmVector = availabilityManagers();
      for (const AvailabilityManager& avm : avmVector) {
        AvailabilityManager avmClone = avm.clone(model).cast<AvailabilityManager>();
        bool ok = avmListClone.addAvailabilityManager(avmClone);
        OS_ASSERT(ok);
      }
      return avmListClone;
    }

    unsigned AvailabilityManagerAssignmentList_Impl::availabilityManagerPriority(const AvailabilityManager& avm) const {

      // TODO: perhaps I chould explicitly check that the avm is
      // 1. in the same model
      // 2. in the avm list
      // In order to LOG a helpful message to the user.
      boost::optional<unsigned> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      int i = 1;

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo =
          group.cast<WorkspaceExtensibleGroup>().getTarget(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName);

        OS_ASSERT(wo);

        if (wo->handle() == avm.handle()) {
          result = i;
          break;
        }
        i++;
      }

      if (!result.is_initialized()) {
        LOG(Warn, avm.briefDescription() << " wasn't found in " << this->briefDescription());
        // Should we return 0 for eg?
      }

      OS_ASSERT(result);

      return result.get();
    }

    std::vector<AvailabilityManager> AvailabilityManagerAssignmentList_Impl::availabilityManagers() const {
      std::vector<AvailabilityManager> result;

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();

      for (const auto& group : groups) {
        boost::optional<WorkspaceObject> wo =
          group.cast<WorkspaceExtensibleGroup>().getTarget(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName);

        if (wo) {
          result.push_back(wo->cast<AvailabilityManager>());
        }
      }

      return result;
    }

    bool AvailabilityManagerAssignmentList_Impl::addAvailabilityManager(const AvailabilityManager& avm) {
      WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

      bool ok = eg.setPointer(OS_AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName, avm.handle());

      return ok;
    }

    bool AvailabilityManagerAssignmentList_Impl::addAvailabilityManager(const AvailabilityManager& avm, unsigned priority) {
      bool ok = addAvailabilityManager(avm);
      OS_ASSERT(ok);
      ok = setAvailabilityManagerPriority(avm, priority);
      return ok;
    }

    bool AvailabilityManagerAssignmentList_Impl::setAvailabilityManagerPriority(const AvailabilityManager& avm, unsigned priority) {
      std::vector<AvailabilityManager> avmVector = availabilityManagers();

      OS_ASSERT(std::find(avmVector.begin(), avmVector.end(), avm) != avmVector.end());

      if (priority > avmVector.size()) priority = avmVector.size();
      if (priority < 1) priority = 1;

      avmVector.erase(std::find(avmVector.begin(), avmVector.end(), avm));

      avmVector.insert(avmVector.begin() + (priority - 1), avm);

      // Clear the extensible groups, and redo them
      clearExtensibleGroups();
      for (const AvailabilityManager& a : avmVector) {
        bool ok = addAvailabilityManager(a);
        OS_ASSERT(ok);
      }
      return true;
    }

    bool AvailabilityManagerAssignmentList_Impl::setAvailabilityManagers(const std::vector<AvailabilityManager>& avms) {
      // Clear the extensible groups, and redo them
      clearExtensibleGroups();
      for (const AvailabilityManager& a : avms) {
        bool ok = addAvailabilityManager(a);
        OS_ASSERT(ok);
      }
      return true;
    }

    void AvailabilityManagerAssignmentList_Impl::resetAvailabilityManagers() {
      clearExtensibleGroups();
    }

    bool AvailabilityManagerAssignmentList_Impl::removeAvailabilityManager(const AvailabilityManager& avm) {

      std::vector<AvailabilityManager> avmVector = availabilityManagers();

      OS_ASSERT(std::find(avmVector.begin(), avmVector.end(), avm) != avmVector.end());

      avmVector.erase(std::find(avmVector.begin(), avmVector.end(), avm));

      // Clear the extensible groups, and redo them
      clearExtensibleGroups();
      for (const AvailabilityManager& a : avmVector) {
        bool ok = addAvailabilityManager(a);
        OS_ASSERT(ok);
      }
      return true;
    }

    bool AvailabilityManagerAssignmentList_Impl::removeAvailabilityManager(const unsigned priority) {

      std::vector<AvailabilityManager> avmVector = availabilityManagers();

      if ((priority > avmVector.size()) || (priority < 1)) {
        LOG(Warn, "Priority of '" << priority << " is out of bounds for " << briefDescription());
        return false;
      }

      // Clear the extensible groups, and redo them
      clearExtensibleGroups();
      // priority starts at one

      unsigned i = 1;
      bool result = false;

      for (const AvailabilityManager& a : avmVector) {
        if (i == priority) {
          result = true;
        } else {
          bool ok = addAvailabilityManager(a);
          OS_ASSERT(ok);
        }
        ++i;
      }
      return result;
    }

    boost::optional<AirLoopHVAC> AvailabilityManagerAssignmentList_Impl::airLoopHVAC() const {
      std::vector<AirLoopHVAC> airLoops = this->model().getConcreteModelObjects<AirLoopHVAC>();

      for (auto& airLoop : airLoops) {
        // Call the Impl one (there is no public)
        AvailabilityManagerAssignmentList avmList = airLoop.getImpl<detail::AirLoopHVAC_Impl>()->availabilityManagerAssignmentList();

        if (avmList.handle() == this->handle()) {
          return airLoop;
        }
      }
      return boost::none;
    }

    boost::optional<PlantLoop> AvailabilityManagerAssignmentList_Impl::plantLoop() const {
      boost::optional<PlantLoop> plantLoop;
      // try with getModelObjectSources instead
      std::vector<PlantLoop> plantLoops = getObject<ModelObject>().getModelObjectSources<PlantLoop>(PlantLoop::iddObjectType());

      if (plantLoops.size() > 0u) {
        if (plantLoops.size() > 1u) {
          LOG(Error, briefDescription() << " is referenced by more than one plantLoop, returning the first");
        }
        plantLoop = plantLoops[0];
      }
      /*
 *    std::vector<PlantLoop> plantLoops = this->model().getConcreteModelObjects<PlantLoop>();
 *
 *    for( auto & plantLoop : plantLoops)
 *    {
 *      // Call the Impl one (there is no public)
 *      AvailabilityManagerAssignmentList avmList = plantLoop.getImpl<detail::PlantLoop_Impl>()->availabilityManagerAssignmentList();
 *      if (avmList.handle() == this->handle())
 *      {
 *        return plantLoop;
 *      }
 *    }
 */
      return plantLoop;
    }

    boost::optional<Loop> AvailabilityManagerAssignmentList_Impl::loop() const {
      if (boost::optional<AirLoopHVAC> airLoopHVAC = this->airLoopHVAC()) {
        return airLoopHVAC->optionalCast<Loop>();
      } else if (boost::optional<PlantLoop> plantLoop = this->plantLoop()) {
        return plantLoop->optionalCast<Loop>();
      } else {
        return boost::none;
      }
    }

    boost::optional<ZoneHVACComponent> AvailabilityManagerAssignmentList_Impl::containingZoneHVACComponent() const {

      boost::optional<ZoneHVACComponent> result;

      // E+ V8-8-0 IDD, list of objects that reference SystemAvailabilityManagerLists
      // ZoneHVAC:FourPipeFanCoil
      std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils =
        getObject<ModelObject>().getModelObjectSources<ZoneHVACFourPipeFanCoil>(ZoneHVACFourPipeFanCoil::iddObjectType());

      if (zoneHVACFourPipeFanCoils.size() > 0u) {
        if (zoneHVACFourPipeFanCoils.size() > 1u) {
          LOG(Error, briefDescription() << " is referenced by more than one zoneHVACFourPipeFanCoils, returning the first");
        }
        return zoneHVACFourPipeFanCoils[0];
      }

      /*
     *for( const auto & zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils )
     *{
     *  if( boost::optional<HVACComponent> coil = zoneHVACFourPipeFanCoil.supplyAirFan() )
     *  {
     *    if( coil->handle() == this->handle() )
     *    {
     *      return zoneHVACFourPipeFanCoil;
     *    }
     *  }
     *}
     */

      // ZoneHVAC:WindowAirConditioner
      // ZoneHVAC:PackagedTerminalAirConditioner
      // ZoneHVAC:PackagedTerminalHeatPump
      // ZoneHVAC:WaterToAirHeatPump
      // ZoneHVAC:EnergyRecoveryVentilator
      // ZoneHVAC:UnitVentilator
      // ZoneHVAC:UnitHeater
      // ZoneHVAC:EvaporativeCoolerUnit
      // ZoneHVAC:OutdoorAirUnit
      // ZoneHVAC:TerminalUnit:VariableRefrigerantFlow
      // ZoneHVAC:VentilatedSlab

      // For this to work, would need ZoneHVACComponent to have a static iddObjectType defined
      /*
 *    std::vector<ZoneHVACComponent> zoneHVACComps = getObject<ModelObject>().getModelObjectSources<ZoneHVACComponent>(ZoneHVACComponent::iddObjectType());
 *
 *    if( zoneHVACComps.size() > 0u) {
 *      if( zoneHVACComps.size() > 1u) {
 *        LOG(Error, briefDescription() << " is referenced by more than one ZoneHVACComponent, returning the first");
 *      }
 *      result = zoneHVACComps[0];
 *    }
 */

      return result;
    }

  }  // namespace detail

  /*
 * Ctor, accepts Loop as an input parameter (for now that's the only thing that accepts an AVMlist in OS, but ZoneHVAC stuff should too)
 */
  AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(const Loop& loop)
    : ModelObject(AvailabilityManagerAssignmentList::iddObjectType(), loop.model()) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerAssignmentList_Impl>());

    this->setName(loop.name().get() + " AvailabilityManagerAssignmentList");
  }

  AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(const Model& model)
    : ModelObject(AvailabilityManagerAssignmentList::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerAssignmentList_Impl>());
  }

  IddObjectType AvailabilityManagerAssignmentList::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AvailabilityManagerAssignmentList);
  }

  std::vector<AvailabilityManager> AvailabilityManagerAssignmentList::availabilityManagers() const {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->availabilityManagers();
  }

  bool AvailabilityManagerAssignmentList::setAvailabilityManagers(const std::vector<AvailabilityManager>& avms) {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->setAvailabilityManagers(avms);
  }

  void AvailabilityManagerAssignmentList::resetAvailabilityManagers() {
    getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->resetAvailabilityManagers();
  }

  bool AvailabilityManagerAssignmentList::addAvailabilityManager(const AvailabilityManager& avm) {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->addAvailabilityManager(avm);
  }
  bool AvailabilityManagerAssignmentList::addAvailabilityManager(const AvailabilityManager& avm, unsigned priority) {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->addAvailabilityManager(avm, priority);
  }

  bool AvailabilityManagerAssignmentList::removeAvailabilityManager(const AvailabilityManager& avm) {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->removeAvailabilityManager(avm);
  }
  bool AvailabilityManagerAssignmentList::removeAvailabilityManager(unsigned priority) {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->removeAvailabilityManager(priority);
  }

  unsigned AvailabilityManagerAssignmentList::availabilityManagerPriority(const AvailabilityManager& avm) const {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->availabilityManagerPriority(avm);
  }
  bool AvailabilityManagerAssignmentList::setAvailabilityManagerPriority(const AvailabilityManager& avm, unsigned priority) {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->setAvailabilityManagerPriority(avm, priority);
  }

  boost::optional<Loop> AvailabilityManagerAssignmentList::loop() const {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->loop();
  }
  boost::optional<AirLoopHVAC> AvailabilityManagerAssignmentList::airLoopHVAC() const {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->airLoopHVAC();
  }
  boost::optional<PlantLoop> AvailabilityManagerAssignmentList::plantLoop() const {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->plantLoop();
  }
  boost::optional<ZoneHVACComponent> AvailabilityManagerAssignmentList::containingZoneHVACComponent() const {
    return getImpl<detail::AvailabilityManagerAssignmentList_Impl>()->containingZoneHVACComponent();
  }

  /// @cond
  AvailabilityManagerAssignmentList::AvailabilityManagerAssignmentList(std::shared_ptr<detail::AvailabilityManagerAssignmentList_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
