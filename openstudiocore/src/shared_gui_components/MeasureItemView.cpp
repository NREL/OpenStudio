/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "MeasureItemView.hpp"
#include "OSViewSwitcher.hpp"
#include "OSListView.hpp"
#include "HeaderViews.hpp"
#include "Buttons.hpp"
#include "LocalLibraryController.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QMimeData>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDragEnterEvent>

namespace openstudio{
  
namespace measuretab {

  RectangularDropZone::RectangularDropZone(bool t_fixed)
  : QWidget()
{
  setAcceptDrops(true);

  setFixedHeight(30);

  if (t_fixed)
  {
    setStyleSheet("openstudio--measuretab--RectangularDropZone {background: #F4F4F4; border: 2px dashed #A5A5A5;}");
  } else {
    setStyleSheet("openstudio--measuretab--RectangularDropZone {background: #E0E0E0; border: 2px dashed gray;}");
  }

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  nameLabel = new QLabel();
  mainHLayout->addWidget(nameLabel);
}

void RectangularDropZone::setAcceptedMimeType(const QString & type)
{
  m_acceptedMimeType = type;
}

void RectangularDropZone::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void RectangularDropZone::dropEvent(QDropEvent * event)
{
  emit dataDropped(event);
}

void RectangularDropZone::dragEnterEvent(QDragEnterEvent * event)
{
  if(event->mimeData()->hasFormat(m_acceptedMimeType))
  {
    event->accept();
  }
}

MeasureDropZone::MeasureDropZone()
  : RectangularDropZone(false)
{
  nameLabel->setStyleSheet("QLabel { color: #7D7D7D; }");
  nameLabel->setText("Drop Measure From Library");
}

MeasureItemButton::MeasureItemButton(bool t_fixed)
  : QAbstractButton()
{
  setFixedHeight(25);

  setHasEmphasis(false);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  nameLabel = new QLabel();
  if (t_fixed)
  {
    nameLabel->setStyleSheet("font:italic");
  }

  mainHLayout->addWidget(nameLabel);

  mainHLayout->addStretch();

  cautionLabel = new QLabel();
  cautionLabel->setPixmap(QPixmap(":/shared_gui_components/images/warning_icon.png"));
  mainHLayout->addWidget(cautionLabel);
  cautionLabel->setVisible(false);
}

void MeasureItemButton::setHasEmphasis(bool hasEmphasis)
{
  if( hasEmphasis )
  {
    setStyleSheet("openstudio--measuretab--MeasureItemButton { background: #FECD60; border: 2px solid #EE641A; }");
  }
  else
  {
    setStyleSheet("openstudio--measuretab--MeasureItemButton {background: #E0E0E0;}");
  }
}

void MeasureItemButton::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

MeasureItemView::MeasureItemView(bool t_fixed)
  : QWidget()
{
  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0,0,0,0);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  measureItemButton = new MeasureItemButton(t_fixed);
  mainHLayout->addWidget(measureItemButton);

  duplicateButton = new SofterDuplicateButton();
  mainHLayout->addWidget(duplicateButton);

  removeButton = new SofterRemoveButton();
  mainHLayout->addWidget(removeButton);

  if (t_fixed)
  {
    duplicateButton->setVisible(false);
    removeButton->setVisible(false);
  }
}

void MeasureItemView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

} // measuretab

} // openstudio

