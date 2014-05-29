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

