/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP
#define MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** HumidifierSteamElectric_Impl is a StraightComponent_Impl that is the implementation class for HumidifierSteamElectric.*/
  class MODEL_API HumidifierSteamElectric_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    HumidifierSteamElectric_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    HumidifierSteamElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    HumidifierSteamElectric_Impl(const HumidifierSteamElectric_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~HumidifierSteamElectric_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> ratedCapacity() const;

    bool isRatedCapacityAutosized() const;

    boost::optional<double> ratedPower() const;

    bool isRatedPowerAutosized() const;

    boost::optional<double> ratedFanPower() const;

    boost::optional<double> standbyPower() const;

    // boost::optional<WaterStorageTank> waterStorageTank() const;

  boost::optional<double> autosizedRatedCapacity() const ;

  boost::optional<double> autosizedRatedPower() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRatedCapacity(boost::optional<double> ratedCapacity);

    void autosizeRatedCapacity();

    bool setRatedPower(boost::optional<double> ratedPower);

    void resetRatedPower();

    void autosizeRatedPower();

    bool setRatedFanPower(boost::optional<double> ratedFanPower);

    void resetRatedFanPower();

    bool setStandbyPower(boost::optional<double> standbyPower);

    void resetStandbyPower();

    // bool setWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    // void resetWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.HumidifierSteamElectric");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_HUMIDIFIERSTEAMELECTRIC_IMPL_HPP

