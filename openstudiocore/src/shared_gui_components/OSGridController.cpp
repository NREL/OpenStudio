/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "OSGridController.hpp"

#include <model/Model_impl.hpp>
#include <model/ModelObject_impl.hpp>
#include <model/RefrigerationCase.hpp>
#include <model/RefrigerationCase_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QWidget>

namespace openstudio {

OSGridController::OSGridController()
  : QObject()
{
}

OSGridController::OSGridController(IddObjectType iddObjectType, model::Model model)
  : QObject(),
  m_model(model),
  m_iddObjectType(iddObjectType),
  m_columnTypes(std::vector<ColumnType>())
{

  model::Schedule schedule = m_model.alwaysOnDiscreteSchedule();
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  m_modelObjects.push_back(model::RefrigerationCase(m_model,schedule));
  
  addComboBoxColumn(QString("Defrost Type"),
                    &model::RefrigerationCase::caseDefrostTypeValues,
                    &model::RefrigerationCase::caseDefrostType,
                    &model::RefrigerationCase::setCaseDefrostType);
                    
}

OSGridController::~OSGridController()
{
}

QWidget * OSGridController::widgetAt(int i, int j)
{
  QWidget * result = 0;

  
  if( j < m_comboBoxConcepts.size() )
  {
    if( i < m_modelObjects.size() )
    {
      QSharedPointer<ComboBoxConcept> comboBoxConcept = m_comboBoxConcepts[j];
      OSComboBox2 * cb2 = new OSComboBox2();

      model::ModelObject mo = m_modelObjects[i];
      cb2->bindRequired(mo,
                boost::bind(&ComboBoxConcept::choices,comboBoxConcept.data()),
                boost::bind(&ComboBoxConcept::get,comboBoxConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&ComboBoxConcept::set,comboBoxConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      result = cb2;
    }
  }
  else
  {
    QString string("Hello ");
    QString temp;
    string += "row: ";
    string += temp.setNum(i);  
    string += ", column: ";
    string += temp.setNum(j);

    result = new QLabel(string);
  }
  
  
  //std::vector<model::ModelObject> modelObjects = m_model.getModelObjectsByType(m_iddObjectType).sortSomewayortheother(); TODO
  //std::vector<model::ModelObject> modelObjects = m_model.getModelObjects<m_iddObjectType>();

  //model::ModelObject mo = modelObjects[i];

  //if( m_columnTypes.size() > j && m_columnTypes.at(j) == OSGridController::COMBOBOX )
  //{
  //}

  return result;
}

int OSGridController::rowCount() const
{
  return 5; // TODO
}
   
int OSGridController::columnCount() const
{
  return 5; // TODO
}

std::vector<QWidget *> OSGridController::row(int i)
{
  return std::vector<QWidget *>();
}

} // openstudio

