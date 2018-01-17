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

#include "EnergyPlusPostProcessJob.hpp"
#include "PostProcessReporting.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include "../../utilities/data/EndUses.hpp"
#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  EnergyPlusPostProcessJob::EnergyPlusPostProcessJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::EnergyPlusPostProcess, tools, params, files, t_restoreData)
  {
    try {
      m_sql = files.getLastByFilename("eplusout.sql");
      resetFiles(m_files, m_sql);
    } catch (const std::runtime_error &) {
    }

    // what should this be?
    m_description = buildDescription("sql");
  }

  EnergyPlusPostProcessJob::~EnergyPlusPostProcessJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void EnergyPlusPostProcessJob::requestStop()
  {
  }

  bool EnergyPlusPostProcessJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun) {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_sql){
        return filesChanged(m_files, *t_lastrun);
      } else {
        // must have a sql
        return true;
      }
    }
  }

  void EnergyPlusPostProcessJob::cleanup()
  {
  }


  std::string EnergyPlusPostProcessJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void EnergyPlusPostProcessJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir();
    QWriteLocker l(&m_mutex);

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    
    try {
      m_sql = sqlFile();
      resetFiles(m_files, m_sql);
    } catch (const std::exception &e) {
      std::vector<std::pair<ErrorType, std::string> > err;
      err.push_back(std::make_pair(ErrorType::Error, e.what()));
      errors = JobErrors(ruleset::OSResultValue::Fail, err);
    }

    if (m_sql)
    {
      LOG(Info, "EnergyPlusPostProcess starting, filename: " << toString(m_sql->fullPath));
    }

    LOG(Info, "EnergyPlusPostProcess starting, outdir: " << toString(outpath));

    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    if (errors.result == ruleset::OSResultValue::Fail)
    {
      setErrors(errors);
      return;
    }

    SqlFile sqlFile(m_sql->fullPath);


    try {
      boost::filesystem::create_directories(outpath);

      std::vector<Attribute> attributes = PostProcessReporting::go(sqlFile,"EnergyPlusPostProcess");

      if (attributes.empty())
      {
        LOG(Warn, "No attributes loaded for report");
      }

      Attribute report("Report", attributes);
      bool result = report.saveToXml(outpath / openstudio::toPath("report.xml"));
      if (!result){
        LOG_AND_THROW("Failed to write report.xml");
      }

    } catch (const std::exception &e) {
      LOG(Error, "Error with EnergyPlusPostProcessJob: " + std::string(e.what()));
      errors.addError(ErrorType::Error, "Error with EnergyPlusPostProcessJob: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    // Change this to whatever output files you generate
    emitOutputFileChanged(RunManager_Util::dirFile(outpath / openstudio::toPath("report.xml")));
    setErrors(errors);
  }

  std::string EnergyPlusPostProcessJob::getOutput() const
  {
    return "";
  }

  void EnergyPlusPostProcessJob::basePathChanged()
  {
    m_sql.reset();
    resetFiles(m_files);
  }

  FileInfo EnergyPlusPostProcessJob::sqlFile() const
  {
    if (!m_sql)
    {
      return allInputFiles().getLastByFilename("eplusout.sql");
    } else {
      return *m_sql;
    }
  }

  Files EnergyPlusPostProcessJob::outputFilesImpl() const
  {
    openstudio::path outpath = outdir();
    // Dan: what's the output files generated?
    if (!boost::filesystem::exists(outpath / toPath("report.xml")))
    {
      // no output file has been generated yet
      return Files();
    }

    Files f;
    f.append(FileInfo(outpath / toPath("report.xml"), "xml"));
    return f;
  }


} // detail
} // runmanager
} // openstudio



