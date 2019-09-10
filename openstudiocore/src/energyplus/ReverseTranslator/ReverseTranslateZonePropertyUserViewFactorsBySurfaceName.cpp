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

#include "../ReverseTranslator.hpp"

#include "../../model/ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZonePropertyUserViewFactorsBySurfaceName( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ZoneProperty_UserViewFactors_bySurfaceName ){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZonePropertyUserViewFactorsBySurfaceName");
    return boost::none;
  }

  OptionalModelObject result;

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneProperty_UserViewFactors_bySurfaceNameFields::ZoneorZoneListName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ThermalZone>()){
        boost::optional<ThermalZone> thermalZone = modelObject->optionalCast<ThermalZone>();
        ZonePropertyUserViewFactorsBySurfaceName zoneProp = thermalZone.get().getZonePropertyUserViewFactorsBySurfaceName();

        for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()){
          WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
          OptionalWorkspaceObject fromTarget = workspaceGroup.getTarget(ZoneProperty_UserViewFactors_bySurfaceNameExtensibleFields::FromSurface);
          OptionalModelObject fromModelObject = translateAndMapWorkspaceObject(*fromTarget);
          OptionalWorkspaceObject toTarget = workspaceGroup.getTarget(ZoneProperty_UserViewFactors_bySurfaceNameExtensibleFields::ToSurface);
          OptionalModelObject toModelObject = translateAndMapWorkspaceObject(*toTarget);
          OptionalDouble viewFactor = workspaceGroup.getDouble(ZoneProperty_UserViewFactors_bySurfaceNameExtensibleFields::ViewFactor);
          
          // add the view factor
          if (fromModelObject->optionalCast<Surface>() && toModelObject->optionalCast<Surface>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<Surface>().get(), toModelObject->optionalCast<Surface>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<Surface>() && toModelObject->optionalCast<SubSurface>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<Surface>().get(), toModelObject->optionalCast<SubSurface>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<Surface>() && toModelObject->optionalCast<InternalMass>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<Surface>().get(), toModelObject->optionalCast<InternalMass>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<SubSurface>() && toModelObject->optionalCast<SubSurface>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<SubSurface>().get(), toModelObject->optionalCast<SubSurface>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<SubSurface>() && toModelObject->optionalCast<Surface>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<SubSurface>().get(), toModelObject->optionalCast<Surface>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<SubSurface>() && toModelObject->optionalCast<InternalMass>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<SubSurface>().get(), toModelObject->optionalCast<InternalMass>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<InternalMass>() && toModelObject->optionalCast<InternalMass>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<InternalMass>().get(), toModelObject->optionalCast<InternalMass>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<InternalMass>() && toModelObject->optionalCast<Surface>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<InternalMass>().get(), toModelObject->optionalCast<Surface>().get(), *viewFactor);
          } else if (fromModelObject->optionalCast<InternalMass>() && toModelObject->optionalCast<SubSurface>() && viewFactor) {
            zoneProp.addViewFactor(fromModelObject->optionalCast<InternalMass>().get(), toModelObject->optionalCast<SubSurface>().get(), *viewFactor);
          }
        }
      
        result = zoneProp;
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio

