/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ExteriorLights.hpp"
#include "../../model/ExteriorLightsDefinition.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Exterior_Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateExteriorLights(
    model::ExteriorLights& modelObject)
{
  IdfObject idfObject(IddObjectType::Exterior_Lights);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(Exterior_LightsFields::Name, modelObject.name().get());

  OptionalIdfObject relatedIdfObject;

  boost::optional<model::Schedule> schedule;
  if( auto t_schedule = modelObject.schedule() ) {
    schedule = t_schedule.get();
  } else {
    schedule = modelObject.model().alwaysOnDiscreteSchedule();
  }
  OS_ASSERT(schedule);
  relatedIdfObject = translateAndMapModelObject(schedule.get());
  OS_ASSERT(relatedIdfObject);
  idfObject.setString(Exterior_LightsFields::ScheduleName,relatedIdfObject->name().get());

  model::ExteriorLightsDefinition definition = modelObject.exteriorLightsDefinition();

  double designLevel = definition.designLevel()*modelObject.multiplier();
  idfObject.setDouble(Exterior_LightsFields::DesignLevel,designLevel);

  if (!modelObject.isControlOptionDefaulted()) {
    idfObject.setString(Exterior_LightsFields::ControlOption,modelObject.controlOption());
  }

  if (!modelObject.isEndUseSubcategoryDefaulted()) {
    idfObject.setString(Exterior_LightsFields::EndUseSubcategory,modelObject.endUseSubcategory());
  }

  return idfObject;
}

} // energyplus
} // openstudio
