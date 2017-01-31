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

  auto layout = new QGridLayout();
  layout->setContentsMargins(7,7,7,7);
  layout->setSpacing(7);
  this->setLayout(layout);
}

void PlanarSurfaceWidget::attach(const openstudio::model::PlanarSurface& planarSurface)
{
  detach();

  m_planarSurface = planarSurface;

  m_planarSurface->getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<PlanarSurfaceWidget, &PlanarSurfaceWidget::refresh>(this);

  refresh();
}

void PlanarSurfaceWidget::detach()
{
  clear();

  if (m_planarSurface){
    // disconnect(m_planarSurface->getImpl<model::detail::ModelObject_Impl>().get());
    m_planarSurface->getImpl<model::detail::ModelObject_Impl>().get()->onChange.disconnect<PlanarSurfaceWidget, &PlanarSurfaceWidget::refresh>(this);
    m_planarSurface.reset();
  }
}

void PlanarSurfaceWidget::clear()
{
  QLayoutItem *child;
  while ((child = this->layout()->takeAt(0)) != nullptr) {
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

    auto label = new QLabel();
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

