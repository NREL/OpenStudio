/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirLoopHVAC_DedicatedOutdoorAirSystem_FieldEnums.hxx>

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACDedicatedOutdoorAirSystem_Impl::AirLoopHVACDedicatedOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                         bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACDedicatedOutdoorAirSystem::iddObjectType());
    }

    AirLoopHVACDedicatedOutdoorAirSystem_Impl::AirLoopHVACDedicatedOutdoorAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACDedicatedOutdoorAirSystem::iddObjectType());
    }

    AirLoopHVACDedicatedOutdoorAirSystem_Impl::AirLoopHVACDedicatedOutdoorAirSystem_Impl(const AirLoopHVACDedicatedOutdoorAirSystem_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    ModelObject AirLoopHVACDedicatedOutdoorAirSystem_Impl::clone(Model model) const {
      ModelObject result = ModelObject_Impl::clone(model);

      Model m = this->model();
      if (model == m) {
        // cloned into same model, erase reference to parent
        // this object is now invalid but having two objects point to same outdoor air system would also be invalid
        result.setString(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::OutdoorAirSystem, "");
        LOG(Warn, "Cloning the AirLoopHVACDedicatedOutdoorAirSystem resets the Outdoor Air System attached to it while it is a required field. "
                  "You should call `setOutdoorAirSystem(OutdoorAirSystem&)` on the clone");
      }

      return result;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& AirLoopHVACDedicatedOutdoorAirSystem_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not Appropriate: No variables available
      return result;
    }

    IddObjectType AirLoopHVACDedicatedOutdoorAirSystem_Impl::iddObjectType() const {
      return AirLoopHVACDedicatedOutdoorAirSystem::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirLoopHVACDedicatedOutdoorAirSystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACDedicatedOutdoorAirSystem", "Availability Schedule"));
      }
      return result;
    }

    AirLoopHVACOutdoorAirSystem AirLoopHVACDedicatedOutdoorAirSystem_Impl::airLoopHVACOutdoorAirSystem() const {
      boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem =
        getObject<ModelObject>().getModelObjectTarget<AirLoopHVACOutdoorAirSystem>(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::OutdoorAirSystem);
      OS_ASSERT(oaSystem);
      return oaSystem.get();
    }

    Schedule AirLoopHVACDedicatedOutdoorAirSystem_Impl::availabilitySchedule() const {
      boost::optional<Schedule> schedule =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilitySchedule);
      OS_ASSERT(schedule);
      return schedule.get();
    }

    double AirLoopHVACDedicatedOutdoorAirSystem_Impl::preheatDesignTemperature() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACDedicatedOutdoorAirSystem_Impl::preheatDesignHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACDedicatedOutdoorAirSystem_Impl::precoolDesignTemperature() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACDedicatedOutdoorAirSystem_Impl::precoolDesignHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    unsigned int AirLoopHVACDedicatedOutdoorAirSystem_Impl::numberofAirLoops() const {
      return numExtensibleGroups();
    }

    std::vector<AirLoopHVAC> AirLoopHVACDedicatedOutdoorAirSystem_Impl::airLoops() const {
      std::vector<AirLoopHVAC> result;
      auto groups = extensibleGroups();
      for (const auto& group : groups) {
        auto target = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoop);
        if (target) {
          if (auto airLoop = target->optionalCast<AirLoopHVAC>()) {
            result.push_back(airLoop.get());
          }
        }
      }
      return result;
    }

    boost::optional<unsigned> AirLoopHVACDedicatedOutdoorAirSystem_Impl::airLoopIndex(const AirLoopHVAC& airLoopHVAC) const {

      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto h = openstudio::toString(airLoopHVAC.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoop).get() == h);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it) + 1;
      }

      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::setAirLoopHVACOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& airLoopHVACOutdoorAirSystem) {
      boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> currentDOAS = airLoopHVACOutdoorAirSystem.airLoopHVACDedicatedOutdoorAirSystem();
      if (currentDOAS) {
        if (currentDOAS->handle() == this->handle()) {
          return true;
        }
        currentDOAS->remove();
      }
      return setPointer(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::OutdoorAirSystem, airLoopHVACOutdoorAirSystem.handle());
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilitySchedule, "AirLoopHVACDedicatedOutdoorAirSystem",
                                "Availability Schedule", schedule);
      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::setPreheatDesignTemperature(double preheatDesignTemperature) {
      bool result = setDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature, preheatDesignTemperature);
      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio) {
      bool result = setDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio, preheatDesignHumidityRatio);
      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::setPrecoolDesignTemperature(double precoolDesignTemperature) {
      bool result = setDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature, precoolDesignTemperature);
      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio) {
      bool result = setDouble(OS_AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignHumidityRatio, precoolDesignHumidityRatio);
      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::addAirLoop(const AirLoopHVAC& airLoopHVAC) {
      // Check if airLoopHVAC already has a doas
      if (boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> doas = airLoopHVAC.airLoopHVACDedicatedOutdoorAirSystem()) {
        LOG(Warn, "For " << airLoopHVAC.nameString() << ", " << briefDescription() << " already exists.");
        return true;
      }

      // Check if airLoopHVAC already exists
      boost::optional<unsigned> _existingIndex = airLoopIndex(airLoopHVAC);
      if (_existingIndex) {
        LOG(Warn, "For " << briefDescription() << ", AirLoopHVAC already exists.");
        return true;
      }

      bool result;

      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool airloop = eg.setPointer(OS_AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoop, airLoopHVAC.handle());
      if (airloop) {
        result = true;
      } else {
        // Something went wrong
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      return result;
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::removeAirLoop(const AirLoopHVAC& airLoopHVAC) {
      boost::optional<unsigned> idx = airLoopIndex(airLoopHVAC);
      if (!idx) {
        LOG(Warn, "For " << briefDescription() << " cannot remove air loop " << airLoopHVAC.briefDescription() << " since it is not part of it.");
        return false;
      }

      return removeAirLoop(idx.get());
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::removeAirLoop(unsigned groupIndex) {
      bool result = false;
      if ((groupIndex > 0) && (groupIndex <= numberofAirLoops())) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex - 1);
        result = true;
      }
      return result;
    }

    void AirLoopHVACDedicatedOutdoorAirSystem_Impl::removeAllAirLoops() {
      clearExtensibleGroups();
    }

    std::vector<openstudio::IdfObject> AirLoopHVACDedicatedOutdoorAirSystem_Impl::remove() {

      airLoopHVACOutdoorAirSystem().remove();

      return ModelObject_Impl::remove();
    }

    bool AirLoopHVACDedicatedOutdoorAirSystem_Impl::addAirLoops(const std::vector<AirLoopHVAC>& airLoopHVACs) {
      bool ok = true;
      for (const AirLoopHVAC& airLoopHVAC : airLoopHVACs) {
        ok &= addAirLoop(airLoopHVAC);
      }
      return ok;
    }

  }  // namespace detail

  AirLoopHVACDedicatedOutdoorAirSystem::AirLoopHVACDedicatedOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& airLoopHVACOutdoorAirSystem)
    : ModelObject(AirLoopHVACDedicatedOutdoorAirSystem::iddObjectType(), airLoopHVACOutdoorAirSystem.model()) {
    OS_ASSERT(getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>());

    setAirLoopHVACOutdoorAirSystem(airLoopHVACOutdoorAirSystem);
    {
      auto schedule = airLoopHVACOutdoorAirSystem.model().alwaysOnDiscreteSchedule();
      setAvailabilitySchedule(schedule);
    }
    setPreheatDesignTemperature(4.5);
    setPreheatDesignHumidityRatio(0.004);
    setPrecoolDesignTemperature(17.5);
    setPrecoolDesignHumidityRatio(0.012);
  }

  IddObjectType AirLoopHVACDedicatedOutdoorAirSystem::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_DedicatedOutdoorAirSystem};
  }

  AirLoopHVACOutdoorAirSystem AirLoopHVACDedicatedOutdoorAirSystem::airLoopHVACOutdoorAirSystem() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->airLoopHVACOutdoorAirSystem();
  }

  Schedule AirLoopHVACDedicatedOutdoorAirSystem::availabilitySchedule() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->availabilitySchedule();
  }

  double AirLoopHVACDedicatedOutdoorAirSystem::preheatDesignTemperature() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->preheatDesignTemperature();
  }

  double AirLoopHVACDedicatedOutdoorAirSystem::preheatDesignHumidityRatio() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->preheatDesignHumidityRatio();
  }

  double AirLoopHVACDedicatedOutdoorAirSystem::precoolDesignTemperature() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->precoolDesignTemperature();
  }

  double AirLoopHVACDedicatedOutdoorAirSystem::precoolDesignHumidityRatio() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->precoolDesignHumidityRatio();
  }

  unsigned int AirLoopHVACDedicatedOutdoorAirSystem::numberofAirLoops() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->numberofAirLoops();
  }

  std::vector<AirLoopHVAC> AirLoopHVACDedicatedOutdoorAirSystem::airLoops() const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->airLoops();
  }

  boost::optional<unsigned> AirLoopHVACDedicatedOutdoorAirSystem::airLoopIndex(const AirLoopHVAC& airLoopHVAC) const {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->airLoopIndex(airLoopHVAC);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::setAirLoopHVACOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& airLoopHVACOutdoorAirSystem) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->setAirLoopHVACOutdoorAirSystem(airLoopHVACOutdoorAirSystem);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::setPreheatDesignTemperature(double preheatDesignTemperature) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->setPreheatDesignTemperature(preheatDesignTemperature);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->setPreheatDesignHumidityRatio(preheatDesignHumidityRatio);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::setPrecoolDesignTemperature(double precoolDesignTemperature) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->setPrecoolDesignTemperature(precoolDesignTemperature);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->setPrecoolDesignHumidityRatio(precoolDesignHumidityRatio);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::addAirLoop(const AirLoopHVAC& airLoopHVAC) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->addAirLoop(airLoopHVAC);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::removeAirLoop(const AirLoopHVAC& airLoopHVAC) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->removeAirLoop(airLoopHVAC);
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::removeAirLoop(unsigned groupIndex) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->removeAirLoop(groupIndex);
  }

  void AirLoopHVACDedicatedOutdoorAirSystem::removeAllAirLoops() {
    getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->removeAllAirLoops();
  }

  bool AirLoopHVACDedicatedOutdoorAirSystem::addAirLoops(const std::vector<AirLoopHVAC>& airLoopHVACs) {
    return getImpl<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl>()->addAirLoops(airLoopHVACs);
  }

  /// @cond
  AirLoopHVACDedicatedOutdoorAirSystem::AirLoopHVACDedicatedOutdoorAirSystem(std::shared_ptr<detail::AirLoopHVACDedicatedOutdoorAirSystem_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
