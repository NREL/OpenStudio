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

#ifndef OPENSTUDIO_APPLYMEASURENOWDIALOG_H
#define OPENSTUDIO_APPLYMEASURENOWDIALOG_H

#include <shared_gui_components/OSDialog.hpp>

class QStackedWidget;
class QTextEdit;
class QTimer;

namespace openstudio{

class EditController;
class LocalLibraryController;

class ApplyMeasureNowDialog : public OSDialog
{
  Q_OBJECT

public:

  ApplyMeasureNowDialog(QWidget * parent = 0);

  virtual ~ApplyMeasureNowDialog();

  QSize sizeHint() const;
  QSharedPointer<EditController> m_editController;

protected slots:

  virtual void on_cancelButton(bool checked);

  virtual void on_okButton(bool checked);


protected:

  void closeEvent(QCloseEvent * event);

private:

  void createWidgets();

  QSharedPointer<LocalLibraryController> m_localLibraryController;

  QStackedWidget * m_mainPaneStackedWidget;

  QStackedWidget * m_rightPaneStackedWidget;

  QTextEdit * m_argumentsFailedTextEdit;

  QTextEdit * m_outputWindow;

  QTimer * m_timer;

  int m_inputPageIdx;

  int m_runningPageIdx;

  int m_outputPageIdx;

  int m_argumentsFailedPageIdx;

  int m_argumentsOkPageIdx;

};

} // openstudio

#endif // OPENSTUDIO_APPLYMEASURENOWDIALOG_H
