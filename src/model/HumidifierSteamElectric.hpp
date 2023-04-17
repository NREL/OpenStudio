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

#ifndef MODEL_HUMIDIFIERSTEAMELECTRIC_HPP
#define MODEL_HUMIDIFIERSTEAMELECTRIC_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class HumidifierSteamElectric_Impl;

  }  // namespace detail

  /** HumidifierSteamElectric is a StraightComponent that wraps the OpenStudio IDD object 'OS:Humidifier:Steam:Electric'. */
  class MODEL_API HumidifierSteamElectric : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HumidifierSteamElectric(const Model& model);

    virtual ~HumidifierSteamElectric() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HumidifierSteamElectric(const HumidifierSteamElectric& other) = default;
    HumidifierSteamElectric(HumidifierSteamElectric&& other) = default;
    HumidifierSteamElectric& operator=(const HumidifierSteamElectric&) = default;
    HumidifierSteamElectric& operator=(HumidifierSteamElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

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

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setRatedCapacity(double ratedCapacity);

    void autosizeRatedCapacity();

    bool setRatedPower(double ratedPower);

    void resetRatedPower();

    void autosizeRatedPower();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setStandbyPower(double standbyPower);

    void resetStandbyPower();

    // bool setWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedCapacity() const;

    boost::optional<double> autosizedRatedPower() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HumidifierSteamElectric_Impl;

    explicit HumidifierSteamElectric(std::shared_ptr<detail::HumidifierSteamElectric_Impl> impl);

    friend class detail::HumidifierSteamElectric_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HumidifierSteamElectric");
  };

  /** \relates HumidifierSteamElectric*/
  using OptionalHumidifierSteamElectric = boost::optional<HumidifierSteamElectric>;

  /** \relates HumidifierSteamElectric*/
  using HumidifierSteamElectricVector = std::vector<HumidifierSteamElectric>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HUMIDIFIERSTEAMELECTRIC_HPP
