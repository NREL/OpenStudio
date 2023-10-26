/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef EPJSON_TRANSLATOR_HPP
#define EPJSON_TRANSLATOR_HPP

#include <string>
#include "epJSONAPI.hpp"

#include "../utilities/core/Filesystem.hpp"

namespace Json {
class Value;
}

namespace openstudio {
class IdfFile;
class Workspace;
struct IddFileType;
}  // namespace openstudio

namespace openstudio::epJSON {

EPJSON_API openstudio::path defaultSchemaPath(openstudio::IddFileType filetype);

EPJSON_API Json::Value loadJSON(const openstudio::path& path);

EPJSON_API Json::Value toJSON(const openstudio::IdfFile& inputFile, const openstudio::path& schemaPath = openstudio::path());
EPJSON_API std::string toJSONString(const openstudio::IdfFile& inputFile, const openstudio::path& schemaPath = openstudio::path());

EPJSON_API Json::Value toJSON(const openstudio::Workspace& workspace, const openstudio::path& schemaPath = openstudio::path());
EPJSON_API std::string toJSONString(const openstudio::Workspace& workspace, const openstudio::path& schemaPath = openstudio::path());

}  // namespace openstudio::epJSON

#endif
