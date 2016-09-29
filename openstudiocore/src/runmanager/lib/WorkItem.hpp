/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef RUNMANAGER_LIB_WORKITEM_HPP
#define RUNMANAGER_LIB_WORKITEM_HPP

#include "RunManagerAPI.hpp"
#include "JobType.hpp"
#include "ToolInfo.hpp"
#include "FileInfo.hpp"
#include "JobParam.hpp"

#include "../../utilities/core/Logger.hpp"

namespace openstudio {

class FileReferenceType;

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
 
  /// \returns WorkItem represented by the given JSON strong
  ///
  /// \throws std::exception with description if something goes wrong 
  static WorkItem fromJSON(const std::string &t_json);

  /// \returns a JSON representation of this WorkItem
  ///
  /// This is the prefered method of transferring Workflow components over a network
  /// and between computers.
  ///
  /// \sa openstudio::runmanager::Job::toJSON for more advanced methods of persisting
  ///     and transferring Worfklow objects
  std::string toJSON() const;

  openstudio::runmanager::JobType type;
  openstudio::runmanager::Tools tools;
  openstudio::runmanager::JobParams params;
  openstudio::runmanager::Files files;
  std::string jobkeyname;

  REGISTER_LOGGER("openstudio.runmanager.WorkItem");
};

RUNMANAGER_API bool saveJSON(const std::vector<WorkItem> &t_workItems,
                             const openstudio::path& p,
                             bool overwrite=false);

RUNMANAGER_API std::ostream& toJSON(const std::vector<WorkItem> &t_workItems,
                                    std::ostream& os);

RUNMANAGER_API std::string toJSON(const std::vector<WorkItem> &t_workItems);

RUNMANAGER_API std::vector<WorkItem> toVectorOfWorkItem(const openstudio::path &t_pathToJson);

RUNMANAGER_API std::vector<WorkItem> toVectorOfWorkItem(std::istream &t_json);

RUNMANAGER_API std::vector<WorkItem> toVectorOfWorkItem(const std::string &t_json);

/// gets the argument for the report request measure from a vector of work items
// DLM: not sure if this is the best place for this
RUNMANAGER_API std::string getReportRequestMeasureArgument(const std::vector<runmanager::WorkItem>& workItems);

} // runmanager
} // openstudio

#endif // RUNMANAGER_LIB_WORKITEM_HPP
