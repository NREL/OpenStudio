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

#ifndef SHAREDGUICOMPONENTS_OSSWITCH_HPP
#define SHAREDGUICOMPONENTS_OSSWITCH_HPP

#include "FieldMethodTypedefs.hpp"

#include "../model/Model.hpp"
#include <QPushButton>

namespace openstudio {

class OSSwitch2 : public QPushButton {
  Q_OBJECT
 public:

  OSSwitch2(QWidget * parent = nullptr);

  virtual ~OSSwitch2() {}

  void bind(model::ModelObject & modelObject,
            BoolGetter get,
            boost::optional<BoolSetter> set=boost::none,
            boost::optional<NoFailAction> reset=boost::none,
            boost::optional<BasicQuery> isDefaulted=boost::none);

  void unbind();

 private slots:

  void onClicked(bool checked);

  void onModelObjectChange();

  void onModelObjectRemove(Handle handle);

 private:
  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<BoolGetter> m_get;
  boost::optional<BoolSetter> m_set;
  boost::optional<NoFailAction> m_reset;
  boost::optional<BasicQuery> m_isDefaulted;
};

class OSSwitch : public QPushButton
{
  Q_OBJECT

  public:

  OSSwitch(QWidget * parent = nullptr);

  virtual ~OSSwitch() {}

  void bind(model::ModelObject & modelObject, const char * property);

  void unbind();

  private slots:

  void onClicked(bool checked);

  void onModelObjectChange();

  void onModelObjectRemove(Handle handle);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  std::string m_property;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSSWITCH_HPP

