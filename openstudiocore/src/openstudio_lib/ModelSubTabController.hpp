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

#ifndef OPENSTUDIO_MODELSUBTABCONTROLLER_HPP
#define OPENSTUDIO_MODELSUBTABCONTROLLER_HPP

#include "SubTabController.hpp"
#include "ModelSubTabView.hpp"

#include "../model/Model.hpp"

#include <QObject>

namespace openstudio {

class OSItem;
class OSItemId;
class SubTabView;

namespace model {
  class Component;
}

class ModelSubTabController : public SubTabController
{
  Q_OBJECT

  public:

    ModelSubTabController(ModelSubTabView* subTabView,
                     const model::Model & model);

    virtual ~ModelSubTabController() {}

  signals:

    void modelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

    void toggleUnitsClicked(bool displayIP);

  protected:

    openstudio::model::Model model() const;

    bool fromModel(const OSItemId& itemId) const;

    bool fromComponentLibrary(const OSItemId& itemId) const;

    boost::optional<model::ModelObject> getModelObject(const OSItemId& itemId) const;

    boost::optional<model::Component> getComponent(const OSItemId& itemId) const;

    virtual void onAddObject(const openstudio::IddObjectType& iddObjectType) = 0;

    virtual void onCopyObject(const openstudio::model::ModelObject& modelObject) = 0;

    virtual void onRemoveObject(openstudio::model::ModelObject modelObject) = 0;

    virtual void onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId) = 0;

    virtual void onPurgeObjects(const openstudio::IddObjectType& iddObjectType) = 0;

    virtual void onDrop(const OSItemId& itemId) = 0;

    virtual void onAddItem();

    virtual void onCopyItem();

    virtual void onRemoveItem(OSItem *item);

    virtual void onReplaceItem(OSItem *item, const OSItemId& replacementItemId);

    virtual void onPurgeItems();

  private slots:

  private:

    boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

    openstudio::IddObjectType currentIddObjectType() const;

    openstudio::model::Model m_model;
};

} // openstudio

#endif // OPENSTUDIO_MODELSUBTABCONTROLLER_HPP
