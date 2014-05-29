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

  bool isConnected = false;

  m_lineEdit = new QLineEdit(this);

  auto searchButton = new QPushButton();
  searchButton->setToolTip("Click to add a search term to the selected category");
  searchButton->setStyleSheet("QPushButton { border: none; background-image: url(\":/shared_gui_components/images/searchbox_magnifyingglass.png\"); }");
  searchButton->setFixedSize(24,24);

  isConnected = connect(searchButton,SIGNAL(clicked()),
                        this,SLOT(on_searchButton()));
  OS_ASSERT(isConnected);

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

  isConnected = connect(m_tidTreeView, SIGNAL(clicked(const QModelIndex &)),
                        this, SLOT(on_tidClicked(const QModelIndex &)));
  OS_ASSERT(isConnected);

  leftPanelayout->addWidget(m_tidTreeView,10);

  leftPanelayout->addStretch();

  // The central pane

  m_centralWidget = new BuildingComponentDialogCentralWidget();

  isConnected = connect(m_centralWidget, SIGNAL(headerClicked(bool)),
                        this, SIGNAL(headerClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(headerClicked(bool)),
                        this, SLOT(on_headerClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(componentClicked(bool)),
                        this, SIGNAL(componentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(componentClicked(bool)),
                        this, SLOT(on_componentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(collapsibleComponentClicked(bool)),
                        this, SIGNAL(collapsibleComponentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(collapsibleComponentClicked(bool)),
                        this, SLOT(on_collapsibleComponentClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(getComponentsByPage(int)),
                        this, SIGNAL(getComponentsByPage(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(getComponentsByPage(int)),
                        this, SLOT(on_getComponentsByPage(int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(componentsReady()),
                        this, SLOT(on_componentsReady()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(requestComponents(const std::string&,int,int,const QString &)),
                        this, SLOT(on_requestComponents(const std::string&,int,int,const QString &)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_centralWidget, SIGNAL(noComponents()),
                        this, SLOT(on_noComponents()));
  OS_ASSERT(isConnected);

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
  connect(m_timer,SIGNAL(timeout()),busyIcon,SLOT(rotate()));

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

