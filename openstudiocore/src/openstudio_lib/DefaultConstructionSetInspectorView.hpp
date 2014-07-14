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

#ifndef OPENSTUDIO_DEFAULTCONSTRUCTIONSETINSPECTORVIEW_HPP
#define OPENSTUDIO_DEFAULTCONSTRUCTIONSETINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/DefaultConstructionSet.hpp"
#include "../model/Model.hpp"

namespace openstudio {

class OSDropZone;
class OSLineEdit;

class WallConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~WallConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class FloorConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~FloorConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class RoofConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~RoofConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class FixedWindowVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~FixedWindowVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class OperableWindowVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~OperableWindowVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class DoorVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~DoorVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class GlassDoorConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~GlassDoorConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class OverheadDoorConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~OverheadDoorConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class SkylightConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SkylightConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class TubularDaylightDomeConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~TubularDaylightDomeConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class TubularDaylightDiffuserConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~TubularDaylightDiffuserConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class InteriorPartitionsVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~InteriorPartitionsVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class SpaceShadingVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SpaceShadingVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class BuildingShadingVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~BuildingShadingVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class SiteShadingVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SiteShadingVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class DefaultConstructionSetInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:
    DefaultConstructionSetInspectorView(const model::Model& model,
                              QWidget * parent = 0);
    virtual ~DefaultConstructionSetInspectorView() {}

  protected:
    virtual void onClearSelection();
    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);
    virtual void onUpdate();

  private:
    void attach(openstudio::model::DefaultConstructionSet & defaultConstructionSet);
    void detach();
    void configDropZones();

    OSLineEdit * m_nameEdit;

    OSDropZone * m_exteriorWallConstructionDZ;
    OSDropZone * m_exteriorFloorConstructionDZ;
    OSDropZone * m_exteriorRoofConstructionDZ;
    OSDropZone * m_interiorWallConstructionDZ;
    OSDropZone * m_interiorFloorConstructionDZ;
    OSDropZone * m_interiorRoofConstructionDZ;
    OSDropZone * m_groundWallConstructionDZ;
    OSDropZone * m_groundFloorConstructionDZ;
    OSDropZone * m_groundRoofConstructionDZ;
    OSDropZone * m_exteriorFixedWindowDZ;
    OSDropZone * m_exteriorOperableWindowDZ;
    OSDropZone * m_exteriorDoorDZ;
    OSDropZone * m_glassDoorConstructionDZ;
    OSDropZone * m_overheadDoorConstructionDZ;
    OSDropZone * m_skylightConstructionDZ;
    OSDropZone * m_tubularDaylightDomeConstructionDZ;
    OSDropZone * m_tubularDaylightDiffuserConstructionDZ;
    OSDropZone * m_interiorFixedWindowDZ;
    OSDropZone * m_interiorOperableWindowDZ;
    OSDropZone * m_interiorDoorDZ;
    OSDropZone * m_interiorPartitionsDZ;
    OSDropZone * m_spaceShadingDZ;
    OSDropZone * m_buildingShadingDZ;
    OSDropZone * m_siteShadingDZ;

    WallConstructionVC * m_exteriorWallConstructionVC;
    FloorConstructionVC * m_exteriorFloorConstructionVC;
    RoofConstructionVC * m_exteriorRoofConstructionVC;
    WallConstructionVC * m_interiorWallConstructionVC;
    FloorConstructionVC * m_interiorFloorConstructionVC;
    RoofConstructionVC * m_interiorRoofConstructionVC;
    WallConstructionVC * m_groundWallConstructionVC;
    FloorConstructionVC * m_groundFloorConstructionVC;
    RoofConstructionVC * m_groundRoofConstructionVC;
    FixedWindowVC * m_exteriorFixedWindowVC;
    OperableWindowVC * m_exteriorOperableWindowVC;
    DoorVC * m_exteriorDoorVC;
    GlassDoorConstructionVC * m_glassDoorConstructionVC;
    OverheadDoorConstructionVC * m_overheadDoorConstructionVC;
    SkylightConstructionVC * m_skylightConstructionVC;
    TubularDaylightDomeConstructionVC * m_tubularDaylightDomeConstructionVC;
    TubularDaylightDiffuserConstructionVC * m_tubularDaylightDiffuserConstructionVC;
    FixedWindowVC * m_interiorFixedWindowVC;
    OperableWindowVC * m_interiorOperableWindowVC;
    DoorVC * m_interiorDoorVC;
    InteriorPartitionsVC * m_interiorPartitionsVC;
    SpaceShadingVC * m_spaceShadingVC;
    BuildingShadingVC * m_buildingShadingVC;
    SiteShadingVC * m_siteShadingVC;

    std::vector<ModelObjectVectorController *> m_vectorControllers;
    std::vector<OSDropZone *> m_dropZones;
};

} // openstudio

#endif // OPENSTUDIO_DEFAULTCONSTRUCTIONSETINSPECTORVIEW_HPP
