/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SurfacePropertyLocalEnvironment.hpp"
#include "SurfacePropertyLocalEnvironment_Impl.hpp"

#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"

#include "SurfacePropertySurroundingSurfaces.hpp"
#include "SurfacePropertySurroundingSurfaces_Impl.hpp"
#include "SurfacePropertyGroundSurfaces.hpp"
#include "SurfacePropertyGroundSurfaces_Impl.hpp"
//#include "OutdoorAirNode.hpp"
// #include "OutdoorAirNode_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_LocalEnvironment_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SurfacePropertyLocalEnvironment_Impl::SurfacePropertyLocalEnvironment_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyLocalEnvironment::iddObjectType());
    }

    SurfacePropertyLocalEnvironment_Impl::SurfacePropertyLocalEnvironment_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfacePropertyLocalEnvironment::iddObjectType());
    }

    SurfacePropertyLocalEnvironment_Impl::SurfacePropertyLocalEnvironment_Impl(const SurfacePropertyLocalEnvironment_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SurfacePropertyLocalEnvironment_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType SurfacePropertyLocalEnvironment_Impl::iddObjectType() const {
      return SurfacePropertyLocalEnvironment::iddObjectType();
    }

    std::vector<ScheduleTypeKey> SurfacePropertyLocalEnvironment_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName) != e) {
        result.emplace_back("SurfacePropertyLocalEnvironment", "External Shading Fraction");
      }
      return result;
    }

    boost::optional<PlanarSurface> SurfacePropertyLocalEnvironment_Impl::optionalExteriorSurface() const {
      if (auto mo_ = getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName)) {
        return mo_->cast<PlanarSurface>();
      }
      return boost::none;
    }

    PlanarSurface SurfacePropertyLocalEnvironment_Impl::exteriorSurface() const {
      boost::optional<PlanarSurface> value = optionalExteriorSurface();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Exterior Surface attached.");
      }
      return value.get();
    }

    boost::optional<Surface> SurfacePropertyLocalEnvironment_Impl::exteriorSurfaceAsSurface() const {
      return getObject<ModelObject>().getModelObjectTarget<Surface>(OS_SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName);
    }

    boost::optional<SubSurface> SurfacePropertyLocalEnvironment_Impl::exteriorSurfaceAsSubSurface() const {
      return getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName);
    }

    boost::optional<Schedule> SurfacePropertyLocalEnvironment_Impl::externalShadingFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName);
    }

    boost::optional<SurfacePropertySurroundingSurfaces> SurfacePropertyLocalEnvironment_Impl::surfacePropertySurroundingSurfaces() const {
      return getObject<ModelObject>().getModelObjectTarget<SurfacePropertySurroundingSurfaces>(
        OS_SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName);
    }

    boost::optional<SurfacePropertyGroundSurfaces> SurfacePropertyLocalEnvironment_Impl::surfacePropertyGroundSurfaces() const {
      return getObject<ModelObject>().getModelObjectTarget<SurfacePropertyGroundSurfaces>(
        OS_SurfaceProperty_LocalEnvironmentFields::GroundSurfacesObjectName);
    }

    bool SurfacePropertyLocalEnvironment_Impl::setExteriorSurface(const PlanarSurface& surface) {

      boost::optional<SurfacePropertyLocalEnvironment> currentLocalEnv;
      if (auto s_ = surface.optionalCast<Surface>()) {
        currentLocalEnv = s_->surfacePropertyLocalEnvironment();
      } else if (auto s_ = surface.optionalCast<SubSurface>()) {
        currentLocalEnv = s_->surfacePropertyLocalEnvironment();
      } else {
        LOG(Error,
            "SurfacePropertyLocalEnvironment currently only accepts Surface and SubSurface, object isn't accepted: " << surface.briefDescription());
        return false;
      }
      if (currentLocalEnv) {
        if (currentLocalEnv->handle() == this->handle()) {
          return true;
        }
        LOG(Warn, "Surface " << surface.nameString()
                             << " already has a SurfacePropertyLocalEnvironment attached, and it will be **removed**. Please check your inputs.");

        currentLocalEnv->remove();
      }
      return setPointer(OS_SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName, surface.handle());
    }

    bool SurfacePropertyLocalEnvironment_Impl::setExternalShadingFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName, "SurfacePropertyLocalEnvironment",
                                "External Shading Fraction", schedule);
      return result;
    }

    void SurfacePropertyLocalEnvironment_Impl::resetExternalShadingFractionSchedule() {
      bool result = setString(OS_SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName, "");
      OS_ASSERT(result);
    }

    bool SurfacePropertyLocalEnvironment_Impl::setSurfacePropertySurroundingSurfaces(
      const SurfacePropertySurroundingSurfaces& surfacePropertySurroundingSurfaces) {
      bool result = setPointer(OS_SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName, surfacePropertySurroundingSurfaces.handle());
      return result;
    }

    void SurfacePropertyLocalEnvironment_Impl::resetSurfacePropertySurroundingSurfaces() {
      bool result = setString(OS_SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName, "");
      OS_ASSERT(result);
    }

    bool SurfacePropertyLocalEnvironment_Impl::setSurfacePropertyGroundSurfaces(const SurfacePropertyGroundSurfaces& surfacePropertyGroundSurfaces) {
      bool result = setPointer(OS_SurfaceProperty_LocalEnvironmentFields::GroundSurfacesObjectName, surfacePropertyGroundSurfaces.handle());
      return result;
    }

    void SurfacePropertyLocalEnvironment_Impl::resetSurfacePropertyGroundSurfaces() {
      bool result = setString(OS_SurfaceProperty_LocalEnvironmentFields::GroundSurfacesObjectName, "");
      OS_ASSERT(result);
    }

    //boost::optional<OutdoorAirNode> SurfacePropertyLocalEnvironment_Impl::outdoorAirNode() const {
    //return getObject<ModelObject>().getModelObjectTarget<OutdoorAirNode>(OS_SurfaceProperty_LocalEnvironmentFields::OutdoorAirNodeName);
    //}

    //bool SurfacePropertyLocalEnvironment_Impl::setOutdoorAirNode(const OutdoorAirNode& outdoorAirNode) {
    //bool result = setPointer(OS_SurfaceProperty_LocalEnvironmentFields::OutdoorAirNodeName, outdoorAirNode.handle());
    //return result;
    //}

    //void SurfacePropertyLocalEnvironment_Impl::resetOutdoorAirNode() {
    //bool result = setString(OS_SurfaceProperty_LocalEnvironmentFields::OutdoorAirNodeName, "");
    //OS_ASSERT(result);
    //}

  }  // namespace detail

  SurfacePropertyLocalEnvironment::SurfacePropertyLocalEnvironment(const Surface& surface)
    : ModelObject(SurfacePropertyLocalEnvironment::iddObjectType(), surface.model()) {
    OS_ASSERT(getImpl<detail::SurfacePropertyLocalEnvironment_Impl>());

    bool ok = setExteriorSurface(surface);
    if (!ok) {
      this->remove();
      LOG_AND_THROW("Cannot create a SurfacePropertyLocalEnvironment pointing to " << surface.briefDescription());
    }
  }

  SurfacePropertyLocalEnvironment::SurfacePropertyLocalEnvironment(const SubSurface& subSurface)
    : ModelObject(SurfacePropertyLocalEnvironment::iddObjectType(), subSurface.model()) {
    OS_ASSERT(getImpl<detail::SurfacePropertyLocalEnvironment_Impl>());

    bool ok = setExteriorSurface(subSurface);
    if (!ok) {
      this->remove();
      LOG_AND_THROW("Cannot create a SurfacePropertyLocalEnvironment pointing to " << subSurface.briefDescription());
    }
  }

  IddObjectType SurfacePropertyLocalEnvironment::iddObjectType() {
    return {IddObjectType::OS_SurfaceProperty_LocalEnvironment};
  }

  PlanarSurface SurfacePropertyLocalEnvironment::exteriorSurface() const {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->exteriorSurface();
  }

  boost::optional<Surface> SurfacePropertyLocalEnvironment::exteriorSurfaceAsSurface() const {
    return getImpl<ImplType>()->exteriorSurfaceAsSurface();
  }

  boost::optional<SubSurface> SurfacePropertyLocalEnvironment::exteriorSurfaceAsSubSurface() const {
    return getImpl<ImplType>()->exteriorSurfaceAsSubSurface();
  }

  boost::optional<Schedule> SurfacePropertyLocalEnvironment::externalShadingFractionSchedule() const {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->externalShadingFractionSchedule();
  }

  boost::optional<SurfacePropertySurroundingSurfaces> SurfacePropertyLocalEnvironment::surfacePropertySurroundingSurfaces() const {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->surfacePropertySurroundingSurfaces();
  }

  boost::optional<SurfacePropertyGroundSurfaces> SurfacePropertyLocalEnvironment::surfacePropertyGroundSurfaces() const {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->surfacePropertyGroundSurfaces();
  }

  bool SurfacePropertyLocalEnvironment::setExteriorSurface(const PlanarSurface& surface) {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->setExteriorSurface(surface);
  }

  bool SurfacePropertyLocalEnvironment::setExternalShadingFractionSchedule(Schedule& schedule) {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->setExternalShadingFractionSchedule(schedule);
  }

  void SurfacePropertyLocalEnvironment::resetExternalShadingFractionSchedule() {
    getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->resetExternalShadingFractionSchedule();
  }

  bool SurfacePropertyLocalEnvironment::setSurfacePropertySurroundingSurfaces(
    const SurfacePropertySurroundingSurfaces& surfacePropertySurroundingSurfaces) {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->setSurfacePropertySurroundingSurfaces(surfacePropertySurroundingSurfaces);
  }

  void SurfacePropertyLocalEnvironment::resetSurfacePropertySurroundingSurfaces() {
    getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->resetSurfacePropertySurroundingSurfaces();
  }

  bool SurfacePropertyLocalEnvironment::setSurfacePropertyGroundSurfaces(const SurfacePropertyGroundSurfaces& surfacePropertyGroundSurfaces) {
    return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->setSurfacePropertyGroundSurfaces(surfacePropertyGroundSurfaces);
  }

  void SurfacePropertyLocalEnvironment::resetSurfacePropertyGroundSurfaces() {
    getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->resetSurfacePropertyGroundSurfaces();
  }

  //boost::optional<OutdoorAirNode> SurfacePropertyLocalEnvironment::outdoorAirNode() const {
  //return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->outdoorAirNode();
  //}

  //bool SurfacePropertyLocalEnvironment::setOutdoorAirNode(const OutdoorAirNode& outdoorAirNode) {
  //return getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->setOutdoorAirNode(outdoorAirNode);
  //}

  //void SurfacePropertyLocalEnvironment::resetOutdoorAirNode() {
  //getImpl<detail::SurfacePropertyLocalEnvironment_Impl>()->resetOutdoorAirNode();
  //}

  /// @cond
  SurfacePropertyLocalEnvironment::SurfacePropertyLocalEnvironment(std::shared_ptr<detail::SurfacePropertyLocalEnvironment_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
