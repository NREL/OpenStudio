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

#ifndef OPENSTUDIO_VARIABLESTABVIEW_HPP
#define OPENSTUDIO_VARIABLESTABVIEW_HPP

#include "MainTabView.hpp"
#include "../model/Model.hpp"
#include "../model/OutputVariable.hpp"
#include <boost/optional.hpp>

class QComboBox;
class QPushButton;
class QVBoxLayout;

namespace openstudio {
  class OSSwitch;
  class OSComboBox;

  class VariableListItem : public QWidget
  {
    Q_OBJECT;

    public:
      VariableListItem(const std::string &t_name, 
          const boost::optional<openstudio::model::OutputVariable> &t_variable,
          const openstudio::model::Model &t_model);

      virtual ~VariableListItem() { }

    public slots:
      void setVariableEnabled(bool);

    private slots:
      void onOffClicked(bool);
      void indexChanged(const QString &t_frequency);

    private:
      REGISTER_LOGGER("openstudio.VariableListItem");

      std::string m_name;
      boost::optional<openstudio::model::OutputVariable> m_variable;
      openstudio::model::Model m_model;

      OSComboBox *m_combobox;
      OSSwitch *m_onOffButton;
  };

  class VariablesList : public QWidget
  {
    Q_OBJECT;

    public:
      VariablesList(openstudio::model::Model t_model);
      virtual ~VariablesList();

    private slots:
      void onAdded(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&);
      void onRemoved(const WorkspaceObject&, const openstudio::IddObjectType&, const openstudio::UUID&);

      void allOnClicked();
      void allOffClicked();

      void enableAll(bool);
      void updateVariableList();

    private:
      REGISTER_LOGGER("openstudio.VariablesList");
      openstudio::model::Model m_model;
      QPushButton *m_allOnBtn;
      QPushButton *m_allOffBtn;
      QVBoxLayout *m_listLayout;
      bool m_dirty;
      std::vector<VariableListItem *> m_variables; 
  };


  class VariablesTabView : public MainTabView
  {
    Q_OBJECT;

    public:

      VariablesTabView(openstudio::model::Model t_model, QWidget * parent = 0);

      virtual ~VariablesTabView() {}

    private:
      REGISTER_LOGGER("openstudio.VariablesTabView");
  };

} // openstudio

#endif // OPENSTUDIO_VARIABLESTABVIEW_HPP
