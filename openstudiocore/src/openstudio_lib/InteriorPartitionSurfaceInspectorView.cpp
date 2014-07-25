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

#include "InteriorPartitionSurfaceInspectorView.hpp"
#include "PlanarSurfaceWidget.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSSwitch.hpp"
#include "OSVectorController.hpp"
#include "OSDropZone.hpp"

#include "../model/InteriorPartitionSurface.hpp"
#include "../model/InteriorPartitionSurface_Impl.hpp"
#include "../model/Construction.hpp"

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
  class InteriorPartitionSurfaceConstructionVectorController : public OSVectorController
  {
  public:
    InteriorPartitionSurfaceConstructionVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : OSVectorController(model, parent)
    {}

    void setInteriorPartitionSurface(const model::InteriorPartitionSurface& interiorPartitionSurface)
    {
      m_interiorPartitionSurface = interiorPartitionSurface;
    }

    void clearInteriorPartitionSurface()
    {
      m_interiorPartitionSurface.reset();
    }

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_interiorPartitionSurface){
        boost::optional<model::ConstructionBase> construction = m_interiorPartitionSurface->construction();
        if (construction){
          bool isDefaulted = m_interiorPartitionSurface->isConstructionDefaulted();
          m_objects.push_back(OSVectorControllerStruct(*construction, isDefaulted));
        }
      }
    }

  private:
    boost::optional<model::InteriorPartitionSurface> m_interiorPartitionSurface;
  };
*/
InteriorPartitionSurfaceInspectorView::InteriorPartitionSurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
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

  // construction
  vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Construction: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_constructionVectorController = new InteriorPartitionSurfaceConstructionVectorController(model, this);
  //m_constructionDropZone = new OSDropZone(model, m_constructionVectorController, "Construction: ");
  //vLayout->addWidget(m_constructionDropZone);

  mainGridLayout->addLayout(vLayout,1,0,1,2);

  // surface area and convert to internal mass
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Surface Area: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  bool isConnected = false;

  m_surfaceAreaEdit = new OSQuantityEdit(this);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_surfaceAreaEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_surfaceAreaEdit);

  mainGridLayout->addLayout(vLayout,2,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Convert to Internal Mass: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_convertToInternalMassSwitch = new OSSwitch(this);
  vLayout->addWidget(m_convertToInternalMassSwitch);

  mainGridLayout->addLayout(vLayout,2,1, Qt::AlignTop|Qt::AlignLeft);

  // separator
  QWidget * hLine = new QWidget();
  hLine->setObjectName("HLine");
  hLine->setStyleSheet("QWidget#HLine { background: #445051;}");
  hLine->setFixedHeight(2);

  mainGridLayout->addWidget(hLine,3,0,1,2);

  // planar surface widget
  m_planarSurfaceWidget = new PlanarSurfaceWidget(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_surfaceAreaEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);

  mainGridLayout->addWidget(m_planarSurfaceWidget,4,0,1,2);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnMinimumWidth(2, 80);
  mainGridLayout->setColumnStretch(3,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowMinimumHeight(3, 30);
  mainGridLayout->setRowMinimumHeight(4, 30);
  mainGridLayout->setRowStretch(5,1);
}

void InteriorPartitionSurfaceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void InteriorPartitionSurfaceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::InteriorPartitionSurface interiorPartitionSurface = modelObject.cast<model::InteriorPartitionSurface>();
  attach(interiorPartitionSurface);
  refresh();
}

void InteriorPartitionSurfaceInspectorView::onUpdate()
{
  refresh();
}

void InteriorPartitionSurfaceInspectorView::attach(openstudio::model::InteriorPartitionSurface& interiorPartitionSurface)
{
  m_nameEdit->bind(interiorPartitionSurface, "name");

  //m_constructionVectorController->setInteriorPartitionSurface(interiorPartitionSurface);
  m_planarSurfaceWidget->attach(interiorPartitionSurface);

  this->stackedWidget()->setCurrentIndex(1);
}

void InteriorPartitionSurfaceInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  //m_constructionVectorController->clearInteriorPartitionSurface();
  m_planarSurfaceWidget->detach();
}

void InteriorPartitionSurfaceInspectorView::refresh()
{
}

void InteriorPartitionSurfaceInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

