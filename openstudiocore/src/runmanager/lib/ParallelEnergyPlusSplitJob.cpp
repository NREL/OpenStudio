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

#include "ParallelEnergyPlusSplitJob.hpp"
#include "ParallelEnergyPlus/ParallelEnergyPlus.hpp"
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
#include <utilities/idd/RunPeriod_FieldEnums.hxx>

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  ParallelEnergyPlusSplitJob::ParallelEnergyPlusSplitJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::ParallelEnergyPlusSplit, tools, params, files, t_restoreData),
      m_numSplits(boost::lexical_cast<int>(params.get("numsplits").children.at(0).value)),
      m_offset(boost::lexical_cast<int>(params.get("offset").children.at(0).value))
  {
    try {
      m_input = files.getLastByExtension("idf");
      resetFiles(m_files, m_input);
    } catch (const std::runtime_error &) {
    }

    m_description = buildDescription("idf");
  }

  ParallelEnergyPlusSplitJob::~ParallelEnergyPlusSplitJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void ParallelEnergyPlusSplitJob::requestStop()
  {
  }

  bool ParallelEnergyPlusSplitJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun)
    {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_input)
      {
        return filesChanged(m_files, *t_lastrun);
      } else {
        // if the model file we are using has not been establised yet,
        // return outofdate
        return true;
      }
    }
  }

  void ParallelEnergyPlusSplitJob::cleanup()
  {
  }

  std::string ParallelEnergyPlusSplitJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void ParallelEnergyPlusSplitJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir();
    QWriteLocker l(&m_mutex);
    if (!m_input)
    {
      m_input = inputFile();
      resetFiles(m_files, m_input);
    }

    LOG(Info, "ParallelEnergyPlusSplit starting, filename: " << toString(m_input->fullPath));
    LOG(Info, "ParallelEnergyPlusSplit starting, outdir: " << toString(outpath));

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    try {
      boost::filesystem::create_directories(outpath);

      FileInfo inputfile = inputFile();
      openstudio::path input = inputfile.fullPath;
      LOG(Debug, "Splitting inputfile: " << toString(input) << " into " << m_numSplits << " parts");
      std::vector<openstudio::path> outfilepaths = generateFileNames(outpath, m_numSplits);

      ParallelEnergyPlus p(input, m_numSplits, m_offset);

      for (int i = 0; i < m_numSplits; ++i)
      {
        p.writePartition(i, outfilepaths[i]);
        emitOutputFileChanged(RunManager_Util::dirFile(outfilepaths[i]));
      }


    } catch (const std::exception &e) {
      LOG(Debug, "Error executing split job: " << e.what());
      errors.addError(ErrorType::Error, "Error with execution: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    setErrors(errors);
  }

  std::vector<openstudio::path> ParallelEnergyPlusSplitJob::generateFileNames(const openstudio::path &t_outdir, int t_num)
  {
    std::vector<openstudio::path> retval;

    for (int i = 0; i < t_num; ++i)
    {
      retval.push_back(t_outdir / toPath("out" + boost::lexical_cast<std::string>(i) + ".idf"));
    }

    return retval;
  }

  std::string ParallelEnergyPlusSplitJob::getOutput() const
  {
    return "";
  }

  void ParallelEnergyPlusSplitJob::basePathChanged()
  {
    m_input.reset();
    resetFiles(m_files);
  }

  FileInfo ParallelEnergyPlusSplitJob::inputFile() const
  {
    if (!m_input)
    {
      return allInputFiles().getLastByExtension("idf");
    } else {
      return *m_input;
    }
  }

  Files ParallelEnergyPlusSplitJob::outputFilesImpl() const
  {
    // Save off the set of requiredfiles, see below
    std::vector<std::pair<QUrl, openstudio::path> > requiredFiles = inputFile().requiredFiles;
    std::vector<openstudio::path> files = generateFileNames(outdir(), m_numSplits);

    Files retval;

    for (std::vector<openstudio::path>::const_iterator itr = files.begin();
         itr != files.end();
         ++itr)
    {
      if (boost::filesystem::exists(*itr))
      {
        FileInfo fi = RunManager_Util::dirFile(*itr);
        // we want to carry along the set of required files used in the original IDF to the children
        // (ie, the epw file)
        fi.requiredFiles = requiredFiles;
        retval.append(fi);
      }
    }

    return retval;
  }


} // detail
} // runmanager
} // openstudio



