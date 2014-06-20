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

#ifndef SYNCMEASURESDIALOGCENTRALWIDGET_H
#define SYNCMEASURESDIALOGCENTRALWIDGET_H

#include <QWidget>

#include <utilities/bcl/BCLMeasure.hpp>

class QPushButton;

namespace openstudio {

class BCLComponent;
class BCLMeasure;
class Component;
class ComponentList;
class CollapsibleComponentList;
class MeasureManager;

namespace analysisdriver {

class SimpleProject;

}

class SyncMeasuresDialogCentralWidget : public QWidget
{
  Q_OBJECT

public:
  SyncMeasuresDialogCentralWidget(analysisdriver::SimpleProject * project,
    MeasureManager * measureManager,
    QWidget * parent = 0);
  virtual ~SyncMeasuresDialogCentralWidget() {}

  Component * checkedComponent() const;
  void setMeasures(const std::vector<BCLMeasure> & measures);
  QPushButton * lowerPushButton;

protected:

private:
  void createLayout();
  void init();
  void displayMeasures(int pageIdx);

  CollapsibleComponentList * m_collapsibleComponentList;
  ComponentList * m_componentList;
  int m_pageIdx;
  std::vector<BCLMeasure> m_measures;
  analysisdriver::SimpleProject * m_project;
  MeasureManager * m_measureManager;

signals:
  void componentClicked(bool checked);
  void getComponentsByPage(int pageNum);
  void noComponents();
  void closeDlg();

private slots:
  void upperPushButtonClicked();
  void lowerPushButtonClicked();
  void on_getComponentsByPage(int pageIdx);

};

} // namespace openstudio

#endif // SYNCMEASURESDIALOGCENTRALWIDGET_H
