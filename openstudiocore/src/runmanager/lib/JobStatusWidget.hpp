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

          virtual bool operator<(const QStandardItem &qsi) const;

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
