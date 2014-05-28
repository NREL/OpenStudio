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

#ifndef RUNMANAGER_LIB_JOBOUTPUTCLEANUP_HPP
#define RUNMANAGER_LIB_JOBOUTPUTCLEANUP_HPP

#include <string>
#include <QTimer>
#include "RunManager.hpp"
#include <ui_JobOutputCleanup.h>


namespace openstudio {
namespace runmanager {

  //! QDialog based user interface for allowing the use to select a set of files to clean
  //! up (delete) from a job run.
  //! \sa http://doc.qt.nokia.com/qdialog.html
  class JobOutputCleanup : public QDialog
  {
    Q_OBJECT

  public:
    /** Create a JobOutputCleanup dialog for choosing files to remove */
    JobOutputCleanup(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr, 
        const std::vector<std::string> &files = std::vector<std::string>());

    virtual ~JobOutputCleanup();

    //! Returns the list of files selected by the user
    //! This return value should only be used if the JobOutputCleanup exec() or
    //! result() methods return QDialog::Accepted - indicating that the user
    //! did not cancel out of the dialog.
    //! \return The ConfigOptions object as it was modified by the user
    std::vector<std::string> getSelectedFiles();

  private:
    //! Used for QDesigner classes.
    Ui::JobOutputCleanup ui;


  };


} // runmanager namespace
} // openstudio namespace


#endif // RUNMANAGER_LIB_JOBOUTPUTCLEANUP_HPP
