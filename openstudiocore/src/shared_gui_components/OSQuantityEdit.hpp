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

#ifndef SHAREDGUICOMPONENTS_OSQUANTITYEDIT_HPP
#define SHAREDGUICOMPONENTS_OSQUANTITYEDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/ModelObject.hpp"

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Logger.hpp"

#include <QLineEdit>
#include <QLabel>

namespace openstudio {

class Unit;

class OSQuantityEdit2: public QWidget {
  Q_OBJECT
 public:

  OSQuantityEdit2(const std::string& modelUnits, const std::string& siUnits, 
                  const std::string& ipUnits, bool isIP, QWidget * parent = nullptr);

  virtual ~OSQuantityEdit2() {}

  void bind(bool isIP,
            model::ModelObject& modelObject,
            DoubleGetter get,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(bool isIP,
            model::ModelObject& modelObject,
            OptionalDoubleGetter optionalGet,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(bool isIP,
            model::ModelObject& modelObject,
            DoubleGetter get,
            DoubleSetterVoidReturn set,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(bool isIP,
            model::ModelObject& modelObject,
            OptionalDoubleGetter optionalGet,
            DoubleSetterVoidReturn set,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void unbind();

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onUnitSystemChange(bool isIP);

  void onModelObjectRemove(Handle handle);

 private:

  QLineEdit* m_lineEdit;
  QLabel* m_units;

  bool m_isIP;
  std::string m_modelUnits;
  std::string m_siUnits;
  std::string m_ipUnits;
  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<DoubleGetter> m_get;
  boost::optional<OptionalDoubleGetter> m_optionalGet;
  boost::optional<DoubleSetter> m_set;
  boost::optional<DoubleSetterVoidReturn> m_setVoidReturn;
  boost::optional<NoFailAction> m_reset;
  boost::optional<NoFailAction> m_autosize;
  boost::optional<NoFailAction> m_autocalculate;
  boost::optional<BasicQuery> m_isDefaulted;
  boost::optional<BasicQuery> m_isAutosized;
  boost::optional<BasicQuery> m_isAutocalculated;

  bool m_isScientific;
  boost::optional<int> m_precision;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  void completeBind(bool isIP,
                    model::ModelObject& modelObject,
                    boost::optional<NoFailAction> reset,
                    boost::optional<NoFailAction> autosize,
                    boost::optional<NoFailAction> autocalculate,
                    boost::optional<BasicQuery> isDefaulted,
                    boost::optional<BasicQuery> isAutosized,
                    boost::optional<BasicQuery> isAutocalculated);

  REGISTER_LOGGER("openstudio.OSQuantityEdit");
};

/** \deprecated Use OSQuantityEdit2. */
class OSQuantityEdit: public QWidget {
  Q_OBJECT

 public:

  OSQuantityEdit(bool isIP, QWidget * parent = nullptr);

  virtual ~OSQuantityEdit() {}

  void bind(model::ModelObject& modelObject,
            const char* property,
            bool isIP,
            const boost::optional<std::string>& isDefaultedProperty = boost::none,
            const boost::optional<std::string>& isAutosizedProperty = boost::none,
            const boost::optional<std::string>& isAutocalucatedProperty = boost::none);

  void unbind();

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onUnitSystemChange(bool isIP);

  void onModelObjectRemove(Handle handle);

 private:

  QLineEdit* m_lineEdit;
  QLabel* m_units;

  boost::optional<model::ModelObject> m_modelObject;
  std::string m_property;
  bool m_isIP;
  boost::optional<std::string> m_isDefaultedProperty;
  boost::optional<std::string> m_isAutosizedProperty;
  boost::optional<std::string> m_isAutocalculatedProperty;

  bool m_isScientific;
  boost::optional<int> m_precision;

  std::string propertyWithSystem() const;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  REGISTER_LOGGER("openstudio.OSQuantityEdit");
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSQUANTITYEDIT_HPP

