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

#ifndef OPENSTUDIO_SURFACEINSPECTORVIEW_HPP
#define OPENSTUDIO_SURFACEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

class QLabel;
class QPushButton;
class QColor;

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSComboBox;
class PlanarSurfaceWidget;

namespace model {
  class Surface;
  class Space;
  class SpaceType;
  class BuildingStory;
  class Building;
  class DefaultConstructionSet;
  class DefaultSurfaceConstructions;
}

class SurfaceConstructionVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SurfaceConstructionVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual void onDataChange(const model::ModelObject& modelObject);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);

private:

  void attachOtherModelObjects(const model::Surface& surface);
  void attachOtherModelObjects(const model::Space& space);
  void attachOtherModelObjects(const model::SpaceType& spaceType);
  void attachOtherModelObjects(const model::BuildingStory& buildingStory);
  void attachOtherModelObjects(const model::Building& building);
  void attachOtherModelObjects(const model::DefaultConstructionSet& defaultConstructionSet);
  void attachOtherModelObjects(const model::DefaultSurfaceConstructions& defaultSurfaceConstructions);
};

class SurfaceOutsideBoundaryConditionObjectVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SurfaceOutsideBoundaryConditionObjectVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class SurfaceInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    SurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~SurfaceInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::Surface& surface);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;
    OSComboBox* m_surfaceType;
    SurfaceConstructionVectorController* m_constructionVectorController;
    OSDropZone* m_constructionDropZone;
    OSComboBox* m_outsideBoundaryCondition;
    SurfaceOutsideBoundaryConditionObjectVectorController* m_surfaceOutsideBoundaryConditionObjectVectorController;
    OSDropZone* m_outsideBoundaryConditionObjectDropZone;
    OSComboBox* m_sunExposure;
    OSComboBox* m_windExposure;
    PlanarSurfaceWidget* m_planarSurfaceWidget;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_SURFACEINSPECTORVIEW_HPP

