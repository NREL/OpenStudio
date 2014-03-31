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

#ifndef OPENSTUDIO_RUNMANAGER_JOBERRORDISPLAY_HPP__
#define OPENSTUDIO_RUNMANAGER_JOBERRORDISPLAY_HPP__

#include <string>
#include <QTimer>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/ui_JobErrorsDisplay.h>
#include "JobErrors.hpp"


namespace openstudio {
namespace runmanager {

  //! QDialog based user interface for setting RunManager configuration options. 
  //! \sa http://doc.qt.nokia.com/qdialog.html
  class JobErrorsDisplay : public QDialog
  {
    Q_OBJECT

  public:
    /** Create a JobErrorsDisplay dialog for displaying job warning / error output */
    JobErrorsDisplay(QWidget *parent = 0, Qt::WindowFlags flags = 0, const JobErrors &errors = JobErrors());

    virtual ~JobErrorsDisplay();

  private:
    //! Used for QDesigner classes.
    Ui::JobErrorsDisplay ui;
  };


} // runmanager namespace
} // openstudio namespace


#endif // OPENSTUDIO_RUNMANAGER_JOBERRORDISPLAY_HPP_
