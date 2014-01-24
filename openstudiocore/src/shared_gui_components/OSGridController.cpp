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
  
  //addCheckBoxColumn(QString(""),
  //                  &model::RefrigerationCase::,
  //                  &model::RefrigerationCase::);

  addComboBoxColumn(QString("Defrost Type"),
                    &model::RefrigerationCase::caseDefrostTypeValues,
                    &model::RefrigerationCase::caseDefrostType,
                    &model::RefrigerationCase::setCaseDefrostType);
             
  addDoubleEditColumn(QString("Operating Temperature"),
                    &model::RefrigerationCase::caseOperatingTemperature,
                    &model::RefrigerationCase::setCaseOperatingTemperature);
    
  //addIntegerEditColumn(QString(""),
  //                  &model::RefrigerationCase::,
  //                  &model::RefrigerationCase::);

  addLineEditColumn(QString("Curve Type"),
                    &model::RefrigerationCase::latentCaseCreditCurveType,
                    &model::RefrigerationCase::setLatentCaseCreditCurveType);
             
  //addQuantityEditColumn(QString(""),
  //                  &model::RefrigerationCase::,
  //                  &model::RefrigerationCase::,  
             
  //addUnsignedEditColumn(QString(""),
  //                  &model::RefrigerationCase::,
  //                  &model::RefrigerationCase::);
}

OSGridController::~OSGridController()
{
}

QWidget * OSGridController::widgetAt(int i, int j)
{

  QWidget * result = 0;

  if( j < static_cast<int>(m_checkBoxConcepts.size()) )
  {
    if( i < static_cast<int>(m_modelObjects.size()) )
    {
      model::ModelObject mo = m_modelObjects[i];


      QSharedPointer<CheckBoxConcept> checkBoxConcept = m_checkBoxConcepts[j];
      OSCheckBox2 * checkBox = new OSCheckBox2();

      checkBox->bind(mo,
                boost::bind(&CheckBoxConcept::get,checkBoxConcept.data(),mo),
                boost::optional<BoolSetter>(boost::bind(&CheckBoxConcept::set,checkBoxConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      result = checkBox;


      QSharedPointer<ComboBoxConcept> comboBoxConcept = m_comboBoxConcepts[j];
      OSComboBox2 * comboBox = new OSComboBox2();

      comboBox->bindRequired(mo,
                boost::bind(&ComboBoxConcept::choices,comboBoxConcept.data()),
                boost::bind(&ComboBoxConcept::get,comboBoxConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&ComboBoxConcept::set,comboBoxConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      result = comboBox;


      QSharedPointer<DoubleEditConcept> doubleEdiConcept = m_doubleEditConcepts[j];
      OSDoubleEdit2 * doubleEdit = new OSDoubleEdit2();

      doubleEdit->bindRequired(mo,
                boost::bind(&DoubleEditConcept::get,doubleEdiConcept.data(),mo),
                boost::optional<DoubleSetter>(boost::bind(&DoubleEditConcept::set,doubleEdiConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      result = doubleEdit;


      QSharedPointer<IntegerEditConcept> integerEditConcept = m_integerEditConcepts[j];
      OSIntegerEdit2 * integerEdit = new OSIntegerEdit2();

      integerEdit->bindRequired(mo,
                boost::bind(&IntegerEditConcept::get,integerEditConcept.data(),mo),
                boost::optional<IntSetter>(boost::bind(&IntegerEditConcept::set,integerEditConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      result = integerEdit;


      QSharedPointer<LineEditConcept> lineEditConcept = m_lineEditConcepts[j];
      OSLineEdit2 * lineEdit = new OSLineEdit2();

      lineEdit->bindRequired(mo,
                boost::bind(&LineEditConcept::get,lineEditConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&LineEditConcept::set,lineEditConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      result = lineEdit;


      QSharedPointer<QuantityEditConcept> quantityEditConcept = m_quantityEditConcepts[j];
      OSQuantityEdit2 * quantityEdit = new OSQuantityEdit2("people/m^2", "people/m^2", "people/ft^2", true);

      quantityEdit->bindRequired(true,
                mo,
                boost::bind(&QuantityEditConcept::get,quantityEditConcept.data(),mo),
                boost::optional<DoubleSetter>(boost::bind(&QuantityEditConcept::set,quantityEditConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      result = quantityEdit;


      QSharedPointer<UnsignedEditConcept> unsignedEdiConcept = m_unsignedEditConcepts[j];
      OSUnsignedEdit2 * unsignedEdit = new OSUnsignedEdit2();

      unsignedEdit->bind(mo,
                boost::bind(&UnsignedEditConcept::get,unsignedEdiConcept.data(),mo),
                boost::optional<UnsignedSetter>(boost::bind(&UnsignedEditConcept::set,unsignedEdiConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      result = unsignedEdit;

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
  

  return result;

    
  OS_ASSERT(i >= 0);
  OS_ASSERT(j >= 0);
  OS_ASSERT(m_columnTypes.size() > static_cast<unsigned>(j));  
  OS_ASSERT(m_modelObjects.size() > static_cast<unsigned>(i));

  if( m_columnTypes.at(j) == OSGridController::CHECKBOX ){
  } else if( m_columnTypes.at(j) == OSGridController::COMBOBOX ){
  } else if( m_columnTypes.at(j) == OSGridController::DOUBLE ){
  } else if( m_columnTypes.at(j) == OSGridController::DROPZONE ){
  } else if( m_columnTypes.at(j) == OSGridController::INTEGER ){
  } else if( m_columnTypes.at(j) == OSGridController::LINEEDIT ){
  } else if( m_columnTypes.at(j) == OSGridController::QUANTITY ){
  } else if( m_columnTypes.at(j) == OSGridController::UNSIGNED ){
  } else if( m_columnTypes.at(j) == OSGridController::NOTVALID ){
    OS_ASSERT(false);
  } else {
    OS_ASSERT(false);
  }


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

