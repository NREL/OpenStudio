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

#include "FloorplanJS.hpp"
#include "ThreeJS.hpp"

#include "../core/Assert.hpp"
#include "../core/Path.hpp"
#include "../core/Json.hpp"

#include <jsoncpp/json.h>

#include <iostream>
#include <string>

namespace openstudio{
    
  FloorplanJS::FloorplanJS(const std::string& s)
  {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s, m_value);

    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();

      // see if this is a path
      openstudio::path p = toPath(s);
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)){
        // open file
        std::ifstream ifs(openstudio::toString(p));
        m_value.clear();
        parsingSuccessful = reader.parse(ifs, m_value);
      }

      if (!parsingSuccessful){
        LOG_AND_THROW("ThreeJS JSON cannot be processed, " << errors);
      }
    }
  }
    
  boost::optional<FloorplanJS> FloorplanJS::load(const std::string& json)
  {  
    try {
      FloorplanJS result(json);
      return result;
    } catch (...) {
      LOG(Error, "Could not parse JSON input");
    }
    return boost::none;
  }

  std::string FloorplanJS::toJSON(bool prettyPrint) const
  {
    // write to string
    std::string result;
    if (prettyPrint){
      Json::StyledWriter writer;
      result = writer.write(m_value);
    } else{
      Json::FastWriter writer;
      result = writer.write(m_value);
    }

    return result;
  }

  std::vector<ThreeGeometry> getGeometries(const Json::Value& story, const Json::Value& space, const std::string& faceId)
  {
    const std::string uuid;
    const std::string type;
    std::vector<double> vertices;
    const std::vector<size_t> faces;
    ThreeGeometryData data(vertices, faces);
    ThreeGeometry geometry(uuid, type, data);

    std::vector<ThreeGeometry> result;
    return result;
  }

  std::vector<ThreeSceneChild> getSceneChildren(const Json::Value& story, const Json::Value& space, const std::string& faceId)
  {
    const std::string uuid;
    const std::string name; 
    const std::string type;
    const std::string geometryId;
    const std::string materialId;
    const ThreeUserData userData;

    ThreeSceneChild sceneChild(uuid, name, type, geometryId, materialId, userData);

    std::vector<ThreeSceneChild> result;
    return result;
  }

  ThreeScene FloorplanJS::toThreeScene(bool triangulateSurfaces) const
  {
    assertKeyAndType(m_value, "stories", Json::arrayValue);

    std::vector<ThreeGeometry> geometries;
    std::vector<ThreeMaterial> materials;
    std::vector<ThreeSceneChild> children;
    std::vector<std::string> buildingStoryNames;

    double zHeight = 0;

    Json::Value stories = m_value.get("stories", Json::arrayValue);
    Json::ArrayIndex storyN = stories.size();
    for (Json::ArrayIndex storyIdx = 0; storyIdx < storyN; ++storyIdx){
      assertKeyAndType(stories[storyIdx], "spaces", Json::stringValue);

      std::string storyName = stories[storyIdx].get("name", "").asString();

      Json::Value spaces = stories[storyIdx].get("spaces", Json::arrayValue);
      Json::ArrayIndex spaceN = spaces.size();
      for (Json::ArrayIndex spaceIdx = 0; spaceIdx < spaceN; ++spaceIdx){

        if (checkKeyAndType(spaces[spaceIdx], "face_id", Json::stringValue)){
          std::string faceId = spaces[spaceIdx].get("face_id", "").asString(); 

          std::vector<ThreeGeometry> g = getGeometries(stories[storyIdx], spaces[spaceIdx], faceId);

          std::vector<ThreeSceneChild> c = getSceneChildren(stories[storyIdx], spaces[spaceIdx], faceId);

          geometries.insert(g.begin(), g.end(), geometries.end());
          children.insert(c.begin(), c.end(), children.end());
        }

      }

    }

    
    ThreeBoundingBox boundingBox(0,0,0,0,0,0,0,0,0,0);
    ThreeSceneMetadata metadata(buildingStoryNames, boundingBox);


    ThreeSceneObject sceneObject("", children);

    ThreeScene result(metadata, geometries, materials, sceneObject);

    return result;
  }


} // openstudio
