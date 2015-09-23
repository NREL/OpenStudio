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

#ifndef OPENSTUDIO_INTERNALMASSINSPECTORVIEW_HPP
#define OPENSTUDIO_INTERNALMASSINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

namespace openstudio {

namespace model {

class InternalMassDefinition;

}

class OSLineEdit;

class OSQuantityEdit;

class OSDropZone;

class ModelObjectVectorController;

class InternalMassDefinitionInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    InternalMassDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr );

    virtual ~InternalMassDefinitionInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void attach(openstudio::model::InternalMassDefinition& internalMassDefinition);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;

    OSQuantityEdit * m_surfaceAreaEdit;

    OSQuantityEdit * m_surfaceAreaPerSpaceFloorAreaEdit;

    OSQuantityEdit * m_surfaceAreaPerPersonEdit;

    OSDropZone * m_ConstructionDropZone;

    ModelObjectVectorController * m_ConstructionVectorController;

    bool m_isIP;

    class ConstructionVectorController : public ModelObjectVectorController
    {
      public:

      boost::optional<model::InternalMassDefinition> internalMassDefinition();

      protected:

      std::vector<OSItemId> makeVector() override;

      void onChangeRelationship(
             const openstudio::model::ModelObject& modelObject, 
             int index, 
             Handle newHandle, 
             Handle oldHandle) override;

      void onRemoveItem(OSItem* item) override;

      void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

      void onDrop(const OSItemId& itemId) override;
    };

  public slots:

    void toggleUnits(bool displayIP) override;
};

} // openstudio

#endif // OPENSTUDIO_INTERNALMASSINSPECTORVIEW_HPP

