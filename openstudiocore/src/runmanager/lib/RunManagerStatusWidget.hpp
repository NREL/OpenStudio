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

#ifndef RUNMANAGER_LIB_RUNMANAGERSTATUSWIDGET_HPP
#define RUNMANAGER_LIB_RUNMANAGERSTATUSWIDGET_HPP

#include <string>
#include <QTimer>
#include "RunManager.hpp"
#include <runmanager/lib/ui_RunManagerStatusWidget.h>
#include "JobErrors.hpp"
#include "RunManagerAPI.hpp"
#include <QStandardItem>

namespace openstudio {
  namespace runmanager {

    //! QDialog based user interface for displaying RunManager queue job status to the user
    //! \sa http://doc.qt.nokia.com/qdialog.html
    class RUNMANAGER_API RunManagerStatusWidget : public QWidget
    {
      Q_OBJECT

      public:
        //! Creates a RunManagerStatusWidget dialog for displaying job status
        RunManagerStatusWidget(QWidget *parent = nullptr);
        virtual ~RunManagerStatusWidget();


        /// Sets the runmanager to monitor the status of
        void setRunManager(const RunManager &t_runmanager);

        /// Enables or disables the "goToOutput" menu option.
        void setGoToOutputEnabled(bool enabled);

        /// Returns a vector of QAction objects to display on a toolbar or some other easily accessible location
        std::vector<std::pair<bool, QAction *> > publicActions();

      signals:
        void jobSelectionCleared();
        void jobSelectionChanged(const openstudio::runmanager::Job &job);
        void goToOutput();


      private slots:
        void raiseJobPriority();
        void lowerJobPriority();
        void cancelResumeJob();
        void rerunJob();
        void showJobWarnings();
        void cleanupJobOutput();
        void deleteJob();
        void pausedToggled(bool t_paused);

        void rowInserted();
        void jobTreeSelectionChanged(const QItemSelection &t_selection);
        void completedJobTreeSelectionChanged(const QItemSelection &t_selection);

        void resizeCols();
        void openOutputFolder();
        void emitGoToOutput();
        void itemChanged(QStandardItem *i);
        void updateActions(const QModelIndexList &selection);
        void clearActions();
 
      private:
        REGISTER_LOGGER("openstudio::runmanager::RunManagerStatusWidget");
        //! Used for QDesigner classes.
        Ui::RunManagerStatusWidget ui;

        boost::optional<RunManager> m_runmanager;

        void openFile(const openstudio::path &p);

        QModelIndex getCurrentIndex();
        QModelIndexList getSelectedRows(); 

    };


} // runmanager namespace
} // openstudio namespace


#endif // RUNMANAGER_LIB_RUNMANAGERSTATUSWIDGET_HPP
