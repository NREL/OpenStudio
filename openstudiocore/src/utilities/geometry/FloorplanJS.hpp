/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_GEOMETRY_FLOORPLANJS_HPP
#define UTILITIES_GEOMETRY_FLOORPLANJS_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"
#include "Transformation.hpp"
#include "BoundingBox.hpp"

#include "../core/Logger.hpp"
#include "../core/UUID.hpp"

#include <jsoncpp/json.h>

#include <vector>
#include <set>
#include <boost/optional.hpp>

namespace openstudio{

  class ThreeScene;
  class ThreeSceneChild;
  class ThreeGeometry;
  class ThreeModelObjectMetadata;
  class FloorplanObjectId;
  class Point3d;
  class FloorplanJS;

  /** FloorplanObject is used to update data in a FloorplanJS object
  *
  *  The class is not impl-ized in hopes that it can be ported to JavaScript via emscripten
  */
  class UTILITIES_API FloorplanObject{
  public:

    /// pass empty string for any null values
    FloorplanObject(const std::string& id, const std::string& name, const std::string& handleString);
    FloorplanObject(const std::string& id, const std::string& name, const UUID& handle);

    /// id in FloorplanJS
    std::string id() const;

    /// object name
    std::string name() const;

    /// handle of corresponding OpenStudio Model object, will be null if none
    UUID handle() const;

    /// handle of corresponding OpenStudio Model object, will be empty string if none
    std::string handleString() const;

    /// handle of corresponding OpenStudio Model parent object, will be null if none
    boost::optional<std::string> parentHandleString() const;
    void setParentHandleString(const std::string& parentHandleString);
    void resetParentHandleString();

    boost::optional<double> getDataDouble(const std::string& key) const;
    boost::optional<int> getDataInt(const std::string& key) const;
    boost::optional<bool> getDataBool(const std::string& key) const; // DLM: warning, optional bool!
    boost::optional<std::string> getDataString(const std::string& key) const;
    boost::optional<FloorplanObject> getDataReference(const std::string& key) const;

    void setDataDouble(const std::string& key, double value);
    void setDataInt(const std::string& key, int value);
    void setDataBool(const std::string& key, bool value);
    void setDataString(const std::string& key, const std::string& value);
    void setDataReference(const std::string& key, const FloorplanObject& value);

  private:
    friend class FloorplanJS;

    FloorplanObject(const Json::Value& value);

    Json::Value data() const;

    std::map<std::string, FloorplanObject> objectReferenceMap() const;

    std::string m_id;
    std::string m_name;
    UUID m_handle;
    std::string m_handleString;
    boost::optional<std::string> m_parentHandleString;

    Json::Value m_data;

    std::map<std::string, FloorplanObject> m_objectReferenceMap;
  };

  /** FloorplanJS is an adapter for the FloorspaceJS JSON format.  This class includes code which transforms a FloorspaceJS JSON into a 3D model in ThreeJS format.
  *   There are two variations of the ThreeJS format, one which is suitable for rendering with ThreeJS and one that preserves all vertices in a surface for
  *   conversion to OpenStudio Model format.  Converting from FloorspaceJS to ThreeJS to OpenStudio ensures that the ThreeJS preview of a FloorspaceJS model is as
  *   accurate as possible.  These conversions can also be used to convert a FloorspaceJS file to a 3D model for translation to other 3D formats.
  *
  *  The class is not impl-ized in hopes that it can be ported to JavaScript via emscripten
  */
  class UTILITIES_API FloorplanJS{
  public:

    /// default constructor
    FloorplanJS();

    /// constructor from JSON formatted string, will throw if error
    FloorplanJS(const std::string& json);

    /// load from string
    static boost::optional<FloorplanJS> load(const std::string& json);

    /// print to JSON
    std::string toJSON(bool prettyPrint = false) const;

    /// convert to ThreeJS, will throw if error
    /// if openstudioFormat is true, surfaces will be created with arbitrary number of vertices, not suitable for rendering using ThreeJS
    /// if openstudioFormat is false, surfaces will be created with with either 3 or 4 vertices, suitable for rendering using ThreeJS
    /// ThreeJS file produced will always be in metric units, NorthAxis will not be applied during this conversion
    ThreeScene toThreeScene(bool openstudioFormat) const;

    /// degrees from North measured clockwise
    double northAxis() const;

    /// update object names in Floorplan with external data
    /// if object with same handle exists, name will be updated
    /// else if object with same name exists, handle will be assigned
    /// else new object will be added
    /// if removeMissingObjects is true, then existing objects not in input will be deleted
    void updateStories(const std::vector<FloorplanObject>& objects, bool removeMissingObjects);
    void updateSpaces(const std::vector<FloorplanObject>& objects, bool removeMissingObjects);
    void updateBuildingUnits(const std::vector<FloorplanObject>& objects, bool removeMissingObjects);
    void updateThermalZones(const std::vector<FloorplanObject>& objects, bool removeMissingObjects);
    void updateSpaceTypes(const std::vector<FloorplanObject>& objects, bool removeMissingObjects);
    void updateConstructionSets(const std::vector<FloorplanObject>& objects, bool removeMissingObjects);

  private:
    REGISTER_LOGGER("FloorplanJS");

    FloorplanJS(const Json::Value& value);

    ThreeModelObjectMetadata makeModelObjectMetadata(const std::string& iddObjectType, const Json::Value& object) const;

    void makeGeometries(const Json::Value& story, const Json::Value& spaceOrShading, bool belowFloorPlenum, bool aboveCeilingPlenum,
      double lengthToMeters, double minZ, double maxZ, const Json::Value& vertices, const Json::Value& edges, const Json::Value& faces, const std::string& faceId,
      bool openstudioFormat, std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren, bool openToBelow) const;

    std::string makeSurface(const Json::Value& story, const Json::Value& spaceOrShading, const std::string& parentSurfaceName, const std::string& parentSubSurfaceName,
      bool belowFloorPlenum, bool aboveCeilingPlenum, const std::string& surfaceType, const Point3dVector& vertices, size_t faceFormat,
      std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren, double illuminanceSetpoint, bool airWall) const;

    std::string getHandleString(const Json::Value& value) const;
    std::string getName(const Json::Value& value) const;
    std::string getId(const Json::Value& value) const;
    std::string getFaceId(const Json::Value& value) const;

    std::string getNextId();
    void setLastId(const Json::Value& value);

    Json::Value* findByHandleString(Json::Value& value, const std::string& key, const std::string& handleString);
    Json::Value* findByName(Json::Value& value, const std::string& key, const std::string& name, bool requireEmptyHandle);
    Json::Value* findById(Json::Value& value, const std::string& key, const std::string& id);

    const Json::Value* findById(const Json::Value& values, const std::string& id) const;

    void updateObjects(Json::Value& value, const std::string& key, const std::vector<FloorplanObject>& objects, bool removeMissingObjects);
    void updateObjectReference(Json::Value& value, const std::string& key, const FloorplanObject& objectReference, bool removeMissingObjects);

    void removeFaces(Json::Value& value, const std::set<std::string>& faceIdsToRemove);
    void removeEdges(Json::Value& value, const std::set<std::string>& edgeIdsToRemove);
    void removeVertices(Json::Value& value, const std::set<std::string>& vertexIdsToRemove);

    Json::Value m_value;

    unsigned m_lastId;
    mutable std::set<std::string> m_plenumThermalZoneNames;
    mutable BoundingBox m_boundingBox;
  };

  /// convienence method, converts a FloorplanJS JSON string to a ThreeJS JSON string
  UTILITIES_API std::string floorplanToThreeJS(const std::string& json, bool openstudioFormat);

} // openstudio

#endif //UTILITIES_GEOMETRY_FLOORPLANJS_HPP
