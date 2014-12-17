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

#include "ConstructionInspectorView.hpp"

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
  : ConstructionBaseInspectorView(isIP, model, parent),
    m_constructionDZ(nullptr)
{
  createLayout();
  showFenestration();
  disableFenestration();
}

void ConstructionInspectorView::createLayout()
{
  int row = m_mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Layer

  label = new QLabel("Layer: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  label = new QLabel("Outside");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  m_constructionVC = new ConstructionObjectVectorController(this);
  m_constructionDZ = new OSDropZone(m_constructionVC,"Drag From Library",QSize(0,0),false);
  m_constructionDZ->setMinItems(0);
  m_constructionDZ->setMaxItems(12);
  m_constructionDZ->setItemsAcceptDrops(true);
  m_constructionDZ->setFixedSize(QSize(OSItem::ITEM_WIDTH + 20,600));
  m_mainGridLayout->addWidget(m_constructionDZ,row,0);

  ++row;

  label = new QLabel("Inside");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row,0);

  ++row;

  // Stretch

  m_mainGridLayout->setRowStretch(row,100);

  m_mainGridLayout->setColumnStretch(100,100);
}

void ConstructionInspectorView::onClearSelection()
{
  ConstructionBaseInspectorView::onClearSelection(); // call parent implementation

  detach();
}

void ConstructionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Construction construction = modelObject.cast<model::Construction>();
  attach(construction);
  refresh();
}

void ConstructionInspectorView::populateStandardsConstructionType()
{
  ConstructionBaseInspectorView::populateStandardsConstructionType(); // call parent implementation

  connect(m_standardsConstructionType, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &ConstructionInspectorView::standardsConstructionTypeChanged);
  connect(m_standardsConstructionType, &QComboBox::editTextChanged, this, &ConstructionInspectorView::editStandardsConstructionType);
}

void ConstructionInspectorView::attach(openstudio::model::Construction & construction)
{
  m_nameEdit->bind(construction,"name");

  m_constructionVC->attach(construction);
  m_constructionVC->reportItems();

  m_standardsInformation = construction.standardsInformation();

  m_intendedSurfaceType->bind<std::string>(
      *m_standardsInformation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceTypeValues),
      std::function<boost::optional<std::string> ()>(std::bind(&openstudio::model::StandardsInformationConstruction::intendedSurfaceType,m_standardsInformation.get_ptr())),
      std::bind(&openstudio::model::StandardsInformationConstruction::setIntendedSurfaceType,m_standardsInformation.get_ptr(),std::placeholders::_1),
      NoFailAction(std::bind(&model::StandardsInformationConstruction::resetIntendedSurfaceType,m_standardsInformation.get_ptr())));

  connect(m_standardsInformation->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), &openstudio::model::detail::ModelObject_Impl::onChange, this, &ConstructionInspectorView::populateStandardsConstructionType);

  m_standardsConstructionType->setEnabled(true);
  populateStandardsConstructionType();

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionInspectorView::detach()
{
  ConstructionBaseInspectorView::detach(); // call parent implementation

  m_constructionVC->detach();
}

} // openstudio
