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

#ifndef OPENSTUDIO_WAITDIALOG_H
#define OPENSTUDIO_WAITDIALOG_H

#include <shared_gui_components/OSDialog.hpp>

namespace openstudio{

class WaitDialog : public OSDialog
{
  Q_OBJECT

public:

  WaitDialog(const QString & windowTitle,
    const QString & windowMessage,
    QWidget * parent = 0);

  virtual ~WaitDialog();

  QSize sizeHint() const;

protected slots:

  virtual void on_cancelButton(bool checked);

protected:

  void closeEvent(QCloseEvent * event);

private:

  virtual void createWidgets();

  QString m_windowMessage;

};

} // openstudio

#endif // OPENSTUDIO_WAITDIALOG_H
