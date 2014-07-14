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

#ifndef MODEL_SIZINGSYSTEM_HPP
#define MODEL_SIZINGSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class AirLoopHVAC;

namespace detail {

class SizingSystem_Impl;

} // detail

/** SizingSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:System'. */
class MODEL_API SizingSystem : public ModelObject 
{
  public:

  explicit SizingSystem(const Model& model, const AirLoopHVAC & airLoopHVAC);

  virtual ~SizingSystem() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validTypeofLoadtoSizeOnValues();

  static std::vector<std::string> validSizingOptionValues();

  static std::vector<std::string> validCoolingDesignAirFlowMethodValues();

  static std::vector<std::string> validHeatingDesignAirFlowMethodValues();

  static std::vector<std::string> validSystemOutdoorAirMethodValues();

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

  bool setTypeofLoadtoSizeOn(std::string typeofLoadtoSizeOn);

  void resetTypeofLoadtoSizeOn();

  bool setDesignOutdoorAirFlowRate(double designOutdoorAirFlowRate);

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

  AirLoopHVAC airLoopHVAC() const;

  protected:

  /// @cond

  void setAirLoopHVAC(const AirLoopHVAC & airLoopHVAC);

  typedef detail::SizingSystem_Impl ImplType;

  explicit SizingSystem(std::shared_ptr<detail::SizingSystem_Impl> impl);

  friend class detail::SizingSystem_Impl;
  friend class Model;
  friend class IdfObject;
  friend class AirLoopHVAC;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

  private:

  REGISTER_LOGGER("openstudio.model.SizingSystem");
};

/** \relates SizingSystem*/
typedef boost::optional<SizingSystem> OptionalSizingSystem;

/** \relates SizingSystem*/
typedef std::vector<SizingSystem> SizingSystemVector;

} // model

} // openstudio

#endif // MODEL_SIZINGSYSTEM_HPP

