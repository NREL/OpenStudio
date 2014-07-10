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

#include "../openstudio_lib/OSDropZone.hpp"
#include "../openstudio_lib/OSItem.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddObject.hpp"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>

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

OSGridView::OSGridView(OSGridController * gridController, const QString & headerText, const QString & dropZoneText, QWidget * parent)
  : QWidget(parent),
  m_gridLayout(nullptr),
  m_CollapsibleView(nullptr),
  m_gridController(gridController)
{
  m_gridLayout = new QGridLayout();
  m_gridLayout->setSpacing(0);
  m_gridLayout->setContentsMargins(0,0,0,0);
  m_gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  auto buttonGroup = new QButtonGroup();
  bool isConnected = false;
  isConnected = connect(buttonGroup, SIGNAL(buttonClicked(int)),
    this, SLOT(selectCategory(int)));
  OS_ASSERT(isConnected);

  auto buttonLayout = new QHBoxLayout();
  buttonLayout->setSpacing(3);
  buttonLayout->setContentsMargins(0,0,0,0);
  buttonLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  auto vectorController = new GridViewDropZoneVectorController();
  OSDropZone * dropZone = nullptr; 
#ifdef Q_OS_MAC
    dropZone = new OSDropZone(vectorController, dropZoneText, QSize(WIDTH_DZ,HEIGHT_DZ));
#else
    dropZone = new OSDropZone(vectorController, dropZoneText, QSize(WIDTH,HEIGHT));
#endif
  dropZone->setMaxItems(1);

  isConnected = connect(dropZone,SIGNAL(itemDropped(const OSItemId&)),
    m_gridController,SLOT(onItemDropped(const OSItemId&)));
  OS_ASSERT(isConnected);

  buttonLayout->addWidget(dropZone,0,Qt::AlignLeft);

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

  auto header = new DarkGradientHeader();
  header->label->setText(headerText);

  auto widget = new QWidget;

  auto collabsibleView = new OSCollapsibleView(this);
  layout->addWidget(collabsibleView);
  collabsibleView->setHeader(header);
  collabsibleView->setContent(widget);
  collabsibleView->setExpanded(true);

  setGridController(m_gridController);

  QVBoxLayout * m_contentLayout = nullptr;
  m_contentLayout = new QVBoxLayout();
  m_contentLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  m_contentLayout->setSpacing(10);
  m_contentLayout->setContentsMargins(0,10,0,0);
  widget->setLayout(m_contentLayout);
  m_contentLayout->addLayout(buttonLayout);
  m_contentLayout->addLayout(m_gridLayout);

  setContentsMargins(5,5,5,5);

  std::vector<QAbstractButton *> buttons = buttonGroup->buttons().toVector().toStdVector();
  if(buttons.size() > 0){
    QPushButton * button = qobject_cast<QPushButton *>(buttons.at(0));
    OS_ASSERT(button);
    button->setChecked(true);
    selectCategory(0);
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

  isConnected = connect(m_gridController,SIGNAL(itemInserted(int,int)),this,SLOT(addWidget(int,int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_gridController,SIGNAL(itemRemoved(int,int)),this,SLOT(removeWidget(int,int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_gridController,SIGNAL(itemChanged(int,int)),this,SLOT(refresh(int,int)));
  OS_ASSERT(isConnected);

  isConnected = connect(m_gridController,SIGNAL(modelReset()),this,SLOT(refreshAll()));
  OS_ASSERT(isConnected);

  refreshAll();
}

void OSGridView::refresh(int row, int column)
{
  removeWidget(row,column);

  addWidget(row,column);
}

void OSGridView::removeWidget(int row, int column)
{
  QLayoutItem * item = m_gridLayout->itemAtPosition(row,column);

  OS_ASSERT(item);

  QWidget * widget = item->widget();

  OS_ASSERT(widget);

  delete widget;

  delete item;
}

void OSGridView::deleteAll()
{
  QLayoutItem * child;
  while((child = m_gridLayout->takeAt(0)) != nullptr)
  {
    QWidget * widget = child->widget();

    OS_ASSERT(widget);

    delete widget;

    delete child;
  }
}

void OSGridView::refreshAll()
{
  deleteAll();

  if( m_gridController )
  {
    m_gridController->refreshModelObjects();

    for( int i = 0; i < m_gridController->rowCount(); i++ )
    {
      for( int j = 0; j < m_gridController->columnCount(); j++ )
      {
        addWidget(i,j);
      }
    }
  }
  // NOTE This was added to make dissimilar widget types in a given column to
  // fill and justify correctly.  It appeared to be the most simple solution.
  auto widget = new QWidget();
  widget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  m_gridLayout->addWidget(widget,0,m_gridController->columnCount());
}

void OSGridView::addWidget(int row, int column)
{
  OS_ASSERT(m_gridController);

  QWidget * widget = m_gridController->widgetAt(row,column);

  OS_ASSERT(m_gridLayout);

  m_gridLayout->addWidget(widget,row,column);
}

void OSGridView::setHorizontalHeader(std::vector<QWidget *> widgets)
{
  OS_ASSERT(m_gridLayout);

  int column = 0;
  for (QWidget * widget : widgets) {
    m_gridLayout->addWidget(widget,0,column++);
  }
}

void OSGridView::setHorizontalHeader(std::vector<QString> names)
{
  OS_ASSERT(m_gridLayout);

  int column = 0;
  for (const QString& name : names) {
    m_gridLayout->addWidget(new QLabel(name),0,column++);
  }
}

void OSGridView::selectCategory(int index)
{
  deleteAll();

  m_gridController->categorySelected(index);

  refreshAll();

}

} // openstudio

