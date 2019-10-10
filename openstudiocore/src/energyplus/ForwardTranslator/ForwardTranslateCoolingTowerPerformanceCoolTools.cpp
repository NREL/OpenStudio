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
#include "../../model/CoolingTowerPerformanceCoolTools.hpp"
#include "../../model/CoolingTowerPerformanceCoolTools_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/CoolingTowerPerformance_CoolTools_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoolingTowerPerformanceCoolTools( CoolingTowerPerformanceCoolTools & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::CoolingTowerPerformance_CoolTools);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // MinimumInletAirWetBulbTemperature

  if( (d = modelObject.minimumInletAirWetBulbTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MinimumInletAirWetBulbTemperature,d.get());
  }

  // MaximumInletAirWetBulbTemperature

  if( (d = modelObject.maximumInletAirWetBulbTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MaximumInletAirWetBulbTemperature,d.get());
  }

  // MinimumRangeTemperature

  if( (d = modelObject.minimumRangeTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MinimumRangeTemperature,d.get());
  }

  // MaximumRangeTemperature

  if( (d = modelObject.maximumRangeTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MaximumRangeTemperature,d.get());
  }

  // MinimumApproachTemperature

  if( (d = modelObject.minimumApproachTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MinimumApproachTemperature,d.get());
  }

  // MaximumApproachTemperature

  if( (d = modelObject.maximumApproachTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MaximumApproachTemperature,d.get());
  }

  // MinimumWaterFlowRateRatio

  if( (d = modelObject.minimumWaterFlowRateRatio()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MinimumWaterFlowRateRatio,d.get());
  }

  // MaximumWaterFlowRateRatio

  if( (d = modelObject.maximumWaterFlowRateRatio()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::MaximumWaterFlowRateRatio,d.get());
  }

  // Coefficient1

  if( (d = modelObject.coefficient1()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient1,d.get());
  }

  // Coefficient2

  if( (d = modelObject.coefficient2()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient2,d.get());
  }

  // Coefficient3

  if( (d = modelObject.coefficient3()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient3,d.get());
  }

  // Coefficient4

  if( (d = modelObject.coefficient4()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient4,d.get());
  }

  // Coefficient5

  if( (d = modelObject.coefficient5()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient5,d.get());
  }

  // Coefficient6

  if( (d = modelObject.coefficient6()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient6,d.get());
  }

  // Coefficient7

  if( (d = modelObject.coefficient7()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient7,d.get());
  }

  // Coefficient8

  if( (d = modelObject.coefficient8()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient8,d.get());
  }

  // Coefficient9

  if( (d = modelObject.coefficient9()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient9,d.get());
  }

  // Coefficient10

  if( (d = modelObject.coefficient10()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient10,d.get());
  }

  // Coefficient11

  if( (d = modelObject.coefficient11()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient11,d.get());
  }

  // Coefficient12

  if( (d = modelObject.coefficient12()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient12,d.get());
  }

  // Coefficient13

  if( (d = modelObject.coefficient13()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient13,d.get());
  }

  // Coefficient14

  if( (d = modelObject.coefficient14()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient14,d.get());
  }

  // Coefficient15

  if( (d = modelObject.coefficient15()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient15,d.get());
  }

  // Coefficient16

  if( (d = modelObject.coefficient16()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient16,d.get());
  }

  // Coefficient17

  if( (d = modelObject.coefficient17()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient17,d.get());
  }

  // Coefficient18

  if( (d = modelObject.coefficient18()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient18,d.get());
  }

  // Coefficient19

  if( (d = modelObject.coefficient19()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient19,d.get());
  }

  // Coefficient20

  if( (d = modelObject.coefficient20()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient20,d.get());
  }

  // Coefficient21

  if( (d = modelObject.coefficient21()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient21,d.get());
  }

  // Coefficient22

  if( (d = modelObject.coefficient22()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient22,d.get());
  }

  // Coefficient23

  if( (d = modelObject.coefficient23()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient23,d.get());
  }

  // Coefficient24

  if( (d = modelObject.coefficient24()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient24,d.get());
  }

  // Coefficient25

  if( (d = modelObject.coefficient25()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient25,d.get());
  }

  // Coefficient26

  if( (d = modelObject.coefficient26()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient26,d.get());
  }

  // Coefficient27

  if( (d = modelObject.coefficient27()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient27,d.get());
  }

  // Coefficient28

  if( (d = modelObject.coefficient28()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient28,d.get());
  }

  // Coefficient29

  if( (d = modelObject.coefficient29()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient29,d.get());
  }

  // Coefficient30

  if( (d = modelObject.coefficient30()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient30,d.get());
  }

  // Coefficient31

  if( (d = modelObject.coefficient31()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient31,d.get());
  }

  // Coefficient32

  if( (d = modelObject.coefficient32()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient32,d.get());
  }

  // Coefficient33

  if( (d = modelObject.coefficient33()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient33,d.get());
  }

  // Coefficient34

  if( (d = modelObject.coefficient34()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient34,d.get());
  }

  // Coefficient35

  if( (d = modelObject.coefficient35()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_CoolToolsFields::Coefficient35,d.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

