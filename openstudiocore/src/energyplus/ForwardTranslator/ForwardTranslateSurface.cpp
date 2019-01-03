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
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/SurfacePropertyOtherSideCoefficients.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/FoundationKiva.hpp"
#include "../../model/FoundationKiva_Impl.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSurface( model::Surface & modelObject )
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::BuildingSurface_Detailed,
                                                       modelObject);

  std::string surfaceType = modelObject.surfaceType();
  if (istringEqual("RoofCeiling", surfaceType)){
    if (modelObject.isPartOfEnvelope()){
      surfaceType = "Roof";
    }else{
      surfaceType = "Ceiling";
    }
  }
  idfObject.setString(BuildingSurface_DetailedFields::SurfaceType, surfaceType);

  boost::optional<ConstructionBase> construction = modelObject.construction();
  if (construction){
    idfObject.setString(BuildingSurface_DetailedFields::ConstructionName, construction->name().get());
  }

  boost::optional<Space> space = modelObject.space();
  if (space){
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone){
      idfObject.setString(BuildingSurface_DetailedFields::ZoneName, thermalZone->name().get());
    }
  }

  boost::optional<Surface> adjacentSurface = modelObject.adjacentSurface();
  boost::optional<FoundationKiva> adjacentFoundation = modelObject.adjacentFoundation();
  boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients = modelObject.surfacePropertyOtherSideCoefficients();
  boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel = modelObject.surfacePropertyOtherSideConditionsModel();
  std::string outsideBoundaryCondition = modelObject.outsideBoundaryCondition();
  if (istringEqual("Surface", outsideBoundaryCondition)){
    if (!adjacentSurface){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' has blank Outside Boundary Condition Object.  "
                "Changing Outside Boundary Condition from 'Surface' to 'Adiabatic'");

      outsideBoundaryCondition = "Adiabatic";
    }
  }else if (istringEqual("OtherSideCoefficients", outsideBoundaryCondition)){
    if (!surfacePropertyOtherSideCoefficients){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' has blank Outside Boundary Condition Object.  "
          "Changing Outside Boundary Condition from 'Surface' to 'Adiabatic'");

      outsideBoundaryCondition = "OtherSideCoefficients";
    }
  }else if (istringEqual("OtherSideConditionsModel", outsideBoundaryCondition)){
    if (!surfacePropertyOtherSideConditionsModel){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' has blank Outside Boundary Condition Object.  "
          "Changing Outside Boundary Condition from 'OtherSideConditionsModel' to 'Adiabatic'");

      outsideBoundaryCondition = "Adiabatic";
    }
  }

  if (istringEqual("Adiabatic", outsideBoundaryCondition)){
    std::vector<SubSurface> subSurfaces = modelObject.subSurfaces();
    if (!subSurfaces.empty()){
      LOG(Warn, "Surface '" << modelObject.name().get() << "' is adiabatic, removing all sub surfaces");
      for (auto subSurface : subSurfaces){
        subSurface.remove();
      }
    }
  }

  idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, outsideBoundaryCondition);

  if (adjacentSurface){
    // do not translate and map here, wait for adjacent surface to be translated on its own
    idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentSurface->name().get());
  }

  if (adjacentFoundation){
    // do not translate and map here, wait for adjacent foundation to be translated on its own
    idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentFoundation->name().get());
  }  
  
  if (surfacePropertyOtherSideCoefficients){
    boost::optional<IdfObject> osc = translateAndMapModelObject(*surfacePropertyOtherSideCoefficients);
    if (osc && osc->name()){
      idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, osc->name().get());
    } else{
      LOG(Error, "Surface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
    }
  }

  if (surfacePropertyOtherSideConditionsModel){
    boost::optional<IdfObject> oscm = translateAndMapModelObject(*surfacePropertyOtherSideConditionsModel);
    if (oscm && oscm->name()){
      idfObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, oscm->name().get());
    } else{
      LOG(Error, "Surface '" << modelObject.name().get() << "', could not translate OutsideBoundaryConditionObject");
    }
  }

  if (!modelObject.isSunExposureDefaulted()){
    idfObject.setString(BuildingSurface_DetailedFields::SunExposure, modelObject.sunExposure());
  }

  if (!modelObject.isWindExposureDefaulted()){
    idfObject.setString(BuildingSurface_DetailedFields::WindExposure, modelObject.windExposure());
  }

  boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
  if (viewFactortoGround){
    idfObject.setDouble(BuildingSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
  }

  idfObject.clearExtensibleGroups();
  for (const Point3d& point : modelObject.vertices()){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OS_ASSERT(group.numFields() == 3);
    group.setDouble(0, point.x());
    group.setDouble(1, point.y());
    group.setDouble(2, point.z());
  }

  // translate subsurfaces
  SubSurfaceVector subSurfaces = modelObject.subSurfaces();
  std::sort(subSurfaces.begin(), subSurfaces.end(), WorkspaceObjectNameLess());
  for (SubSurface& subSurface : subSurfaces){
    translateAndMapModelObject(subSurface);
  }

  // create a defaulted SurfacePropertyExposedFoundationPerimeter object if it doesn't exist
  if ((adjacentFoundation) && (surfaceType == "Floor")) {
    if (!modelObject.surfacePropertyExposedFoundationPerimeter()) {
      boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop = modelObject.createSurfacePropertyExposedFoundationPerimeter("ExposedPerimeterFraction", 1);
      translateAndMapModelObject(optprop.get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

