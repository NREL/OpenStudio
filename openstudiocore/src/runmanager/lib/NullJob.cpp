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

#include "NullJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/WeatherFile_Impl.hpp"
#include "../../energyplus/ForwardTranslator.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>
#include "../../utilities/core/System.hpp"

#include <QDir>
#include <QDateTime>
#include <QMutexLocker>

namespace openstudio {
namespace runmanager {
namespace detail {


  NullJob::NullJob(const UUID &t_uuid,
                   const Tools &tools,
                   const JobParams &params,
                   const Files &files,
                   const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::Null, tools, params, files, t_restoreData)
  {
  }

  NullJob::~NullJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void NullJob::requestStop()
  {
  }

  bool NullJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    return !t_lastrun;
  }

  void NullJob::cleanup()
  {
  }

  std::string NullJob::description() const
  {
    return "Null Job";
  }

  void NullJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    LOG(Info, "Null starting");

    {
      QMutexLocker l(&m_mutex);
    }

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    openstudio::path outpath;
    try {
      outpath = outdir(true);
      boost::filesystem::create_directories(outpath);
      setErrors(JobErrors(ruleset::OSResultValue::Success, std::vector<std::pair<runmanager::ErrorType, std::string> >() ));
    } catch (const std::exception &e) {
      LOG(Error, "NullJob error starting job: " << e.what() << ". Job path is: " 
          << toString(outpath));
      std::vector<std::pair<runmanager::ErrorType, std::string> > err;
      err.push_back(std::make_pair(runmanager::ErrorType::Error, e.what()));
      setErrors(JobErrors(ruleset::OSResultValue::Fail, err));
    }

  }

  std::string NullJob::getOutput() const
  {
    return "";
  }


  Files NullJob::outputFilesImpl() const
  {
    return Files();
  }


} // detail
} // runmanager
} // openstudio



