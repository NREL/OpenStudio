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

#ifndef OPENSTUDIO_SUBSURFACEINSPECTORVIEW_HPP
#define OPENSTUDIO_SUBSURFACEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

class QLabel;
class QPushButton;
class QColor;

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSComboBox;
class OSDoubleEdit;
class PlanarSurfaceWidget;

namespace model {
  class SubSurface;
  class Surface;
  class Space;
  class SpaceType;
  class BuildingStory;
  class Building;
  class DefaultConstructionSet;
  class DefaultSubSurfaceConstructions;
}

class SubSurfaceConstructionVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SubSurfaceConstructionVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual void onDataChange(const model::ModelObject& modelObject);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);

private:

  void attachOtherModelObjects(const model::SubSurface& subSurface);
  void attachOtherModelObjects(const model::Surface& surface);
  void attachOtherModelObjects(const model::Space& space);
  void attachOtherModelObjects(const model::SpaceType& spaceType);
  void attachOtherModelObjects(const model::BuildingStory& buildingStory);
  void attachOtherModelObjects(const model::Building& building);
  void attachOtherModelObjects(const model::DefaultConstructionSet& defaultConstructionSet);
  void attachOtherModelObjects(const model::DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions);
};

class SubSurfaceOutsideBoundaryConditionObjectVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SubSurfaceOutsideBoundaryConditionObjectVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SubSurfaceInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    SubSurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~SubSurfaceInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::SubSurface& subSurface);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;
    OSComboBox* m_subSurfaceType;
    SubSurfaceConstructionVectorController* m_constructionVectorController;
    OSDropZone* m_constructionDropZone;
    SubSurfaceOutsideBoundaryConditionObjectVectorController* m_outsideBoundaryConditionObjectVectorController;
    OSDropZone* m_outsideBoundaryConditionObjectDropZone;
    OSDoubleEdit* m_multiplier;
    PlanarSurfaceWidget* m_planarSurfaceWidget;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_SUBSURFACEINSPECTORVIEW_HPP

