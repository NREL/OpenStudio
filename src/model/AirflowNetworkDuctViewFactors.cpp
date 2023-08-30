/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkDuctViewFactors.hpp"
#include "AirflowNetworkDuctViewFactors_Impl.hpp"

#include "ModelExtensibleGroup.hpp"
#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"

#include "AirflowNetworkLinkage.hpp"
#include "AirflowNetworkLinkage_Impl.hpp"

#include "Model.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkDuctViewFactors_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  ViewFactorData::ViewFactorData(const PlanarSurface& s, double viewFactor) : m_planarSurface(s), m_viewFactor(viewFactor) {}

  PlanarSurface ViewFactorData::planarSurface() const {
    return m_planarSurface;
  }

  double ViewFactorData::viewFactor() const {
    return m_viewFactor;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::ViewFactorData& vf) {
    out << "(" << vf.planarSurface().nameString() << ",  " << vf.viewFactor() << ")";
    return out;
  }

  namespace detail {

    AirflowNetworkDuctViewFactors_Impl::AirflowNetworkDuctViewFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDuctViewFactors::iddObjectType());
    }

    AirflowNetworkDuctViewFactors_Impl::AirflowNetworkDuctViewFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkDuctViewFactors::iddObjectType());
    }

    AirflowNetworkDuctViewFactors_Impl::AirflowNetworkDuctViewFactors_Impl(const AirflowNetworkDuctViewFactors_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkDuctViewFactors_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkDuctViewFactors_Impl::iddObjectType() const {
      return AirflowNetworkDuctViewFactors::iddObjectType();
    }

    AirflowNetworkLinkage AirflowNetworkDuctViewFactors_Impl::linkage() const {
      boost::optional<AirflowNetworkLinkage> value = optionalLinkage();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Linkage attached.");
      }
      return value.get();
    }

    double AirflowNetworkDuctViewFactors_Impl::ductSurfaceExposureFraction() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceExposureFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkDuctViewFactors_Impl::isDuctSurfaceExposureFractionDefaulted() const {
      return isEmpty(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceExposureFraction);
    }

    double AirflowNetworkDuctViewFactors_Impl::ductSurfaceEmittance() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceEmittance, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkDuctViewFactors_Impl::isDuctSurfaceEmittanceDefaulted() const {
      return isEmpty(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceEmittance);
    }

    bool AirflowNetworkDuctViewFactors_Impl::setLinkage(const AirflowNetworkLinkage& linkage) {
      bool result = setPointer(OS_AirflowNetworkDuctViewFactorsFields::LinkageName, linkage.handle());
      return result;
    }

    bool AirflowNetworkDuctViewFactors_Impl::setDuctSurfaceExposureFraction(double ductSurfaceExposureFraction) {
      bool result = setDouble(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceExposureFraction, ductSurfaceExposureFraction);
      return result;
    }

    void AirflowNetworkDuctViewFactors_Impl::resetDuctSurfaceExposureFraction() {
      bool result = setString(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceExposureFraction, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkDuctViewFactors_Impl::setDuctSurfaceEmittance(double ductSurfaceEmittance) {
      bool result = setDouble(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceEmittance, ductSurfaceEmittance);
      return result;
    }

    void AirflowNetworkDuctViewFactors_Impl::resetDuctSurfaceEmittance() {
      bool result = setString(OS_AirflowNetworkDuctViewFactorsFields::DuctSurfaceEmittance, "");
      OS_ASSERT(result);
    }

    boost::optional<AirflowNetworkLinkage> AirflowNetworkDuctViewFactors_Impl::optionalLinkage() const {
      return getObject<ModelObject>().getModelObjectTarget<AirflowNetworkLinkage>(OS_AirflowNetworkDuctViewFactorsFields::LinkageName);
    }

    boost::optional<double> AirflowNetworkDuctViewFactors_Impl::getViewFactor(const PlanarSurface& surf) const {
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        OptionalString name = group.getString(0);
        if (name) {
          if (name.get() == surf.name()) {
            OptionalDouble value = group.getDouble(1);
            if (value) {
              return value;
            } else {
              LOG(Warn, "Could not read surface view factor " << group.groupIndex() << " in " << briefDescription() << ".");
              return boost::none;
            }
          }
        } else {
          LOG(Warn, "Could not read surface name " << group.groupIndex() << " in " << briefDescription() << ".");
        }
      }
      return boost::none;
    }

    std::vector<ViewFactorData> AirflowNetworkDuctViewFactors_Impl::viewFactors() const {
      std::vector<ViewFactorData> result;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        OptionalString name = group.getString(0);
        if (name) {
          OptionalDouble value = group.getDouble(1);
          if (value) {
            boost::optional<PlanarSurface> optsurface = group.getModelObjectTarget<PlanarSurface>(0);
            if (optsurface) {
              result.push_back(ViewFactorData{optsurface.get(), value.get()});
            } else {
              LOG(Warn, "Failed to find surface named " << name.get() << " in " << briefDescription() << ".");
            }
          } else {
            LOG(Warn, "Could not read surface view factor " << group.groupIndex() << " in " << briefDescription() << ".");
          }
        } else {
          LOG(Warn, "Could not read surface name " << group.groupIndex() << " in " << briefDescription() << ".");
        }
      }
      return result;
    }

    //std::unordered_map<PlanarSurface, double> AirflowNetworkDuctViewFactors_Impl::viewFactorMap() const
    //{
    //  std::unordered_map<PlanarSurface, double> result;
    //  for (auto &f : viewFactors()) {
    //    result[f.first] = f.second;
    //  }
    //  return result;
    //}

    bool AirflowNetworkDuctViewFactors_Impl::setViewFactor(const PlanarSurface& surf, double F) {
      //std::vector<unsigned> ids = castVector<WorkspaceExtensibleGroup>(extensibleGroups())  getSourceFieldIndices(surf.handle());
      std::vector<unsigned> ids = getSourceIndices(surf.handle());
      if (ids.empty()) {
        LOG(Info, "Did not find the surface!");
        // Didn't find the surface
        std::vector<std::string> values = {toString(surf.handle()), toString(F)};
        auto group = pushExtensibleGroup(values, false).cast<ModelExtensibleGroup>();
        OS_ASSERT(!group.empty());
      } else {
        LOG(Info, "Found the surface!");
        // Found the surface
      }
      return false;
    }

    bool AirflowNetworkDuctViewFactors_Impl::removeViewFactor(const PlanarSurface& surf) {
      std::vector<unsigned> ids = getSourceIndices(surf.handle());
      if (ids.empty()) {
        LOG(Info, "Did not find the surface!");
        // Didn't find the surface
        return false;
      }
      LOG(Info, "Found the surface!");
      // Found the surface, maybe need to check if there is more than one?
      return !eraseExtensibleGroup(ids[0]).empty();
    }

    void AirflowNetworkDuctViewFactors_Impl::resetViewFactors() {
      auto results = clearExtensibleGroups(false);
      OS_ASSERT(!results.empty());
    }

  }  // namespace detail

  AirflowNetworkDuctViewFactors::AirflowNetworkDuctViewFactors(const Model& model)
    : ModelObject(AirflowNetworkDuctViewFactors::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkDuctViewFactors_Impl>());

    // TODO: Appropriately handle the following required object-list fields.
    //     OS_AirflowNetworkDuctViewFactorsFields::LinkageName
    bool ok = true;
    // ok = setLinkage();
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkDuctViewFactors::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkDuctViewFactors};
  }

  AirflowNetworkLinkage AirflowNetworkDuctViewFactors::linkage() const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->linkage();
  }

  double AirflowNetworkDuctViewFactors::ductSurfaceExposureFraction() const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->ductSurfaceExposureFraction();
  }

  bool AirflowNetworkDuctViewFactors::isDuctSurfaceExposureFractionDefaulted() const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->isDuctSurfaceExposureFractionDefaulted();
  }

  double AirflowNetworkDuctViewFactors::ductSurfaceEmittance() const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->ductSurfaceEmittance();
  }

  bool AirflowNetworkDuctViewFactors::isDuctSurfaceEmittanceDefaulted() const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->isDuctSurfaceEmittanceDefaulted();
  }

  bool AirflowNetworkDuctViewFactors::setLinkage(const AirflowNetworkLinkage& linkage) {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->setLinkage(linkage);
  }

  bool AirflowNetworkDuctViewFactors::setDuctSurfaceExposureFraction(double ductSurfaceExposureFraction) {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->setDuctSurfaceExposureFraction(ductSurfaceExposureFraction);
  }

  void AirflowNetworkDuctViewFactors::resetDuctSurfaceExposureFraction() {
    getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->resetDuctSurfaceExposureFraction();
  }

  bool AirflowNetworkDuctViewFactors::setDuctSurfaceEmittance(double ductSurfaceEmittance) {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->setDuctSurfaceEmittance(ductSurfaceEmittance);
  }

  void AirflowNetworkDuctViewFactors::resetDuctSurfaceEmittance() {
    getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->resetDuctSurfaceEmittance();
  }

  boost::optional<double> AirflowNetworkDuctViewFactors::getViewFactor(const PlanarSurface& surf) const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->getViewFactor(surf);
  }

  std::vector<ViewFactorData> AirflowNetworkDuctViewFactors::viewFactors() const {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->viewFactors();
  }

  //std::unordered_map<PlanarSurface, double> AirflowNetworkDuctViewFactors::viewFactorMap() const
  //{
  //  return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->viewFactorMap();
  //}

  bool AirflowNetworkDuctViewFactors::setViewFactor(const PlanarSurface& surf, double F) {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->setViewFactor(surf, F);
  }

  bool AirflowNetworkDuctViewFactors::removeViewFactor(const PlanarSurface& surf) {
    return getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->removeViewFactor(surf);
  }

  void AirflowNetworkDuctViewFactors::resetViewFactors() {
    getImpl<detail::AirflowNetworkDuctViewFactors_Impl>()->resetViewFactors();
  }

  /// @cond
  AirflowNetworkDuctViewFactors::AirflowNetworkDuctViewFactors(std::shared_ptr<detail::AirflowNetworkDuctViewFactors_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
