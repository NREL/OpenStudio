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

#include "../../model/IlluminanceMap.hpp"
#include "../../model/IlluminanceMap_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include <utilities/idd/Output_IlluminanceMap_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateOutputIlluminanceMap( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Output_IlluminanceMap ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Output:IlluminanceMap");
    return boost::none;
  }

  IlluminanceMap illuminanceMap( m_model );

  OptionalString s = workspaceObject.name();
  if (s){
    illuminanceMap.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(Output_IlluminanceMapFields::ZoneName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        illuminanceMap.setSpace(modelObject->cast<Space>());
      }
    }
  }

  OptionalDouble d = workspaceObject.getDouble(Output_IlluminanceMapFields::Zheight);
  if (d){
    illuminanceMap.setOriginZCoordinate(*d);
  }

  d = workspaceObject.getDouble(Output_IlluminanceMapFields::XMinimumCoordinate);
  if (d){
    illuminanceMap.setOriginXCoordinate(*d);

    OptionalDouble maxX = workspaceObject.getDouble(Output_IlluminanceMapFields::XMaximumCoordinate);
    if (maxX){
      illuminanceMap.setXLength(*maxX - *d);
    }
  }

  OptionalInt i = workspaceObject.getInt(Output_IlluminanceMapFields::NumberofXGridPoints);
  if (i){
    illuminanceMap.setNumberofXGridPoints(*i);
  }

  d = workspaceObject.getDouble(Output_IlluminanceMapFields::YMinimumCoordinate);
  if (d){
    illuminanceMap.setOriginYCoordinate(*d);

    OptionalDouble maxY = workspaceObject.getDouble(Output_IlluminanceMapFields::YMaximumCoordinate);
    if (maxY){
      illuminanceMap.setYLength(*maxY - *d);
    }
  }

  i = workspaceObject.getInt(Output_IlluminanceMapFields::NumberofYGridPoints);
  if (i){
    illuminanceMap.setNumberofYGridPoints(*i);
  }

  return illuminanceMap;
}

} // energyplus

} // openstudio

