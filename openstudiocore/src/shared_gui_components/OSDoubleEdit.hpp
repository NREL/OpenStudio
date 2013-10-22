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

#ifndef OPENSTUDIO_OSDOUBLEEDIT_HPP
#define OPENSTUDIO_OSDOUBLEEDIT_HPP

#include <shared_gui_components/FieldMethodTypedefs.hpp>

#include <model/ModelObject.hpp>
#include <model/ModelExtensibleGroup.hpp>

#include <utilities/core/Logger.hpp>

#include <QLineEdit>

namespace openstudio {

/** Should only be used for dimensionless real fields. Real fields with units should use
 *  OSQuantityEdit. */
class OSDoubleEdit2: public QLineEdit {
  Q_OBJECT
 public:

  OSDoubleEdit2(QWidget * parent = 0);

  virtual ~OSDoubleEdit2() {}

  void bind(model::ModelObject& modelObject,
            DoubleGetter get,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelObject& modelObject,
            OptionalDoubleGetter get,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelExtensibleGroup& modelExtensibleGroup,
            DoubleGetter get,
            boost::optional<DoubleSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<NoFailAction> autosize=boost::none,
            boost::optional<NoFailAction> autocalculate=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none,
            boost::optional<BasicQuery> isAutosized=boost::none,
            boost::optional<BasicQuery> isAutocalculated=boost::none);

  void bind(model::ModelExtensibleGroup& modelExtensibleGroup,
            OptionalDoubleGetter get,
            boost::optional<DoubleSetter> set=boost::none,
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

  void onModelObjectRemove(Handle handle);

 private:
  boost::optional<model::ModelObject> m_modelObject; // will be set if attached to ModelObject or ModelExtensibleGroup
  boost::optional<model::ModelExtensibleGroup> m_modelExtensibleGroup; // will only be set if attached to ModelExtensibleGroup
  boost::optional<DoubleGetter> m_get;
  boost::optional<OptionalDoubleGetter> m_getOptional;
  boost::optional<DoubleSetter> m_set;
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

  void completeBind();

  REGISTER_LOGGER("openstudio.OSDoubleEdit");
};

/** Should only be used for dimensionless real fields. Real fields with units should use
 *  OSQuantityEdit. */
class OSDoubleEdit: public QLineEdit {
  Q_OBJECT

 public:

  OSDoubleEdit(QWidget * parent = 0);

  virtual ~OSDoubleEdit() {}

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

  REGISTER_LOGGER("openstudio.OSDoubleEdit");
};

} // openstudio

#endif // OPENSTUDIO_OSDOUBLEEDIT_HPP

