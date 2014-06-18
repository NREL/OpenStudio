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

#include "LoopLibraryDialog.hpp"
#include <QLabel>
#include <QFrame>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

namespace openstudio {

LoopLibraryDialog::LoopLibraryDialog(QWidget * parent)
  : QDialog(parent)
{
  setObjectName("GrayWidget");

  setFixedSize(280,584);

  setWindowTitle("Add HVAC System");
  setWindowFlags(Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint));

  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);

  QLabel * loopsLabel = new QLabel("HVAC Systems");
  loopsLabel->setStyleSheet("QLabel { margin-left: 5px; }");
  mainVLayout->addSpacing(5);
  mainVLayout->addWidget(loopsLabel);
  mainVLayout->addSpacing(5);

  QFrame * divider = new QFrame();
  divider->setFrameShape(QFrame::HLine);
  divider->setFrameShadow(QFrame::Sunken);
  mainVLayout->addWidget(divider);

  QWidget * scrollAreaWidget = new QWidget();
  scrollAreaWidget->setContentsMargins(0,0,0,0);
  scrollAreaWidget->setObjectName("GrayWidget");

  QVBoxLayout * scrollAreaLayout = new QVBoxLayout();
  scrollAreaLayout->setContentsMargins(0,0,0,0);
  scrollAreaLayout->setSpacing(0);
  scrollAreaLayout->setAlignment(Qt::AlignTop);

  scrollAreaWidget->setLayout(scrollAreaLayout);

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameStyle(QFrame::NoFrame);
  m_scrollArea->setWidget(scrollAreaWidget);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  mainVLayout->addWidget(m_scrollArea,100);

  mainVLayout->addStretch();

  //newItem( ADDTOMODEL_SYSTEM_TYPE_1,
  //         QString("Packaged Terminal \nAir Conditioner"),
  //         QPixmap(":/images/system_type_1.png") );

  //newItem( ADDTOMODEL_SYSTEM_TYPE_2,
  //         QString("Packaged Terminal Heat Pump"),
  //         QPixmap(":/images/system_type_2.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_3,
           QString("Packaged Rooftop Unit"),
           QPixmap(":/images/system_type_3.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_4,
           QString("Packaged Rooftop Heat Pump"),
           QPixmap(":/images/system_type_4.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_5,
           QString("Packaged DX Rooftop VAV \nwith Reheat"),
           QPixmap(":/images/system_type_5.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_6,
           QString("Packaged Rooftop \nVAV with Parallel Fan \nPower Boxes and reheat"),
           QPixmap(":/images/system_type_6.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_7,
           QString("Packaged Rooftop \nVAV with Reheat"),
           QPixmap(":/images/system_type_7.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_8,
           QString("VAV with Parallel Fan-Powered \nBoxes and Reheat"),
           QPixmap(":/images/system_type_8.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_9,
           QString("Warm Air Furnace \nGas Fired"),
           QPixmap(":/images/system_type_9.png") );

  newItem( ADDTOMODEL_SYSTEM_TYPE_10,
           QString("Warm Air Furnace \nElectric"),
           QPixmap(":/images/system_type_10.png") );

  newItem( ADDTOMODEL_AIRLOOPHVAC,
           QString("Empty Air Loop"),
           QPixmap(":/images/air_loop_icon.png") );

  newItem( ADDTOMODEL_PLANTLOOP,
           QString("Empty Plant Loop"),
           QPixmap(":/images/plant_loop_icon.png") );
}

void LoopLibraryDialog::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void LoopLibraryDialog::onAddToModelClicked( AddToModelEnum addToModelEnum )
{
  m_addToModelEnum = addToModelEnum;

  accept();
}

boost::optional<AddToModelEnum> LoopLibraryDialog::addToModelEnum() const
{
  return m_addToModelEnum;
}

void LoopLibraryDialog::newItem( const AddToModelEnum & addToModelEnum,
              const QString & detailedText,
              const QPixmap & pixmap )
{
  LoopItemView * itemView = new LoopItemView( addToModelEnum,
                                              detailedText,
                                              pixmap );

  connect( itemView,SIGNAL(addToModelClicked( AddToModelEnum )),this,SLOT(onAddToModelClicked( AddToModelEnum )) );

  m_scrollArea->widget()->layout()->addWidget(itemView);

  QFrame * divider = new QFrame();
  divider->setFrameShape(QFrame::HLine);
  divider->setFrameShadow(QFrame::Sunken);

  m_scrollArea->widget()->layout()->addWidget(divider);
}

LoopItemView::LoopItemView( const AddToModelEnum & addToModelEnum,
                            const QString & detailedText,
                            const QPixmap & pixmap,
                            QWidget * parent ) 
  : QWidget( parent ),
    m_addToModelEnum( addToModelEnum ),
    m_detailedText( detailedText ),
    m_pixmap( pixmap )
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();

  setLayout(mainVLayout);

  QLabel * label = new QLabel(detailedText);
  mainVLayout->addWidget(label,0,Qt::AlignLeft);

  QLabel * imageLabel = new QLabel();
  imageLabel->setFixedSize(m_pixmap.size());
  imageLabel->setPixmap(m_pixmap);
  mainVLayout->addWidget(imageLabel,0,Qt::AlignCenter);

  QPushButton * button = new QPushButton();
  button->setText("Add to Model");
  button->setObjectName("StandardGrayButton");
  mainVLayout->addWidget(button,0,Qt::AlignRight);

  connect( button,SIGNAL(clicked()),this,SLOT(emitAddToModelClicked()) );
}

void LoopItemView::emitAddToModelClicked()
{
  emit addToModelClicked( m_addToModelEnum );
}

//void LoopItemView::paintEvent ( QPaintEvent * event )
//{
//  QStyleOption opt;
//  opt.init(this);
//  QPainter p(this);
//  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}

} // namespace openstudio


