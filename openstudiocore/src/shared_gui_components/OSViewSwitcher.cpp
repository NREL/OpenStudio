/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "OSViewSwitcher.hpp"

#include <QBoxLayout>
#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

OSViewSwitcher::OSViewSwitcher(QWidget * parent)
  : QWidget(parent),
    m_view(nullptr)
{
  // This is weird. A QWidget with a QVBoxLayout that has a QStackedWidget (which is a convenience Widget that exposes a QStackedLayout)
  auto layout = new QVBoxLayout();
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);

  m_stack = new QStackedWidget();

  layout->addWidget(m_stack);
}

OSViewSwitcher::~OSViewSwitcher()
{
}

void OSViewSwitcher::setView(QWidget * view)
{
  // Evan note: It's bad to play with null pointers
  // Unfortunately, the app crashes if you don't them
  if (!view) {
    //return;
  }
  // If the QStackWidget already has a widget, we remove it
  if( QWidget * widget = m_stack->currentWidget() )
  {
    m_stack->removeWidget( widget );
    // When we determine why there is a crash in Pat frm not
    // using a null pointer, we can again delte these widgets
    //widget->deleteLater();
  }
  OS_ASSERT(m_stack->count() == 0);

  // Store the view
  m_view = view;

  // TODO: this line triggers all of the warnings
  // Warning: QLayout: Attempting to add QLayout "" to openstudio::OSGridView "", which already has a layout ((null):0, (null))
  // qDebug() << m_view << "\n\n";
  // When this happens: m_view => openstudio::SpaceTypesTabView(0x7560610, name="BlueGradientWidget"

  // The QStackedWidget is empty before this function is called, so currentWidget() becomes the current widget.
  // Ownership of widget is passed on to the QStackedWidget.
  m_stack->addWidget(m_view);
}

QWidget * OSViewSwitcher::view() const
{
  return m_view;
}

void OSViewSwitcher::clear()
{
  if( m_view )
  {
    m_stack->removeWidget(m_view);
  }

  m_view = nullptr;
}

void OSViewSwitcher::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

}
