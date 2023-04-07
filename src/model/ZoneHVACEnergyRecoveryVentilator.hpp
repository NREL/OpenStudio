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

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;
  class ZoneHVACEnergyRecoveryVentilatorController;

  namespace detail {

    class ZoneHVACEnergyRecoveryVentilator_Impl;

  }  // namespace detail

  /** ZoneHVACEnergyRecoveryVentilator is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:EnergyRecoveryVentilator'. */
  class MODEL_API ZoneHVACEnergyRecoveryVentilator : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACEnergyRecoveryVentilator(const Model& model);

    explicit ZoneHVACEnergyRecoveryVentilator(const Model& model, const HVACComponent& heatExchanger, const HVACComponent& supplyAirFan,
                                              const HVACComponent& exhaustAirFan);

    virtual ~ZoneHVACEnergyRecoveryVentilator() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACEnergyRecoveryVentilator(const ZoneHVACEnergyRecoveryVentilator& other) = default;
    ZoneHVACEnergyRecoveryVentilator(ZoneHVACEnergyRecoveryVentilator&& other) = default;
    ZoneHVACEnergyRecoveryVentilator& operator=(const ZoneHVACEnergyRecoveryVentilator&) = default;
    ZoneHVACEnergyRecoveryVentilator& operator=(ZoneHVACEnergyRecoveryVentilator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    HVACComponent heatExchanger() const;

    boost::optional<double> supplyAirFlowRate() const;

    bool isSupplyAirFlowRateAutosized() const;

    boost::optional<double> exhaustAirFlowRate() const;

    bool isExhaustAirFlowRateAutosized() const;

    HVACComponent supplyAirFan() const;

    HVACComponent exhaustAirFan() const;

    boost::optional<ZoneHVACEnergyRecoveryVentilatorController> controller() const;

    double ventilationRateperUnitFloorArea() const;

    double ventilationRateperOccupant() const;

    // boost::optional<ModelObject> availabilityManagerList() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent);

    bool setSupplyAirFlowRate(double supplyAirFlowRate);

    void autosizeSupplyAirFlowRate();

    bool setExhaustAirFlowRate(double exhaustAirFlowRate);

    void autosizeExhaustAirFlowRate();

    bool setSupplyAirFan(const HVACComponent& supplyAirFan);

    bool setExhaustAirFan(const HVACComponent& exhaustAirFan);

    bool setController(const ZoneHVACEnergyRecoveryVentilatorController& zoneHVACEnergyRecoveryVentilatorController);

    void resetController();

    bool setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea);

    bool setVentilationRateperOccupant(double ventilationRateperOccupant);

    // bool setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists);

    // void resetAvailabilityManagerList();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedSupplyAirFlowRate() const;

    boost::optional<double> autosizedExhaustAirFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACEnergyRecoveryVentilator_Impl;

    explicit ZoneHVACEnergyRecoveryVentilator(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilator_Impl> impl);

    friend class detail::ZoneHVACEnergyRecoveryVentilator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilator");
  };

  /** \relates ZoneHVACEnergyRecoveryVentilator*/
  using OptionalZoneHVACEnergyRecoveryVentilator = boost::optional<ZoneHVACEnergyRecoveryVentilator>;

  /** \relates ZoneHVACEnergyRecoveryVentilator*/
  using ZoneHVACEnergyRecoveryVentilatorVector = std::vector<ZoneHVACEnergyRecoveryVentilator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_HPP
