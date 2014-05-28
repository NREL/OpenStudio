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

#ifndef OPENSTUDIO_SPACEINSPECTORVIEW_HPP
#define OPENSTUDIO_SPACEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

class QLabel;
class QPushButton;
class QColor;

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSCheckBox;
class OSSwitch;
class OSQuantityEdit;
class SpaceLoadInstancesWidget;

namespace model {
  class Space;
}

class SpaceBuildingStoryVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceBuildingStoryVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceThermalZoneVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceThermalZoneVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceSpaceTypeVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceSpaceTypeVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceDefaultConstructionSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceDefaultConstructionSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceDefaultScheduleSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceDefaultScheduleSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceDesignSpecificationOutdoorAirVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceDesignSpecificationOutdoorAirVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();

private:

  void attachOtherModelObjects(const model::Space& space);
};

class SpaceSpaceInfiltrationDesignFlowRateVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceSpaceInfiltrationDesignFlowRateVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onRemoveItem(OSItem* item);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();

private:

  void attachOtherModelObjects(const model::Space& space);
};

class SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onRemoveItem(OSItem* item);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();

private:

  void attachOtherModelObjects(const model::Space& space);
};

class SpaceInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    SpaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~SpaceInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::Space& space);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;
        
    SpaceBuildingStoryVectorController* m_buildingStoryVectorController;
    OSDropZone* m_buildingStoryDropZone;
    SpaceThermalZoneVectorController* m_thermalZoneVectorController;
    OSDropZone* m_thermalZoneDropZone;

    SpaceSpaceTypeVectorController* m_spaceTypeVectorController;
    OSDropZone* m_spaceTypeDropZone;

    SpaceDefaultConstructionSetVectorController* m_defaultConstructionSetVectorController;
    OSDropZone* m_defaultConstructionSetDropZone;
    SpaceDefaultScheduleSetVectorController* m_defaultScheduleSetVectorController;
    OSDropZone* m_defaultScheduleSetDropZone;

    SpaceDesignSpecificationOutdoorAirVectorController* m_designSpecificationOutdoorAirVectorController;
    OSDropZone* m_designSpecificationOutdoorAirDropZone;

    SpaceSpaceInfiltrationDesignFlowRateVectorController* m_spaceInfiltrationDesignFlowRateVectorController;
    OSDropZone* m_spaceInfiltrationDesignFlowRateDropZone;

    SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController* m_spaceInfiltrationEffectiveLeakageAreaVectorController;
    OSDropZone* m_spaceInfiltrationEffectiveLeakageAreaDropZone;

    OSQuantityEdit* m_directionofRelativeNorthEdit;
    OSSwitch* m_partofTotalFloorAreaSwitch;

    OSQuantityEdit* m_xOriginEdit;
    OSQuantityEdit* m_yOriginEdit;
    OSQuantityEdit* m_zOriginEdit;

    SpaceLoadInstancesWidget* m_spaceLoadInstancesWidget;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_SPACEINSPECTORVIEW_HPP

