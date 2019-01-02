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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/DaylightingDeviceShelf.hpp"
#include "../../model/DaylightingDeviceShelf_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>

#include <utilities/idd/DaylightingDevice_Shelf_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateDaylightingDeviceShelf( model::DaylightingDeviceShelf & modelObject )
{
  SubSurface window = modelObject.subSurface();
  boost::optional<Space> space = window.space();
  boost::optional<InteriorPartitionSurface> insideShelf = modelObject.insideShelf();
  boost::optional<ShadingSurface> outsideShelf = modelObject.outsideShelf();

  if (!space){
    return boost::none;
  }

  if (!(insideShelf || outsideShelf)){
    return boost::none;
  }

  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::DaylightingDevice_Shelf,
                                                       modelObject);

  idfObject.setString(DaylightingDevice_ShelfFields::WindowName, window.name().get());


  // inside shelf is converted to a surface
  if (insideShelf){

    openstudio::Transformation transformation;
    boost::optional<InteriorPartitionSurfaceGroup> group = insideShelf->interiorPartitionSurfaceGroup();
    if (group){
      transformation = group->transformation();
    }

    Point3dVector vertices = transformation*insideShelf->vertices();
    Surface newSurface(vertices, modelObject.model());
    newSurface.setName(modelObject.name().get());
    newSurface.setSpace(*space);
    newSurface.setAdjacentSurface(newSurface);

    boost::optional<ConstructionBase> construction = insideShelf->construction();
    if (!construction){
      Model t_model = modelObject.model();
      construction = interiorPartitionSurfaceConstruction(t_model);
    }
    OS_ASSERT(construction);
    newSurface.setConstruction(*construction);

    boost::optional<IdfObject> newSurfaceObject = translateAndMapModelObject(newSurface);
    if (newSurfaceObject){
      idfObject.setString(DaylightingDevice_ShelfFields::InsideShelfName, newSurfaceObject->name().get());
    }
  }

  if (outsideShelf){

    idfObject.setString(DaylightingDevice_ShelfFields::OutsideShelfName, outsideShelf->name().get());

    boost::optional<ConstructionBase> construction = outsideShelf->construction();
    if (!construction){
      Model t_model = modelObject.model();
      construction = exteriorSurfaceConstruction(t_model);
    }
    OS_ASSERT(construction);
    idfObject.setString(DaylightingDevice_ShelfFields::OutsideShelfConstructionName, construction->name().get());

    OptionalDouble d = modelObject.viewFactortoOutsideShelf();
    if (d){
      idfObject.setDouble(DaylightingDevice_ShelfFields::ViewFactortoOutsideShelf, *d);
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

