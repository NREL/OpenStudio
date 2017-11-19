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

#include "PhotovoltaicThermalInspectorView.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/PhotovoltaicThermal.hpp"
#include "../model/PhotovoltaicThermal_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

PhotovoltaicThermalInspectorView::PhotovoltaicThermalInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
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

  m_surfaceAreaEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_surfaceAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_surfaceAreaEdit, 3, 0);

  label = new QLabel("System Efficiency: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 2, 1);

  m_systemEfficiencyEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_systemEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_systemEfficiencyEdit, 3, 1);

  label = new QLabel("Collector Active Area Faction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 2, 2);

  m_factionActiveEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_factionActiveEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_factionActiveEdit, 3, 2);

  label = new QLabel("Collector Efficiency: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 4, 0);

  m_collectorEfficiencyEdit  = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_collectorEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_collectorEfficiencyEdit, 5, 0);

  label = new QLabel("Boiler Efficiency: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 4, 1);

  m_boilerEfficiencyEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_boilerEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_boilerEfficiencyEdit, 5, 1);

  label = new QLabel("Azimuth Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 6, 0);

  m_azimuthAngleEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_azimuthAngleEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_azimuthAngleEdit, 7, 0);

  label = new QLabel("Inclination Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 6, 1);

  m_inclinationAngleEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_inclinationAngleEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_inclinationAngleEdit, 7, 1);

  label = new QLabel("Solar Irradiation : ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 8, 0);

  m_gtEfficiencyEdit = new OSQuantityEdit2("","","",m_isIP);
  connect(this, &PhotovoltaicThermalInspectorView::toggleUnitsClicked, m_gtEfficiencyEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_gtEfficiencyEdit, 9, 0);

    // Stretch

  mainGridLayout->setRowStretch(10,100);

  mainGridLayout->setColumnStretch(3,100);
}

void PhotovoltaicThermalInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void PhotovoltaicThermalInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::PhotovoltaicThermal photovoltaicThermal = modelObject.cast<model::PhotovoltaicThermal>();
  attach(photovoltaicThermal);
  refresh();
}

void PhotovoltaicThermalInspectorView::onUpdate()
{
  refresh();
}

void PhotovoltaicThermalInspectorView::attach(openstudio::model::PhotovoltaicThermal & PhotovoltaicThermal)
{
  m_photovoltaicThermal = PhotovoltaicThermal;

  //m_nameEdit->bind(PhotovoltaicThermal,"name");
  m_nameEdit->bind(
    *m_photovoltaicThermal,
    OptionalStringGetter(std::bind(&model::PhotovoltaicThermal::name, m_photovoltaicThermal.get_ptr(), true)),
    boost::optional<StringSetter>(std::bind(&model::PhotovoltaicThermal::setName, m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );
  
  //m_surfaceAreaEdit->bind(PhotovoltaicThermal, "surfaceArea", m_isIP);
  m_surfaceAreaEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::surfaceArea, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setSurfaceArea), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );
  
 // m_factionActiveEdit->bind(PhotovoltaicThermal, "factionActive", m_isIP);
  m_factionActiveEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::factionActive, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setFactionActive), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );
  //m_boilerEfficiencyEdit->bind(PhotovoltaicThermal,"boilerEfficiency", m_isIP);
  m_boilerEfficiencyEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::boilerEfficiency, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setBoilerEfficiency), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );

  //m_collectorEfficiencyEdit->bind(PhotovoltaicThermal, "collectorEfficiency", m_isIP);
  m_collectorEfficiencyEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::collectorEfficiency, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setCollectorEfficiency), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );

  //m_azimuthAngleEdit->bind(PhotovoltaicThermal, "azimuthAngle", m_isIP);
  m_azimuthAngleEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::azimuthAngle, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setAzimuthAngle), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );

  //m_inclinationAngleEdit->bind(PhotovoltaicThermal, "inclinationAngle", m_isIP);
  m_inclinationAngleEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::inclinationAngle, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setInclinationAngle), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );

  //m_gtEfficiencyEdit->bind(PhotovoltaicThermal, "gtEfficiency", m_isIP);
  m_gtEfficiencyEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::gtEfficiency, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setGTEfficiency), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );

  //m_systemEfficiencyEdit->bind(PhotovoltaicThermal, "cellEfficiency", m_isIP);
  m_systemEfficiencyEdit->bind(
    m_isIP,
    *m_photovoltaicThermal,
    DoubleGetter(std::bind(&model::PhotovoltaicThermal::cellEfficiency, m_photovoltaicThermal.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::PhotovoltaicThermal::*)(double)>(&model::PhotovoltaicThermal::setCellEfficiency), m_photovoltaicThermal.get_ptr(), std::placeholders::_1))
    );
  this->stackedWidget()->setCurrentIndex(1);
}

void PhotovoltaicThermalInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_surfaceAreaEdit->unbind();
  m_factionActiveEdit->unbind();
  m_boilerEfficiencyEdit->unbind();
  m_collectorEfficiencyEdit->unbind();
  m_azimuthAngleEdit->unbind();
  m_inclinationAngleEdit->unbind();
  m_gtEfficiencyEdit->unbind();
  m_systemEfficiencyEdit->unbind();

}

void PhotovoltaicThermalInspectorView::refresh()
{
}

void PhotovoltaicThermalInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

