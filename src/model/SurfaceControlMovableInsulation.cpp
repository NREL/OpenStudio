/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
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
    return {IddObjectType::OS_SurfaceControl_MovableInsulation};
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
