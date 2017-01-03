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

#include "MainTabView.hpp"

#include "../shared_gui_components/OSViewSwitcher.hpp"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QStyleOption>

#include "../utilities/core/Assert.hpp"

#include <vector>

static const int TAB_SEPARATION = 10;

namespace openstudio {

MainTabView::MainTabView(const QString & tabLabel, TabType tabType, QWidget * parent)
  : QWidget(parent),
  m_editView(new OSViewSwitcher()),
  m_tabType(tabType)
{
  this->setObjectName("BlueGradientWidget");

  m_tabLabel = new QLabel(tabLabel,this);
  m_tabLabel->setFixedHeight(20);
  m_tabLabel->setStyleSheet("QLabel { color: white; }");
  m_tabLabel->adjustSize();
  m_tabLabel->setFixedWidth(m_tabLabel->width());
  m_tabLabel->move(7,5);

  auto label = new QLabel();
  label->setObjectName("H2");

  m_editView->setView(label);

  m_innerLayout = new QVBoxLayout();
  m_innerLayout->setSpacing(0);
  m_innerLayout->addWidget(m_editView);

  m_mainWidget = new QWidget(this);
  m_mainWidget->setObjectName("MainTabView");
  m_mainWidget->move(7,25);
  m_mainWidget->setLayout(m_innerLayout);

  setTabType(tabType);
}

MainTabView::~MainTabView()
{
  if (m_editView) { delete m_editView; }
}

void MainTabView::setTabType(MainTabView::TabType tabType)
{
  QString style;

  switch (tabType)
  {
  case MAIN_TAB:
    style.append("QWidget#MainTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(0, 0, 0, 0);
    break;
  case SUB_TAB:
    style.append("QWidget#MainTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("  border-top-left-radius: 5px; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(7, 10, 0, 0);
    break;
  case GRIDVIEW_SUB_TAB:
    style.append("QWidget#MainTabView { ");
    style.append("  background: #E6E6E6; ");
    style.append("  border-left: 1px solid black; ");
    style.append("  border-top: 1px solid black; ");
    style.append("  border-top-left-radius: 5px; ");
    style.append("}");
    m_mainWidget->layout()->setContentsMargins(1, 2, 0, 0);
    break;
  default:
    OS_ASSERT(false);
  }

  m_mainWidget->setStyleSheet(style);
}

bool MainTabView::addTabWidget(QWidget * widget)
{
  // This method should only be called in cases where the tab will not have sub tabs
  OS_ASSERT(m_tabType == MAIN_TAB);
  if (m_tabType != MAIN_TAB) return false;

  m_editView->setView(widget);

  return true;
}

bool MainTabView::addSubTab(const QString & subTablabel, int id)
{
  // This method should only be called in cases where the tab will have sub tabs
  OS_ASSERT(m_tabType != MAIN_TAB);
  if (m_tabType == MAIN_TAB) return false;

  auto button = new QPushButton(this);
  button->setText(subTablabel);
  button->setFixedHeight(21);
  m_tabButtons.push_back(button);
  connect(button, &QPushButton::clicked, this, &MainTabView::select);

  m_ids.push_back(id);

  setCurrentIndex(0);
  return true;
}

void MainTabView::setSubTab(QWidget * widget)
{
  m_editView->setView(widget);
}

void MainTabView::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it ){
    if( *it == button ){
      break;
    } else {
      index++;
    }
  } 

  setCurrentIndex(index);
}

void MainTabView::setCurrentIndex(int index)
{
  int xPos = m_tabLabel->width() + TAB_SEPARATION;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {
    QPushButton * button = m_tabButtons[i];
    QString style;

    style.append("QPushButton { border: none; background-color: #BBCDE3; ");
    style.append("              border-right: 1px solid black;");
    style.append("              border-bottom: 1px solid black;");
    style.append("              border-top: 1px solid black;");
    style.append("              border-left: 1px solid black;");
    style.append("              border-top-left-radius: 5px;");
    style.append("              border-top-right-radius: 5px;");
    style.append("              padding-left: 10px;");
    style.append("              padding-right: 10px;");
    style.append("              color: black;");
    style.append("}");

    button->setStyleSheet(style); 
    button->adjustSize();
    button->move(xPos,5);

    button->stackUnder(m_mainWidget);

    xPos += TAB_SEPARATION + button->width();
  }

  QPushButton * button = m_tabButtons[index];

  QString style;

  style.append("QPushButton { border: none; background-color: #E6E6E6; ");
  style.append("              border-right: 1px solid black;");
  style.append("              border-bottom: none;");
  style.append("              border-top: 1px solid black;");
  style.append("              border-left: 1px solid black;");
  style.append("              border-top-left-radius: 5px;");
  style.append("              border-top-right-radius: 5px;");
  style.append("              padding-left: 10px;");
  style.append("              padding-right: 10px;");
  style.append("              color: black;");
  style.append("}");

  button->setStyleSheet(style); 
  button->raise();

  emit tabSelected(m_ids[index]);
}

void MainTabView::setCurrentWidget(QWidget * widget)
{
  OS_ASSERT(false);
}

void MainTabView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MainTabView::resizeEvent( QResizeEvent * event )
{
  QSize size = event->size();  

  QSize newSize(size.width() - 7,size.height() - 25);

  m_mainWidget->resize(newSize);
}

//int MainTabView::subTabId() const
//{
//  OS_ASSERT(false);
//  if (m_tabType != MAIN_TAB)
//  {
//    return m_ids[subTabIndex()];
//  }
//  else
//  {
//    return -1;
//  }
//}

//int MainTabView::subTabIndex() const
//{
//  OS_ASSERT(false);
//  return -1;
//}

bool MainTabView::selectSubTabByIndex(int index)
{
  if (m_tabType != MAIN_TAB){
    if(index < 0){
      return false;
    } else {
      setCurrentIndex(index);
      return true;
    }
  }
  return false;
}

} // namespace openstudio

