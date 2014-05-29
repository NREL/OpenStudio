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

#include "ShadingSurfaceInspectorView.hpp"
#include "PlanarSurfaceWidget.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "OSVectorController.hpp"
#include "OSDropZone.hpp"

#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Schedule.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {
/*
  class ShadingSurfaceConstructionVectorController : public OSVectorController
  {
  public:
    ShadingSurfaceConstructionVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : OSVectorController(model, parent)
    {}

    void setShadingSurface(const model::ShadingSurface& shadingSurface)
    {
      m_shadingSurface = shadingSurface;
    }

    void clearShadingSurface()
    {
      m_shadingSurface.reset();
    }

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_shadingSurface){
        boost::optional<model::ConstructionBase> construction = m_shadingSurface->construction();
        if (construction){
          bool isDefaulted = m_shadingSurface->isConstructionDefaulted();
          m_objects.push_back(OSVectorControllerStruct(*construction, isDefaulted));
        }
      }
    }

  private:
    boost::optional<model::ShadingSurface> m_shadingSurface;
  };

class ShadingSurfaceTransmittanceScheduleVectorController : public OSVectorController
  {
  public:
    ShadingSurfaceTransmittanceScheduleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : OSVectorController(model, parent)
    {}

    void setShadingSurface(const model::ShadingSurface& shadingSurface)
    {
      m_shadingSurface = shadingSurface;
    }

    void clearShadingSurface()
    {
      m_shadingSurface.reset();
    }

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_shadingSurface){
        boost::optional<model::Schedule> schedule = m_shadingSurface->transmittanceSchedule();
        if (schedule){
          m_objects.push_back(*schedule);
        }
      }
    }

  private:
    boost::optional<model::ShadingSurface> m_shadingSurface;
  };
*/
ShadingSurfaceInspectorView::ShadingSurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name
  QVBoxLayout* vLayout = new QVBoxLayout();

  QLabel* label = new QLabel();
  label->setText("Name: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  mainGridLayout->addLayout(vLayout,0,0,1,2, Qt::AlignTop);

  // construction and transmittance schedule
  vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Construction: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_constructionVectorController = new ShadingSurfaceConstructionVectorController(model, this);
  //m_constructionDropZone = new OSDropZone(model, m_constructionVectorController, "Construction: ");
  //vLayout->addWidget(m_constructionDropZone);

  mainGridLayout->addLayout(vLayout,1,0);

  vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Transmittance Schedule: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_transmittanceScheduleVectorController = new ShadingSurfaceTransmittanceScheduleVectorController(model, this);
  //m_transmittanceScheduleDropZone = new OSDropZone(model, m_transmittanceScheduleVectorController, "Transmittance Schedule: ");
  //vLayout->addWidget(m_transmittanceScheduleDropZone);

  mainGridLayout->addLayout(vLayout,1,1);

  // separator
  QWidget * hLine = new QWidget();
  hLine->setObjectName("HLine");
  hLine->setStyleSheet("QWidget#HLine { background: #445051;}");
  hLine->setFixedHeight(2);

  mainGridLayout->addWidget(hLine,2,0,1,2);

  // planar surface widget
  m_planarSurfaceWidget = new PlanarSurfaceWidget(m_isIP);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_planarSurfaceWidget, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  mainGridLayout->addWidget(m_planarSurfaceWidget,3,0,1,2);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnMinimumWidth(2, 80);
  mainGridLayout->setColumnStretch(3,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowMinimumHeight(3, 30);
  mainGridLayout->setRowStretch(4,1);
}

void ShadingSurfaceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void ShadingSurfaceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::ShadingSurface shadingSurface = modelObject.cast<model::ShadingSurface>();
  attach(shadingSurface);
  refresh();
}

void ShadingSurfaceInspectorView::onUpdate()
{
  refresh();
}

void ShadingSurfaceInspectorView::attach(openstudio::model::ShadingSurface& shadingSurface)
{
  m_nameEdit->bind(shadingSurface, "name");

  //m_constructionVectorController->setShadingSurface(shadingSurface);
  m_planarSurfaceWidget->attach(shadingSurface);

  this->stackedWidget()->setCurrentIndex(1);
}

void ShadingSurfaceInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  //m_constructionVectorController->clearShadingSurface();
  m_planarSurfaceWidget->detach();
}

void ShadingSurfaceInspectorView::refresh()
{
}

void ShadingSurfaceInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

