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

#ifndef OPENSTUDIO_OSQUANTITYEDIT_HPP
#define OPENSTUDIO_OSQUANTITYEDIT_HPP

#include <model/ModelObject.hpp>

#include <utilities/units/Unit.hpp>
#include <utilities/core/Logger.hpp>

#include <boost/function.hpp>

#include <QLineEdit>
#include <QLabel>

namespace openstudio {

class Unit;

typedef boost::function<OSOptionalQuantity (bool)> QuantityGetter;
typedef boost::function<bool (const Quantity&)> QuantitySetter;
typedef boost::function<void ()> NoFailAction;
typedef boost::function<bool ()> BasicQuery;

class OSQuantityEdit2: public QWidget {
  Q_OBJECT
 public:

  OSQuantityEdit2(bool isIP, QWidget * parent = 0);

  virtual ~OSQuantityEdit2() {}

  // Ok to have one getter bind that returns OSOptionalQuantity? Check with field that returns Quantity.
  // (Use PeopleDefinition--has both.)
  void bind(bool isIP,
            model::ModelObject& modelObject,
            QuantityGetter get,
            boost::optional<QuantitySetter> set=boost::none,
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
  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<QuantityGetter> m_get;
  boost::optional<QuantitySetter> m_set;
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

  REGISTER_LOGGER("openstudio.OSQuantityEdit");
};

/** \deprecated Use OSQuantityEdit2. */
class OSQuantityEdit: public QWidget {
  Q_OBJECT

 public:

  OSQuantityEdit(bool isIP, QWidget * parent = 0);

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

#endif // OPENSTUDIO_OSQUANTITYEDIT_HPP

