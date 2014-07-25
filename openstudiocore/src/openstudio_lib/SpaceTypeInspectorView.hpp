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

#ifndef OPENSTUDIO_SPACETYPEINSPECTORVIEW_HPP
#define OPENSTUDIO_SPACETYPEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/SpaceType.hpp"

class QLabel;
class QPushButton;
class QColor;
class QComboBox;

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSQuantityEdit;
class SpaceLoadInstancesWidget;
class RenderingColorWidget;

class SpaceTypeDefaultConstructionSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeDefaultConstructionSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceTypeDefaultScheduleSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeDefaultScheduleSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceTypeDesignSpecificationOutdoorAirVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeDesignSpecificationOutdoorAirVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();
};

class SpaceTypeSpaceInfiltrationDesignFlowRateVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeSpaceInfiltrationDesignFlowRateVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onRemoveItem(OSItem* item);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();
};

class SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onRemoveItem(OSItem* item);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();
};

class SpaceTypeSpacesVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeSpacesVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onRemoveItem(OSItem* item);

  virtual void onDrop(const OSItemId& itemId);
};

class SpaceTypeUnassignedSpacesVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceTypeUnassignedSpacesVectorController() {}

  virtual void attachModel(const model::Model& model);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
};

class SpaceTypeInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    SpaceTypeInspectorView(const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~SpaceTypeInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  protected slots:

    void editStandardsBuildingType(const QString & text);
    void standardsBuildingTypeChanged(const QString & text);

    void editStandardsSpaceType(const QString & text);
    void standardsSpaceTypeChanged(const QString & text);

  private:

    void attach(openstudio::model::SpaceType& spaceType);

    void detach();

    void populateStandardsBuildingTypes();

    void populateStandardsSpaceTypes();

    boost::optional<openstudio::model::SpaceType> m_spaceType;

    OSLineEdit* m_nameEdit;
    SpaceTypeDefaultConstructionSetVectorController* m_defaultConstructionSetVectorController;
    OSDropZone* m_defaultConstructionSetDropZone;
    SpaceTypeDefaultScheduleSetVectorController* m_defaultScheduleSetVectorController;
    OSDropZone* m_defaultScheduleSetDropZone;
    RenderingColorWidget* m_renderingColorWidget;
    QComboBox* m_standardsBuildingTypeComboBox;
    QComboBox* m_standardsSpaceTypeComboBox;
    SpaceTypeDesignSpecificationOutdoorAirVectorController* m_designSpecificationOutdoorAirVectorController;
    OSDropZone* m_designSpecificationOutdoorAirDropZone;
    SpaceTypeSpaceInfiltrationDesignFlowRateVectorController* m_spaceInfiltrationDesignFlowRateVectorController;
    OSDropZone* m_spaceInfiltrationDesignFlowRateDropZone;
    SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController* m_spaceInfiltrationEffectiveLeakageAreaVectorController;
    OSDropZone* m_spaceInfiltrationEffectiveLeakageAreaDropZone;
    //SpaceTypeSpacesVectorController* m_spacesVectorController;
    //OSDropZone* m_spacesDropZone;
    SpaceLoadInstancesWidget* m_spaceLoadInstancesWidget;
};


} // openstudio

#endif // OPENSTUDIO_SPACETYPEINSPECTORVIEW_HPP

