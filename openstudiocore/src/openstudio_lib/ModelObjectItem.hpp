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

#ifndef OPENSTUDIO_MODELOBJECTITEM_HPP
#define OPENSTUDIO_MODELOBJECTITEM_HPP

#include "OSItem.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QWidget>

class QPushButton;
class QLabel;

namespace openstudio {

OSItemId modelObjectToItemId(const openstudio::model::ModelObject& modelObject, bool isDefaulted);

QString modelToSourceId(const openstudio::model::Model& model);

class ModelObjectItem : public OSItem
{
  Q_OBJECT

  public:

    ModelObjectItem(const openstudio::model::ModelObject& modelObject,
                    bool isDefaulted,
                    OSItemType type,
                    QWidget * parent = 0 );

    virtual ~ModelObjectItem() {}

    openstudio::Handle handle() const;

    openstudio::model::ModelObject modelObject() const;

    virtual bool equal(const openstudio::OSItem *) const;

  private slots:

    void onObjectChanged();

  private:

    openstudio::Handle m_handle;

    openstudio::model::ModelObject m_modelObject;
};



} // openstudio

#endif // OPENSTUDIO_MODELOBJECTITEM_HPP

