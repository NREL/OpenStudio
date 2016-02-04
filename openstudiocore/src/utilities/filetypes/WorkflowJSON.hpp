/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWJSON_HPP
#define UTILITIES_FILETYPES_WORKFLOWJSON_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"

#include <jsoncpp/json.h>

namespace openstudio{

/** Class for accessing the OpenStudio Workflow (OSW) JSON format. */
class UTILITIES_API WorkflowJSON
{
public:

  /** Create a new, empty workflow. */
  WorkflowJSON();
  
  /** Constructor with path, will throw if path does not exist or file is incorrect. */
  WorkflowJSON(const openstudio::path& p);
  
  /** Attempt to load a WorkflowJSON from path */
  static boost::optional<WorkflowJSON> load(const openstudio::path& p);

  /** Saves this file to the current location. */
  bool save() const;

  /** Saves this file to a new location. */
  bool saveAs(const openstudio::path& p) const;

  /** Returns the original path this workflow was loaded from, can be empty. */
  openstudio::path path() const;

  /** Returns the absolute path to the root directory, can be empty. */
  openstudio::path rootPath() const;

  /** Returns the absolute path to the seed file, can be empty. */
  openstudio::path seedPath() const;

  /** Returns the absolute path to the weather file, can be empty. */
  openstudio::path weatherPath() const;

  /** Returns the absolute path to the measures directory, can be empty. */
  openstudio::path measuresDir() const;

private:

  // configure logging
  REGISTER_LOGGER("openstudio.WorkflowJSON");
  
  openstudio::path m_path;
  Json::Value m_json;
};

} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWJSON_HPP
