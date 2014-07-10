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

#ifndef ANALYSISDRIVER_ANALYSISRUNOPTIONS_HPP
#define ANALYSISDRIVER_ANALYSISRUNOPTIONS_HPP

#include "AnalysisDriverAPI.hpp"

#include "../runmanager/lib/ToolInfo.hpp"

#include "../utilities/idf/URLSearchPath.hpp"
#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace analysisdriver {

/** \class QueuePausingBehavior
 *  \brief QueuePausingBehavoir says for how long RunManager should be paused while data points
 *  are being queued.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(QueuePausingBehavior,
    ((NoPause))
    ((PauseForFirstN))
    ((FullPause))
    ((FullPauseManualUnpause))
);
 *  \endcode */
OPENSTUDIO_ENUM(QueuePausingBehavior,
    ((NoPause))
    ((PauseForFirstN))
    ((FullPause))
    ((FullPauseManualUnpause))
);

/** \class JobCleanUpBehavior
 *  \brief JobCleanUpBehavior indicates the level to which files needed to run RunManager Jobs
 *  are to be kept on the file system once a Job completes successfully. It maps to the RunManager
 *  JobParam 'cleanoutfiles'.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(JobCleanUpBehavior,
    ((None)(Verbose))
    ((Standard))
    ((Maximum))
);
 *  \endcode */
OPENSTUDIO_ENUM(JobCleanUpBehavior,
    ((none)(verbose))
    ((standard))
    ((maximum))
);

/** AnalysisRunOptions is an options class for AnalysisDriver. It is used to specify important
 *  directories and runmanager::RunManager behavior for a given analysis::Analysis run. */
class ANALYSISDRIVER_API AnalysisRunOptions {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Constructor requires a specification of the analysis workingDirectory, which is where all
   *  analysis::DataPoints will be run (in folders called dataPoint1, dataPoint2, etc.). The
   *  rubyIncludeDirectory should be specified if you are working off a developer build of
   *  OpenStudio; it is not required if using installed OpenStudio Ruby bindings. At this time,
   *  the dakotaExePath must be explicitly specified if you plan to use an
   *  analysis::DakotaAlgorithm. */
  AnalysisRunOptions(const openstudio::path& workingDirectory,
                     const openstudio::path& rubyIncludeDirectory = openstudio::path(),
                     const openstudio::path& dakotaExePath = openstudio::path());

  //@}
  /** @name Getters */
  //@{

  openstudio::path workingDirectory() const;

  openstudio::path rubyIncludeDirectory() const;

  /** Defaults to QueuePausingBehavior::NoPause. */
  QueuePausingBehavior queuePausingBehavior() const;

  /** Defaults to JobCleanUpBehavior::standard, which attempts to purge input files and keep
   *  output files. */
  JobCleanUpBehavior jobCleanUpBehavior() const;

  /** Returns the number of data points to queue at a time. This options is passed to Dakota as
   *  evaluation_concurrency  when using DakotaAlgorithms. Otherwise the queueSize() is directly
   *  enforced by AnalysisDriver. */
  boost::optional<int> queueSize() const;

  std::vector<openstudio::URLSearchPath> urlSearchPaths() const;

  /** Defaults to false. */
  bool force() const;

  /** Returns number of data points to queue before unpausing RunManager queue. Defaults to 4. */
  int firstN() const;

  boost::optional<runmanager::Tools> runManagerTools() const;

  openstudio::path dakotaExePath() const;

  /** Returns true if the params.in and results.out files used to communicate with DAKOTA should
   *  be saved. Defaults to true. */
  bool dakotaFileSave() const;

  //@}
  /** @name Setters */
  //@{

  void setRubyIncludeDirectory(const openstudio::path& includeDir);

  void setQueuePausingBehavior(const QueuePausingBehavior& queueBehavior);

  void setJobCleanUpBehavior(const JobCleanUpBehavior& jobCleanUpBehavior);

  void setQueueSize(int queueSize);

  void clearQueueSize();

  void setUrlSearchPaths(const std::vector<openstudio::URLSearchPath>& searchPaths);

  void appendUrlSearchPath(const openstudio::URLSearchPath& searchPath);

  void clearUrlSearchPaths();

  void setForce(bool force);

  void setFirstN(int firstN);

  void setRunManagerTools(const runmanager::Tools& tools);

  void clearRunManagerTools();

  void setDakotaExePath(const openstudio::path& dakotaPath);

  void setDakotaFileSave(bool value);

  //@}
 private:
  REGISTER_LOGGER("openstudio.analysisdriver.AnalysisRunOptions");

  openstudio::path m_workingDirectory;
  QueuePausingBehavior m_queuePausingBehavior;
  JobCleanUpBehavior m_jobCleanUpBehavior;
  boost::optional<int> m_queueSize;
  openstudio::path m_rubyIncludeDirectory;
  std::vector<openstudio::URLSearchPath> m_urlSearchPaths;
  bool m_force;
  int m_firstN;
  boost::optional<runmanager::Tools> m_tools;

  openstudio::path m_dakotaExePath;
  bool m_dakotaFileSave;
};

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_ANALYSISRUNOPTIONS_HPP

