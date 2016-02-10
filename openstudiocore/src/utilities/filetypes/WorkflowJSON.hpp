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
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <jsoncpp/json.h>

namespace openstudio{
  
/** Class for accessing the OpenStudio Workflow (OSW) JSON format. */
class UTILITIES_API WorkflowStep
{
public:
  
  WorkflowStep(const std::string& measureDirName);

  std::string measureDirName() const;
    
  // DLM: OSW JSON examples have vector but run method takes map 
  std::map<std::string, Variant> arguments() const;

  boost::optional<Variant> getArgument(const std::string& name) const;

  void setArgument(const std::string& name, const Variant& value);
  void setArgument(const std::string& name, bool value);
  void setArgument(const std::string& name, double value);
  void setArgument(const std::string& name, int value);
  void setArgument(const std::string& name, const std::string& value);

  void removeArgument(const std::string& name);

  void clearArguments();

private:

  std::string m_measureDirName;
  std::map<std::string, Variant> m_arguments;

};

/** Class for accessing the OpenStudio Workflow (OSW) JSON format. */
class UTILITIES_API WorkflowJSON
{
public:

  /** Create a new, empty workflow. */
  WorkflowJSON();

  /** Constructor with string, will throw if string is incorrect. */
  WorkflowJSON(const std::string& s);

  /** Constructor with path, will throw if path does not exist or file is incorrect. */
  WorkflowJSON(const openstudio::path& p);

  /** Attempt to load a WorkflowJSON from string */
  static boost::optional<WorkflowJSON> load(const std::string& s);

  /** Attempt to load a WorkflowJSON from path */
  static boost::optional<WorkflowJSON> load(const openstudio::path& p);

  /** Get the workflow as a string. */
  std::string string() const;

  /** Get a hash of the workflow. */
  std::string hash() const;

  /** Saves this file to the current location. */
  bool save() const;

  /** Saves this file to a new location. */
  bool saveAs(const openstudio::path& p) const;

  /** Returns the original path this workflow was loaded from, can be empty. */
  openstudio::path path() const;

  /** Returns the absolute path to the root directory, can be empty. 
   ** Key name is 'root', default value is '.' */
  openstudio::path rootPath() const;

  /** Returns the absolute path to the seed file, can be empty. 
   ** Key name is 'seed', default value is '' */
  openstudio::path seedPath() const;

  /** Returns the absolute path to the weather file, can be empty. 
   ** Key name is 'weather_file', default value is '' */
  openstudio::path weatherPath() const;

  /** Returns the absolute path to the measures directory, can be empty. */
  openstudio::path measuresDir() const;

  /** Returns the attributes (other than steps). */
  std::vector<Attribute> attributes() const;

  /** Gets an attribute (other than steps). */
  boost::optional<Attribute> getAttribute(const std::string& name) const;

  /** Removes an attribute (other than steps). */
  void removeAttribute(const std::string& name);

  /** Sets an attribute (other than steps). */
  void setAttribute(const Attribute& attribute);
  
  /** Clears all attributes (other than steps). */
  void clearAttributes();

  /** Returns the workflow steps. */
  std::vector<WorkflowStep> workflowSteps() const;

  /** Assigns the workflow steps. */
  void setWorkflowSteps(const std::vector<WorkflowStep>& steps);

private:

  void parse(const Json::Value& json);

  Json::Value json(bool includeHash) const;

  // configure logging
  REGISTER_LOGGER("openstudio.WorkflowJSON");
  
  openstudio::path m_path;
  
  std::vector<Attribute> m_attributes; // does not store 'steps'
  std::vector<WorkflowStep> m_workflowSteps;
};

} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWJSON_HPP
