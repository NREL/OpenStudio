/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "PhotovoltaicInspectorView.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/Photovoltaic.hpp"
#include "../model/Photovoltaic_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

PhotovoltaicInspectorView::PhotovoltaicInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // Name

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,2);

  label = new QLabel("Surface Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 2, 0);

  m_surfaceAreaEdit = new OSQuantityEdit2("", "", "", m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_surfaceAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_surfaceAreaEdit, 3, 0);

  label = new QLabel("PV Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 2, 1);

  m_PVTypeComboBox = new OSComboBox2();
  m_PVTypeComboBox->setFixedWidth(OSItem::ITEM_WIDTH);
  m_PVTypeComboBox->addItem("Monocrystalline");
  m_PVTypeComboBox->addItem("Polycrystalline");
  m_PVTypeComboBox->addItem("Amorphous");
  mainGridLayout->addWidget(m_PVTypeComboBox, 3, 1);

  label = new QLabel("System Efficiency: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 2, 2);

  m_systemEfficiencyEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_systemEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_systemEfficiencyEdit, 3, 2);

  label = new QLabel("Active Cell Area Faction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 4, 0);

  m_factionActiveEdit = new OSQuantityEdit2("", "", "", m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_factionActiveEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_factionActiveEdit, 5, 0);

  label = new QLabel("Inverter Efficiency: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 6, 0);

  m_inverterEfficiencyEdit = new OSQuantityEdit2("", "", "", m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_inverterEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_inverterEfficiencyEdit, 7, 0);

  label = new QLabel("Azimuth Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 8, 0);

  m_azimuthAngleEdit = new OSQuantityEdit2("", "", "", m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_azimuthAngleEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_azimuthAngleEdit, 9, 0);

  label = new QLabel("Inclination Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 8, 1);

  m_inclinationAngleEdit = new OSQuantityEdit2("", "", "", m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_inclinationAngleEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_inclinationAngleEdit, 9, 1);

  label = new QLabel("Solar Irradiation : ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 10, 0);

  m_gtEfficiencyEdit = new OSQuantityEdit2("", "", "", m_isIP);
  connect(this, &PhotovoltaicInspectorView::toggleUnitsClicked, m_gtEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_gtEfficiencyEdit, 11, 0);

    // Stretch

  mainGridLayout->setRowStretch(12,100);

  mainGridLayout->setColumnStretch(3,100);
}

void PhotovoltaicInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void PhotovoltaicInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Photovoltaic photovoltaic = modelObject.cast<model::Photovoltaic>();
  attach(photovoltaic);
  refresh();
}

void PhotovoltaicInspectorView::onUpdate()
{
  refresh();
}

void PhotovoltaicInspectorView::attach(openstudio::model::Photovoltaic & Photovoltaic)
{
  m_Photovoltaic = Photovoltaic;

  // m_nameEdit->bind(Photovoltaic, "name");
  m_nameEdit->bind(
    *m_Photovoltaic,
    OptionalStringGetter(std::bind(&model::Photovoltaic::name, m_Photovoltaic.get_ptr(), true)),
    boost::optional<StringSetter>(std::bind(&model::Photovoltaic::setName, m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );
  
  //m_PVTypeComboBox->bind(Photovoltaic,"PVType");
  if (m_PVTypeComboBox) {
    m_PVTypeComboBox->bind<std::string>(
      *m_Photovoltaic,
      static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
      &model::Photovoltaic::PVTypeValues,
      std::bind(&model::Photovoltaic::PVType, m_Photovoltaic.get_ptr()),
      std::bind(&model::Photovoltaic::setPVType, m_Photovoltaic.get_ptr(), std::placeholders::_1),
      boost::none,
      boost::none);
  }

  //m_surfaceAreaEdit->bind(Photovoltaic, "surfaceArea", m_isIP);
  m_surfaceAreaEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::surfaceArea, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setSurfaceArea), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  //m_factionActiveEdit->bind(Photovoltaic, "factionActive", m_isIP);
  m_factionActiveEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::factionActive, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setFactionActive), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  //m_inverterEfficiencyEdit->bind(Photovoltaic, "inverterEfficiency", m_isIP);
  m_inverterEfficiencyEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::inverterEfficiency, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setInverterEfficiency), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  //m_azimuthAngleEdit->bind(Photovoltaic, "azimuthAngle",  m_isIP);
  m_azimuthAngleEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::azimuthAngle, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setAzimuthAngle), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  //m_inclinationAngleEdit->bind(Photovoltaic, "inclinationAngle", m_isIP);
  m_inclinationAngleEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::inclinationAngle, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setInclinationAngle), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  //m_gtEfficiencyEdit->bind(Photovoltaic, "gtEfficiency", m_isIP);
  m_gtEfficiencyEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::gtEfficiency, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setGTEfficiency), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  //m_systemEfficiencyEdit->bind(Photovoltaic, "cellEfficiency", m_isIP);
  m_systemEfficiencyEdit->bind(
    m_isIP,
    *m_Photovoltaic,
    DoubleGetter(std::bind(&model::Photovoltaic::cellEfficiency, m_Photovoltaic.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Photovoltaic::*)(double)>(&model::Photovoltaic::setCellEfficiency), m_Photovoltaic.get_ptr(), std::placeholders::_1))
    );

  this->stackedWidget()->setCurrentIndex(1);
}

void PhotovoltaicInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_PVTypeComboBox->unbind();
  m_surfaceAreaEdit->unbind();
  m_factionActiveEdit->unbind();
  m_inverterEfficiencyEdit->unbind();
  m_azimuthAngleEdit->unbind();
  m_inclinationAngleEdit->unbind();
  m_gtEfficiencyEdit->unbind();
  m_systemEfficiencyEdit->unbind();

}

void PhotovoltaicInspectorView::refresh()
{
}

void PhotovoltaicInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

