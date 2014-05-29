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

#include "PlanarSurfaceWidget.hpp"

#include "../model/PlanarSurface_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>

#define FEET_PER_METER 3.280839895013123

namespace openstudio {

PlanarSurfaceWidget::PlanarSurfaceWidget(bool isIP, QWidget * parent )
  : QWidget(parent)
{
  this->setObjectName("GrayWidget");

  setUnits(isIP);

  QGridLayout* layout = new QGridLayout();
  layout->setContentsMargins(7,7,7,7);
  layout->setSpacing(7);
  this->setLayout(layout);
}

void PlanarSurfaceWidget::attach(const openstudio::model::PlanarSurface& planarSurface)
{
  detach();

  m_planarSurface = planarSurface;

  bool isConnected = connect(m_planarSurface->getImpl<model::detail::ModelObject_Impl>().get(),
                             SIGNAL(onChange()),
                             this, 
                             SLOT(refresh()));
  OS_ASSERT(isConnected);

  refresh();
}

void PlanarSurfaceWidget::detach()
{
  clear();

  if (m_planarSurface){
    this->disconnect(m_planarSurface->getImpl<model::detail::ModelObject_Impl>().get());
    m_planarSurface.reset();
  }
}

void PlanarSurfaceWidget::clear()
{
  QLayoutItem *child;
  while ((child = this->layout()->takeAt(0)) != 0) {
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }
}

void PlanarSurfaceWidget::refresh()
{
  clear();

  if (m_planarSurface){

    double multiplicationFactor = 1;
    QString units;
    if(m_units == "SI"){
      units = "m";
    }
    else if(m_units == "IP"){
      units = "ft";
      multiplicationFactor = FEET_PER_METER;
    }

    QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());

    QLabel* label = new QLabel();
    label->setText("Number");
    label->setStyleSheet("QLabel { font: bold; }");
    layout->addWidget(label,0,0);

    QString string;

    string = "x (";
    string += units;
    string += ")";

    label = new QLabel();
    label->setText(string);
    label->setStyleSheet("QLabel { font: bold; }");
    layout->addWidget(label,0,1);

    string = "y (";
    string += units;
    string += ")";

    label = new QLabel();
    label->setText(string);
    label->setStyleSheet("QLabel { font: bold; }");
    layout->addWidget(label,0,2);

    string = "z (";
    string += units;
    string += ")";

    label = new QLabel();
    label->setText(string);
    label->setStyleSheet("QLabel { font: bold; }");
    layout->addWidget(label,0,3);

    std::vector<openstudio::Point3d> vertices = m_planarSurface->vertices();
    for (unsigned i = 0; i < vertices.size(); ++i){

      int n = i+1;

      label = new QLabel();
      label->setText(QString::number(n));
      layout->addWidget(label,n,0);

      label = new QLabel();
      label->setText(QString::number(vertices[i].x()*multiplicationFactor));
      layout->addWidget(label,n,1);

      label = new QLabel();
      label->setText(QString::number(vertices[i].y()*multiplicationFactor));
      layout->addWidget(label,n,2);

      label = new QLabel();
      label->setText(QString::number(vertices[i].z()*multiplicationFactor));
      layout->addWidget(label,n,3);

    }
  }
}

void PlanarSurfaceWidget::setUnits(bool displayIP)
{
  if(displayIP){
    m_units = "IP";
  }
  else{
    m_units = "SI";
  }
}

void PlanarSurfaceWidget::toggleUnits(bool displayIP)
{
  setUnits(displayIP);

  refresh();
}

} // openstudio

