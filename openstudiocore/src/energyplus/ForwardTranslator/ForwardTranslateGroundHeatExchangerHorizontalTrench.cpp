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
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/GroundHeatExchanger_HorizontalTrench_FieldEnums.hxx>
#include <utilities/idd/Site_GroundTemperature_Undisturbed_KusudaAchenbach_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGroundHeatExchangerHorizontalTrench( 
    GroundHeatExchangerHorizontalTrench & modelObject)
{
  IdfObject idfObject(IddObjectType::GroundHeatExchanger_HorizontalTrench);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // InletNodeName
  if( auto mo = modelObject.inletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::InletNodeName,node->name().get());
    }
  }

  // OutletNodeName
  if( auto mo = modelObject.outletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::OutletNodeName,node->name().get());
    }
  }

  // DesignFlowRate
  {
    auto value = modelObject.designFlowRate();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::DesignFlowRate,value);
  }

  // TrenchLengthinPipeAxialDirection
  {
    auto value = modelObject.trenchLengthinPipeAxialDirection();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::TrenchLengthinPipeAxialDirection,value);
  }

  // NumberofTrenches
  {
    auto value = modelObject.numberofTrenches();
    idfObject.setInt(GroundHeatExchanger_HorizontalTrenchFields::NumberofTrenches,value);
  }

  // HorizontalSpacingBetweenPipes
  {
    auto value = modelObject.horizontalSpacingBetweenPipes();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::HorizontalSpacingBetweenPipes,value);
  }

  // PipeInnerDiameter
  {
    auto value = modelObject.pipeInnerDiameter();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeInnerDiameter,value);
  }

  // PipeOuterDiameter
  {
    auto value = modelObject.pipeOuterDiameter();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeOuterDiameter,value);
  }

  // BurialDepth
  {
    auto value = modelObject.burialDepth();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::BurialDepth,value);
  }

  // SoilThermalConductivity
  {
    auto value = modelObject.soilThermalConductivity();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilThermalConductivity,value);
  }

  // SoilDensity`
  {
    auto value = modelObject.soilDensity();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilDensity,value);
  }

  // SoilSpecificHeat
  {
    auto value = modelObject.soilSpecificHeat();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilSpecificHeat,value);
  }

  // PipeThermalConductivity
  {
    auto value = modelObject.pipeThermalConductivity();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeThermalConductivity,value);
  }

  // PipeDensity
  {
    auto value = modelObject.pipeDensity();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeDensity,value);
  }

  // PipeSpecificHeat
  {
    auto value = modelObject.pipeSpecificHeat();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeSpecificHeat,value);
  }

  // SoilMoistureContentPercent
  {
    auto value = modelObject.soilMoistureContentPercent();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercent,value);
  }

  // SoilMoistureContentPercentatSaturation
  {
    auto value = modelObject.soilMoistureContentPercentatSaturation();
    idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercentatSaturation,value);
  }

  if( istringEqual(modelObject.groundTemperatureModel(),"KusudaAchenbach") ) {
    IdfObject groundModel(IddObjectType::Site_GroundTemperature_Undisturbed_KusudaAchenbach);
    m_idfObjects.push_back(groundModel);

    groundModel.setName(modelObject.name().get() + " Ground Model");

    idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName,groundModel.name().get());

    // KusudaAchenbachAverageSurfaceTemperature
    {
      auto value = modelObject.kusudaAchenbachAverageSurfaceTemperature();
      groundModel.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature,value);
    }

    // KusudaAchenbachAverageAmplitudeofSurfaceTemperature
    {
      auto value = modelObject.kusudaAchenbachAverageAmplitudeofSurfaceTemperature();
      groundModel.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature,value);
    }

    // KusudaAchenbachPhaseShiftofMinimumSurfaceTemperature
    {
      auto value = modelObject.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature();
      groundModel.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature,value);
    }

    // EvapotranspirationGroundCoverParameter
    {
      auto value = modelObject.evapotranspirationGroundCoverParameter();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter,value);
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
