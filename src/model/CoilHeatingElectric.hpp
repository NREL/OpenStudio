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

#ifndef MODEL_COILHEATINGELECTRIC_HPP
#define MODEL_COILHEATINGELECTRIC_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;
  class AirflowNewtorkEquivalentDuct;

  namespace detail {

    class CoilHeatingElectric_Impl;

  }  // namespace detail

  /** CoilHeatingElectric is a StraightComponent that wraps the OpenStudio IDD
 *  object 'OS_Coil_Heating_Electric'. */
  class MODEL_API CoilHeatingElectric : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingElectric(const Model& model, Schedule& schedule);

    CoilHeatingElectric(const Model& model);

    virtual ~CoilHeatingElectric() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingElectric(const CoilHeatingElectric& other) = default;
    CoilHeatingElectric(CoilHeatingElectric&& other) = default;
    CoilHeatingElectric& operator=(const CoilHeatingElectric&) = default;
    CoilHeatingElectric& operator=(CoilHeatingElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double efficiency() const;

    bool isEfficiencyDefaulted() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityDefaulted() const;

    bool isNominalCapacityAutosized() const;

    boost::optional<Node> temperatureSetpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setEfficiency(double efficiency);

    void resetEfficiency();

    bool setNominalCapacity(double nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    bool setTemperatureSetpointNode(Node& temperatureSetpointNode);

    void resetTemperatureSetpointNode();

    //@}

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond

    using ImplType = detail::CoilHeatingElectric_Impl;

    friend class detail::CoilHeatingElectric_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit CoilHeatingElectric(std::shared_ptr<detail::CoilHeatingElectric_Impl> impl);

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingElectric");
  };

  /** \relates CoilHeatingElectric*/
  using OptionalCoilHeatingElectric = boost::optional<CoilHeatingElectric>;

  /** \relates CoilHeatingElectric*/
  using CoilHeatingElectricVector = std::vector<CoilHeatingElectric>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRIC_HPP
