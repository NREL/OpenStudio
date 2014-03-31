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

#ifndef OPENSTUDIO_RUNMANAGER_ADDTOOL_HPP__
#define OPENSTUDIO_RUNMANAGER_ADDTOOL_HPP__

#include <string>
#include <QTimer>
#include <boost/optional.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/ui_AddTool.h>
#include <runmanager/lib/Job.hpp>


namespace openstudio {
namespace runmanager {

  //! QDialog based user interface for setting RunManager configuration options. 
  //! \sa http://doc.qt.nokia.com/qdialog.html
  class AddTool : public QDialog
  {
    Q_OBJECT

  public:
    //! Create a AddTool dialog for setting ConfigOptions
    AddTool(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~AddTool();

    //! Returns the tool created during execution of the dialog
    boost::optional<std::pair<ToolVersion, ToolLocationInfo> > getTool();


  private:
    //! Used for QDesigner classes.
    Ui::AddToolDialog ui;

  private slots:
    void localBrowse();
    void remoteBrowse();
  };


} // runmanager namespace
} // openstudio namespace


#endif // OPENSTUDIO_RUNMANAGER_ADDTOOL_HPP_
