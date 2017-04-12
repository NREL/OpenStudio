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

#include <jsoncpp/json.h>

namespace openstudio{

  
  unsigned toThreeColor(unsigned r, unsigned g, unsigned b)
  {
    //return "0x#{r.to_s(16).rjust(2,'0')}#{g.to_s(16).rjust(2,'0')}#{b.to_s(16).rjust(2,'0')}"
    return 0;
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
        //result = []
    //vertices.each do |vertex|
     // #result << vertex.x
     // #result << vertex.y
     // #result << vertex.z
     // 
     // result << vertex.x.round(3)
     // result << vertex.z.round(3)
     // result << -vertex.y.round(3)
   // end
    return std::vector<double>();
  }

  std::vector<double> toThreeMatrix(const Transformation& matrix)
  {
    return std::vector<double>();
  }

  ThreeScene::ThreeScene(const ThreeSceneMetadata& metadata, const std::vector<ThreeGeometry>& geometries, const std::vector<ThreeMaterial>& materials, const ThreeSceneObject& sceneObject)
    : m_metadata(metadata), m_geometries(geometries), m_materials(materials), m_sceneObject(sceneObject)
  {
  }
    
  ThreeScene::ThreeScene(const std::string& json)
    : m_metadata(std::vector<std::string>(), ThreeBoundingBox(0,0,0,0,0,0,0,0,0,0)), m_sceneObject(ThreeSceneObject("", std::vector<ThreeSceneChild>()))
  {
    throw std::runtime_error("No implemented");
  }
    
  boost::optional<ThreeScene> ThreeScene::load(const std::string& json)
  {
    return boost::none;
  }

  std::string ThreeScene::toJSON(bool prettyPrint) const
  {
    Json::Value scene(Json::objectValue);

    // metadata
    Json::Value metadata(Json::objectValue);
    scene["metadata"] = metadata;

    Json::Value boundingBox(Json::objectValue);
    boundingBox["minX"] = m_metadata.boundingBox().minX();
    boundingBox["minY"] = m_metadata.boundingBox().minY();
    boundingBox["minZ"] = m_metadata.boundingBox().minZ();
    boundingBox["maxX"] = m_metadata.boundingBox().maxX();
    boundingBox["maxY"] = m_metadata.boundingBox().maxY();
    boundingBox["maxZ"] = m_metadata.boundingBox().maxZ();
    boundingBox["lookAtX"] = m_metadata.boundingBox().lookAtX();
    boundingBox["lookAtY"] = m_metadata.boundingBox().lookAtY();
    boundingBox["lookAtZ"] = m_metadata.boundingBox().lookAtZ();
    boundingBox["lookAtR"] = m_metadata.boundingBox().lookAtR();

    Json::Value buildingStoryNames(Json::arrayValue);
    for (const auto& buildingStoryName : m_metadata.buildingStoryNames()){
      buildingStoryNames.append(buildingStoryName);
    }

    metadata["version"] = m_metadata.version();
    metadata["type"] = m_metadata.type();
    metadata["generator"] = m_metadata.generator();
    metadata["buildingStoryNames"] = buildingStoryNames;
    metadata["boundingBox"] = boundingBox;

    // geometries
    Json::Value geometries(Json::arrayValue);
    for (const auto& g : m_geometries)
    {
      ThreeGeometryData d = g.data();

      Json::Value geometry(Json::objectValue);
      
      Json::Value vertices(Json::arrayValue);
      for (const auto& v : d.vertices()){
        vertices.append(v);
      }

      Json::Value normals(Json::arrayValue);

      Json::Value uvs(Json::arrayValue);

      Json::Value faces(Json::arrayValue);
      for (const auto& f : d.faces()){
        faces.append(f);
      }

      Json::Value data(Json::objectValue);
      data["vertices"] = vertices;
      data["vertices"] = normals;
      data["vertices"] = uvs;
      data["faces"] = faces;
      data["scale"] = d.scale();
      data["visible"] = d.visible();
      data["castShadow"] = d.castShadow();
      data["receiveShadow"] = d.receiveShadow();
      data["doubleSided"] = d.doubleSided();

      geometry["uuid"] = g.uuid();
      geometry["type"] = g.type();
      geometry["data"] = data;
      geometries.append(geometry);
    }
    scene["geometries"] = geometries;

    // materials
    Json::Value materials(Json::arrayValue);
    for (const auto& m : m_materials)
    {
      Json::Value material(Json::objectValue);
      material["uuid"] = m.uuid();
      material["name"] = m.name();
      material["type"] = m.type();
      material["color"] = m.color();
      material["ambient"] = m.ambient();
      material["emissive"] = m.emissive();
      material["specular"] = m.specular();
      material["shininess"] = m.shininess();
      material["opacity"] = m.opacity();
      material["transparent"] = m.transparent();
      material["wireframe"] = m.wireframe();
      material["side"] = m.side();

      materials.append(material);
    }
    scene["materials"] = materials;

    // object
    Json::Value object(Json::objectValue);
    scene["object"] = object;


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

  std::vector<ThreeMaterial> ThreeScene::materials() const
  {
    return m_materials;
  }

  ThreeSceneObject ThreeScene::object() const
  {
    return m_sceneObject;
  }

  ThreeGeometryData::ThreeGeometryData(const std::vector<double>& vertices, const std::vector<size_t>& faces)
    : m_vertices(vertices), m_normals(), m_uvs(), m_faces(faces), m_scale(1.0), m_visible(true), m_castShadow(true), m_receiveShadow(true), m_doubleSided(true)
  {}

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
  {}

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

  std::string ThreeUserData::constructionMaterialName() const
  {
    return m_constructionMaterialName;
  }

  std::string ThreeUserData::spaceName() const
  {
    return m_spaceName;
  }

  std::string ThreeUserData::thermalZoneName() const
  {
    return m_thermalZoneName;
  }

  std::string ThreeUserData::thermalZoneMaterialName() const
  {
    return m_thermalZoneMaterialName;
  }

  std::string ThreeUserData::spaceTypeName() const
  {
    return m_spaceTypeName;
  }

  std::string ThreeUserData::spaceTypeMaterialName() const
  {
    return m_spaceTypeMaterialName;
  }

  std::string ThreeUserData::buildingStoryName() const
  {
    return m_buildingStoryName;
  }

  std::string ThreeUserData::buildingStoryMaterialName() const
  {
    return m_buildingStoryMaterialName;
  }

  std::string ThreeUserData::buildingUnitName() const
  {
    return m_buildingUnitName;
  }

  std::string ThreeUserData::buildingUnitMaterialName() const
  {
    return m_buildingUnitMaterialName;
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

  void ThreeUserData::setConstructionMaterialName(const std::string& s)
  {
    m_constructionMaterialName = s;
  }

  void ThreeUserData::setSpaceName(const std::string& s)
  {
    m_spaceName = s;
  }

  void ThreeUserData::setThermalZoneName(const std::string& s)
  {
    m_thermalZoneName = s;
  }

  void ThreeUserData::setThermalZoneMaterialName(const std::string& s)
  {
    m_thermalZoneMaterialName = s;
  }

  void ThreeUserData::setSpaceTypeName(const std::string& s)
  {
    m_spaceTypeName = s;
  }

  void ThreeUserData::setSpaceTypeMaterialName(const std::string& s)
  {
    m_spaceTypeMaterialName = s;
  }

  void ThreeUserData::setBuildingStoryName(const std::string& s)
  {
    m_buildingStoryName = s;
  }

  void ThreeUserData::setBuildingStoryMaterialName(const std::string& s)
  {
    m_buildingStoryMaterialName = s;
  }

  void ThreeUserData::setBuildingUnitName(const std::string& s)
  {
    m_buildingUnitName = s;
  }

  void ThreeUserData::setBuildingUnitMaterialName(const std::string& s)
  {
    m_buildingUnitMaterialName = s;
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

  ThreeSceneChild::ThreeSceneChild(const std::string& uuid, const std::string& name, const std::string& type,
                    const std::string& geometryId, const std::string& materialId, const ThreeUserData& userData)
    : m_uuid(uuid), m_name(name), m_type(type), m_geometryId(geometryId), m_materialId(materialId), m_matrix(), m_userData(userData)
  {}

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

  Transformation ThreeSceneChild::matrix() const
  {
    return m_matrix;
  }

  ThreeUserData ThreeSceneChild::userData() const
  {
    return m_userData;
  }

  ThreeSceneObject::ThreeSceneObject(const std::string& uuid, const std::vector<ThreeSceneChild>& children)
    : m_uuid(uuid), m_type("Scene"), m_matrix(), m_children(children)
  {}

  std::string ThreeSceneObject::uuid() const
  {
    return m_uuid;
  }
    
  std::string ThreeSceneObject::type() const
  {
    return m_type;
  }

  Transformation ThreeSceneObject::matrix() const
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

  ThreeSceneMetadata::ThreeSceneMetadata(const std::vector<std::string>& buildingStoryNames, const ThreeBoundingBox& boundingBox)
    : m_version("4.3"), m_type("Object"), m_generator("OpenStudio"), m_buildingStoryNames(buildingStoryNames), m_boundingBox(boundingBox)
  {}

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

} // openstudio
