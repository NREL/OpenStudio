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

#ifndef MODEL_AIRFLOWNETWORKSpecifiedFlowRate_HPP
#define MODEL_AIRFLOWNETWORKSpecifiedFlowRate_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkSpecifiedFlowRate_Impl;

  }  // namespace detail

  /** AirflowNetworkSpecifiedFlowRate is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkSpecifiedFlowRate'. */
  class MODEL_API AirflowNetworkSpecifiedFlowRate : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct a specified flow rate object with a zero airflow value. */
    AirflowNetworkSpecifiedFlowRate(const Model& model);
    /** Construct a specified flow rate object with a specified airflow value. */
    AirflowNetworkSpecifiedFlowRate(const Model& model, double airFlowValue);

    virtual ~AirflowNetworkSpecifiedFlowRate() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkSpecifiedFlowRate(const AirflowNetworkSpecifiedFlowRate& other) = default;
    AirflowNetworkSpecifiedFlowRate(AirflowNetworkSpecifiedFlowRate&& other) = default;
    AirflowNetworkSpecifiedFlowRate& operator=(const AirflowNetworkSpecifiedFlowRate&) = default;
    AirflowNetworkSpecifiedFlowRate& operator=(AirflowNetworkSpecifiedFlowRate&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> airFlowUnitsValues();

    static std::vector<std::string> validAirFlowUnitsValues();

    /** @name Getters */
    //@{
    /** Returns the air flow value. */
    double airFlowValue() const;
    /** Returns the air flow units. */
    std::string airFlowUnits() const;
    /** Returns true if the air flow units is defaulted. */
    bool isAirFlowUnitsDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the air flow value. */
    bool setAirFlowValue(double airFlowValue);
    /** Sets the air flow units. */
    bool setAirFlowUnits(const std::string& airFlowUnits);
    /** Returns the air flow units to the default value. */
    void resetAirFlowUnits();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkSpecifiedFlowRate_Impl;

    explicit AirflowNetworkSpecifiedFlowRate(std::shared_ptr<detail::AirflowNetworkSpecifiedFlowRate_Impl> impl);

    friend class detail::AirflowNetworkSpecifiedFlowRate_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkSpecifiedFlowRate");
  };

  /** \relates AirflowNetworkSpecifiedFlowRate*/
  using OptionalAirflowNetworkSpecifiedFlowRate = boost::optional<AirflowNetworkSpecifiedFlowRate>;

  /** \relates AirflowNetworkSpecifiedFlowRate*/
  using AirflowNetworkSpecifiedFlowRateVector = std::vector<AirflowNetworkSpecifiedFlowRate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKSpecifiedFlowRate_HPP
