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

#ifndef MODEL_AIRFLOWNETWORKFAN_HPP
#define MODEL_AIRFLOWNETWORKFAN_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class FanConstantVolume;
  class FanVariableVolume;
  class FanOnOff;
  class FanSystemModel;

  namespace detail {

    class AirflowNetworkFan_Impl;
    class FanConstantVolume_Impl;
    class FanVariableVolume_Impl;
    class FanOnOff_Impl;
    class FanSystemModel_Impl;

  }  // namespace detail

  /** AirflowNetworkFan is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:Fan'. */
  class MODEL_API AirflowNetworkFan : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AirflowNetworkFan() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkFan(const AirflowNetworkFan& other) = default;
    AirflowNetworkFan(AirflowNetworkFan&& other) = default;
    AirflowNetworkFan& operator=(const AirflowNetworkFan&) = default;
    AirflowNetworkFan& operator=(AirflowNetworkFan&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<FanConstantVolume> fanConstantVolume() const;
    boost::optional<FanVariableVolume> fanVariableVolume() const;
    boost::optional<FanOnOff> fanOnOff() const;
    boost::optional<FanSystemModel> fanSystemModel() const;

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkFan_Impl;

    explicit AirflowNetworkFan(std::shared_ptr<detail::AirflowNetworkFan_Impl> impl);

    AirflowNetworkFan(const Model& model, const Handle& handle);

    friend class detail::FanConstantVolume_Impl;
    friend class detail::FanVariableVolume_Impl;
    friend class detail::FanOnOff_Impl;
    friend class detail::FanSystemModel_Impl;

    void resetFan();

    friend class detail::AirflowNetworkFan_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkFan");
  };

  /** \relates AirflowNetworkFan*/
  using OptionalAirflowNetworkFan = boost::optional<AirflowNetworkFan>;

  /** \relates AirflowNetworkFan*/
  using AirflowNetworkFanVector = std::vector<AirflowNetworkFan>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKFAN_HPP
