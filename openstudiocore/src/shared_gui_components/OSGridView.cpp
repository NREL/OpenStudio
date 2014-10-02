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

#include "OSGridView.hpp"

#include "HeaderViews.hpp"
#include "OSCollapsibleView.hpp"
#include "OSGridController.hpp"

#include "../openstudio_lib/ModelObjectInspectorView.hpp"
#include "../openstudio_lib/ModelSubTabView.hpp"
#include "../openstudio_lib/OSDropZone.hpp"
#include "../openstudio_lib/OSItem.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddObject.hpp"

#include <QApplication>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QHideEvent>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QShowEvent>
#include <QStackedWidget>

#ifdef Q_OS_MAC
  #define WIDTH  110
  #define HEIGHT 60
  #define WIDTH_DZ 98
  #define HEIGHT_DZ 48
#else
  #define WIDTH  80
  #define HEIGHT 60
#endif

namespace openstudio {

QGridLayout *OSGridView::makeGridLayout()
{
  auto gridLayout = new QGridLayout();
  gridLayout->setSpacing(0);
  gridLayout->setContentsMargins(0,0,0,0);
  //gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  return gridLayout;
}

OSGridView::OSGridView(OSGridController * gridController,
  const QString & headerText,
  const QString & dropZoneText,
  bool useHeader,
  QWidget * parent)
  : QWidget(parent),
    m_dropZone(nullptr),
    m_contentLayout(nullptr),
    m_CollapsibleView(nullptr),
    m_gridController(gridController)
{

  auto buttonGroup = new QButtonGroup();
  connect(buttonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &OSGridView::selectCategory);

  auto buttonLayout = new QHBoxLayout();
  buttonLayout->setSpacing(3);
  buttonLayout->setContentsMargins(0,0,0,10);
  buttonLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  auto vectorController = new GridViewDropZoneVectorController();
#ifdef Q_OS_MAC
  m_dropZone = new OSDropZone(vectorController, dropZoneText, QSize(WIDTH_DZ,HEIGHT_DZ));
#else
  m_dropZone = new OSDropZone(vectorController, dropZoneText, QSize(WIDTH,HEIGHT));
#endif
  m_dropZone->setMaxItems(1);

  connect(m_dropZone, &OSDropZone::itemDropped, m_gridController, &OSGridController::onItemDropped);

  auto isConnected = connect(this, SIGNAL(dropZoneItemClicked(OSItem*)), this, SLOT(onDropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  buttonLayout->addWidget(m_dropZone,0,Qt::AlignLeft);

  std::vector<QString> categories = m_gridController->categories();
  for(unsigned i=0; i<categories.size(); i++){
    auto button = new QPushButton(categories.at(i));
#ifdef Q_OS_MAC
    button->setFixedSize(WIDTH,HEIGHT);
#else
    button->setMinimumSize(WIDTH,HEIGHT);
#endif
    button->setCheckable(true);
    buttonLayout->addWidget(button,0,Qt::AlignLeft);
    buttonGroup->addButton(button,buttonGroup->buttons().size());
  }
  buttonLayout->addStretch();

  QVBoxLayout * layout = nullptr;

  layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);

  auto widget = new QWidget;

  if (useHeader) {
    auto header = new DarkGradientHeader();
    header->label->setText(headerText); 
    auto collabsibleView = new OSCollapsibleView(true);
    collabsibleView->setHeader(header);
    collabsibleView->setContent(widget);
    collabsibleView->setExpanded(true); 
    layout->addWidget(collabsibleView);
  } else {
    layout->addWidget(widget);
  }


  m_contentLayout = new QVBoxLayout();
  m_contentLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_contentLayout->setSpacing(0);
  m_contentLayout->setContentsMargins(0,10,0,0);
  widget->setLayout(m_contentLayout);
  m_contentLayout->addLayout(buttonLayout);


  setGridController(m_gridController);

  setContentsMargins(5,5,5,5);

  std::vector<QAbstractButton *> buttons = buttonGroup->buttons().toVector().toStdVector();
  if(buttons.size() > 0){
    QPushButton * button = qobject_cast<QPushButton *>(buttons.at(0));
    OS_ASSERT(button);
    button->setChecked(true);
    selectCategory(0);
  }

  m_timer.setSingleShot(true);
  connect(&m_timer, &QTimer::timeout, this, &OSGridView::doRefresh);

  if (this->isVisible()) {
    m_gridController->connectToModel();
    refreshAll();
  }
}

void OSGridView::setGridController(OSGridController * gridController)
{
  if( m_gridController )
  {
    m_gridController->disconnect(this);
  }

  m_gridController = gridController;

  m_gridController->setParent(this);

  bool isConnected = false;


  isConnected = connect(m_gridController, SIGNAL(gridRowSelected(OSItem *)), this, SIGNAL(gridRowSelected(OSItem *)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemSelected(OSItem *)), m_gridController, SLOT(onItemSelected(OSItem *)));
  OS_ASSERT(isConnected);
}

void OSGridView::refreshCell(int row, int column)
{
  removeWidget(row,column);

  addWidget(row,column);
}

void OSGridView::refreshRow(int row)
{
  std::cout << " REFRESH ROW CALLED " << row << std::endl;

  for( int j = 0; j < m_gridController->columnCount(); j++ )
  {
    refreshCell(row, j);
  }
}

QLayoutItem * OSGridView::itemAtPosition(int row, int column)
{
  auto layoutnum = row / ROWS_PER_LAYOUT;
  auto relativerow = row % ROWS_PER_LAYOUT;

  return m_gridLayouts.at(layoutnum)->itemAtPosition(relativerow, column);
}

void OSGridView::removeWidget(int row, int column)
{
  QLayoutItem * item = itemAtPosition(row,column);

  OS_ASSERT(item);

  QWidget * widget = item->widget();

  OS_ASSERT(widget);

  delete widget;

  delete item;
}

void OSGridView::deleteAll()
{
  for (auto layout : m_gridLayouts)
  {
    QLayoutItem * child;
    while((child = layout->takeAt(0)) != nullptr)
    {
      QWidget * widget = child->widget();

      OS_ASSERT(widget);

      delete widget;

      delete child;
    }
  }
}

void OSGridView::refreshGrid()
{
  std::cout << " REFRESH GRID CALLED " << std::endl;

  if( m_gridController )
  {
    for( int i = 1; i < m_gridController->rowCount(); i++ )
    {
      for( int j = 0; j < m_gridController->columnCount(); j++ )
      {
        refreshCell(i, j);
      }
    }
  }
}

void OSGridView::requestRefreshAll()
{
  std::cout << "REQUEST REFRESHALL CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  m_refreshRequests.emplace_back(RefreshAll);
}

void OSGridView::requestRefreshGrid()
{
  std::cout << "REQUEST REFRESHGRID CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  m_refreshRequests.emplace_back(RefreshGrid);
}

void OSGridView::requestRefreshRow(int t_row)
{
  std::cout << "REQUEST REFRESH ROW CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  // TODO to do increase resolution here
  m_refreshRequests.emplace_back(RefreshGrid);
}

void OSGridView::doRefresh()
{
  std::cout << " DO REFRESH CALLED " << m_refreshRequests.size() << std::endl;

  if (m_refreshRequests.empty())
  {
    setEnabled(true);
    return;
  }

//  bool has_refresh_grid = false;
  bool has_refresh_all = false;

  for (const auto &r : m_refreshRequests)
  {
//    if (r == RefreshGrid) has_refresh_grid = true;
    if (r == RefreshAll) has_refresh_all = true;
  }

  m_refreshRequests.clear();

  if (true)
  //if (has_refresh_all)
  {
    refreshAll();
  } else {
    refreshGrid(); // This now causes a crash
  }

  setEnabled(true);
}

void OSGridView::refreshAll()
{
  std::cout << " REFRESHALL CALLED " << std::endl;
  m_refreshRequests.clear();
  deleteAll();

  if( m_gridController )
  {
    m_gridController->refreshModelObjects();

    for( int i = 0; i < m_gridController->rowCount(); i++ )
    {
      if (i > 0)
      {
        //connect(m_gridController->modelObject(i).getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, std::bind(&OSGridView::refreshRow, this, i));
      }
      for( int j = 0; j < m_gridController->columnCount(); j++ )
      {
        addWidget(i,j);
      }
    }
  }
  // NOTE This was added to make dissimilar widget types in a given column to
  // fill and justify correctly.  It appeared to be the most simple solution.
  auto widget = new QWidget();
  //widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  addWidget(widget, 0, m_gridController->columnCount());

  // Get selected item
  auto selectedItem = m_gridController->getSelectedItemFromModelSubTabView();
  if (!selectedItem) return;

  // Get new index
  int newIndex;
  if (m_gridController->getRowIndexByItem(selectedItem, newIndex)) {
    // Update the old index
    m_gridController->m_oldIndex = newIndex;
  }

  normalizeColumnWidths();

  // If the index is valid, call slot
  if (m_gridController->m_oldIndex > -1){
    QTimer::singleShot(0, this, SLOT(doRowSelect()));
  }
}

void OSGridView::normalizeColumnWidths()
{

  std::vector<int> colmins(m_gridController->columnCount(), 0);

  for( int i = 0; i < m_gridController->rowCount(); i++ )
  {
    for( int j = 0; j < m_gridController->columnCount(); j++ )
    {
      const auto *w = itemAtPosition(i, j)->widget();
      OS_ASSERT(w);
      colmins[j] = std::max(colmins[j], w->minimumWidth());
    }
  }

  for(int i = 0; i < m_gridController->rowCount(); i += ROWS_PER_LAYOUT)
  {
    for( int j = 0; j < m_gridController->columnCount(); j++ )
    {
      auto *w = itemAtPosition(i, j)->widget();
      OS_ASSERT(w);
      w->setMinimumWidth(colmins[j]);
    }
  }
}

void OSGridView::doRowSelect()
{
  // If the index is valid, do some work
  if (m_gridController->m_oldIndex > -1){
    m_gridController->selectRow(m_gridController->m_oldIndex, true);
  }
}

void OSGridView::addWidget(int row, int column)
{
  OS_ASSERT(m_gridController);

  QWidget * widget = m_gridController->widgetAt(row,column);

  addWidget(widget, row, column);
}

void OSGridView::addWidget(QWidget *w, int row, int column)
{
  unsigned layoutindex = row / ROWS_PER_LAYOUT;
  auto relativerow = row % ROWS_PER_LAYOUT;

  if (layoutindex >= m_gridLayouts.size())
  {
    auto grid = makeGridLayout();
    OS_ASSERT(grid);

    m_gridLayouts.push_back(grid);
    OS_ASSERT(m_contentLayout);
    m_contentLayout->addLayout(grid);
  }

  m_gridLayouts[layoutindex]->addWidget(w, relativerow, column);
}

void OSGridView::setHorizontalHeader(std::vector<QWidget *> widgets)
{
  int column = 0;
  for (QWidget * widget : widgets) {
    addWidget(widget,0,column++);
  }
}

void OSGridView::setHorizontalHeader(std::vector<QString> names)
{
  int column = 0;
  for (const QString& name : names) {
    addWidget(new QLabel(name),0,column++);
  }
}

void OSGridView::selectCategory(int index)
{
  m_gridController->categorySelected(index);

  requestRefreshAll();
}

ModelSubTabView * OSGridView::modelSubTabView()
{
  ModelSubTabView *  modelSubTabView = nullptr;

  if (!this->parent() || !this->parent()->parent()) return modelSubTabView;

  auto stackedWidget = qobject_cast<QStackedWidget *>(this->parent()->parent());
  if (!stackedWidget) return modelSubTabView;

  auto widget = qobject_cast<QWidget *>(stackedWidget->parent());
  if (!widget) return modelSubTabView;

  auto scrollArea = qobject_cast<QScrollArea *>(widget->parent());
  if (!scrollArea) return modelSubTabView;

  auto modelObjectInspectorView = qobject_cast<ModelObjectInspectorView *>(scrollArea->parent());
  if (!modelObjectInspectorView) return modelSubTabView;

  auto object = qobject_cast<QObject *>(modelObjectInspectorView->parent());
  if (!object) return modelSubTabView;

  modelSubTabView = qobject_cast<ModelSubTabView *>(object->parent());
  return modelSubTabView;
}

void OSGridView::onSelectionCleared()
{
  m_gridController->onSelectionCleared();
}

void OSGridView::onDropZoneItemClicked(OSItem* item)
{
}

void OSGridView::hideEvent(QHideEvent * event)
{
  m_gridController->disconnectFromModel();

  QWidget::hideEvent(event);
}

void OSGridView::showEvent(QShowEvent * event)
{
  m_gridController->connectToModel();
  refreshAll();

  QWidget::showEvent(event);
}

} // openstudio
