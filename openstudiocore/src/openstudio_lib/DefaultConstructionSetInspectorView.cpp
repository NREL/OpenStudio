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

#include "DefaultConstructionSetInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "MainWindow.hpp"

#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultSubSurfaceConstructions.hpp"
#include "../model/DefaultSubSurfaceConstructions_Impl.hpp"
#include "../model/DefaultSurfaceConstructions.hpp"
#include "../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ComponentData.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_DefaultConstructionSet_FieldEnums.hxx>
#include <utilities/idd/OS_DefaultSubSurfaceConstructions_FieldEnums.hxx>
#include <utilities/idd/OS_DefaultSurfaceConstructions_FieldEnums.hxx>

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMessageBox>

#include <sstream>

namespace openstudio {

// WallConstructionVC

void WallConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSurfaceConstructionsFields::WallConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> WallConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSurfaceConstructions.wallConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void WallConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();
    defaultSurfaceConstructions.resetWallConstruction();
  }
}

void WallConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void WallConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();

    unsigned numSources = defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSurfaceConstructions.clone(defaultSurfaceConstructions.model());
        defaultSurfaceConstructions = newObject.cast<model::DefaultSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSurfaceConstructions.setWallConstruction(*constructionBase);
    }
  }
}

// FloorConstructionVC

void FloorConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSurfaceConstructionsFields::FloorConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> FloorConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSurfaceConstructions.floorConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void FloorConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();
    defaultSurfaceConstructions.resetFloorConstruction();
  }
}

void FloorConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void FloorConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();

    unsigned numSources = defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSurfaceConstructions.clone(defaultSurfaceConstructions.model());
        defaultSurfaceConstructions = newObject.cast<model::DefaultSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSurfaceConstructions.setFloorConstruction(*constructionBase);
    }
  }
}

// RoofConstructionVC

void RoofConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSurfaceConstructionsFields::RoofCeilingConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> RoofConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSurfaceConstructions.roofCeilingConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void RoofConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();
    defaultSurfaceConstructions.resetRoofCeilingConstruction();
  }
}

void RoofConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void RoofConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSurfaceConstructions defaultSurfaceConstructions = m_modelObject->cast<model::DefaultSurfaceConstructions>();

    unsigned numSources = defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSurfaceConstructions.clone(defaultSurfaceConstructions.model());
        defaultSurfaceConstructions = newObject.cast<model::DefaultSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSurfaceConstructions.setRoofCeilingConstruction(*constructionBase);
    }
  }
}

// GlassDoorConstructionVC

void GlassDoorConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::GlassDoorConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> GlassDoorConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.glassDoorConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void GlassDoorConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetGlassDoorConstruction();
  }
}

void GlassDoorConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void GlassDoorConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setGlassDoorConstruction(*constructionBase);
    }
  }
}

// OverheadDoorConstructionVC

void OverheadDoorConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::OverheadDoorConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> OverheadDoorConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.overheadDoorConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void OverheadDoorConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetOverheadDoorConstruction();
  }
}

void OverheadDoorConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void OverheadDoorConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setOverheadDoorConstruction(*constructionBase);
    }
  }
}

// SkylightConstructionVC

void SkylightConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::SkylightConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SkylightConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.skylightConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void SkylightConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetSkylightConstruction();
  }
}

void SkylightConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SkylightConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setSkylightConstruction(*constructionBase);
    }
  }
}

// TubularDaylightDomeConstructionVC

void TubularDaylightDomeConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDomeConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> TubularDaylightDomeConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.tubularDaylightDomeConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void TubularDaylightDomeConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetTubularDaylightDomeConstruction();
  }
}

void TubularDaylightDomeConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void TubularDaylightDomeConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setTubularDaylightDomeConstruction(*constructionBase);
    }
  }
}

// TubularDaylightDiffuserConstructionVC

void TubularDaylightDiffuserConstructionVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::TubularDaylightDiffuserConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> TubularDaylightDiffuserConstructionVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.tubularDaylightDiffuserConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void TubularDaylightDiffuserConstructionVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetTubularDaylightDiffuserConstruction();
  }
}

void TubularDaylightDiffuserConstructionVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void TubularDaylightDiffuserConstructionVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(*constructionBase);
    }
  }
}

// FixedWindowVC

void FixedWindowVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::FixedWindowConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> FixedWindowVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.fixedWindowConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void FixedWindowVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetFixedWindowConstruction();
  }
}

void FixedWindowVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void FixedWindowVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setFixedWindowConstruction(*constructionBase);
    }
  }
}

// OperableWindowVC

void OperableWindowVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::OperableWindowConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> OperableWindowVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.operableWindowConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void OperableWindowVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetOperableWindowConstruction();
  }
}

void OperableWindowVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void OperableWindowVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setOperableWindowConstruction(*constructionBase);
    }
  }
}

// DoorVC

void DoorVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultSubSurfaceConstructionsFields::DoorConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> DoorVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    boost::optional<model::ConstructionBase> constructionBase = defaultSubSurfaceConstructions.doorConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void DoorVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();
    defaultSubSurfaceConstructions.resetDoorConstruction();
  }
}

void DoorVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void DoorVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultSubSurfaceConstructions defaultSubSurfaceConstructions = m_modelObject->cast<model::DefaultSubSurfaceConstructions>();

    unsigned numSources = defaultSubSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size();
    if (numSources > 1){
      QWidget* parent = OSAppBase::instance()->currentDocument()->mainWindow();
      QMessageBox::StandardButton button = QMessageBox::question(parent, "Shared Construction Set Resources", 
        "This construction is shared by " + QString::number(numSources-1) + " other construction sets.\n\n" +
        "Click \"Yes\" to change the construction in all construction sets.\n" + 
        "Click \"No\" to change it in only this construction set.\n" +
        "Click \"Cancel\" to do nothing.",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::No);
      if (button == QMessageBox::Cancel){
        return;
      }else if (button == QMessageBox::No){
        model::ModelObject newObject = defaultSubSurfaceConstructions.clone(defaultSubSurfaceConstructions.model());
        defaultSubSurfaceConstructions = newObject.cast<model::DefaultSubSurfaceConstructions>();
      }
    }

    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultSubSurfaceConstructions.setDoorConstruction(*constructionBase);
    }
  }
}

// InteriorPartitionsVC

void InteriorPartitionsVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultConstructionSetFields::InteriorPartitionConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> InteriorPartitionsVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = defaultConstructionSet.interiorPartitionConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void InteriorPartitionsVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    defaultConstructionSet.resetInteriorPartitionConstruction();
  }
}

void InteriorPartitionsVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void InteriorPartitionsVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultConstructionSet.setInteriorPartitionConstruction(*constructionBase);
    }
  }
}

// SpaceShadingVC

void SpaceShadingVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultConstructionSetFields::SpaceShadingConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceShadingVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = defaultConstructionSet.spaceShadingConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void SpaceShadingVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    defaultConstructionSet.resetSpaceShadingConstruction();
  }
}

void SpaceShadingVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceShadingVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultConstructionSet.setSpaceShadingConstruction(*constructionBase);
    }
  }
}

// BuildingShadingVC

void BuildingShadingVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultConstructionSetFields::BuildingShadingConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> BuildingShadingVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = defaultConstructionSet.buildingShadingConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void BuildingShadingVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    defaultConstructionSet.resetBuildingShadingConstruction();
  }
}

void BuildingShadingVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void BuildingShadingVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultConstructionSet.setBuildingShadingConstruction(*constructionBase);
    }
  }
}

// SiteShadingVC

void SiteShadingVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultConstructionSetFields::SiteShadingConstructionName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SiteShadingVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = defaultConstructionSet.siteShadingConstruction();
    if (constructionBase){
      result.push_back(modelObjectToItemId(*constructionBase, false));
    }
  }
  return result;
}

void SiteShadingVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    defaultConstructionSet.resetSiteShadingConstruction();
  }
}

void SiteShadingVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SiteShadingVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultConstructionSet defaultConstructionSet = m_modelObject->cast<model::DefaultConstructionSet>();
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      defaultConstructionSet.setSiteShadingConstruction(*constructionBase);
    }
  }
}

DefaultConstructionSetInspectorView::DefaultConstructionSetInspectorView(const model::Model& model,
                                                                         QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
  m_nameEdit(NULL),

  m_exteriorWallConstructionDZ(NULL),
  m_exteriorFloorConstructionDZ(NULL),
  m_exteriorRoofConstructionDZ(NULL),
  m_interiorWallConstructionDZ(NULL),
  m_interiorFloorConstructionDZ(NULL),
  m_interiorRoofConstructionDZ(NULL),
  m_groundWallConstructionDZ(NULL),
  m_groundFloorConstructionDZ(NULL),
  m_groundRoofConstructionDZ(NULL),
  m_exteriorFixedWindowDZ(NULL),
  m_exteriorOperableWindowDZ(NULL),
  m_exteriorDoorDZ(NULL),
  m_glassDoorConstructionDZ(NULL),
  m_overheadDoorConstructionDZ(NULL),
  m_skylightConstructionDZ(NULL),
  m_tubularDaylightDomeConstructionDZ(NULL),
  m_tubularDaylightDiffuserConstructionDZ(NULL),
  m_interiorFixedWindowDZ(NULL),
  m_interiorOperableWindowDZ(NULL),
  m_interiorDoorDZ(NULL),
  m_interiorPartitionsDZ(NULL),
  m_spaceShadingDZ(NULL),
  m_buildingShadingDZ(NULL),
  m_siteShadingDZ(NULL),

  m_exteriorWallConstructionVC(NULL),
  m_exteriorFloorConstructionVC(NULL),
  m_exteriorRoofConstructionVC(NULL),
  m_interiorWallConstructionVC(NULL),
  m_interiorFloorConstructionVC(NULL),
  m_interiorRoofConstructionVC(NULL),
  m_groundWallConstructionVC(NULL),
  m_groundFloorConstructionVC(NULL),
  m_groundRoofConstructionVC(NULL),
  m_exteriorFixedWindowVC(NULL),
  m_exteriorOperableWindowVC(NULL),
  m_exteriorDoorVC(NULL),
  m_glassDoorConstructionVC(NULL),
  m_overheadDoorConstructionVC(NULL),
  m_skylightConstructionVC(NULL),
  m_tubularDaylightDomeConstructionVC(NULL),
  m_tubularDaylightDiffuserConstructionVC(NULL),
  m_interiorFixedWindowVC(NULL),
  m_interiorOperableWindowVC(NULL),
  m_interiorDoorVC(NULL),
  m_interiorPartitionsVC(NULL),
  m_spaceShadingVC(NULL),
  m_buildingShadingVC(NULL),
  m_siteShadingVC(NULL),

  m_vectorControllers(std::vector<ModelObjectVectorController *>()),
  m_dropZones(std::vector<OSDropZone *>())
{
  //int padding = 7;

  //QHBoxLayout * hLayout = new QHBoxLayout();
  //hLayout->setContentsMargins(padding,padding,padding,padding);
  //hLayout->setSpacing(padding);
  //hLayout->addWidget(label);
  //hLayout->addWidget(m_nameEdit);
  
  QGridLayout * gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(10,10,10,10);
  gridLayout->setSpacing(10);
  gridLayout->setColumnStretch(3,10);

  gridLayout->setColumnMinimumWidth(0,160);
  gridLayout->setColumnMinimumWidth(1,160);
  gridLayout->setColumnMinimumWidth(2,160);

  int leftCol = 0;
  int middleCol = 1;
  int rightCol = 2;
  int row = 0;

  QLabel * label = NULL;
  
  label = new QLabel();
  label->setText("Name");
  label->setObjectName("H2");

  gridLayout->addWidget(label,row,leftCol);
  row++;

  m_nameEdit = new OSLineEdit();
  gridLayout->addWidget(m_nameEdit,row,leftCol,1,3);
  row++;

  //label = new QLabel();
  //label->setWordWrap(true);
  //label->setText("Surfaces");
  //////label->setContentsMargins(padding,0,padding,0);
  //gridLayout->addWidget(label,row,leftCol);
  //row++;

  QFrame * line;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,leftCol,1,4);
  row++;

  label = new QLabel();
  label->setText("Exterior Surface Constructions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,leftCol,1,3);
  row++;

  label = new QLabel();
  label->setText("Walls");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_exteriorWallConstructionVC = new WallConstructionVC();
  m_vectorControllers.push_back(m_exteriorWallConstructionVC);
  m_exteriorWallConstructionDZ = new OSDropZone(m_exteriorWallConstructionVC);
  m_dropZones.push_back(m_exteriorWallConstructionDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_exteriorWallConstructionDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Floors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_exteriorFloorConstructionVC = new FloorConstructionVC();
  m_vectorControllers.push_back(m_exteriorFloorConstructionVC);
  m_exteriorFloorConstructionDZ = new OSDropZone(m_exteriorFloorConstructionVC);
  m_dropZones.push_back(m_exteriorFloorConstructionDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_exteriorFloorConstructionDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Roofs");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_exteriorRoofConstructionVC = new RoofConstructionVC();
  m_vectorControllers.push_back(m_exteriorRoofConstructionVC);
  m_exteriorRoofConstructionDZ = new OSDropZone(m_exteriorRoofConstructionVC);
  m_dropZones.push_back(m_exteriorRoofConstructionDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_exteriorRoofConstructionDZ,row+1,rightCol);

  row += 2;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,leftCol,1,4);

  row++;

  label = new QLabel();
  label->setText("Interior Surface Constructions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,leftCol,1,3);
  row++;

  label = new QLabel();
  label->setText("Walls");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_interiorWallConstructionVC = new WallConstructionVC();
  m_vectorControllers.push_back(m_interiorWallConstructionVC);
  m_interiorWallConstructionDZ = new OSDropZone(m_interiorWallConstructionVC);
  m_dropZones.push_back(m_interiorWallConstructionDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_interiorWallConstructionDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Floors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_interiorFloorConstructionVC = new FloorConstructionVC();
  m_vectorControllers.push_back(m_interiorFloorConstructionVC);
  m_interiorFloorConstructionDZ = new OSDropZone(m_interiorFloorConstructionVC);
  m_dropZones.push_back(m_interiorFloorConstructionDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_interiorFloorConstructionDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Ceilings");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_interiorRoofConstructionVC = new RoofConstructionVC();
  m_vectorControllers.push_back(m_interiorRoofConstructionVC);
  m_interiorRoofConstructionDZ = new OSDropZone(m_interiorRoofConstructionVC);
  m_dropZones.push_back(m_interiorRoofConstructionDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_interiorRoofConstructionDZ,row+1,rightCol);
  
  row += 2;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,leftCol,1,4);

  row++;

  label = new QLabel();
  label->setText("Ground Contact Surface Constructions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,leftCol,1,3);
  row++;

  label = new QLabel();
  label->setText("Walls");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_groundWallConstructionVC = new WallConstructionVC();
  m_vectorControllers.push_back(m_groundWallConstructionVC);
  m_groundWallConstructionDZ = new OSDropZone(m_groundWallConstructionVC);
  m_dropZones.push_back(m_groundWallConstructionDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_groundWallConstructionDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Floors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_groundFloorConstructionVC = new FloorConstructionVC();
  m_vectorControllers.push_back(m_groundFloorConstructionVC);
  m_groundFloorConstructionDZ = new OSDropZone(m_groundFloorConstructionVC);
  m_dropZones.push_back(m_groundFloorConstructionDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_groundFloorConstructionDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Ceilings");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_groundRoofConstructionVC = new RoofConstructionVC();
  m_vectorControllers.push_back(m_groundRoofConstructionVC);
  m_groundRoofConstructionDZ = new OSDropZone(m_groundRoofConstructionVC);
  m_dropZones.push_back(m_groundRoofConstructionDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_groundRoofConstructionDZ,row+1,rightCol);

  row += 2;

  //label = new QLabel();
  //label->setWordWrap(true);
  //label->setText("Subsurfaces");
  ////label->setContentsMargins(padding,0,padding,0);
  //gridLayout->addWidget(label,row,leftCol);
  //row++;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,leftCol,1,4);
  row++;

  label = new QLabel();
  label->setText("Exterior Sub Surface Constructions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,leftCol,1,3);
  row++;


  label = new QLabel();
  label->setText("Fixed Windows");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_exteriorFixedWindowVC = new FixedWindowVC();
  m_vectorControllers.push_back(m_exteriorFixedWindowVC);
  m_exteriorFixedWindowDZ = new OSDropZone(m_exteriorFixedWindowVC);
  m_dropZones.push_back(m_exteriorFixedWindowDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_exteriorFixedWindowDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Operable Windows");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_exteriorOperableWindowVC = new OperableWindowVC();
  m_vectorControllers.push_back(m_exteriorOperableWindowVC);
  m_exteriorOperableWindowDZ = new OSDropZone(m_exteriorOperableWindowVC);
  m_dropZones.push_back(m_exteriorOperableWindowDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_exteriorOperableWindowDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Doors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_exteriorDoorVC = new DoorVC();
  m_vectorControllers.push_back(m_exteriorDoorVC);
  m_exteriorDoorDZ = new OSDropZone(m_exteriorDoorVC);
  m_dropZones.push_back(m_exteriorDoorDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_exteriorDoorDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setText("Glass Doors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_glassDoorConstructionVC = new GlassDoorConstructionVC();
  m_vectorControllers.push_back(m_glassDoorConstructionVC);
  m_glassDoorConstructionDZ = new OSDropZone(m_glassDoorConstructionVC);
  m_dropZones.push_back(m_glassDoorConstructionDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_glassDoorConstructionDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Overhead Doors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_overheadDoorConstructionVC = new OverheadDoorConstructionVC();
  m_vectorControllers.push_back(m_overheadDoorConstructionVC);
  m_overheadDoorConstructionDZ = new OSDropZone(m_overheadDoorConstructionVC);
  m_dropZones.push_back(m_overheadDoorConstructionDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_overheadDoorConstructionDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Skylights");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_skylightConstructionVC = new SkylightConstructionVC();
  m_vectorControllers.push_back(m_skylightConstructionVC);
  m_skylightConstructionDZ = new OSDropZone(m_skylightConstructionVC);
  m_dropZones.push_back(m_skylightConstructionDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_skylightConstructionDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setText("Tubular Daylight Domes");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_tubularDaylightDomeConstructionVC = new TubularDaylightDomeConstructionVC();
  m_vectorControllers.push_back(m_tubularDaylightDomeConstructionVC);
  m_tubularDaylightDomeConstructionDZ = new OSDropZone(m_tubularDaylightDomeConstructionVC);
  m_dropZones.push_back(m_tubularDaylightDomeConstructionDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_tubularDaylightDomeConstructionDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Tubular Daylight Diffusers");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_tubularDaylightDiffuserConstructionVC = new TubularDaylightDiffuserConstructionVC();
  m_vectorControllers.push_back(m_tubularDaylightDiffuserConstructionVC);
  m_tubularDaylightDiffuserConstructionDZ = new OSDropZone(m_tubularDaylightDiffuserConstructionVC);
  m_dropZones.push_back(m_tubularDaylightDiffuserConstructionDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_tubularDaylightDiffuserConstructionDZ,row+1,middleCol);

  row += 2;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,leftCol,1,4);
  row++;

  label = new QLabel();
  label->setText("Interior Sub Surface Constructions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,leftCol,1,3);
  row++;

  label = new QLabel();
  label->setText("Fixed Windows");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_interiorFixedWindowVC = new FixedWindowVC();
  m_vectorControllers.push_back(m_interiorFixedWindowVC);
  m_interiorFixedWindowDZ = new OSDropZone(m_interiorFixedWindowVC);
  m_dropZones.push_back(m_interiorFixedWindowDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_interiorFixedWindowDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Operable Windows");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_interiorOperableWindowVC = new OperableWindowVC();
  m_vectorControllers.push_back(m_interiorOperableWindowVC);
  m_interiorOperableWindowDZ = new OSDropZone(m_interiorOperableWindowVC);
  m_dropZones.push_back(m_interiorOperableWindowDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_interiorOperableWindowDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Doors");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);  
  m_interiorDoorVC = new DoorVC();
  m_vectorControllers.push_back(m_interiorDoorVC);
  m_interiorDoorDZ = new OSDropZone(m_interiorDoorVC);
  m_dropZones.push_back(m_interiorDoorDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_interiorDoorDZ,row+1,rightCol);

  row += 2;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,leftCol,1,4);
  row++;

  label = new QLabel();
  label->setText("Other Constructions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,leftCol,1,3);
  row++;

  label = new QLabel();
  label->setText("Space Shading");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_spaceShadingVC = new SpaceShadingVC();
  m_vectorControllers.push_back(m_spaceShadingVC);
  m_spaceShadingDZ = new OSDropZone(m_spaceShadingVC);
  m_dropZones.push_back(m_spaceShadingDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_spaceShadingDZ,row+1,leftCol);

  label = new QLabel();
  label->setText("Building Shading");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_buildingShadingVC = new BuildingShadingVC();
  m_vectorControllers.push_back(m_buildingShadingVC);
  m_buildingShadingDZ = new OSDropZone(m_buildingShadingVC);
  m_dropZones.push_back(m_buildingShadingDZ);
  gridLayout->addWidget(label,row,middleCol);
  gridLayout->addWidget(m_buildingShadingDZ,row+1,middleCol);

  label = new QLabel();
  label->setText("Site Shading");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_siteShadingVC = new SiteShadingVC();
  m_vectorControllers.push_back(m_siteShadingVC);
  m_siteShadingDZ = new OSDropZone(m_siteShadingVC);
  m_dropZones.push_back(m_siteShadingDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_siteShadingDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setText("Interior Partitions");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  m_interiorPartitionsVC = new InteriorPartitionsVC();
  m_vectorControllers.push_back(m_interiorPartitionsVC);
  m_interiorPartitionsDZ = new OSDropZone(m_interiorPartitionsVC);
  m_dropZones.push_back(m_interiorPartitionsDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_interiorPartitionsDZ,row+1,leftCol);

  OS_ASSERT(m_vectorControllers.size() == 24);

  configDropZones();

  //QVBoxLayout * mainVLayout = new QVBoxLayout();
  //mainVLayout->setContentsMargins(padding,padding,padding,padding);
  //mainVLayout->setSpacing(0);

  //mainVLayout->addLayout(hLayout);
  //mainVLayout->addLayout(gridLayout);

  QWidget * widget = new QWidget();
  //widget->setLayout(mainVLayout);
  widget->setLayout(gridLayout);

  //int index;
  QWidget * hiddenWidget = new QWidget();
  /*index =*/ this->stackedWidget()->addWidget(hiddenWidget);
  /*index =*/ this->stackedWidget()->addWidget(widget);
}

void DefaultConstructionSetInspectorView::configDropZones()
{
  //int padding = 7;

  for (openstudio::OSDropZone * dropZone : m_dropZones){
    //dropZone->setContentsMargins(padding,0,padding,padding);
    dropZone->setMinItems(0);
    dropZone->setMaxItems(1);
    dropZone->setItemsAcceptDrops(true);
  }
}

void DefaultConstructionSetInspectorView::onClearSelection()
{
  detach();
}

void DefaultConstructionSetInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::DefaultConstructionSet defaultConstructionSet = modelObject.cast<model::DefaultConstructionSet>();
  attach(defaultConstructionSet);
}

void DefaultConstructionSetInspectorView::onUpdate()
{
}

void DefaultConstructionSetInspectorView::attach(openstudio::model::DefaultConstructionSet & defaultConstructionSet)
{

  ///! Exterior Surface
  {
    boost::optional<model::DefaultSurfaceConstructions> defaultExteriorSurfaceConstructions = defaultConstructionSet.defaultExteriorSurfaceConstructions();
    if (!defaultExteriorSurfaceConstructions){
      defaultExteriorSurfaceConstructions = model::DefaultSurfaceConstructions(defaultConstructionSet.model());
      defaultConstructionSet.setDefaultExteriorSurfaceConstructions(*defaultExteriorSurfaceConstructions);
    }
    if (defaultExteriorSurfaceConstructions->getModelObjectSources<model::DefaultConstructionSet>().size() > 1){
      model::ModelObject clone = defaultExteriorSurfaceConstructions->clone(defaultConstructionSet.model());
      defaultExteriorSurfaceConstructions = clone.cast<model::DefaultSurfaceConstructions>();
      defaultConstructionSet.setDefaultExteriorSurfaceConstructions(*defaultExteriorSurfaceConstructions);
    }

    m_exteriorWallConstructionVC->attach(*defaultExteriorSurfaceConstructions);
    m_exteriorWallConstructionVC->reportItems();

    m_exteriorFloorConstructionVC->attach(*defaultExteriorSurfaceConstructions);
    m_exteriorFloorConstructionVC->reportItems();

    m_exteriorRoofConstructionVC->attach(*defaultExteriorSurfaceConstructions);
    m_exteriorRoofConstructionVC->reportItems();
  }

  ///! Interior Surface
  {
    boost::optional<model::DefaultSurfaceConstructions> defaultInteriorSurfaceConstructions = defaultConstructionSet.defaultInteriorSurfaceConstructions();
    if (!defaultInteriorSurfaceConstructions){
      defaultInteriorSurfaceConstructions = model::DefaultSurfaceConstructions(defaultConstructionSet.model());
      defaultConstructionSet.setDefaultInteriorSurfaceConstructions(*defaultInteriorSurfaceConstructions);
    }
    if (defaultInteriorSurfaceConstructions->getModelObjectSources<model::DefaultConstructionSet>().size() > 1){
      model::ModelObject clone = defaultInteriorSurfaceConstructions->clone(defaultConstructionSet.model());
      defaultInteriorSurfaceConstructions = clone.cast<model::DefaultSurfaceConstructions>();
      defaultConstructionSet.setDefaultInteriorSurfaceConstructions(*defaultInteriorSurfaceConstructions);
    }

    m_interiorWallConstructionVC->attach(*defaultInteriorSurfaceConstructions);
    m_interiorWallConstructionVC->reportItems();

    m_interiorFloorConstructionVC->attach(*defaultInteriorSurfaceConstructions);
    m_interiorFloorConstructionVC->reportItems();

    m_interiorRoofConstructionVC->attach(*defaultInteriorSurfaceConstructions);
    m_interiorRoofConstructionVC->reportItems();
  }

  ///! Ground Contact
  {
    boost::optional<model::DefaultSurfaceConstructions> defaultGroundContactSurfaceConstructions = defaultConstructionSet.defaultGroundContactSurfaceConstructions();
    if (!defaultGroundContactSurfaceConstructions){
      defaultGroundContactSurfaceConstructions = model::DefaultSurfaceConstructions(defaultConstructionSet.model());
      defaultConstructionSet.setDefaultGroundContactSurfaceConstructions(*defaultGroundContactSurfaceConstructions);
    }
    if (defaultGroundContactSurfaceConstructions->getModelObjectSources<model::DefaultConstructionSet>().size() > 1){
      model::ModelObject clone = defaultGroundContactSurfaceConstructions->clone(defaultConstructionSet.model());
      defaultGroundContactSurfaceConstructions = clone.cast<model::DefaultSurfaceConstructions>();
      defaultConstructionSet.setDefaultGroundContactSurfaceConstructions(*defaultGroundContactSurfaceConstructions);
    }

    m_groundWallConstructionVC->attach(*defaultGroundContactSurfaceConstructions);
    m_groundWallConstructionVC->reportItems();

    m_groundFloorConstructionVC->attach(*defaultGroundContactSurfaceConstructions);
    m_groundFloorConstructionVC->reportItems();

    m_groundRoofConstructionVC->attach(*defaultGroundContactSurfaceConstructions);
    m_groundRoofConstructionVC->reportItems();
  }

  ///! Exterior Sub Surface
  {
    boost::optional<model::DefaultSubSurfaceConstructions> defaultExteriorSubSurfaceConstructions = defaultConstructionSet.defaultExteriorSubSurfaceConstructions();
    if (!defaultExteriorSubSurfaceConstructions){
      defaultExteriorSubSurfaceConstructions = model::DefaultSubSurfaceConstructions(defaultConstructionSet.model());
      defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(*defaultExteriorSubSurfaceConstructions);
    }
    if (defaultExteriorSubSurfaceConstructions->getModelObjectSources<model::DefaultConstructionSet>().size() > 1){
      model::ModelObject clone = defaultExteriorSubSurfaceConstructions->clone(defaultConstructionSet.model());
      defaultExteriorSubSurfaceConstructions = clone.cast<model::DefaultSubSurfaceConstructions>();
      defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(*defaultExteriorSubSurfaceConstructions);
    }

    m_exteriorFixedWindowVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_exteriorFixedWindowVC->reportItems();

    m_exteriorOperableWindowVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_exteriorOperableWindowVC->reportItems();

    m_exteriorDoorVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_exteriorDoorVC->reportItems();

    m_glassDoorConstructionVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_glassDoorConstructionVC->reportItems();

    m_overheadDoorConstructionVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_overheadDoorConstructionVC->reportItems();

    m_skylightConstructionVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_skylightConstructionVC->reportItems();

    m_tubularDaylightDomeConstructionVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_tubularDaylightDomeConstructionVC->reportItems();

    m_tubularDaylightDiffuserConstructionVC->attach(*defaultExteriorSubSurfaceConstructions);
    m_tubularDaylightDiffuserConstructionVC->reportItems();
  }

  ///! Interior Sub Surface
  {
    boost::optional<model::DefaultSubSurfaceConstructions> defaultInteriorSubSurfaceConstructions = defaultConstructionSet.defaultInteriorSubSurfaceConstructions();
    if (!defaultInteriorSubSurfaceConstructions){
      defaultInteriorSubSurfaceConstructions = model::DefaultSubSurfaceConstructions(defaultConstructionSet.model());
      defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(*defaultInteriorSubSurfaceConstructions);
    }
    if (defaultInteriorSubSurfaceConstructions->getModelObjectSources<model::DefaultConstructionSet>().size() > 1){
      model::ModelObject clone = defaultInteriorSubSurfaceConstructions->clone(defaultConstructionSet.model());
      defaultInteriorSubSurfaceConstructions = clone.cast<model::DefaultSubSurfaceConstructions>();
      defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(*defaultInteriorSubSurfaceConstructions);
    }

    m_interiorFixedWindowVC->attach(*defaultInteriorSubSurfaceConstructions);
    m_interiorFixedWindowVC->reportItems();

    m_interiorOperableWindowVC->attach(*defaultInteriorSubSurfaceConstructions);
    m_interiorOperableWindowVC->reportItems();

    m_interiorDoorVC->attach(*defaultInteriorSubSurfaceConstructions);
    m_interiorDoorVC->reportItems();
  }

  ///! Default

  m_interiorPartitionsVC->attach(defaultConstructionSet);
  m_interiorPartitionsVC->reportItems();

  m_spaceShadingVC->attach(defaultConstructionSet);
  m_spaceShadingVC->reportItems();

  m_buildingShadingVC->attach(defaultConstructionSet);
  m_buildingShadingVC->reportItems();

  m_siteShadingVC->attach(defaultConstructionSet);
  m_siteShadingVC->reportItems();

  m_nameEdit->bind(defaultConstructionSet, "name");

  this->stackedWidget()->setCurrentIndex(1);
}

void DefaultConstructionSetInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  for (ModelObjectVectorController * vc : m_vectorControllers){
    vc->detach();
    vc->reportItems();
  }
}

} // openstudio
