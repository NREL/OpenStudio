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

#ifndef MODEL_WATERUSECONNECTIONS_HPP
#define MODEL_WATERUSECONNECTIONS_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class WaterUseEquipment;

  namespace detail {

    class WaterUseConnections_Impl;

  }  // namespace detail

  /** WaterUseConnections is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:WaterUse:Connections'. */
  class MODEL_API WaterUseConnections : public StraightComponent
  {

   public:
    explicit WaterUseConnections(const Model& model);

    virtual ~WaterUseConnections() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterUseConnections(const WaterUseConnections& other) = default;
    WaterUseConnections(WaterUseConnections&& other) = default;
    WaterUseConnections& operator=(const WaterUseConnections&) = default;
    WaterUseConnections& operator=(WaterUseConnections&&) = default;

    static IddObjectType iddObjectType();

    boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

    bool setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule);

    void resetHotWaterSupplyTemperatureSchedule();

    boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

    bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

    void resetColdWaterSupplyTemperatureSchedule();

    std::vector<WaterUseEquipment> waterUseEquipment() const;

    bool addWaterUseEquipment(const WaterUseEquipment& waterUseEquipment);

    // Detaches waterUseEquipment from this object but does not remove it from the model
    bool removeWaterUseEquipment(WaterUseEquipment& waterUseEquipment);

    unsigned inletPort() const;

    unsigned outletPort() const;

    std::string drainWaterHeatExchangerType() const;

    bool setDrainWaterHeatExchangerType(const std::string& drainWaterHeatExchangerType);

    std::string drainWaterHeatExchangerDestination() const;

    bool setDrainWaterHeatExchangerDestination(const std::string& drainWaterHeatExchangerDestination);

    boost::optional<double> drainWaterHeatExchangerUFactorTimesArea() const;

    bool setDrainWaterHeatExchangerUFactorTimesArea(double drainWaterHeatExchangerUFactorTimesArea);

    void resetDrainWaterHeatExchangerUFactorTimesArea();

   protected:
    /// @cond
    using ImplType = detail::WaterUseConnections_Impl;

    explicit WaterUseConnections(std::shared_ptr<detail::WaterUseConnections_Impl> impl);

    friend class detail::WaterUseConnections_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.WaterUseConnections");
  };

  /** \relates WaterUseConnections*/
  using OptionalWaterUseConnections = boost::optional<WaterUseConnections>;

  /** \relates WaterUseConnections*/
  using WaterUseConnectionsVector = std::vector<WaterUseConnections>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSECONNECTIONS_HPP
