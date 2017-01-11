/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "MaterialsController.hpp"

#include "MaterialsView.hpp"

#include "../model/AirGap.hpp"
#include "../model/AirWallMaterial.hpp"
#include "../model/Blind.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/DaylightRedirectionDevice.hpp"
#include "../model/InfraredTransparentMaterial.hpp"
#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/Gas.hpp"
#include "../model/GasMixture.hpp"
#include "../model/RefractionExtinctionGlazing.hpp"
#include "../model/RoofVegetation.hpp"
#include "../model/Screen.hpp"
#include "../model/Shade.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/StandardGlazing.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/ThermochromicGlazing.hpp"

#include "../utilities/core/Logger.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

MaterialsController::MaterialsController(bool isIP, const model::Model& model)
  : ModelSubTabController(new MaterialsView(isIP, model, "Materials", false), model)
{
}

MaterialsController::~MaterialsController()
{
}

void MaterialsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  model::Model model = this->model();
  boost::optional<model::Material> mat;

  switch(iddObjectType.value()){
    case IddObjectType::OS_Material:
      mat = openstudio::model::StandardOpaqueMaterial(model);
      break;
    case IddObjectType::OS_Material_NoMass:
      mat = openstudio::model::MasslessOpaqueMaterial(model);
      break;
    case IddObjectType::OS_Material_AirGap:
      mat = openstudio::model::AirGap(model);
      break;
    case IddObjectType::OS_Material_AirWall:
      mat = openstudio::model::AirWallMaterial(model);
      break;
    case IddObjectType::OS_Material_InfraredTransparent:
      mat = openstudio::model::InfraredTransparentMaterial(model);
      break;
    case IddObjectType::OS_Material_RoofVegetation:
      mat = openstudio::model::RoofVegetation(model);
      break;
    case IddObjectType::OS_WindowMaterial_SimpleGlazingSystem:
      mat = openstudio::model::SimpleGlazing(model);
      break;
    case IddObjectType::OS_WindowMaterial_Glazing:
      mat = openstudio::model::StandardGlazing(model);
      break;
    case IddObjectType::OS_WindowMaterial_Gas:
      mat = openstudio::model::Gas(model);
      break;
    case IddObjectType::OS_WindowMaterial_GasMixture:
      mat = openstudio::model::GasMixture(model);
      break;
    case IddObjectType::OS_WindowMaterial_Blind:
      mat = openstudio::model::Blind(model);
      break;
    case IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice:
      mat = openstudio::model::DaylightRedirectionDevice(model);
      break;
    case IddObjectType::OS_WindowMaterial_Screen:
      mat = openstudio::model::Screen(model);
      break;
    case IddObjectType::OS_WindowMaterial_Shade:
      mat = openstudio::model::Shade(model);
      break;
    case IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod:
      mat = openstudio::model::RefractionExtinctionGlazing(model);
      break;
    case IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic:
      mat = openstudio::model::ThermochromicGlazing(model);
      break;
    default:
      LOG_FREE_AND_THROW("MaterialsController", "Unknown IddObjectType '" << iddObjectType.valueName() << "'");
  }
}

void MaterialsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void MaterialsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void MaterialsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void MaterialsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  this->model().purgeUnusedResourceObjects(iddObjectType);
}

void MaterialsController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if(modelObject){
    if(modelObject->optionalCast<model::Material>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }
  else{
    boost::optional<model::Component> component = this->getComponent(itemId);
    if (component){
      if (component->primaryObject().optionalCast<model::ModelObject>()){
        this->model().insertComponent(*component);
      }
    }
  }
}

void MaterialsController::onInspectItem(OSItem* item)
{
}

} // openstudio

