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

#include <vector>
#include "Material.hpp"
#include "Material_Impl.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_MaterialProperty_MoisturePenetrationDepth_Settings_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  MaterialPropertyMoisturePenetrationDepthSettings_Impl::MaterialPropertyMoisturePenetrationDepthSettings_Impl(const IdfObject& idfObject,
                                                                                                               Model_Impl* model,
                                                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType());
  }

  MaterialPropertyMoisturePenetrationDepthSettings_Impl::MaterialPropertyMoisturePenetrationDepthSettings_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                               Model_Impl* model,
                                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType());
  }

  MaterialPropertyMoisturePenetrationDepthSettings_Impl::MaterialPropertyMoisturePenetrationDepthSettings_Impl(const MaterialPropertyMoisturePenetrationDepthSettings_Impl& other,
                                                                                                               Model_Impl* model,
                                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& MaterialPropertyMoisturePenetrationDepthSettings_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType MaterialPropertyMoisturePenetrationDepthSettings_Impl::iddObjectType() const {
    return MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType();
  }

  std::string MaterialPropertyMoisturePenetrationDepthSettings_Impl::materialName() const {
    boost::optional<Material> material = getObject<ModelObject>().getModelObjectTarget<Material>(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MaterialName);
    OS_ASSERT(material);
    return material.get().name().get();
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::waterVaporDiffusionResistanceFactor() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::WaterVaporDiffusionResistanceFactor);
    OS_ASSERT(value);
    return value.get();
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::moistureEquationCoefficientA() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta);
    OS_ASSERT(value);
    return value.get();
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::moistureEquationCoefficientB() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb);
    OS_ASSERT(value);
    return value.get();
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::moistureEquationCoefficientC() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc);
    OS_ASSERT(value);
    return value.get();
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::moistureEquationCoefficientD() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd);
    OS_ASSERT(value);
    return value.get();
  }

  std::vector<double> MaterialPropertyMoisturePenetrationDepthSettings_Impl::moistureEquationCoefficients() const {
    boost::optional<double> valuea = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta);
    OS_ASSERT(valuea);
    boost::optional<double> valueb = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb);
    OS_ASSERT(valueb);
    boost::optional<double> valuec = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc);
    OS_ASSERT(valuec);
    boost::optional<double> valued = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd);
    OS_ASSERT(valued);
    return {valuea.get(), valueb.get(), valuec.get(), valued.get()};
  }

  boost::optional<double> MaterialPropertyMoisturePenetrationDepthSettings_Impl::surfaceLayerPenetrationDepth() {
    return getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth, true);
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::isSurfaceLayerPenetrationDepthAutocalculated() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autocalculate");
    }
    return result;
  }

  boost::optional<double> MaterialPropertyMoisturePenetrationDepthSettings_Impl::deepLayerPenetrationDepth() {
    return getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth, true);
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::isDeepLayerPenetrationDepthAutocalculated() {
    bool result = false;
    boost::optional<std::string> value = getString(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autocalculate");
    }
    return result;
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::coatingLayerThickness() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerThickness);
    OS_ASSERT(value);
    return value.get();
  }

  double MaterialPropertyMoisturePenetrationDepthSettings_Impl::coatingLayerWaterVaporDiffusionResistanceFactor() const {
    boost::optional<double> value = getDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerWaterVaporDiffusionResistanceFactor);
    OS_ASSERT(value);
    return value.get();
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setWaterVaporDiffusionResistanceFactor(double waterVaporDiffusionResistanceFactor) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::WaterVaporDiffusionResistanceFactor, waterVaporDiffusionResistanceFactor);
    OS_ASSERT(result);
    return result;
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setMoistureEquationCoefficientA(double moistureEquationCoefficientA) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta, moistureEquationCoefficientA);
    OS_ASSERT(result);
    return result;
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setMoistureEquationCoefficientB(double moistureEquationCoefficientB) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb, moistureEquationCoefficientB);
    OS_ASSERT(result);
    return result;
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setMoistureEquationCoefficientC(double moistureEquationCoefficientC) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc, moistureEquationCoefficientC);
    OS_ASSERT(result);
    return result;
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setMoistureEquationCoefficientD(double moistureEquationCoefficientD) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd, moistureEquationCoefficientD);
    OS_ASSERT(result);
    return result;
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setMoistureEquationCoefficients(double moistureEquationCoefficientA, double moistureEquationCoefficientB, double moistureEquationCoefficientC, double moistureEquationCoefficientD) {
    bool resulta = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta, moistureEquationCoefficientA);
    OS_ASSERT(resulta);
    bool resultb = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb, moistureEquationCoefficientB);
    OS_ASSERT(resultb);
    bool resultc = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc, moistureEquationCoefficientC);
    OS_ASSERT(resultc);
    bool resultd = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd, moistureEquationCoefficientD);
    OS_ASSERT(resultd);
    if (resulta && resultb && resultc && resultd) {
      return true;
    } else {
      return false;
    }
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setSurfaceLayerPenetrationDepth(double surfaceLayerPenetrationDepth) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth, surfaceLayerPenetrationDepth);
    OS_ASSERT(result);
    return result;
  }

  void MaterialPropertyMoisturePenetrationDepthSettings_Impl::autocalculateSurfaceLayerPenetrationDepth() {
    bool result = setString(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth, "");
    OS_ASSERT(result);
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setDeepLayerPenetrationDepth(double deepLayerPenetrationDepth) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth, deepLayerPenetrationDepth);
    OS_ASSERT(result);
    return result;
  }

  void MaterialPropertyMoisturePenetrationDepthSettings_Impl::autocalculateDeepLayerPenetrationDepth() {
    bool result = setString(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth, "");
    OS_ASSERT(result);
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setCoatingLayerThickness(double coatingLayerThickness) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerThickness, coatingLayerThickness);
    OS_ASSERT(result);
    return result;
  }

  bool MaterialPropertyMoisturePenetrationDepthSettings_Impl::setCoatingLayerWaterVaporDiffusionResistanceFactor(double coatingLayerWaterVaporDiffusionResistanceFactor) {
    bool result = setDouble(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerWaterVaporDiffusionResistanceFactor, coatingLayerWaterVaporDiffusionResistanceFactor);
    OS_ASSERT(result);
    return result;
  }

} // detail

MaterialPropertyMoisturePenetrationDepthSettings::MaterialPropertyMoisturePenetrationDepthSettings(Material& material, double waterVaporDiffusionResistanceFactor, double moistureEquationCoefficientA, double moistureEquationCoefficientB, double moistureEquationCoefficientC, double moistureEquationCoefficientD, double coatingLayerThickness, double coatingLayerWaterVaporDiffusionResistanceFactor)
  : ModelObject(MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType(), material.model())
{
  OS_ASSERT(getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>());

  if (material.materialPropertyMoisturePenetrationDepthSettings())
  {
    LOG_AND_THROW("Material '" << material.nameString() << "' already has an associated MaterialPropertyMoisturePenetrationDepthSettings object");
  }

  bool ok = true;
  OS_ASSERT(ok);

  ok = setPointer(OS_MaterialProperty_MoisturePenetrationDepth_SettingsFields::MaterialName, material.handle());
  OS_ASSERT(ok);

  ok = setWaterVaporDiffusionResistanceFactor(waterVaporDiffusionResistanceFactor);
  OS_ASSERT(ok);

  ok = setMoistureEquationCoefficientA(moistureEquationCoefficientA);
  OS_ASSERT(ok);

  ok = setMoistureEquationCoefficientB(moistureEquationCoefficientB);
  OS_ASSERT(ok);

  ok = setMoistureEquationCoefficientC(moistureEquationCoefficientC);
  OS_ASSERT(ok);

  ok = setMoistureEquationCoefficientD(moistureEquationCoefficientD);
  OS_ASSERT(ok);

  autocalculateSurfaceLayerPenetrationDepth();
  autocalculateDeepLayerPenetrationDepth();

  ok = setCoatingLayerThickness(coatingLayerThickness);
  OS_ASSERT(ok);

  ok = setCoatingLayerWaterVaporDiffusionResistanceFactor(coatingLayerWaterVaporDiffusionResistanceFactor);
  OS_ASSERT(ok);
}

IddObjectType MaterialPropertyMoisturePenetrationDepthSettings::iddObjectType() {
  return IddObjectType(IddObjectType::OS_MaterialProperty_MoisturePenetrationDepth_Settings);
}

std::string MaterialPropertyMoisturePenetrationDepthSettings::materialName() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->materialName();
}

double MaterialPropertyMoisturePenetrationDepthSettings::waterVaporDiffusionResistanceFactor() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->waterVaporDiffusionResistanceFactor();
}

double MaterialPropertyMoisturePenetrationDepthSettings::moistureEquationCoefficientA() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->moistureEquationCoefficientA();
}

double MaterialPropertyMoisturePenetrationDepthSettings::moistureEquationCoefficientB() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->moistureEquationCoefficientB();
}

double MaterialPropertyMoisturePenetrationDepthSettings::moistureEquationCoefficientC() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->moistureEquationCoefficientC();
}

double MaterialPropertyMoisturePenetrationDepthSettings::moistureEquationCoefficientD() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->moistureEquationCoefficientD();
}

std::vector<double> MaterialPropertyMoisturePenetrationDepthSettings::moistureEquationCoefficients() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->moistureEquationCoefficients();
}

boost::optional<double> MaterialPropertyMoisturePenetrationDepthSettings::surfaceLayerPenetrationDepth() {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->surfaceLayerPenetrationDepth();
}

bool MaterialPropertyMoisturePenetrationDepthSettings::isSurfaceLayerPenetrationDepthAutocalculated() {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->isSurfaceLayerPenetrationDepthAutocalculated();
}

boost::optional<double> MaterialPropertyMoisturePenetrationDepthSettings::deepLayerPenetrationDepth() {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->deepLayerPenetrationDepth();
}

bool MaterialPropertyMoisturePenetrationDepthSettings::isDeepLayerPenetrationDepthAutocalculated() {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->isDeepLayerPenetrationDepthAutocalculated();
}

double MaterialPropertyMoisturePenetrationDepthSettings::coatingLayerThickness() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->coatingLayerThickness();
}

double MaterialPropertyMoisturePenetrationDepthSettings::coatingLayerWaterVaporDiffusionResistanceFactor() const {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->coatingLayerWaterVaporDiffusionResistanceFactor();
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setWaterVaporDiffusionResistanceFactor(double waterVaporDiffusionResistanceFactor) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setWaterVaporDiffusionResistanceFactor(waterVaporDiffusionResistanceFactor);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setMoistureEquationCoefficientA(double moistureEquationCoefficientA) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setMoistureEquationCoefficientA(moistureEquationCoefficientA);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setMoistureEquationCoefficientB(double moistureEquationCoefficientB) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setMoistureEquationCoefficientB(moistureEquationCoefficientB);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setMoistureEquationCoefficientC(double moistureEquationCoefficientC) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setMoistureEquationCoefficientC(moistureEquationCoefficientC);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setMoistureEquationCoefficientD(double moistureEquationCoefficientD) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setMoistureEquationCoefficientD(moistureEquationCoefficientD);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setMoistureEquationCoefficients(double moistureEquationCoefficientA, double moistureEquationCoefficientB, double moistureEquationCoefficientC, double moistureEquationCoefficientD) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setMoistureEquationCoefficients(moistureEquationCoefficientA, moistureEquationCoefficientB, moistureEquationCoefficientC, moistureEquationCoefficientD);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setSurfaceLayerPenetrationDepth(double surfaceLayerPenetrationDepth) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setSurfaceLayerPenetrationDepth(surfaceLayerPenetrationDepth);
}

void MaterialPropertyMoisturePenetrationDepthSettings::autocalculateSurfaceLayerPenetrationDepth() {
  getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->autocalculateSurfaceLayerPenetrationDepth();
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setDeepLayerPenetrationDepth(double deepLayerPenetrationDepth) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setDeepLayerPenetrationDepth(deepLayerPenetrationDepth);
}

void MaterialPropertyMoisturePenetrationDepthSettings::autocalculateDeepLayerPenetrationDepth() {
  getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->autocalculateDeepLayerPenetrationDepth();
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setCoatingLayerThickness(double coatingLayerThickness) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setCoatingLayerThickness(coatingLayerThickness);
}

bool MaterialPropertyMoisturePenetrationDepthSettings::setCoatingLayerWaterVaporDiffusionResistanceFactor(double coatingLayerWaterVaporDiffusionResistanceFactor) {
  return getImpl<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl>()->setCoatingLayerWaterVaporDiffusionResistanceFactor(coatingLayerWaterVaporDiffusionResistanceFactor);
}

/// @cond
MaterialPropertyMoisturePenetrationDepthSettings::MaterialPropertyMoisturePenetrationDepthSettings(std::shared_ptr<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio