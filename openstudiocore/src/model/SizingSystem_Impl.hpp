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

#ifndef MODEL_SIZINGSYSTEM_IMPL_HPP
#define MODEL_SIZINGSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

class AirLoopHVAC;

namespace detail {

/** SizingSystem_Impl is a ModelObject_Impl that is the implementation class for SizingSystem.*/
class MODEL_API SizingSystem_Impl : public ModelObject_Impl 
{
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  public:

  SizingSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  SizingSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

  SizingSystem_Impl(const SizingSystem_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

  virtual ~SizingSystem_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  std::string typeofLoadtoSizeOn() const;

  bool isTypeofLoadtoSizeOnDefaulted() const;

  boost::optional<double> designOutdoorAirFlowRate() const;

  bool isDesignOutdoorAirFlowRateDefaulted() const;

  bool isDesignOutdoorAirFlowRateAutosized() const;

  double minimumSystemAirFlowRatio() const;

  double preheatDesignTemperature() const;

  double preheatDesignHumidityRatio() const;

  double precoolDesignTemperature() const;

  double precoolDesignHumidityRatio() const;

  double centralCoolingDesignSupplyAirTemperature() const;

  double centralHeatingDesignSupplyAirTemperature() const;

  std::string sizingOption() const;

  bool isSizingOptionDefaulted() const;

  bool allOutdoorAirinCooling() const;

  bool isAllOutdoorAirinCoolingDefaulted() const;

  bool allOutdoorAirinHeating() const;

  bool isAllOutdoorAirinHeatingDefaulted() const;

  double centralCoolingDesignSupplyAirHumidityRatio() const;

  bool isCentralCoolingDesignSupplyAirHumidityRatioDefaulted() const;

  double centralHeatingDesignSupplyAirHumidityRatio() const;

  bool isCentralHeatingDesignSupplyAirHumidityRatioDefaulted() const;

  std::string coolingDesignAirFlowMethod() const;

  bool isCoolingDesignAirFlowMethodDefaulted() const;

  double coolingDesignAirFlowRate() const;

  bool isCoolingDesignAirFlowRateDefaulted() const;

  std::string heatingDesignAirFlowMethod() const;

  bool isHeatingDesignAirFlowMethodDefaulted() const;

  double heatingDesignAirFlowRate() const;

  bool isHeatingDesignAirFlowRateDefaulted() const;

  std::string systemOutdoorAirMethod() const;

  bool isSystemOutdoorAirMethodDefaulted() const;

  double zoneMaximumOutdoorAirFraction() const;

  double coolingSupplyAirFlowRatePerFloorArea() const;

  double coolingFractionofAutosizedCoolingSupplyAirFlowRate() const;

  double coolingSupplyAirFlowRatePerUnitCoolingCapacity() const;

  double heatingSupplyAirFlowRatePerFloorArea() const;

  double heatingFractionofAutosizedHeatingSupplyAirFlowRate() const;

  double heatingFractionofAutosizedCoolingSupplyAirFlowRate() const;

  double heatingSupplyAirFlowRatePerUnitHeatingCapacity() const;

  std::string coolingDesignCapacityMethod() const;

  boost::optional<double> coolingDesignCapacity() const;

  bool isCoolingDesignCapacityAutosized() const;

  double coolingDesignCapacityPerFloorArea() const;

  double fractionofAutosizedCoolingDesignCapacity() const;

  std::string heatingDesignCapacityMethod() const;

  boost::optional<double> heatingDesignCapacity() const;

  bool isHeatingDesignCapacityAutosized() const;

  double heatingDesignCapacityPerFloorArea() const;

  double fractionofAutosizedHeatingDesignCapacity() const;

  std::string centralCoolingCapacityControlMethod() const;

  bool setTypeofLoadtoSizeOn(std::string typeofLoadtoSizeOn);

  void resetTypeofLoadtoSizeOn();

  bool setDesignOutdoorAirFlowRate(boost::optional<double> designOutdoorAirFlowRate);

  void resetDesignOutdoorAirFlowRate();

  void autosizeDesignOutdoorAirFlowRate();

  bool setMinimumSystemAirFlowRatio(double minimumSystemAirFlowRatio);

  void setPreheatDesignTemperature(double preheatDesignTemperature);

  void setPreheatDesignHumidityRatio(double preheatDesignHumidityRatio);

  void setPrecoolDesignTemperature(double precoolDesignTemperature);

  void setPrecoolDesignHumidityRatio(double precoolDesignHumidityRatio);

  void setCentralCoolingDesignSupplyAirTemperature(double centralCoolingDesignSupplyAirTemperature);

  void setCentralHeatingDesignSupplyAirTemperature(double centralHeatingDesignSupplyAirTemperature);

  bool setSizingOption(std::string sizingOption);

  void resetSizingOption();

  void setAllOutdoorAirinCooling(bool allOutdoorAirinCooling);

  void resetAllOutdoorAirinCooling();

  void setAllOutdoorAirinHeating(bool allOutdoorAirinHeating);

  void resetAllOutdoorAirinHeating();

  void setCentralCoolingDesignSupplyAirHumidityRatio(double centralCoolingDesignSupplyAirHumidityRatio);

  void resetCentralCoolingDesignSupplyAirHumidityRatio();

  void setCentralHeatingDesignSupplyAirHumidityRatio(double centralHeatingDesignSupplyAirHumidityRatio);

  void resetCentralHeatingDesignSupplyAirHumidityRatio();

  bool setCoolingDesignAirFlowMethod(std::string coolingDesignAirFlowMethod);

  void resetCoolingDesignAirFlowMethod();

  bool setCoolingDesignAirFlowRate(double coolingDesignAirFlowRate);

  void resetCoolingDesignAirFlowRate();

  bool setHeatingDesignAirFlowMethod(std::string heatingDesignAirFlowMethod);

  void resetHeatingDesignAirFlowMethod();

  bool setHeatingDesignAirFlowRate(double heatingDesignAirFlowRate);

  void resetHeatingDesignAirFlowRate();

  bool setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod);

  void resetSystemOutdoorAirMethod();

  bool setZoneMaximumOutdoorAirFraction(double zoneMaximumOutdoorAirFraction);

  bool setCoolingSupplyAirFlowRatePerFloorArea(double coolingSupplyAirFlowRatePerFloorArea);

  bool setCoolingFractionofAutosizedCoolingSupplyAirFlowRate(double coolingFractionofAutosizedCoolingSupplyAirFlowRate);

  bool setCoolingSupplyAirFlowRatePerUnitCoolingCapacity(double coolingSupplyAirFlowRatePerUnitCoolingCapacity);

  bool setHeatingSupplyAirFlowRatePerFloorArea(double heatingSupplyAirFlowRatePerFloorArea);

  bool setHeatingFractionofAutosizedHeatingSupplyAirFlowRate(double heatingFractionofAutosizedHeatingSupplyAirFlowRate);

  bool setHeatingFractionofAutosizedCoolingSupplyAirFlowRate(double heatingFractionofAutosizedCoolingSupplyAirFlowRate);

  bool setHeatingSupplyAirFlowRatePerUnitHeatingCapacity(double heatingSupplyAirFlowRatePerUnitHeatingCapacity);

  bool setCoolingDesignCapacityMethod(std::string coolingDesignCapacityMethod);

  bool setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity);

  void autosizeCoolingDesignCapacity();

  bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

  bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

  bool setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod);

  bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

  void autosizeHeatingDesignCapacity();

  bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

  bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

  bool setCentralCoolingCapacityControlMethod(std::string centralCoolingCapacityControlMethod);

  AirLoopHVAC airLoopHVAC() const;

  void setAirLoopHVAC(const AirLoopHVAC & airLoopHVAC);

  protected:

  private:

  REGISTER_LOGGER("openstudio.model.SizingSystem");
};

} // detail

} // model

} // openstudio

#endif // MODEL_SIZINGSYSTEM_IMPL_HPP

