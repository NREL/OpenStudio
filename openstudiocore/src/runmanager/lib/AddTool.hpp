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

#ifndef RUNMANAGER_LIB_ADDTOOL_HPP
#define RUNMANAGER_LIB_ADDTOOL_HPP

#include <string>
#include <QTimer>
#include <boost/optional.hpp>
#include "RunManager.hpp"
#include <runmanager/lib/ui_AddTool.h>
#include "Job.hpp"


namespace openstudio {
namespace runmanager {

  //! QDialog based user interface for setting RunManager configuration options. 
  //! \sa http://doc.qt.nokia.com/qdialog.html
  class AddTool : public QDialog
  {
    Q_OBJECT

  public:
    //! Create a AddTool dialog for setting ConfigOptions
    AddTool(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr);
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


#endif // RUNMANAGER_LIB_ADDTOOL_HPP
