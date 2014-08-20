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

#ifndef SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP
#define SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/Model.hpp"

#include <QLineEdit>
#include <QPixmap>

namespace openstudio {

class OSLoadNamePixmapLineEdit : public QLineEdit {
  Q_OBJECT

 public:

  OSLoadNamePixmapLineEdit(QWidget * parent = nullptr);

  virtual ~OSLoadNamePixmapLineEdit() {}

  void bind(model::ModelObject& modelObject,
            StringGetter get,
            boost::optional<StringSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void bind(model::ModelObject& modelObject, 
            OptionalStringGetter get,
            boost::optional<StringSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);
  
  void bind(model::ModelObject& modelObject,
            OptionalStringGetterBoolArg get,
            boost::optional<StringSetterOptionalStringReturn> set,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void unbind();

 private slots:

  void onEditingFinished();

  void onModelObjectChange();

  void onModelObjectRemove(Handle handle);

 private:

  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<StringGetter> m_get;
  boost::optional<OptionalStringGetter> m_getOptional;
  boost::optional<OptionalStringGetterBoolArg> m_getOptionalBoolArg;
  boost::optional<StringSetter> m_set;
  boost::optional<StringSetterOptionalStringReturn> m_setOptionalStringReturn;
  boost::optional<NoFailAction> m_reset;
  boost::optional<BasicQuery> m_isDefaulted;

  void completeBind();

  QPixmap * m_pixmap = nullptr;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP

