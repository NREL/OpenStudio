/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_CHILLERELECTRICASHRAE205_HPP
#define MODEL_CHILLERELECTRICASHRAE205_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class ExternalFile;
  class Schedule;
  class ThermalZone;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;
  class Connection;

  namespace detail {

    class ChillerElectricASHRAE205_Impl;

  }  // namespace detail

  /** ChillerElectricASHRAE205 is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Electric:ASHRAE205'. */
  class MODEL_API ChillerElectricASHRAE205 : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ChillerElectricASHRAE205(const Model& model);

    virtual ~ChillerElectricASHRAE205() = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInterpolationMethodValues();

    static std::vector<std::string> ambientTemperatureIndicatorValues();

    static std::vector<std::string> chillerFlowModeValues();

    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: ExternalFile.
    ExternalFile representationFile() const;

    std::string performanceInterpolationMethod() const;

    boost::optional<double> ratedCapacity() const;

    bool isRatedCapacityAutosized() const;

    boost::optional<double> autosizedRatedCapacity();

    double sizingFactor() const;

    std::string ambientTemperatureIndicator() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> ambientTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    boost::optional<ThermalZone> ambientTemperatureZone() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> ambientTemperatureOutdoorAirNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection chilledWaterInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection chilledWaterOutletNode() const;

    boost::optional<double> chilledWaterMaximumRequestedFlowRate() const;

    bool isChilledWaterMaximumRequestedFlowRateAutosized() const;

    boost::optional<double> autosizedChilledWaterMaximumRequestedFlowRate();

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> condenserInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> condenserOutletNode() const;

    boost::optional<double> condenserMaximumRequestedFlowRate() const;

    bool isCondenserMaximumRequestedFlowRateAutosized() const;

    boost::optional<double> autosizedCondenserMaximumRequestedFlowRate();

    std::string chillerFlowMode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> oilCoolerInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> oilCoolerOutletNode() const;

    boost::optional<double> oilCoolerDesignFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> auxiliaryInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> auxiliaryOutletNode() const;

    boost::optional<double> auxiliaryCoolingDesignFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> heatRecoveryInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> heatRecoveryOutletNode() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: ExternalFile.
    bool setRepresentationFile(const ExternalFile& externalFile);

    bool setPerformanceInterpolationMethod(const std::string& performanceInterpolationMethod);

    bool setRatedCapacity(double ratedCapacity);

    void autosizeRatedCapacity();

    bool setSizingFactor(double sizingFactor);

    bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    // Note Schedules are passed by reference, not const reference.
    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setAmbientTemperatureZone(const ThermalZone& thermalZone);

    void resetAmbientTemperatureZone();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAmbientTemperatureOutdoorAirNode(const Connection& connection);

    void resetAmbientTemperatureOutdoorAirNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setChilledWaterInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setChilledWaterOutletNode(const Connection& connection);

    bool setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate);

    void autosizeChilledWaterMaximumRequestedFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCondenserInletNode(const Connection& connection);

    void resetCondenserInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCondenserOutletNode(const Connection& connection);

    void resetCondenserOutletNode();

    bool setCondenserMaximumRequestedFlowRate(double condenserMaximumRequestedFlowRate);

    void autosizeCondenserMaximumRequestedFlowRate();

    bool setChillerFlowMode(const std::string& chillerFlowMode);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setOilCoolerInletNode(const Connection& connection);

    void resetOilCoolerInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setOilCoolerOutletNode(const Connection& connection);

    void resetOilCoolerOutletNode();

    bool setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate);

    void resetOilCoolerDesignFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAuxiliaryInletNode(const Connection& connection);

    void resetAuxiliaryInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAuxiliaryOutletNode(const Connection& connection);

    void resetAuxiliaryOutletNode();

    bool setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate);

    void resetAuxiliaryCoolingDesignFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHeatRecoveryInletNode(const Connection& connection);

    void resetHeatRecoveryInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHeatRecoveryOutletNode(const Connection& connection);

    void resetHeatRecoveryOutletNode();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerElectricASHRAE205_Impl;

    explicit ChillerElectricASHRAE205(std::shared_ptr<detail::ChillerElectricASHRAE205_Impl> impl);

    friend class detail::ChillerElectricASHRAE205_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerElectricASHRAE205");
  };

  /** \relates ChillerElectricASHRAE205*/
  using OptionalChillerElectricASHRAE205 = boost::optional<ChillerElectricASHRAE205>;

  /** \relates ChillerElectricASHRAE205*/
  using ChillerElectricASHRAE205Vector = std::vector<ChillerElectricASHRAE205>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICASHRAE205_HPP
