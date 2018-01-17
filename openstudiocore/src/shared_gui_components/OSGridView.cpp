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
  //gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
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
  buttonLayout->setContentsMargins(10,10,10,10);
  buttonLayout->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

  auto vectorController = new GridViewDropZoneVectorController();
  m_dropZone = new OSDropZone(vectorController, dropZoneText);
  m_dropZone->setMaxItems(1);

  connect(m_dropZone, &OSDropZone::itemDropped, m_gridController, &OSGridController::onItemDropped);

  buttonLayout->addWidget(m_dropZone);

  std::vector<QString> categories = m_gridController->categories();
  for(unsigned i=0; i<categories.size(); i++){
    auto button = new QPushButton(categories.at(i));
    button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::MinimumExpanding);
    button->setCheckable(true);
    buttonLayout->addWidget(button);
    buttonGroup->addButton(button,buttonGroup->buttons().size());
  }

  auto layout = new QVBoxLayout();
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
  m_contentLayout->setContentsMargins(0,0,0,0);
  widget->setLayout(m_contentLayout);
  m_contentLayout->addLayout(buttonLayout);
  widget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);

  setGridController(m_gridController);

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
}

void OSGridView::requestAddRow(int row)
{
  std::cout << "REQUEST ADDROW CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  m_rowToAdd = row;

  m_queueRequests.emplace_back(AddRow);
}

void OSGridView::requestRemoveRow(int row)
{
  std::cout << "REQUEST REMOVEROW CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  m_rowToRemove = row;

  m_queueRequests.emplace_back(RemoveRow);
}

//void OSGridView::refreshRow(int row)
//{
//  for( int j = 0; j < m_gridController->columnCount(); j++ )
//  {
//    refreshCell(row, j);
//  }
//}

QLayoutItem * OSGridView::itemAtPosition(int row, int column)
{
  auto layoutnum = row / ROWS_PER_LAYOUT;
  auto relativerow = row % ROWS_PER_LAYOUT;

  return m_gridLayouts.at(layoutnum)->itemAtPosition(relativerow, column);
}

//void OSGridView::removeWidget(int row, int column)
//{
//  // Currently this is cruft code
//  OS_ASSERT(false);
//
//  auto layoutnum = row / ROWS_PER_LAYOUT;
//  auto relativerow = row % ROWS_PER_LAYOUT;
//
//  auto index = m_gridController->columnCount() * relativerow; // this assumes that you are always deleting from the zero index column
//
//  auto count = m_gridLayouts.at(layoutnum)->count();
//
//  QLayoutItem * item = m_gridLayouts.at(layoutnum)->itemAtPosition(relativerow, column);
//
//  if (!item) return;
//
//  QLayoutItem * item2 = m_gridLayouts.at(layoutnum)->takeAt(index);
//
//  OS_ASSERT(item2);
//
//  if (item == item2){
//    QString("Good");
//  }
//  else {
//    QString("Bad");
//  }
//
//  QWidget * widget = item->widget();
//
//  if (!widget) return;
//
//  auto indexOfWidget = m_gridLayouts.at(layoutnum)->indexOf(widget);
//
//  delete widget;
//
//  delete item;
//}

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
      // Using deleteLater is actually slower than calling delete directly on the widget
      // deleteLater also introduces a strange redraw issue where the select all check box
      // is not redrawn, after being checked.
      //widget->deleteLater();

      delete child;
    }
  }
}

//void OSGridView::refreshGrid()
//{
//  if( m_gridController )
//  {
//    m_gridController->refreshModelObjects();

//    for( int i = 1; i < m_gridController->rowCount(); i++ )
//    {
//      for( int j = 0; j < m_gridController->columnCount(); j++ )
//      {
//        refreshCell(i, j);
//      }
//    }
//  }
//}

void OSGridView::requestRefreshAll()
{
  std::cout << "REQUEST REFRESHALL CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  m_queueRequests.emplace_back(RefreshAll);
}

void OSGridView::requestRefreshGrid()
{
  std::cout << "REQUEST REFRESHGRID CALLED " << std::endl;
  setEnabled(false);

  m_timer.start();

  m_queueRequests.emplace_back(RefreshGrid);
}

//void OSGridView::requestRefreshRow(int t_row)
//{
//  setEnabled(false);
//
//  m_timer.start();
//
//  m_queueRequests.emplace_back(RefreshRow);
//}

void OSGridView::doRefresh()
{
  std::cout << " DO REFRESH CALLED " << m_queueRequests.size() << std::endl;

  if (m_queueRequests.empty())
  {
    setEnabled(true);
    return;
  }

  bool has_add_row = false;
  bool has_remove_row = false;
  bool has_refresh_grid = false;
  bool has_refresh_all = false;

  for (const auto &r : m_queueRequests)
  {
    if (r == AddRow) has_add_row = true;
    if (r == RemoveRow) has_remove_row = true;
    if (r == RefreshGrid) has_refresh_grid = true;
    if (r == RefreshAll) has_refresh_all = true;
  }

  m_queueRequests.clear();

  //if (has_refresh_all) {
  //  refreshAll();
  //}
  //else if (has_refresh_grid) {
  //  refreshGrid(); // This now causes a crash
  //}
  //else if (has_add_row) {
  //  addRow(m_rowToAdd);
  //}
  //else if (has_remove_row) {
  //  removeRow(m_rowToRemove);
  //}
  //else {
  //  // Should never get here
  //  OS_ASSERT(false);
  //}

  refreshAll(); // TODO remove this and uncomment the block above for finer granularity refreshes
  setEnabled(true);
}

void OSGridView::refreshAll()
{
  std::cout << " REFRESHALL CALLED " << std::endl;
  m_queueRequests.clear();
  deleteAll();

  if (m_gridController)
  {
    m_gridController->refreshModelObjects();

    for (int i = 0; i < m_gridController->rowCount(); i++)
    {
      for (int j = 0; j < m_gridController->columnCount(); j++)
      {
        addWidget(i, j);
      }
    }

    this->m_gridController->getObjectSelector()->updateWidgets();

    QTimer::singleShot(0, this, SLOT(selectRowDeterminedByModelSubTabView()));
  }
}

void OSGridView::selectRowDeterminedByModelSubTabView()
{
  // Get selected item
  auto selectedItem = m_gridController->getSelectedItemFromModelSubTabView();
  if (!selectedItem) return;

  // Get new index
  int newIndex;
  if (m_gridController->getRowIndexByItem(selectedItem, newIndex)) {
    // Update the old index
    m_gridController->m_oldIndex = newIndex;
  }

  // If the index is valid, call slot
  if (m_gridController->m_oldIndex > -1){
    QTimer::singleShot(0, this, SLOT(doRowSelect()));
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
