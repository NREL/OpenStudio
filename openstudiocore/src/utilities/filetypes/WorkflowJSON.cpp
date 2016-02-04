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

#include "WorkflowJSON.hpp"

#include "../core/Assert.hpp"

#include <boost/optional.hpp>

#include <string>
#include <fstream>

namespace openstudio{
  
WorkflowJSON::WorkflowJSON()
{}

WorkflowJSON::WorkflowJSON(const openstudio::path& p)
  : m_path(p)
{
  if (!boost::filesystem::exists(m_path) || !boost::filesystem::is_regular_file(m_path)){
    LOG_AND_THROW("Path '" << m_path << "' is not a WorkflowJSON file");
  }

  // open file
  std::ifstream ifs(openstudio::toString(m_path));
  
  Json::Reader reader;
  bool parsingSuccessful = reader.parse(ifs, m_json);
  if ( !parsingSuccessful ){
    std::string errors = reader.getFormattedErrorMessages();
    LOG_AND_THROW("WorkflowJSON '" << toString(m_path) << "' cannot be processed, " << errors);
  }
}

boost::optional<WorkflowJSON> WorkflowJSON::load(const openstudio::path& p)
{
  boost::optional<WorkflowJSON> result;
  try {
    result = WorkflowJSON(p);
  }catch(const std::exception&){
  }
  return result;
}

bool WorkflowJSON::save() const
{
  return false;
}

bool WorkflowJSON::saveAs(const openstudio::path& p) const
{
  return false;
}

openstudio::path WorkflowJSON::path() const
{
  return m_path;
}

openstudio::path WorkflowJSON::rootPath() const
{
  openstudio::path rootPath = toPath(".");
  Json::Value root = m_json.get("root", ".");
  if (root.isConvertibleTo(Json::stringValue)){
    rootPath = toPath(root.asString());
  }

  if (rootPath.is_absolute()){
    return rootPath;
  }

  if (m_path.empty()){
    return openstudio::path();
  } 

  return boost::filesystem::absolute(rootPath, m_path.parent_path());
}

openstudio::path WorkflowJSON::seedPath() const
{
  openstudio::path rootPath = this->rootPath();
  if (rootPath.empty()){
    return openstudio::path();
  }

  std::string seedName;
  Json::Value seed = m_json.get("seed", "");
  if (seed.isConvertibleTo(Json::stringValue)){
    seedName = seed.asString();
  }

  if (seedName.empty()){
    return openstudio::path();
  }

  return rootPath / toPath("seed") / toPath(seedName);
}

openstudio::path WorkflowJSON::weatherPath() const
{
  openstudio::path rootPath = this->rootPath();
  if (rootPath.empty()){
    return openstudio::path();
  }

  std::string weatherName;
  Json::Value weather = m_json.get("weather_file", "");
  if (weather.isConvertibleTo(Json::stringValue)){
    weatherName = weather.asString();
  }

  if (weatherName.empty()){
    return openstudio::path();
  }

  return rootPath / toPath("weather") / toPath(weatherName);
}

openstudio::path WorkflowJSON::measuresDir() const
{
  openstudio::path rootPath = this->rootPath();
  if (rootPath.empty()){
    return openstudio::path();
  }
  return rootPath / toPath("measures/");
}

} // openstudio
