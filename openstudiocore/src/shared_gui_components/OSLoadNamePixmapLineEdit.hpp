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

#ifndef SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP
#define SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/Model.hpp"

#include <QWidget>

class QLabel;

namespace openstudio {

class OSItem;
class OSLineEdit2;

class OSLoadNamePixmapLineEdit : public QWidget {
  Q_OBJECT

 public:

  OSLoadNamePixmapLineEdit(QWidget * parent = nullptr);

  virtual ~OSLoadNamePixmapLineEdit() {}

  void enableClickFocus();

  void createWidgets();

  void setIcon();

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

signals:

  void itemClicked(OSItem* item);

  void objectRemoved(boost::optional<model::ParentObject> parent);

 private slots:

  void onModelObjectChange();

 private:

  boost::optional<model::ModelObject> m_modelObject;

  void completeBind();

  OSLineEdit2 * m_lineEdit = nullptr;
  QLabel * m_label = nullptr;

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSLOADNAMEPIXMAPLINEEDIT_HPP

