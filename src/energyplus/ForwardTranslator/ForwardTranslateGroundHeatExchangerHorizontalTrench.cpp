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

  boost::optional<IdfObject> ForwardTranslator::translateGroundHeatExchangerHorizontalTrench(GroundHeatExchangerHorizontalTrench& modelObject) {
    IdfObject idfObject(IddObjectType::GroundHeatExchanger_HorizontalTrench);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::OutletNodeName, node->name().get());
      }
    }

    // DesignFlowRate
    {
      auto value = modelObject.designFlowRate();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::DesignFlowRate, value);
    }

    // TrenchLengthinPipeAxialDirection
    {
      auto value = modelObject.trenchLengthinPipeAxialDirection();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::TrenchLengthinPipeAxialDirection, value);
    }

    // NumberofTrenches
    {
      auto value = modelObject.numberofTrenches();
      idfObject.setInt(GroundHeatExchanger_HorizontalTrenchFields::NumberofTrenches, value);
    }

    // HorizontalSpacingBetweenPipes
    {
      auto value = modelObject.horizontalSpacingBetweenPipes();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::HorizontalSpacingBetweenPipes, value);
    }

    // PipeInnerDiameter
    {
      auto value = modelObject.pipeInnerDiameter();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeInnerDiameter, value);
    }

    // PipeOuterDiameter
    {
      auto value = modelObject.pipeOuterDiameter();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeOuterDiameter, value);
    }

    // BurialDepth
    {
      auto value = modelObject.burialDepth();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::BurialDepth, value);
    }

    // SoilThermalConductivity
    {
      auto value = modelObject.soilThermalConductivity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilThermalConductivity, value);
    }

    // SoilDensity`
    {
      auto value = modelObject.soilDensity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilDensity, value);
    }

    // SoilSpecificHeat
    {
      auto value = modelObject.soilSpecificHeat();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilSpecificHeat, value);
    }

    // PipeThermalConductivity
    {
      auto value = modelObject.pipeThermalConductivity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeThermalConductivity, value);
    }

    // PipeDensity
    {
      auto value = modelObject.pipeDensity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeDensity, value);
    }

    // PipeSpecificHeat
    {
      auto value = modelObject.pipeSpecificHeat();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeSpecificHeat, value);
    }

    // SoilMoistureContentPercent
    {
      auto value = modelObject.soilMoistureContentPercent();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercent, value);
    }

    // SoilMoistureContentPercentatSaturation
    {
      auto value = modelObject.soilMoistureContentPercentatSaturation();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercentatSaturation, value);
    }

    if (istringEqual(modelObject.groundTemperatureModel(), "KusudaAchenbach")) {
      IdfObject groundModel(IddObjectType::Site_GroundTemperature_Undisturbed_KusudaAchenbach);
      m_idfObjects.push_back(groundModel);

      groundModel.setName(modelObject.name().get() + " Ground Model");

      idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName, groundModel.name().get());

      // KusudaAchenbachAverageSurfaceTemperature
      {
        auto value = modelObject.kusudaAchenbachAverageSurfaceTemperature();
        groundModel.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageSoilSurfaceTemperature, value);
      }

      // KusudaAchenbachAverageAmplitudeofSurfaceTemperature
      {
        auto value = modelObject.kusudaAchenbachAverageAmplitudeofSurfaceTemperature();
        groundModel.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::AverageAmplitudeofSurfaceTemperature, value);
      }

      // KusudaAchenbachPhaseShiftofMinimumSurfaceTemperature
      {
        auto value = modelObject.kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature();
        groundModel.setDouble(Site_GroundTemperature_Undisturbed_KusudaAchenbachFields::PhaseShiftofMinimumSurfaceTemperature, value);
      }

      // EvapotranspirationGroundCoverParameter
      {
        auto value = modelObject.evapotranspirationGroundCoverParameter();
        idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter, value);
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
