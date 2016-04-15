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

#ifndef UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "WorkflowStep.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <jsoncpp/json.h>

namespace openstudio{
namespace detail {

    class UTILITIES_API WorkflowJSON_Impl
    {
    public:

      /** Create a new, empty workflow. */
      WorkflowJSON_Impl();

      /** Constructor with string, will throw if string is incorrect. */
      WorkflowJSON_Impl(const std::string& s);

      /** Constructor with path, will throw if path does not exist or file is incorrect. */
      WorkflowJSON_Impl(const openstudio::path& p);

      /** Get the workflow as a string. */
      std::string string(bool includeHash = true) const;

      /** Get a stored hash of the workflow. */
      std::string hash() const;

      /** Compute the current hash of the workflow. */
      std::string computeHash() const;

      /** Check for updates and return true if there are any, updates value of the stored hash. */
      bool checkForUpdates();

      /** Saves this file to the current location. */
      bool save() const;

      /** Saves this file to a new location. */
      bool saveAs(const openstudio::path& p) const;

      /** Returns the absolute path this workflow was loaded from or saved to, empty for new WorkflowJSON. */
      boost::optional<openstudio::path> oswPath() const;

      /** Returns the absolute path to the directory this workflow was loaded from or saved to.  Returns current working dir for new WorkflowJSON. */
      openstudio::path oswDir() const;

      /** Returns the root directory, default value is '.'. Evaluated relative to oswDir if not absolute. */
      openstudio::path rootDir() const;
      openstudio::path absoluteRootDir() const;

      /** Returns the paths that will be searched in order for files, default value is './files/'. Evaluated relative to rootDir if not absolute. */
      std::vector<openstudio::path> filePaths() const;
      std::vector<openstudio::path> absoluteFilePaths() const;

      /** Attempts to find a file by name, searches through filePaths in order and returns first match. */
      boost::optional<openstudio::path> findFile(const openstudio::path& file);
      boost::optional<openstudio::path> findFile(const std::string& fileName);

      /** Returns the paths that will be searched in order for measures, default value is './measures/'. Evaluated relative to rootDir if not absolute. */
      std::vector<openstudio::path> measurePaths() const;
      std::vector<openstudio::path> absoluteMeasurePaths() const;

      /** Attempts to find a measure by name, searches through measurePaths in order and returns first match. */
      boost::optional<openstudio::path> findMeasure(const openstudio::path& measureDir);
      boost::optional<openstudio::path> findMeasure(const std::string& measureDirName);

      /** Returns the seed file path. Evaluated relative to filePaths if not absolute. */
      boost::optional<openstudio::path> seedFile() const;

      /** Returns the weather file path. Evaluated relative to filePaths if not absolute. */
      boost::optional<openstudio::path> weatherFile() const;

      /** Returns the workflow steps. */
      std::vector<WorkflowStep> workflowSteps() const;

      /** Assigns the workflow steps. */
      bool setWorkflowSteps(const std::vector<WorkflowStep>& steps);

    private:

      // configure logging
      REGISTER_LOGGER("openstudio.WorkflowJSON");

      openstudio::path m_path;
      Json::Value m_value;
    };

} // detail
} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWJSON_IMPL_HPP
