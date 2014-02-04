#ifndef OPENSTUDIO_RUNMANAGER_WATCHER_HPP
#define OPENSTUDIO_RUNMANAGER_WATCHER_HPP

#include <vector>
#include <string>
#include <QObject>
#include <utilities/core/UUID.hpp>
#include "RunManager.hpp"

namespace openstudio {
  class DateTime;

namespace runmanager {
  class JobErrors;
  class FileInfo;
  class AdvancedStatus;
  class Job;

  class RunManagerWatcher : public QObject
  {
    Q_OBJECT;

    public:
      RunManagerWatcher(const RunManager &t_runManager);

    protected:
      virtual void jobStarted(const openstudio::UUID &t_id) = 0;
      virtual void jobFinished(const openstudio::UUID &id, const openstudio::runmanager::JobErrors& errors) = 0;
      virtual void jobFinishedExt(const openstudio::UUID &id, const openstudio::runmanager::JobErrors& errors, 
          const openstudio::DateTime &lastRun, const std::vector<openstudio::runmanager::FileInfo> &outputfiles) = 0;
      virtual void jobOutputFileChanged(const openstudio::UUID &id, const openstudio::runmanager::FileInfo& file) = 0;
      virtual void jobStateChanged(const openstudio::UUID &id) = 0;
      virtual void jobOutputDataAdded(const openstudio::UUID &id, const std::string &data) = 0;
      virtual void jobStatusChanged(const openstudio::runmanager::AdvancedStatus &status) = 0;
      virtual void jobChildrenChanged(const openstudio::UUID &id) = 0;
      virtual void jobParentChanged(const openstudio::UUID &id) = 0;
      virtual void jobTreeChanged(const openstudio::UUID &id) = 0;
      virtual void jobUuidChanged(const openstudio::UUID &t_oldUUID, const openstudio::UUID &t_newUUID) = 0;

      virtual void runManagerPausedChanged(bool) = 0;
      virtual void runManagerStatsChanged() = 0;
      virtual void runManagerJobTreeAdded(const openstudio::runmanager::Job &t_job) = 0;

      Job getJob(const openstudio::UUID &t_uuid);

    private:
      void hookUpSignals(const openstudio::runmanager::Job &t_job);
      void runManagerJobTreeAddedInternal(const openstudio::runmanager::Job &t_job);

      RunManager m_runManager;

    private slots:
      /// Emitted after the job has started
      void started(const openstudio::UUID &id);

      /// Emitted after the job has finished
      void finished(const openstudio::UUID &id, const openstudio::runmanager::JobErrors& errors);

      /// Emitted after the job has finished
      void finishedExt(const openstudio::UUID &id, const openstudio::runmanager::JobErrors& errors, 
          const openstudio::DateTime &lastRun, const std::vector<openstudio::runmanager::FileInfo> &outputfiles);

      /// Emitted after generated output file has changed.
      void outputFileChanged(const openstudio::UUID &id, const openstudio::runmanager::FileInfo& file);

      /// Emitted after the state (runnable/canceled/outofdate) or index of a job has changed
      void stateChanged(const openstudio::UUID &id);

      /// Emitted after any data has been sent to stdout from the job, does not wait for
      /// complete lines
      void outputDataAdded(const openstudio::UUID &id, const std::string &data);

      /// Emitted after new detailed status information becomes available
      void statusChanged(const openstudio::runmanager::AdvancedStatus &status);

      /// Emitted after the list of children or the finished job child changes
      void childrenChanged(const openstudio::UUID &id);

      /// Emitted after the the parent of this job changes
      void parentChanged(const openstudio::UUID &id);

      /// Emitted whenever this state of this job tree (this job and any child) changes
      void treeChanged(const openstudio::UUID &id);


      /// Emitted when a Job's UUID has changed via a updateJob() call
      void uuidChanged(const openstudio::UUID &t_oldUUID, const openstudio::UUID &t_newUUID);


      /// Emitted when the paused state has change
      void pausedChanged(bool);

      /// Stats changed
      void statsChanged();

      /// Job tree added
      void jobTreeAdded(const openstudio::runmanager::Job &t_job);
  };

}
}

#endif
