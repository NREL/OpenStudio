/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "PatMainTabView.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>

#include <vector>

namespace openstudio {
  
namespace pat { 

PatMainTabView::PatMainTabView()
  : QWidget()
{
  this->setObjectName("OrangeGradientWidget");

  m_titleLabel = new QLabel(this);
  m_titleLabel->setFixedHeight(20);
  m_titleLabel->setStyleSheet("QLabel { font: bold; }");
  m_titleLabel->move(7,5);

  m_mainWidget = new QWidget(this);
  m_mainWidget->setObjectName("MainTabView");
  QString style;
  style.append("QWidget#MainTabView { " );
  style.append("  background: #E6E6E6; " );
  style.append("  border-left: 1px solid black; " );
  style.append("  border-top: 1px solid black; " );
  style.append("}" );
  m_mainWidget->setStyleSheet(style);

  m_mainWidget->move(7,30);

  auto innerLayout = new QVBoxLayout();
  innerLayout->setContentsMargins(1,1,0,0);
  innerLayout->setSpacing(0);
  m_mainWidget->setLayout(innerLayout);

  viewSwitcher = new OSViewSwitcher(); 
  innerLayout->addWidget(viewSwitcher);
}

void PatMainTabView::setTitle(const QString & title)
{
  m_titleLabel->setText(title);
  m_titleLabel->adjustSize();
}

void PatMainTabView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void PatMainTabView::resizeEvent( QResizeEvent * event )
{
  QSize size = event->size();  

  QSize newSize(size.width() - 7,size.height() - 30);

  m_mainWidget->resize(newSize);
}

} // namespace pat

}

