/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "VerticalTabWidget.hpp"

#include "MainTabView.hpp"

#include "../shared_gui_components/OSViewSwitcher.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QPushButton>

#include <algorithm>

#include <vector>

namespace openstudio {

VerticalTabWidget::VerticalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  auto mainLayout = new QHBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(5,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setMinimumWidth(39);

  m_tabBar->setMaximumWidth(39);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  m_viewSwitcher = new OSViewSwitcher();

  m_viewSwitcher->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_viewSwitcher);
}

void VerticalTabWidget::addTabButton(int id,
  QString toolTip,
  const QString & selectedImagePath,
  const QString & unSelectedImagePath,
  const QString & disabledImagePath)
{
  auto button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(39, 42));

  button->setToolTip(toolTip);

  m_tabButtons.push_back(button);

  connect(button, &QPushButton::clicked, this, &VerticalTabWidget::select);

  m_selectedPixmaps.push_back(selectedImagePath);

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  m_disabledPixmaps.push_back(disabledImagePath);

  m_ids.push_back(id);
}

void VerticalTabWidget::setView(MainTabView * view, int id)
{
  m_viewSwitcher->setView(view);

  setCurrentIndex(getIndex(id));
}

MainTabView * VerticalTabWidget::view() const
{
  MainTabView * view = qobject_cast<MainTabView *>(m_viewSwitcher->view());
  OS_ASSERT(view);

  return view;
}

void VerticalTabWidget::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it ) {
    if( *it == button ){
      break;
    } else {
      index++;
    }
  } 

  setCurrentIndex(index);
}

int VerticalTabWidget::getIndex(int id)
{
  int index = -1;
  std::vector<int>::iterator it;

  it = std::find(m_ids.begin(),m_ids.end(),id);

  if( it != m_ids.end() ){
    index = it - m_ids.begin();
  }
  OS_ASSERT(index >= 0);
  return index;
}

void VerticalTabWidget::refreshTabButtons()
{
  setCurrentIndex(currentIndex);
}

void VerticalTabWidget::setCurrentIndex(int index)
{
  int yPos = 25;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    button->move(0,yPos);

    yPos = yPos + button->height();

    QString imagePath;

    QString style;

    if (i == static_cast<unsigned>(index))
    {
      imagePath = m_selectedPixmaps[i];

      // Ignore clicks to the already active tab
      if(currentIndex != index){
        currentIndex = index;
        
        style.append("QPushButton { background-color: blue; background-image: url(\"");
        style.append(imagePath);
        style.append("\"); border: none; background-repeat: 0; }");

        button->setStyleSheet(style); 

        currentIndex = index;

        emit tabSelected(m_ids[index]);
      }
    }
    else
    {
      if (button->isEnabled()){
        imagePath = m_unSelectedPixmaps[i];
        style.append("QPushButton { background-image: url(\"");
        style.append(imagePath);
        style.append("\"); border: none; background-color: red; background-repeat: 0; }");
      }
      else {
        imagePath = m_disabledPixmaps[i];
        style.append("QPushButton { background-image: url(\"");
        style.append(imagePath);
        style.append("\"); border: none; background-color: red; background-repeat: 0; }");
      }

      button->setStyleSheet(style); 
    }
  }
}

void VerticalTabWidget::enableTabButton(int id, bool enable)
{
  m_tabButtons.at(getIndex(id))->setEnabled(enable);
}

int VerticalTabWidget::verticalTabIndex()
{
  return currentIndex;
}

} // namespace openstudio
