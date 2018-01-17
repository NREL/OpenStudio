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

#ifndef RUNMANAGER_LIB_JOBSTATUSWIDGET_HPP
#define RUNMANAGER_LIB_JOBSTATUSWIDGET_HPP

#include <string>
#include <QTimer>
#include "RunManager.hpp"
#include <runmanager/lib/ui_JobStatusWidget.h>
#include "JobErrors.hpp"
#include "Job.hpp"
#include "RunManagerAPI.hpp"
#include <QStandardItemModel>


namespace openstudio {
  namespace runmanager {
 

    namespace detail {

      /// JobItem used for providing data about Jobs through the 
      /// QStandardItemModel interface
      /// Uses signals and slots to track the state of the Job and
      /// update the model as changeds occur.
      class JobItem : public QObject, public QStandardItem
      {
        Q_OBJECT;

        public:
          JobItem(const Job &, int col);
          virtual ~JobItem() {}

          Job getJob();

          static const int numColumns = 5;
          static QStringList columnHeaders();

          virtual bool operator<(const QStandardItem &qsi) const override;

        private slots:
          void jobOutputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo &t_fi);
          void stateChanged();
          void statusChanged(const openstudio::runmanager::AdvancedStatus &t_status);

        private:
          void updateOutputFiles();
          void updateDescription();
          void updateOutputFile(const openstudio::runmanager::FileInfo &t_fi);

          Job m_job;
          int m_col;
      };

      /// Represents an output file from an executing RunManager job in the 
      /// QAbstractItemModel. It is created by JobItem objects
      class FileItem : public QStandardItem
      {
        public: 
          FileItem(const FileInfo &);
          virtual ~FileItem() {}


          void setFileInfo(const FileInfo &t_fi);
          FileInfo fileInfo();
          void updateDisplay();

        private:
          FileInfo m_info;
      };

    } // detail

    //! QDialog based user interface for displaying RunManager queue job status to the user
    //! \sa http://doc.qt.nokia.com/qdialog.html
    class RUNMANAGER_API JobStatusWidget : public QWidget
    {
      Q_OBJECT

      public:
        //! Create a JobStatusWidget dialog for displaying job status
        JobStatusWidget(openstudio::runmanager::RunManager t_runmanager, QWidget *parent = nullptr);
        virtual ~JobStatusWidget();

        /// Returns a vector of QAction objects to display on a toolbar or some other easily accessible location
        std::vector<std::pair<bool, QAction *> > publicActions();

      public slots:
        /// Clears the currently displayed workflow
        void clearWorkflow();

        /// Sets the workflow to display
        void setWorkflow(const openstudio::runmanager::Job &t_j);

      private slots:
        /// cancel / resume the currently selected jobs
        void cancelResumeJob();

        /// rerun the currently selected jobs
        void rerunJob();

        /// Show the warnings and errors list for the currently selected job.
        void showJobWarnings();

        /// Show the "cleanup output" dialog for the currently selected job
        void cleanupJobOutput();

        /// Open the currently selected file with the operating system defined handler
        void openEditFile();

        /// Open the currently selected directory with the operating system shell
        void openDirectory();

        void updateDetails();
        void jobTreeSelectionChanged(const QItemSelection &t_selection);
        void workflowTreeSelectionChanged(const QItemSelection &t_selection);
        void outputDataAdded(const openstudio::UUID &t_jobid, const std::string &t_data);

        void jobSelectionCleared();
        void jobSelectionChanged(const openstudio::runmanager::Job &t_j);

        void itemActivated(const QModelIndex &t_index);
        void resizeCols();
        void rowsInserted(const QModelIndex &i, int start, int end);
        
        void runmanagerStatsChanged();

      private:
        REGISTER_LOGGER("openstudio::runmanager::JobStatusWidget");

        openstudio::runmanager::RunManager m_runmanager;

        //! Used for QDesigner classes.
        Ui::JobStatusWidget ui;

        QStandardItemModel m_model;
        boost::optional<openstudio::runmanager::Job> m_job;

        static bool jobIndexLessThan(const Job& lhs, const Job &rhs);

        void addJob(openstudio::runmanager::Job job);
        boost::optional<openstudio::runmanager::Job> m_outputjob;

        void openFile(const openstudio::path &p);

        std::vector<openstudio::runmanager::Job> getJobs(const QModelIndexList &t_indexes) const;
        openstudio::runmanager::Job getJob(const QModelIndex &t_index) const;

        /// \returns a vector of all files contained in the QModelIndexList provided
        std::vector<openstudio::path> getFiles(const QModelIndexList &t_indexes) const;
        /// \returns Outputfile matching t_index or throws if not found
        openstudio::path getFile(const QModelIndex &t_index) const;

        detail::JobItem *getJobItem(const Job &t_job) const;

        detail::JobItem *getJobItemImpl(const Job &t_job, const QModelIndex &parent) const;
    };


} // runmanager namespace
} // openstudio namespace


#endif // RUNMANAGER_LIB_JOBSTATUSWIDGET_HPP
