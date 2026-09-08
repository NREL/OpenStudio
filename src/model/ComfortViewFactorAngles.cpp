/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ComfortViewFactorAngles.hpp"
#include "ComfortViewFactorAngles_Impl.hpp"

#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ComfortViewFactorAngles_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  AngleFactor::AngleFactor(const Surface& surface, double angleFactor) : m_surface(surface), m_angleFactor(angleFactor) {
    if ((m_angleFactor < 0.0) || (m_angleFactor > 1.0)) {
      LOG_AND_THROW("Angle Factor must be between 0 and 1.");
    }
  }

  Surface AngleFactor::surface() const {
    return m_surface;
  }

  double AngleFactor::angleFactor() const {
    return m_angleFactor;
  }

  bool AngleFactor::operator!=(const AngleFactor& other) const {
    return !operator==(other);
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::AngleFactor& angleFactor) {
    out << "(surface='" << angleFactor.surface().nameString() << "', angle factor=" << angleFactor.angleFactor() << ")";
    return out;
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

    std::vector<AngleFactor> ComfortViewFactorAngles_Impl::angleFactors() const {
      std::vector<AngleFactor> result;
      for (unsigned i = 0; i < numberofAngleFactors(); ++i) {
        if (auto angleFactor = getAngleFactor(i)) {
          result.push_back(*angleFactor);
        }
      }
      return result;
    }

    unsigned int ComfortViewFactorAngles_Impl::numberofAngleFactors() const {
      return numExtensibleGroups();
    }

    boost::optional<unsigned> ComfortViewFactorAngles_Impl::angleFactorIndex(const Surface& surface) const {
      for (unsigned i = 0; i < numberofAngleFactors(); ++i) {
        if (auto existingSurface = getSurface(i); existingSurface && (existingSurface->handle() == surface.handle())) {
          return i;
        }
      }
      return boost::none;
    }

    boost::optional<Surface> ComfortViewFactorAngles_Impl::getSurface(unsigned groupIndex) const {
      boost::optional<Surface> result;

      if (groupIndex >= numberofAngleFactors()) {
        LOG(Error, "Asked to get Surface with index " << groupIndex << ", but " << briefDescription() << " has just " << numberofAngleFactors()
                                                        << " angle factors.");
        return result;
      }
      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();
      result = group.getModelObjectTarget<Surface>(OS_ComfortViewFactorAnglesExtensibleFields::SurfaceName);

      if (!result) {
        LOG(Error, "Could not retrieve Surface Name for extensible group " << group.groupIndex() << ".");
      }
      return result;
    }

    boost::optional<double> ComfortViewFactorAngles_Impl::getAngleFactorValue(unsigned groupIndex) const {
      if (groupIndex >= numberofAngleFactors()) {
        LOG(Error, "Asked to get Angle Factor with index " << groupIndex << ", but " << briefDescription() << " has just " << numberofAngleFactors()
                                                             << " angle factors.");
        return boost::none;
      }
      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();
      auto result = group.getDouble(OS_ComfortViewFactorAnglesExtensibleFields::AngleFactor);
      if (!result) {
        LOG(Error, "Could not retrieve Angle Factor for extensible group " << group.groupIndex() << ".");
      }
      return result;
    }

    boost::optional<AngleFactor> ComfortViewFactorAngles_Impl::getAngleFactor(unsigned groupIndex) const {
      auto surface = getSurface(groupIndex);
      auto angleFactor = getAngleFactorValue(groupIndex);
      if (surface && angleFactor) {
        return AngleFactor(*surface, *angleFactor);
      }
      return boost::none;
    }

    bool ComfortViewFactorAngles_Impl::addAngleFactor(const AngleFactor& angleFactor) {
      if (angleFactor.surface().model() != model()) {
        LOG(Error, "Cannot add a Surface from another Model to " << briefDescription() << ".");
        return false;
      }
      const auto existingIndex = angleFactorIndex(angleFactor.surface());

      double sum = angleFactor.angleFactor();
      for (unsigned i = 0; i < numberofAngleFactors(); ++i) {
        if (existingIndex && (i == *existingIndex)) {
          continue;
        }
        auto existingValue = getAngleFactorValue(i);
        if (!existingValue || (*existingValue < 0.0) || (*existingValue > 1.0)) {
          LOG(Error, "Cannot add an AngleFactor to " << briefDescription() << " because an existing Angle Factor is invalid.");
          return false;
        }
        sum += *existingValue;
      }

      constexpr double tolerance = 0.000001;
      if (sum > 1.0 + tolerance) {
        LOG(Error, "Cannot add an AngleFactor to " << briefDescription() << " because the Angle Factors would sum to " << sum
                                                      << ", which is greater than 1.");
        return false;
      }

      auto group = (existingIndex ? getExtensibleGroup(*existingIndex).cast<ModelExtensibleGroup>()
                  : pushExtensibleGroup({}, false).cast<ModelExtensibleGroup>());
      bool surfaceSet = group.setPointer(OS_ComfortViewFactorAnglesExtensibleFields::SurfaceName, angleFactor.surface().handle(), false);
      bool angleFactorSet = group.setDouble(OS_ComfortViewFactorAnglesExtensibleFields::AngleFactor, angleFactor.angleFactor());
      if (surfaceSet && angleFactorSet) {
        return true;
      }
      if (!existingIndex) {
        getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
      }
      return false;
    }

    bool ComfortViewFactorAngles_Impl::addAngleFactor(const Surface& surface, double angleFactor) {
      return addAngleFactor(AngleFactor(surface, angleFactor));
    }

    bool ComfortViewFactorAngles_Impl::addAngleFactors(const std::vector<AngleFactor>& angleFactors) {
      bool result = true;
      for (const auto& angleFactor : angleFactors) {
        if (!addAngleFactor(angleFactor)) {
          result = false;
        }
      }
      return result;
    }

    bool ComfortViewFactorAngles_Impl::removeAngleFactor(unsigned groupIndex) {
      if (groupIndex >= numberofAngleFactors()) {
        return false;
      }
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      return true;
    }

    void ComfortViewFactorAngles_Impl::removeAllAngleFactors() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

  }  // namespace detail

  ComfortViewFactorAngles::ComfortViewFactorAngles(const Model& model) : ModelObject(ComfortViewFactorAngles::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ComfortViewFactorAngles_Impl>());
  }

  IddObjectType ComfortViewFactorAngles::iddObjectType() {
    return IddObjectType::OS_ComfortViewFactorAngles;
  }

  std::vector<AngleFactor> ComfortViewFactorAngles::angleFactors() const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->angleFactors();
  }

  unsigned int ComfortViewFactorAngles::numberofAngleFactors() const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->numberofAngleFactors();
  }

  boost::optional<unsigned> ComfortViewFactorAngles::angleFactorIndex(const Surface& surface) const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->angleFactorIndex(surface);
  }

  boost::optional<AngleFactor> ComfortViewFactorAngles::getAngleFactor(unsigned groupIndex) const {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->getAngleFactor(groupIndex);
  }

  bool ComfortViewFactorAngles::addAngleFactor(const AngleFactor& angleFactor) {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->addAngleFactor(angleFactor);
  }

  bool ComfortViewFactorAngles::addAngleFactor(const Surface& surface, double angleFactor) {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->addAngleFactor(surface, angleFactor);
  }

  bool ComfortViewFactorAngles::addAngleFactors(const std::vector<AngleFactor>& angleFactors) {
    return getImpl<detail::ComfortViewFactorAngles_Impl>()->addAngleFactors(angleFactors);
  }

  void ComfortViewFactorAngles::removeAngleFactor(int groupIndex) {
    getImpl<detail::ComfortViewFactorAngles_Impl>()->removeAngleFactor(groupIndex);
  }

  void ComfortViewFactorAngles::removeAllAngleFactors() {
    getImpl<detail::ComfortViewFactorAngles_Impl>()->removeAllAngleFactors();
  }

  ComfortViewFactorAngles::ComfortViewFactorAngles(std::shared_ptr<detail::ComfortViewFactorAngles_Impl> impl) : ModelObject(std::move(impl)) {}

}  // namespace model
}  // namespace openstudio
