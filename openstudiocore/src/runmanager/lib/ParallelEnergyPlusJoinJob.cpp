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

#include "ParallelEnergyPlusJoinJob.hpp"
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
#include "../../utilities/sql/SqlFile.hpp"

#include <sqlite/sqlite3.h>

#include "ParallelEnergyPlus/SqliteMerge.hpp"
#include "ParallelEnergyPlus/SqliteObject.hpp"

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  ParallelEnergyPlusJoinJob::ParallelEnergyPlusJoinJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
          const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::ParallelEnergyPlusJoin, tools, params, files, t_restoreData),
      m_numSplits(boost::lexical_cast<int>(params.get("numsplits").children.at(0).value)),
      m_offset(boost::lexical_cast<int>(params.get("offset").children.at(0).value))
  {
    m_description = buildDescription("");
  }

  ParallelEnergyPlusJoinJob::~ParallelEnergyPlusJoinJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void ParallelEnergyPlusJoinJob::requestStop()
  {
  }

  bool ParallelEnergyPlusJoinJob::outOfDateImpl(const boost::optional<QDateTime> &) const
  {
    return false;
  }

  void ParallelEnergyPlusJoinJob::cleanup()
  {
  }

  void ParallelEnergyPlusJoinJob::basePathChanged()
  {
  }

  std::string ParallelEnergyPlusJoinJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void ParallelEnergyPlusJoinJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir(true);

    QWriteLocker l(&m_mutex);

    LOG(Info, "ParallelEnergyPlusJoin starting, outdir: " << toString(outpath));

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    try {
      LOG(Error, "Merging of jobs does not seem to be fully implemented yet");

      boost::filesystem::create_directories(outpath);

      LOG(Debug, "Joining energyplus job run from " << m_numSplits << " parts");

      // Files will be returned in order or the children
      Files f = allInputFiles().getAllByFilename("eplusout.sql");



      std::vector<FileInfo> eplussqlfiles = f.files();

      LOG(Info, eplussqlfiles.size() << " input eplusout.sql files found");

      if (static_cast<int>(eplussqlfiles.size()) != m_numSplits)
      {
        throw std::runtime_error("Num sqlfiles does not match splits expected");
      }

      openstudio::path outFile = outpath / toPath("eplusout.sql");



      // clean up generated sql files, removing duplicated data
      for (size_t i = 1; i < eplussqlfiles.size(); ++i)
      {
        SqliteObject sql(eplussqlfiles[i].fullPath);

        boost::gregorian::date tmp_date(sql.getStartDay());
        boost::gregorian::date start_date(tmp_date);

        // BLB  remove the design days from the slaves in case they are still there.
        sql.removeDesignDay();
        start_date += boost::gregorian::date_duration(m_offset);
          
        while (tmp_date < start_date)
        {
          sql.deleteDay(tmp_date);  //BLBtest
          //std::cout << boost::gregorian::to_simple_string(tmp_date) << std::endl;
          tmp_date = tmp_date + boost::gregorian::date_duration(1);
        }
      }


      SqliteMerge merge;
      LOG(Info, "Copying 0th file into place: " << openstudio::toString(eplussqlfiles[0].fullPath) << " to " << openstudio::toString(outFile));
      boost::filesystem::remove(outFile);
      boost::filesystem::copy_file(eplussqlfiles[0].fullPath, outFile, boost::filesystem::copy_option::overwrite_if_exists);

      LOG(Info, "Merging base, 0th file: " << openstudio::toString(outFile));
      merge.loadFile(outFile);

      // clean up generated sql files, removing duplicated data
      for (size_t i = 1; i < eplussqlfiles.size(); ++i)
      {
        LOG(Info, "Merging " << i << "th file: " << openstudio::toString(eplussqlfiles[i].fullPath));
        merge.loadFile(eplussqlfiles[i].fullPath);
      }

      merge.mergeFiles();

      // emit the file changed
      emitOutputFileChanged(RunManager_Util::dirFile(outFile));
    } catch (const std::exception &e) {
      errors.addError(ErrorType::Error, "Error with execution: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    setErrors(errors);
  }

  std::string ParallelEnergyPlusJoinJob::getOutput() const
  {
    return "";
  }

  Files ParallelEnergyPlusJoinJob::outputFilesImpl() const
  {
    openstudio::path outfile = outdir() / toPath("eplusout.sql");

    Files retval;

    if (boost::filesystem::exists(outfile))
    {
      retval.append(RunManager_Util::dirFile(outfile));
    }

    return retval;
  }


} // detail
} // runmanager
} // openstudio



