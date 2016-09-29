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

#include "HorizontalTabWidget.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QStyleOption>

#include <algorithm>
#include <vector>

namespace openstudio {
  
namespace pat { 

HorizontalTabWidget::HorizontalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  auto mainLayout = new QVBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(0,0,0,0);

  setLayout(mainLayout);

  // Tab bar

  m_tabBar = new QWidget(this);

  m_tabBar->setFixedHeight(31);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  // Tab bar line

  m_tabBarLine = new QWidget(m_tabBar);

  m_tabBarLine->setFixedHeight(5);

  m_tabBarLine->move(0,m_tabBar->height() - 5);

  // NOTE: Yellow Orange Lighter #fdbc3b and the Darker Orange is #f47920
  m_tabBarLine->setStyleSheet("QWidget { border-top: 1px solid black; background-color: #f47920; }");

  mainViewSwitcher = new OSViewSwitcher();
  mainViewSwitcher->setObjectName("HorizontalTabWidget");
  mainViewSwitcher->setStyleSheet("QWidget#HorizontalTabWidget { background-color: #E0E0E0; }");
  

  layout()->addWidget(mainViewSwitcher);
}


void HorizontalTabWidget::addTab( int id,
                                  const QString & label )
{
  auto button = new QPushButton(m_tabBar);

  button->setText(label);

  button->setFixedHeight(27);

  m_tabButtons.push_back(button);

  connect(button, &QPushButton::clicked, this, &HorizontalTabWidget::select);

  m_ids.push_back(id);

  setCurrentIndex(0);
}

void HorizontalTabWidget::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( const auto & tabButton : m_tabButtons )
  {
    if( tabButton == button )
    {
      break;
    }
    else
    {
      index++;
    }
  } 

  setCurrentIndex(index);
}

void HorizontalTabWidget::setCurrentId(int id)
{
  std::vector<int>::iterator it;

  it = std::find(m_ids.begin(),m_ids.end(),id);

  if( it != m_ids.end() )
  {
    setCurrentIndex(*it);
  }
}

void HorizontalTabWidget::setCurrentIndex(int index)
{
  int xPos = 0;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    QString style;

    style.append("QPushButton { border: none; background-color: #808080; ");
    style.append("              border-right: 1px solid black;");
    style.append("              border-bottom: 1px solid black;");
    style.append("              padding-left: 10px;");
    style.append("              padding-right: 10px;");
    style.append("              padding-top: 5px;");
    style.append("              font: bold;");
    if( i == m_tabButtons.size() - 1 )
    {
      style.append("            border-right: none;");
    }
    style.append("}");

    button->setStyleSheet(style); 

    button->adjustSize();

    button->move(xPos,0);

    xPos = xPos + button->width();
  }

  m_tabBarLine->setFixedWidth(xPos);
  m_tabBarLine->raise();

  QPushButton * button = m_tabButtons[index];

  QString style;

  // NOTE: Yellow Orange Lighter #fdbc3b and the Darker Orange is #f47920
  style.append("QPushButton { border: none; background-color: #f47920; ");
  style.append("              border-right: 1px solid black;");
  style.append("              padding-left: 10px;");
  style.append("              padding-right: 10px;");
  style.append("              padding-top: 5px;");
  style.append("              font: bold;");
  if( index == int(m_tabButtons.size() - 1) )
  {
    style.append("            border-right: none;");
  }
  style.append("}");

  button->setStyleSheet(style); 
  button->raise();

  emit tabSelected(m_ids[index]);
}

void HorizontalTabWidget::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

} // namespace pat

}

