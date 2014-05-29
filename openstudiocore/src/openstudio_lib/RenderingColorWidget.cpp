/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "RenderingColorWidget.hpp"

#include "../model/RenderingColor_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>

namespace openstudio {

RenderingColorWidget::RenderingColorWidget(QWidget * parent )
  : QWidget(parent)
{
  this->setObjectName("GrayWidget");

  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0,0,0,0);
  hLayout->setSpacing(10);
  this->setLayout(hLayout);

  QLabel* renderingColorLabel = new QLabel();
  renderingColorLabel->setText("Rendering Color: ");
  renderingColorLabel->setStyleSheet("QLabel { font: bold; }");
  hLayout->addWidget(renderingColorLabel);

  m_renderColorWidget = new QWidget();
  m_renderColorWidget->setFixedHeight(30);
  m_renderColorWidget->setFixedWidth(30);
  hLayout->addWidget(m_renderColorWidget);

  m_renderColorButton = new QPushButton();
  m_renderColorButton->setFlat(true); 
  m_renderColorButton->setText("Select Color");
  m_renderColorButton->setObjectName("StandardGrayButton");
  hLayout->addWidget(m_renderColorButton);
  hLayout->addStretch();

  bool isConnected = connect(m_renderColorButton, SIGNAL(clicked()), this, SLOT(renderColorButtonClicked()));
  OS_ASSERT(isConnected);
}

void RenderingColorWidget::attach(const openstudio::model::RenderingColor& renderingColor)
{
  detach();

  m_renderingColor = renderingColor;

  bool isConnected = connect(m_renderingColor->getImpl<model::detail::ModelObject_Impl>().get(),
                        SIGNAL(onChange()),
                        this, 
                        SLOT(refresh()));
  OS_ASSERT(isConnected);

  refresh();
}

void RenderingColorWidget::detach()
{
  clear();

  if (m_renderingColor){
    this->disconnect(m_renderingColor->getImpl<model::detail::ModelObject_Impl>().get());
    m_renderingColor.reset();
  }
}

void RenderingColorWidget::clear()
{
  QString style = "QWidget { background-color : rgba(255,255,255,255);}";
  m_renderColorWidget->setStyleSheet(style);
  m_renderColorButton->setEnabled(false);
}

void RenderingColorWidget::refresh()
{
  clear();

  if (m_renderingColor){

    int r = m_renderingColor->renderingRedValue();
    int g = m_renderingColor->renderingGreenValue();
    int b = m_renderingColor->renderingBlueValue();
    int a = m_renderingColor->renderingAlphaValue();
    QString style = "QWidget { background-color : rgba(" + QString::number(r) + "," + QString::number(g) + ", " + QString::number(b) + ", " + QString::number(a) + ");}";
    m_renderColorWidget->setStyleSheet(style);
    m_renderColorButton->setEnabled(true);
  }
}

void RenderingColorWidget::renderColorButtonClicked()
{
  if(m_renderingColor){
    int r = m_renderingColor->renderingRedValue();
    int g = m_renderingColor->renderingGreenValue();
    int b = m_renderingColor->renderingBlueValue();
    int a = m_renderingColor->renderingAlphaValue();
    QColor initialColor = QColor(r, g, b, a);
  
    QColor color = QColorDialog::getColor(initialColor, this, "Choose Rendering Color", QColorDialog::ShowAlphaChannel);

    if (color.isValid()){
      m_renderingColor->setRenderingRedValue(color.red());
      m_renderingColor->setRenderingGreenValue(color.green());
      m_renderingColor->setRenderingBlueValue(color.blue());
      m_renderingColor->setRenderingAlphaValue(color.alpha());

      refresh();
    }
  }
}

} // openstudio

