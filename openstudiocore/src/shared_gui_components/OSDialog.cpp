/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <shared_gui_components/OSDialog.hpp>

#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>

namespace openstudio {

// Note: Window flags are not able to present the user
//       with buttons on the upper right corner

OSDialog::OSDialog(bool isIP,
                   QWidget * parent)
  : QDialog(parent),
  m_upperLayout(NULL),
  m_cancelButton(NULL),
  m_okButton(NULL),
  m_sizeHint(QSize(800,500)),
  m_layoutContentsMargins(QMargins(20,70,20,20)),
  m_isIP(isIP)
{
  setStyleSheet("openstudio--OSDialog { background: #E6E6E6; }");
  
#ifdef Q_WS_MAC
  setWindowFlags(Qt::FramelessWindowHint);
#else
  setWindowFlags(Qt::CustomizeWindowHint);
#endif

  createLayout();
}

void OSDialog::createLayout()
{
  m_upperLayout = new QVBoxLayout();

  QHBoxLayout * lowerLayout = new QHBoxLayout();

  lowerLayout->addStretch();

  bool isConnected = false;

  m_cancelButton = new QPushButton("Cancel",this);
  isConnected = connect(m_cancelButton, SIGNAL(clicked(bool)),
                        this, SLOT(on_cancelButton(bool))) ;
  OS_ASSERT(isConnected);
  isConnected = connect(m_cancelButton, SIGNAL(clicked(bool)),
                        this, SIGNAL(cancelButtonClicked(bool))) ;
  OS_ASSERT(isConnected);
  lowerLayout->addWidget(m_cancelButton);

  m_okButton = new QPushButton("OK",this);
  m_okButton->setDefault(true);
  isConnected = connect(m_okButton, SIGNAL(clicked(bool)),
                        this, SLOT(on_okButton(bool))) ;
  OS_ASSERT(isConnected);
  isConnected = connect(m_okButton, SIGNAL(clicked(bool)),
                        this, SIGNAL(okButtonClicked(bool))) ;
  OS_ASSERT(isConnected);
  lowerLayout->addWidget(m_okButton);

  QVBoxLayout * mainLayout = new QVBoxLayout();
  mainLayout->setContentsMargins(m_layoutContentsMargins);
  mainLayout->addLayout(m_upperLayout);
  mainLayout->addLayout(lowerLayout);

  setLayout(mainLayout);
}

void OSDialog::setOkButtonAsDefault(bool isDefault)
{
  if(isDefault){
    setTabOrder(m_okButton,m_cancelButton);
  }
  else{
    setTabOrder(m_cancelButton,m_okButton);
  }
}

QBoxLayout * OSDialog::upperLayout()
{
  return m_upperLayout;
}

QPushButton * OSDialog::cancelButton()
{
  return m_cancelButton;
}

QPushButton * OSDialog::okButton()
{
  return m_okButton;
}

void OSDialog::resizeEvent(QResizeEvent * event)
{
#ifdef Q_WS_MAC
  QPainterPath path;
  path.addRoundedRect(rect(),9.0,9.0);
  QPolygon p = path.toFillPolygon().toPolygon();
  QRegion region(p);
  setMask(region);
#endif
}

void OSDialog::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);

  QImage leftHeader = QImage(":/images/image_header.png");
  painter.drawImage(0,0,leftHeader);  

  QImage centerHeader = QImage(":/images/header-backgnd-1px-wide.png");

  for(int i = leftHeader.width();i < width();i++ ){
    painter.drawImage(i,0,centerHeader);  
  }
}

void OSDialog::setSizeHint(const QSize & sizeHint)
{
  m_sizeHint = sizeHint;
}

QSize OSDialog::sizeHint() const
{
  return m_sizeHint;
}

QMargins OSDialog::layoutContentsMargins() const
{
  return m_layoutContentsMargins;
}

void OSDialog::setLayoutContentsMargins(const QMargins & layoutContentsMargins)
{
  m_layoutContentsMargins = layoutContentsMargins;
  this->layout()->setContentsMargins(m_layoutContentsMargins);
}

// ***** SLOTS *****

void OSDialog::on_cancelButton(bool checked)
{
  hide();
  setResult(0);
}

void OSDialog::on_okButton(bool checked)
{
  hide();
  setResult(1);
}

void OSDialog::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

