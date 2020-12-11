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

#include "SurfaceControlMovableInsulation.hpp"
#include "SurfaceControlMovableInsulation_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "Material.hpp"
#include "Material_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceControl_MovableInsulation_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SurfaceControlMovableInsulation_Impl::SurfaceControlMovableInsulation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfaceControlMovableInsulation::iddObjectType());
    }

    SurfaceControlMovableInsulation_Impl::SurfaceControlMovableInsulation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfaceControlMovableInsulation::iddObjectType());
    }

    SurfaceControlMovableInsulation_Impl::SurfaceControlMovableInsulation_Impl(const SurfaceControlMovableInsulation_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    ModelObject SurfaceControlMovableInsulation_Impl::clone(Model model) const {
      ModelObject result = ModelObject_Impl::clone(model);

      Model m = this->model();
      if (model == m) {
        // cloned into same model, erase reference to parent
        // this object is now invalid but having two objects point to same surface would also be invalid
        result.setString(OS_SurfaceControl_MovableInsulationFields::SurfaceName, "");
        LOG(Warn, "Cloning the SurfaceControlMoveableInsulation resets the Surface attached to it while it is a required field. "
                  "You should call `setSurface(Surface&)` on the clone");
      }

      return result;
    }

    const std::vector<std::string>& SurfaceControlMovableInsulation_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      return result;
    }

    IddObjectType SurfaceControlMovableInsulation_Impl::iddObjectType() const {
      return SurfaceControlMovableInsulation::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SurfaceControlMovableInsulation_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_SurfaceControl_MovableInsulationFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("SurfaceControlMovableInsulation", "Resistance Modifier Fraction"));
      }
      return result;
    }

    std::string SurfaceControlMovableInsulation_Impl::insulationType() const {
      boost::optional<std::string> insulationType = getString(OS_SurfaceControl_MovableInsulationFields::InsulationType, true, true);
      OS_ASSERT(insulationType);
      return insulationType.get();
    }

    Surface SurfaceControlMovableInsulation_Impl::surface() const {
      boost::optional<Surface> surface =
        getObject<ModelObject>().getModelObjectTarget<Surface>(OS_SurfaceControl_MovableInsulationFields::SurfaceName);
      OS_ASSERT(surface);
      return surface.get();
    }

    Material SurfaceControlMovableInsulation_Impl::material() const {
      boost::optional<Material> material =
        getObject<ModelObject>().getModelObjectTarget<Material>(OS_SurfaceControl_MovableInsulationFields::MaterialName);
      OS_ASSERT(material);
      return material.get();
    }

    Schedule SurfaceControlMovableInsulation_Impl::schedule() const {
      boost::optional<Schedule> schedule =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceControl_MovableInsulationFields::ScheduleName);
      OS_ASSERT(schedule);
      return schedule.get();
    }

    bool SurfaceControlMovableInsulation_Impl::setInsulationType(const std::string& insulationType) {
      bool result = setString(OS_SurfaceControl_MovableInsulationFields::InsulationType, insulationType);
      return result;
    }

    bool SurfaceControlMovableInsulation_Impl::setSurface(const Surface& surface) {
      boost::optional<SurfaceControlMovableInsulation> currentIns = surface.surfaceControlMovableInsulation();
      if (currentIns) {
        if (currentIns->handle() == this->handle()) {
          return true;
        }
        currentIns->remove();
      }
      return setPointer(OS_SurfaceControl_MovableInsulationFields::SurfaceName, surface.handle());
    }

    bool SurfaceControlMovableInsulation_Impl::setMaterial(const Material& material) {
      return setPointer(OS_SurfaceControl_MovableInsulationFields::MaterialName, material.handle());
    }

    bool SurfaceControlMovableInsulation_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_SurfaceControl_MovableInsulationFields::ScheduleName, "SurfaceControlMovableInsulation",
                                                  "Resistance Modifier Fraction", schedule);
      return result;
    }

  }  // namespace detail

  SurfaceControlMovableInsulation::SurfaceControlMovableInsulation(const Surface& surface, const Material& material)
    : ModelObject(SurfaceControlMovableInsulation::iddObjectType(), surface.model()) {
    OS_ASSERT(getImpl<detail::SurfaceControlMovableInsulation_Impl>());

    setInsulationType("Outside");

    setSurface(surface);

    setMaterial(material);

    {
      auto schedule = surface.model().alwaysOnContinuousSchedule();
      setSchedule(schedule);
    }
  }

  IddObjectType SurfaceControlMovableInsulation::iddObjectType() {
    return IddObjectType(IddObjectType::OS_SurfaceControl_MovableInsulation);
  }

  std::vector<std::string> SurfaceControlMovableInsulation::insulationTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SurfaceControl_MovableInsulationFields::InsulationType);
  }

  std::vector<std::string> SurfaceControlMovableInsulation::validInsulationTypeValues() {
    return SurfaceControlMovableInsulation::insulationTypeValues();
  }

  std::string SurfaceControlMovableInsulation::insulationType() const {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->insulationType();
  }

  Surface SurfaceControlMovableInsulation::surface() const {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->surface();
  }

  Material SurfaceControlMovableInsulation::material() const {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->material();
  }

  Schedule SurfaceControlMovableInsulation::schedule() const {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->schedule();
  }

  bool SurfaceControlMovableInsulation::setInsulationType(const std::string& insulationType) {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->setInsulationType(insulationType);
  }

  bool SurfaceControlMovableInsulation::setSurface(const Surface& surface) {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->setSurface(surface);
  }

  bool SurfaceControlMovableInsulation::setMaterial(const Material& material) {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->setMaterial(material);
  }

  bool SurfaceControlMovableInsulation::setSchedule(Schedule& schedule) {
    return getImpl<detail::SurfaceControlMovableInsulation_Impl>()->setSchedule(schedule);
  }

  /// @cond
  SurfaceControlMovableInsulation::SurfaceControlMovableInsulation(std::shared_ptr<detail::SurfaceControlMovableInsulation_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
