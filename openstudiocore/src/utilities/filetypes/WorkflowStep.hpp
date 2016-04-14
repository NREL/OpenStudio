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

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEP_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEP_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
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

  // configure logging
  REGISTER_LOGGER("openstudio.WorkflowStep");

  std::string m_measureDirName;
  std::map<std::string, Variant> m_arguments;

};

} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWSTEP_HPP
