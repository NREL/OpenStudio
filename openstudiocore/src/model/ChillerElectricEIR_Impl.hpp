/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
  Q_OBJECT;

  Q_PROPERTY(boost::optional<double> referenceCapacity READ referenceCapacity WRITE setReferenceCapacity);
  Q_PROPERTY(double referenceCOP READ referenceCOP WRITE setReferenceCOP);
  Q_PROPERTY(double referenceLeavingChilledWaterTemperature READ referenceLeavingChilledWaterTemperature WRITE setReferenceLeavingChilledWaterTemperature RESET resetReferenceLeavingChilledWaterTemperature);
  Q_PROPERTY(double referenceEnteringCondenserFluidTemperature READ referenceEnteringCondenserFluidTemperature WRITE setReferenceEnteringCondenserFluidTemperature RESET resetReferenceEnteringCondenserFluidTemperature);
  Q_PROPERTY(boost::optional<double> referenceChilledWaterFlowRate READ referenceChilledWaterFlowRate WRITE setReferenceChilledWaterFlowRate RESET resetReferenceChilledWaterFlowRate);
  Q_PROPERTY(boost::optional<double> referenceCondenserFluidFlowRate READ referenceCondenserFluidFlowRate WRITE setReferenceCondenserFluidFlowRate RESET resetReferenceCondenserFluidFlowRate);
  Q_PROPERTY(double minimumPartLoadRatio READ minimumPartLoadRatio WRITE setMinimumPartLoadRatio RESET resetMinimumPartLoadRatio);
  Q_PROPERTY(double maximumPartLoadRatio READ maximumPartLoadRatio WRITE setMaximumPartLoadRatio RESET resetMaximumPartLoadRatio);
  Q_PROPERTY(double optimumPartLoadRatio READ optimumPartLoadRatio WRITE setOptimumPartLoadRatio RESET resetOptimumPartLoadRatio);
  Q_PROPERTY(double minimumUnloadingRatio READ minimumUnloadingRatio WRITE setMinimumUnloadingRatio RESET resetMinimumUnloadingRatio);
  Q_PROPERTY(std::string chilledWaterInletNodeName READ chilledWaterInletNodeName WRITE setChilledWaterInletNodeName);
  Q_PROPERTY(std::string chilledWaterOutletNodeName READ chilledWaterOutletNodeName WRITE setChilledWaterOutletNodeName);
  Q_PROPERTY(boost::optional<std::string> condenserInletNodeName READ condenserInletNodeName WRITE setCondenserInletNodeName RESET resetCondenserInletNodeName);
  Q_PROPERTY(boost::optional<std::string> condenserOutletNodeName READ condenserOutletNodeName WRITE setCondenserOutletNodeName RESET resetCondenserOutletNodeName);
  Q_PROPERTY(std::string condenserType READ condenserType WRITE setCondenserType RESET resetCondenserType);
  Q_PROPERTY(double condenserFanPowerRatio READ condenserFanPowerRatio WRITE setCondenserFanPowerRatio RESET resetCondenserFanPowerRatio);
  Q_PROPERTY(double compressorMotorEfficiency READ compressorMotorEfficiency WRITE setCompressorMotorEfficiency RESET resetCompressorMotorEfficiency);
  Q_PROPERTY(double leavingChilledWaterLowerTemperatureLimit READ leavingChilledWaterLowerTemperatureLimit WRITE setLeavingChilledWaterLowerTemperatureLimit RESET resetLeavingChilledWaterLowerTemperatureLimit);
  Q_PROPERTY(std::string chillerFlowMode READ chillerFlowMode WRITE setChillerFlowMode RESET resetChillerFlowMode);
  Q_PROPERTY(double designHeatRecoveryWaterFlowRate READ designHeatRecoveryWaterFlowRate WRITE setDesignHeatRecoveryWaterFlowRate RESET resetDesignHeatRecoveryWaterFlowRate);
  Q_PROPERTY(boost::optional<std::string> heatRecoveryInletNodeName READ heatRecoveryInletNodeName WRITE setHeatRecoveryInletNodeName RESET resetHeatRecoveryInletNodeName);
  Q_PROPERTY(boost::optional<std::string> heatRecoveryOutletNodeName READ heatRecoveryOutletNodeName WRITE setHeatRecoveryOutletNodeName RESET resetHeatRecoveryOutletNodeName);
  Q_PROPERTY(double sizingFactor READ sizingFactor WRITE setSizingFactor RESET resetSizingFactor);
  Q_PROPERTY(double basinHeaterCapacity READ basinHeaterCapacity WRITE setBasinHeaterCapacity RESET resetBasinHeaterCapacity);
  Q_PROPERTY(double basinHeaterSetpointTemperature READ basinHeaterSetpointTemperature WRITE setBasinHeaterSetpointTemperature RESET resetBasinHeaterSetpointTemperature);

  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> basinHeaterSchedule READ basinHeaterScheduleAsModelObject WRITE setBasinHeaterScheduleAsModelObject RESET resetBasinHeaterSchedule);
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

  virtual ModelObject clone(Model model) const;

  virtual std::vector<ModelObject> children() const;

  virtual IddObjectType iddObjectType() const;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

  virtual const std::vector<std::string> & outputVariableNames() const;

  virtual bool addToNode(Node & node);

  virtual unsigned supplyInletPort();

  virtual unsigned supplyOutletPort();

  virtual unsigned demandInletPort();

  virtual unsigned demandOutletPort();

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

