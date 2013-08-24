/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/Model.hpp>

#include <model/Material.hpp>
#include <model/Material_Impl.hpp>
#include <model/OpaqueMaterial.hpp>
#include <model/FenestrationMaterial.hpp>
#include <model/ModelPartitionMaterial.hpp>
#include <model/ModelExtensibleGroup.hpp>
#include <model/ShadingMaterial.hpp>
#include <model/ShadingMaterial_Impl.hpp>

#include <utilities/idd/OS_Construction_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  Construction_Impl::Construction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : LayeredConstruction_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Construction::iddObjectType());
  }

  Construction_Impl::Construction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : LayeredConstruction_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Construction::iddObjectType());
  }

  Construction_Impl::Construction_Impl(const Construction_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : LayeredConstruction_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& Construction_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Construction_Impl::iddObjectType() const {
    return Construction::iddObjectType();
  }

  int Construction_Impl::renderingColorIndex() const
  {
    return OS_ConstructionFields::SurfaceRenderingName;
  }

  Construction Construction_Impl::reverseConstruction() const
  {
    if (this->isSymmetric()){
      return this->getObject<Construction>();
    }

    MaterialVector reverseLayers(this->layers());
    std::reverse(reverseLayers.begin(), reverseLayers.end());

    Model model = this->model();
    BOOST_FOREACH(const Construction& other, model.getModelObjects<Construction>()) {
      
      MaterialVector layers = other.layers();
      if (layers.size() != reverseLayers.size()){
        continue;
      }

      bool test = true;
      for (unsigned i = 0; i < layers.size(); ++i){
        if (layers[i].handle() != reverseLayers[i].handle()){
          test = false;
          break; // break out of loop over layers
        }
      }

      if (test){
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

} // detail

Construction::Construction(const Model& model)
  : LayeredConstruction(Construction::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Construction_Impl>());
}

Construction::Construction(const std::vector<OpaqueMaterial>& opaqueMaterials)
  : LayeredConstruction(Construction::iddObjectType(),opaqueMaterials.at(0).model())
{
  std::vector<Material> materials = castVector<Material>(opaqueMaterials);
  bool ok = setLayers(materials);
  OS_ASSERT(ok);
}

Construction::Construction(const std::vector<FenestrationMaterial>& fenestrationMaterials)
  : LayeredConstruction(Construction::iddObjectType(),fenestrationMaterials.at(0).model())
{
  std::vector<Material> materials = castVector<Material>(fenestrationMaterials);
  bool ok = setLayers(materials);
  OS_ASSERT(ok);
}

Construction::Construction(const ModelPartitionMaterial& modelPartitionMaterial)
  : LayeredConstruction(Construction::iddObjectType(),modelPartitionMaterial.model())
{
  bool ok = setLayer(modelPartitionMaterial);
  OS_ASSERT(ok);
}

Construction Construction::reverseConstruction() const{
  return getImpl<detail::Construction_Impl>()->reverseConstruction();
}

IddObjectType Construction::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Construction);
  return result;
}

/// @cond
Construction::Construction(boost::shared_ptr<detail::Construction_Impl> impl)
  : LayeredConstruction(impl)
{}
/// @endcond

} // model
} // openstudio
