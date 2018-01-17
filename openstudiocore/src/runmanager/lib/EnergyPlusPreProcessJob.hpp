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

#ifndef RUNMANAGER_LIB_ENERGYPLUSPREPROCESSJOB_HPP
#define RUNMANAGER_LIB_ENERGYPLUSPREPROCESSJOB_HPP


#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "JobParam.hpp"
#include "../../utilities/core/Checksum.hpp"

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /**
   * Job type that postprocesses an EnergyPlus run
   *
   * This job runs in process as a separate thread.
   *
   * \sa \ref EnergyPlusPreProcessJobType
   */
  class EnergyPlusPreProcessJob : public Job_Impl
  {
    Q_OBJECT;

    public:
      /// EnergyPlusPreProcessJob constructor. 
      EnergyPlusPreProcessJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData);

      virtual ~EnergyPlusPreProcessJob();

      // Reimplemented virtual functions from Job_Impl
      virtual bool outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const override;
      virtual std::string description() const override;
      virtual Files outputFilesImpl() const override;
      virtual std::string getOutput() const override;
      virtual void cleanup() override;

      virtual void requestStop() override;

    protected:
      virtual void startImpl(const std::shared_ptr<ProcessCreator> &t_creator) override;

      virtual void basePathChanged() override;
      virtual void standardCleanImpl() override { /* nothing to do for this job type */ }

    private:
      REGISTER_LOGGER("openstudio.runmanager.EnergyPlusPreProcessJob");

      /// Returns the IDF file used
      FileInfo idfFile() const;

      mutable QReadWriteLock m_mutex;

      std::map<openstudio::path, FileTrack> m_files; //< Files tracked for outOfDate status
      boost::optional<FileInfo> m_idf; //< EnergyPlus Idf 
      mutable boost::optional<Files> m_outputfiles; //< Xml file that was created from run
      std::string m_description; //< Description of job
  }; 

}
}
}
#endif // RUNMANAGER_LIB_ENERGYPLUSPREPROCESSJOB_HPP
