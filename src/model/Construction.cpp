/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Construction.hpp"
#include "Construction_Impl.hpp"
#include "Model.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "OpaqueMaterial.hpp"
#include "FenestrationMaterial.hpp"
#include "ModelPartitionMaterial.hpp"
#include "ModelExtensibleGroup.hpp"
#include "ShadingMaterial.hpp"
#include "ShadingMaterial_Impl.hpp"

#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    Construction_Impl::Construction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : LayeredConstruction_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Construction::iddObjectType());
    }

    Construction_Impl::Construction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : LayeredConstruction_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Construction::iddObjectType());
    }

    Construction_Impl::Construction_Impl(const Construction_Impl& other, Model_Impl* model, bool keepHandle)
      : LayeredConstruction_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& Construction_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType Construction_Impl::iddObjectType() const {
      return Construction::iddObjectType();
    }

    int Construction_Impl::renderingColorIndex() const {
      return OS_ConstructionFields::SurfaceRenderingName;
    }

    Construction Construction_Impl::reverseConstruction() const {
      if (this->isSymmetric()) {
        return this->getObject<Construction>();
      }

      MaterialVector reverseLayers(this->layers());
      std::reverse(reverseLayers.begin(), reverseLayers.end());

      Model model = this->model();
      for (const Construction& other : model.getConcreteModelObjects<Construction>()) {

        MaterialVector layers = other.layers();
        if (layers.size() != reverseLayers.size()) {
          continue;
        }

        bool test = true;
        for (unsigned i = 0; i < layers.size(); ++i) {
          if (layers[i].handle() != reverseLayers[i].handle()) {
            test = false;
            break;  // break out of loop over layers
          }
        }

        if (test) {
          return other;
        }
      }

      // TODO: this should also copy (and modify) standards information object

      // no match, make one
      Construction result(model);
      result.setName(this->name().get() + " Reversed");
      result.setLayers(reverseLayers);

      return result;
    }

  }  // namespace detail

  Construction::Construction(const Model& model) : LayeredConstruction(Construction::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Construction_Impl>());
  }

  Construction::Construction(const std::vector<OpaqueMaterial>& opaqueMaterials)
    : LayeredConstruction(Construction::iddObjectType(), (opaqueMaterials.empty() ? openstudio::model::Model() : opaqueMaterials.at(0).model())) {
    if (opaqueMaterials.empty()) {
      // DLM: do not remove, this was only added to a temporary model
      //this->remove();
      LOG_AND_THROW("Cannot create construction from empty opaque layers.");
    } else if (opaqueMaterials.size() > 10) {
      this->remove();
      LOG_AND_THROW("Cannot create construction with more than 10 opaque layers.");
    }

    std::vector<Material> materials = castVector<Material>(opaqueMaterials);
    bool ok = setLayers(materials);
    OS_ASSERT(ok);
  }

  Construction::Construction(const std::vector<FenestrationMaterial>& fenestrationMaterials)
    : LayeredConstruction(Construction::iddObjectType(),
                          (fenestrationMaterials.empty() ? openstudio::model::Model() : fenestrationMaterials.at(0).model())) {
    if (fenestrationMaterials.empty()) {
      // DLM: do not remove, this was only added to a temporary model
      //this->remove();
      LOG_AND_THROW("Cannot create construction from empty fenestration layers.");
    } else if (fenestrationMaterials.size() > 8) {
      this->remove();
      LOG_AND_THROW("Cannot create construction with more than 8 fenestration layers.");
    }

    std::vector<Material> materials = castVector<Material>(fenestrationMaterials);
    bool ok = setLayers(materials);
    OS_ASSERT(ok);
  }

  Construction::Construction(const ModelPartitionMaterial& modelPartitionMaterial)
    : LayeredConstruction(Construction::iddObjectType(), modelPartitionMaterial.model()) {
    bool ok = setLayer(modelPartitionMaterial);
    OS_ASSERT(ok);
  }

  Construction Construction::reverseConstruction() const {
    return getImpl<detail::Construction_Impl>()->reverseConstruction();
  }

  IddObjectType Construction::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Construction);
    return result;
  }

  /// @cond
  Construction::Construction(std::shared_ptr<detail::Construction_Impl> impl) : LayeredConstruction(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
