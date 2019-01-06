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
#include "../../model/RefrigerationSecondarySystem.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/RefrigerationAirChiller.hpp"
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationWalkIn.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Refrigeration_SecondarySystem_FieldEnums.hxx>
#include <utilities/idd/Refrigeration_CaseAndWalkInList_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSecondarySystem( RefrigerationSecondarySystem & modelObject )
{
  OptionalModelObject temp;
  boost::optional<std::string> s;
  boost::optional<double> d;
  boost::optional<int> i;
  std::string name;

// Name
  IdfObject refrigerationSecondarySystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_SecondarySystem,
                                                          modelObject);

//RefrigeratedCaseorWalkinorCaseAndWalkInListName
  std::vector<RefrigerationCase> cases = modelObject.cases();
  std::vector<RefrigerationWalkIn> walkins = modelObject.walkins();
  std::vector<RefrigerationAirChiller> airChillers = modelObject.airChillers();

  if( !cases.empty() || !walkins.empty() || !airChillers.empty() )
  {
    // Name
    name = " Case and Walkin List";
    refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::RefrigeratedCaseorWalkinorCaseAndWalkInListName, refrigerationSecondarySystem.name().get() + name);

    IdfObject _caseAndWalkinList(IddObjectType::Refrigeration_CaseAndWalkInList);

    m_idfObjects.push_back(_caseAndWalkinList);

    _caseAndWalkinList.setName(refrigerationSecondarySystem.name().get() + name);

    for( auto & elem : cases )
    {
      boost::optional<IdfObject> _case = translateAndMapModelObject(elem);

      if( _case )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_case->name().get());
      }
    }

    for( auto & elem : walkins )
    {
      boost::optional<IdfObject> _walkin = translateAndMapModelObject(elem);

      if( _walkin )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_walkin->name().get());
      }
    }

    for( auto & elem : airChillers )
    {
      boost::optional<IdfObject> _airChiller = translateAndMapModelObject(elem);

      if( _airChiller )
      {
        IdfExtensibleGroup eg = _caseAndWalkinList.pushExtensibleGroup();

        eg.setString(Refrigeration_CaseAndWalkInListExtensibleFields::CaseorWalkInName,_airChiller->name().get());
      }
    }
  }

//CirculatingFluidType
//CirculatingFluidName
  s = modelObject.circulatingFluidName();
  if (s) {
    i = modelObject.glycolConcentration();
    boost::optional<IdfObject> fluidProperties;
    if ( i && (istringEqual(s.get(), "PropyleneGlycol") || istringEqual(s.get(), "EthyleneGlycol")) ) {
      fluidProperties = createFluidProperties(s.get(), i.get());
      refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::CirculatingFluidType,"FluidAlwaysLiquid");
    } else {
      fluidProperties = createFluidProperties(s.get());
      refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::CirculatingFluidType,"FluidPhaseChange");
    }
    if( fluidProperties ) {
      boost::optional<std::string> value = fluidProperties.get().getString(FluidProperties_NameFields::FluidName,true);
      if( value ) {
        refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::CirculatingFluidName,value.get());
      }
    }
  }

//EvaporatorCapacity
  d = modelObject.evaporatorCapacity();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::EvaporatorCapacity,d.get());
  }

//EvaporatorFlowRateforSecondaryFluid
  d = modelObject.evaporatorFlowRateforSecondaryFluid();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::EvaporatorFlowRateforSecondaryFluid,d.get());
  }

//EvaporatorEvaporatingTemperature
  d = modelObject.evaporatorEvaporatingTemperature();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::EvaporatorEvaporatingTemperature,d.get());
  }

//EvaporatorApproachTemperatureDifference
  d = modelObject.evaporatorApproachTemperatureDifference();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::EvaporatorApproachTemperatureDifference,d.get());
  }

//EvaporatorRangeTemperatureDifference
  d = modelObject.evaporatorRangeTemperatureDifference();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::EvaporatorRangeTemperatureDifference,d.get());
  }

//NumberofPumpsinLoop
  i = modelObject.numberofPumpsinLoop();
  if (i) {
    refrigerationSecondarySystem.setInt(Refrigeration_SecondarySystemFields::NumberofPumpsinLoop,i.get());
  }

//TotalPumpFlowRate
  d = modelObject.totalPumpFlowRate();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::TotalPumpFlowRate,d.get());
  }

//TotalPumpPower
  d = modelObject.totalPumpPower();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::TotalPumpPower,d.get());
  }

//TotalPumpHead
  d = modelObject.totalPumpHead();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::TotalPumpHead,d.get());
  }

//PhaseChangeCirculatingRate
  d = modelObject.phaseChangeCirculatingRate();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::PhaseChangeCirculatingRate,d.get());
  }

//PumpDriveType
  s = modelObject.pumpDriveType();
  if (s) {
    refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::PumpDriveType,s.get());
  }

//VariableSpeedPumpCubicCurveName
  boost::optional<CurveCubic> variableSpeedPumpCubicCurve = modelObject.variableSpeedPumpCubicCurve();

  if( variableSpeedPumpCubicCurve )
  {
    boost::optional<IdfObject> _variableSpeedPumpCubicCurve = translateAndMapModelObject(variableSpeedPumpCubicCurve.get());

    if( _variableSpeedPumpCubicCurve && _variableSpeedPumpCubicCurve->name() )
    {
      refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::VariableSpeedPumpCubicCurveName,_variableSpeedPumpCubicCurve->name().get());
    }
  }

//PumpMotorHeattoFluid
  d = modelObject.pumpMotorHeattoFluid();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::PumpMotorHeattoFluid,d.get());
  }

//SumUADistributionPiping
  d = modelObject.sumUADistributionPiping();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::SumUADistributionPiping,d.get());
  }

//DistributionPipingZoneName
  boost::optional<ThermalZone> distributionPipingZone = modelObject.distributionPipingZone();

  if( distributionPipingZone )
  {
    boost::optional<IdfObject> _distributionPipingZone = translateAndMapModelObject(distributionPipingZone.get());

    if( _distributionPipingZone && _distributionPipingZone->name() )
    {
      refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::DistributionPipingZoneName,_distributionPipingZone->name().get());
    }
  }

//SumUAReceiver_SeparatorShell
  d = modelObject.sumUAReceiverSeparatorShell();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::SumUAReceiver_SeparatorShell,d.get());
  }

//Receiver_SeparatorZoneName
  boost::optional<ThermalZone> receiverSeparatorZone = modelObject.receiverSeparatorZone();

  if( receiverSeparatorZone )
  {
    boost::optional<IdfObject> _receiverSeparatorZone = translateAndMapModelObject(receiverSeparatorZone.get());

    if( _receiverSeparatorZone && _receiverSeparatorZone->name() )
    {
      refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::Receiver_SeparatorZoneName,_receiverSeparatorZone->name().get());
    }
  }

//EvaporatorRefrigerantInventory
  d = modelObject.evaporatorRefrigerantInventory();
  if (d) {
    refrigerationSecondarySystem.setDouble(Refrigeration_SecondarySystemFields::EvaporatorRefrigerantInventory,d.get());
  }

//EndUseSubcategory
  s = modelObject.endUseSubcategory();
  if (s) {
    refrigerationSecondarySystem.setString(Refrigeration_SecondarySystemFields::EndUseSubcategory,s.get());
  }

  return refrigerationSecondarySystem;

}
}
}
