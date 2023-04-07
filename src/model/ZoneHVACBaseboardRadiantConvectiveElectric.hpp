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

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ZoneHVACBaseboardRadiantConvectiveElectric_Impl;

  }  // namespace detail

  /** ZoneHVACBaseboardRadiantConvectiveElectric is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Baseboard:RadiantConvective:Electric'. */
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveElectric : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACBaseboardRadiantConvectiveElectric(const Model& model);

    virtual ~ZoneHVACBaseboardRadiantConvectiveElectric() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACBaseboardRadiantConvectiveElectric(const ZoneHVACBaseboardRadiantConvectiveElectric& other) = default;
    ZoneHVACBaseboardRadiantConvectiveElectric(ZoneHVACBaseboardRadiantConvectiveElectric&& other) = default;
    ZoneHVACBaseboardRadiantConvectiveElectric& operator=(const ZoneHVACBaseboardRadiantConvectiveElectric&) = default;
    ZoneHVACBaseboardRadiantConvectiveElectric& operator=(ZoneHVACBaseboardRadiantConvectiveElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    double efficiency() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setEfficiency(double efficiency);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    /** Returns the optional ThermalZone that this ZoneHVACBaseboardRadiantConvectiveElectric is attached to
   **/
    boost::optional<ThermalZone> thermalZone() const override;

    /** Adds this ZoneHVACBaseboardRadiantConvectiveElectric to the thermal zone.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACBaseboardRadiantConvectiveElectric from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   **/
    void removeFromThermalZone();

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl;

    explicit ZoneHVACBaseboardRadiantConvectiveElectric(std::shared_ptr<detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl> impl);

    friend class detail::ZoneHVACBaseboardRadiantConvectiveElectric_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveElectric");
  };

  /** \relates ZoneHVACBaseboardRadiantConvectiveElectric*/
  using OptionalZoneHVACBaseboardRadiantConvectiveElectric = boost::optional<ZoneHVACBaseboardRadiantConvectiveElectric>;

  /** \relates ZoneHVACBaseboardRadiantConvectiveElectric*/
  using ZoneHVACBaseboardRadiantConvectiveElectricVector = std::vector<ZoneHVACBaseboardRadiantConvectiveElectric>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_HPP
