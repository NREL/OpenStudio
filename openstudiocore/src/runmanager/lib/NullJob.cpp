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



