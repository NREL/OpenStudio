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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingWater( CoilCoolingWater & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Coil_Cooling_Water);

  m_idfObjects.push_back(idfObject);

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  Schedule sched = modelObject.availableSchedule();
  boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
  if( _sched )
  {
    idfObject.setString(Coil_Cooling_WaterFields::AvailabilityScheduleName,
                        _sched->name().get() );
  }

  // DesignWaterFlowRate

  if( modelObject.isDesignWaterFlowRateAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.designWaterFlowRate()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignWaterFlowRate,value.get());
  }

  // DesignAirFlowRate

  if( modelObject.isDesignAirFlowRateAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.designAirFlowRate()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignAirFlowRate,value.get());
  }

  // DesignInletWaterTemperature

  if( modelObject.isDesignInletWaterTemperatureAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignInletWaterTemperature,"Autosize");
  }
  else if( (value = modelObject.designInletWaterTemperature()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignInletWaterTemperature,value.get());
  }

  // DesignInletAirTemperature

  if( modelObject.isDesignInletAirTemperatureAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignInletAirTemperature,"Autosize");
  }
  else if( (value = modelObject.designInletAirTemperature()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignInletAirTemperature,value.get());
  }

  // DesignOutletAirTemperature

  if( modelObject.isDesignOutletAirTemperatureAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignOutletAirTemperature,"Autosize");
  }
  else if( (value = modelObject.designOutletAirTemperature()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignOutletAirTemperature,value.get());
  }

  // DesignInletAirHumidityRatio

  if( modelObject.isDesignInletAirHumidityRatioAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignInletAirHumidityRatio,"Autosize");
  }
  else if( (value = modelObject.designInletAirHumidityRatio()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignInletAirHumidityRatio,value.get());
  }

  // DesignOutletAirHumidityRatio

  if( modelObject.isDesignOutletAirHumidityRatioAutosized() )
  {
    idfObject.setString(Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio,"Autosize");
  }
  else if( (value = modelObject.designOutletAirHumidityRatio()) )
  {
    idfObject.setDouble(Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio,value.get());
  }

  // WaterInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.waterInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterFields::WaterInletNodeName,node->name().get());
    }
  }

  // WaterOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.waterOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterFields::WaterOutletNodeName,node->name().get());
    }
  }

  // AirInletNodeName

  if( boost::optional<ModelObject> mo = modelObject.airInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterFields::AirInletNodeName,node->name().get());
    }
  }

  // AirOutletNodeName

  if( boost::optional<ModelObject> mo = modelObject.airOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterFields::AirOutletNodeName,node->name().get());
    }
  }

  // TypeofAnalysis

  s = modelObject.typeOfAnalysis();
  if( s )
  {
    idfObject.setString(Coil_Cooling_WaterFields::TypeofAnalysis,s.get());
  }

  // HeatExchangerConfiguration

  s = modelObject.heatExchangerConfiguration();
  if( s )
  {
    idfObject.setString(Coil_Cooling_WaterFields::HeatExchangerConfiguration,s.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

//((Name)(Name))
//((AvailabilityScheduleName)(Availability Schedule Name))

} // energyplus

} // openstudio

