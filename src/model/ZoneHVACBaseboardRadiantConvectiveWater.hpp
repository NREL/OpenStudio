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

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACBaseboardRadiantConvectiveWater_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardRadiantConvectiveWater is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Water'. */
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveWater : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACBaseboardRadiantConvectiveWater(const Model& model);

    virtual ~ZoneHVACBaseboardRadiantConvectiveWater() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardRadiantConvectiveWater(const ZoneHVACBaseboardRadiantConvectiveWater& other) = default;
    ZoneHVACBaseboardRadiantConvectiveWater(ZoneHVACBaseboardRadiantConvectiveWater&& other) = default;
    ZoneHVACBaseboardRadiantConvectiveWater& operator=(const ZoneHVACBaseboardRadiantConvectiveWater&) = default;
    ZoneHVACBaseboardRadiantConvectiveWater& operator=(ZoneHVACBaseboardRadiantConvectiveWater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    HVACComponent heatingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    bool setHeatingCoil(const HVACComponent& heatingCoil);

    //@}
    /** @name Other */
    //@{

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveWater is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardRadiantConvectiveWater to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardRadiantConvectiveWater from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl;

    explicit ZoneHVACBaseboardRadiantConvectiveWater(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl> impl);

    friend class detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveWater");
  };

  /** \relates ZoneHVACBaseboardRadiantConvectiveWater*/
  using OptionalZoneHVACBaseboardRadiantConvectiveWater = boost::optional<ZoneHVACBaseboardRadiantConvectiveWater>;

  /** \relates ZoneHVACBaseboardRadiantConvectiveWater*/
  using ZoneHVACBaseboardRadiantConvectiveWaterVector = std::vector<ZoneHVACBaseboardRadiantConvectiveWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_HPP
