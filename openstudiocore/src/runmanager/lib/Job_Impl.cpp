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

#include "Job_Impl.hpp"
#include "JobErrorsDisplay.hpp"
#include "RunManager_Impl.hpp"
#include "MergeJobError.hpp"
#include "MergedJobResults.hpp"
#include <QReadLocker>
#include <QWriteLocker>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../../utilities/core/Application.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include <QDir>

namespace openstudio {
namespace runmanager {
namespace detail {

  Job_Impl::Job_Impl(const UUID &t_id, const JobType &t_jobType,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_inputFiles,
      const JobState &t_jobState)
    : m_cleanTypeToRun(none), m_id(t_id), m_jobType(t_jobType), m_index(-1),
      m_tools(t_tools), m_params(t_params), m_inputFiles(t_inputFiles),
      m_canceled(false), m_runnable(true), m_force(false),
      m_basePath(boost::filesystem::initial_path<openstudio::path>()),
      m_jobState(t_jobState),
      m_hasRunSinceLoading(false),
      m_externallyManaged(t_params.has("jobExternallyManaged"))
  {
    RunManager_Impl::registerMetaTypes();

    if (m_params.has("job_base_path"))
    {
      JobParam p = m_params.get("job_base_path");
      if (p.children.size() == 1)
      {
        openstudio::path basepath = toPath(p.children[0].value);
        m_basePath = basepath;
        LOG(Debug, "loaded basepath " << openstudio::toString(basepath));
        m_params.remove("job_base_path"); // house keeping, it will get persisted via the getBasePath method if necessary
      }
    }

    m_history.push_back(std::make_pair(boost::posix_time::microsec_clock::universal_time(), AdvancedStatus(AdvancedStatusEnum::Idle)));

    connect(this, SIGNAL(finished()), this, SLOT(threadFinished()));
  }

  Job_Impl::~Job_Impl()
  {
    QWriteLocker l(&m_mutex);
    m_parent.reset();
    m_finishedJob.reset();
    m_children.clear();
    wait();
  }

  boost::optional<QDateTime> Job_Impl::lastRun() const
  {
    QReadLocker l(&m_mutex);
    return lastRunInternal();
  }

  boost::optional<QDateTime> Job_Impl::lastRunInternal() const
  {
    if (m_hasRunSinceLoading)
    {
      return m_lastRun;
    } else {
      return m_jobState.lastRun;
    }
  }

  Files Job_Impl::outputFiles() const
  {
    openstudio::path outpath = outdir();

    QReadLocker l(&m_mutex);

    if (m_hasRunSinceLoading)
    {
      l.unlock();
      return outputFilesImpl();
    } else {
      LOG(Debug, "Returning previously saved outputfiles");
      std::vector<FileInfo> files = m_jobState.outputFiles.files();

      for (auto & file : files)
      {
        if (!file.fullPath.is_complete())
        {
          if (relativePath(file.fullPath, outpath).empty())
          {
            file.fullPath = outpath / file.fullPath;
          }
        }
      }
      return Files(files);
    }
  }

  int Job_Impl::index() const
  {
    QReadLocker l(&m_mutex);
    return m_index;
  }

  void Job_Impl::setIndex(int index)
  {
    QWriteLocker l(&m_mutex);
    if (index != m_index)
    {
      m_index = index;
      l.unlock();

      emitStateChanged();
    }
  }

  void Job_Impl::mergeJob(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job)
  {
    if (t_job->parent() != t_parent)
    {
      throw MergeJobError("RHS is not a child of LHS");
    }

    if (t_parent->children().size() > 1)
    {
      throw MergeJobError("LHS Job has more than one child - merging not possible");
    }

    if (t_parent->finishedJob() == t_job && !t_parent->children().empty())
    {
      throw MergeJobError("LHS Job has children and job is finishedJob - merging not possible");
    }


    if (t_job->jobType() == JobType::Null)
    {
      std::vector<std::shared_ptr<Job_Impl> > children = t_job->children();

      std::for_each(children.begin(), children.end(), std::bind(&Job_Impl::addChild, t_parent, std::placeholders::_1));

      removeChild(t_job);

      QWriteLocker l(&m_mutex);
      m_tools.append(t_job->tools());
      m_params.append(t_job->params());
      m_inputFiles.append(t_job->inputFiles());

    } else {
      mergeJobImpl(t_parent, t_job);
    }
  }

  void Job_Impl::mergeJobImpl(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job)
  {
    throw MergeJobError("mergeJob not implemented for types " + jobType().valueName() + " and " + t_job->jobType().valueName());
  }


  Tools Job_Impl::allTools() const
  {
    {
      QReadLocker l(&m_cacheMutex);
      if (m_allTools)
      {
        return *m_allTools;
      }
    }


    std::shared_ptr<Job_Impl> p = m_parent.lock();
    Tools t = tools();

    QReadLocker l(&m_mutex);
    Tools ret;
    if (p)
    {
      ret.append(p->allTools());

      if (p->m_finishedJob.get() == this)
      {
        ret.append(p->allChildTools());
      }
    }
    ret.append(t);

    {
      QWriteLocker l2(&m_cacheMutex);
      m_allTools = ret;
    }

    return ret;

  }

  void Job_Impl::setRemoteId(int t_remoteid, int t_remoteTaskId)
  {
    QWriteLocker l(&m_mutex);
    m_remoteid = std::make_pair(t_remoteid, t_remoteTaskId);

  }

  boost::optional<std::pair<int, int> > Job_Impl::getRemoteId()
  {
    QReadLocker l(&m_mutex);
    return m_remoteid;
  }

  void Job_Impl::run()
  {
    //LOG(Info, boost::posix_time::microsec_clock::local_time() << " run thread starting: " << toString(m_id) << " " << QThread::currentThreadId());

    QWriteLocker l(&m_mutex);
    m_hasRunSinceLoading = true;
    m_lastRun = QDateTime::currentDateTime();
    if (m_lastStartTime)
    {
      LOG(Info, "Starting previously run job: " << toString(m_id) << " index: " << m_index);
    }
    m_lastStartTime = QDateTime::currentDateTime();
    m_osLastStartTime = openstudio::toDateTime(*m_lastStartTime);
    {
      QWriteLocker l2(&m_cacheMutex);
      m_allTools.reset();
      m_allParams.reset();
      m_allInputFiles.reset();
      m_outdir.reset();
    }
    l.unlock();

    startImpl(m_processCreator);

    l.relock();
    m_lastEndTime = QDateTime::currentDateTime();
    m_osLastEndTime = openstudio::toDateTime(*m_lastEndTime);
    l.unlock();

    //LOG(Info, boost::posix_time::microsec_clock::local_time() << " run thread finished: " << toString(m_id) << " " << QThread::currentThreadId());
    moveToThread(QApplication::instance()->thread()); // move event processing to main thread
    exit();

    //LOG(Info, boost::posix_time::microsec_clock::local_time() << " run thread moved: " << toString(m_id) << " " << QThread::currentThreadId());
  }

  void Job_Impl::threadFinished()
  {
    //LOG(Info, boost::posix_time::microsec_clock::local_time() << " threadFinished signal received: " << toString(m_id) << " " << QThread::currentThreadId());
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Idle));
    exit();

    OS_ASSERT(this != QThread::currentThread());


    while (!wait(1))
    {
      openstudio::Application::instance().processEvents();
    }

    OS_ASSERT(!isRunning());
    OS_ASSERT(isFinished());

    JobErrors errors = this->errors();

    m_cleanTypeToRun = standard;

    if (errors.result == ruleset::OSResultValue::Success)
    {

      JobParams params = allParams();
      if (params.has("cleanoutfiles") && !params.get("cleanoutfiles").children.empty())
      {
        std::string typestr = params.get("cleanoutfiles").children.at(0).value;

        if (typestr == "none")
        {
          m_cleanTypeToRun = none;
        } else if (typestr == "standard") {
          m_cleanTypeToRun = standard;
        } else if (typestr == "maximum") {
          m_cleanTypeToRun = maximum;
        } else {
          LOG(Error, "Unknown cleanoutfiles setting: " << typestr);
        }
      } 

    } else {
      m_cleanTypeToRun = none;
      LOG(Info, "Not running cleanup, job failed");
    }

    doCleanUp();

    emitFinished(errors, lastRun(), outputFiles());

    //LOG(Info, boost::posix_time::microsec_clock::local_time() << " thread joined: " << toString(m_id));
  }

  void Job_Impl::doCleanUp()
  {
    std::shared_ptr<Job_Impl> p = parent();
    TreeStatusEnum treestatus = treeStatus();

    if (treestatus == TreeStatusEnum::Finished)
    {
      if (p) p->doCleanUp();
      switch (m_cleanTypeToRun)
      {
      case none:
        return;
      case standard:
        m_cleanTypeToRun = none;
        standardClean();
        break;
      case maximum:      
        m_cleanTypeToRun = none;
        maximumClean();
        break;
      };
    }

  }

  bool Job_Impl::fileComparitor(const openstudio::path &t_lhs, const openstudio::path &t_rhs)
  {
    // Err on the side of caution, always be case insensitive
    return openstudio::toPath(openstudio::toQString(t_lhs).toUpper()) < openstudio::toPath(openstudio::toQString(t_rhs).toUpper());
  }

  void Job_Impl::standardClean()
  {
    openstudio::path outpath = outdir();
    std::vector<FileInfo> files = outputFiles().files();

    // make the set case-insensitive
    std::set<openstudio::path, bool (*)(const openstudio::path &, const openstudio::path &)> outputfiles(&Job_Impl::fileComparitor);

    // Build a set of all of the output files generated by this job,
    // so that we don't accidentally delete them.
    for (const auto & fileInfo : files)
    {
      openstudio::path f = fileInfo.fullPath;
      if (!f.is_complete())
      {
        // only add outpath if we are not relative already
        if (relativePath(f, outpath).empty())
        {
          f = outpath / f;
        }
      }

      f = openstudio::completeAndNormalize(f);
      outputfiles.insert(f);

      LOG(Trace, "StandardClean: OutputFile reported from job: " << openstudio::toString(f));

      for (const auto & requiredFile : fileInfo.requiredFiles)
      {
        openstudio::path r = openstudio::toPath(requiredFile.first.toLocalFile());
        if (!r.is_complete())
        {
          r = outpath / r;
        }
        outputfiles.insert(r);
        LOG(Trace, "StandardClean: OutputFile reported from job (required file): " << openstudio::toString(r));

      }
    }


    QDir dir(openstudio::toQString(outpath));

    QFileInfoList list = dir.entryInfoList();

    for (const auto & fileInfo : list)
    {
      QString extension = fileInfo.suffix().toLower();

      openstudio::path foundpath = openstudio::toPath(fileInfo.absoluteFilePath());

      if (outputfiles.find(foundpath) == outputfiles.end()
          && outputfiles.find(foundpath.replace_extension(openstudio::toPath(".osm").string())) == outputfiles.end())
      {
        if (!fileInfo.isDir())
        {
          LOG(Debug, "standardClean: " << openstudio::toString(fileInfo.absoluteFilePath()));

          if (!dir.remove(fileInfo.absoluteFilePath()))
          {
            LOG(Error, "standardClean: error removing file: " << openstudio::toString(fileInfo.absoluteFilePath()));
          }

          if (openstudio::toString(foundpath.extension()) == ".osm")
          {
            QFileInfo potentialdir(openstudio::toQString(foundpath.replace_extension(openstudio::toPath("").string())));
            if (potentialdir.isDir())
            {
              LOG(Debug, "standardClean: " << openstudio::toString(potentialdir.absoluteFilePath()));
              removeDirectory(openstudio::toPath(potentialdir.absoluteFilePath()));
            }
          }
        }
      } else {
        LOG(Debug, "standardClean skipping output file: " << openstudio::toString(fileInfo.absoluteFilePath()));
      }
    }


    standardCleanImpl();
  }

  void Job_Impl::maximumClean()
  {
    // start with a standardClean, then get what it leaves behind
    standardClean();

    QDir dir(openstudio::toQString(outdir()));

    QFileInfoList list = dir.entryInfoList();

    for (const auto & fileInfo : list)
    {
      QString extension = fileInfo.suffix().toLower();

      if (fileInfo.fileName().startsWith("out", Qt::CaseInsensitive)
          || extension == "osm"
          || extension == "idf"
          || extension == "sql"
          || extension == "xml"
          || extension == "htm"
          )
      {
        LOG(Debug, "maximumClean: skipping file: " << openstudio::toString(fileInfo.absoluteFilePath()));
      } else {
        if (!fileInfo.isDir())
        {
          LOG(Debug, "maximumClean: " << openstudio::toString(fileInfo.absoluteFilePath()));

          if (!dir.remove(fileInfo.absoluteFilePath()))
          {
            LOG(Error, "maximumClean: error removing file: " << openstudio::toString(fileInfo.absoluteFilePath()));
          }

          openstudio::path foundpath(openstudio::toPath(fileInfo.absoluteFilePath()));
          if (openstudio::toString(foundpath.extension()) == ".osm")
          {
            QFileInfo potentialdir(openstudio::toQString(foundpath.replace_extension(openstudio::toPath("").string())));
            if (potentialdir.isDir())
            {
              LOG(Debug, "maximumClean: " << openstudio::toString(fileInfo.absoluteFilePath()));
              removeDirectory(openstudio::toPath(potentialdir.absoluteFilePath()));
            }
          }
        }
      }
    }
  }

  boost::optional<QDateTime> Job_Impl::startTime() const
  {
    QReadLocker l(&m_mutex);
    return m_lastStartTime;
  }

  boost::optional<QDateTime> Job_Impl::endTime() const
  {
    QReadLocker l(&m_mutex);
    return m_lastEndTime;
  }

  boost::optional<openstudio::DateTime> Job_Impl::osStartTime() const
  {
    QReadLocker l(&m_mutex);
    return m_osLastStartTime;
  }

  boost::optional<openstudio::DateTime> Job_Impl::osEndTime() const
  {
    QReadLocker l(&m_mutex);
    return m_osLastEndTime;
  }


  bool Job_Impl::runningRemotely() const
  {
    QReadLocker l(&m_mutex);
    if (m_processCreator)
    {
      return m_processCreator->isRemoteManager() && isRunning();
    } else {
      return false;
    }
  }

  void Job_Impl::setProcessCreator(const std::shared_ptr<ProcessCreator> &t_pc)
  {
    QWriteLocker l(&m_mutex);
    m_processCreator = t_pc;
  }

  openstudio::path Job_Impl::outdir(bool t_throws) const
  {
    bool flatoutdir = false;
    std::string prefix;
    std::string suffix;
    openstudio::path parentpath;

    {
      QReadLocker l(&m_cacheMutex);
      if (m_outdir)
      {
        return *m_outdir;
      }

      if (!m_jobState.outdir.empty())
      {
        openstudio::path dir = m_jobState.outdir;
        if (!dir.is_complete())
        {
          dir = m_basePath / dir;
        }
        return dir;
      }
    }

    if (allParams().has("flatoutdir"))
    {
      flatoutdir = true;
      prefix = boost::lexical_cast<std::string>(index()) + "-";
    } 

    QReadLocker l(&m_mutex);
    std::shared_ptr<Job_Impl> p = m_parent.lock();
    if (p)
    {
      if (p->finishedJob() && p->finishedJob()->uuid() == m_id)
      {
        suffix = "-finishedjob";
      } else {
        suffix = "-" + boost::lexical_cast<std::string>(p->childIndexInList(m_id));
      }

      if (!flatoutdir)
      {
        parentpath = p->outdir();
      } else {
        parentpath = p->outdir().parent_path();
      }

    } else {
      try {
        parentpath = toPath(m_params.get("outdir").children.at(0).value);
      } catch (const std::exception &) {
        if (m_basePath.empty())
        {
          LOG(Error, "Error getting 'outdir' from parents, and no basepath set. Error in Job configuration");
          if (t_throws)
          {
            throw;
          }
        }
      }
    }

    openstudio::path ret = parentpath / toPath(prefix + m_jobType.valueName() + suffix);

    if (!m_basePath.empty())
    {
      ret = boost::filesystem::complete(ret, m_basePath);
    }

    {
      QWriteLocker l2(&m_cacheMutex);
      m_outdir = ret;
    }

    return ret;
  }

  openstudio::path Job_Impl::getBasePath() const
  {
    return m_basePath;
  }

  JobParams Job_Impl::allParams() const
  {
    {
      QReadLocker l(&m_cacheMutex);
      if (m_allParams)
      {
        return *m_allParams;
      }
    }

    JobParams myparams = params();
    QReadLocker l(&m_mutex);
    std::shared_ptr<Job_Impl> p = m_parent.lock();

    JobParams ret;

    if (p)
    {
      ret.append(p->allParams());

      if (p->m_finishedJob.get() == this)
      {
        ret.append(p->allChildParams());
      }
    }

    ret.append(myparams);

    {
      QWriteLocker l2(&m_cacheMutex);
      m_allParams = ret;
    }

    return ret;
  }

  Files Job_Impl::allFiles() const
  {
    Files f = allInputFiles();
    f.append(outputFiles());
    return f.complete(m_basePath);
  }

  Files Job_Impl::allInputFiles() const
  {
    {
      QReadLocker l(&m_cacheMutex);
      if (m_allInputFiles)
      {
        return *m_allInputFiles;
      }
    }


    Files inputfiles = inputFiles();

    QReadLocker l(&m_mutex);
    std::shared_ptr<Job_Impl> p = m_parent.lock();

    Files ret;
    if (p)
    {
      ret.append(p->allFiles());

      if (p->m_finishedJob.get() == this)
      {
        ret.append(p->allChildFiles());
      }
    }
    ret.append(inputfiles);
    ret = ret.complete(m_basePath);


    {
      QWriteLocker l2(&m_cacheMutex);
      m_allInputFiles = ret;
    }

    return ret;

  }

  Tools Job_Impl::allChildTools() const
  {
    Tools ret;

    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      ret.append(child->tools());
    }

    return ret;
  }

  JobParams Job_Impl::allChildParams() const
  {
    JobParams ret;

    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      ret.append(child->params());
    }

    return ret;
  }

  Files Job_Impl::allChildFiles() const
  {
    Files ret;

    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      ret.append(child->inputFiles());
      ret.append(child->outputFiles());
      ret.append(child->allChildFiles());
    }

    return ret.complete(m_basePath);
  }

  void Job_Impl::setParams(const JobParams &t_newParams)
  {
    QWriteLocker l(&m_mutex);
    m_params = t_newParams;
    m_allTools.reset();
    m_allParams.reset();
    m_allInputFiles.reset();
    m_outdir.reset();
  }

  void Job_Impl::setFiles(const Files &t_newFiles)
  {
    QWriteLocker l(&m_mutex);
    m_inputFiles = t_newFiles;
    m_allTools.reset();
    m_allParams.reset();
    m_allInputFiles.reset();
    m_outdir.reset();
  }


  Files Job_Impl::rawInputFiles() const
  {
    QReadLocker l(&m_mutex);
    return m_inputFiles;
  }


  Files Job_Impl::inputFiles() const
  {
    QReadLocker l(&m_mutex);
    return m_inputFiles.complete(m_basePath);
  }

  Tools Job_Impl::tools() const
  {
    QReadLocker l(&m_mutex);
    return m_tools;
  }

  JobParams Job_Impl::params() const
  {
    QReadLocker l(&m_mutex);
    return m_params;
  }

  UUID Job_Impl::uuid() const
  {
    QReadLocker l(&m_mutex);
    return m_id;
  }

  JobType Job_Impl::jobType() const
  {
    QReadLocker l(&m_mutex);
    return m_jobType;
  }

  bool Job_Impl::canceled() const
  {
    QReadLocker l(&m_mutex);
    return m_canceled;
  }

  boost::optional<QDateTime> Job_Impl::childrenLastRun() const
  {
    boost::optional<QDateTime> lr;

    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      boost::optional<QDateTime> childlastrun = child->lastRun();

      if (childlastrun)
      {
        if (lr)
        {
          if (*childlastrun > *lr)
          {
            childlastrun = lr;
          }
        } else {
          lr = childlastrun;
        }
      }

    }

    return lr;
  }

  bool Job_Impl::childrenRunning() const
  {
    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      if (child->isRunning())
      {
        return true;
      }
    }

    return false;
  }

  bool Job_Impl::childrenTreesRunning() const
  {
    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      if (child->treeRunning())
      {
        return true;
      }
    }

    return false;
  }

  JobErrors Job_Impl::childrenErrors() const
  {
    QReadLocker l(&m_mutex);
    JobErrors e;
    e.result = ruleset::OSResultValue::Success; // assume we succeeded until we failed
    for (const auto & child : m_children)
    {
      e = e + child->errors();
    }

    return e;
  }

  JobErrors Job_Impl::childrenTreesErrors() const
  {
    QReadLocker l(&m_mutex);
    JobErrors e;
    e.result = ruleset::OSResultValue::Success; // assume we succeeded until we failed
    for (const auto & child : m_children)
    {
      e = e + child->treeErrors();
    }

    return e;
  }

  bool Job_Impl::outOfDate() const
  {
    return outOfDateInternal(parent(), lastRun());
  }

  bool Job_Impl::outOfDateInternal(const std::shared_ptr<Job_Impl> &t_parent, const boost::optional<QDateTime> &t_lastrun) const
  {
    if (m_force || outOfDateImpl(t_lastrun))
    {
      return true;
    }

    if (t_parent)
    {
      if (t_parent->outOfDate() || t_parent->lastRun() > lastRun())
      {
        return true;
      }

      if (t_parent->finishedJob())
      {
        if ( t_parent->finishedJob()->m_id == m_id )
        {
          if (t_parent->childrenTreesOutOfDate() || t_parent->childrenLastRun() > lastRun())
          {
            // if we are the finished job, we are dependent on the children
            return true;
          }
        }
      }
    }

    // neither we nor any of our dependencies are out of date
    return false;
  }

  void Job_Impl::setCanceled(bool t_canceled)
  {
    QWriteLocker l(&m_mutex);
    const bool changed = (t_canceled != m_canceled);
    m_canceled = t_canceled;

    l.unlock();

    if (changed)
    {
      emitStateChanged();
    }
  }


  AdvancedStatus Job_Impl::status() const
  {
    QReadLocker l(&m_mutex);

    if (m_hasRunSinceLoading)
    {
      return m_status;
    } else {
      return m_jobState.status;
    }
  }


  void Job_Impl::addChildInternal(const std::shared_ptr<Job_Impl> &t_child)
  {
    QWriteLocker l(&m_mutex);
    m_children.push_back(t_child);
    LOG(Trace, "Child added: " + toString(m_id) + " " + toString(t_child->m_id) + " num children " + boost::lexical_cast<std::string>(m_children.size()));
    connect(t_child.get(), SIGNAL(treeChanged(const openstudio::UUID &)), this, SIGNAL(treeChanged(const openstudio::UUID &)));
  }

  void Job_Impl::addChild(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job)
  {
    std::shared_ptr<Job_Impl> jobparent(t_job->parent());
    if (jobparent)
    {
      jobparent->removeChild(t_job);
    }

    t_job->setParent(t_parent);
    t_parent->addChildInternal(t_job);
    t_parent->emitChildrenChanged();
  }

  bool Job_Impl::removeChild(const std::shared_ptr<Job_Impl> &t_job)
  {
    std::shared_ptr<Job_Impl> jobparent = t_job->parent();
    QWriteLocker l(&m_mutex);
    if (jobparent.get() == this)
    {
      if (t_job == m_finishedJob)
      {
        disconnect(m_finishedJob.get(), SIGNAL(treeChanged(const openstudio::UUID &)), this, SIGNAL(treeChanged(const openstudio::UUID &)));
        m_finishedJob.reset();
        return true;
      }

      for (auto itr = m_children.begin();
          itr != m_children.end();
          ++itr)
      {
        if (*itr == t_job)
        {
          LOG(Trace, "Child removed: " + toString(m_id) + " " + toString(t_job->m_id) + " num children " );

          (*itr)->setParent(std::shared_ptr<Job_Impl>());
          m_children.erase(itr);
          disconnect(t_job.get(), SIGNAL(treeChanged(const openstudio::UUID &)), this, SIGNAL(treeChanged(const openstudio::UUID &)));
          emitChildrenChanged();
          return true;
        }
      }
    }

    return false;
  }

  std::vector<std::shared_ptr<Job_Impl> > Job_Impl::children() const
  {
    QReadLocker l(&m_mutex);
    return m_children;
  }

  bool Job_Impl::childrenOutOfDate() const
  {
    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      if (child->outOfDate())
      {
        return true;
      }
    }

    return false;
  }

  bool Job_Impl::childrenTreesOutOfDate() const
  {
    std::vector<std::shared_ptr<Job_Impl> > children;

    {
      // scope for RAII mutex
      QReadLocker l(&m_mutex);
      children = m_children;
    }

    for (const auto & child : children)
    {
      if (child->treeOutOfDate())
      {
        return true;
      }
    }

    return false;
  }

  std::shared_ptr<Job_Impl> Job_Impl::finishedJob() const
  {
    QReadLocker l(&m_mutex);
    return m_finishedJob;
  }

  void Job_Impl::setFinishedJobInternal(const std::shared_ptr<Job_Impl> &t_job)
  {
    QWriteLocker l(&m_mutex);
    m_finishedJob = t_job;
    connect(t_job.get(), SIGNAL(treeChanged(const openstudio::UUID &)), this, SIGNAL(treeChanged(const openstudio::UUID &)));
  }

  void Job_Impl::setFinishedJob(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job)
  {
    std::shared_ptr<Job_Impl> parent = t_job->parent();
    if (parent)
    {
      t_job->setParent(std::shared_ptr<Job_Impl>());
      parent->removeChild(t_job);
    }

    t_job->setParent(t_parent);

    t_parent->setFinishedJobInternal(t_job);
    t_parent->emitChildrenChanged();
  }

  std::shared_ptr<Job_Impl> Job_Impl::parent() const
  {
    QReadLocker l(&m_mutex);
    return m_parent.lock();
  }

  void Job_Impl::setParent(const std::shared_ptr<Job_Impl> &t_parent)
  {
    QWriteLocker l(&m_mutex);
    QWriteLocker l2(&m_cacheMutex);
    m_parent = t_parent;
    m_allTools.reset();
    m_allParams.reset();
    m_allInputFiles.reset();
    m_outdir.reset();
    l.unlock();
    emit parentChanged(m_id);
  }

  std::vector<std::pair<boost::posix_time::ptime, AdvancedStatus> > Job_Impl::history() const
  {
    QReadLocker l(&m_mutex);
    return m_history;
  }

  void Job_Impl::emitStatusChanged(const AdvancedStatus &t_stat)
  {
    QWriteLocker l(&m_mutex);
    if (!m_hasRunSinceLoading)
    {
      m_jobState.status = t_stat;
      l.unlock();
      emit statusChanged(t_stat);
      emit treeChanged(m_id);
    } else {
      if (t_stat != m_status)
      {
        m_status = t_stat;
        m_history.push_back(std::make_pair(boost::posix_time::microsec_clock::universal_time(), t_stat));
        l.unlock();
        emit statusChanged(t_stat);
        emit treeChanged(m_id);
      }
    }
  }

  void Job_Impl::emitChildrenChanged()
  {
    emit childrenChanged(m_id);
    emit treeChanged(m_id);
  }

  JobErrors Job_Impl::errors() const
  {
    QReadLocker l(&m_mutex);

    if (m_hasRunSinceLoading)
    {
      return m_errors;
    } else {
      return m_jobState.errors;
    }
  }

  void Job_Impl::showErrors() const
  {
    JobErrors err = errors(); // Safely get it from a locked function
    JobErrorsDisplay jed(nullptr,Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint),err);
    jed.exec();
  }

  void Job_Impl::showTreeErrors() const
  {
    JobErrors err = treeErrors(); // Safely get it from a locked function
    JobErrorsDisplay jed(nullptr,Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint),err);
    jed.exec();
  }

  void Job_Impl::setRunnable(bool t_force)
  {
    QWriteLocker l(&m_mutex);

    const bool changed = (m_runnable != true) || (m_force != t_force);

    m_runnable = true;
    m_force = t_force;

    if (changed)
    {
      l.unlock();
      emitStateChanged();
    }
  }


  bool Job_Impl::runnable() const
  {
    QReadLocker l(&m_mutex);

    // Let's take care of the short circuiting cases
    if (m_externallyManaged) return false;
    if (m_force) return true;
    if (!m_runnable || m_canceled || isRunning()) return false;

    openstudio::UUID id = m_id;
    boost::optional<QDateTime> lastrun = lastRunInternal();
    std::shared_ptr<Job_Impl> p = m_parent.lock();

    l.unlock();

    // We are runnable if:
    //   status forced
    //   m_runnable and out of date and not canceled and not currently running
    //   and our parent is !outofdate, and if we are the finishedJob, all parent's
    //   children are not out of date. Also, none of those deps can be running themselves
    //

    
    bool parentRunning = false;
    bool parentChildrenRunning = false;
    bool parentFailed = false;
    bool parentChildrenFailed = false;
    bool parentOutOfDate = false;


    if (p)
    {
      bool amFinishedJob = false;

      if (!p->endTime()) return false; // parent has not yet finished, nip this in the bud

      parentRunning = p->isRunning();

      if (parentRunning) return false;

      parentFailed = p->errors().result == ruleset::OSResultValue::Fail;

      if (parentFailed) return false;

      parentOutOfDate = p->outOfDate();

      if (parentOutOfDate) return false;

      if (p->finishedJob())
      {
        amFinishedJob = (id == p->finishedJob()->uuid());
      }

      // We only care if the children are not out of date if we are the "finishedJob"
      if (amFinishedJob)
      {
        parentChildrenRunning = p->childrenTreesRunning();
        parentChildrenFailed = p->childrenTreesErrors().result == ruleset::OSResultValue::Fail;

    //    LOG(Debug, "amFinishedJob:  parentChildrenRunning " << parentChildrenRunning << " parentChildrenFailed " << parentChildrenFailed << " outofdateinternal " << outofdateinternal << " id " << openstudio::toString(id));

      }
    }

    bool outofdateinternal = outOfDateInternal(p, lastrun);

    // LOG(Debug, description() << " " << outofdateinternal << " " << parentOutOfDate << " " << parentRunning << " " << parentChildrenRunning << " " << parentRunning << " " << parentFailed << " " << parentChildrenFailed << " " << (lastrun?openstudio::toString(lastrun->toString()):"not previously run") )
    return outofdateinternal && !parentOutOfDate
               && !parentRunning && !parentChildrenRunning
               && !parentFailed && !parentChildrenFailed;

  }

  void Job_Impl::setErrors(const JobErrors &t_e)
  {
    QWriteLocker l(&m_mutex);
    m_errors = t_e;
  }

  void Job_Impl::emitStarted()
  {
    QWriteLocker l(&m_mutex);
    m_runnable = false;
    m_force = false;
    l.unlock();
    emit started(m_id);
    emit treeChanged(m_id);
  }

  Files Job_Impl::relativeOutputFiles() const
  {
    return Files(relativeOutputFilesInternal(outputFiles(), outdir()));
  }

  std::vector<FileInfo> Job_Impl::relativeOutputFilesInternal(const openstudio::runmanager::Files &t_outputFiles,
      const openstudio::path &t_outpath) const
  {
    std::vector<FileInfo> files = t_outputFiles.files();
    for (auto & file : files)
    {
      openstudio::path p = openstudio::relativePath(file.fullPath, t_outpath);
      if (!p.empty())
      {
        file.fullPath = p;
      }
    }

    return files;
  }

  void Job_Impl::emitFinished(const openstudio::runmanager::JobErrors &t_e, const boost::optional<QDateTime> &t_lastRun,
      const Files &t_outputFiles)
  {
    openstudio::path outpath = outdir();

    LOG(Debug,  "Emitting job finished: " << openstudio::toString(m_id));
    //m_runnable = true;
    if (t_lastRun)
    {
      openstudio::DateTime dt = openstudio::toDateTime(*t_lastRun);
      LOG(Debug, "Date converted: " << openstudio::toString(m_id) << " " << dt);

      emit finishedExt(m_id, t_e, dt, relativeOutputFilesInternal(t_outputFiles, outpath));
    }

    emit finished(m_id, t_e);
    emit treeChanged(m_id);
    LOG(Debug,  "Done Emitting job finished: " << openstudio::toString(m_id));
  }

  void Job_Impl::emitOutputFileChanged(const openstudio::runmanager::FileInfo& t_file)
  {
    emit outputFileChanged(m_id, t_file);
  }

  void Job_Impl::emitStateChanged() const
  {
    emit stateChanged(m_id);
    emit treeChanged(m_id);
  }

  void Job_Impl::emitOutputDataAdded(const std::string &t_data)
  {
    emit outputDataAdded(m_id, t_data);
  }

  std::string Job_Impl::detailedDescription() const
  {
    return "";
  }

  size_t Job_Impl::childIndexInList(const UUID &t_uuid) const
  {
    QReadLocker l(&m_mutex);
    for (size_t index = 0; index < m_children.size(); ++index)
    {
      if (m_children[index]->m_id == t_uuid)
      {
        return index;
      }
    }

    throw std::runtime_error("child not found in list: " + toString(m_id) + " " + toString(t_uuid));
  }

  bool Job_Impl::treeRunning() const
  {
    if (isRunning()) { return true; }

    QReadLocker l(&m_mutex);
    if (m_finishedJob && m_finishedJob->treeRunning()) { return true; }

    for (const auto & child : m_children)
    {
      if (child->treeRunning()) { return true; }
    }

    // I'm not running and neither is any of my children jobs
    return false;

  }

  void Job_Impl::setTreeRunnable(bool force)
  {
    setRunnable(force);

    for (std::vector<std::shared_ptr<Job_Impl> >::const_iterator itr = m_children.begin();
         itr != m_children.end();
         ++itr)
    {
      (*itr)->setTreeRunnable(force);
    }

    if (m_finishedJob)
    {
      m_finishedJob->setTreeRunnable(force);
    }
  }

  void Job_Impl::setTreeCanceled(bool t_canceled)
  {
    setCanceled(t_canceled);

    for (const auto & child : m_children)
    {
      child->setTreeCanceled(t_canceled);
    }

    if (m_finishedJob)
    {
      m_finishedJob->setTreeCanceled(t_canceled);
    }
  }



  std::string Job_Impl::buildDescription(const std::string &extension) const
  {
    std::string filename;

    openstudio::runmanager::Files files = inputFiles().getAllByExtension(extension);

    if (!files.files().empty())
    {
      openstudio::runmanager::FileInfo file = files.files().back();

      // If the filename is "in" look at the parent path for a more intelligent description
      if (toString(file.fullPath.stem()) == "in")
      {
        std::string parentpath = toString(file.fullPath.parent_path().filename());
        filename = toString(toPath(parentpath) / file.fullPath.filename());
      } else {
        filename = toString(file.fullPath.filename());
      }
    }

    if (filename.empty())
    {
      return m_jobType.valueDescription();
    } else {
      return m_jobType.valueDescription() + " (" + filename + ")";
    }
  }

  std::string Job_Impl::treeDescription() const
  {
    QReadLocker l(&m_mutex);

    std::string childdesc;

    for (const auto & child : m_children)
    {
      if (!childdesc.empty())
      {
        childdesc += ", ";
      }
      childdesc += child->treeDescription();
    }

    if (m_children.size() > 1)
    {
      childdesc = "(" + childdesc + ")";
    }

    if (!(m_children.empty()))
    {
      childdesc = "->" + childdesc;
    }

    if (m_finishedJob)
    {
      childdesc = "(" + childdesc + ")->" + m_finishedJob->treeDescription();
    }

    return description() + childdesc;
  }

  bool Job_Impl::treeOutOfDate() const
  {
    if (outOfDateInternal(parent(), lastRun())) { return true; }

    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      if (child->treeOutOfDate())
      {
        return true;
      }
    }

    if (m_finishedJob && m_finishedJob->treeOutOfDate()) { return true; }

    return false;
  }

  Files Job_Impl::treeAllFiles() const {
    Files files = inputFiles();
    files.append(outputFiles());
    QReadLocker l(&m_mutex);


    for (const auto & child : m_children)
    {
      files.append(child->treeAllFiles());
    }

    if (m_finishedJob)
    {
      files.append(m_finishedJob->treeAllFiles());
    }

    return files.complete(m_basePath);
  }

  Files Job_Impl::treeOutputFiles() const
  {
    Files files = outputFiles();
    QReadLocker l(&m_mutex);

    for (const auto & child : m_children)
    {
      files.append(child->treeOutputFiles());
    }

    if (m_finishedJob)
    {
      files.append(m_finishedJob->treeOutputFiles());
    }

    return files.complete(m_basePath);
  }


  JobErrors Job_Impl::treeErrors() const
  {
    boost::optional<JobErrors> temp;
    if (hasMergedJobs()) {
      for (const MergedJobResults& mergedJob : mergedJobResults()) {
        if (temp) {
          temp = temp.get() + mergedJob.errors;
        }
        else {
          temp = mergedJob.errors;
        }
      }
    }
    else {
      temp = errors();
    }

    // it's possible there are no mergedjobresults available yet, but this is a mergedjob
    if (!temp)
    {
      temp = errors();
    }

    JobErrors err = *temp;

    QReadLocker l(&m_mutex);


    for (const auto & child : m_children)
    {
      err = err + child->treeErrors();
    }

    if (m_finishedJob)
    {
      err = err + m_finishedJob->treeErrors();
    }

    return err;
  }

  TreeStatusEnum Job_Impl::compareTreeStatus(const TreeStatusEnum &lhs, const TreeStatusEnum &rhs)
  {
    if (lhs == TreeStatusEnum::Failed || rhs == TreeStatusEnum::Failed)
    {
      return TreeStatusEnum::Failed;
    } else if (lhs == TreeStatusEnum::Running || rhs == TreeStatusEnum::Running) {
      return TreeStatusEnum::Running;
    } else if (lhs == TreeStatusEnum::Canceled || rhs == TreeStatusEnum::Canceled) {
      return TreeStatusEnum::Canceled;
    } else if (lhs == TreeStatusEnum::Waiting || rhs == TreeStatusEnum::Waiting) {
      return TreeStatusEnum::Waiting;
    } else {
      return TreeStatusEnum::Finished;
    }
  }

  TreeStatusEnum Job_Impl::treeStatus() const
  {
    JobErrors err = errors();

    QReadLocker l(&m_mutex);
    TreeStatusEnum res;

    if (isRunning() && !isFinished())
    {
      res = TreeStatusEnum::Running;
    } else if (lastRunInternal()) {
      if (err.succeeded())
      {
        res = TreeStatusEnum::Finished;
      } else {
        res = TreeStatusEnum::Failed;
      }
    } else if (m_canceled) {
      res = TreeStatusEnum::Canceled;
    } else {
      res = TreeStatusEnum::Waiting;
    }

    for (const auto & child : m_children)
    {
      res = compareTreeStatus(res, child->treeStatus());
    }

    if (m_finishedJob)
    {
      res = compareTreeStatus(res, m_finishedJob->treeStatus());
    }

    return res;
  }

  std::vector<std::string> Job_Impl::treeDetailedDescription() const
  {
    std::vector<std::string> results;

    std::string desc = detailedDescription();

    if (!desc.empty()) { results.push_back(desc); }
    QReadLocker l(&m_mutex);

    for (const auto & child : m_children)
    {
      std::vector<std::string> childresults = child->treeDetailedDescription();
      results.insert(results.end(), childresults.begin(), childresults.end());
    }

    if (m_finishedJob)
    {
      std::vector<std::string> childresults = m_finishedJob->treeDetailedDescription();
      results.insert(results.end(), childresults.begin(), childresults.end());
    }

    return results;
  }


  boost::optional<QDateTime> Job_Impl::treeLastRun() const
  {
    QReadLocker l(&m_mutex);

    boost::optional<QDateTime> lastrun = lastRunInternal();

    if (!lastrun) { return boost::optional<QDateTime>(); }


    for (const auto & child : m_children)
    {
      boost::optional<QDateTime> childlastrun = child->treeLastRun();
      if (!childlastrun) { return boost::optional<QDateTime>(); }

      if (*childlastrun > *lastrun) { lastrun = childlastrun; }
    }

    if (m_finishedJob)
    {
      boost::optional<QDateTime> childlastrun = m_finishedJob->treeLastRun();
      if (!childlastrun) { return boost::optional<QDateTime>(); }

      if (*childlastrun > *lastrun) { lastrun = childlastrun; }
    }

    return lastrun;
  }

  void Job_Impl::setBasePath(const openstudio::path &t_basePath)
  {
    QWriteLocker l(&m_mutex);

    if (m_basePath != t_basePath)
    {
      m_basePath = t_basePath;
      m_outdir = boost::none;
      basePathChanged();
    }
  }

  void Job_Impl::setBasePathRecursive(const openstudio::path &t_basePath)
  {
    setBasePath(t_basePath);

    QReadLocker l(&m_mutex);
    for (const auto & child : m_children)
    {
      child->setBasePathRecursive(t_basePath);
    }

    if (m_finishedJob)
    {
      m_finishedJob->setBasePathRecursive(t_basePath);
    }
  }

  void Job_Impl::addFile(std::map<openstudio::path, FileTrack> &t_files, const openstudio::path &t_path)
  {
    t_files[t_path] = FileTrack(t_path);
  }

  bool Job_Impl::filesChanged(const std::map<openstudio::path, FileTrack> &t_files, const QDateTime &t_lastRun)
  {
    for (const auto & file : t_files)
    {
      QFileInfo qfi(openstudio::toQString(file.first));
      if (qfi.lastModified() >= t_lastRun)
      {
        std::string checksum = openstudio::checksum(file.first);
        if (checksum != file.second.checksum)
        {
          return true;
        }
      }
    }

    return false;
  }

  void Job_Impl::resetFiles(std::map<openstudio::path, FileTrack> &t_files)
  {
    t_files.clear();
  }

  void Job_Impl::resetFiles(std::map<openstudio::path, FileTrack> &t_files, const boost::optional<FileInfo> &t_file)
  {
    t_files.clear();

    if (t_file)
    {
      addFile(t_files, t_file->fullPath);
    }
  }

  void Job_Impl::resetFiles(std::map<openstudio::path, FileTrack> &t_files, const boost::optional<FileInfo> &t_file, const boost::optional<FileInfo> &t_file2)
  {
    if (t_file)
    {
      addFile(t_files, t_file->fullPath);
    }

    if (t_file2)
    {
      addFile(t_files, t_file2->fullPath);
    }
  }


  void Job_Impl::updateJob(const std::shared_ptr<Job_Impl> &t_other, bool t_allowUUIDUpdate)
  {

    if (t_other.get() == this)
    {
      LOG(Info, "Updating job is current job, nothing to do: " << toString(uuid().toString())); 
      return;
    } else {
      LOG(Info, "Updating job: " << toString(uuid().toString()));
    }

    QWriteLocker l(&m_mutex);

    if (!m_externallyManaged
        || !t_other->m_externallyManaged)
    {
      throw std::runtime_error("Job updating is only valid for externallyManaged jobs");
    }

    openstudio::UUID oldUUID = m_id;
    openstudio::UUID newUUID = t_other->uuid();

    if (!t_allowUUIDUpdate && oldUUID != newUUID)
    {
      throw std::runtime_error("Jobs UUIDs do not match, unable to updateJob");
    } 

    if (t_allowUUIDUpdate && m_jobType != t_other->jobType())
    {
      throw std::runtime_error("Jobs types do not match, unable to updateJob");
    }

    std::vector<std::shared_ptr<Job_Impl> > myChildren = m_children;
    std::vector<std::shared_ptr<Job_Impl> > otherChildren = t_other->children();

    if (!t_allowUUIDUpdate)
    {
      std::set<openstudio::UUID> myChildrenIds;

      for (std::vector<std::shared_ptr<Job_Impl> >::const_iterator itr = myChildren.begin();
          itr != myChildren.end();
          ++itr)
      {
        myChildrenIds.insert((*itr)->uuid());
      }

      std::set<openstudio::UUID> otherChildrenIds;

      for (std::vector<std::shared_ptr<Job_Impl> >::const_iterator itr = otherChildren.begin();
          itr != otherChildren.end();
          ++itr)
      {
        otherChildrenIds.insert((*itr)->uuid());
      }

      if (myChildrenIds != otherChildrenIds)
      {
        throw std::runtime_error("Job children UUIDs do not match, unable to updateJob");
      }

    } else {
      size_t mynumChildren = myChildren.size();
      size_t othernumChildren = otherChildren.size();

      if (mynumChildren != othernumChildren || mynumChildren > 1)
      {
        throw std::runtime_error("Unable to update UUIDs with more than one child job or mismatched child sizes");
      }

      if (mynumChildren == 1 && othernumChildren == 1)
      {
        if (myChildren[0]->jobType() != otherChildren[0]->jobType())
        {
          throw std::runtime_error("Unable to update UUIDs with mismatched child job types");
        }
      }
    }

    std::shared_ptr<Job_Impl> myFinishedJob = m_finishedJob;
  bool myHasFinishedJob = (myFinishedJob ? true : false);

    std::shared_ptr<Job_Impl> otherFinishedJob = t_other->finishedJob();
  bool otherHasFinishedJob = (otherFinishedJob ? true : false);

    if (myHasFinishedJob != otherHasFinishedJob)
    {
      throw std::runtime_error("Finished jobs mismatch, unable to updateJob");
    }

    if (myHasFinishedJob)
    {
      if (!t_allowUUIDUpdate)
      {
        if (myFinishedJob->uuid() != otherFinishedJob->uuid())
        {
          throw std::runtime_error("Finished jobs UUIDs do not match, unable to updateJob");
        }
      } else {
        if (myFinishedJob->jobType() != otherFinishedJob->jobType())
        {
          throw std::runtime_error("Finished jobs types do not match, unable to updateJob");
        }
      }
    }



    JobParams newparams = t_other->m_params;
    Tools newtools = t_other->m_tools;
    Files newfiles = t_other->m_inputFiles;

    JobState oldState = m_jobState;
    JobState newState = t_other->m_jobState;

    l.unlock();


    if (t_allowUUIDUpdate)
    {
      if (myChildren.size() == 1)
      {
        myChildren[0]->updateJob(otherChildren[0], true);
      }
    } else {
      // update children first, to make sure they can be updated
      for (const auto & child : myChildren)
      {
        for (const auto & otherChild : otherChildren)
        {
          if (child->uuid() == otherChild->uuid())
          {
            child->updateJob(otherChild, t_allowUUIDUpdate);
            break;
          }
        }
      }
    }

    if (myHasFinishedJob)
    {
      myFinishedJob->updateJob(otherFinishedJob, t_allowUUIDUpdate);
    }


    // then update myself and send signals
    l.relock();
    m_jobState = newState;
    m_inputFiles = newfiles;
    m_tools = newtools;
    m_params = newparams;
    m_id = newUUID;
    m_outdir = boost::none;
    m_allTools = boost::none;
    m_allParams = boost::none;
    m_allInputFiles = boost::none;
    l.unlock();

    sendSignals(oldState, newState, oldUUID, newUUID);
  }

  void Job_Impl::setStatus(const AdvancedStatus &t_status)
  {
    if (!externallyManaged())
    {
      throw std::runtime_error("Unable to set status on non-externally managed job");
    } else {
      emitStatusChanged(t_status);
    }
  }

  void Job_Impl::sendSignals()
  {
    QReadLocker l(&m_mutex);
    JobState state = m_jobState;
    openstudio::UUID id = m_id;
    l.unlock();

    sendSignals(JobState(), state, id, id);
  }

  void Job_Impl::sendSignals(JobState oldState, JobState newState, const openstudio::UUID &t_oldUUID, const openstudio::UUID &t_newUUID)
  {
    // always send the status
    bool sendStatus = true;
    bool sendFinished = false;
    bool sendStarted = false;

    if (!oldState.lastRun && newState.lastRun)
    {
      sendFinished = true;
      sendStarted = true;
    }

    if (oldState.status.value() == AdvancedStatusEnum::Idle 
        && newState.status.value() != AdvancedStatusEnum::Idle)
    {
      sendStarted = true;
    }

    bool uuidchanged = t_oldUUID != t_newUUID;

    std::vector<FileInfo> diffFiles;
    std::set<FileInfo> oldFiles(oldState.outputFiles.files().begin(), oldState.outputFiles.files().end());
    std::set<FileInfo> newFiles(newState.outputFiles.files().begin(), newState.outputFiles.files().end());
    
    std::set_symmetric_difference(oldFiles.begin(), oldFiles.end(),
                        newFiles.begin(), newFiles.end(),
                        std::back_inserter(diffFiles));

    if (uuidchanged) {
      LOG(Debug, "updateJob: uuidChanged()");
      emit uuidChanged(t_oldUUID, t_newUUID);
    }

    if (sendStarted)
    {
      LOG(Debug, "updateJob: emitStarted()");
      emitStarted();
    }

    if (sendStatus)
    {
      LOG(Debug, "updateJob: emitStatusChanged()");
      emitStatusChanged(newState.status);
    }

    LOG(Debug, "updateJob: emitOutputFileChanged()");
    std::for_each(diffFiles.begin(), diffFiles.end(), std::bind(&Job_Impl::emitOutputFileChanged, this, std::placeholders::_1));

    if (sendFinished)
    {
      LOG(Debug, "updateJob: emitFinished()");
      emitFinished(newState.errors, newState.lastRun, Files(std::vector<FileInfo>(newFiles.begin(), newFiles.end())));
    }

  }


  bool Job_Impl::externallyManaged() const
  {
    QReadLocker l(&m_mutex);
    return m_externallyManaged;
  }

  /// sets this job (and children) as being externally managed
  void Job_Impl::makeExternallyManaged()
  {

    QWriteLocker l(&m_mutex);
    m_externallyManaged = true;
    if (!m_params.has("jobExternallyManaged"))
    {
      m_params.append("jobExternallyManaged", "true");
    }

    for (const auto & child : m_children)
    {
      child->makeExternallyManaged();
    }

    if (m_finishedJob)
    {
      m_finishedJob->makeExternallyManaged();
    }
  }

  bool Job_Impl::hasMergedJobs() const
  {
    return hasMergedJobsImpl();
  }

  std::vector<MergedJobResults> Job_Impl::mergedJobResults() const 
  {
    return mergedJobResultsImpl();
  }

  bool Job_Impl::hasMergedJobsImpl() const
  {
    return false;
  }

  std::vector<MergedJobResults> Job_Impl::mergedJobResultsImpl() const
  {
    return std::vector<MergedJobResults>();
  }


}
}
}

