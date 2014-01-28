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

#include <shared_gui_components/OSGridController.hpp>

#include <shared_gui_components/OSCheckBox.hpp>
#include <shared_gui_components/OSComboBox.hpp>
#include <shared_gui_components/OSDoubleEdit.hpp>
#include <shared_gui_components/OSIntegerEdit.hpp>
#include <shared_gui_components/OSLineEdit.hpp>
#include <shared_gui_components/OSQuantityEdit.hpp>
#include <shared_gui_components/OSUnsignedEdit.hpp>

#include <model/Model_impl.hpp>
#include <model/ModelObject_impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QWidget>

namespace openstudio {

OSGridController::OSGridController()
  : QObject()
{
}

OSGridController::OSGridController(IddObjectType iddObjectType,
  model::Model model,
  std::vector<model::ModelObject> modelObjects)
  : QObject(),
  m_baseConcepts(std::vector<QSharedPointer<BaseConcept> >()),
  m_model(model),
  m_modelObjects(modelObjects),
  m_iddObjectType(iddObjectType),
  m_categoriesAndFields(std::vector<std::pair<QString,std::vector<QString>>>())
{
}

OSGridController::~OSGridController()
{
}

std::vector<QString> OSGridController::categories()
{
  std::vector<QString> categories;

  for(unsigned i = 0; i < m_categoriesAndFields.size(); i++){
    categories.push_back(m_categoriesAndFields.at(i).first);
  }

  return categories;
}

std::vector<std::pair<QString,std::vector<QString>>> OSGridController::categoriesAndFields()
{
  return m_categoriesAndFields;
}

void OSGridController::categorySelected(int index)
{
  std::vector<QString> fields = m_categoriesAndFields.at(index).second;
 
  addColumns(fields);
}

QWidget * OSGridController::widgetAt(int i, int j)
{
  OS_ASSERT(i >= 0);
  OS_ASSERT(j >= 0);
  OS_ASSERT(m_modelObjects.size() > static_cast<unsigned>(i));
  OS_ASSERT(m_baseConcepts.size() > static_cast<unsigned>(j));

  model::ModelObject mo = m_modelObjects[i];

  QSharedPointer<BaseConcept> baseConcept = m_baseConcepts[j];

  if(QSharedPointer<CheckBoxConcept> checkBoxConcept = baseConcept.dynamicCast<CheckBoxConcept>()){

    OSCheckBox2 * checkBox = new OSCheckBox2();

    checkBox->bind(mo,
              boost::bind(&CheckBoxConcept::get,checkBoxConcept.data(),mo),
              boost::optional<BoolSetter>(boost::bind(&CheckBoxConcept::set,checkBoxConcept.data(),mo,_1)),
              boost::none,
              boost::none);

    return checkBox;

  } else if(QSharedPointer<ComboBoxConcept> comboBoxConcept = baseConcept.dynamicCast<ComboBoxConcept>()) {

      OSComboBox2 * comboBox = new OSComboBox2();

      comboBox->bindRequired(mo,
                boost::bind(&ComboBoxConcept::choices,comboBoxConcept.data()),
                boost::bind(&ComboBoxConcept::get,comboBoxConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&ComboBoxConcept::set,comboBoxConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      return comboBox;

  } else if(QSharedPointer<DoubleEditConcept> doubleEditConcept = baseConcept.dynamicCast<DoubleEditConcept>()) {

      OSDoubleEdit2 * doubleEdit = new OSDoubleEdit2();

      doubleEdit->bindRequired(mo,
                boost::bind(&DoubleEditConcept::get,doubleEditConcept.data(),mo),
                boost::optional<DoubleSetter>(boost::bind(&DoubleEditConcept::set,doubleEditConcept.data(),mo,_1)),
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none,
                boost::none);

      return doubleEdit;

  } else if(QSharedPointer<IntegerEditConcept> integerEditConcept = baseConcept.dynamicCast<IntegerEditConcept>()) {

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

      return integerEdit;

  } else if(QSharedPointer<LineEditConcept> lineEditConcept = baseConcept.dynamicCast<LineEditConcept>()) {

      OSLineEdit2 * lineEdit = new OSLineEdit2();

      lineEdit->bindRequired(mo,
                boost::bind(&LineEditConcept::get,lineEditConcept.data(),mo),
                boost::optional<StringSetter>(boost::bind(&LineEditConcept::set,lineEditConcept.data(),mo,_1)),
                boost::none,
                boost::none);

      return lineEdit;

  } else if(QSharedPointer<QuantityEditConcept> quantityEditConcept = baseConcept.dynamicCast<QuantityEditConcept>()) {

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

      return quantityEdit;

  } else if(QSharedPointer<UnsignedEditConcept> unsignedEdiConcept = baseConcept.dynamicCast<UnsignedEditConcept>()) {

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

      return unsignedEdit;

  } else {
    // Unknown type
    OS_ASSERT(false);
    return new QWidget();
  }
}

int OSGridController::rowCount() const
{
  return m_modelObjects.size();
}
   
int OSGridController::columnCount() const
{
  return m_baseConcepts.size();
}

std::vector<QWidget *> OSGridController::row(int i)
{
  return std::vector<QWidget *>();
}

} // openstudio

