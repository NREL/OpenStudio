/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ChillerAbsorptionIndirect.hpp"
#include "../../model/ChillerAbsorptionIndirect_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Chiller_Absorption_Indirect_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateChillerAbsorptionIndirect( ChillerAbsorptionIndirect & modelObject )
{
  IdfObject idfObject(IddObjectType::Chiller_Absorption_Indirect);

  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // ChilledWaterInletNodeName
  if( auto mo = modelObject.supplyInletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(Chiller_Absorption_IndirectFields::ChilledWaterInletNodeName,node->name().get());
    }
  }

  // ChilledWaterOutletNodeName
  if( auto mo = modelObject.supplyOutletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(Chiller_Absorption_IndirectFields::ChilledWaterOutletNodeName,node->name().get());
    }
  }

  // CondenserInletNodeName
  if( auto mo = modelObject.demandInletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(Chiller_Absorption_IndirectFields::CondenserInletNodeName,node->name().get());
    }
  }

  // CondenserOutletNodeName
  if( auto mo = modelObject.demandOutletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(Chiller_Absorption_IndirectFields::CondenserOutletNodeName,node->name().get());
    }
  }

  if( auto mo = modelObject.tertiaryInletModelObject() ) {
    idfObject.setString(Chiller_Absorption_IndirectFields::GeneratorInletNodeName,mo->name().get());
  }

  if( auto mo = modelObject.tertiaryOutletModelObject() ) {
    idfObject.setString(Chiller_Absorption_IndirectFields::GeneratorOutletNodeName,mo->name().get());
  }

  if( modelObject.isNominalCapacityAutosized() ) {
    idfObject.setString(Chiller_Absorption_IndirectFields::NominalCapacity,"Autosize");
  } else if( auto value = modelObject.nominalCapacity() ) {
    idfObject.setDouble(Chiller_Absorption_IndirectFields::NominalCapacity,value.get());
  }

  if( modelObject.isNominalPumpingPowerAutosized() ) {
    idfObject.setString(Chiller_Absorption_IndirectFields::NominalPumpingPower,"Autosize");
  } else if( auto value = modelObject.nominalPumpingPower() ) {
    idfObject.setDouble(Chiller_Absorption_IndirectFields::NominalPumpingPower,value.get());
  }

  {
    auto value = modelObject.minimumPartLoadRatio();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::MinimumPartLoadRatio,value);
  }

  {
    auto value = modelObject.maximumPartLoadRatio();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::MaximumPartLoadRatio,value);
  }

  {
    auto value = modelObject.optimumPartLoadRatio();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::OptimumPartLoadRatio,value);
  }

  {
    auto value = modelObject.designCondenserInletTemperature();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::DesignCondenserInletTemperature,value);
  }

  {
    auto value = modelObject.condenserInletTemperatureLowerLimit();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::CondenserInletTemperatureLowerLimit,value);
  }

  {
    auto value = modelObject.chilledWaterOutletTemperatureLowerLimit();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::ChilledWaterOutletTemperatureLowerLimit,value);
  }

  if( modelObject.isDesignChilledWaterFlowRateAutosized() ) {
    idfObject.setString(Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate,"Autosize");
  } else if( auto value = modelObject.designChilledWaterFlowRate() ) {
    idfObject.setDouble(Chiller_Absorption_IndirectFields::DesignChilledWaterFlowRate,value.get()); 
  }

  if( modelObject.isDesignCondenserWaterFlowRateAutosized() ) {
    idfObject.setString(Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate,"Autosize");
  } else if( auto value = modelObject.designCondenserWaterFlowRate() ) {
    idfObject.setDouble(Chiller_Absorption_IndirectFields::DesignCondenserWaterFlowRate,value.get());
  }

  {
    auto value = modelObject.chillerFlowMode();
    idfObject.setString(Chiller_Absorption_IndirectFields::ChillerFlowMode,value);
  }

  {
    auto curve = modelObject.generatorHeatInputFunctionofPartLoadRatioCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::GeneratorHeatInputFunctionofPartLoadRatioCurveName,idf->name().get());
  }

  {
    auto curve = modelObject.pumpElectricInputFunctionofPartLoadRatioCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::PumpElectricInputFunctionofPartLoadRatioCurveName,idf->name().get());
  }

  {
    auto curve = modelObject.capacityCorrectionFunctionofCondenserTemperatureCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofCondenserTemperatureCurveName,idf->name().get());
  }

  {
    auto curve = modelObject.capacityCorrectionFunctionofChilledWaterTemperatureCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofChilledWaterTemperatureCurveName,idf->name().get());
  }

  {
    auto curve = modelObject.capacityCorrectionFunctionofGeneratorTemperatureCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::CapacityCorrectionFunctionofGeneratorTemperatureCurveName,idf->name().get());
  }

  {
    auto curve = modelObject.generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurveName,idf->name().get());
  }

  {
    auto curve = modelObject.generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve();
    auto idf = translateAndMapModelObject(curve);
    OS_ASSERT(idf);
    idfObject.setString(Chiller_Absorption_IndirectFields::GeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurveName,idf->name().get());
  }

  {
    auto value = modelObject.generatorHeatSourceType();
    idfObject.setString(Chiller_Absorption_IndirectFields::GeneratorHeatSourceType,value);
  }

  if( modelObject.tertiaryPlantLoop() ) {
    if( modelObject.isDesignGeneratorFluidFlowRateAutosized() ) {
      idfObject.setString(Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate,"Autosize");
    } else if( auto value = modelObject.designGeneratorFluidFlowRate() ) {
      idfObject.setDouble(Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate,value.get());
    }
  } else {
    idfObject.setString(Chiller_Absorption_IndirectFields::DesignGeneratorFluidFlowRate,"");
  }

  {
    auto value = modelObject.temperatureLowerLimitGeneratorInlet();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::TemperatureLowerLimitGeneratorInlet,value);
  }

  {
    auto value = modelObject.degreeofSubcoolinginSteamGenerator();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamGenerator,value);
  }

  {
    auto value = modelObject.degreeofSubcoolinginSteamCondensateLoop();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::DegreeofSubcoolinginSteamCondensateLoop,value);
  }

  {
    auto value = modelObject.sizingFactor();
    idfObject.setDouble(Chiller_Absorption_IndirectFields::SizingFactor,value);
  }

  return idfObject;
}

} // energyplus

} // openstudio

