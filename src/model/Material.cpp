/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "StandardsInformationMaterial.hpp"
#include "StandardsInformationMaterial_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    Material_Impl::Material_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {}

    Material_Impl::Material_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    Material_Impl::Material_Impl(const Material_Impl& other, Model_Impl* model, bool keepHandle) : ResourceObject_Impl(other, model, keepHandle) {}

    std::vector<ModelObject> Material_Impl::children() const {
      std::vector<ModelObject> results(castVector<ModelObject>(getObject<Material>().getModelObjectSources<StandardsInformationMaterial>()));

      return results;
    }

    double Material_Impl::thickness() const {
      return 0.0;
    }

    boost::optional<double> Material_Impl::interiorVisibleAbsorptance() const {
      return boost::none;
    }

    boost::optional<double> Material_Impl::exteriorVisibleAbsorptance() const {
      return boost::none;
    }

    bool Material_Impl::setThickness(double /*value*/) {
      return false;
    }

    StandardsInformationMaterial Material_Impl::standardsInformation() const {
      StandardsInformationMaterialVector candidates = getObject<Material>().getModelObjectSources<StandardsInformationMaterial>();
      if (candidates.size() > 1) {
        for (unsigned i = 1, n = candidates.size(); i < n; ++i) {
          candidates[i].remove();
        }
        LOG(Warn, "Removed extraneous StandardsInformationMaterial objects pointing to " << briefDescription() << ".");
      }
      if (candidates.size() == 1) {
        return candidates[0];
      }
      return StandardsInformationMaterial(getObject<Material>());
    }

  }  // namespace detail

  Material::Material(IddObjectType type, const Model& model) : ResourceObject(type, model) {
    OS_ASSERT(getImpl<detail::Material_Impl>());
  }

  double Material::thickness() const {
    return getImpl<detail::Material_Impl>()->thickness();
  }

  OptionalDouble Material::getVisibleTransmittance() const {
    return getImpl<detail::Material_Impl>()->getVisibleTransmittance();
  }

  boost::optional<double> Material::interiorVisibleAbsorptance() const {
    return getImpl<detail::Material_Impl>()->interiorVisibleAbsorptance();
  }

  boost::optional<double> Material::exteriorVisibleAbsorptance() const {
    return getImpl<detail::Material_Impl>()->exteriorVisibleAbsorptance();
  }

  bool Material::setThickness(double value) {
    return getImpl<detail::Material_Impl>()->setThickness(value);
  }

  StandardsInformationMaterial Material::standardsInformation() const {
    return getImpl<detail::Material_Impl>()->standardsInformation();
  }

  /// @cond
  Material::Material(std::shared_ptr<detail::Material_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
