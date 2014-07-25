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

namespace openstudio {

  VariableListItem::VariableListItem(const std::string &t_name, 
      const boost::optional<openstudio::model::OutputVariable> &t_variable,
      const openstudio::model::Model &t_model)
    : m_name(t_name),
      m_variable(t_variable),
      m_model(t_model)
  {
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setContentsMargins(10,10,10,10);
    hbox->setSpacing(10);

    m_onOffButton = new OSSwitch();
    connect(m_onOffButton, SIGNAL(clicked(bool)), this, SLOT(onOffClicked(bool)));

    hbox->addWidget(m_onOffButton);
    m_onOffButton->setChecked(m_variable.is_initialized());

    hbox->addWidget(new QLabel(openstudio::toQString(m_name)));
    hbox->addStretch();
    m_combobox = new OSComboBox();
    connect(m_combobox, SIGNAL(currentIndexChanged(const QString &)),
        this, SLOT(indexChanged(const QString &)));
    
    if (m_variable)
    {
      m_combobox->bind(*m_variable, "reportingFrequency");
    }


    hbox->addWidget(m_combobox);


    setLayout(hbox);
    onOffClicked(m_variable.is_initialized());

  }

  VariablesList::~VariablesList() {

    for (std::vector<VariableListItem*>::iterator itr = m_variables.begin();
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
        outputVariable.setKeyValue("*");
        m_variable = outputVariable;
        m_combobox->bind(*m_variable, "reportingFrequency");
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
    connect(t_model.getImpl<openstudio::model::detail::Model_Impl>().get(), 
        SIGNAL(addWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
        this, SLOT(onAdded(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)));

    connect(t_model.getImpl<openstudio::model::detail::Model_Impl>().get(), 
        SIGNAL(removeWorkspaceObject(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)),
        this, SLOT(onRemoved(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&)));

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setContentsMargins(10,10,10,10);
    vbox->setSpacing(10);
    setLayout(vbox);

    vbox->addWidget(new QLabel("<b>Possible Output Variables</b>"));

    QVBoxLayout *innerbox = new QVBoxLayout();
    m_allOnBtn = new QPushButton("All On");
    m_allOnBtn->setFlat(true);
    m_allOnBtn->setObjectName("StandardGrayButton");
    m_allOffBtn = new QPushButton("All Off");
    m_allOffBtn->setFlat(true);
    m_allOffBtn->setObjectName("StandardGrayButton");
    innerbox->addWidget(m_allOnBtn);
    innerbox->addWidget(m_allOffBtn);

    connect(m_allOnBtn, SIGNAL(clicked(bool)),
        this, SLOT(allOnClicked()));
    connect(m_allOffBtn, SIGNAL(clicked(bool)),
        this, SLOT(allOffClicked()));


    QHBoxLayout *outerbox = new QHBoxLayout();
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


  void VariablesList::updateVariableList()
  {
    m_variables.clear();
    while (m_listLayout->count() > 0)
    {
      QLayoutItem *qli = m_listLayout->takeAt(0);
      delete qli->widget();
      delete qli;
    }

    
    // map of variable name to output variable
    std::map<std::string, boost::optional<openstudio::model::OutputVariable> > outputVariableMap;

    // get list of all variable names
    for (const openstudio::model::ModelObject& modelObject : m_model.getModelObjects<openstudio::model::ModelObject>())
    {
      for (const std::string& variableName : modelObject.outputVariableNames())
      {
        LOG(Debug, "Found variableName: " << variableName);
        outputVariableMap[variableName] = boost::none;
      }
    }

    // add all variables to map, allow only one variable per variable name in this application 
    for (openstudio::model::OutputVariable outputVariable : m_model.getConcreteModelObjects<openstudio::model::OutputVariable>())
    {
      if (outputVariableMap.count(outputVariable.variableName()) == 0)
      {
        // there is no place for this outputvariable with the current objects, delete it.
        outputVariable.remove();
      } else {
        if (outputVariableMap[outputVariable.variableName()]) {
          // already have output variable for this name, then remove this object
          outputVariable.remove();
        } else {
          // make sure that key value is set to '*'
          outputVariable.setKeyValue("*");

          outputVariableMap[outputVariable.variableName()] = outputVariable;
        }
      }
    }

    
    for (std::map<std::string, boost::optional<openstudio::model::OutputVariable> >::const_iterator itr = outputVariableMap.begin();
         itr != outputVariableMap.end();
         ++itr)
    {
      QFrame *hline = new QFrame();
      hline->setFrameShape(QFrame::HLine);
      hline->setFrameShadow(QFrame::Sunken);
      m_listLayout->addWidget(hline);

      LOG(Debug, "Creating VariableListItem for: " << itr->first);
      VariableListItem *li = new VariableListItem(itr->first, itr->second, m_model);
      m_listLayout->addWidget(li);
      m_variables.push_back(li);
    }

    m_listLayout->addStretch();

    m_dirty = false;
  }

  VariablesTabView::VariablesTabView(openstudio::model::Model t_model, QWidget * parent)
    : MainTabView("Output Variables",false,parent)
  {
    QScrollArea *scrollarea = new QScrollArea();
    VariablesList *vl = new VariablesList(t_model);
    scrollarea->setWidget(vl);
    scrollarea->setWidgetResizable(true);
    addTabWidget(scrollarea);
    vl->setAutoFillBackground(false);
  }

} // openstudio

