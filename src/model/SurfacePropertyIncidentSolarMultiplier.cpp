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

#include "SurfacePropertyIncidentSolarMultiplier.hpp"
#include "SurfacePropertyIncidentSolarMultiplier_Impl.hpp"

#include "Model.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_IncidentSolarMultiplier_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SurfacePropertyIncidentSolarMultiplier_Impl::SurfacePropertyIncidentSolarMultiplier_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyIncidentSolarMultiplier::iddObjectType());
    }

    SurfacePropertyIncidentSolarMultiplier_Impl::SurfacePropertyIncidentSolarMultiplier_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfacePropertyIncidentSolarMultiplier::iddObjectType());
    }

    SurfacePropertyIncidentSolarMultiplier_Impl::SurfacePropertyIncidentSolarMultiplier_Impl(const SurfacePropertyIncidentSolarMultiplier_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SurfacePropertyIncidentSolarMultiplier_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType SurfacePropertyIncidentSolarMultiplier_Impl::iddObjectType() const {
      return SurfacePropertyIncidentSolarMultiplier::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SurfacePropertyIncidentSolarMultiplier_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName) != e) {
        result.emplace_back("SurfacePropertyIncidentSolarMultiplier", "Incident Solar Multiplier");
      }
      return result;
    }

    SubSurface SurfacePropertyIncidentSolarMultiplier_Impl::subSurface() const {
      boost::optional<SubSurface> value = optionalSubSurface();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Surface attached.");
      }
      return value.get();
    }

    double SurfacePropertyIncidentSolarMultiplier_Impl::incidentSolarMultiplier() const {
      boost::optional<double> value = getDouble(OS_SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> SurfacePropertyIncidentSolarMultiplier_Impl::incidentSolarMultiplierSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName);
    }

    bool SurfacePropertyIncidentSolarMultiplier_Impl::setSubSurface(const SubSurface& subSurface) {
      bool result = setPointer(OS_SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName, subSurface.handle());
      return result;
    }

    bool SurfacePropertyIncidentSolarMultiplier_Impl::setIncidentSolarMultiplier(double incidentSolarMultiplier) {
      bool result = setDouble(OS_SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplier, incidentSolarMultiplier);
      return result;
    }

    bool SurfacePropertyIncidentSolarMultiplier_Impl::setIncidentSolarMultiplierSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName,
                                "SurfacePropertyIncidentSolarMultiplier", "Incident Solar Multiplier", schedule);
      return result;
    }

    void SurfacePropertyIncidentSolarMultiplier_Impl::resetIncidentSolarMultiplierSchedule() {
      bool result = setString(OS_SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName, "");
      OS_ASSERT(result);
    }

    boost::optional<SubSurface> SurfacePropertyIncidentSolarMultiplier_Impl::optionalSubSurface() const {
      return getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName);
    }

  }  // namespace detail

  SurfacePropertyIncidentSolarMultiplier::SurfacePropertyIncidentSolarMultiplier(const SubSurface& subSurface)
    : ModelObject(SurfacePropertyIncidentSolarMultiplier::iddObjectType(), subSurface.model()) {
    OS_ASSERT(getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>());

    auto subSurfaceType = subSurface.subSurfaceType();
    if (!(istringEqual("FixedWindow", subSurfaceType) || istringEqual("OperableWindow", subSurfaceType)
          || istringEqual("Skylight", subSurfaceType))) {
      LOG_AND_THROW("Wrong SubSurface Type: only accepts windows, was passed " << subSurface.briefDescription());
    }
    bool ok = true;
    ok = setSubSurface(subSurface);
    OS_ASSERT(ok);

    // TODO: default to alwaysOnContinous? Or leave as optional?
    // {
    //   auto schedule = subSurface.model().alwaysOnContinuousSchedule();
    //   ok = setIncidentSolarMultiplierSchedule(schedule);
    //   OS_ASSERT(ok);
    // }

    ok = setIncidentSolarMultiplier(1.0);
    OS_ASSERT(ok);
  }

  IddObjectType SurfacePropertyIncidentSolarMultiplier::iddObjectType() {
    return {IddObjectType::OS_SurfaceProperty_IncidentSolarMultiplier};
  }

  SubSurface SurfacePropertyIncidentSolarMultiplier::subSurface() const {
    return getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->subSurface();
  }

  double SurfacePropertyIncidentSolarMultiplier::incidentSolarMultiplier() const {
    return getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->incidentSolarMultiplier();
  }

  boost::optional<Schedule> SurfacePropertyIncidentSolarMultiplier::incidentSolarMultiplierSchedule() const {
    return getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->incidentSolarMultiplierSchedule();
  }

  bool SurfacePropertyIncidentSolarMultiplier::setSubSurface(const SubSurface& subSurface) {
    return getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->setSubSurface(subSurface);
  }

  bool SurfacePropertyIncidentSolarMultiplier::setIncidentSolarMultiplier(double incidentSolarMultiplier) {
    return getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->setIncidentSolarMultiplier(incidentSolarMultiplier);
  }

  bool SurfacePropertyIncidentSolarMultiplier::setIncidentSolarMultiplierSchedule(Schedule& schedule) {
    return getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->setIncidentSolarMultiplierSchedule(schedule);
  }

  void SurfacePropertyIncidentSolarMultiplier::resetIncidentSolarMultiplierSchedule() {
    getImpl<detail::SurfacePropertyIncidentSolarMultiplier_Impl>()->resetIncidentSolarMultiplierSchedule();
  }

  /// @cond
  SurfacePropertyIncidentSolarMultiplier::SurfacePropertyIncidentSolarMultiplier(
    std::shared_ptr<detail::SurfacePropertyIncidentSolarMultiplier_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
