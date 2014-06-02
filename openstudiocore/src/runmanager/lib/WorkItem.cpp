/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "WorkItem.hpp"
#include "JSON.hpp"

#include "RubyJobUtils.hpp"

#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/core/PathHelpers.hpp"

namespace openstudio {
namespace runmanager {

WorkItem::WorkItem()
{
}

WorkItem::WorkItem(openstudio::runmanager::JobType t_type)
  : type(t_type)
{
}

WorkItem::WorkItem(openstudio::runmanager::JobType t_type, const openstudio::runmanager::Tools &t_tools,
                   const openstudio::runmanager::JobParams &t_params, const openstudio::runmanager::Files &t_files,
                   const std::string &t_jobkeyname)
  : type(t_type), tools(t_tools), params(t_params), files(t_files), jobkeyname(t_jobkeyname)
{
}

bool WorkItem::operator==(const WorkItem &t_rhs) const
{
  return type == t_rhs.type
      && tools == t_rhs.tools
      && params == t_rhs.params
      && files == t_rhs.files
      && jobkeyname == t_rhs.jobkeyname;
}

boost::optional<FileReferenceType> WorkItem::inputFileType() const {
  OptionalFileReferenceType result;
  switch (type.value()) {
    // job types where input file type is undefined/doesn't matter
    case JobType::Workflow :
    case JobType::Null :
    case JobType::Dakota :
    case JobType::EnergyPlusPostProcess :
    case JobType::OpenStudioPostProcess :
      break;
      // job types that take IDF
    case JobType::EnergyPlus :
    case JobType::ExpandObjects :
    case JobType::ParallelEnergyPlusSplit :
    case JobType::ParallelEnergyPlusJoin :
    case JobType::IdfToModel :
    case JobType::EnergyPlusPreProcess :
    case JobType::Basement :
    case JobType::Slab :
      result = FileReferenceType(FileReferenceType::IDF);
      break;
      // job types that take OSM
    case JobType::ModelToIdf :
    case JobType::ModelToRad :
    case JobType::ModelToRadPreProcess :
      result = FileReferenceType(FileReferenceType::OSM);
      break;
      // job types that take XML
    case JobType::XMLPreprocessor :
      result = FileReferenceType(FileReferenceType::XML);
      break;
    case JobType::Ruby :
    case JobType::UserScript :
    {
      // get extension of argument 3 in RubyJobBuilder::addInputFile
      RubyJobBuilder builder(*this);
      std::vector< std::tuple<FileSelection, FileSource, std::string, std::string> > inputFiles = builder.inputFiles();
      for (std::vector< std::tuple<FileSelection, FileSource, std::string, std::string> >::const_iterator it = inputFiles.begin(),
           itEnd = inputFiles.end(); it != itEnd; ++it)
      {
        std::string ext = getFileExtension(toPath(std::get<2>(*it)));
        try {
          result = FileReferenceType(ext);
          break;
        }
        catch (...) {}
      }
      break;
    }
      // job types that I am unsure about
    case JobType::CalculateEconomics :
    case JobType::ReadVars :
    case JobType::PreviewIES :
      break;
    default:
      LOG_AND_THROW("This JobType not handled yet.");
  }
  return result;
}

boost::optional<FileReferenceType> WorkItem::outputFileType() const {
  OptionalFileReferenceType result;
  switch (type.value()) {
    // job types where output file type is undefined/is not a mainline energy model
    case JobType::Workflow :
    case JobType::Null :
    case JobType::EnergyPlusPostProcess :
    case JobType::OpenStudioPostProcess :
    case JobType::Dakota :
      break;
      // job types that return IDF
    case JobType::ModelToIdf :
    case JobType::ExpandObjects :
    case JobType::XMLPreprocessor :
    case JobType::ParallelEnergyPlusSplit :
    case JobType::ParallelEnergyPlusJoin :
    case JobType::EnergyPlusPreProcess :
    case JobType::Basement :
    case JobType::Slab :
      result = FileReferenceType(FileReferenceType::IDF);
      break;
      // job types that return OSM
    case JobType::IdfToModel :
    case JobType::ModelToRadPreProcess :
      result = FileReferenceType(FileReferenceType::OSM);
      break;
      // job types that return SQL
    case JobType::EnergyPlus :
      result = FileReferenceType(FileReferenceType::SQL);
      break;
    case JobType::Ruby :
    case JobType::UserScript :
    {
      // get argument 2 in RubyJobBuilder::copyRequiredFiles
      RubyJobBuilder builder(*this);
      std::vector< std::tuple<std::string, std::string, std::string> > requiredFiles = builder.copyRequiredFiles();
      for (std::vector< std::tuple<std::string, std::string, std::string> >::const_iterator it = requiredFiles.begin(),
           itEnd = requiredFiles.end(); it != itEnd; ++it)
      {
        std::string ext = std::get<1>(*it);
        try {
          result = FileReferenceType(ext);
          break;
        }
        catch (...) {}
      }
      break;
    }
      // job types that I am unsure about
    case JobType::CalculateEconomics :
    case JobType::ReadVars :
    case JobType::PreviewIES :
      break;
    default:
      LOG_AND_THROW("This JobType not handled yet.");
  }
  return result;
}


WorkItem WorkItem::fromJSON(const std::string &t_json)
{
  return detail::JSON::toWorkItem(t_json);
}

std::string WorkItem::toJSON() const
{
  return detail::JSON::toJSON(*this);
}

bool saveJSON(const std::vector<WorkItem> &t_workItems,
              const openstudio::path& p,
              bool overwrite)
{
  return detail::JSON::saveJSON(t_workItems,p,overwrite);
}

std::ostream& toJSON(const std::vector<WorkItem> &t_workItems,
                     std::ostream& os)
{
  os << detail::JSON::toJSON(t_workItems);
  return os;
}

std::string toJSON(const std::vector<WorkItem> &t_workItems) {
  return detail::JSON::toJSON(t_workItems);
}

std::vector<WorkItem> toVectorOfWorkItem(const openstudio::path &t_pathToJson) {
  return detail::JSON::toVectorOfWorkItem(t_pathToJson);
}

std::vector<WorkItem> toVectorOfWorkItem(std::istream &t_json) {
  // istream -> string code from
  // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  std::string contents;
  t_json.seekg(0, std::ios::end);
  contents.resize(t_json.tellg());
  t_json.seekg(0, std::ios::beg);
  t_json.read(&contents[0], contents.size());
  return detail::JSON::toVectorOfWorkItem(contents);
}

std::vector<WorkItem> toVectorOfWorkItem(const std::string &t_json) {
  return detail::JSON::toVectorOfWorkItem(t_json);
}

} // runmanager
} // openstudio


