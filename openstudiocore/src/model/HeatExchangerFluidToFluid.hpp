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

  void setOperationMinimumTemperatureLimit(double operationMinimumTemperatureLimit);

  void resetOperationMinimumTemperatureLimit();

  void setOperationMaximumTemperatureLimit(double operationMaximumTemperatureLimit);

  void resetOperationMaximumTemperatureLimit();

  //@}
  /** @name Other */
  //@{

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

