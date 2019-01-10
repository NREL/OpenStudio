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
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateDesignSpecificationOutdoorAir( DesignSpecificationOutdoorAir & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::DesignSpecification_OutdoorAir);

  idfObject.setString(DesignSpecification_OutdoorAirFields::Name, modelObject.name().get());

  std::string outdoorAirMethod = modelObject.outdoorAirMethod();
  idfObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, outdoorAirMethod);

  double flowPerPerson = modelObject.outdoorAirFlowperPerson();
  double flowPerArea = modelObject.outdoorAirFlowperFloorArea();
  double flowPerZone = modelObject.outdoorAirFlowRate();
  double ach = modelObject.outdoorAirFlowAirChangesperHour();

  if (istringEqual(outdoorAirMethod, "Sum") || istringEqual(outdoorAirMethod, "Maximum")){

    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, flowPerPerson);
    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea, flowPerArea);
    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone, flowPerZone);
    idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, ach);

  }else{
    LOG(Error, "Unknown OutdoorAirMethod '" << outdoorAirMethod << "' specified for OS:DesignSpecification:OutdoorAir named '" << modelObject.name().get() << "'");
    return boost::none;
  }

  boost::optional<Schedule> schedule = modelObject.outdoorAirFlowRateFractionSchedule();
  if (schedule){
    idfObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirScheduleName, schedule->name().get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

