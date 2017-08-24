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

#ifndef UTILITIES_GEOMETRY_THREEJS_HPP
#define UTILITIES_GEOMETRY_THREEJS_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"
#include "Transformation.hpp"

#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace Json{
  class Value;
}

namespace openstudio{

  class ThreeScene;

  /// enum for materials
  enum ThreeSide{FrontSide = 0, BackSide = 1, DoubleSide = 2};

  /// convert RGB to unsigned
  UTILITIES_API unsigned toThreeColor(unsigned r, unsigned g, unsigned b);

  /// format a UUID, to limit dependencies UUIDs must be generated outside of this code
  UTILITIES_API std::string toThreeUUID(const std::string& uuid);

  UTILITIES_API std::string fromThreeUUID(const std::string& uuid);

  /// format a list of vertices
  UTILITIES_API std::vector<double> toThreeVector(const Point3dVector& vertices);

  UTILITIES_API Point3dVector fromThreeVector(const std::vector<double>& vertices);

  /// format a Transformation matrix
  UTILITIES_API std::vector<double> toThreeMatrix(const Transformation& matrix);

  UTILITIES_API Transformation toThreeMatrix(const std::vector<double>& matrix);

  /// ThreeGeometryData holds the geometry data for an object
  class UTILITIES_API ThreeGeometryData{
  public: 
    ThreeGeometryData(const std::vector<double>& vertices, const std::vector<size_t>& faces);
    std::vector<double> vertices() const;
    std::vector<size_t> normals() const;
    std::vector<size_t> uvs() const;
    std::vector<size_t> faces() const;
    double scale() const;
    bool visible() const;
    bool castShadow() const;
    bool receiveShadow() const;
    bool doubleSided() const;

  private:
    friend class ThreeGeometry;
    ThreeGeometryData(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::vector<double> m_vertices;
    std::vector<size_t> m_normals;
    std::vector<size_t> m_uvs;
    std::vector<size_t> m_faces;
    double m_scale;
    bool m_visible;
    bool m_castShadow;
    bool m_receiveShadow;
    bool m_doubleSided;
  };

  /// ThreeGeometry holds the geometry for an object
  class UTILITIES_API ThreeGeometry{
  public: 
    ThreeGeometry(const std::string& uuid, const::std::string& type, const ThreeGeometryData& data);
    std::string uuid() const;
    std::string type() const;
    ThreeGeometryData data() const;

  private:
    friend class ThreeScene;
    ThreeGeometry(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_uuid;
    std::string m_type;
    ThreeGeometryData m_data;
  };
 
  /// ThreeMaterial defines a rendering material
  class UTILITIES_API ThreeMaterial{
  public:
    ThreeMaterial(const std::string& uuid, const std::string& name, const::std::string& type,
      unsigned color, unsigned ambient, unsigned emissive, unsigned specular, unsigned shininess,
      double opacity, bool transparent, bool wireframe, unsigned side);
    std::string uuid() const;
    std::string name() const;
    std::string type() const;
    unsigned color() const;
    unsigned ambient() const;
    unsigned emissive() const;
    unsigned specular() const;
    unsigned shininess() const;
    double opacity() const;
    bool transparent() const;
    bool wireframe() const;
    unsigned side() const;   

  private:
    friend class ThreeScene;
    ThreeMaterial(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_uuid;
    std::string m_name;
    std::string m_type;
    unsigned m_color;
    unsigned m_ambient;
    unsigned m_emissive;
    unsigned m_specular;
    unsigned m_shininess;
    double m_opacity;
    bool m_transparent;
    bool m_wireframe;
    unsigned m_side; 
  };

  /// ThreeUserData decorates a ThreeSceneChild with additional information
  class UTILITIES_API ThreeUserData{
  public: 
    ThreeUserData();
    std::string handle() const;
    std::string name() const;
    std::string surfaceType() const;
    std::string surfaceTypeMaterialName() const;
    std::string constructionName() const;
    std::string constructionHandle() const;
    std::string constructionMaterialName() const;
    std::string spaceName() const;
    std::string spaceHandle() const;
    std::string thermalZoneName() const;
    std::string thermalZoneHandle() const;
    std::string thermalZoneMaterialName() const;
    std::string spaceTypeName() const;
    std::string spaceTypeHandle() const;
    std::string spaceTypeMaterialName() const;
    std::string buildingStoryName() const;
    std::string buildingStoryHandle() const;
    std::string buildingStoryMaterialName() const;
    std::string buildingUnitName() const;
    std::string buildingUnitHandle() const;
    std::string buildingUnitMaterialName() const;
    std::string constructionSetName() const;
    std::string constructionSetHandle() const;
    std::string constructionSetMaterialName() const;
    std::string outsideBoundaryCondition() const;
    std::string outsideBoundaryConditionObjectName() const;
    std::string outsideBoundaryConditionObjectHandle() const;
    std::string boundaryMaterialName() const;
    bool coincidentWithOutsideObject() const;
    std::string sunExposure() const;
    std::string windExposure() const;
    //bool plenum() const;
    //bool belowFloorPlenum() const;
    //bool aboveCeilingPlenum() const;

    void setHandle(const std::string& s);
    void setName(const std::string& s);
    void setSurfaceType(const std::string& s);
    void setSurfaceTypeMaterialName(const std::string& s);
    void setConstructionName(const std::string& s);
    void setConstructionHandle(const std::string& s);
    void setConstructionMaterialName(const std::string& s);
    void setSpaceName(const std::string& s);
    void setSpaceHandle(const std::string& s);
    void setThermalZoneName(const std::string& s);
    void setThermalZoneHandle(const std::string& s);
    void setThermalZoneMaterialName(const std::string& s);
    void setSpaceTypeName(const std::string& s);
    void setSpaceTypeHandle(const std::string& s);
    void setSpaceTypeMaterialName(const std::string& s);
    void setBuildingStoryName(const std::string& s);
    void setBuildingStoryHandle(const std::string& s);
    void setBuildingStoryMaterialName(const std::string& s);
    void setBuildingUnitName(const std::string& s);
    void setBuildingUnitHandle(const std::string& s);
    void setBuildingUnitMaterialName(const std::string& s);
    void setConstructionSetName(const std::string& s);
    void setConstructionSetHandle(const std::string& s);
    void setConstructionSetMaterialName(const std::string& s);
    void setOutsideBoundaryCondition(const std::string& s);
    void setOutsideBoundaryConditionObjectName(const std::string& s);
    void setOutsideBoundaryConditionObjectHandle(const std::string& s);
    void setBoundaryMaterialName(const std::string& s);
    void setCoincidentWithOutsideObject(bool b);
    void setSunExposure(const std::string& s);
    void setWindExposure(const std::string& s);
    //void setBelowFloorPlenum(bool v);
    //void setAboveCeilingPlenum(bool v);

  private:
    friend class ThreeSceneChild;
    ThreeUserData(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_handle;
    std::string m_name;
    std::string m_surfaceType;
    std::string m_surfaceTypeMaterialName;
    std::string m_constructionName;
    std::string m_constructionHandle;
    std::string m_constructionMaterialName;
    std::string m_spaceName;
    std::string m_spaceHandle;
    std::string m_thermalZoneName;
    std::string m_thermalZoneHandle;
    std::string m_thermalZoneMaterialName;
    std::string m_spaceTypeName;
    std::string m_spaceTypeHandle;
    std::string m_spaceTypeMaterialName;
    std::string m_buildingStoryName;
    std::string m_buildingStoryHandle;
    std::string m_buildingStoryMaterialName;
    std::string m_buildingUnitName;
    std::string m_buildingUnitHandle;
    std::string m_buildingUnitMaterialName;
    std::string m_constructionSetName;
    std::string m_constructionSetHandle;
    std::string m_constructionSetMaterialName;
    std::string m_outsideBoundaryCondition;
    std::string m_outsideBoundaryConditionObjectName;
    std::string m_outsideBoundaryConditionObjectHandle;
    std::string m_boundaryMaterialName;
    bool m_coincidentWithOutsideObject;
    std::string m_sunExposure;
    std::string m_windExposure;
    //bool m_belowFloorPlenum;
    //bool m_aboveCeilingPlenum;
  };

  
  /// ThreeSceneChild is a child object of a ThreeSceneObject
  class UTILITIES_API ThreeSceneChild{
  public: 
    ThreeSceneChild(const std::string& uuid, const std::string& name, const std::string& type,
                    const std::string& geometryId, const std::string& materialId, const ThreeUserData& userData);
    std::string uuid() const;
    std::string name() const;
    std::string type() const;
    std::string geometry() const;
    std::string material() const;
    std::vector<double> matrix() const;
    ThreeUserData userData() const;

  private:
    friend class ThreeSceneObject;
    ThreeSceneChild(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_uuid;
    std::string m_name;
    std::string m_type;
    std::string m_geometryId;
    std::string m_materialId;
    std::vector<double> m_matrix;
    ThreeUserData m_userData;
  };

  /// ThreeSceneObject is the root object in a ThreeScene
  class UTILITIES_API ThreeSceneObject{
  public: 
    ThreeSceneObject(const std::string& uuid, const std::vector<ThreeSceneChild>& children);
    std::string uuid() const;
    std::string type() const;
    std::vector<double> matrix() const;
    std::vector<ThreeSceneChild> children() const;

  private:
    friend class ThreeScene;
    ThreeSceneObject(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_uuid;
    std::string m_type;
    std::vector<double> m_matrix;
    std::vector<ThreeSceneChild> m_children;
  };

  /// ThreeBoundingBox includes information about a bounding box 
  class UTILITIES_API ThreeBoundingBox{
  public: 
    ThreeBoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ, 
                     double lookAtX, double lookAtY, double lookAtZ, double lookAtR);
    double minX() const;
    double minY() const;
    double minZ() const;
    double maxX() const;
    double maxY() const;
    double maxZ() const;
    double lookAtX() const;
    double lookAtY() const;
    double lookAtZ() const;
    double lookAtR() const;

  private:
    friend class ThreeSceneMetadata;
    ThreeBoundingBox(const Json::Value& json);
    Json::Value toJsonValue() const;

    double m_minX;
    double m_minY;
    double m_minZ;
    double m_maxX;
    double m_maxY;
    double m_maxZ;
    double m_lookAtX;
    double m_lookAtY;
    double m_lookAtZ;
    double m_lookAtR;
  };

  /// ThreeModelObjectMetadata includes metadata about an OpenStudio ModelObject
  class UTILITIES_API ThreeModelObjectMetadata{
  public: 

    // DLM: public default ctor seems to be only to make SWIG happy, normal tricks of ignoring vector resize did not work 
    // additionally private default ctor with template<class _Ty> friend class std::allocator; also did not work
    ThreeModelObjectMetadata();

    ThreeModelObjectMetadata(const std::string& iddObjectType, const std::string& handle, const std::string& name);
    std::string iddObjectType() const;
    std::string handle() const;
    std::string name() const;

  private:
    friend class ThreeSceneMetadata;

    ThreeModelObjectMetadata(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_iddObjectType;
    std::string m_handle;
    std::string m_name;
  };

  /// ThreeSceneMetadata includes metadata about an OpenStudio Model Object
  class UTILITIES_API ThreeSceneMetadata{
  public: 
    ThreeSceneMetadata(const std::vector<std::string>& buildingStoryNames, const ThreeBoundingBox& boundingBox, const std::vector<ThreeModelObjectMetadata>& modelObjectMetadata);
    std::string version() const;
    std::string type() const;
    std::string generator() const;
    std::vector<std::string> buildingStoryNames() const;
    ThreeBoundingBox boundingBox() const;
    std::vector<ThreeModelObjectMetadata> modelObjectMetadata() const;

  private:
    friend class ThreeScene;
    ThreeSceneMetadata(const Json::Value& json);
    Json::Value toJsonValue() const;

    std::string m_version;
    std::string m_type;
    std::string m_generator;
    std::vector<std::string> m_buildingStoryNames;
    ThreeBoundingBox m_boundingBox;
    std::vector<ThreeModelObjectMetadata> m_modelObjectMetadata;
  };

  /** ThreeScene is an adapter for a scene in the three.js geometry format, defined at:
  *   https://github.com/mrdoob/three.js/wiki/JSON-Object-Scene-format-4
  *
  *  The class is not impl-ized in hopes that it can be ported to JavaScript via emscripten
  */
  class UTILITIES_API ThreeScene{
  public:

    /// constructor 
    ThreeScene(const ThreeSceneMetadata& metadata, const std::vector<ThreeGeometry>& geometries, const std::vector<ThreeMaterial>& materials, const ThreeSceneObject& sceneObject);

    /// constructor from JSON formatted string, will throw if error
    ThreeScene(const std::string& json);

    /// load from string
    static boost::optional<ThreeScene> load(const std::string& json);

    /// print to JSON
    std::string toJSON(bool prettyPrint = false) const;
  
    ThreeSceneMetadata metadata() const;
    std::vector<ThreeGeometry> geometries() const;
    boost::optional<ThreeGeometry> getGeometry(const std::string& geometryId) const;
    std::vector<ThreeMaterial> materials() const;
    boost::optional<ThreeMaterial> getMaterial(const std::string& materialId) const;
    ThreeSceneObject object() const;

  private:
    REGISTER_LOGGER("ThreeScene");

    ThreeSceneMetadata m_metadata;
    std::vector<ThreeGeometry> m_geometries;
    std::vector<ThreeMaterial> m_materials;
    ThreeSceneObject m_sceneObject;
  };

} // openstudio

#endif //UTILITIES_GEOMETRY_THREEJS_HPP
