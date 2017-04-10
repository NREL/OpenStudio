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

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  enum ThreeSide{FrontSide = 0, BackSide = 1, DoubleSide = 2};

  UTILITIES_API unsigned threeColor(unsigned r, unsigned g, unsigned b);

  UTILITIES_API std::string threeUUID(const std::string& uuid);

  class UTILITIES_API ThreeGeometry{
  public: 
    ThreeGeometry(const std::string& uuid, const::std::string& type);
    std::string uuid() const;
    std::string type() const;

  private:
    std::string m_uuid;
    std::string m_type;
  };
  //Geometry = Struct.new(:uuid, :type, :data)
  //GeometryData = Struct.new(:vertices, :normals, :uvs, :faces, :scale, :visible, :castShadow, :receiveShadow, :doubleSided)

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

  class UTILITIES_API ThreeSceneObject{
  public: 
    ThreeSceneObject(const std::string& uuid);
    std::string uuid() const;
    std::string type() const;

  private:
    std::string m_uuid;
    std::string m_type;
  };
  //SceneObject = Struct.new(:uuid, :type, :matrix, :children)
  //SceneChild = Struct.new(:uuid, :name, :type, :geometry, :material, :matrix, :userData)
  //UserData = Struct.new(:handle, :name, :surfaceType, :constructionName, :spaceName, :thermalZoneName, :spaceTypeName, :buildingStoryName, 
     //                   :outsideBoundaryCondition, :outsideBoundaryConditionObjectName, 
     //                   :outsideBoundaryConditionObjectHandle, :coincidentWithOutsideObject,
     //                   :sunExposure, :windExposure, #:vertices, 
     //                   :surfaceTypeMaterialName, :boundaryMaterialName, :constructionMaterialName,  :thermalZoneMaterialName, 
     //                   :spaceTypeMaterialName, :buildingStoryMaterialName) 

  class UTILITIES_API ThreeAmbientLight{
  public: 
    ThreeAmbientLight(const std::string& uuid, const::std::string& type, unsigned color);
    std::string uuid() const;
    std::string type() const;
    unsigned color() const;

  private:
    std::string m_uuid;
    std::string m_type;
    unsigned m_color;
  };

  /** ThreeJs class is an adapter for the three.js geometry format, defined at:
  *   https://github.com/mrdoob/three.js/wiki/JSON-Object-Scene-format-4
  *
  *  The class is not impl-ized in hopes that it can be ported to JavaScript via emscripten
  */
  class UTILITIES_API ThreeScene{
  public:

    /// constructor 
    ThreeScene(const std::vector<ThreeGeometry>& geometries, const std::vector<ThreeMaterial>& materials, const ThreeSceneObject& sceneObject);

    /// constructor from JSON formatted string, will throw if error
    ThreeScene(const std::string& json);

    /// load from string
    static boost::optional<ThreeScene> load(const std::string& json);

    /// print to JSON
    std::string toJSON(bool prettyPrint = false) const;
  
  private:

    std::vector<ThreeGeometry> m_geometries;
    std::vector<ThreeMaterial> m_materials;
    ThreeSceneObject m_sceneObject;
  };

} // openstudio

#endif //UTILITIES_GEOMETRY_THREEJS_HPP
