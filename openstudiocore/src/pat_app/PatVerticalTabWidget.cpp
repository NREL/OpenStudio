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

#include "PatVerticalTabWidget.hpp"

#include "../shared_gui_components/OSViewSwitcher.hpp"

#include "../utilities/core/Assert.hpp"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QKeySequence>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QShortcut>
#include <QTimer>
#include <QVBoxLayout>

#include <algorithm>
#include <vector>

namespace openstudio {
  
namespace pat {

PatVerticalTabWidget::PatVerticalTabWidget(QWidget * parent)
  : QWidget(parent)
{
  m_buttonGroup = new QButtonGroup(this);
  m_buttonGroup->setExclusive(true);

  connect(m_buttonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), this, &PatVerticalTabWidget::on_buttonClicked);

  auto mainLayout = new QHBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(5,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setMinimumWidth(39);

  m_tabBar->setMaximumWidth(39);

  m_tabBar->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_tabBar);

  mainViewSwitcher = new OSViewSwitcher();

  layout()->addWidget(mainViewSwitcher);

  currentIndex = -1;

  QShortcut* nextTabShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab), this);
  connect(nextTabShortcut, &QShortcut::activated, this, &PatVerticalTabWidget::nextTab);

  QShortcut* previousTabShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Tab), this);
  connect(previousTabShortcut, &QShortcut::activated, this, &PatVerticalTabWidget::previousTab);
}

PatVerticalTabWidget::~PatVerticalTabWidget() 
{ 
  if (mainViewSwitcher){
    mainViewSwitcher->clear(); 
  }
}

void PatVerticalTabWidget::addTab( int id,
                                   QString toolTip,
                                   const QString & selectedImagePath,
                                   const QString & unSelectedImagePath,
                                   const QString & disabledImagePath,
                                   const QString & doneImagePath,
                                   bool enabled )
{
  auto button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(39,42));

  button->setToolTip(toolTip);

  button->setCheckable(true);

  button->setEnabled(enabled);

  m_buttonGroup->addButton(button);
  m_buttonGroup->setId(button,m_buttonGroup->buttons().size() - 1);

  m_selectedPixmaps.push_back(selectedImagePath); 

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  m_disabledPixmaps.push_back(disabledImagePath);

  m_donePixmaps.push_back(doneImagePath);

  setCurrentIndex(0);
}

void PatVerticalTabWidget::enableTab(bool enable, int index)
{
  OS_ASSERT(index >= 0);
  OS_ASSERT(index < (int)m_buttonGroup->buttons().size());

  // Ignore disabling disabled tabs, and enabling enabled tabs
  if (enable == m_buttonGroup->button(index)->isEnabled())
  {
    return;
  }

  m_buttonGroup->button(index)->setEnabled(enable);
  QTimer::singleShot(0, this, SLOT(refreshIcons()));
}

void PatVerticalTabWidget::enableTabs(bool enable)
{
  for(int i=0; i<m_buttonGroup->buttons().size(); i++)
  {
    enableTab(enable,i);
  }
}

void PatVerticalTabWidget::selectTab(int id)
{
  //TODO m_buttonGroup->button(id)->setChecked(true);
  m_buttonGroup->button(id)->click();
}

void PatVerticalTabWidget::selectNone()
{
  for(int i = 0; i < m_buttonGroup->buttons().size(); i++)
  {  
    QAbstractButton * button = m_buttonGroup->button(i);

    button->setChecked(false);

    currentIndex = -1;
  }
  QTimer::singleShot(0, this, SLOT(refreshIcons()));
}

void PatVerticalTabWidget::nextTab()
{
  int size = m_buttonGroup->buttons().size(),
      nextIndex = (currentIndex + 1) % size;
  for(int i = 0; i < size; i++)
  {
    if (!m_buttonGroup->button(nextIndex)->isEnabled()){
      nextIndex = (nextIndex + 1) % size;
    }else{
      selectTab(nextIndex);
    }
  }
}

void PatVerticalTabWidget::previousTab()
{
  int size = m_buttonGroup->buttons().size(),
      previousIndex = (((currentIndex < 0 ? -1 : currentIndex - 1) % size) + size) % size;
  for(int i = 0; i < size; i++)
  {
    if (!m_buttonGroup->button(previousIndex)->isEnabled()){
      previousIndex = (((previousIndex - 1) % size) + size) % size;
    }else{
      selectTab(previousIndex);
    }
  }
}

void PatVerticalTabWidget::setCurrentIndex(int index)
{
  int yPos = 25;
  
  for(int i = 0; i < m_buttonGroup->buttons().size(); i++)
  {
    QAbstractButton * button = m_buttonGroup->button(i);

    button->move(0,yPos);

    yPos += button->height();

    // Ignore clicks to the already active tab
    if(button->isEnabled() && button->isChecked() && currentIndex != index){
      currentIndex = index;
      emit tabSelected(i);
    }
  }
  QTimer::singleShot(0, this, SLOT(refreshIcons()));
}

void PatVerticalTabWidget::refreshIcons()
{
  for(int i = 0; i < m_buttonGroup->buttons().size(); i++)
  {
    // TODO "done" not handled
    //imagePath = m_donePixmaps[i]; 

    QAbstractButton * button = m_buttonGroup->button(i);
  
    QString imagePath;
    if(button->isEnabled() && button->isChecked()){
      imagePath = m_selectedPixmaps[i];
    }
    else if(button->isEnabled() && !button->isChecked()){
      imagePath = m_unSelectedPixmaps[i];
    }
    else if(!button->isEnabled() && button->isChecked()){
      imagePath = m_selectedPixmaps[i];
    }
    else if(!button->isEnabled() && !button->isChecked()){
      imagePath = m_disabledPixmaps[i];
    }
    else{
      // you should not be here
      OS_ASSERT(false);
    }

    QString style;
    style.append("QPushButton { background-color: blue; background-image: url(\"");
    style.append(imagePath);
    style.append("\"); border: none; background-repeat: 0; }");

    button->setStyleSheet(style);
  }
}

//////////////////////////////// SLOTS ///////////////////////////////

void PatVerticalTabWidget::on_buttonClicked(int id)
{
  setCurrentIndex(id);
}

} // namespace pat

}

