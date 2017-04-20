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

  boost::optional<Json::Value> getById(const Json::Value& objects, const std::string& id)
  {
    boost::optional<Json::Value> result;
    Json::ArrayIndex n = objects.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (checkKeyAndType(objects[i], "id", Json::stringValue)){
        if (id == objects[i].get("id", "").asString()){
          return objects[i];
        }
      }
    }
    return boost::none;
  }

  void makeGeometries(const Json::Value& vertices, const Json::Value& edges, const Json::Value& faces, const std::string& faceId,
                     std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren)
  {
    std::vector<Point3d> floorVertices;
    std::vector<size_t> floorFaces;

    // openstudio format
    floorFaces.push_back(1024u);

    boost::optional<Json::Value> face = getById(faces, faceId);
    if (face){
      Json::Value edgeIds = face->get("edge_ids", Json::arrayValue);
      Json::Value edgeOrders = face->get("edge_order", Json::arrayValue);
      Json::ArrayIndex edgeN = edgeIds.size();
      OS_ASSERT(edgeN == edgeOrders.size());

      for (Json::ArrayIndex edgeIdx = 0; edgeIdx < edgeN; ++edgeIdx){
        std::string edgeId = edgeIds[edgeIdx].asString();
        unsigned edgeOrder = edgeOrders[edgeIdx].asUInt();

        boost::optional<Json::Value> edge = getById(edges, edgeId);
        if (edge){
          Json::Value vertexIds = edge->get("vertex_ids", Json::arrayValue);
          OS_ASSERT(2u == vertexIds.size());

          boost::optional<Json::Value> vertex1 = getById(vertices, vertexIds[0].asString());
          boost::optional<Json::Value> vertex2 = getById(vertices, vertexIds[1].asString());
          OS_ASSERT(vertex1);
          OS_ASSERT(vertex2);

          if (edgeOrder == 0){
            boost::optional<Json::Value> vertex3 = vertex1;
            vertex1 = vertex2;
            vertex2 = vertex3;
          }

          assertKeyAndType(*vertex1, "x", Json::realValue);
          assertKeyAndType(*vertex1, "y", Json::realValue);
          floorVertices.push_back(Point3d(vertex1->get("x", 0.0).asDouble(), vertex1->get("y", 0.0).asDouble(), 0.0));
          floorFaces.push_back(edgeIdx);
        }
      }
    }

    {
      std::string uuid = faceId;
      std::string type = "Mesh";
      ThreeGeometryData data(toThreeVector(floorVertices), floorFaces);
      ThreeGeometry geometry(uuid, type, data);
      geometries.push_back(geometry);
    }
    {
      std::string uuid = "1";
      std::string name = "Unknown";
      std::string type = "Mesh";
      std::string geometryId = faceId;
      std::string materialId;
      const ThreeUserData userData;

      ThreeSceneChild sceneChild(uuid, name, type, geometryId, materialId, userData);
      sceneChildren.push_back(sceneChild);
    }
  }

  ThreeScene FloorplanJS::toThreeScene(bool breakSurfaces) const
  {
    std::vector<ThreeGeometry> geometries;
    std::vector<ThreeMaterial> materials;
    std::vector<ThreeSceneChild> children;
    std::vector<std::string> buildingStoryNames;

    double zHeight = 0;

    Json::Value stories = m_value.get("stories", Json::arrayValue);
    Json::ArrayIndex storyN = stories.size();
    for (Json::ArrayIndex storyIdx = 0; storyIdx < storyN; ++storyIdx){

      assertKeyAndType(stories[storyIdx], "name", Json::stringValue);
      std::string storyName = stories[storyIdx].get("name", "").asString();

      assertKeyAndType(stories[storyIdx], "geometry", Json::objectValue);
      Json::Value geometry = stories[storyIdx].get("geometry", Json::arrayValue);
      Json::Value vertices = geometry.get("vertices", Json::arrayValue);
      Json::Value edges = geometry.get("edges", Json::arrayValue);
      Json::Value faces = geometry.get("faces", Json::arrayValue);

      Json::Value spaces = stories[storyIdx].get("spaces", Json::arrayValue);
      Json::ArrayIndex spaceN = spaces.size();
      for (Json::ArrayIndex spaceIdx = 0; spaceIdx < spaceN; ++spaceIdx){

        if (checkKeyAndType(spaces[spaceIdx], "face_id", Json::stringValue)){
          std::string faceId = spaces[spaceIdx].get("face_id", "").asString(); 

          std::vector<ThreeGeometry> g;
          std::vector<ThreeSceneChild> c;
          makeGeometries(vertices, edges, faces, faceId, g, c);

          geometries.insert(geometries.end(), g.begin(), g.end());
          children.insert(children.end(), c.begin(), c.end());
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
