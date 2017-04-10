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


namespace openstudio{

  
  unsigned threeColor(unsigned r, unsigned g, unsigned b)
  {
    //return "0x#{r.to_s(16).rjust(2,'0')}#{g.to_s(16).rjust(2,'0')}#{b.to_s(16).rjust(2,'0')}"
    return 0;
  }

  std::string threeUUID(const std::string& uuid)
  {
    // uuid.to_s.gsub('{','').gsub('}','')
    return uuid;
  }

  ThreeScene::ThreeScene(const std::vector<ThreeGeometry>& geometries, const std::vector<ThreeMaterial>& materials, const ThreeSceneObject& sceneObject)
    : m_geometries(geometries), m_materials(materials), m_sceneObject(sceneObject)
  {
  }
    
  ThreeScene::ThreeScene(const std::string& json)
    : m_sceneObject("")
  {
  }
    
  boost::optional<ThreeScene> ThreeScene::load(const std::string& json)
  {
    return boost::none;
  }

  std::string ThreeScene::toJSON(bool prettyPrint) const
  {
    return "";
  }
  
  ThreeGeometry::ThreeGeometry(const std::string& uuid, const::std::string& type)
     : m_uuid(uuid), m_type(type)
   {}

   std::string ThreeGeometry::uuid() const
   {
     return m_uuid;
   }
    
   std::string ThreeGeometry::type() const
   {
     return m_type;
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

  ThreeSceneObject::ThreeSceneObject(const std::string& uuid)
    : m_uuid(uuid), m_type("Scene")
  {}

  std::string ThreeSceneObject::uuid() const
  {
    return m_uuid;
  }
    
  std::string ThreeSceneObject::type() const
  {
    return m_type;
  }

  ThreeAmbientLight::ThreeAmbientLight(const std::string& uuid, const::std::string& type, unsigned color)
    : m_uuid(uuid), m_type(type), m_color(color)
  {}

  std::string ThreeAmbientLight::uuid() const
  {
    return m_uuid;
  }
    
  std::string ThreeAmbientLight::type() const
  {
    return m_type;
  }    

  unsigned ThreeAmbientLight::color() const
  {
    return m_color;
  }

} // openstudio
