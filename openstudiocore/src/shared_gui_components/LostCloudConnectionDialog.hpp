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

#ifndef SHAREDGUICOMPONENTS_LOSTCLOUDCONNECTIONDIALOG_HPP
#define SHAREDGUICOMPONENTS_LOSTCLOUDCONNECTIONDIALOG_HPP

#include "OSDialog.hpp"

class QVBoxLayout;

namespace openstudio{

class LostCloudConnectionDialog : public OSDialog
{
  Q_OBJECT

public:

  LostCloudConnectionDialog(bool internetAvailable,
    bool authenticated,
    bool cloudRunning,
    QWidget * parent = nullptr);

  virtual ~LostCloudConnectionDialog();

  bool clearCloudSession() {return m_clearCloudSession;}

protected slots:

  //virtual void on_okButton(bool checked);

private:

  void createWidgets(bool internetAvailable,
    bool authenticated,
    bool cloudRunning);

  void createCloudConnectionWidgets(QVBoxLayout * vLayout);

  bool m_clearCloudSession;

private slots:

  void on_launchAWSConsole(bool checked);

  void on_clearCloudSession(bool checked);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_LOSTCLOUDCONNECTIONDIALOG_HPP
