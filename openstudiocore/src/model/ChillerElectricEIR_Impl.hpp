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

#ifndef MODEL_CHILLERELECTRICEIR_IMPL_HPP
#define MODEL_CHILLERELECTRICEIR_IMPL_HPP

#include "WaterToWaterComponent_Impl.hpp"

namespace openstudio {
namespace model {

class ChillerElectricEIR;
class CurveBiquadratic;
class  CurveQuadratic;
class Schedule;

namespace detail {

class MODEL_API ChillerElectricEIR_Impl : public WaterToWaterComponent_Impl
{
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  
 public:


  ChillerElectricEIR_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  ChillerElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

  ChillerElectricEIR_Impl(const ChillerElectricEIR_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

  virtual ~ChillerElectricEIR_Impl() {}

  /** @name Virtual Methods */
  //@{

  virtual ModelObject clone(Model model) const override;

  virtual std::vector<ModelObject> children() const override;

  virtual IddObjectType iddObjectType() const override;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

  virtual const std::vector<std::string> & outputVariableNames() const override;

  virtual bool addToNode(Node & node) override;

  virtual unsigned supplyInletPort() override;

  virtual unsigned supplyOutletPort() override;

  virtual unsigned demandInletPort() override;

  virtual unsigned demandOutletPort() override;

  //@}
  boost::optional<double> referenceCapacity() const;

  bool isReferenceCapacityAutosized() const;

  double referenceCOP() const;

  double referenceLeavingChilledWaterTemperature() const;

  bool isReferenceLeavingChilledWaterTemperatureDefaulted() const;

  double referenceEnteringCondenserFluidTemperature() const;

  bool isReferenceEnteringCondenserFluidTemperatureDefaulted() const;

  boost::optional<double> referenceChilledWaterFlowRate() const;

  bool isReferenceChilledWaterFlowRateAutosized() const;

  boost::optional<double> referenceCondenserFluidFlowRate() const;

  bool isReferenceCondenserFluidFlowRateAutosized() const;

  CurveBiquadratic coolingCapacityFunctionOfTemperature() const;

  CurveBiquadratic  electricInputToCoolingOutputRatioFunctionOfTemperature() const;

  CurveQuadratic electricInputToCoolingOutputRatioFunctionOfPLR() const;

  double minimumPartLoadRatio() const;

  bool isMinimumPartLoadRatioDefaulted() const;

  double maximumPartLoadRatio() const;

  bool isMaximumPartLoadRatioDefaulted() const;

  double optimumPartLoadRatio() const;

  bool isOptimumPartLoadRatioDefaulted() const;

  double minimumUnloadingRatio() const;

  bool isMinimumUnloadingRatioDefaulted() const;

  std::string chilledWaterInletNodeName() const;

  std::string chilledWaterOutletNodeName() const;

  boost::optional<std::string> condenserInletNodeName() const;

  boost::optional<std::string> condenserOutletNodeName() const;

  std::string condenserType() const;

  bool isCondenserTypeDefaulted() const;

  double condenserFanPowerRatio() const;

  bool isCondenserFanPowerRatioDefaulted() const;

  double compressorMotorEfficiency() const;

  bool isCompressorMotorEfficiencyDefaulted() const;

  double leavingChilledWaterLowerTemperatureLimit() const;

  bool isLeavingChilledWaterLowerTemperatureLimitDefaulted() const;

  std::string chillerFlowMode() const;

  bool isChillerFlowModeDefaulted() const;

  double designHeatRecoveryWaterFlowRate() const;

  bool isDesignHeatRecoveryWaterFlowRateDefaulted() const;

  boost::optional<std::string> heatRecoveryInletNodeName() const;

  boost::optional<std::string> heatRecoveryOutletNodeName() const;

  double sizingFactor() const;

  bool isSizingFactorDefaulted() const;

  double basinHeaterCapacity() const;

  bool isBasinHeaterCapacityDefaulted() const;

  double basinHeaterSetpointTemperature() const;

  bool isBasinHeaterSetpointTemperatureDefaulted() const;

  boost::optional<Schedule> basinHeaterSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setReferenceCapacity(boost::optional<double> referenceCapacity);

  bool setReferenceCapacity(double referenceCapacity);

  void autosizeReferenceCapacity();

  bool setReferenceCOP(double referenceCOP);

  void setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature);

  void resetReferenceLeavingChilledWaterTemperature();

  void setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature);

  void resetReferenceEnteringCondenserFluidTemperature();

  bool setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate);

  bool setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate);

  void resetReferenceChilledWaterFlowRate();

  void autosizeReferenceChilledWaterFlowRate();

  bool setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate);

  bool setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate);

  void resetReferenceCondenserFluidFlowRate();

  void autosizeReferenceCondenserFluidFlowRate();

  bool setCoolingCapacityFunctionOfTemperature(const CurveBiquadratic& );

  bool setElectricInputToCoolingOutputRatioFunctionOfTemperature(const CurveBiquadratic& );

  bool setElectricInputToCoolingOutputRatioFunctionOfPLR(const CurveQuadratic& );

  bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

  void resetMinimumPartLoadRatio();

  bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

  void resetMaximumPartLoadRatio();

  bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

  void resetOptimumPartLoadRatio();

  bool setMinimumUnloadingRatio(double minimumUnloadingRatio);

  void resetMinimumUnloadingRatio();

  void setChilledWaterInletNodeName(std::string chilledWaterInletNodeName);

  void setChilledWaterOutletNodeName(std::string chilledWaterOutletNodeName);

  void setCondenserInletNodeName(boost::optional<std::string> condenserInletNodeName);

  void setCondenserInletNodeName(std::string condenserInletNodeName);

  void resetCondenserInletNodeName();

  void setCondenserOutletNodeName(boost::optional<std::string> condenserOutletNodeName);

  void setCondenserOutletNodeName(std::string condenserOutletNodeName);

  void resetCondenserOutletNodeName();

  bool setCondenserType(std::string condenserType);

  void resetCondenserType();

  bool setCondenserFanPowerRatio(double condenserFanPowerRatio);

  void resetCondenserFanPowerRatio();

  bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

  void resetCompressorMotorEfficiency();

  void setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit);

  void resetLeavingChilledWaterLowerTemperatureLimit();

  bool setChillerFlowMode(std::string chillerFlowMode);

  void resetChillerFlowMode();

  bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

  void resetDesignHeatRecoveryWaterFlowRate();

  void setHeatRecoveryInletNodeName(boost::optional<std::string> heatRecoveryInletNodeName);

  void setHeatRecoveryInletNodeName(std::string heatRecoveryInletNodeName);

  void resetHeatRecoveryInletNodeName();

  void setHeatRecoveryOutletNodeName(boost::optional<std::string> heatRecoveryOutletNodeName);

  void setHeatRecoveryOutletNodeName(std::string heatRecoveryOutletNodeName);

  void resetHeatRecoveryOutletNodeName();

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

  bool setBasinHeaterCapacity(double basinHeaterCapacity);

  void resetBasinHeaterCapacity();

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

  void resetBasinHeaterSetpointTemperature();

  bool setBasinHeaterSchedule(Schedule & schedule );

  void resetBasinHeaterSchedule();

  //@}
 protected:
 private:
  REGISTER_LOGGER("openstudio.model.ChillerElectricEIR");

  boost::optional<ModelObject> basinHeaterScheduleAsModelObject() const;

  bool setBasinHeaterScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
};

} // detail

} // model

} // openstudio

#endif // MODEL_CHILLERELECTRICEIR_IMPL_HPP

