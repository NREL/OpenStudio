/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef SHAREDGUICOMPONENTS_WAITDIALOG_HPP
#define SHAREDGUICOMPONENTS_WAITDIALOG_HPP

#include "OSDialog.hpp"

namespace openstudio{

class WaitDialog : public OSDialog
{
  Q_OBJECT

public:

  WaitDialog(const QString & windowTitle,
    const QString & windowMessage,
    QWidget * parent = nullptr);

  virtual ~WaitDialog();

  QSize sizeHint() const override;

protected slots:

  virtual void on_cancelButton(bool checked) override;

protected:

  void closeEvent(QCloseEvent * event) override;

private:

  virtual void createWidgets();

  QString m_windowMessage;

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_WAITDIALOG_HPP
