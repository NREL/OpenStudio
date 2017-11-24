/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "StandardsInformationMaterial.hpp"
#include "StandardsInformationMaterial_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Material_Impl::Material_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject, model, keepHandle)
  {}

  Material_Impl::Material_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ResourceObject_Impl(other, model, keepHandle)
  {}

  Material_Impl::Material_Impl(const Material_Impl& other,Model_Impl* model,bool keepHandle)
    : ResourceObject_Impl(other, model, keepHandle)
  {}

  std::vector<ModelObject> Material_Impl::children() const
  {
    std::vector<ModelObject> results(castVector<ModelObject>(getObject<Material>().getModelObjectSources<StandardsInformationMaterial>()));
    
    if (boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> empd = this->materialPropertyMoisturePenetrationDepthSettings()) {
      results.push_back(empd.get());
    }

    return results;
  }

  double Material_Impl::thickness() const { return 0.0; }

  boost::optional<double> Material_Impl::interiorVisibleAbsorptance() const { return boost::none; }

  boost::optional<double> Material_Impl::exteriorVisibleAbsorptance() const { return boost::none; }

  bool Material_Impl::setThickness(double value) { return false; }

  StandardsInformationMaterial Material_Impl::standardsInformation() const
  {
    StandardsInformationMaterialVector candidates = getObject<Material>().getModelObjectSources<StandardsInformationMaterial>();
    if (candidates.size() > 1) {
      for (unsigned i = 1, n = candidates.size(); i < n; ++i) {
        candidates[i].remove();
      }
      LOG(Warn, "Removed extraneous StandardsInformationMaterial objects pointing to "
          << briefDescription() << ".");
    }
    if (candidates.size() == 1) { return candidates[0]; }
    return StandardsInformationMaterial(getObject<Material>());
  }

  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> Material_Impl::createMaterialPropertyMoisturePenetrationDepthSettings(double waterVaporDiffusionResistanceFactor,
                                                                                                                                          double moistureEquationCoefficientA,
                                                                                                                                          double moistureEquationCoefficientB,
                                                                                                                                          double moistureEquationCoefficientC,
                                                                                                                                          double moistureEquationCoefficientD,
                                                                                                                                          double coatingLayerThickness,
                                                                                                                                          double coatingLayerWaterVaporDiffusionResistanceFactor) {
    Material thisMaterial = getObject<Material>();
    std::vector<MaterialPropertyMoisturePenetrationDepthSettings> empds = thisMaterial.getModelObjectSources<MaterialPropertyMoisturePenetrationDepthSettings>(MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType());
    if (!empds.empty()) {
      return boost::none;
    }    
    
    MaterialPropertyMoisturePenetrationDepthSettings empd(thisMaterial, waterVaporDiffusionResistanceFactor, moistureEquationCoefficientA, moistureEquationCoefficientB, moistureEquationCoefficientC, moistureEquationCoefficientD, coatingLayerThickness, coatingLayerWaterVaporDiffusionResistanceFactor);
    return empd;
  }  

  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> Material_Impl::materialPropertyMoisturePenetrationDepthSettings() const {
    std::vector<MaterialPropertyMoisturePenetrationDepthSettings> empds = getObject<ModelObject>().getModelObjectSources<MaterialPropertyMoisturePenetrationDepthSettings>(MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType());
    if (empds.empty()) {
      // no error
    } else if (empds.size() == 1) {
      return empds[0];
    } else {
      // error
    }
    return boost::none;
  }

  void Material_Impl::resetMaterialPropertyMoisturePenetrationDepthSettings() {
    boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> empd = this->materialPropertyMoisturePenetrationDepthSettings();
    if (empd) {
      empd->remove();
    }
  }
  
} // detail

Material::Material(IddObjectType type,const Model& model)
  : ResourceObject(type,model)
{
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

StandardsInformationMaterial Material::standardsInformation() const
{
  return getImpl<detail::Material_Impl>()->standardsInformation();
}

boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> Material::createMaterialPropertyMoisturePenetrationDepthSettings(double waterVaporDiffusionResistanceFactor,
                                                                                                                                   double moistureEquationCoefficientA,
                                                                                                                                   double moistureEquationCoefficientB,
                                                                                                                                   double moistureEquationCoefficientC,
                                                                                                                                   double moistureEquationCoefficientD,
                                                                                                                                   double coatingLayerThickness,
                                                                                                                                   double coatingLayerWaterVaporDiffusionResistanceFactor)
{
  return getImpl<detail::Material_Impl>()->createMaterialPropertyMoisturePenetrationDepthSettings(waterVaporDiffusionResistanceFactor, moistureEquationCoefficientA, moistureEquationCoefficientB, moistureEquationCoefficientC, moistureEquationCoefficientD, coatingLayerThickness, coatingLayerWaterVaporDiffusionResistanceFactor);
}

boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> Material::materialPropertyMoisturePenetrationDepthSettings() const 
{
  return getImpl<detail::Material_Impl>()->materialPropertyMoisturePenetrationDepthSettings();
}

void Material::resetMaterialPropertyMoisturePenetrationDepthSettings() {
  getImpl<detail::Material_Impl>()->resetMaterialPropertyMoisturePenetrationDepthSettings();
}

/// @cond
Material::Material(std::shared_ptr<detail::Material_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
