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

#include "ThreeJS.hpp"

#include "../core/Assert.hpp"
#include "../core/Path.hpp"
#include "../core/Json.hpp"

#include <jsoncpp/json.h>

#include <iostream>
#include <string>

namespace openstudio{
  
  unsigned toThreeColor(unsigned r, unsigned g, unsigned b)
  {
    return 65536 * r + 256 * g + b;
  }

  std::string toThreeUUID(const std::string& uuid)
  {
    // uuid.to_s.gsub('{','').gsub('}','')
    if (uuid.size() > 2){
      if ((uuid[0] == '{') && (uuid[uuid.size() - 1] == '}')){
        return uuid.substr(1, uuid.size() - 2);
      }
    }
    return uuid;
  }

  std::string fromThreeUUID(const std::string& uuid)
  {
    if (uuid.size() > 2){
      if ((uuid[0] != '{') && (uuid[uuid.size() - 1] != '}')){
        return "{" + uuid + "}";
      }
    }
    return uuid;
  }

  std::vector<double> toThreeVector(const Point3dVector& vertices)
  {
    std::vector<double> result;
    result.reserve(vertices.size() * 3);
    for (const auto& vertex : vertices){
      result.push_back(vertex.x());
      result.push_back(vertex.z());
      result.push_back(-vertex.y());
    }
    return result;
  }

  Point3dVector fromThreeVector(const std::vector<double>& vertices)
  {
    Point3dVector result;
    size_t n = vertices.size();
    for (size_t i = 0; i + 2 < n; i+=3){
      result.push_back(Point3d(vertices[i], -vertices[i + 2], vertices[i + 1]));
    }
    return result;
  }

  std::vector<double> toThreeMatrix(const Transformation& matrix)
  {
    return std::vector<double>();
  }

  Transformation toThreeMatrix(const std::vector<double>& matrix)
  {
    return Transformation();
  }

  ThreeScene::ThreeScene(const ThreeSceneMetadata& metadata, const std::vector<ThreeGeometry>& geometries, const std::vector<ThreeMaterial>& materials, const ThreeSceneObject& sceneObject)
    : m_metadata(metadata), m_geometries(geometries), m_materials(materials), m_sceneObject(sceneObject)
  {
  }
    
  ThreeScene::ThreeScene(const std::string& s)
    : m_metadata(std::vector<std::string>(), ThreeBoundingBox(0,0,0,0,0,0,0,0,0,0), std::vector<ThreeModelObjectMetadata>()), m_sceneObject(ThreeSceneObject("", std::vector<ThreeSceneChild>()))
  {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s, root);

    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();

      // see if this is a path
      openstudio::path p = toPath(s);
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)){
        // open file
        std::ifstream ifs(openstudio::toString(p));
        root.clear();
        parsingSuccessful = reader.parse(ifs, root);
      }

      if (!parsingSuccessful){
        LOG_AND_THROW("ThreeJS JSON cannot be processed, " << errors);
      }
    }

    assertKeyAndType(root, "metadata", Json::objectValue);
    assertKeyAndType(root, "geometries", Json::arrayValue);
    assertKeyAndType(root, "materials", Json::arrayValue);
    assertKeyAndType(root, "object", Json::objectValue);

    m_metadata = ThreeSceneMetadata(root.get("metadata", Json::objectValue));

    Json::Value geometries = root.get("geometries", Json::arrayValue);
    for (const auto& g : geometries) {
      m_geometries.push_back(ThreeGeometry(g));
    }

    Json::Value materials = root.get("materials", Json::arrayValue);
    for (const auto& m : materials) {
      m_materials.push_back(ThreeMaterial(m));
    }

    m_sceneObject = ThreeSceneObject(root.get("object", Json::objectValue));
  }
    
  boost::optional<ThreeScene> ThreeScene::load(const std::string& json)
  {  
    try {
      ThreeScene scene(json);
      return scene;
    } catch (...) {
      LOG(Error, "Could not parse JSON input");
    }
    return boost::none;
  }

  std::string ThreeScene::toJSON(bool prettyPrint) const
  {
    Json::Value scene(Json::objectValue);

    // metadata
    scene["metadata"] = m_metadata.toJsonValue();

    // geometries
    Json::Value geometries(Json::arrayValue);
    for (const auto& g : m_geometries) {
      geometries.append(g.toJsonValue());
    }
    scene["geometries"] = geometries;

    // materials
    Json::Value materials(Json::arrayValue);
    for (const auto& m : m_materials){
      materials.append(m.toJsonValue());
    }
    scene["materials"] = materials;

    // object
    scene["object"] = m_sceneObject.toJsonValue();

    // write to string
    std::string result;
    if (prettyPrint){
      Json::StyledWriter writer;
      result = writer.write(scene);
    } else{
      Json::FastWriter writer;
      result = writer.write(scene);
    }

    return result;
  }

  ThreeSceneMetadata ThreeScene::metadata() const
  {
    return m_metadata;
  }

  std::vector<ThreeGeometry> ThreeScene::geometries() const
  {
    return m_geometries;
  }
  
  boost::optional<ThreeGeometry> ThreeScene::getGeometry(const std::string& geometryId) const
  {
    for (const auto& geometry : m_geometries){
      if (geometry.uuid() == geometryId){
        return geometry;
      }
    }
    return boost::none;
  }

  std::vector<ThreeMaterial> ThreeScene::materials() const
  {
    return m_materials;
  }

  boost::optional<ThreeMaterial> ThreeScene::getMaterial(const std::string& materialId) const
  {
    for (const auto& material : m_materials){
      if (material.uuid() == materialId){
        return material;
      }
    }
    return boost::none;
  }
  
  ThreeSceneObject ThreeScene::object() const
  {
    return m_sceneObject;
  }

  ThreeGeometryData::ThreeGeometryData(const std::vector<double>& vertices, const std::vector<size_t>& faces)
    : m_vertices(vertices), m_normals(), m_uvs(), m_faces(faces), m_scale(1.0), m_visible(true), m_castShadow(true), m_receiveShadow(true), m_doubleSided(true)
  {}


  ThreeGeometryData::ThreeGeometryData(const Json::Value& value)
  {
    assertKeyAndType(value, "vertices", Json::arrayValue);
    assertKeyAndType(value, "normals", Json::arrayValue);
    assertKeyAndType(value, "uvs", Json::arrayValue);
    assertKeyAndType(value, "faces", Json::arrayValue);
    assertKeyAndType(value, "scale", Json::realValue);
    assertKeyAndType(value, "visible", Json::booleanValue);
    assertKeyAndType(value, "castShadow", Json::booleanValue);
    assertKeyAndType(value, "receiveShadow", Json::booleanValue);
    assertKeyAndType(value, "doubleSided", Json::booleanValue);

    Json::Value scale = value.get("scale", 1.0);
    if (scale.isConvertibleTo(Json::realValue)){
      m_scale = scale.asDouble();
    } else{
      m_scale = 1.0;
    }
    m_visible = value.get("visible", true).asBool();
    m_castShadow = value.get("castShadow", true).asBool();
    m_receiveShadow = value.get("receiveShadow", true).asBool();
    m_doubleSided = value.get("doubleSided", true).asBool();

    Json::Value vertices = value.get("vertices", Json::arrayValue);
    Json::ArrayIndex n = vertices.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_vertices.push_back(vertices[i].asDouble());
    }

    Json::Value normals = value.get("normals", Json::arrayValue);
    n = normals.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_normals.push_back(normals[i].asDouble());
    }

    Json::Value uvs = value.get("uvs", Json::arrayValue);
    n = uvs.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_uvs.push_back(uvs[i].asDouble());
    }

    Json::Value faces = value.get("faces", Json::arrayValue);
    n = faces.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_faces.push_back(faces[i].asDouble());
    }

  }

  Json::Value ThreeGeometryData::toJsonValue() const
  {
    Json::Value result;

    Json::Value vertices(Json::arrayValue);
    for (const auto& v : m_vertices){
      vertices.append(v);
    }

    Json::Value normals(Json::arrayValue);

    Json::Value uvs(Json::arrayValue);

    Json::Value faces(Json::arrayValue);
    for (const size_t& f : m_faces){
      faces.append(static_cast<unsigned>(f));
    }

    result["vertices"] = vertices;
    result["normals"] = normals;
    result["uvs"] = uvs;
    result["faces"] = faces;
    result["scale"] = m_scale;
    result["visible"] = m_visible;
    result["castShadow"] = m_castShadow;
    result["receiveShadow"] = m_receiveShadow;
    result["doubleSided"] = m_doubleSided;

    return result;
  }


  std::vector<double> ThreeGeometryData::vertices() const
  {
    return m_vertices;
  }

  std::vector<size_t> ThreeGeometryData::normals() const
  {
    return m_normals;
  }

  std::vector<size_t> ThreeGeometryData::uvs() const
  {
    return m_uvs;
  }

  std::vector<size_t> ThreeGeometryData::faces() const
  {
    return m_faces;
  }

  double ThreeGeometryData::scale() const
  {
    return m_scale;
  }

  bool ThreeGeometryData::visible() const
  {
    return m_visible;
  }

  bool ThreeGeometryData::castShadow() const
  {
    return m_castShadow;
  }

  bool ThreeGeometryData::receiveShadow() const
  {
    return m_receiveShadow;
  }

  bool ThreeGeometryData::doubleSided() const
  {
    return m_doubleSided;
  }
  
  ThreeGeometry::ThreeGeometry(const std::string& uuid, const::std::string& type, const ThreeGeometryData& data)
    : m_uuid(uuid), m_type(type), m_data(data)
   {}

  ThreeGeometry::ThreeGeometry(const Json::Value& value)
    : m_data(value.get("data", Json::objectValue))
  {
    assertKeyAndType(value, "data", Json::objectValue);
    assertKeyAndType(value, "uuid", Json::stringValue);
    assertKeyAndType(value, "type", Json::stringValue);
    m_uuid = value.get("uuid", "").asString();
    m_type = value.get("type", "").asString();
  }

  Json::Value ThreeGeometry::toJsonValue() const
  {
    Json::Value result(Json::objectValue);
    result["uuid"] = m_uuid;
    result["type"] = m_type;
    result["data"] = m_data.toJsonValue();

    return result;
  }

   std::string ThreeGeometry::uuid() const
   {
     return m_uuid;
   }
    
   std::string ThreeGeometry::type() const
   {
     return m_type;
   }

   ThreeGeometryData ThreeGeometry::data() const
   {
     return m_data;
   }

  ThreeMaterial::ThreeMaterial(const std::string& uuid, const std::string& name, const::std::string& type,
      unsigned color, unsigned ambient, unsigned emissive, unsigned specular, unsigned shininess,
      double opacity, bool transparent, bool wireframe, unsigned side)
      : m_uuid(uuid), m_name(name), m_type(type), m_color(color), m_ambient(ambient), m_emissive(emissive), 
      m_specular(specular), m_shininess(shininess), m_opacity(opacity), m_transparent(transparent),
      m_wireframe(wireframe), m_side(side)
  {}

  ThreeMaterial::ThreeMaterial(const Json::Value& value)
  {
    assertKeyAndType(value, "uuid", Json::stringValue);
    assertKeyAndType(value, "name", Json::stringValue);
    assertKeyAndType(value, "type", Json::stringValue);
    assertKeyAndType(value, "color", Json::uintValue);
    assertType(value, "ambient", Json::uintValue);
    assertType(value, "emissive", Json::uintValue);
    assertType(value, "specular", Json::uintValue);
    assertType(value, "shininess", Json::uintValue);
    assertType(value, "opacity", Json::realValue);
    assertType(value, "transparent", Json::booleanValue);
    assertType(value, "wireframe", Json::booleanValue);
    assertKeyAndType(value, "side", Json::uintValue);

    m_uuid = value.get("uuid", "").asString();
    m_name = value.get("name", "").asString();
    m_type = value.get("type", "").asString();
    m_color = value.get("color", 0).asUInt();
    m_ambient = value.get("ambient", m_color).asUInt();
    m_emissive = value.get("emissive", m_color).asUInt();
    m_specular = value.get("specular", m_color).asUInt();
    m_shininess = value.get("shininess", 50).asUInt();
    m_opacity = value.get("opacity", 1).asDouble();
    m_transparent = value.get("transparent", false).asBool();
    m_wireframe = value.get("wireframe", false).asBool();
    m_side = value.get("side", 0).asUInt();
  }

  Json::Value ThreeMaterial::toJsonValue() const
  {
    Json::Value result(Json::objectValue);

    result["uuid"] = m_uuid;
    result["name"] = m_name;
    result["type"] = m_type;
    result["color"] = m_color;
    result["ambient"] = m_ambient;
    result["emissive"] = m_emissive;
    result["specular"] = m_specular;
    result["shininess"] = m_shininess;
    result["opacity"] = m_opacity;
    result["transparent"] = m_transparent;
    result["wireframe"] = m_wireframe;
    result["side"] = m_side;

    return result;
  }


  std::string ThreeMaterial::uuid() const
  {
    return m_uuid;
  }

  std::string ThreeMaterial::name() const
  {
    return m_name;
  }

  std::string ThreeMaterial::type() const
  {
    return m_type;
  }

  unsigned ThreeMaterial::color() const
  {
    return m_color;
  }

  unsigned ThreeMaterial::ambient() const
  {
    return m_ambient;
  }

  unsigned ThreeMaterial::emissive() const
  {
    return m_emissive;
  }

  unsigned ThreeMaterial::specular() const
  {
    return m_specular;
  }

  unsigned ThreeMaterial::shininess() const
  {
    return m_shininess;
  }

  double ThreeMaterial::opacity() const
  {
    return m_opacity;
  }

  bool ThreeMaterial::transparent() const
  {
    return m_transparent;
  }

  bool ThreeMaterial::wireframe() const
  {
    return m_wireframe;
  }

  unsigned ThreeMaterial::side() const
  {
    return m_side;
  }

  ThreeUserData::ThreeUserData()
    : m_coincidentWithOutsideObject(false)
      //m_belowFloorPlenum(false),
      //m_aboveCeilingPlenum(false)
  {}

  ThreeUserData::ThreeUserData(const Json::Value& value)
  {
    assertType(value, "handle", Json::stringValue);
    assertType(value, "name", Json::stringValue);
    assertType(value, "surfaceType", Json::stringValue);
    assertType(value, "surfaceTypeMaterialName", Json::stringValue);
    assertType(value, "constructionName", Json::stringValue);
    assertType(value, "constructionHandle", Json::stringValue);
    assertType(value, "constructionMaterialName", Json::stringValue);
    assertType(value, "spaceName", Json::stringValue);
    assertType(value, "spaceHandle", Json::stringValue);
    assertType(value, "thermalZoneName", Json::stringValue);
    assertType(value, "thermalZoneHandle", Json::stringValue);
    assertType(value, "thermalZoneMaterialName", Json::stringValue);
    assertType(value, "spaceTypeName", Json::stringValue);
    assertType(value, "spaceTypeHandle", Json::stringValue);
    assertType(value, "spaceTypeMaterialName", Json::stringValue);
    assertType(value, "buildingStoryName", Json::stringValue);
    assertType(value, "buildingStoryHandle", Json::stringValue);
    assertType(value, "buildingStoryMaterialName", Json::stringValue);
    assertType(value, "buildingUnitName", Json::stringValue);
    assertType(value, "buildingUnitHandle", Json::stringValue);
    assertType(value, "buildingUnitMaterialName", Json::stringValue);
    assertType(value, "constructionSetName", Json::stringValue);
    assertType(value, "constructionSetHandle", Json::stringValue);
    assertType(value, "constructionSetMaterialName", Json::stringValue);
    assertType(value, "boundaryMaterialName", Json::stringValue);
    assertType(value, "outsideBoundaryCondition", Json::stringValue);
    assertType(value, "outsideBoundaryConditionObjectName", Json::stringValue);
    assertType(value, "outsideBoundaryConditionObjectHandle", Json::stringValue);
    assertType(value, "coincidentWithOutsideObject", Json::stringValue);
    assertType(value, "sunExposure", Json::stringValue);
    assertType(value, "windExposure", Json::stringValue);
    assertType(value, "belowFloorPlenum", Json::booleanValue);
    assertType(value, "aboveCeilingPlenum", Json::booleanValue);

    m_handle = value.get("handle", "").asString();
    m_name = value.get("name", "").asString();
    m_surfaceType = value.get("surfaceType", "").asString();
    m_surfaceTypeMaterialName = value.get("surfaceTypeMaterialName", "").asString();
    m_constructionName = value.get("constructionName", "").asString();
    m_constructionHandle = value.get("constructionHandle", "").asString();
    m_constructionMaterialName = value.get("constructionMaterialName", "").asString();
    m_spaceName = value.get("spaceName", "").asString();
    m_spaceHandle = value.get("spaceHandle", "").asString();
    m_thermalZoneName = value.get("thermalZoneName", "").asString();
    m_thermalZoneHandle = value.get("thermalZoneHandle", "").asString();
    m_thermalZoneMaterialName = value.get("thermalZoneMaterialName", "").asString();
    m_spaceTypeName = value.get("spaceTypeName", "").asString();
    m_spaceTypeHandle = value.get("spaceTypeHandle", "").asString();
    m_spaceTypeMaterialName = value.get("spaceTypeMaterialName", "").asString();
    m_buildingStoryName = value.get("buildingStoryName", "").asString();
    m_buildingStoryHandle = value.get("buildingStoryHandle", "").asString();
    m_buildingStoryMaterialName = value.get("buildingStoryMaterialName", "").asString();
    m_buildingUnitName = value.get("buildingUnitName", "").asString();
    m_buildingUnitHandle = value.get("buildingUnitHandle", "").asString();
    m_buildingUnitMaterialName = value.get("buildingUnitMaterialName", "").asString();
    m_constructionSetName = value.get("constructionSetName", "").asString();
    m_constructionSetHandle = value.get("constructionSetHandle", "").asString();
    m_constructionSetMaterialName = value.get("constructionSetMaterialName", "").asString();
    m_boundaryMaterialName = value.get("boundaryMaterialName", "").asString();
    m_outsideBoundaryCondition = value.get("outsideBoundaryCondition", "").asString();
    m_outsideBoundaryConditionObjectName = value.get("outsideBoundaryConditionObjectName", "").asString();
    m_outsideBoundaryConditionObjectHandle = value.get("outsideBoundaryConditionObjectHandle", "").asString();
    m_coincidentWithOutsideObject = value.get("coincidentWithOutsideObject", false).asBool();
    m_sunExposure = value.get("sunExposure", "").asString();
    m_windExposure = value.get("windExposure", "").asString();
    //m_belowFloorPlenum = value.get("belowFloorPlenum", "").asBool();
    //m_aboveCeilingPlenum = value.get("aboveCeilingPlenum", "").asBool();
  }

  Json::Value ThreeUserData::toJsonValue() const
  {
    Json::Value result(Json::objectValue);

    result["handle"] = m_handle;
    result["name"] = m_name;
    result["surfaceType"] = m_surfaceType;
    result["surfaceTypeMaterialName"] = m_surfaceTypeMaterialName;
    result["constructionName"] = m_constructionName;
    result["constructionName"] = m_constructionName;
    result["constructionMaterialName"] = m_constructionMaterialName;
    result["spaceName"] = m_spaceName;
    result["spaceHandle"] = m_spaceHandle;
    result["thermalZoneName"] = m_thermalZoneName;
    result["thermalZoneHandle"] = m_thermalZoneHandle;
    result["thermalZoneMaterialName"] = m_thermalZoneMaterialName;
    result["spaceTypeName"] = m_spaceTypeName;
    result["spaceTypeHandle"] = m_spaceTypeHandle;
    result["spaceTypeMaterialName"] = m_spaceTypeMaterialName;
    result["buildingStoryName"] = m_buildingStoryName;
    result["buildingStoryHandle"] = m_buildingStoryHandle;
    result["buildingStoryMaterialName"] = m_buildingStoryMaterialName;
    result["buildingUnitName"] = m_buildingUnitName;
    result["buildingUnitHandle"] = m_buildingUnitHandle;
    result["buildingUnitMaterialName"] = m_buildingUnitMaterialName;
    result["constructionSetName"] = m_constructionSetName;
    result["constructionSetHandle"] = m_constructionSetHandle;
    result["constructionSetMaterialName"] = m_constructionSetMaterialName;
    result["boundaryMaterialName"] = m_boundaryMaterialName;
    result["outsideBoundaryCondition"] = m_outsideBoundaryCondition;
    result["outsideBoundaryConditionObjectName"] = m_outsideBoundaryConditionObjectName;
    result["outsideBoundaryConditionObjectHandle"] = m_outsideBoundaryConditionObjectHandle;
    result["coincidentWithOutsideObject"] = m_coincidentWithOutsideObject;
    result["sunExposure"] = m_sunExposure;
    result["windExposure"] = m_windExposure;
    //result["belowFloorPlenum"] = m_belowFloorPlenum;
    //result["aboveCeilingPlenum"] = m_aboveCeilingPlenum;

    return result;
  }

  std::string ThreeUserData::handle() const
  {
    return m_handle;
  }

  std::string ThreeUserData::name() const
  {
    return m_name;
  }

  std::string ThreeUserData::surfaceType() const
  {
    return m_surfaceType;
  }

  std::string ThreeUserData::surfaceTypeMaterialName() const
  {
    return m_surfaceTypeMaterialName;
  }

  std::string ThreeUserData::constructionName() const
  {
    return m_constructionName;
  }

  std::string ThreeUserData::constructionHandle() const
  {
    return m_constructionHandle;
  }

  std::string ThreeUserData::constructionMaterialName() const
  {
    return m_constructionMaterialName;
  }

  std::string ThreeUserData::spaceName() const
  {
    return m_spaceName;
  }

  std::string ThreeUserData::spaceHandle() const
  {
    return m_spaceHandle;
  }

  std::string ThreeUserData::thermalZoneName() const
  {
    return m_thermalZoneName;
  }

  std::string ThreeUserData::thermalZoneHandle() const
  {
    return m_thermalZoneHandle;
  }

  std::string ThreeUserData::thermalZoneMaterialName() const
  {
    return m_thermalZoneMaterialName;
  }

  std::string ThreeUserData::spaceTypeName() const
  {
    return m_spaceTypeName;
  }

  std::string ThreeUserData::spaceTypeHandle() const
  {
    return m_spaceTypeHandle;
  }

  std::string ThreeUserData::spaceTypeMaterialName() const
  {
    return m_spaceTypeMaterialName;
  }

  std::string ThreeUserData::buildingStoryName() const
  {
    return m_buildingStoryName;
  }

  std::string ThreeUserData::buildingStoryHandle() const
  {
    return m_buildingStoryHandle;
  }

  std::string ThreeUserData::buildingStoryMaterialName() const
  {
    return m_buildingStoryMaterialName;
  }

  std::string ThreeUserData::buildingUnitName() const
  {
    return m_buildingUnitName;
  }

    std::string ThreeUserData::buildingUnitHandle() const
  {
    return m_buildingUnitHandle;
  }

  std::string ThreeUserData::buildingUnitMaterialName() const
  {
    return m_buildingUnitMaterialName;
  }

  std::string ThreeUserData::constructionSetName() const
  {
    return m_constructionSetName;
  }

    std::string ThreeUserData::constructionSetHandle() const
  {
    return m_constructionSetHandle;
  }

  std::string ThreeUserData::constructionSetMaterialName() const
  {
    return m_constructionSetMaterialName;
  }

  std::string ThreeUserData::outsideBoundaryCondition() const
  {
    return m_outsideBoundaryCondition;
  }

  std::string ThreeUserData::outsideBoundaryConditionObjectName() const
  {
    return m_outsideBoundaryConditionObjectName;
  }

  std::string ThreeUserData::outsideBoundaryConditionObjectHandle() const
  {
    return m_outsideBoundaryConditionObjectHandle;
  }

  std::string ThreeUserData::boundaryMaterialName() const
  {
    return m_boundaryMaterialName;
  }

  bool ThreeUserData::coincidentWithOutsideObject() const
  {
    return m_coincidentWithOutsideObject;
  }

  std::string ThreeUserData::sunExposure() const
  {
    return m_sunExposure;
  }

  std::string ThreeUserData::windExposure() const
  {
    return m_windExposure;
  }

  //bool ThreeUserData::plenum() const
  //{
  //  return (m_belowFloorPlenum || m_aboveCeilingPlenum);
  //}

  //bool ThreeUserData::belowFloorPlenum() const
  //{
  //  return m_belowFloorPlenum;
  //}

  //bool ThreeUserData::aboveCeilingPlenum() const
  //{
  //  return m_aboveCeilingPlenum;
  //}

  void ThreeUserData::setHandle(const std::string& s)
  {
    m_handle = s;
  }

  void ThreeUserData::setName(const std::string& s)
  {
    m_name = s;
  }

  void ThreeUserData::setSurfaceType(const std::string& s)
  {
    m_surfaceType = s;
  }

  void ThreeUserData::setSurfaceTypeMaterialName(const std::string& s)
  {
    m_surfaceTypeMaterialName = s;
  }

  void ThreeUserData::setConstructionName(const std::string& s)
  {
    m_constructionName = s;
  }

  void ThreeUserData::setConstructionHandle(const std::string& s)
  {
    m_constructionHandle = s;
  }

  void ThreeUserData::setConstructionMaterialName(const std::string& s)
  {
    m_constructionMaterialName = s;
  }

  void ThreeUserData::setSpaceName(const std::string& s)
  {
    m_spaceName = s;
  }

  void ThreeUserData::setSpaceHandle(const std::string& s)
  {
    m_spaceHandle = s;
  }

  void ThreeUserData::setThermalZoneName(const std::string& s)
  {
    m_thermalZoneName = s;
  }

  void ThreeUserData::setThermalZoneHandle(const std::string& s)
  {
    m_thermalZoneHandle = s;
  }

  void ThreeUserData::setThermalZoneMaterialName(const std::string& s)
  {
    m_thermalZoneMaterialName = s;
  }

  void ThreeUserData::setSpaceTypeName(const std::string& s)
  {
    m_spaceTypeName = s;
  }
    
  void ThreeUserData::setSpaceTypeHandle(const std::string& s)
  {
    m_spaceTypeHandle = s;
  }

  void ThreeUserData::setSpaceTypeMaterialName(const std::string& s)
  {
    m_spaceTypeMaterialName = s;
  }

  void ThreeUserData::setBuildingStoryName(const std::string& s)
  {
    m_buildingStoryName = s;
  }
    
  void ThreeUserData::setBuildingStoryHandle(const std::string& s)
  {
    m_buildingStoryHandle = s;
  }

  void ThreeUserData::setBuildingStoryMaterialName(const std::string& s)
  {
    m_buildingStoryMaterialName = s;
  }

  void ThreeUserData::setBuildingUnitName(const std::string& s)
  {
    m_buildingUnitName = s;
  }
    
  void ThreeUserData::setBuildingUnitHandle(const std::string& s)
  {
    m_buildingUnitHandle = s;
  }

  void ThreeUserData::setBuildingUnitMaterialName(const std::string& s)
  {
    m_buildingUnitMaterialName = s;
  }

  void ThreeUserData::setConstructionSetName(const std::string& s)
  {
    m_constructionSetName = s;
  }
    
  void ThreeUserData::setConstructionSetHandle(const std::string& s)
  {
    m_constructionSetHandle = s;
  }

  void ThreeUserData::setConstructionSetMaterialName(const std::string& s)
  {
    m_constructionSetMaterialName = s;
  }

  void ThreeUserData::setOutsideBoundaryCondition(const std::string& s)
  {
    m_outsideBoundaryCondition = s;
  }

  void ThreeUserData::setOutsideBoundaryConditionObjectName(const std::string& s)
  {
    m_outsideBoundaryConditionObjectName = s;
  }

  void ThreeUserData::setOutsideBoundaryConditionObjectHandle(const std::string& s)
  {
    m_outsideBoundaryConditionObjectHandle = s;
  }

  void ThreeUserData::setBoundaryMaterialName(const std::string& s)
  {
    m_boundaryMaterialName = s;
  }

  void ThreeUserData::setCoincidentWithOutsideObject(bool b)
  {
    m_coincidentWithOutsideObject = b;
  }

  void ThreeUserData::setSunExposure(const std::string& s)
  {
    m_sunExposure = s;
  }

  void ThreeUserData::setWindExposure(const std::string& s)
  {
    m_windExposure = s;
  }

  //void ThreeUserData::setBelowFloorPlenum(bool v)
  //{
  //  m_belowFloorPlenum = v;
  //  if (v){
  //    m_aboveCeilingPlenum = false;
  //  }
  //}

  //void ThreeUserData::setAboveCeilingPlenum(bool v)
  //{
  //  m_aboveCeilingPlenum = v;
  //  if (v){
  //    m_belowFloorPlenum = false;
  //  }
  //}

  ThreeSceneChild::ThreeSceneChild(const std::string& uuid, const std::string& name, const std::string& type,
                    const std::string& geometryId, const std::string& materialId, const ThreeUserData& userData)
                    : m_uuid(uuid), m_name(name), m_type(type), m_geometryId(geometryId), m_materialId(materialId), m_matrix({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}), m_userData(userData)
  {}

  ThreeSceneChild::ThreeSceneChild(const Json::Value& value)
  {
    assertKeyAndType(value, "uuid", Json::stringValue);
    assertKeyAndType(value, "name", Json::stringValue);
    assertKeyAndType(value, "type", Json::stringValue);
    assertKeyAndType(value, "geometry", Json::stringValue);
    assertKeyAndType(value, "material", Json::stringValue);
    assertKeyAndType(value, "matrix", Json::arrayValue);
    assertKeyAndType(value, "userData", Json::objectValue);

    m_uuid = value.get("uuid", "").asString();
    m_name = value.get("name", "").asString();
    m_type = value.get("type", "").asString();
    m_geometryId = value.get("geometry", "").asString();
    m_materialId = value.get("material", "").asString();

    Json::Value matrix = value.get("matrix", Json::arrayValue);
    Json::ArrayIndex n = matrix.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_matrix.push_back(matrix[i].asDouble());
    }

    m_userData = ThreeUserData(value.get("userData", Json::objectValue));
  }

  Json::Value ThreeSceneChild::toJsonValue() const
  {
    Json::Value result(Json::objectValue);

    Json::Value childMatrix(Json::arrayValue);
    for (const auto& d : m_matrix){
      childMatrix.append(d);
    }

    result["uuid"] = m_uuid;
    result["name"] = m_name;
    result["type"] = m_type;
    result["geometry"] = m_geometryId;
    result["material"] = m_materialId;
    result["matrix"] = childMatrix;
    result["userData"] = m_userData.toJsonValue();

    return result;
  }

  std::string ThreeSceneChild::uuid() const
  {
    return m_uuid;
  }

  std::string ThreeSceneChild::name() const
  {
    return m_name;
  }
    
  std::string ThreeSceneChild::type() const
  {
    return m_type;
  }

  std::string ThreeSceneChild::geometry() const
  {
    return m_geometryId;
  }

  std::string ThreeSceneChild::material() const
  {
    return m_materialId;
  }

  std::vector<double> ThreeSceneChild::matrix() const
  {
    return m_matrix;
  }

  ThreeUserData ThreeSceneChild::userData() const
  {
    return m_userData;
  }

  ThreeSceneObject::ThreeSceneObject(const std::string& uuid, const std::vector<ThreeSceneChild>& children)
    : m_uuid(uuid), m_type("Scene"), m_matrix({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}), m_children(children)
  {}

  ThreeSceneObject::ThreeSceneObject(const Json::Value& value)
  {
    assertKeyAndType(value, "uuid", Json::stringValue);
    assertKeyAndType(value, "type", Json::stringValue);
    assertKeyAndType(value, "children", Json::arrayValue);
    assertKeyAndType(value, "matrix", Json::arrayValue);

    m_uuid = value.get("uuid", "").asString();
    m_type = value.get("type", "").asString();

    Json::Value children = value.get("children", Json::arrayValue);
    Json::ArrayIndex n = children.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_children.push_back(ThreeSceneChild(children[i]));
    }

    Json::Value matrix = value.get("matrix", Json::arrayValue);
    n = matrix.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_matrix.push_back(matrix[i].asDouble());
    }
  }

  Json::Value ThreeSceneObject::toJsonValue() const
  {
    Json::Value result;

    Json::Value sceneChildren(Json::arrayValue);
    for (const auto& c : m_children){
      sceneChildren.append(c.toJsonValue());
    }

    Json::Value sceneMatrix(Json::arrayValue);
    for (const auto& d : m_matrix){
      sceneMatrix.append(d);
    }

    result["uuid"] = m_uuid;
    result["type"] = m_type;
    result["matrix"] = sceneMatrix;
    result["children"] = sceneChildren;

    return result;
  }

  std::string ThreeSceneObject::uuid() const
  {
    return m_uuid;
  }
    
  std::string ThreeSceneObject::type() const
  {
    return m_type;
  }

  std::vector<double> ThreeSceneObject::matrix() const
  {
    return m_matrix;
  }

  std::vector<ThreeSceneChild> ThreeSceneObject::children() const
  {
    return m_children;
  }

  ThreeBoundingBox::ThreeBoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ,
                                     double lookAtX, double lookAtY, double lookAtZ, double lookAtR)
    : m_minX(minX), m_minY(minY), m_minZ(minZ), m_maxX(maxX), m_maxY(maxY), m_maxZ(maxZ),
      m_lookAtX(lookAtX), m_lookAtY(lookAtY), m_lookAtZ(lookAtZ), m_lookAtR(lookAtR)
  {}

  ThreeBoundingBox::ThreeBoundingBox(const Json::Value& value)
  {

  }

  Json::Value ThreeBoundingBox::toJsonValue() const
  {
    Json::Value result(Json::objectValue);
    result["minX"] = m_minX;
    result["minY"] = m_minY;
    result["minZ"] = m_minZ;
    result["maxX"] = m_maxX;
    result["maxY"] = m_maxY;
    result["maxZ"] = m_maxZ;
    result["lookAtX"] = m_lookAtX;
    result["lookAtY"] = m_lookAtY;
    result["lookAtZ"] = m_lookAtZ;
    result["lookAtR"] = m_lookAtR;

    return result;
  }

  double ThreeBoundingBox::minX() const
  {
    return m_minX;
  }

  double ThreeBoundingBox::minY() const
  {
    return m_minY;
  }

  double ThreeBoundingBox::minZ() const
  {
    return m_minZ;
  }

  double ThreeBoundingBox::maxX() const
  {
    return m_maxX;
  }

  double ThreeBoundingBox::maxY() const
  {
    return m_maxY;
  }

  double ThreeBoundingBox::maxZ() const
  {
    return m_maxZ;
  }

  double ThreeBoundingBox::lookAtX() const
  {
    return m_lookAtX;
  }

  double ThreeBoundingBox::lookAtY() const
  {
    return m_lookAtY;
  }

  double ThreeBoundingBox::lookAtZ() const
  {
    return m_lookAtZ;
  }

  double ThreeBoundingBox::lookAtR() const
  {
    return m_lookAtR;
  }

  ThreeModelObjectMetadata::ThreeModelObjectMetadata(const std::string& iddObjectType, const std::string& handle, const std::string& name)
    : m_iddObjectType(iddObjectType), m_handle(handle), m_name(name)
  {}

  ThreeModelObjectMetadata::ThreeModelObjectMetadata()
  {}
  
  ThreeModelObjectMetadata::ThreeModelObjectMetadata(const Json::Value& value)
  {
    assertKeyAndType(value, "iddObjectType", Json::stringValue);
    assertKeyAndType(value, "handle", Json::stringValue);
    assertKeyAndType(value, "name", Json::stringValue);

    m_iddObjectType = value.get("iddObjectType", "").asString();
    m_handle = value.get("handle", "").asString();
    m_name = value.get("name", "").asString();
  }

  std::string ThreeModelObjectMetadata::iddObjectType() const
  {
    return m_iddObjectType;
  }

  std::string ThreeModelObjectMetadata::handle() const
  {
    return m_handle;
  }

  std::string ThreeModelObjectMetadata::name() const
  {
    return m_name;
  }

  Json::Value ThreeModelObjectMetadata::toJsonValue() const
  {
    Json::Value result;

    result["iddObjectType"] = m_iddObjectType;
    result["handle"] = m_handle;
    result["name"] = m_name;

    return result;
  }


  ThreeSceneMetadata::ThreeSceneMetadata(const std::vector<std::string>& buildingStoryNames, const ThreeBoundingBox& boundingBox, const std::vector<ThreeModelObjectMetadata>& modelObjectMetadata)
    : m_version("4.3"), m_type("Object"), m_generator("OpenStudio"), m_buildingStoryNames(buildingStoryNames), m_boundingBox(boundingBox), m_modelObjectMetadata(modelObjectMetadata)
  {}

  ThreeSceneMetadata::ThreeSceneMetadata(const Json::Value& value)
    : m_boundingBox(value.get("boundinmgBox", Json::objectValue))
  {
    assertKeyAndType(value, "version", Json::stringValue);
    assertKeyAndType(value, "type", Json::stringValue);
    assertKeyAndType(value, "generator", Json::stringValue);
    assertKeyAndType(value, "buildingStoryNames", Json::arrayValue);
    assertKeyAndType(value, "boundingBox", Json::objectValue);
    assertKeyAndType(value, "modelObjectMetadata", Json::arrayValue);

    Json::Value version = value.get("version", "");
    if (version.isConvertibleTo(Json::stringValue)){
      m_version = version.asString();
    }
    m_type = value.get("type", "").asString();
    m_generator = value.get("generator", "").asString();

    Json::Value buildingStoryNames = value.get("buildingStoryNames", Json::arrayValue);
    Json::ArrayIndex n = buildingStoryNames.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_buildingStoryNames.push_back(buildingStoryNames[i].asString());
    }

    // DLM: done in initializer
    //boundingBox = ThreeBoundingBox(value.get("boundinmgBox", Json::objectValue));

    Json::Value modelObjectMetadata = value.get("modelObjectMetadata", Json::arrayValue);
    n = modelObjectMetadata.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      m_modelObjectMetadata.push_back(ThreeModelObjectMetadata(modelObjectMetadata[i]));
    }
  }

  Json::Value ThreeSceneMetadata::toJsonValue() const
  {
    Json::Value result;

    Json::Value buildingStoryNames(Json::arrayValue);
    for (const auto& buildingStoryName : m_buildingStoryNames){
      buildingStoryNames.append(buildingStoryName);
    }

    Json::Value modelObjectMetadata(Json::arrayValue);
    for (const auto& m : m_modelObjectMetadata){
      modelObjectMetadata.append(m.toJsonValue());
    }

    result["version"] = m_version;
    result["type"] = m_type;
    result["generator"] = m_generator;
    result["buildingStoryNames"] = buildingStoryNames;
    result["boundingBox"] = m_boundingBox.toJsonValue();
    result["modelObjectMetadata"] = modelObjectMetadata;

    return result;
  }

  std::string ThreeSceneMetadata::version() const
  {
    return m_version;
  }

  std::string ThreeSceneMetadata::type() const
  {
    return m_type;
  }

  std::string ThreeSceneMetadata::generator() const
  {
    return m_generator;
  }

  std::vector<std::string> ThreeSceneMetadata::buildingStoryNames() const
  {
    return m_buildingStoryNames;
  }
  
  ThreeBoundingBox ThreeSceneMetadata::boundingBox() const
  {
    return m_boundingBox;
  }

  std::vector<ThreeModelObjectMetadata> ThreeSceneMetadata::modelObjectMetadata() const
  {
    return m_modelObjectMetadata;
  }

} // openstudio
