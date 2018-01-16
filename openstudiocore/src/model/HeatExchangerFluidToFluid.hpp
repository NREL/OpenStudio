/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERFLUIDTOFLUID_HPP
#define MODEL_HEATEXCHANGERFLUIDTOFLUID_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Node;

namespace detail {

  class HeatExchangerFluidToFluid_Impl;

} // detail

/** HeatExchangerFluidToFluid is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatExchanger:FluidToFluid'. */
class MODEL_API HeatExchangerFluidToFluid : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit HeatExchangerFluidToFluid(const Model& model);

  virtual ~HeatExchangerFluidToFluid() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatExchangeModelTypeValues();

  static std::vector<std::string> controlTypeValues();

  static std::vector<std::string> heatTransferMeteringEndUseTypeValues();

  static std::vector<std::string> componentOverrideCoolingControlTemperatureModeValues();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> loopDemandSideDesignFlowRate() const;

  bool isLoopDemandSideDesignFlowRateAutosized() const;

  boost::optional<double> loopSupplySideDesignFlowRate() const;

  bool isLoopSupplySideDesignFlowRateAutosized() const;

  std::string heatExchangeModelType() const;

  bool isHeatExchangeModelTypeDefaulted() const;

  boost::optional<double> heatExchangerUFactorTimesAreaValue() const;

  bool isHeatExchangerUFactorTimesAreaValueAutosized() const;

  std::string controlType() const;

  bool isControlTypeDefaulted() const;

  double minimumTemperatureDifferencetoActivateHeatExchanger() const;

  bool isMinimumTemperatureDifferencetoActivateHeatExchangerDefaulted() const;

  std::string heatTransferMeteringEndUseType() const;

  bool isHeatTransferMeteringEndUseTypeDefaulted() const;

  boost::optional<Node> componentOverrideLoopSupplySideInletNode() const;

  boost::optional<Node> componentOverrideLoopDemandSideInletNode() const;

  std::string componentOverrideCoolingControlTemperatureMode() const;

  bool isComponentOverrideCoolingControlTemperatureModeDefaulted() const;

  double sizingFactor() const;

  bool isSizingFactorDefaulted() const;

  boost::optional<double> operationMinimumTemperatureLimit() const;

  boost::optional<double> operationMaximumTemperatureLimit() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setLoopDemandSideDesignFlowRate(double loopDemandSideDesignFlowRate);

  void autosizeLoopDemandSideDesignFlowRate();

  bool setLoopSupplySideDesignFlowRate(double loopSupplySideDesignFlowRate);

  void autosizeLoopSupplySideDesignFlowRate();

  bool setHeatExchangeModelType(std::string heatExchangeModelType);

  void resetHeatExchangeModelType();

  bool setHeatExchangerUFactorTimesAreaValue(double heatExchangerUFactorTimesAreaValue);

  void autosizeHeatExchangerUFactorTimesAreaValue();

  bool setControlType(std::string controlType);

  void resetControlType();

  bool setMinimumTemperatureDifferencetoActivateHeatExchanger(double minimumTemperatureDifferencetoActivateHeatExchanger);

  void resetMinimumTemperatureDifferencetoActivateHeatExchanger();

  bool setHeatTransferMeteringEndUseType(std::string heatTransferMeteringEndUseType);

  void resetHeatTransferMeteringEndUseType();

  bool setComponentOverrideLoopSupplySideInletNode(const Node& node);

  void resetComponentOverrideLoopSupplySideInletNode();

  bool setComponentOverrideLoopDemandSideInletNode(const Node& node);

  void resetComponentOverrideLoopDemandSideInletNode();

  bool setComponentOverrideCoolingControlTemperatureMode(std::string componentOverrideCoolingControlTemperatureMode);

  void resetComponentOverrideCoolingControlTemperatureMode();

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

  bool setOperationMinimumTemperatureLimit(double operationMinimumTemperatureLimit);

  void resetOperationMinimumTemperatureLimit();

  bool setOperationMaximumTemperatureLimit(double operationMaximumTemperatureLimit);

  void resetOperationMaximumTemperatureLimit();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedLoopDemandSideDesignFlowRate() const ;

  boost::optional<double> autosizedLoopSupplySideDesignFlowRate() const ;

  boost::optional<double> autosizedHeatExchangerUFactorTimesAreaValue() const ;



  //@}
 protected:
  /// @cond
  typedef detail::HeatExchangerFluidToFluid_Impl ImplType;

  explicit HeatExchangerFluidToFluid(std::shared_ptr<detail::HeatExchangerFluidToFluid_Impl> impl);

  friend class detail::HeatExchangerFluidToFluid_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.HeatExchangerFluidToFluid");
};

/** \relates HeatExchangerFluidToFluid*/
typedef boost::optional<HeatExchangerFluidToFluid> OptionalHeatExchangerFluidToFluid;

/** \relates HeatExchangerFluidToFluid*/
typedef std::vector<HeatExchangerFluidToFluid> HeatExchangerFluidToFluidVector;

} // model
} // openstudio

#endif // MODEL_HEATEXCHANGERFLUIDTOFLUID_HPP
