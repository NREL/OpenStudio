/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "EMSInspectorView.hpp"
#include "../model/ModelObject.hpp"
#include "../model/EnergyManagementSystemActuator.hpp"
#include "../model/EnergyManagementSystemActuator_Impl.hpp"
#include "../model/ParentObject.hpp"
#include "../model/ParentObject_Impl.hpp"
#include "../model/Model.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFrame>
#include <QLabel>
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
      if( actuator.actuatedComponentControlType() != names.controlTypeName() ) {
        result = false;
      } else if( actuator.actuatedComponentType() != names.componentTypeName() ) {
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
        if( m_isMatchingActuator(actuator) ) {
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

EMSInspectorView::EMSInspectorView(QWidget* parent) : QWidget(parent) {
  setContentsMargins(0,0,0,0);
  m_layout = new QVBoxLayout();
  m_layout->setSpacing(10);
  m_layout->setMargin(0);
  m_layout->setAlignment(Qt::AlignTop);
  setLayout(m_layout);
}

void EMSInspectorView::layoutModelObject( const model::ModelObject & modelObject ) {
  std::vector<model::ModelObject> modelObjects{modelObject};

  auto parentObject = modelObject.optionalCast<model::ParentObject>();
  if ( parentObject ) {
    auto children = parentObject->children();
    modelObjects.insert(modelObjects.end(),children.begin(),children.end());
  }

  for( const auto & objecti : modelObjects ) {
    auto label = new EMSInspectorHeader(QString::fromStdString(objecti.nameString()) + " Acuators");
    m_layout->addWidget(label);

    const auto sensorNames = objecti.emsActuatorNames();

    for ( const auto sensor : sensorNames ) {
      auto actuatorWidget = new EMSInspectorActuator(sensor, objecti);
      m_layout->addWidget(actuatorWidget);
    }
  }
}

} // openstudio

