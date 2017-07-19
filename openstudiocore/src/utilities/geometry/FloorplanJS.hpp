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

#ifndef UTILITIES_GEOMETRY_FLOORPLANJS_HPP
#define UTILITIES_GEOMETRY_FLOORPLANJS_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"
#include "Transformation.hpp"

#include "../core/Logger.hpp"
#include "../core/UUID.hpp"

#include <jsoncpp/json.h>

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  class ThreeScene;

  class UTILITIES_API FloorplanObjectId{
  public:

    // pass empty string for any null values
    FloorplanObjectId(const std::string& id, const std::string& name, const UUID& handle);

    std::string id() const;
    std::string name() const;
    UUID handle() const;
    std::string handleString() const;

  private:
    std::string m_id;
    std::string m_name;
    UUID m_handle;
    std::string m_handleString;
  };

  /** FloorplanJS is an adapter for the Geometry Editor JSON format
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
    ThreeScene toThreeScene(bool breakSurfaces) const;

    /// update object names in Floorplan with external data
    /// if object with same handle exists, name will be updated
    /// else if object with same name exists, handle will be assigned
    /// else new object will be added
    void updateStories(const std::vector<FloorplanObjectId>& objects);
    void updateBuildingUnits(const std::vector<FloorplanObjectId>& objects);
    void updateThermalZones(const std::vector<FloorplanObjectId>& objects);
    void updateSpaceTypes(const std::vector<FloorplanObjectId>& objects);
    void updateConstructionSets(const std::vector<FloorplanObjectId>& objects);

  private:
    REGISTER_LOGGER("FloorplanJS");

    FloorplanJS(const Json::Value& value);

    std::string getHandleString(const Json::Value& value);
    std::string getName(const Json::Value& value);

    Json::Value* findByHandleString(const Json::Value& value, const std::string& key, const std::string& handleString);
    Json::Value* findByNameOnly(const Json::Value& value, const std::string& key, const std::string& name);

    Json::Value* removeMisingHandles(const Json::Value& value, const std::string& key, const std::vector<UUID>& handle);
    
    void updateObjects(Json::Value& value, const std::string& key, const std::vector<FloorplanObjectId>& objects);

    Json::Value m_value;
  };

} // openstudio

#endif //UTILITIES_GEOMETRY_FLOORPLANJS_HPP
