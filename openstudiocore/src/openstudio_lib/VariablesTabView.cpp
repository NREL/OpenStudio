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

#include "VariablesTabView.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/OutputVariable.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/OutputVariable_Impl.hpp"

#include "../utilities/sql/SqlFileEnums.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>

#include "../shared_gui_components/OSSwitch.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

  VariableListItem::VariableListItem(const std::string &t_name, 
      const std::string& t_keyValue,
      const boost::optional<openstudio::model::OutputVariable> &t_variable,
      const openstudio::model::Model &t_model)
    : m_name(t_name),
      m_keyValue(t_keyValue),
      m_variable(t_variable),
      m_model(t_model)
  {
    auto hbox = new QHBoxLayout();
    hbox->setContentsMargins(10,10,10,10);
    hbox->setSpacing(10);

    m_onOffButton = new OSSwitch2();
    connect(m_onOffButton, &OSSwitch2::clicked, this, &VariableListItem::onOffClicked);
    hbox->addWidget(m_onOffButton);
    m_onOffButton->setChecked(m_variable.is_initialized());

    hbox->addWidget(new QLabel(openstudio::toQString(m_name + ",")));
    hbox->addWidget(new QLabel(openstudio::toQString(m_keyValue)));
    hbox->addStretch();
    m_combobox = new OSComboBox2();
    connect(m_combobox, static_cast<void (OSComboBox2::*)(const QString &)>(&OSComboBox2::currentIndexChanged), this, &VariableListItem::indexChanged);
    if (m_variable)
    {
      // m_combobox->bind(*m_variable, "reportingFrequency");
      m_combobox->bind<std::string>(
        *m_variable,
        static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
        std::bind(&model::OutputVariable::reportingFrequencyValues),
        std::bind(&model::OutputVariable::reportingFrequency, m_variable.get_ptr()),
        std::bind(&model::OutputVariable::setReportingFrequency, m_variable.get_ptr(), std::placeholders::_1),
        boost::optional<NoFailAction>(std::bind(&model::OutputVariable::resetReportingFrequency, m_variable.get_ptr())),
        boost::optional<BasicQuery>(std::bind(&model::OutputVariable::isReportingFrequencyDefaulted, m_variable.get_ptr())));
    }

    hbox->addWidget(m_combobox);

    setLayout(hbox);
    onOffClicked(m_variable.is_initialized());

  }

  VariablesList::~VariablesList() {

    for (auto itr = m_variables.begin();
          itr != m_variables.end();
          ++itr)
    {
      //(*itr)->setParent(0); 
      //(*itr)->setStyleSheet("");
      m_listLayout->removeWidget(*itr);
      delete (*itr);
    }

    delete m_allOnBtn;
    delete m_allOffBtn;
    delete m_listLayout;
  }

  void VariableListItem::indexChanged(const QString &t_frequency)
  {
    if (m_variable)
    {
      m_variable->setReportingFrequency(openstudio::toString(t_frequency));
    }
  }

  void VariableListItem::setVariableEnabled(bool t_enabled)
  {
    m_onOffButton->setChecked(t_enabled);
    onOffClicked(t_enabled);
  }

  void VariableListItem::onOffClicked(bool t_on)
  {
    LOG(Debug, "onOffClicked " << t_on);
    m_combobox->setEnabled(t_on);

    if (t_on)
    {
      if (!m_variable)
      {
        openstudio::model::OutputVariable outputVariable(m_name, m_model);
        outputVariable.setReportingFrequency("Hourly");
        outputVariable.setKeyValue(m_keyValue);
        m_variable = outputVariable;
        
        // m_combobox->bind(*m_variable, "reportingFrequency");
        m_combobox->bind<std::string>(
          *m_variable,
          static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
          std::bind(&model::OutputVariable::reportingFrequencyValues),
          std::bind(&model::OutputVariable::reportingFrequency, m_variable.get_ptr()),
          std::bind(&model::OutputVariable::setReportingFrequency, m_variable.get_ptr(), std::placeholders::_1),
          boost::none,
          boost::none);
      }
    } else {
      if (m_variable)
      {
        m_combobox->unbind();
        m_variable->remove();
        m_variable = boost::none;
      }
    }
  }

  VariablesList::VariablesList(openstudio::model::Model t_model)
    : m_model(t_model), m_dirty(true)
  {
    t_model.getImpl<openstudio::model::detail::Model_Impl>().get()->addWorkspaceObject.connect<VariablesList, &VariablesList::onAdded>(this);

    t_model.getImpl<openstudio::model::detail::Model_Impl>().get()->removeWorkspaceObject.connect<VariablesList, &VariablesList::onRemoved>(this);
    auto vbox = new QVBoxLayout();
    vbox->setContentsMargins(10,10,10,10);
    vbox->setSpacing(10);
    setLayout(vbox);

    vbox->addWidget(new QLabel("<b>Possible Output Variables</b>"));

    auto innerbox = new QVBoxLayout();
    m_allOnBtn = new QPushButton("All On");
    m_allOnBtn->setFlat(true);
    m_allOnBtn->setObjectName("StandardGrayButton");
    m_allOffBtn = new QPushButton("All Off");
    m_allOffBtn->setFlat(true);
    m_allOffBtn->setObjectName("StandardGrayButton");
    innerbox->addWidget(m_allOnBtn);
    innerbox->addWidget(m_allOffBtn);

    connect(m_allOnBtn, &QPushButton::clicked, this, &VariablesList::allOnClicked);
    connect(m_allOffBtn, &QPushButton::clicked, this, &VariablesList::allOffClicked);
    auto outerbox = new QHBoxLayout();
    outerbox->addLayout(innerbox);
    outerbox->addStretch();

    vbox->addLayout(outerbox);

    m_listLayout = new QVBoxLayout();
    vbox->addLayout(m_listLayout);

    updateVariableList();
  }

  void VariablesList::allOnClicked()
  {
    enableAll(true);
  }

  void VariablesList::allOffClicked()
  {
    enableAll(false);
  }

  void VariablesList::enableAll(bool t_enabled)
  {
    for (std::vector<VariableListItem *>::const_iterator itr = m_variables.begin();
         itr != m_variables.end();
         ++itr)
    {
      (*itr)->setVariableEnabled(t_enabled);
    }
  }

  void VariablesList::onAdded(const WorkspaceObject&, const openstudio::IddObjectType& type, const openstudio::UUID&)
  {
    LOG(Debug, "onAdded: " << type.valueName());

    /// \todo if the user is able to add an output variable through some other means it will not show up here and now
    if (type != openstudio::IddObjectType::OS_Output_Variable)
    {
      if (!m_dirty){
        m_dirty = true;
        QTimer::singleShot(0, this, SLOT(updateVariableList()));
      }
    }
  }

  void VariablesList::onRemoved(const WorkspaceObject&, const openstudio::IddObjectType& type, const openstudio::UUID&)
  {
    LOG(Debug, "onRemoved " << type.valueName());

    /// \todo if the user is remove to add an output variable through some other means it will not show up here and now
    if (type != openstudio::IddObjectType::OS_Output_Variable)
    {
      if (!m_dirty){
        m_dirty = true;
        QTimer::singleShot(0, this, SLOT(updateVariableList()));
      }
    }
  }

  struct PotentialOutputVariable{
    std::string name;
    std::string keyValue;
    boost::optional<openstudio::model::OutputVariable> variable;
  };


  void VariablesList::updateVariableList()
  {
    m_variables.clear();
    while (m_listLayout->count() > 0)
    {
      QLayoutItem *qli = m_listLayout->takeAt(0);
      delete qli->widget();
      delete qli;
    }

    
    // map of variable name + keyValue to PotentialOutputVariable
    std::map<std::string, PotentialOutputVariable> potentialOutputVariableMap;

    // make list of all potential variables
    for (const openstudio::model::ModelObject& modelObject : m_model.getModelObjects<openstudio::model::ModelObject>())
    {
      for (const std::string& variableName : modelObject.outputVariableNames())
      {
        //LOG(Debug, "Found variableName: " << variableName);
        std::string variableNameKeyValue = variableName + "*";
        if (potentialOutputVariableMap.count(variableNameKeyValue) == 0){
          PotentialOutputVariable pov;
          pov.name = variableName;
          pov.keyValue = "*";
          potentialOutputVariableMap.insert(std::pair<std::string, PotentialOutputVariable>(variableNameKeyValue, pov));
        }
      }
    }

    // add all variables to map, allow only one variable per variable name + keyValue in this application 
    for (openstudio::model::OutputVariable outputVariable : m_model.getConcreteModelObjects<openstudio::model::OutputVariable>())
    {
      std::string variableName = outputVariable.variableName();
      std::string keyValue = outputVariable.keyValue();
      std::string variableNameKeyValue = variableName + keyValue;

      if (potentialOutputVariableMap.count(variableNameKeyValue) == 0)
      {
        // DLM: this was causing too much trouble because it kept deleting variables added by users
        // there is no place for this outputvariable with the current objects, delete it.
        //outputVariable.remove();

        // user defined variable, add it to the list
        PotentialOutputVariable pov;
        pov.name = outputVariable.variableName();
        pov.keyValue = outputVariable.keyValue();
        pov.variable = outputVariable;
        potentialOutputVariableMap.insert(std::pair<std::string, PotentialOutputVariable>(variableNameKeyValue, pov));

      } else {

        if (potentialOutputVariableMap[variableNameKeyValue].variable) {
          // already have output variable for this name + keyName, then remove this object
          outputVariable.remove();
        } else {
          // this is a predefined variable 

          // make sure that key value is set to '*'
          // DLM: we know this is already '*' because all predefined variables are set to '*'
          //outputVariable.setKeyValue("*");

          potentialOutputVariableMap[variableNameKeyValue].variable = outputVariable;
        }
      }
    }

    
    for (std::map<std::string, PotentialOutputVariable>::const_iterator itr = potentialOutputVariableMap.begin();
         itr != potentialOutputVariableMap.end();
         ++itr)
    {
      auto hline = new QFrame();
      hline->setFrameShape(QFrame::HLine);
      hline->setFrameShadow(QFrame::Sunken);
      m_listLayout->addWidget(hline);

      PotentialOutputVariable pov = itr->second;

      //LOG(Debug, "Creating VariableListItem for: " << pov.name << ", " << pov.keyValue);
      auto li = new VariableListItem(pov.name, pov.keyValue, pov.variable, m_model);
      m_listLayout->addWidget(li);
      m_variables.push_back(li);
    }

    m_listLayout->addStretch();

    m_dirty = false;
  }

  VariablesTabView::VariablesTabView(openstudio::model::Model t_model, QWidget * parent)
    : MainTabView("Output Variables", MainTabView::MAIN_TAB, parent)
  {
    auto scrollarea = new QScrollArea();
    VariablesList *vl = new VariablesList(t_model);
    scrollarea->setWidget(vl);
    scrollarea->setWidgetResizable(true);
    addTabWidget(scrollarea);
    vl->setAutoFillBackground(false);
  }

} // openstudio

