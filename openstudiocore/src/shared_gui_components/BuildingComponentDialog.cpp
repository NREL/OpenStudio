/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "BuildingComponentDialog.hpp"
#include "BuildingComponentDialogCentralWidget.hpp"
#include "BusyWidget.hpp"
#include "Component.hpp"
#include "TIDItemModel.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QModelIndex>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTimer>
#include <QTreeView>

namespace openstudio {

BuildingComponentDialog::BuildingComponentDialog(std::string& filterType, bool isBclDlg, QWidget * parent)
: QDialog(parent, Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
  m_filterType(filterType),
  m_dlgTitle(QString()),
  m_tidTreeView(nullptr),
  m_centralWidget(nullptr),
  m_rightScrollArea(nullptr),
  m_expandedComponent(nullptr),
  m_lineEdit(nullptr),
  m_stackedWidget(nullptr),
  m_timer(nullptr)
{
  createLayout(isBclDlg);
}

void BuildingComponentDialog::createLayout(bool isBclDlg)
{
  if(isBclDlg){
    m_dlgTitle = "Online BCL";
  }
  else{
    m_dlgTitle = "Local Library";
  }
  setWindowTitle(m_dlgTitle);

  setModal(true);

  setSizeGripEnabled(true);

  setObjectName("BlueGradientWidget");

  // The left pane

  auto leftPanelayout = new QVBoxLayout();

  auto leftPaneWidget = new QWidget(this);
  leftPaneWidget->setObjectName("GrayWidget");
  leftPaneWidget->setLayout(leftPanelayout);

  m_lineEdit = new QLineEdit(this);

  auto searchButton = new QPushButton();
  searchButton->setToolTip("Click to add a search term to the selected category");
  searchButton->setStyleSheet("QPushButton { border: none; background-image: url(\":/shared_gui_components/images/searchbox_magnifyingglass.png\"); }");
  searchButton->setFixedSize(24,24);

  connect(searchButton, &QPushButton::clicked, this, &BuildingComponentDialog::on_searchButton);

  auto searchlayout = new QHBoxLayout();
  searchlayout->addWidget(searchButton);
  searchlayout->addWidget(m_lineEdit);
  leftPanelayout->addLayout(searchlayout);

  QLabel * categoryLabel = new QLabel("Categories");
  categoryLabel->setObjectName("H1");
  leftPanelayout->addWidget(categoryLabel);

  QDomDocument doc;

  QString docString;
  QFile file;
  if (m_filterType == "components")
  {
    file.setFileName(":/library/tid_api.xml");
  }
  else if (m_filterType == "measures")
  {
    file.setFileName(":/shared_gui_components/taxonomy.xml");
  }

  if(file.open(QFile::ReadOnly))
  {
    QTextStream docIn(&file);
    docString = docIn.readAll();
    file.close();
  }

  doc.setContent(docString);

  auto model = new TIDItemModel(doc,this);

  m_tidTreeView = new QTreeView();

  m_tidTreeView->setHeaderHidden(true);

  m_tidTreeView->setModel(model);

  m_tidTreeView->setColumnHidden(1,true);

  m_tidTreeView->setRootIsDecorated(true);

  connect(m_tidTreeView, &QTreeView::clicked, this, &BuildingComponentDialog::on_tidClicked);

  leftPanelayout->addWidget(m_tidTreeView,10);

  leftPanelayout->addStretch();

  // The central pane

  m_centralWidget = new BuildingComponentDialogCentralWidget();

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::headerClicked, this, &BuildingComponentDialog::headerClicked);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::headerClicked, this, &BuildingComponentDialog::on_headerClicked);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::componentClicked, this, &BuildingComponentDialog::componentClicked);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::componentClicked, this, &BuildingComponentDialog::on_componentClicked);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::collapsibleComponentClicked, this, &BuildingComponentDialog::collapsibleComponentClicked);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::collapsibleComponentClicked, this, &BuildingComponentDialog::on_collapsibleComponentClicked);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::getComponentsByPage, this, &BuildingComponentDialog::getComponentsByPage);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::getComponentsByPage, this, &BuildingComponentDialog::on_getComponentsByPage);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::componentsReady, this, &BuildingComponentDialog::on_componentsReady);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::requestComponents, this, &BuildingComponentDialog::on_requestComponents);

  connect(m_centralWidget, &BuildingComponentDialogCentralWidget::noComponents, this, &BuildingComponentDialog::on_noComponents);

  auto centralScrollArea = new QScrollArea(this);
  centralScrollArea->setFrameStyle(QFrame::NoFrame);
  centralScrollArea->setObjectName("GrayWidget");
  centralScrollArea->setWidgetResizable(true);
  centralScrollArea->setWidget(m_centralWidget);

  // The right pane

  m_rightScrollArea = new QScrollArea(this);
  m_rightScrollArea->setFrameStyle(QFrame::NoFrame);
  m_rightScrollArea->setObjectName("GrayWidget");
  m_rightScrollArea->setWidgetResizable(true);

  auto splitter = new QSplitter(this);
  splitter->setOrientation(Qt::Horizontal);
  splitter->addWidget(leftPaneWidget);
  splitter->addWidget(centralScrollArea);
  splitter->addWidget(m_rightScrollArea);

  QLabel * busyLabel = new QLabel("Searching BCL...");
  busyLabel->setObjectName("H1");

  auto busyIcon = new BusyWidget();

  auto busyLayout = new QVBoxLayout();
  busyLayout->addStretch();
  busyLayout->addWidget(busyLabel,0,Qt::AlignCenter);
  busyLayout->addWidget(busyIcon,0,Qt::AlignCenter);
  busyLayout->addStretch();

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, busyIcon, &BusyWidget::rotate);
  auto busy = new QWidget();
  busy->setLayout(busyLayout);

  m_stackedWidget = new QStackedWidget();
  m_stackedWidget->insertWidget(0,busy);
  m_stackedWidget->insertWidget(1,splitter);

  auto mainLayout = new QHBoxLayout();
  mainLayout->addWidget(m_stackedWidget);

  setLayout(mainLayout);

  // select the first tree node
  QModelIndex index = model->index(0, 0);
  m_tidTreeView->setCurrentIndex(index);
  on_tidClicked(index);
}

void BuildingComponentDialog::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

int BuildingComponentDialog::currentTIDSelection()
{
  QModelIndex index = m_tidTreeView->currentIndex();

  if( index.isValid() )
  {
    QAbstractItemModel * model = m_tidTreeView->model();

    QVariant variant = model->data(index.sibling(index.row(),1));

    return variant.toInt();
  }

  return 0;
}

int BuildingComponentDialog::rootTID()
{
  QModelIndex rootIndex = m_tidTreeView->rootIndex();

  if( rootIndex.isValid() )
  {
    QVariant variant = m_tidTreeView->model()->data(rootIndex.sibling(rootIndex.row(),1));

    return variant.toInt();
  }

  return 0;
}

void BuildingComponentDialog::setRootTID(int tid)
{
  QModelIndex index = static_cast<TIDItemModel *>(m_tidTreeView->model())->indexForTID(tid);

  m_tidTreeView->setRootIndex(index);
}

void BuildingComponentDialog::requestComponents(const std::string& filterType, int tids, int pageIdx, const QString & searchString)
{
  m_timer->start(50);
  m_stackedWidget->setCurrentIndex(0);
  QModelIndex index = m_tidTreeView->currentIndex();
  while (index.parent().isValid())
  {
    index = index.parent();
  }
  QString parentTaxonomy = m_tidTreeView->model()->data(index).toString();

  m_centralWidget->setTid(filterType, tids, pageIdx, parentTaxonomy, searchString);
}

bool BuildingComponentDialog::showNewComponents()
{
  return m_centralWidget->showNewComponents();
}

void BuildingComponentDialog::setShowNewComponents(bool showNewComponents)
{
  m_centralWidget->setShowNewComponents(showNewComponents);
}

///! Slots

void BuildingComponentDialog::on_searchButton()
{
  QModelIndex index = m_tidTreeView->currentIndex();
  if(!index.isValid()) return;

  QVariant variant = m_tidTreeView->model()->data(index.sibling(index.row(),1));

  requestComponents(m_filterType, variant.toInt(), m_centralWidget->pageIdx(), m_lineEdit->text());
}

void BuildingComponentDialog::on_tidClicked(const QModelIndex & index)
{
  // clear the line edit when initiating search from the tree
  m_lineEdit->setText("");

  QVariant variant = m_tidTreeView->model()->data(index.sibling(index.row(),1));

  requestComponents(m_filterType, variant.toInt(), m_centralWidget->pageIdx(), QString());
}

void BuildingComponentDialog::on_headerClicked(bool checked)
{
}

void BuildingComponentDialog::on_componentClicked(bool checked)
{
  if(m_expandedComponent){
    delete m_expandedComponent;
    m_expandedComponent = nullptr;
  }
  m_expandedComponent = new Component(*m_centralWidget->checkedComponent());
  m_expandedComponent->setCheckable(false);
  m_rightScrollArea->setWidget(m_expandedComponent);
}

void BuildingComponentDialog::on_collapsibleComponentClicked(bool checked)
{
}

void BuildingComponentDialog::on_getComponentsByPage(int pageIdx)
{
}

void BuildingComponentDialog::on_componentsReady()
{
  m_stackedWidget->setCurrentIndex(1);
  m_timer->stop();
}

void BuildingComponentDialog::on_requestComponents(const std::string& filterType, int tids, int pageIdx, const QString & searchString)
{
  if(m_lineEdit->text() != searchString){
    m_lineEdit->setText(searchString);
  }
  requestComponents(filterType, tids, pageIdx, searchString);
}

void BuildingComponentDialog::on_noComponents()
{
  m_expandedComponent = new Component();
  m_expandedComponent->setCheckable(false);
  m_rightScrollArea->setWidget(m_expandedComponent);
}

} // namespace openstudio

