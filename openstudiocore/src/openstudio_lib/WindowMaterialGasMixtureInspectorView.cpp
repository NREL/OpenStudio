/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "WindowMaterialGasMixtureInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSIntegerEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/GasMixture.hpp"
#include "../model/GasMixture_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialGasMixtureInspectorView

WindowMaterialGasMixtureInspectorView::WindowMaterialGasMixtureInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGasMixtureInspectorView::createLayout()
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

  m_standardsInformationWidget = new StandardsInformationMaterialWidget(m_isIP, mainGridLayout, row);

  ++row;

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasMixtureInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Number Of Gases In Mixture

  label = new QLabel("Number Of Gases In Mixture: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_numberOfGasesInMixture = new OSIntegerEdit2();
  mainGridLayout->addWidget(m_numberOfGasesInMixture,row++,0,1,3);

  //************************* 1 *************************

  // Gas Fraction

  label = new QLabel("Gas 1 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas1Fraction = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasMixtureInspectorView::toggleUnitsClicked, m_gas1Fraction, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_gas1Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 1 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas1Type = new OSComboBox2();
  m_gas1Type->addItem("Air");
  m_gas1Type->addItem("Argon");
  m_gas1Type->addItem("Krypton");
  m_gas1Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas1Type,row++,0,1,3);


  //************************* 2 *************************

  // Gas Fraction

  label = new QLabel("Gas 2 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas2Fraction = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasMixtureInspectorView::toggleUnitsClicked, m_gas2Fraction, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_gas2Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 2 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas2Type = new OSComboBox2();
  m_gas2Type->addItem("Air");
  m_gas2Type->addItem("Argon");
  m_gas2Type->addItem("Krypton");
  m_gas2Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas2Type,row++,0,1,3);

  //************************* 3 *************************

  // Gas Fraction

  label = new QLabel("Gas 3 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas3Fraction = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasMixtureInspectorView::toggleUnitsClicked, m_gas3Fraction, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_gas3Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 3 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas3Type = new OSComboBox2();
  m_gas3Type->addItem("Air");
  m_gas3Type->addItem("Argon");
  m_gas3Type->addItem("Krypton");
  m_gas3Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas3Type,row++,0,1,3);

  //************************* 4 *************************

  // Gas Fraction

  label = new QLabel("Gas 4 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas4Fraction = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasMixtureInspectorView::toggleUnitsClicked, m_gas4Fraction, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_gas4Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 4 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gas4Type = new OSComboBox2();
  m_gas4Type->addItem("Air");
  m_gas4Type->addItem("Argon");
  m_gas4Type->addItem("Krypton");
  m_gas4Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas4Type,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialGasMixtureInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialGasMixtureInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::GasMixture gasMixture = modelObject.cast<model::GasMixture>();
  attach(gasMixture);
  refresh();
}

void WindowMaterialGasMixtureInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialGasMixtureInspectorView::attach(openstudio::model::GasMixture & gasMixture)
{
  // m_gas1Type->bind(gasMixture,"gas1Type");
  m_gas1Type->bind<std::string>(
    gasMixture,
    static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    &model::GasMixture::gas1TypeValues,
    std::bind(&model::GasMixture::gas1Type, &gasMixture),
    std::bind(&model::GasMixture::setGas1Type, &gasMixture, std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::GasMixture::resetGas1Type, &gasMixture)),
    boost::none);

  // m_gas2Type->bind(gasMixture,"gas2Type");
  m_gas2Type->bind<std::string>(
    gasMixture,
    static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    &model::GasMixture::gas2TypeValues,
    std::bind(&model::GasMixture::gas2Type, &gasMixture),
    std::bind(&model::GasMixture::setGas2Type, &gasMixture, std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::GasMixture::resetGas2Type, &gasMixture)),
    boost::none);

  // m_gas3Type->bind(gasMixture,"gas3Type");
  m_gas3Type->bind<std::string>(
    gasMixture,
    static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    &model::GasMixture::gas3TypeValues,
    std::bind(&model::GasMixture::gas3Type, &gasMixture),
    std::bind(&model::GasMixture::setGas3Type, &gasMixture, std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::GasMixture::resetGas3Type, &gasMixture)),
    boost::none);

  // m_gas4Type->bind(gasMixture,"gas4Type");
  m_gas4Type->bind<std::string>(
    gasMixture,
    static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
    &model::GasMixture::gas4TypeValues,
    std::bind(&model::GasMixture::gas4Type, &gasMixture),
    std::bind(&model::GasMixture::setGas4Type, &gasMixture, std::placeholders::_1),
    boost::optional<NoFailAction>(std::bind(&model::GasMixture::resetGas4Type, &gasMixture)),
    boost::none);

  // m_numberOfGasesInMixture->bind(gasMixture,"numberofGasesinMixture");

  boost::optional<model::GasMixture> m_gasMixture = gasMixture;

  m_numberOfGasesInMixture->bind(
    *m_gasMixture,
    OptionalIntGetter(std::bind(&model::GasMixture::numberofGasesinMixture, m_gasMixture.get_ptr())),
    boost::optional<IntSetter>(std::bind(&model::GasMixture::setNumberofGasesinMixture, m_gasMixture.get_ptr(), std::placeholders::_1))
  );

  // m_nameEdit->bind(gasMixture,"name");
  m_nameEdit->bind(
    *m_gasMixture,
    OptionalStringGetter(std::bind(&model::GasMixture::name, m_gasMixture.get_ptr(), true)),
    boost::optional<StringSetter>(std::bind(&model::GasMixture::setName, m_gasMixture.get_ptr(),std::placeholders::_1))
  );

  // OSQuantityEdit
  m_thickness->bind(gasMixture,"thickness",m_isIP);
  m_gas1Fraction->bind(gasMixture,"gas1Fraction",m_isIP);
  m_gas2Fraction->bind(gasMixture,"gas2Fraction",m_isIP);
  m_gas3Fraction->bind(gasMixture,"gas3Fraction",m_isIP);
  m_gas4Fraction->bind(gasMixture,"gas4Fraction",m_isIP);

  m_standardsInformationWidget->attach(gasMixture);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialGasMixtureInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_gas1Type->unbind();
  m_gas2Type->unbind();
  m_gas3Type->unbind();
  m_gas4Type->unbind();

  m_numberOfGasesInMixture->unbind();

  m_nameEdit->unbind();
  m_thickness->unbind();
  m_gas1Fraction->unbind();
  m_gas2Fraction->unbind();
  m_gas3Fraction->unbind();
  m_gas4Fraction->unbind();

  m_standardsInformationWidget->detach();
}

void WindowMaterialGasMixtureInspectorView::refresh()
{
}

} // openstudio
