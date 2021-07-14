/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilSystemIntegratedHeatPumpAirSource(CoilSystemIntegratedHeatPumpAirSource& modelObject) {
    IdfObject idfObject(IddObjectType::CoilSystem_IntegratedHeatPump_AirSource);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<int> i;

    // Space Cooling Coil Name
    boost::optional<IdfObject> _spaceCoolingCoil;
    auto spaceCoolingCoil = modelObject.spaceCoolingCoil();
    if ((_spaceCoolingCoil = translateAndMapModelObject(spaceCoolingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoilName, _spaceCoolingCoil->name().get());
    }

    // Space Heating Coil Name
    boost::optional<IdfObject> _spaceHeatingCoil;
    auto spaceHeatingCoil = modelObject.spaceHeatingCoil();
    if ((_spaceHeatingCoil = translateAndMapModelObject(spaceHeatingCoil))) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoilName, _spaceHeatingCoil->name().get());
    }

    // Dedicated Water Heating Coil Name
    boost::optional<IdfObject> _dedicatedWaterHeatingCoil;
    if (auto dedicatedWaterHeatingCoil = modelObject.dedicatedWaterHeatingCoil()) {
      if ((_dedicatedWaterHeatingCoil = translateAndMapModelObject(dedicatedWaterHeatingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoilName, _dedicatedWaterHeatingCoil->name().get());
      }
    }

    // SCWH Coil Name
    boost::optional<IdfObject> _scwhCoil;
    if (auto scwhCoil = modelObject.scwhCoil()) {
      if ((_scwhCoil = translateAndMapModelObject(scwhCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoilName, _scwhCoil->name().get());
      }
    }

    // SCDWH Cooling Coil Name
    boost::optional<IdfObject> _scdwhCoolingCoil;
    if (auto scdwhCoolingCoil = modelObject.scdwhCoolingCoil()) {
      if ((_scdwhCoolingCoil = translateAndMapModelObject(scdwhCoolingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoilName, _scdwhCoolingCoil->name().get());
      }
    }

    // SCDWH Water Heating Coil Name
    boost::optional<IdfObject> _scdwhWaterHeatingCoil;
    if (auto scdwhWaterHeatingCoil = modelObject.scdwhWaterHeatingCoil()) {
      if ((_scdwhWaterHeatingCoil = translateAndMapModelObject(scdwhWaterHeatingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoilName, _scdwhWaterHeatingCoil->name().get());
      }
    }

    // SHDWH Heating Coil Name
    boost::optional<IdfObject> _shdwhHeatingCoil;
    if (auto shdwhHeatingCoil = modelObject.shdwhHeatingCoil()) {
      if ((_shdwhHeatingCoil = translateAndMapModelObject(shdwhHeatingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoilName, _shdwhHeatingCoil->name().get());
      }
    }

    // SHDWH Water Heating Coil Name
    boost::optional<IdfObject> _shdwhWaterHeatingCoil;
    if (auto shdwhWaterHeatingCoil = modelObject.shdwhWaterHeatingCoil()) {
      if ((_shdwhWaterHeatingCoil = translateAndMapModelObject(shdwhWaterHeatingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoilName, _shdwhWaterHeatingCoil->name().get());
      }
    }

    // Enhanced Dehumidification Cooling Coil Name
    boost::optional<IdfObject> _enhancedDehumidificationCoolingCoil;
    if (auto enhancedDehumidificationCoolingCoil = modelObject.enhancedDehumidificationCoolingCoil()) {
      if ((_enhancedDehumidificationCoolingCoil = translateAndMapModelObject(enhancedDehumidificationCoolingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoilName,
                            _enhancedDehumidificationCoolingCoil->name().get());
      }
    }

    // Grid Response Cooling Coil Name
    boost::optional<IdfObject> _gridResponseCoolingCoil;
    if (auto gridResponseCoolingCoil = modelObject.gridResponseCoolingCoil()) {
      if ((_gridResponseCoolingCoil = translateAndMapModelObject(gridResponseCoolingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoilName, _gridResponseCoolingCoil->name().get());
      }
    }

    // Grid Response Space Heating Coil Name
    boost::optional<IdfObject> _gridResponseHeatingCoil;
    if (auto gridResponseHeatingCoil = modelObject.gridResponseHeatingCoil()) {
      if ((_gridResponseHeatingCoil = translateAndMapModelObject(gridResponseHeatingCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseSpaceHeatingCoilName, _gridResponseHeatingCoil->name().get());
      }
    }

    // Indoor Temperature Limit for SCWH Mode
    if ((value = modelObject.indoorTemperatureLimitForSCWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureLimitforSCWHMode, value.get());
    }

    // Ambient Temperature Limit for SCWH Mode
    if ((value = modelObject.ambientTemperatureLimitForSCWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureLimitforSCWHMode, value.get());
    }

    // Indoor Temperature above Which WH has Higher Priority
    if ((value = modelObject.indoorTemperatureAboveWhichWHHasHigherPriority())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureaboveWhichWHhasHigherPriority, value.get());
    }

    // Ambient Temperature above Which WH has Higher Priority
    if ((value = modelObject.ambientTemperatureAboveWhichWHHasHigherPriority())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureaboveWhichWHhasHigherPriority, value.get());
    }

    // Flag to Indicate Load Control in SCWH Mode
    if ((i = modelObject.flagtoIndicateLoadControlInSCWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::FlagtoIndicateLoadControlinSCWHMode, i.get());
    }

    // Minimum Speed Level for SCWH Mode
    if ((i = modelObject.minimumSpeedLevelForSCWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSCWHMode, i.get());
    }

    // Maximum Water Flow Volume before Switching from SCDWH to SCWH Mode
    if ((value = modelObject.maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumWaterFlowVolumebeforeSwitchingfromSCDWHtoSCWHMode, value.get());
    }

    // Minimum Speed Level for SCDWH Mode
    if ((i = modelObject.minimumSpeedLevelForSCDWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSCDWHMode, i.get());
    }

    // Maximum Running Time before Allowing Electric Resistance Heat Use during SHDWH Mode
    if ((value = modelObject.maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumRunningTimebeforeAllowingElectricResistanceHeatUseduringSHDWHMode,
                          value.get());
    }

    // Minimum Speed Level for SHDWH Mode
    if ((i = modelObject.minimumSpeedLevelForSHDWHMode())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSHDWHMode, i.get());
    }

    // Sizing Ratio of Space Heating Coil to Space Cooling Coil
    if ((value = modelObject.sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil, value.get());
    }

    // Sizing Ratio of Dedicated Water Heating Coil to Space Cooling Coil
    if ((value = modelObject.sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil, value.get());
    }

    // Sizing Ratio of Combined Space Cooling and Water Heating Coil with Full Condensing to Space Cooling Coil
    if ((value = modelObject.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil())) {
      idfObject.setDouble(
        CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil,
        value.get());
    }

    // Sizing Ratio of Combined Space Cooling and Water Heating Coil with Desuperheating - Cooling Capacity to Space Cooling Coil
    if ((value = modelObject.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                            SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil,
                          value.get());
    }

    // Sizing Ratio of Combined Space Cooling and Water Heating Coil with Desuperheating - Water Heating Capacity to Space Cooling Coil
    if ((value = modelObject.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                            SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                          value.get());
    }

    // Sizing Ratio of Combined Space Heating and Water Heating Coil with Desuperheating - Space Heating Capacity to Space Cooling Coil
    if ((value = modelObject.sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                            SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil,
                          value.get());
    }

    // Sizing Ratio of Combined Space Heating and Water Heating Coil with Desuperheating - Water Heating Capacity to Space Cooling Coil
    if ((value = modelObject.sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                            SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                          value.get());
    }

    // Sizing Ratio of Enhanced Dehumidification Coil to Space Cooling Coil
    if ((value = modelObject.sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil, value.get());
    }

    // Sizing Ratio of Grid Response Cooling Coil to Space Cooling Coil
    if ((value = modelObject.sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil, value.get());
    }

    // Sizing Ratio of Grid Response Heating Coil to Space Cooling Coil
    if ((value = modelObject.sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil, value.get());
    }

    // Chiller Coil Name
    boost::optional<IdfObject> _chillerCoil;
    if (auto chillerCoil = modelObject.chillerCoil()) {
      if ((_chillerCoil = translateAndMapModelObject(chillerCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilName, _chillerCoil->name().get());
      }
    }

    // Chiller Coil Belongs to a Single or Separate Unit
    if ((s = modelObject.chillerCoilBelongstoaSingleorSeparateUnit())) {
      idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit, s.get());
    }

    // Chiller Compressor Run Speed
    if ((i = modelObject.chillerCoilCompressorRunSpeed())) {
      idfObject.setInt(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCompressorRunSpeed, i.get());
    }

    // Sizing Ratio of Chiller Coil to Space Cooling Coil
    if ((value = modelObject.sizingRatioofChillerCoiltoSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofChillerCoiltoSpaceCoolingCoil, value.get());
    }

    // Coil Object Type
    // Coil Object Name
    boost::optional<IdfObject> _supplementalChillerCoil;
    if (auto supplementalChillerCoil = modelObject.supplementalChillerCoil()) {
      if ((_supplementalChillerCoil = translateAndMapModelObject(supplementalChillerCoil.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::CoilObjectType, _supplementalChillerCoil->iddObject().name());
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::CoilObjectName, _supplementalChillerCoil->name().get());
      }
    }

    // Air Flow Ratio of Water Coil to the Space Cooling Coil
    if ((value = modelObject.airFlowRatioofWaterCoiltotheSpaceCoolingCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AirFlowRatioofWaterCoiltotheSpaceCoolingCoil, value.get());
    }

    // Water Flow Ratio of Water Coil to the Chiller Coil
    if ((value = modelObject.waterFlowRatioofWaterCoiltotheChillerCoil())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::WaterFlowRatioofWaterCoiltotheChillerCoil, value.get());
    }

    // Tank Object Type
    // Tank Name
    boost::optional<IdfObject> _storageTank;
    if (auto storageTank = modelObject.storageTank()) {
      if ((_storageTank = translateAndMapModelObject(storageTank.get()))) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::TankObjectType, _storageTank->iddObject().name());
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::TankName, _storageTank->name().get());
      }
    }

    // Ice Fraction below which charging starts
    if ((value = modelObject.iceFractionBelowWhichChargingStarts())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionbelowwhichchargingstarts, value.get());
    }

    // Chiller Entering Temperature at 0 Tank Fraction
    if ((value = modelObject.chillerEnteringTemperatureatZeroTankFraction())) {
      idfObject.setDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureat0TankFraction, value.get());
    }

    // Temperature Deviation Curve Name, as a Function of the Tank Fraction
    if (boost::optional<model::Curve> curve = modelObject.temperatureDeviationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurveNameasaFunctionoftheTankFraction,
                            _curve->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
