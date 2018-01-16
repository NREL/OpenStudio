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

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_IMPL_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;
class ZoneHVACEnergyRecoveryVentilatorController;

namespace detail {

  /** ZoneHVACEnergyRecoveryVentilator_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACEnergyRecoveryVentilator.*/
  class MODEL_API ZoneHVACEnergyRecoveryVentilator_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACEnergyRecoveryVentilator_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACEnergyRecoveryVentilator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACEnergyRecoveryVentilator_Impl(const ZoneHVACEnergyRecoveryVentilator_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~ZoneHVACEnergyRecoveryVentilator_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    //@}
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

  boost::optional<double> autosizedSupplyAirFlowRate() const ;

  boost::optional<double> autosizedExhaustAirFlowRate() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent);

    bool setSupplyAirFlowRate(boost::optional<double> supplyAirFlowRate);

    void autosizeSupplyAirFlowRate();

    bool setExhaustAirFlowRate(boost::optional<double> exhaustAirFlowRate);

    void autosizeExhaustAirFlowRate();

    bool setSupplyAirFan(const HVACComponent& supplyAirFan);

    bool setExhaustAirFan(const HVACComponent& exhaustAirFan);

    bool setController(const boost::optional<ZoneHVACEnergyRecoveryVentilatorController>& zoneHVACEnergyRecoveryVentilatorController);

    void resetController();

    bool setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea);

    bool setVentilationRateperOccupant(double ventilationRateperOccupant);

    // bool setAvailabilityManagerList(const boost::optional<ModelObject>& systemAvailabilityManagerLists);

    // void resetAvailabilityManagerList();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilator");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalHeatExchanger() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalExhaustAirFan() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_IMPL_HPP

