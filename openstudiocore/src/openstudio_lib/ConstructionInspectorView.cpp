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

#include "ConstructionInspectorView.hpp"

#include "StandardsInformationConstructionWidget.hpp"
#include "ConstructionObjectVectorController.hpp"
#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"

#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/StandardsInformationConstruction.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Construction_FieldEnums.hxx>

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QStackedWidget>

namespace openstudio {

// ConstructionInspectorView

ConstructionInspectorView::ConstructionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_standardsInformationWidget(nullptr),
    m_constructionDZ(nullptr),
    m_constructionVC(nullptr)
{
  createLayout();
}

void ConstructionInspectorView::createLayout()
{
  auto hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Name

  label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Standards Information

  m_standardsInformationWidget = new StandardsInformationConstructionWidget(m_isIP, mainGridLayout, row);
  m_standardsInformationWidget->showFenestration();
  m_standardsInformationWidget->disableFenestration();
  
  ++row;

  // Layer

  label = new QLabel("Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  label = new QLabel("Outside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_constructionVC = new ConstructionObjectVectorController(this);
  m_constructionDZ = new OSDropZone(m_constructionVC,"Drag From Library",QSize(0,0),false);
  m_constructionDZ->setMinItems(0);
  m_constructionDZ->setMaxItems(12);
  m_constructionDZ->setItemsAcceptDrops(true);
  m_constructionDZ->setFixedSize(QSize(OSItem::ITEM_WIDTH + 20,600));
  mainGridLayout->addWidget(m_constructionDZ, row, 0);

  ++row;

  label = new QLabel("Inside");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  // Stretch

  mainGridLayout->setRowStretch(row, 100);

  mainGridLayout->setColumnStretch(100, 100);
}

void ConstructionInspectorView::onClearSelection()
{
  detach();

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Construction construction = modelObject.cast<model::Construction>();
  attach(construction);

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionInspectorView::onUpdate()
{
  m_constructionVC->reportItems();

  boost::optional<model::ModelObject> modelObject = this->modelObject();
  if (modelObject){
    if (modelObject->cast<model::ConstructionBase>().isFenestration()){
      m_standardsInformationWidget->enableFenestration();
    } else {
      m_standardsInformationWidget->disableFenestration();
    }
  } else {
    m_standardsInformationWidget->disableFenestration();
  }
}

void ConstructionInspectorView::attach(openstudio::model::Construction & construction)
{
  m_construction = construction;

  m_nameEdit->bind(
    *m_construction,
    OptionalStringGetter(std::bind(&model::Construction::name, m_construction.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::Construction::setName, m_construction.get_ptr(),std::placeholders::_1))
  );

  m_constructionVC->attach(construction);
  m_constructionVC->reportItems();

  m_standardsInformationWidget->attach(construction);
}

void ConstructionInspectorView::detach()
{
  m_standardsInformationWidget->detach();
  m_constructionVC->detach();
  m_construction = boost::none;
}

} // openstudio
