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

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // Surface Area

  label = new QLabel("Surface Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  m_surfaceAreaEdit = new OSQuantityEdit2("m^2","m^2","ft^2", m_isIP);
  connect(this, &InternalMassDefinitionInspectorView::toggleUnitsClicked, m_surfaceAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_surfaceAreaEdit,3,0);

  // Surface Area Per Space Floor Area

  label = new QLabel("Surface Area Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_surfaceAreaPerSpaceFloorAreaEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &InternalMassDefinitionInspectorView::toggleUnitsClicked, m_surfaceAreaPerSpaceFloorAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_surfaceAreaPerSpaceFloorAreaEdit,3,1);

  // Surface Area Per Person

  label = new QLabel("Surface Area Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_surfaceAreaPerPersonEdit = new OSQuantityEdit2("m^2/person","m^2/person","ft^2/person", m_isIP);
  connect(this, &InternalMassDefinitionInspectorView::toggleUnitsClicked, m_surfaceAreaPerPersonEdit, &OSQuantityEdit2::onUnitSystemChange);
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
  m_internalMassDefinition = internalMassDefinition;

  // m_nameEdit->bind(internalMassDefinition,"name");
  m_nameEdit->bind(
    *m_internalMassDefinition,
    OptionalStringGetter(std::bind(&model::InternalMassDefinition::name, m_internalMassDefinition.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::InternalMassDefinition::setName, m_internalMassDefinition.get_ptr(),std::placeholders::_1))
  );

  // m_surfaceAreaEdit->bind(internalMassDefinition,"surfaceArea",m_isIP);
  m_surfaceAreaEdit->bind(
    m_isIP,
    *m_internalMassDefinition,
    OptionalDoubleGetter(std::bind(&model::InternalMassDefinition::surfaceArea, m_internalMassDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::InternalMassDefinition::*)(double)>(&model::InternalMassDefinition::setSurfaceArea), m_internalMassDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_surfaceAreaPerPersonEdit->bind(internalMassDefinition,"surfaceAreaperPerson",m_isIP);
  m_surfaceAreaPerPersonEdit->bind(
    m_isIP,
    *m_internalMassDefinition,
    OptionalDoubleGetter(std::bind(&model::InternalMassDefinition::surfaceAreaperPerson, m_internalMassDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::InternalMassDefinition::*)(double)>(&model::InternalMassDefinition::setSurfaceAreaperPerson), m_internalMassDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_surfaceAreaPerSpaceFloorAreaEdit->bind(internalMassDefinition,"surfaceAreaperSpaceFloorArea",m_isIP);
  m_surfaceAreaPerSpaceFloorAreaEdit->bind(
    m_isIP,
    *m_internalMassDefinition,
    OptionalDoubleGetter(std::bind(&model::InternalMassDefinition::surfaceAreaperSpaceFloorArea, m_internalMassDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::InternalMassDefinition::*)(double)>(&model::InternalMassDefinition::setSurfaceAreaperSpaceFloorArea), m_internalMassDefinition.get_ptr(), std::placeholders::_1))
  );

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

  m_internalMassDefinition = boost::none;

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

