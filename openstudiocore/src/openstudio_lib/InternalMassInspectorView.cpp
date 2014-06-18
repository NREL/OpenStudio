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

#include "InternalMassInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/InternalMassDefinition_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_InternalMass_Definition_FieldEnums.hxx>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

InternalMassDefinitionInspectorView::InternalMassDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // Surface Area

  label = new QLabel("Surface Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  bool isConnected = false;

  m_surfaceAreaEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_surfaceAreaEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_surfaceAreaEdit,3,0);

  // Surface Area Per Space Floor Area

  label = new QLabel("Surface Area Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_surfaceAreaPerSpaceFloorAreaEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_surfaceAreaPerSpaceFloorAreaEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_surfaceAreaPerSpaceFloorAreaEdit,3,1);

  // Surface Area Per Person

  label = new QLabel("Surface Area Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_surfaceAreaPerPersonEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_surfaceAreaPerPersonEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_surfaceAreaPerPersonEdit,3,2);

  // Construction

  label = new QLabel("Construction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_ConstructionVectorController = new ConstructionVectorController();

  m_ConstructionDropZone = new OSDropZone(m_ConstructionVectorController);
  m_ConstructionDropZone->setMinItems(0);
  m_ConstructionDropZone->setMaxItems(1);
  m_ConstructionDropZone->setItemsAcceptDrops(true);
  mainGridLayout->addWidget(m_ConstructionDropZone,5,0,1,2);

  // Stretch

  mainGridLayout->setRowStretch(6,100);

  mainGridLayout->setColumnStretch(3,100);
}

void InternalMassDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void InternalMassDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::InternalMassDefinition internalMassDefinition = modelObject.cast<model::InternalMassDefinition>();
  attach(internalMassDefinition);
  refresh();
}

void InternalMassDefinitionInspectorView::onUpdate()
{
  refresh();
}

void InternalMassDefinitionInspectorView::attach(openstudio::model::InternalMassDefinition & internalMassDefinition)
{
  m_nameEdit->bind(internalMassDefinition,"name");
  m_surfaceAreaEdit->bind(internalMassDefinition,"surfaceArea",m_isIP);
  m_surfaceAreaPerPersonEdit->bind(internalMassDefinition,"surfaceAreaperPerson",m_isIP);
  m_surfaceAreaPerSpaceFloorAreaEdit->bind(internalMassDefinition,"surfaceAreaperSpaceFloorArea",m_isIP);

  m_ConstructionVectorController->attach(internalMassDefinition);

  this->stackedWidget()->setCurrentIndex(1);
}

void InternalMassDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_surfaceAreaEdit->unbind();
  m_surfaceAreaPerPersonEdit->unbind();
  m_surfaceAreaPerSpaceFloorAreaEdit->unbind();

  m_ConstructionVectorController->detach();
}

void InternalMassDefinitionInspectorView::refresh()
{
}

void InternalMassDefinitionInspectorView::ConstructionVectorController::onChangeRelationship(
       const openstudio::model::ModelObject& modelObject, 
       int index, 
       Handle newHandle, 
       Handle oldHandle)
{
  if( index == OS_InternalMass_DefinitionFields::ConstructionName )
  {
    reportItems();
  }
}

boost::optional<model::InternalMassDefinition> 
  InternalMassDefinitionInspectorView::ConstructionVectorController::internalMassDefinition()
{
  if( m_modelObject )
  {
    return m_modelObject->optionalCast<model::InternalMassDefinition>();
  }
  else
  {
    return boost::none;
  }
}

std::vector<OSItemId> InternalMassDefinitionInspectorView::ConstructionVectorController::makeVector()
{
  std::vector<OSItemId> result;

  if( boost::optional<model::InternalMassDefinition> mo = internalMassDefinition() )
  {
    if( boost::optional<model::ConstructionBase> c = mo->construction() )
    {
      result.push_back(modelObjectToItemId(c.get(), false)); 
    }
  }

  return result;
}

void InternalMassDefinitionInspectorView::ConstructionVectorController::onRemoveItem(OSItem* item)
{
  if(boost::optional<model::InternalMassDefinition> mo = internalMassDefinition() )
  {
    mo->resetConstruction();
  }
}

void InternalMassDefinitionInspectorView::ConstructionVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void InternalMassDefinitionInspectorView::ConstructionVectorController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::InternalMassDefinition> internalMass = internalMassDefinition();
  if (internalMass){
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      internalMass->setConstruction(*constructionBase);
    }
  }
}

void InternalMassDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

