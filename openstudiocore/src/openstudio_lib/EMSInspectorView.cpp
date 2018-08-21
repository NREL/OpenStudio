/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "EMSInspectorView.hpp"
#include "../model/ModelObject.hpp"
#include "../model/EnergyManagementSystemActuator.hpp"
#include "../model/EnergyManagementSystemActuator_Impl.hpp"
#include "../model/EnergyManagementSystemSensor.hpp"
#include "../model/EnergyManagementSystemSensor_Impl.hpp"
#include "../model/ParentObject.hpp"
#include "../model/ParentObject_Impl.hpp"
#include "../model/Model.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>
#include <functional>

namespace openstudio {


class EMSInspectorHeader : public QWidget {

  public:

  EMSInspectorHeader( const QString & text ) : QWidget() {
    setContentsMargins(0,0,0,0);
    setObjectName("IGHeader");
    auto layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    setLayout(layout);

    auto label = new QLabel( text );
    label->setObjectName("IGHeader");
    label->setStyleSheet("font : bold");
    layout->addWidget(label);
  }
};

class EMSInspectorActuator : public QWidget {

  public:

  EMSInspectorActuator(const model::EMSActuatorNames & names, const model::ModelObject & modelObject) :
    QWidget(),
    m_names(names),
    m_modelObject(modelObject)
  {
    setContentsMargins(0,0,0,0);

    auto layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    auto cb = new QCheckBox(QString::fromStdString(names.controlTypeName()));
    connect(cb, &QCheckBox::toggled, this, &EMSInspectorActuator::onToggled);
    layout->addWidget(cb);

    auto actuators = modelObject.model().getModelObjects<model::EnergyManagementSystemActuator>();

    m_isMatchingActuator = [&](const model::EnergyManagementSystemActuator & actuator) {
      bool result = true;
      if( actuator.actuatedComponentControlType() != m_names.controlTypeName() ) {
        result = false;
      } else if( actuator.actuatedComponentType() != m_names.componentTypeName() ) {
        result = false;
      }
      return result;
    };

    auto it = std::find_if(actuators.begin(),actuators.end(),m_isMatchingActuator);
    if( it != actuators.end() ) {
      cb->setChecked(true);
    }
  }

  private slots:

  void onToggled(bool checked) {
    auto actuators = m_modelObject.model().getModelObjects<model::EnergyManagementSystemActuator>();
    if( checked ) {
      auto it = std::find_if(actuators.begin(),actuators.end(),m_isMatchingActuator);
      if( it == actuators.end() ) {
        model::EnergyManagementSystemActuator(m_modelObject, m_names.componentTypeName(), m_names.controlTypeName());
      }
    } else {
      for( auto & actuator : actuators ) {
        std::cout << "m_names.controlTypeName: " << m_names.controlTypeName() << std::endl;
        std::cout << "m_names.componentTypeName: " << m_names.componentTypeName() << std::endl;
        std::cout << "actuator.actuatedComponentControlType: " << actuator.actuatedComponentControlType() << std::endl;
        std::cout << "actuator.actuatedComponentType: " << actuator.actuatedComponentType() << std::endl;
        if( m_isMatchingActuator(actuator) ) {
          std::cout << "removing" << std::endl;
          actuator.remove();
        }
      }
    }
  }

  private:

  std::function< bool(const model::EnergyManagementSystemActuator &) > m_isMatchingActuator;
  model::EMSActuatorNames m_names;
  model::ModelObject m_modelObject;
};

class EMSInspectorSensor : public QWidget {

  public:

  EMSInspectorSensor(const std::string & name, const model::ModelObject & modelObject) :
    QWidget(),
    m_name(name),
    m_modelObject(modelObject)
  {
    setContentsMargins(0,0,0,0);

    auto layout = new QHBoxLayout();
    layout->setSpacing(10);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    auto cb = new QCheckBox(QString::fromStdString(name));
    connect(cb, &QCheckBox::toggled, this, &EMSInspectorSensor::onToggled);
    layout->addWidget(cb);

    auto sensors = modelObject.model().getModelObjects<model::EnergyManagementSystemSensor>();

    m_isMatchingSensor= [&](const model::EnergyManagementSystemSensor & sensor) {
      bool result = true;
      if( sensor.keyName() != toString(m_modelObject.handle()) ) {
        result = false;
      } else if( sensor.outputVariableOrMeterName() != m_name ) {
        result = false;
      }
      return result;
    };

    auto it = std::find_if(sensors.begin(),sensors.end(),m_isMatchingSensor);
    if( it != sensors.end() ) {
      cb->setChecked(true);
    }
  }

  private slots:

  void onToggled(bool checked) {
    auto sensors = m_modelObject.model().getModelObjects<model::EnergyManagementSystemSensor>();
    if( checked ) {
      auto it = std::find_if(sensors.begin(),sensors.end(),m_isMatchingSensor);
      if( it == sensors.end() ) {
        model::EnergyManagementSystemSensor sensor(m_modelObject.model(),m_name);
        sensor.setKeyName(toString(m_modelObject.handle()));
      }
    } else {
      for( auto & sensor : sensors ) {
        if( m_isMatchingSensor(sensor) ) {
          sensor.remove();
        }
      }
    }
  }

  private:

  std::function< bool(const model::EnergyManagementSystemSensor &) > m_isMatchingSensor;
  std::string m_name;
  model::ModelObject m_modelObject;
};

EMSInspectorView::EMSInspectorView(QWidget* parent, EMSInspectorView::Type type) :
  QWidget(parent),
  m_type(type)
{
  setContentsMargins(0, 0, 0, 0);
  m_layout = new QVBoxLayout();
  m_layout->setSpacing(10);
  m_layout->setMargin(0);
  m_layout->setAlignment(Qt::AlignTop);
  //setLayout(m_layout);

  auto scrollWidget = new QWidget();
  scrollWidget->setObjectName("ScrollWidget");
  scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
  scrollWidget->setLayout(m_layout);

  m_scrollArea = new QScrollArea();
  m_scrollArea->setFrameStyle(QFrame::NoFrame);
  m_scrollArea->setWidget(scrollWidget);
  m_scrollArea->setWidgetResizable(true);
  m_scrollArea->setBackgroundRole(QPalette::NoRole);

  //m_layout->addWidget(m_scrollArea);
  auto scrollLayout = new QVBoxLayout();
  scrollLayout->setContentsMargins(0,0,0,0);
  scrollLayout->addWidget(m_scrollArea);

  setLayout(scrollLayout);
}

void EMSInspectorView::layoutModelObject( const model::ModelObject & modelObject ) {
  std::vector<model::ModelObject> modelObjects{modelObject};

  auto parentObject = modelObject.optionalCast<model::ParentObject>();
  if ( parentObject ) {
    auto children = parentObject->children();
    modelObjects.insert(modelObjects.end(),children.begin(),children.end());
  }

  for( const auto & objecti : modelObjects ) {
    if( m_type == Type::ACTUATOR ) {
      auto label = new EMSInspectorHeader(QString::fromStdString(objecti.nameString()) + " Acuators");
      m_layout->addWidget(label);

      const auto actuatorNames = objecti.emsActuatorNames();

      for ( const auto actuator : actuatorNames ) {
        auto actuatorWidget = new EMSInspectorActuator(actuator, objecti);
        m_layout->addWidget(actuatorWidget);
      }
    } else if( m_type == Type::SENSOR ) {
      auto label = new EMSInspectorHeader(QString::fromStdString(objecti.nameString()) + " Sensors");
      m_layout->addWidget(label);

      const auto sensorNames = objecti.outputVariableNames();

      for ( const auto sensor : sensorNames ) {
        auto sensorWidget = new EMSInspectorSensor(sensor, objecti);
        m_layout->addWidget(sensorWidget);
      }
    }
  }
}

} // openstudio

