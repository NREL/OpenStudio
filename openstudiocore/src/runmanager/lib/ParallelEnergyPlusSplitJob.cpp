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



