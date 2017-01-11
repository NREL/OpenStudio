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

#include "OSCollapsibleView.hpp"
#include "OSViewSwitcher.hpp"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

OSHeader::OSHeader(QPushButton * button)
  : QPushButton()
{
  this->setFlat(true);
  toggleButton = button;
}

void OSHeader::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

OSCollapsibleView::OSCollapsibleView(bool alwaysExpanded, QWidget * parent)
  : QWidget(parent),
  m_alwaysExpanded(alwaysExpanded)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);
  
  m_headerContainer = new OSViewSwitcher();
  mainVLayout->addWidget(m_headerContainer);

  m_contentContainer = new OSViewSwitcher();
  mainVLayout->addWidget(m_contentContainer);
  m_contentContainer->hide();
}

void OSCollapsibleView::setHeader(QWidget * header)
{
  m_headerContainer->setView(header);

  m_osHeader = qobject_cast<OSHeader *>(header);

  if( m_osHeader )
  {
    connect(m_osHeader->toggleButton, &QPushButton::toggled, this, &OSCollapsibleView::setExpanded);
  }
}

void OSCollapsibleView::setContent(QWidget * content)
{
  m_contentContainer->setView(content);
}

void OSCollapsibleView::expand()
{
  setExpanded(true);
}

void OSCollapsibleView::collapse()
{
  setExpanded(false);
}

void OSCollapsibleView::setExpanded(bool expanded)
{
  if( m_alwaysExpanded || expanded )
  {
    if( m_osHeader )
    {
      m_osHeader->toggleButton->blockSignals(true);
      m_osHeader->toggleButton->setChecked(true);
      m_osHeader->toggleButton->blockSignals(false);
    }

    m_contentContainer->show();
  }
  else
  {

    if( m_osHeader )
    {
      m_osHeader->toggleButton->blockSignals(true);
      m_osHeader->toggleButton->setChecked(false);
      m_osHeader->toggleButton->blockSignals(false);
    }

    m_contentContainer->hide();
  }

}

bool OSCollapsibleView::isExpanded()
{
  return ! m_contentContainer->isHidden();
}

} // openstudio

