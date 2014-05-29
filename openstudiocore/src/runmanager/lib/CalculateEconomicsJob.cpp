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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "CalculateEconomicsJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../model/Model.hpp"
#include "../../energyplus/ForwardTranslator.hpp"


#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  CalculateEconomicsJob::CalculateEconomicsJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::CalculateEconomics, t_tools, t_params, t_files, t_restoreData)
  {

  }

  CalculateEconomicsJob::~CalculateEconomicsJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void CalculateEconomicsJob::requestStop()
  {
  }

  bool CalculateEconomicsJob::outOfDateImpl(const boost::optional<QDateTime> &) const
  {
    assert(!"Not implemented");
    return true;
  }

  void CalculateEconomicsJob::cleanup()
  {
    assert(!"Not implemented");
  }

  std::string CalculateEconomicsJob::description() const
  {
    assert(!"Not implemented");
    std::string desc = "CalculateEconomics";
    return desc;
  }

  void CalculateEconomicsJob::startImpl(const std::shared_ptr<ProcessCreator> &t_pc)
  {
    assert(!"Not implemented");
  }

  std::string CalculateEconomicsJob::getOutput() const
  {
    assert(!"Not implemented");
    return "";
  }

  /*
  std::vector<FileInfo> CalculateEconomicsJob::inputFiles() const
  {
    assert(!"Not implemented");
    return std::vector<FileInfo>();
  }
*/

  Files CalculateEconomicsJob::outputFilesImpl() const
  {
    assert(!"Not implemented");
    return Files();
  }

  void CalculateEconomicsJob::basePathChanged()
  {
  }

} // detail
} // runmanager
} // openstudio



