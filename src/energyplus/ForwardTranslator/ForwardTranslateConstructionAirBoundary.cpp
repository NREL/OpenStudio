/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ConstructionAirBoundary.hpp"
#include "../../model/ConstructionAirBoundary_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/Construction_AirBoundary_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConstructionAirBoundary(ConstructionAirBoundary& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Construction_AirBoundary);
    m_idfObjects.push_back(idfObject);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    idfObject.setName(modelObject.name().get());

    idfObject.setString(Construction_AirBoundaryFields::SolarandDaylightingMethod, modelObject.solarAndDaylightingMethod());

    idfObject.setString(Construction_AirBoundaryFields::RadiantExchangeMethod, modelObject.radiantExchangeMethod());

    idfObject.setString(Construction_AirBoundaryFields::AirExchangeMethod, modelObject.airExchangeMethod());

    idfObject.setDouble(Construction_AirBoundaryFields::SimpleMixingAirChangesperHour, modelObject.simpleMixingAirChangesPerHour());

    boost::optional<Schedule> schedule = modelObject.simpleMixingSchedule();
    if (schedule) {
      boost::optional<IdfObject> relatedIdfObject = translateAndMapModelObject(*schedule);
      OS_ASSERT(relatedIdfObject);
      idfObject.setString(Construction_AirBoundaryFields::SimpleMixingScheduleName, relatedIdfObject->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
