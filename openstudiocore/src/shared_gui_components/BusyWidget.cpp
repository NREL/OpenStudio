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

#include "BusyWidget.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QSize>

/********************************************************************************/
// TODO this class exists in PAT.  Once PAT is in the OpenStudio project,
// the PAT implementation of BusyWidget should be used.
// Note that QPixmap pixmap = QPixmap(":/shared_gui_components/images/rotating_arrow.png").scaled(25,25)
// should not scale-down the image for the OS App application
/********************************************************************************/

#include <QStyleOption>

namespace openstudio {

BusyWidget::BusyWidget(QWidget * parent)
  : QWidget(parent)
{
  //QPixmap pixmap = QPixmap(":/shared_gui_components/images/rotating_arrow.png").scaled(25,25);
  QPixmap pixmap = QPixmap(":/shared_gui_components/images/rotating_arrow.png");

  m_pixmap = std::shared_ptr<QPixmap>(new QPixmap(pixmap));

  m_rotation = 0.0;

  setMinimumSize(m_pixmap->width(),m_pixmap->height());
}

void BusyWidget::rotate()
{
  m_rotation = m_rotation + 5.0;

  update();
}

void BusyWidget::paintEvent ( QPaintEvent * event )
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
  painter.setRenderHint(QPainter::Antialiasing,true);

  int transX = m_pixmap->width() / 2;
  int transY = m_pixmap->height() / 2;

  painter.translate(transX,transY);

  painter.rotate(m_rotation);

  painter.drawPixmap(-transX,-transY,*m_pixmap);
}

const QSize BusyWidget::sizeHint()
{
  return QSize(m_pixmap->width(),m_pixmap->height());
}

} // namespace openstudio
