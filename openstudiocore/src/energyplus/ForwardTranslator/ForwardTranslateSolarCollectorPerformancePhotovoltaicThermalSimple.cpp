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

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalSimple.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformancePhotovoltaicThermalSimple(model::SolarCollectorPerformancePhotovoltaicThermalSimple & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_Simple);

  m_idfObjects.push_back(idfObject);

  boost::optional<double> d;

  idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::Name, modelObject.name().get());

  idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FractionofSurfaceAreawithActiveThermalCollector, modelObject.fractionOfSurfaceAreaWithActiveThermalCollector());

  std::string thermalConversionEfficiencyInputModeType = modelObject.thermalConversionEfficiencyInputModeType();
  idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyInputModeType, thermalConversionEfficiencyInputModeType);

  if (istringEqual("Scheduled", thermalConversionEfficiencyInputModeType)){
    boost::optional<Schedule> schedule = modelObject.thermalConversionEfficiencySchedule();
    if (schedule){
      boost::optional<IdfObject> relatedIdfObject = translateAndMapModelObject(*schedule);
      OS_ASSERT(relatedIdfObject);
      idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ThermalConversionEfficiencyScheduleName, relatedIdfObject->name().get());
    } else{
      LOG(Error, "SolarCollectorPerformance:PhotovoltaicThermal:Simple object named '" << modelObject.name().get() << "' has ThermalConversionEfficiencyInputModeType of Scheduled but has no schedule attached.");
    }
  } else {
    d = modelObject.thermalConversionEfficiency();
    if (d){
      idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::ValueforThermalConversionEfficiencyifFixed, *d);
    }else{
      LOG(Error, "SolarCollectorPerformance:PhotovoltaicThermal:Simple object named '" << modelObject.name().get() << "' has ThermalConversionEfficiencyInputModeType of Fixed but has no value set.");
    }
  }

  idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_SimpleFields::FrontSurfaceEmittance, modelObject.frontSurfaceEmittance());

  return idfObject;
}

} // energyplus

} // openstudio

