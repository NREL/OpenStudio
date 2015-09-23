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

#include "ConstructionObjectVectorController.hpp"

#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_Construction_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <QMessageBox>

namespace openstudio {

// ConstructionObjectVectorController

ConstructionObjectVectorController::ConstructionObjectVectorController(QWidget * parentWidget)
  : ModelObjectVectorController(),
  m_parentWidget(parentWidget)
{
}

void ConstructionObjectVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  emit itemIds(makeVector());
}

std::vector<OSItemId> ConstructionObjectVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if(m_modelObject){
    model::LayeredConstruction construction = m_modelObject->cast<model::LayeredConstruction>();
    std::vector<model::Material> layers = construction.layers();
    for (model::Material layer : layers){
      result.push_back(modelObjectToItemId(layer, false));
    }
  }
  return result;
}

void ConstructionObjectVectorController::onRemoveItem(OSItem * item)
{
  if(m_modelObject){
    model::LayeredConstruction construction = m_modelObject->cast<model::LayeredConstruction>();
    std::vector<model::Material> layers = construction.layers();
    OSAppBase * app = OSAppBase::instance();
    unsigned idx = 0;
    for (model::Material layer : layers){
      boost::optional<model::ModelObject> modelObject = app->currentDocument()->getModelObject(item->itemId());
      if(modelObject){
        if(modelObject->handle() == layer.handle()){
          construction.eraseLayer(idx);
          break;
        }
        idx++;
      }
    }
  }
}

void ConstructionObjectVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void ConstructionObjectVectorController::onDrop(const OSItemId& itemId)
{
  if(m_modelObject){
    boost::optional<model::Material> material = this->addToModel<model::Material>(itemId);
    if(!material) return;
    
    model::LayeredConstruction construction = m_modelObject->cast<model::LayeredConstruction>();
    std::vector<model::Material> layers =  construction.layers();
    if(layers.size()){

      IddObjectType existingIddObjectType = layers.at(0).iddObjectType();
      IddObjectType newIddObjectType = material.get().iddObjectType();

      LayerType existingLayerType = getLayerType(existingIddObjectType);
      LayerType newLayerType = getLayerType(newIddObjectType);

      // Need a valid widget to hang the msgbox on
      OS_ASSERT(this->parentWidget());

      if(existingLayerType == ConstructionObjectVectorController::AIRWALL){
        // Only 1 layer allowed for AirWall
        QMessageBox::warning(this->parentWidget(), 
          "Error Adding Layer",
          "Only 1 layer allowed for an AirWall.",
          QMessageBox::Ok);
        return;
      }
      else if(newLayerType != existingLayerType){
        // New layer type must match existing layer type
        QMessageBox::warning(this->parentWidget(), 
          "Error Adding Layer",
          "New layer type must match existing layer type.",
          QMessageBox::Ok);
        return;
      }
    }
    construction.insertLayer(construction.numLayers(),*material);
  }
}

QWidget * ConstructionObjectVectorController::parentWidget()
{
  return m_parentWidget;
}

void ConstructionObjectVectorController::setParentWidget(QWidget * parentWidget)
{
  m_parentWidget = parentWidget;
}

ConstructionObjectVectorController::LayerType ConstructionObjectVectorController::getLayerType(IddObjectType iddObjectType)
{
  if(iddObjectType == IddObjectType::OS_WindowMaterial_Blind ||
     iddObjectType == IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice ||
     iddObjectType == IddObjectType::OS_WindowMaterial_Gas ||
     iddObjectType == IddObjectType::OS_WindowMaterial_GasMixture ||
     iddObjectType == IddObjectType::OS_WindowMaterial_Glazing ||
     iddObjectType == IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod ||
     iddObjectType == IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic ||
     iddObjectType == IddObjectType::OS_WindowMaterial_Screen ||
     iddObjectType == IddObjectType::OS_WindowMaterial_Shade ||
     iddObjectType == IddObjectType::OS_WindowMaterial_SimpleGlazingSystem)
  {
    return ConstructionObjectVectorController::FENESTRATION;
  }
  else if(iddObjectType == IddObjectType::OS_Material ||
          iddObjectType == IddObjectType::OS_Material_AirGap ||
          iddObjectType == IddObjectType::OS_Material_InfraredTransparent ||
          iddObjectType == IddObjectType::OS_Material_NoMass ||
          iddObjectType == IddObjectType::OS_Material_RoofVegetation){
    return ConstructionObjectVectorController::OPAQUE;
  }
  else if(iddObjectType == IddObjectType::OS_Material_AirWall){
    return ConstructionObjectVectorController::AIRWALL;
  }
  else{
    // Should never get here
    OS_ASSERT(false);
    return ConstructionObjectVectorController::UNKNOWN;
  }
}

} // openstudio
