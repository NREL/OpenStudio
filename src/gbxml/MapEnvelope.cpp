/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/AdditionalProperties.hpp"
#include "../model/AdditionalProperties_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/AirGap.hpp"
#include "../model/AirGap_Impl.hpp"
#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
#include "../model/FenestrationMaterial.hpp"
#include "../model/FenestrationMaterial_Impl.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/SimpleGlazing_Impl.hpp"
#include "../model/ModelPartitionMaterial.hpp"
#include "../model/ModelPartitionMaterial_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include <pugixml.hpp>

namespace openstudio {
namespace gbxml {

  boost::optional<openstudio::model::ModelObject>
    ReverseTranslator::translateConstruction(const pugi::xml_node& element, const std::unordered_map<std::string, pugi::xml_node>& layerElements,
                                             openstudio::model::Model& model) {
    // Krishnan, this constructor should only be used for unique objects like Building and Site
    //openstudio::model::Construction construction = model.getUniqueModelObject<openstudio::model::Construction>();

    openstudio::model::Construction construction(model);
    std::string constructionId = element.attribute("id").value();
    m_idToObjectMap.insert(std::make_pair(constructionId, construction));

    std::string constructionName = element.child("Name").text().as_string();
    construction.setName(escapeName(constructionId, constructionName));

    // auto layerIdList = element.children("LayerId");
    // Construction::LayerId (layerIdList) -> Layer (layerElements), Layer::MaterialId -> Material
    std::vector<openstudio::model::Material> materials;
    for (auto& layerIdEl : element.children("LayerId")) {
      std::string layerId = layerIdEl.attribute("layerIdRef").value();

      // find this layerId in all the layers
      auto result = layerElements.find(layerId);
      if (result != layerElements.end()) {
        for (auto& materialIdElement : result->second.children("MaterialId")) {
          std::string materialId = materialIdElement.attribute("materialIdRef").value();
          auto materialIt = m_idToObjectMap.find(materialId);
          if (materialIt != m_idToObjectMap.end()) {
            boost::optional<openstudio::model::Material> material = materialIt->second.optionalCast<openstudio::model::Material>();
            OS_ASSERT(material);  // Krishnan, what type of error handling do you want?
            materials.push_back(*material);
          }
        }
      } else {
        // Error handling?
      }
    }

    if (materials.empty()) {
      return construction;
    }

    // now assign all layers to real material, does gbXML have same layer order convention as E+?
    for (unsigned i = 0; i < materials.size(); ++i) {
      bool test = false;

      if (materials[i].optionalCast<openstudio::model::OpaqueMaterial>()) {
        test = construction.insertLayer(i, materials[i].cast<openstudio::model::OpaqueMaterial>());
      } else if (materials[i].optionalCast<openstudio::model::FenestrationMaterial>()) {
        test = construction.insertLayer(i, materials[i].cast<openstudio::model::FenestrationMaterial>());
      } else if (materials[i].optionalCast<openstudio::model::ModelPartitionMaterial>()) {
        test = construction.setLayer(materials[i].cast<openstudio::model::ModelPartitionMaterial>());
      }

      OS_ASSERT(test);  // Krishnan, what type of error handling do you want?
    }

    return construction;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWindowType(const pugi::xml_node& element,
                                                                                         openstudio::model::Model& model) {
    openstudio::model::Construction construction(model);
    std::string windowTypeId = element.attribute("id").value();
    m_idToObjectMap.insert(std::make_pair(windowTypeId, construction));
    construction.additionalProperties().setFeature("gbXMLId", windowTypeId);

    std::string windowTypeName = element.child("Name").text().as_string();
    construction.setName(escapeName(windowTypeId, windowTypeName));

    boost::optional<double> uValue;
    boost::optional<double> shgc;
    boost::optional<double> tVis;

    for (auto& uValueElement : element.children("U-value")) {
      if (uValueElement.attribute("unit").value() == std::string("WPerSquareMeterK")) {
        uValue = uValueElement.text().as_double();
        break;
      }
    }

    for (auto& shgcElement : element.children("SolarHeatGainCoeff")) {
      if (shgcElement.attribute("unit").value() == std::string("Fraction")) {
        shgc = shgcElement.text().as_double();
        break;
      }
    }

    for (auto& transmittanceElement : element.children("Transmittance")) {
      if (transmittanceElement.attribute("type").value() == std::string("Visible")) {
        tVis = transmittanceElement.text().as_double();
        break;
      }
    }

    if (uValue && shgc && tVis) {
      model::SimpleGlazing glazing(model);
      glazing.setUFactor(*uValue);
      glazing.setSolarHeatGainCoefficient(*shgc);
      glazing.setVisibleTransmittance(*tVis);

      std::vector<model::Material> layers;
      layers.push_back(glazing);
      construction.setLayers(layers);
    }

    return construction;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateMaterial(const pugi::xml_node& element,
                                                                                       openstudio::model::Model& model) {
    boost::optional<openstudio::model::ModelObject> result;

    auto rvalueElement = element.child("R-value");
    auto densityElement = element.child("Density");
    auto conductivityElement = element.child("Conductivity");
    auto thicknessElement = element.child("Thickness");
    auto specificHeatElement = element.child("SpecificHeat");
    auto roughnessElement = element.child("Roughness");

    if (!(densityElement.empty() || conductivityElement.empty() || thicknessElement.empty() || specificHeatElement.empty())) {

      double density = densityElement.text().as_double();
      double conductivity = conductivityElement.text().as_double();
      double thickness = thicknessElement.text().as_double();
      double specificHeat = specificHeatElement.text().as_double();

      std::string roughness = "MediumRough";  // TODO: Shouldn't that be the same default as OS (Smooth)?
      if (!roughnessElement.empty()) {
        roughness = roughnessElement.attribute("value").value();
      }

      openstudio::model::StandardOpaqueMaterial material(model);
      result = material;

      std::string id = element.attribute("id").value();
      m_idToObjectMap.insert(std::make_pair(id, material));
      material.additionalProperties().setFeature("gbXMLId", id);

      std::string name = element.child("Name").text().as_string();
      material.setName(escapeName(id, name));

      material.setDensity(density);
      material.setThermalConductivity(conductivity);
      material.setThickness(thickness);
      material.setSpecificHeat(specificHeat);
      material.setRoughness(roughness);

    } else if (!rvalueElement.empty())  //Material no mass that has only R-value
    {
      // Krishnan, this constructor should only be used for unique objects like Building and Site
      //openstudio::model::MasslessOpaqueMaterial material = model.getUniqueModelObject<openstudio::model::MasslessOpaqueMaterial>();

      double rvalue = rvalueElement.text().as_double();

      // the idd specifies a minimum value of .001 for rvalue
      rvalue = std::max(rvalue, .001);

      openstudio::model::MasslessOpaqueMaterial material(model);
      result = material;

      std::string id = element.attribute("id").value();
      m_idToObjectMap.insert(std::make_pair(id, material));
      material.additionalProperties().setFeature("gbXMLId", id);

      std::string name = element.child("Name").text().as_string();
      material.setName(escapeName(id, name));

      material.setThermalResistance(rvalue);

    } else {

      // make a stub material
      openstudio::model::MasslessOpaqueMaterial material(model);
      result = material;

      std::string id = element.attribute("id").value();
      m_idToObjectMap.insert(std::make_pair(id, material));
      material.additionalProperties().setFeature("gbXMLId", id);

      std::string name = element.child("Name").text().as_string();
      material.setName(escapeName(id, name));

      LOG(Warn, "Creating stub material '" << name << "'");

      material.setThermalResistance(.001);
    }

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase,
                                                                               pugi::xml_node& root) {
    boost::optional<pugi::xml_node> result;

    bool isOpaque = constructionBase.isOpaque();

    if (isOpaque) {
      result = root.append_child("Construction");
      m_translatedObjects[constructionBase.handle()] = *result;
    } else {
      result = root.append_child("WindowType");
      m_translatedObjects[constructionBase.handle()] = *result;
    }

    std::string name = constructionBase.name().get();

    // id
    result->append_attribute("id") = escapeName(name).c_str();

    // name
    auto nameElement = result->append_child("Name");
    nameElement.text() = name.c_str();

    if (isOpaque) {
      if (constructionBase.optionalCast<model::LayeredConstruction>()) {
        for (const auto& layer : constructionBase.cast<model::LayeredConstruction>().layers()) {
          auto layerIdElement = result->append_child("LayerId");
          layerIdElement.append_attribute("layerIdRef") = escapeName(layer.name().get() + " Layer").c_str();

          m_materials.insert(layer);
        }
      }
    } else {
      boost::optional<double> visibleTransmittance;
      boost::optional<double> uFactor;
      boost::optional<double> solarHeatGainCoefficient;

      if (constructionBase.optionalCast<model::LayeredConstruction>()) {
        std::vector<model::Material> layers = constructionBase.cast<model::LayeredConstruction>().layers();
        if (layers.size() == 1u) {
          if (layers[0].optionalCast<model::SimpleGlazing>()) {
            model::SimpleGlazing glazing = layers[0].cast<model::SimpleGlazing>();
            visibleTransmittance = glazing.visibleTransmittance();
            uFactor = glazing.uFactor();
            solarHeatGainCoefficient = glazing.solarHeatGainCoefficient();
          }
        }
      }

      if (!visibleTransmittance) {
        visibleTransmittance = constructionBase.visibleTransmittance();
      }
      if (!uFactor) {
        uFactor = constructionBase.uFactor();
      }
      //if (!solarHeatGainCoefficient){
      //  solarHeatGainCoefficient = constructionBase.solarHeatGainCoefficient();
      //}

      if (visibleTransmittance) {
        auto element = result->append_child("Transmittance");
        element.text() = *visibleTransmittance;
        element.append_attribute("unit") = "Fraction";
        element.append_attribute("type") = "Visible";
        element.append_attribute("surfaceType") = "Both";
      }

      if (uFactor) {
        auto element = result->append_child("U-value");
        element.text() = *uFactor;
        element.append_attribute("unit") = "WPerSquareMeterK";
      }

      if (solarHeatGainCoefficient) {
        auto element = result->append_child("SolarHeatGainCoeff");
        element.text() = *solarHeatGainCoefficient;
        element.append_attribute("unit") = "Fraction";
      }
    }
    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateLayer(const openstudio::model::Material& material, pugi::xml_node& root) {
    auto result = root.append_child("Layer");

    std::string materialName = material.name().get();
    std::string layerName = materialName + " Layer";

    // id
    result.append_attribute("id") = escapeName(layerName).c_str();

    // name
    auto nameElement = result.append_child("Name");
    nameElement.text() = layerName.c_str();

    // name
    auto materialIdElement = result.append_child("MaterialId");
    materialIdElement.append_attribute("materialIdRef") = escapeName(materialName).c_str();

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateMaterial(const openstudio::model::Material& material, pugi::xml_node& root) {
    auto result = root.append_child("Material");

    std::string name = material.name().get();

    // id
    result.append_attribute("id") = escapeName(name).c_str();

    // name
    auto nameElement = result.append_child("Name");
    nameElement.text() = name.c_str();

    boost::optional<double> thermalReflectance;
    boost::optional<double> solarReflectance;
    boost::optional<double> visibleReflectance;
    boost::optional<std::string> roughness;
    boost::optional<double> thickness;
    boost::optional<double> conductivity;
    boost::optional<double> resistance;
    boost::optional<double> density;
    boost::optional<double> specificHeat;
    boost::optional<double> thermalAbsorptance;
    boost::optional<double> solarAbsorptance;
    boost::optional<double> visibleAbsorptance;

    if (material.optionalCast<openstudio::model::StandardOpaqueMaterial>()) {
      openstudio::model::StandardOpaqueMaterial som = material.cast<openstudio::model::StandardOpaqueMaterial>();
      thermalReflectance = som.thermalReflectance();
      solarReflectance = som.solarReflectance();
      visibleReflectance = som.visibleReflectance();
      roughness = som.roughness();
      thickness = som.thickness();
      conductivity = som.conductivity();
      density = som.density();
      specificHeat = som.specificHeat();
      thermalAbsorptance = som.thermalAbsorptance();
      solarAbsorptance = som.solarAbsorptance();
      visibleAbsorptance = som.visibleAbsorptance();
    } else if (material.optionalCast<openstudio::model::MasslessOpaqueMaterial>()) {
      openstudio::model::MasslessOpaqueMaterial mom = material.cast<openstudio::model::MasslessOpaqueMaterial>();
      roughness = mom.roughness();
      resistance = mom.thermalResistance();
      thermalAbsorptance = mom.thermalAbsorptance();
      solarAbsorptance = mom.solarAbsorptance();
      visibleAbsorptance = mom.visibleAbsorptance();
    } else if (material.optionalCast<openstudio::model::AirGap>()) {
      openstudio::model::AirGap gap = material.cast<openstudio::model::AirGap>();
      resistance = gap.thermalResistance();
    }

    if (thermalReflectance) {
      auto element = result.append_child("Reflectance");
      element.text() = *thermalReflectance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "ExtIR";
      element.append_attribute("surfaceType") = "Both";

      element = result.append_child("Reflectance");
      element.text() = *thermalReflectance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "IntIR";
      element.append_attribute("surfaceType") = "Both";
    }

    if (solarReflectance) {
      auto element = result.append_child("Reflectance");
      element.text() = *solarReflectance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "ExtSolar";
      element.append_attribute("surfaceType") = "Both";

      element = result.append_child("Reflectance");
      element.text() = *solarReflectance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "IntSolar";
      element.append_attribute("surfaceType") = "Both";
    }

    if (visibleReflectance) {
      auto element = result.append_child("Reflectance");
      element.text() = *visibleReflectance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "ExtVisible";
      element.append_attribute("surfaceType") = "Both";

      element = result.append_child("Reflectance");
      element.text() = *visibleReflectance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "IntVisible";
      element.append_attribute("surfaceType") = "Both";
    }

    if (roughness) {
      auto element = result.append_child("Roughness");
      element.append_attribute("value") = (*roughness).c_str();
    }

    if (thickness) {
      auto element = result.append_child("Thickness");
      element.text() = *thickness;
      element.append_attribute("unit") = "Meters";
    }

    if (conductivity) {
      auto element = result.append_child("Conductivity");
      element.text() = *conductivity;
      element.append_attribute("unit") = "WPerMeterK";
    }

    if (resistance) {
      auto element = result.append_child("R-value");
      element.text() = *resistance;
      element.append_attribute("unit") = "SquareMeterKPerW";
    }

    if (density) {
      auto element = result.append_child("Density");
      element.text() = *density;
      element.append_attribute("unit") = "KgPerCubicM";
    }

    if (specificHeat) {
      auto element = result.append_child("SpecificHeat");
      element.text() = *specificHeat;
      element.append_attribute("unit") = "JPerKgK";
    }

    if (thermalAbsorptance) {
      auto element = result.append_child("Absorptance");
      element.text() = *thermalAbsorptance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "ExtIR";

      element = result.append_child("Absorptance");
      element.text() = *thermalAbsorptance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "IntIR";
    }

    if (solarAbsorptance) {
      auto element = result.append_child("Absorptance");
      element.text() = *solarAbsorptance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "ExtSolar";

      element = result.append_child("Absorptance");
      element.text() = *solarAbsorptance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "IntSolar";
    }

    if (visibleAbsorptance) {
      auto element = result.append_child("Absorptance");
      element.text() = *visibleAbsorptance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "ExtVisible";

      element = result.append_child("Absorptance");
      element.text() = *visibleAbsorptance;
      element.append_attribute("unit") = "Fraction";
      element.append_attribute("type") = "IntVisible";
    }
    return result;
  }

}  // namespace gbxml
}  // namespace openstudio
