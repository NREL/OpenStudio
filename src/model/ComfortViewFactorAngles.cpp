/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ComfortViewFactorAngles.hpp"
#include "ComfortViewFactorAngles_Impl.hpp"

#include "Model.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ComfortViewFactorAngles_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  ComfortViewFactorAngle::ComfortViewFactorAngle(const Surface& surface, double angleFactor) : m_surface(surface), m_angleFactor(angleFactor) {
    if ((m_angleFactor < 0.0) || (m_angleFactor > 1.0)) {
      LOG_AND_THROW("Angle Factor must be between 0 and 1.");
    }
  }

  Surface ComfortViewFactorAngle::surface() const {
    return m_surface;
  }

  double ComfortViewFactorAngle::angleFactor() const {
    return m_angleFactor;
  }

  namespace detail {

    ComfortViewFactorAngles_Impl::ComfortViewFactorAngles_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ComfortViewFactorAngles::iddObjectType());
    }

    ComfortViewFactorAngles_Impl::ComfortViewFactorAngles_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ComfortViewFactorAngles::iddObjectType());
    }

    ComfortViewFactorAngles_Impl::ComfortViewFactorAngles_Impl(const ComfortViewFactorAngles_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ComfortViewFactorAngles_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ComfortViewFactorAngles_Impl::iddObjectType() const {
      return ComfortViewFactorAngles::iddObjectType();
    }

    std::vector<ComfortViewFactorAngle> ComfortViewFactorAngles_Impl::comfortViewFactorAngles() const {
      std::vector<ComfortViewFactorAngle> result;
      for (unsigned i = 0; i < numberofComfortViewFactorAngles(); ++i) {
        if (auto comfortViewFactorAngle = getComfortViewFactorAngle(i)) {
          result.push_back(*comfortViewFactorAngle);
        }
      }
      return result;
    }

    unsigned int ComfortViewFactorAngles_Impl::numberofComfortViewFactorAngles() const {
      return numExtensibleGroups();
    }

    boost::optional<ComfortViewFactorAngle> ComfortViewFactorAngles_Impl::getComfortViewFactorAngle(unsigned groupIndex) const {
      if (groupIndex >= numberofComfortViewFactorAngles()) {
        return boost::none;
      }
      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();
      auto surface = group.getModelObjectTarget<Surface>(OS_ComfortViewFactorAnglesExtensibleFields::SurfaceName);
      auto angleFactor = group.getDouble(OS_ComfortViewFactorAnglesExtensibleFields::AngleFactor);
      if (surface && angleFactor) {
        return ComfortViewFactorAngle(*surface, *angleFactor);
      }
      return boost::none;
    }

    bool ComfortViewFactorAngles_Impl::addComfortViewFactorAngle(const ComfortViewFactorAngle& comfortViewFactorAngle) {
      if (comfortViewFactorAngle.surface().model() != model()) {
        LOG(Error, "Cannot add a Surface from another Model to " << briefDescription() << ".");
        return false;
      }
      auto group = pushExtensibleGroup({}, false).cast<ModelExtensibleGroup>();
      bool surfaceSet = group.setPointer(OS_ComfortViewFactorAnglesExtensibleFields::SurfaceName, comfortViewFactorAngle.surface().handle(), false);
      bool angleFactorSet = group.setDouble(OS_ComfortViewFactorAnglesExtensibleFields::AngleFactor, comfortViewFactorAngle.angleFactor());
      if (surfaceSet && angleFactorSet) {
        return true;
      }
      getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
      return false;
    }

    bool ComfortViewFactorAngles_Impl::addComfortViewFactorAngle(const Surface& surface, double angleFactor) {
      return addComfortViewFactorAngle(ComfortViewFactorAngle(surface, angleFactor));
    }

    bool ComfortViewFactorAngles_Impl::removeComfortViewFactorAngle(unsigned groupIndex) {
      if (groupIndex >= numberofComfortViewFactorAngles()) {
        return false;
      }
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      return true;
    }

    void ComfortViewFactorAngles_Impl::removeAllComfortViewFactorAngles() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

  }  // namespace detail

  ComfortViewFactorAngles::ComfortViewFactorAngles(const Model& model) : ModelObject(ComfortViewFactorAngles::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ComfortViewFactorAngles_Impl>());
  }

  IddObjectType ComfortViewFactorAngles::iddObjectType() {
    return IddObjectType::OS_ComfortViewFactorAngles;
  }

  std::vector<ComfortViewFactorAngle> ComfortViewFactorAngles::comfortViewFactorAngles() const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->comfortViewFactorAngles();
  }

  unsigned int ComfortViewFactorAngles::numberofComfortViewFactorAngles() const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->numberofComfortViewFactorAngles();
  }

  boost::optional<ComfortViewFactorAngle> ComfortViewFactorAngles::getComfortViewFactorAngle(unsigned groupIndex) const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->getComfortViewFactorAngle(groupIndex);
  }

  bool ComfortViewFactorAngles::addComfortViewFactorAngle(const ComfortViewFactorAngle& comfortViewFactorAngle) {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->addComfortViewFactorAngle(comfortViewFactorAngle);
  }

  bool ComfortViewFactorAngles::addComfortViewFactorAngle(const Surface& surface, double angleFactor) {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->addComfortViewFactorAngle(surface, angleFactor);
  }

  void ComfortViewFactorAngles::removeComfortViewFactorAngle(int groupIndex) {
    getImpl<detail::ComfortViewFactorAngles_Impl>()->removeComfortViewFactorAngle(groupIndex);
  }

  void ComfortViewFactorAngles::removeAllComfortViewFactorAngles() {
    getImpl<detail::ComfortViewFactorAngles_Impl>()->removeAllComfortViewFactorAngles();
  }

  ComfortViewFactorAngles::ComfortViewFactorAngles(std::shared_ptr<detail::ComfortViewFactorAngles_Impl> impl) : ModelObject(std::move(impl)) {}

}  // namespace model
}  // namespace openstudio