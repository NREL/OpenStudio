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

#ifndef SHAREDGUICOMPONENTS_BUILDINGCOMPONENTDIALOG_HPP
#define SHAREDGUICOMPONENTS_BUILDINGCOMPONENTDIALOG_HPP

#include <QDialog>
#include <vector>

class QButtonGroup;
class QGridLayout;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QScrollArea;
class QStackedWidget;
class QTableWidget;
class QTimer;
class QTreeView;
class QWidget;

namespace openstudio {

class BuildingComponentDialogCentralWidget;
class Component;

class BuildingComponentDialog : public QDialog
{
  Q_OBJECT

public:
  BuildingComponentDialog(std::string& filterType, bool isBclDlg = true, QWidget * parent = nullptr);
  virtual ~BuildingComponentDialog() {}

  int currentTIDSelection();
  int rootTID();
  void setRootTID(int tid);
  bool showNewComponents();
  void setShowNewComponents(bool showNewComponents);

protected:
  void paintEvent(QPaintEvent * event) override;

private:
  void requestComponents(const std::string& filterType, int tids, int pageIdx, const QString & searchString);
  void createLayout(bool isBclDlg);

  std::string m_filterType;
  QString m_dlgTitle;
  QTreeView * m_tidTreeView;
  BuildingComponentDialogCentralWidget * m_centralWidget;
  QScrollArea * m_rightScrollArea;
  Component * m_expandedComponent;
  QLineEdit * m_lineEdit;
  QStackedWidget * m_stackedWidget;
  QTimer * m_timer;

signals:
  void headerClicked(bool checked);
  void componentClicked(bool checked);
  void collapsibleComponentClicked(bool checked);
  void getComponentsByPage(int pageNum);

private slots:
  void on_tidClicked(const QModelIndex & index);
  void on_headerClicked(bool checked);
  void on_componentClicked(bool checked);
  void on_collapsibleComponentClicked(bool checked);
  void on_getComponentsByPage(int pageIdx);
  void on_searchButton();
  void on_componentsReady();
  void on_noComponents();
  void on_requestComponents(const std::string& filterType, int tids, int pageIdx, const QString & searchString);

};

} // namespace openstudio

#endif // SHAREDGUICOMPONENTS_BUILDINGCOMPONENTDIALOG_HPP
