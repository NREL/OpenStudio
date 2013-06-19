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

#ifndef OPENSTUDIO_OSINTEGEREDIT_HPP
#define OPENSTUDIO_OSINTEGEREDIT_HPP

#include <model/ModelObject.hpp>

#include <utilities/core/Logger.hpp>

#include <QLineEdit>

namespace openstudio {

class OSIntegerEdit: public QLineEdit {
  Q_OBJECT

 public:

  OSIntegerEdit(QWidget * parent = 0);

  virtual ~OSIntegerEdit() {}

  void bind(model::ModelObject& modelObject,
            const char* property,
            const boost::optional<std::string>& isDefaultedProperty = boost::none,
            const boost::optional<std::string>& isAutosizedProperty = boost::none,
            const boost::optional<std::string>& isAutocalucatedProperty = boost::none);

  void unbind();

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onModelObjectRemove(Handle handle);

 private:
  boost::optional<model::ModelObject> m_modelObject;
  std::string m_property;
  boost::optional<std::string> m_isDefaultedProperty;
  boost::optional<std::string> m_isAutosizedProperty;
  boost::optional<std::string> m_isAutocalculatedProperty;

  bool m_isScientific;
  boost::optional<int> m_precision;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  REGISTER_LOGGER("openstudio.OSIntegerEdit");
};

} // openstudio

#endif // OPENSTUDIO_OSINTEGEREDIT_HPP

