/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef RUNMANAGER_WORKITEM_HPP
#define RUNMANAGER_WORKITEM_HPP

#include "RunManagerAPI.hpp"
#include "JobType.hpp"
#include "ToolInfo.hpp"
#include "FileInfo.hpp"
#include "JobParam.hpp"

#include <utilities/core/Logger.hpp>

namespace openstudio {

class FileReferenceType;

namespace ruleset {
  class ModelRuleset;
}

namespace runmanager {

class RUNMANAGER_API WorkItem {
 public:
  /** @name Constructors */
  //@{

  WorkItem();
  WorkItem(openstudio::runmanager::JobType t_type);
  WorkItem(openstudio::runmanager::JobType t_type,
           const openstudio::runmanager::Tools &t_tools,
           const openstudio::runmanager::JobParams &t_params,
           const openstudio::runmanager::Files &t_files,
           const std::string &t_jobkeyname="");
  WorkItem(const openstudio::ruleset::ModelRuleset& modelRuleset);

  //@}
  /** @name Getters and Queries */
  //@{

  bool operator==(const WorkItem &t_rhs) const;

  /** Returns the type of the primary input file for this work item. Generally used to
   *  keep track of the energy model type (OSM, IDF) at various points in a workflow. */
  boost::optional<FileReferenceType> inputFileType() const;

  /** Returns the type of the primary output file for this work item. Generally used to
   *  keep track of the energy model type (OSM, IDF) at various points in a workflow. */
  boost::optional<FileReferenceType> outputFileType() const;

  //@}

  openstudio::runmanager::JobType type;
  openstudio::runmanager::Tools tools;
  openstudio::runmanager::JobParams params;
  openstudio::runmanager::Files files;
  std::string jobkeyname;

  REGISTER_LOGGER("openstudio.runmanager.WorkItem");
};

} // runmanager
} // openstudio

#endif // RUNMANAGER_WORKITEM_HPP
