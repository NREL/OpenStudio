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

#include "LibraryTabWidget.hpp"

#include <QBoxLayout>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>

#include "../utilities/core/Assert.hpp"

#include <vector>

namespace openstudio {

LibraryTabWidget::LibraryTabWidget(QWidget * parent)
  : QWidget(parent)
{
  auto mainLayout = new QVBoxLayout();

  mainLayout->setSpacing(0);

  mainLayout->setContentsMargins(0,0,0,0);

  setLayout(mainLayout);

  m_tabBar = new QWidget(this);

  m_tabBar->setFixedHeight(29);

  m_tabBar->setObjectName("VBlueGradientWidget");

  m_tabBar->setContentsMargins(0,0,5,0);

  m_removeButton = new QPushButton(this);

  QString str;
  str.append("QWidget { ");
  str.append("border: none;");
  str.append(" background-image: url(\":/images/delete.png\")");
  str.append("}");

  m_removeButton->setFlat(true);
  m_removeButton->setStyleSheet(str);
  m_removeButton->setFixedSize(20, 20);
  m_removeButton->setToolTip("Remove object");

  // Default to hide
  m_removeButton->hide();

  auto isConnected = connect(m_removeButton, SIGNAL(clicked(bool)), this, SIGNAL(removeButtonClicked(bool)));
  OS_ASSERT(isConnected);
 
  auto hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0, 0, 0, 0);
  hLayout->addStretch();
  hLayout->addWidget(m_removeButton, 0, Qt::AlignVCenter);

  m_tabBar->setLayout(hLayout);

  layout()->addWidget(m_tabBar);

  m_pageStack = new QStackedWidget();

  m_pageStack->setStyleSheet("QStackedWidget { border: none;}");

  m_pageStack->setContentsMargins(0,0,0,0);

  layout()->addWidget(m_pageStack);
}

void LibraryTabWidget::showRemoveButton()
{
  m_removeButton->show();
}

void LibraryTabWidget::hideRemoveButton()
{
  m_removeButton->hide();
}

void LibraryTabWidget::addTab( QWidget * widget,
                        const QString & selectedImagePath,
                        const QString & unSelectedImagePath )
{
  auto button = new QPushButton(m_tabBar);

  button->setFixedSize(QSize(29,29));

  m_tabButtons.push_back(button);

  connect(button, &QPushButton::clicked, this, &LibraryTabWidget::select);

  m_pageStack->addWidget(widget);

  m_selectedPixmaps.push_back(selectedImagePath); 

  m_unSelectedPixmaps.push_back(unSelectedImagePath);

  setCurrentIndex(0);
}

void LibraryTabWidget::select()
{
  QPushButton * button = qobject_cast<QPushButton *>(sender());

  int index = 0;

  for( auto it = m_tabButtons.begin();
       it < m_tabButtons.end();
       ++it )
  {
    if( *it == button )
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

void LibraryTabWidget::setCurrentIndex(int index)
{
  int xPos = 0;

  for(unsigned i = 0; i < m_tabButtons.size(); i++)
  {  
    QPushButton * button = m_tabButtons[i];

    button->move(xPos,0);

    xPos = xPos + button->width();

    QString imagePath = m_unSelectedPixmaps[i]; 

    QString style;

    style.append("QPushButton { background: none; background-image: url(\"");
    style.append(imagePath);
    style.append("\"); border: none; }");

    button->setStyleSheet(style); 
  }

  QPushButton * button = m_tabButtons[index];

  QString imagePath = m_selectedPixmaps[index]; 

  QString style;

  style.append("QPushButton { background: none; background-image: url(\"");
  style.append(imagePath);
  style.append("\"); border: none; background-repeat: 0; }");

  button->setStyleSheet(style); 

  m_pageStack->setCurrentIndex(index);
}

void LibraryTabWidget::setCurrentWidget(QWidget * widget)
{
  int i = m_pageStack->indexOf(widget);

  setCurrentIndex(i);
}

} // namespace openstudio

