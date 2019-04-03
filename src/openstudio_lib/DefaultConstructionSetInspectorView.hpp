/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef OPENSTUDIO_DEFAULTCONSTRUCTIONSETINSPECTORVIEW_HPP
#define OPENSTUDIO_DEFAULTCONSTRUCTIONSETINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/DefaultConstructionSet.hpp"
#include "../model/Model.hpp"

namespace openstudio {

class OSDropZone;
class OSLineEdit2;

class WallConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~WallConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class FloorConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~FloorConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class RoofConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~RoofConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class FixedWindowVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~FixedWindowVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class OperableWindowVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~OperableWindowVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class DoorVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~DoorVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class GlassDoorConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~GlassDoorConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class OverheadDoorConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~OverheadDoorConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class SkylightConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SkylightConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class TubularDaylightDomeConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~TubularDaylightDomeConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class TubularDaylightDiffuserConstructionVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~TubularDaylightDiffuserConstructionVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class InteriorPartitionsVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~InteriorPartitionsVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class SpaceShadingVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SpaceShadingVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class BuildingShadingVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~BuildingShadingVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class SiteShadingVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SiteShadingVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class AdiabaticSurfaceVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~AdiabaticSurfaceVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class DefaultConstructionSetInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:
    DefaultConstructionSetInspectorView(const model::Model& model,
                              QWidget * parent = nullptr);
    virtual ~DefaultConstructionSetInspectorView() {}

  protected:
    virtual void onClearSelection() override;
    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;
    virtual void onUpdate() override;

  private:
    void attach(openstudio::model::DefaultConstructionSet & defaultConstructionSet);
    void detach();
    void configDropZones();

    OSLineEdit2 * m_nameEdit;

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
    OSDropZone * m_adiabaticSurfaceDZ;

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
    AdiabaticSurfaceVC * m_adiabaticSurfaceVC;

    std::vector<ModelObjectVectorController *> m_vectorControllers;
    std::vector<OSDropZone *> m_dropZones;

    boost::optional<model::DefaultConstructionSet> m_defaultConstructionSet;
};

} // openstudio

#endif // OPENSTUDIO_DEFAULTCONSTRUCTIONSETINSPECTORVIEW_HPP
