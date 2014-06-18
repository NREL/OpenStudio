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

#include "WindowMaterialGasMixtureInspectorView.hpp"

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
    m_gas1Type(NULL),
    m_gas2Type(NULL),
    m_gas3Type(NULL),
    m_gas4Type(NULL),
    m_nameEdit(NULL),
    m_thickness(NULL),
    m_numberOfGasesInMixture(NULL),
    m_gas1Fraction(NULL),
    m_gas2Fraction(NULL),
    m_gas3Fraction(NULL),
    m_gas4Fraction(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGasMixtureInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  unsigned row = 0;
  unsigned col = 0;

  bool isConnected = false;

  // Name

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,row++,0,1,3);

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_thickness = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thickness, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Number Of Gases In Mixture

  label = new QLabel("Number Of Gases In Mixture: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_numberOfGasesInMixture = new OSIntegerEdit();
  mainGridLayout->addWidget(m_numberOfGasesInMixture,row++,0,1,3);

  //************************* 1 *************************

  // Gas Fraction

  label = new QLabel("Gas 1 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas1Fraction = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gas1Fraction, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_gas1Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 1 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas1Type = new OSComboBox();
  m_gas1Type->addItem("Air");
  m_gas1Type->addItem("Argon");
  m_gas1Type->addItem("Krypton");
  m_gas1Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas1Type,row++,0,1,3);


  //************************* 2 *************************

  // Gas Fraction

  label = new QLabel("Gas 2 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas2Fraction = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gas2Fraction, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_gas2Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 2 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas2Type = new OSComboBox();
  m_gas2Type->addItem("Air");
  m_gas2Type->addItem("Argon");
  m_gas2Type->addItem("Krypton");
  m_gas2Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas2Type,row++,0,1,3);

  //************************* 3 *************************

  // Gas Fraction

  label = new QLabel("Gas 3 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas3Fraction = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gas3Fraction, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_gas3Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 3 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas3Type = new OSComboBox();
  m_gas3Type->addItem("Air");
  m_gas3Type->addItem("Argon");
  m_gas3Type->addItem("Krypton");
  m_gas3Type->addItem("Xenon");
  mainGridLayout->addWidget(m_gas3Type,row++,0,1,3);

  //************************* 4 *************************

  // Gas Fraction

  label = new QLabel("Gas 4 Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas4Fraction = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gas4Fraction, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_gas4Fraction,row++,0,1,3);

  // Gas Type

  label = new QLabel("Gas 4 Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_gas4Type = new OSComboBox();
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
  m_gas1Type->bind(gasMixture,"gas1Type");
  m_gas2Type->bind(gasMixture,"gas2Type");
  m_gas3Type->bind(gasMixture,"gas3Type");
  m_gas4Type->bind(gasMixture,"gas4Type");

  m_numberOfGasesInMixture->bind(gasMixture,"numberofGasesinMixture");

  m_nameEdit->bind(gasMixture,"name");
  m_thickness->bind(gasMixture,"thickness",m_isIP);
  m_gas1Fraction->bind(gasMixture,"gas1Fraction",m_isIP);
  m_gas2Fraction->bind(gasMixture,"gas2Fraction",m_isIP);
  m_gas3Fraction->bind(gasMixture,"gas3Fraction",m_isIP);
  m_gas4Fraction->bind(gasMixture,"gas4Fraction",m_isIP);

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
}

void WindowMaterialGasMixtureInspectorView::refresh()
{
}

void WindowMaterialGasMixtureInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
