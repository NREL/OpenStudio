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
  Q_OBJECT;

  Q_PROPERTY(std::string typeofLoadtoSizeOn READ typeofLoadtoSizeOn WRITE setTypeofLoadtoSizeOn RESET resetTypeofLoadtoSizeOn);
  Q_PROPERTY(bool isTypeofLoadtoSizeOnDefaulted READ isTypeofLoadtoSizeOnDefaulted);
  Q_PROPERTY(boost::optional<double> designOutdoorAirFlowRate READ designOutdoorAirFlowRate WRITE setDesignOutdoorAirFlowRate RESET resetDesignOutdoorAirFlowRate);
  Q_PROPERTY(bool isDesignOutdoorAirFlowRateDefaulted READ isDesignOutdoorAirFlowRateDefaulted);
  Q_PROPERTY(bool isDesignOutdoorAirFlowRateAutosized READ isDesignOutdoorAirFlowRateAutosized);
  Q_PROPERTY(double minimumSystemAirFlowRatio READ minimumSystemAirFlowRatio WRITE setMinimumSystemAirFlowRatio);
  Q_PROPERTY(double preheatDesignTemperature READ preheatDesignTemperature WRITE setPreheatDesignTemperature);
  Q_PROPERTY(double preheatDesignHumidityRatio READ preheatDesignHumidityRatio WRITE setPreheatDesignHumidityRatio);
  Q_PROPERTY(double precoolDesignTemperature READ precoolDesignTemperature WRITE setPrecoolDesignTemperature);
  Q_PROPERTY(double precoolDesignHumidityRatio READ precoolDesignHumidityRatio WRITE setPrecoolDesignHumidityRatio);
  Q_PROPERTY(double centralCoolingDesignSupplyAirTemperature READ centralCoolingDesignSupplyAirTemperature WRITE setCentralCoolingDesignSupplyAirTemperature);
  Q_PROPERTY(double centralHeatingDesignSupplyAirTemperature READ centralHeatingDesignSupplyAirTemperature WRITE setCentralHeatingDesignSupplyAirTemperature);
  Q_PROPERTY(std::string sizingOption READ sizingOption WRITE setSizingOption RESET resetSizingOption);
  Q_PROPERTY(bool isSizingOptionDefaulted READ isSizingOptionDefaulted);
  Q_PROPERTY(bool allOutdoorAirinCooling READ allOutdoorAirinCooling WRITE setAllOutdoorAirinCooling RESET resetAllOutdoorAirinCooling);
  Q_PROPERTY(bool isAllOutdoorAirinCoolingDefaulted READ isAllOutdoorAirinCoolingDefaulted);
  Q_PROPERTY(bool allOutdoorAirinHeating READ allOutdoorAirinHeating WRITE setAllOutdoorAirinHeating RESET resetAllOutdoorAirinHeating);
  Q_PROPERTY(bool isAllOutdoorAirinHeatingDefaulted READ isAllOutdoorAirinHeatingDefaulted);
  Q_PROPERTY(double centralCoolingDesignSupplyAirHumidityRatio READ centralCoolingDesignSupplyAirHumidityRatio WRITE setCentralCoolingDesignSupplyAirHumidityRatio RESET resetCentralCoolingDesignSupplyAirHumidityRatio);
  Q_PROPERTY(bool isCentralCoolingDesignSupplyAirHumidityRatioDefaulted READ isCentralCoolingDesignSupplyAirHumidityRatioDefaulted);
  Q_PROPERTY(double centralHeatingDesignSupplyAirHumidityRatio READ centralHeatingDesignSupplyAirHumidityRatio WRITE setCentralHeatingDesignSupplyAirHumidityRatio RESET resetCentralHeatingDesignSupplyAirHumidityRatio);
  Q_PROPERTY(bool isCentralHeatingDesignSupplyAirHumidityRatioDefaulted READ isCentralHeatingDesignSupplyAirHumidityRatioDefaulted);
  Q_PROPERTY(std::string coolingDesignAirFlowMethod READ coolingDesignAirFlowMethod WRITE setCoolingDesignAirFlowMethod RESET resetCoolingDesignAirFlowMethod);
  Q_PROPERTY(bool isCoolingDesignAirFlowMethodDefaulted READ isCoolingDesignAirFlowMethodDefaulted);
  Q_PROPERTY(double coolingDesignAirFlowRate READ coolingDesignAirFlowRate WRITE setCoolingDesignAirFlowRate RESET resetCoolingDesignAirFlowRate);
  Q_PROPERTY(bool isCoolingDesignAirFlowRateDefaulted READ isCoolingDesignAirFlowRateDefaulted);
  Q_PROPERTY(std::string heatingDesignAirFlowMethod READ heatingDesignAirFlowMethod WRITE setHeatingDesignAirFlowMethod RESET resetHeatingDesignAirFlowMethod);
  Q_PROPERTY(bool isHeatingDesignAirFlowMethodDefaulted READ isHeatingDesignAirFlowMethodDefaulted);
  Q_PROPERTY(double heatingDesignAirFlowRate READ heatingDesignAirFlowRate WRITE setHeatingDesignAirFlowRate RESET resetHeatingDesignAirFlowRate);
  Q_PROPERTY(bool isHeatingDesignAirFlowRateDefaulted READ isHeatingDesignAirFlowRateDefaulted);
  Q_PROPERTY(std::string systemOutdoorAirMethod READ systemOutdoorAirMethod WRITE setSystemOutdoorAirMethod RESET resetSystemOutdoorAirMethod);
  Q_PROPERTY(bool isSystemOutdoorAirMethodDefaulted READ isSystemOutdoorAirMethodDefaulted);

  public:

  SizingSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  SizingSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

  SizingSystem_Impl(const SizingSystem_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

  virtual ~SizingSystem_Impl() {}

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  // TODO: Handle Non-Extensible IddField AirLoop Name.

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

  // TODO: Handle Non-Extensible IddField AirLoop Name.

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

