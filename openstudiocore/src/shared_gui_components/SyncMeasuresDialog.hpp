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

#ifndef SYNCMEASURESDIALOG_H
#define SYNCMEASURESDIALOG_H

#include <QDialog>

#include <utilities/bcl/BCLMeasure.hpp>

#include <vector>

class QScrollArea;
class QWidget;

namespace openstudio {

class Component;
class MeasureManager;
class SyncMeasuresDialogCentralWidget;

namespace analysisdriver {

class SimpleProject;

}
class SyncMeasuresDialog : public QDialog
{
  Q_OBJECT

public:
  SyncMeasuresDialog(analysisdriver::SimpleProject * project,
    MeasureManager * measureManager,
    QWidget * parent = 0);
  virtual ~SyncMeasuresDialog() {}

protected:
  void paintEvent(QPaintEvent * event);

private:
  void createLayout();
  void findUpdates();

  SyncMeasuresDialogCentralWidget * m_centralWidget;
  QScrollArea * m_rightScrollArea;
  Component * m_expandedComponent;
  std::vector<BCLMeasure> m_measuresNeedingUpdates;
  analysisdriver::SimpleProject * m_project; // DLM: why is this a raw pointer?
  MeasureManager * m_measureManager; // DLM: why is this a raw pointer?

private slots:
  void on_componentClicked(bool checked);
  void on_noComponents();
  void closeDlg();

};

} // namespace openstudio

#endif // SYNCMEASURESDIALOG_H
