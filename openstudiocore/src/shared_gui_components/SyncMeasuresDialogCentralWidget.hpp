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

#include <set>
#include <vector>

#include <boost/optional.hpp>

class QProgressBar;

namespace openstudio {

class BCLComponent;
class BCLMeasure;
class Component;
class ComponentList;
class CollapsibleComponentList;

class SyncMeasuresDialogCentralWidget : public QWidget
{
  Q_OBJECT

public:
  SyncMeasuresDialogCentralWidget(QWidget * parent = 0);
  virtual ~SyncMeasuresDialogCentralWidget() {}

  Component * checkedComponent() const;
  int pageIdx();
  bool showNewComponents();
  void setShowNewComponents(bool showNewComponents);
  void setMeasures(std::vector<BCLMeasure> & measures);

protected:

private:
  void createLayout();
  void init();

  CollapsibleComponentList * m_collapsibleComponentList;
  ComponentList * m_componentList; // TODO cruft to be removed
  QProgressBar * m_progressBar;
  std::set<std::string> m_pendingDownloads;
  std::string m_filterType;
  int m_pageIdx;
  QString m_searchString;
  bool m_showNewComponents;

signals:
  void headerClicked(bool checked);
  void componentClicked(bool checked);
  void collapsibleComponentClicked(bool checked);
  void getComponentsByPage(int pageNum);
  void componentsReady();
  void noComponents();
  void requestComponents(const std::string& filterType, int tids, int pageIdx, const QString & searchString);

private slots:
  void upperPushButtonClicked();
  void lowerPushButtonClicked();
  void comboBoxIndexChanged(const QString & text);
  void on_headerClicked(bool checked);
  void componentDownloadComplete(const std::string& uid, const boost::optional<BCLComponent>& component);
  void measureDownloadComplete(const std::string& uid, const boost::optional<BCLMeasure>& measure);
  void on_componentClicked(bool checked);
  void on_collapsibleComponentClicked(bool checked);
  void on_getComponentsByPage(int pageIdx);

};

} // namespace openstudio

#endif // SYNCMEASURESDIALOGCENTRALWIDGET_H
