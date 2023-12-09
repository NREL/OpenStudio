/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/CoilCoolingDXSingleSpeedThermalStorage.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/FluidAndGlycol.hpp"
#include "../../model/FluidAndGlycol_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/BivariateFunctions.hpp"
#include "../../model/BivariateFunctions_Impl.hpp"

#include "../../model/UnivariateFunctions.hpp"
#include "../../model/UnivariateFunctions_Impl.hpp"

#include "../../model/TrivariateFunctions.hpp"
#include "../../model/TrivariateFunctions_Impl.hpp"

#include "../../model/WaterStorageTank.hpp"
#include "../../model/WaterStorageTank_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilCoolingDXSingleSpeedThermalStorage(model::CoilCoolingDXSingleSpeedThermalStorage& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject(openstudio::IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage);
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
    // But in some cases, you'll want to handle failure without pushing to the map
    // Name
    idfObject.setName(modelObject.nameString());

    // Availability Schedule Name: Optional Object
    if (boost::optional<Schedule> availabilitySchedule_ = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilitySchedule_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilityScheduleName, wo_->nameString());
      }
    }

    // Operating Mode Control Method: Required String
    const std::string operatingModeControlMethod = modelObject.operatingModeControlMethod();
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod, operatingModeControlMethod);

    // Operation Mode Control Schedule Name: Optional Object
    if (boost::optional<Schedule> operationModeControlSchedule_ = modelObject.operationModeControlSchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(operationModeControlSchedule_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlScheduleName, wo_->nameString());
      }
    }

    // Storage Type: Required String
    const std::string storageType = modelObject.storageType();
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType, storageType);

    // User Defined Fluid Type: Optional Object
    if (boost::optional<FluidAndGlycol> userDefinedFluidType_ = modelObject.userDefinedFluidType()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(userDefinedFluidType_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType, wo_->nameString());
      }
    }

    if (modelObject.isFluidStorageVolumeAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, "Autocalculate");
    } else {
      // Fluid Storage Volume: boost::optional<double>
      if (boost::optional<double> fluidStorageVolume_ = modelObject.fluidStorageVolume()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, fluidStorageVolume_.get());
      }
    }

    if (modelObject.isIceStorageCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, "Autocalculate");
    } else {
      // Ice Storage Capacity: boost::optional<double>
      if (boost::optional<double> iceStorageCapacity_ = modelObject.iceStorageCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, iceStorageCapacity_.get());
      }
    }

    // Storage Capacity Sizing Factor: boost::optional<double>
    if (boost::optional<double> storageCapacitySizingFactor_ = modelObject.storageCapacitySizingFactor()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, storageCapacitySizingFactor_.get());
    }

    // Storage Tank Ambient Temperature Node Name: Required Node
    Node storageTankAmbientTemperatureNodeName = modelObject.storageTankAmbientTemperatureNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(storageTankAmbientTemperatureNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNodeName, wo_->nameString());
    }

    // Storage Tank to Ambient U-value Times Area Heat Transfer Coefficient: Required Double
    const double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient =
      modelObject.storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient,
                        storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);

    // Fluid Storage Tank Rating Temperature: boost::optional<double>
    if (boost::optional<double> fluidStorageTankRatingTemperature_ = modelObject.fluidStorageTankRatingTemperature()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature,
                          fluidStorageTankRatingTemperature_.get());
    }

    if (modelObject.isRatedEvaporatorAirFlowRateAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, "Autosize");
    } else {
      // Rated Evaporator Air Flow Rate: boost::optional<double>
      if (boost::optional<double> ratedEvaporatorAirFlowRate_ = modelObject.ratedEvaporatorAirFlowRate()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate_.get());
      }
    }

    // Evaporator Air Inlet Node Name: Required Node
    Node evaporatorAirInletNodeName = modelObject.evaporatorAirInletNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(evaporatorAirInletNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName, wo_->nameString());
    }

    // Evaporator Air Outlet Node Name: Required Node
    Node evaporatorAirOutletNodeName = modelObject.evaporatorAirOutletNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(evaporatorAirOutletNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName, wo_->nameString());
    }

    // Cooling Only Mode Available: Required Boolean
    if (modelObject.coolingOnlyModeAvailable()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable, "No");
    }

    if (modelObject.isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity, "Autosize");
    } else {
      // Cooling Only Mode Rated Total Evaporator Cooling Capacity: boost::optional<double>
      if (boost::optional<double> coolingOnlyModeRatedTotalEvaporatorCoolingCapacity_ =
            modelObject.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity,
                            coolingOnlyModeRatedTotalEvaporatorCoolingCapacity_.get());
      }
    }

    // Cooling Only Mode Rated Sensible Heat Ratio: Optional Double
    const double coolingOnlyModeRatedSensibleHeatRatio = modelObject.coolingOnlyModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedSensibleHeatRatio,
                        coolingOnlyModeRatedSensibleHeatRatio);

    // Cooling Only Mode Rated COP: Optional Double
    const double coolingOnlyModeRatedCOP = modelObject.coolingOnlyModeRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedCOP, coolingOnlyModeRatedCOP);

    // Cooling Only Mode Total Evaporator Cooling Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling Only Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling Only Mode Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling Only Mode Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Cooling Only Mode Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingOnlyModePartLoadFractionCorrelationCurve_ =
          modelObject.coolingOnlyModePartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModePartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurveName, wo_->nameString());
      }
    }

    // Cooling Only Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling Only Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Available: Required Boolean
    if (modelObject.coolingAndChargeModeAvailable()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable, "No");
    }

    if (modelObject.isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, "Autocalculate");
    } else {
      // Cooling And Charge Mode Rated Total Evaporator Cooling Capacity: boost::optional<double>
      if (boost::optional<double> coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity_ =
            modelObject.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity,
                            coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity_.get());
      }
    }

    // Cooling And Charge Mode Capacity Sizing Factor: Optional Double
    const double coolingAndChargeModeCapacitySizingFactor = modelObject.coolingAndChargeModeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor,
                        coolingAndChargeModeCapacitySizingFactor);

    if (modelObject.isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, "Autocalculate");
    } else {
      // Cooling And Charge Mode Rated Storage Charging Capacity: boost::optional<double>
      if (boost::optional<double> coolingAndChargeModeRatedStorageChargingCapacity_ =
            modelObject.coolingAndChargeModeRatedStorageChargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity,
                            coolingAndChargeModeRatedStorageChargingCapacity_.get());
      }
    }

    // Cooling And Charge Mode Storage Capacity Sizing Factor: Optional Double
    const double coolingAndChargeModeStorageCapacitySizingFactor = modelObject.coolingAndChargeModeStorageCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageCapacitySizingFactor,
                        coolingAndChargeModeStorageCapacitySizingFactor);

    // Cooling And Charge Mode Rated Sensible Heat Ratio: Optional Double
    const double coolingAndChargeModeRatedSensibleHeatRatio = modelObject.coolingAndChargeModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedSensibleHeatRatio,
                        coolingAndChargeModeRatedSensibleHeatRatio);

    // Cooling And Charge Mode Cooling Rated COP: Optional Double
    const double coolingAndChargeModeCoolingRatedCOP = modelObject.coolingAndChargeModeCoolingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCoolingRatedCOP, coolingAndChargeModeCoolingRatedCOP);

    // Cooling And Charge Mode Charging Rated COP: Optional Double
    const double coolingAndChargeModeChargingRatedCOP = modelObject.coolingAndChargeModeChargingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeChargingRatedCOP, coolingAndChargeModeChargingRatedCOP);

    // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Evaporator Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Charge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Charge Capacity Function of Total Evaporator PLR Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve_ =
          modelObject.coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Charge Mode Storage Energy Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Charge Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Available: Required Boolean
    if (modelObject.coolingAndDischargeModeAvailable()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable, "No");
    }

    if (modelObject.isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity,
                          "Autocalculate");
    } else {
      // Cooling And Discharge Mode Rated Total Evaporator Cooling Capacity: boost::optional<double>
      if (boost::optional<double> coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity_ =
            modelObject.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity,
                            coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity_.get());
      }
    }

    // Cooling And Discharge Mode Evaporator Capacity Sizing Factor: Optional Double
    const double coolingAndDischargeModeEvaporatorCapacitySizingFactor = modelObject.coolingAndDischargeModeEvaporatorCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorCapacitySizingFactor,
                        coolingAndDischargeModeEvaporatorCapacitySizingFactor);

    if (modelObject.isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, "Autocalculate");
    } else {
      // Cooling And Discharge Mode Rated Storage Discharging Capacity: boost::optional<double>
      if (boost::optional<double> coolingAndDischargeModeRatedStorageDischargingCapacity_ =
            modelObject.coolingAndDischargeModeRatedStorageDischargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity,
                            coolingAndDischargeModeRatedStorageDischargingCapacity_.get());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Sizing Factor: Optional Double
    const double coolingAndDischargeModeStorageDischargeCapacitySizingFactor =
      modelObject.coolingAndDischargeModeStorageDischargeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacitySizingFactor,
                        coolingAndDischargeModeStorageDischargeCapacitySizingFactor);

    // Cooling And Discharge Mode Rated Sensible Heat Ratio: Optional Double
    const double coolingAndDischargeModeRatedSensibleHeatRatio = modelObject.coolingAndDischargeModeRatedSensibleHeatRatio();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedSensibleHeatRatio,
                        coolingAndDischargeModeRatedSensibleHeatRatio);

    // Cooling And Discharge Mode Cooling Rated COP: Optional Double
    const double coolingAndDischargeModeCoolingRatedCOP = modelObject.coolingAndDischargeModeCoolingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeCoolingRatedCOP,
                        coolingAndDischargeModeCoolingRatedCOP);

    // Cooling And Discharge Mode Discharging Rated COP: Optional Double
    const double coolingAndDischargeModeDischargingRatedCOP = modelObject.coolingAndDischargeModeDischargingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeDischargingRatedCOP,
                        coolingAndDischargeModeDischargingRatedCOP);

    // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Evaporator Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Discharge Capacity Function of Total Evaporator PLR Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve_ =
          modelObject.coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<TrivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ =
            translateAndMapModelObject(coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Storage Energy Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve_ =
          modelObject.coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurveName,
          wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Cooling And Discharge Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Charge Only Mode Available: Required Boolean
    if (modelObject.chargeOnlyModeAvailable()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable, "No");
    }

    if (modelObject.isChargeOnlyModeRatedStorageChargingCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, "Autocalculate");
    } else {
      // Charge Only Mode Rated Storage Charging Capacity: boost::optional<double>
      if (boost::optional<double> chargeOnlyModeRatedStorageChargingCapacity_ = modelObject.chargeOnlyModeRatedStorageChargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity,
                            chargeOnlyModeRatedStorageChargingCapacity_.get());
      }
    }

    // Charge Only Mode Capacity Sizing Factor: Optional Double
    const double chargeOnlyModeCapacitySizingFactor = modelObject.chargeOnlyModeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeCapacitySizingFactor, chargeOnlyModeCapacitySizingFactor);

    // Charge Only Mode Charging Rated COP: Optional Double
    const double chargeOnlyModeChargingRatedCOP = modelObject.chargeOnlyModeChargingRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeChargingRatedCOP, chargeOnlyModeChargingRatedCOP);

    // Charge Only Mode Storage Charge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve_ =
          modelObject.chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Charge Only Mode Storage Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Available: Required Boolean
    if (modelObject.dischargeOnlyModeAvailable()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable, "No");
    }

    if (modelObject.isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, "Autocalculate");
    } else {
      // Discharge Only Mode Rated Storage Discharging Capacity: boost::optional<double>
      if (boost::optional<double> dischargeOnlyModeRatedStorageDischargingCapacity_ =
            modelObject.dischargeOnlyModeRatedStorageDischargingCapacity()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity,
                            dischargeOnlyModeRatedStorageDischargingCapacity_.get());
      }
    }

    // Discharge Only Mode Capacity Sizing Factor: Optional Double
    const double dischargeOnlyModeCapacitySizingFactor = modelObject.dischargeOnlyModeCapacitySizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor,
                        dischargeOnlyModeCapacitySizingFactor);

    // Discharge Only Mode Rated Sensible Heat Ratio: boost::optional<double>
    if (boost::optional<double> dischargeOnlyModeRatedSensibleHeatRatio_ = modelObject.dischargeOnlyModeRatedSensibleHeatRatio()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio,
                          dischargeOnlyModeRatedSensibleHeatRatio_.get());
    }

    // Discharge Only Mode Rated COP: Optional Double
    const double dischargeOnlyModeRatedCOP = modelObject.dischargeOnlyModeRatedCOP();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP, dischargeOnlyModeRatedCOP);

    // Discharge Only Mode Storage Discharge Capacity Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve_ =
          modelObject.dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Storage Discharge Capacity Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve_ =
          modelObject.dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(
          Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurveName,
          wo_->nameString());
      }
    }

    // Discharge Only Mode Energy Input Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve_ =
          modelObject.dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Energy Input Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_ =
          modelObject.dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Part Load Fraction Correlation Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> dischargeOnlyModePartLoadFractionCorrelationCurve_ =
          modelObject.dischargeOnlyModePartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModePartLoadFractionCorrelationCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Sensible Heat Ratio Function of Temperature Curve Name: Optional Object
    if (boost::optional<BivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_ =
          modelObject.dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurveName,
                            wo_->nameString());
      }
    }

    // Discharge Only Mode Sensible Heat Ratio Function of Flow Fraction Curve Name: Optional Object
    if (boost::optional<UnivariateFunctions> dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_ =
          modelObject.dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurveName,
                            wo_->nameString());
      }
    }

    // Ancillary Electric Power: boost::optional<double>
    if (boost::optional<double> ancillaryElectricPower_ = modelObject.ancillaryElectricPower()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AncillaryElectricPower, ancillaryElectricPower_.get());
    }

    // Cold Weather Operation Minimum Outdoor Air Temperature: boost::optional<double>
    if (boost::optional<double> coldWeatherOperationMinimumOutdoorAirTemperature_ = modelObject.coldWeatherOperationMinimumOutdoorAirTemperature()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationMinimumOutdoorAirTemperature,
                          coldWeatherOperationMinimumOutdoorAirTemperature_.get());
    }

    // Cold Weather Operation Ancillary Power: boost::optional<double>
    if (boost::optional<double> coldWeatherOperationAncillaryPower_ = modelObject.coldWeatherOperationAncillaryPower()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationAncillaryPower,
                          coldWeatherOperationAncillaryPower_.get());
    }

    // Condenser Air Inlet Node Name: Required Node
    Node condenserAirInletNodeName = modelObject.condenserAirInletNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(condenserAirInletNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNodeName, wo_->nameString());
    }

    // Condenser Air Outlet Node Name: Required Node
    Node condenserAirOutletNodeName = modelObject.condenserAirOutletNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(condenserAirOutletNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNodeName, wo_->nameString());
    }

    if (modelObject.isCondenserDesignAirFlowRateAutocalculated()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, "Autocalculate");
    } else {
      // Condenser Design Air Flow Rate: boost::optional<double>
      if (boost::optional<double> condenserDesignAirFlowRate_ = modelObject.condenserDesignAirFlowRate()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, condenserDesignAirFlowRate_.get());
      }
    }

    // Condenser Air Flow Sizing Factor: Optional Double
    const double condenserAirFlowSizingFactor = modelObject.condenserAirFlowSizingFactor();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirFlowSizingFactor, condenserAirFlowSizingFactor);

    // Condenser Type: Optional String
    const std::string condenserType = modelObject.condenserType();
    idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType, condenserType);

    // Evaporative Condenser Effectiveness: Optional Double
    const double evaporativeCondenserEffectiveness = modelObject.evaporativeCondenserEffectiveness();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);

    if (modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized()) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    } else {
      // Evaporative Condenser Pump Rated Power Consumption: boost::optional<double>
      if (boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption_ = modelObject.evaporativeCondenserPumpRatedPowerConsumption()) {
        idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption,
                            evaporativeCondenserPumpRatedPowerConsumption_.get());
      }
    }

    // Basin Heater Capacity: Optional Double
    const double basinHeaterCapacity = modelObject.basinHeaterCapacity();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterCapacity, basinHeaterCapacity);

    // Basin Heater Setpoint Temperature: Optional Double
    const double basinHeaterSetpointTemperature = modelObject.basinHeaterSetpointTemperature();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);

    // Basin Heater Availability Schedule Name: Optional Object
    if (boost::optional<Schedule> basinHeaterAvailabilitySchedule_ = modelObject.basinHeaterAvailabilitySchedule()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(basinHeaterAvailabilitySchedule_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilityScheduleName, wo_->nameString());
      }
    }

    // Supply Water Storage Tank Name: Optional Object
    if (boost::optional<WaterStorageTank> supplyWaterStorageTank_ = modelObject.supplyWaterStorageTank()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(supplyWaterStorageTank_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTankName, wo_->nameString());
      }
    }

    // Condensate Collection Water Storage Tank Name: Optional Object
    if (boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank_ = modelObject.condensateCollectionWaterStorageTank()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(condensateCollectionWaterStorageTank_.get())) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTankName, wo_->nameString());
      }
    }

    // Storage Tank Plant Connection Inlet Node Name: Optional Node
    Node storageTankPlantConnectionInletNodeName = modelObject.storageTankPlantConnectionInletNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(storageTankPlantConnectionInletNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionInletNodeName, wo_->nameString());
    }

    // Storage Tank Plant Connection Outlet Node Name: Optional Node
    Node storageTankPlantConnectionOutletNodeName = modelObject.storageTankPlantConnectionOutletNodeName();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(storageTankPlantConnectionOutletNodeName)) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionOutletNodeName, wo_->nameString());
    }

    // Storage Tank Plant Connection Design Flow Rate: boost::optional<double>
    if (boost::optional<double> storageTankPlantConnectionDesignFlowRate_ = modelObject.storageTankPlantConnectionDesignFlowRate()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate,
                          storageTankPlantConnectionDesignFlowRate_.get());
    }

    // Storage Tank Plant Connection Heat Transfer Effectiveness: Optional Double
    const double storageTankPlantConnectionHeatTransferEffectiveness = modelObject.storageTankPlantConnectionHeatTransferEffectiveness();
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness,
                        storageTankPlantConnectionHeatTransferEffectiveness);

    // Storage Tank Minimum Operating Limit Fluid Temperature: boost::optional<double>
    if (boost::optional<double> storageTankMinimumOperatingLimitFluidTemperature_ = modelObject.storageTankMinimumOperatingLimitFluidTemperature()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature,
                          storageTankMinimumOperatingLimitFluidTemperature_.get());
    }

    // Storage Tank Maximum Operating Limit Fluid Temperature: boost::optional<double>
    if (boost::optional<double> storageTankMaximumOperatingLimitFluidTemperature_ = modelObject.storageTankMaximumOperatingLimitFluidTemperature()) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature,
                          storageTankMaximumOperatingLimitFluidTemperature_.get());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio