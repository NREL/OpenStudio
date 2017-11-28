/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef SHAREDGUICOMPONENTS_BUILDINGCOMPONENTDIALOGCENTRALWIDGET_HPP
#define SHAREDGUICOMPONENTS_BUILDINGCOMPONENTDIALOGCENTRALWIDGET_HPP

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

class BuildingComponentDialogCentralWidget : public QWidget
{
  Q_OBJECT

public:
  BuildingComponentDialogCentralWidget(QWidget * parent = nullptr);
  BuildingComponentDialogCentralWidget(int tid, QWidget * parent = nullptr);
  virtual ~BuildingComponentDialogCentralWidget() {}
  int tid();
  void setTid(const std::string& filterType,
    int tids,
    int pageIdx,
    const QString & title,
    const QString & string = "");
  Component * checkedComponent() const;
  int pageIdx();
  bool showNewComponents();
  void setShowNewComponents(bool showNewComponents);

protected:

private:
  void createLayout();
  void init();
  void setTid();

  int m_tid;
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

#endif // SHAREDGUICOMPONENTS_BUILDINGCOMPONENTDIALOGCENTRALWIDGET_HPP
