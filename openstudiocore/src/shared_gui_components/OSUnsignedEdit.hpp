/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef SHAREDGUICOMPONENTS_OSUNSIGNEDEDIT_HPP
#define SHAREDGUICOMPONENTS_OSUNSIGNEDEDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/ModelObject.hpp"
#include "../model/ModelExtensibleGroup.hpp"

#include "../utilities/core/Logger.hpp"

#include <QLineEdit>
#include <QString>
#include <QValidator>

class QFocusEvent;

namespace openstudio {

class OSUnsignedEdit2: public QLineEdit {
  Q_OBJECT

 public:

  OSUnsignedEdit2(QWidget * parent = nullptr);

  virtual ~OSUnsignedEdit2() {}

  void enableClickFocus() { this->m_hasClickFocus = true; }

  bool hasData() { return !this->text().isEmpty(); }

  QIntValidator * intValidator() { return m_intValidator; }

  void bind(model::ModelObject& modelObject,
            UnsignedGetter get,
            boost::optional<UnsignedSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelObject& modelObject,
            OptionalUnsignedGetter get,
            boost::optional<UnsignedSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelExtensibleGroup& modelExtensibleGroup,
            UnsignedGetter get,
            boost::optional<UnsignedSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelExtensibleGroup& modelExtensibleGroup,
            OptionalUnsignedGetter get,
            boost::optional<UnsignedSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void unbind();
  
protected:

  virtual void focusInEvent(QFocusEvent * e);

  virtual void focusOutEvent(QFocusEvent * e);

 signals:

  void inFocus(bool inFocus, bool hasData);

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onModelObjectRemove(Handle handle);

 private:
  boost::optional<model::ModelObject> m_modelObject; // will be set if attached to ModelObject or ModelExtensibleGroup
  boost::optional<model::ModelExtensibleGroup> m_modelExtensibleGroup; // will only be set if attached to ModelExtensibleGroup
  boost::optional<UnsignedGetter> m_get;
  boost::optional<OptionalUnsignedGetter> m_getOptional;
  boost::optional<UnsignedSetter> m_set;
  boost::optional<NoFailAction> m_reset;
  boost::optional<NoFailAction> m_autosize;
  boost::optional<NoFailAction> m_autocalculate;
  boost::optional<BasicQuery> m_isDefaulted;
  boost::optional<BasicQuery> m_isAutosized;
  boost::optional<BasicQuery> m_isAutocalculated;

  bool m_isScientific;
  bool m_hasClickFocus = false;
  boost::optional<int> m_precision;
  QString m_text = "UNINITIALIZED";
  QIntValidator * m_intValidator = nullptr;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  void completeBind();

  REGISTER_LOGGER("openstudio.OSUnsignedEdit");
};

class OSUnsignedEdit: public QLineEdit {
  Q_OBJECT

 public:

  OSUnsignedEdit(QWidget * parent = nullptr);

  virtual ~OSUnsignedEdit() {}

  QIntValidator * intValidator() { return m_intValidator; }

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
  QIntValidator * m_intValidator = nullptr;

  void refreshTextAndLabel();

  void setPrecision(const std::string& str);

  REGISTER_LOGGER("openstudio.OSUnsignedEdit");
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSUNSIGNEDEDIT_HPP

