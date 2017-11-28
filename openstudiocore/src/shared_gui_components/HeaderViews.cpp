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

#include "HeaderViews.hpp"
#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>

#include "../utilities/bcl/BCLMeasure.hpp"

namespace openstudio{
  
LightHeaderToggleButton::LightHeaderToggleButton(QWidget * parent)
  : QPushButton(parent)
{
  setCheckable(true);

  setFixedSize(11,11);

  QString style;
  style.append("QPushButton { border: none; ");
  style.append("background-image: url(\":/shared_gui_components/images/toggle_arrow_closed.png\"); ");
  style.append(" } ");
  style.append("QPushButton:checked { border: none; ");
  style.append("background-image: url(\":/shared_gui_components/images/toggle_arrow.png\"); ");
  style.append(" } ");
  
  setStyleSheet(style);
}

HeaderToggleButton::HeaderToggleButton(QWidget * parent)
  : QPushButton(parent)
{
  setCheckable(true);

  setFixedSize(11,11);

  QString style;
  style.append("QPushButton { border: none; ");
  style.append("background-image: url(\":/shared_gui_components/images/toggle_arrow_closed.png\"); ");
  style.append(" } ");
  style.append("QPushButton:checked { border: none; ");
  style.append("background-image: url(\":/shared_gui_components/images/toggle_arrow.png\"); ");
  style.append(" } ");
  
  setStyleSheet(style);
}

DarkGradientHeader::DarkGradientHeader(QWidget * parent)
  : OSHeader(new LightHeaderToggleButton())
{
  setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
  setFixedHeight(30);

  QString style;
  style.append("openstudio--DarkGradientHeader { ");
  style.append("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, ");
  style.append("stop: 0 #575757, stop: 0.2 #6C6C6C, stop: 1 #393939); ");
  style.append("} ");
  setStyleSheet(style);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,0,5,0);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  mainHLayout->addWidget(toggleButton);

  label = new QLabel();
  style.clear();
  style.append("QLabel { ");
  style.append("font: bold; ");
  style.append("color: white; ");
  style.append("} ");
  label->setStyleSheet(style);
  mainHLayout->addWidget(label);
}

LightGradientHeader::LightGradientHeader(QWidget * parent)
  : OSHeader(new HeaderToggleButton())
{
  setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
  setFixedHeight(30);

  QString style;
  style.append("openstudio--LightGradientHeader { ");
  style.append("background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, ");
  style.append("stop: 0 #E5E5E5, stop: 1 #807E7E); ");
  style.append("} ");
  setStyleSheet(style);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,0,5,0);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  mainHLayout->addWidget(toggleButton);

  m_measureTypeBadge = new QLabel();
  m_measureTypeBadge->setFixedSize(25,25);
  m_measureTypeBadge->setVisible(false);
  mainHLayout->addWidget(m_measureTypeBadge);

  label = new QLabel();
  style.clear();
  style.append("QLabel { ");
  style.append("font: bold; ");
  style.append("} ");
  label->setStyleSheet(style);
  mainHLayout->addWidget(label);
}

void LightGradientHeader::setMeasureType(MeasureType measureType)
{
  bool found = false;

  if (measureType == MeasureType::ModelMeasure){
    m_measureTypeBadge->setPixmap(QPixmap(":/shared_gui_components/images/openstudio_measure_icon.png").scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    found = true;
  }else if (measureType == MeasureType::EnergyPlusMeasure){
    m_measureTypeBadge->setPixmap(QPixmap(":/shared_gui_components/images/energyplus_measure_icon.png").scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    found = true;
  }else if (measureType == MeasureType::ReportingMeasure){
    m_measureTypeBadge->setPixmap(QPixmap(":/shared_gui_components/images/report_measure_icon.png").scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    found = true;
  }

  m_measureTypeBadge->setVisible(found);
}

LightHeader::LightHeader(QWidget * parent)
  : OSHeader(new HeaderToggleButton())
{
  setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
  setFixedHeight(30);

  QString style;
  style.append("openstudio--LightHeader { ");
  style.append("background: #E0E0E0; ");
  style.append("} ");
  setStyleSheet(style);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,0,5,0);
  setLayout(mainHLayout);

  mainHLayout->addSpacing(15);

  mainHLayout->addWidget(toggleButton);

  label = new QLabel();
  style.clear();
  style.append("QLabel { ");
  style.append("font: bold; ");
  style.append("} ");
  label->setStyleSheet(style);
  mainHLayout->addWidget(label);
}

} // openstudio

