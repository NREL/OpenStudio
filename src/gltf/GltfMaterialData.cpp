/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GltfMaterialData.hpp"
#include "GltfUtils.hpp"

#include "../model/Model.hpp"

#include "../model/RenderingColor.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/ConstructionAirBoundary.hpp"
#include "../model/ConstructionAirBoundary_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/BuildingUnit.hpp"
#include "../model/BuildingUnit_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"

#include <tiny_gltf.h>

#include <array>
#include <string>
#include <string_view>
#include <iostream>

namespace openstudio {
namespace gltf {

  GltfMaterialData::GltfMaterialData(std::string_view materialName, const model::RenderingColor& color, bool isDoubleSided)
    : m_materialName(materialName),
      m_r(color.renderingRedValue()),
      m_g(color.renderingGreenValue()),
      m_b(color.renderingBlueValue()),
      m_a(color.renderingAlphaValue() / 255.0),
      m_isDoubleSided(isDoubleSided) {}

  std::string GltfMaterialData::materialName() const {
    return std::string{m_materialName};
  }
  void GltfMaterialData::setMaterialName(const std::string& materialName) {
    m_materialName = materialName;
  };

  int GltfMaterialData::r() const {
    return m_r;
  }
  void GltfMaterialData::setR(int r) {
    m_r = r;
  };

  int GltfMaterialData::g() const {
    return m_g;
  }
  void GltfMaterialData::setG(int g) {
    m_g = g;
  };

  int GltfMaterialData::b() const {
    return m_b;
  }
  void GltfMaterialData::setB(int b) {
    m_b = b;
  };

  double GltfMaterialData::a() const {
    return m_a;
  }
  void GltfMaterialData::setA(double a) {
    m_a = a;
  };

  bool GltfMaterialData::isDoubleSided() const {
    return m_isDoubleSided;
  }
  void GltfMaterialData::setIsDoubleSided(bool isDoubleSided) {
    m_isDoubleSided = isDoubleSided;
  };

  tinygltf::Material GltfMaterialData::toGltf() const {
    tinygltf::Material m;
    m.name = m_materialName;
    tinygltf::PbrMetallicRoughness pbr;
    // this contains the red, green, blue and alpha components of the main color of the material
    pbr.baseColorFactor = {(static_cast<float>(m_r) / 255.0f), (static_cast<float>(m_g) / 255.0f), (static_cast<float>(m_b) / 255.0f),
                           static_cast<float>(m_a)};
    // this indicates that the material should have very minimum reflection characteristics
    // between that of a metal and non-metal material.
    pbr.metallicFactor = 0.0;
    // this makes the material to not be perfectly mirror-like, but instead scatter the
    // reflected light a bit.
    pbr.roughnessFactor = 1.0;
    m.pbrMetallicRoughness = pbr;
    // emissive texture describes the parts of the object surface that emit light with a certain color
    m.emissiveFactor = {0.01, 0.01, 0.00};
    // occlusion texture can be used to simulate the effect of objects self-shadowing each other.
    // m.occlusionTexture = "";
    // normal map is a texture applied to modulate the surface normal in a way that makes
    // it possible to simulate finer geometric details without the cost of a higher mesh.
    // m.normalTexture = "";
    m.alphaMode = (m_a < 1.0f) ? "BLEND" : "OPAQUE";
    // alphaCutoff should not be 1.0f else MATERIAL_ALPHA_CUTOFF_INVALID_MODE
    // Alpha cutoff is supported only for 'MASK' alpha mode
    m.alphaCutoff = 0.5f;
    m.doubleSided = m_isDoubleSided;
    return m;
  }

  // Creates a collection of raw (values: R,G,B,A & isDoubleSided) for all pre-defined GLTF material
  static constexpr std::array<GltfMaterialData, 56> standardGltfMaterialDatas{{
    {"Undefined", 255, 255, 255, 1, true},

    {"NormalMaterial", 255, 255, 255, 1, true},
    {"NormalMaterial_Ext", 255, 255, 255, 1},
    {"NormalMaterial_Int", 255, 0, 0, 1},

    {"Floor", 140, 140, 140, 1, true},
    {"Floor_Ext", 128, 128, 128, 1},
    {"Floor_Int", 191, 191, 191, 1},

    {"Wall", 150, 131, 75, 1, true},
    {"Wall_Ext", 204, 178, 102, 1},
    {"Wall_Int", 235, 226, 197, 1},

    {"RoofCeiling", 112, 56, 57, 1, true},
    {"RoofCeiling_Ext", 153, 76, 76, 1},
    {"RoofCeiling_Int", 202, 149, 149, 1},

    {"Window", 102, 178, 204, 0.6, true},
    {"Window_Ext", 102, 178, 204, 0.6},
    {"Window_Int", 192, 226, 235, 0.6},

    {"Door", 111, 98, 56, 1, true},
    {"Door_Ext", 153, 133, 76, 1},
    {"Door_Int", 202, 188, 149, 1},

    {"SiteShading", 55, 90, 109, 1, true},
    {"SiteShading_Ext", 75, 124, 149, 1},
    {"SiteShading_Int", 187, 209, 220, 1},

    {"BuildingShading", 83, 56, 111, 1, true},
    {"BuildingShading_Ext", 113, 76, 153, 1},
    {"BuildingShading_Int", 216, 203, 229, 1},

    {"SpaceShading", 55, 81, 130, 1, true},
    {"SpaceShading_Ext", 76, 110, 178, 1},
    {"SpaceShading_Int", 183, 197, 224, 1},

    {"InteriorPartitionSurface", 117, 138, 105, 1, true},
    {"InteriorPartitionSurface_Ext", 158, 188, 143, 1},
    {"InteriorPartitionSurface_Int", 213, 226, 207, 1},

    // start textures for boundary conditions
    {"Boundary_Surface", 0, 153, 0, 1, true},
    {"Boundary_Adiabatic", 255, 0, 0, 1, true},
    {"Boundary_Space", 255, 0, 0, 1, true},
    {"Boundary_Outdoors", 163, 204, 204, 1, true},
    {"Boundary_Outdoors_Sun", 40, 204, 204, 1, true},
    {"Boundary_Outdoors_Wind", 9, 159, 162, 1, true},
    {"Boundary_Outdoors_SunWind", 68, 119, 161, 1, true},
    {"Boundary_Ground", 204, 183, 122, 1, true},
    {"Boundary_Foundation", 117, 30, 122, 1, true},
    {"Boundary_Groundfcfactormethod", 153, 122, 30, 1, true},
    {"Boundary_Groundslabpreprocessoraverage", 255, 191, 0, 1, true},
    {"Boundary_Groundslabpreprocessorcore", 255, 182, 50, 1, true},
    {"Boundary_Groundslabpreprocessorperimeter", 255, 178, 101, 1, true},
    {"Boundary_Groundbasementpreprocessoraveragewall", 204, 51, 0, 1, true},
    {"Boundary_Groundbasementpreprocessoraveragefloor", 204, 81, 40, 1, true},
    {"Boundary_Groundbasementpreprocessorupperwall", 204, 112, 81, 1, true},
    {"Boundary_Groundbasementpreprocessorlowerwall", 204, 173, 163, 1, true},
    {"Boundary_Othersidecoefficients", 63, 63, 63, 1, true},
    {"Boundary_Othersideconditionsmodel", 153, 0, 76, 1, true},

    // special rendering materials
    {"SpaceType_Plenum", 192, 192, 192, 0.1, true},
    {"ThermalZone_Plenum", 192, 192, 192, 0.1, true},

    // special rendering materials, these are components or textures in SketchUp
    {"DaylightingControl", 102, 178, 204, 0.1, true},
    {"AirWall", 102, 178, 204, 0.1, true},
    {"SolarCollector", 255, 255, 255, 1, true},
    {"Photovoltaic", 255, 255, 255, 0.1, true},
  }};

  // Static
  std::vector<GltfMaterialData> GltfMaterialData::buildMaterials(const model::Model& model) {

    // We start by putting all standard materials: NOTE: "Undefined" will always be the first one
    std::vector<GltfMaterialData> allMaterials(standardGltfMaterialDatas.begin(), standardGltfMaterialDatas.end());

    std::set<std::string> allMaterialNamesSet;

    auto getOrCreateRenderingColor = [&model](auto& object) {
      auto color_ = object.renderingColor();
      if (!color_) {
        color_ = model::RenderingColor(model);
        // TODO: Uh Oh, taking model as const but mutating objects by adding rendering colors to them...
        object.setRenderingColor(color_.get());
      }
      return color_.get();
    };

    auto getOrCreateMaterial = [&allMaterials, &allMaterialNamesSet](auto& object, const model::RenderingColor& color) {
      std::string materialName = getObjectGLTFMaterialName(object);
      auto [it, hasInserted] = allMaterialNamesSet.insert(materialName);
      if (hasInserted) {
        // instead of creating the material whose node might get not used inshort to avoid
        // warning "UNUSED_OBJECT". we'll add the material to our allMaterial collection
        // and whie processing the surface if required we'll create and the node.
        // createMaterial(materials, color, materialName, true);
        allMaterials.emplace_back(materialName, color, true);
      } else {
        LOG_FREE(Debug, "GltfForwardTranslator", "buildMaterials, Key " << materialName << " found in materialList: " << *it);
      }
    };

    // make construction materials
    for (auto& construction : model.getModelObjects<model::ConstructionBase>()) {
      // If it's ConstructionAirBoundary, we'll later use the standard material "AirWall"
      if (!construction.optionalCast<model::ConstructionAirBoundary>()) {
        getOrCreateMaterial(construction, getOrCreateRenderingColor(construction));
      }
    }

    // make thermal zone materials
    for (auto& thermalZone : model.getConcreteModelObjects<model::ThermalZone>()) {
      getOrCreateMaterial(thermalZone, getOrCreateRenderingColor(thermalZone));
    }

    // make space type materials
    for (auto& spaceType : model.getConcreteModelObjects<model::SpaceType>()) {
      getOrCreateMaterial(spaceType, getOrCreateRenderingColor(spaceType));
    }

    // make building story materials
    for (auto& buildingStory : model.getConcreteModelObjects<model::BuildingStory>()) {
      getOrCreateMaterial(buildingStory, getOrCreateRenderingColor(buildingStory));
    }

    // make building unit materials
    for (auto& buildingUnit : model.getConcreteModelObjects<model::BuildingUnit>()) {
      getOrCreateMaterial(buildingUnit, getOrCreateRenderingColor(buildingUnit));
    }

    // make air loop HVAC materials (AirLoopHVAC doesn't have a renderingColor() method)
    for (auto& airLoopHVAC : model.getConcreteModelObjects<model::AirLoopHVAC>()) {
      getOrCreateMaterial(airLoopHVAC, model::RenderingColor(model));
    }

    return allMaterials;
  }

  std::ostream& operator<<(std::ostream& out, const GltfMaterialData& gltfMaterialData) {
    out << "GltfMaterialData(r=" << gltfMaterialData.r() << ", g=" << gltfMaterialData.g() << ", b=" << gltfMaterialData.b()
        << ", a=" << gltfMaterialData.a();
    if (gltfMaterialData.isDoubleSided()) {
      out << ", Double Sided";
    }
    out << ")";
    return out;
  }

}  // namespace gltf
}  // namespace openstudio
