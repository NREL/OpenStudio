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

#ifndef MODEL_AIRLOOPHVACEXHAUSTSYSTEM_HPP
#define MODEL_AIRLOOPHVACEXHAUSTSYSTEM_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class ZoneMixers;
  class FansSystemModel;

  namespace detail {

    class AirLoopHVACExhaustSystem_Impl;

  }  // namespace detail

  /** AirLoopHVACExhaustSystem is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:ExhaustSystem'. */
  class MODEL_API AirLoopHVACExhaustSystem : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirLoopHVACExhaustSystem(const Model& model);

    virtual ~AirLoopHVACExhaustSystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACExhaustSystem(const AirLoopHVACExhaustSystem& other) = default;
    AirLoopHVACExhaustSystem(AirLoopHVACExhaustSystem&& other) = default;
    AirLoopHVACExhaustSystem& operator=(const AirLoopHVACExhaustSystem&) = default;
    AirLoopHVACExhaustSystem& operator=(AirLoopHVACExhaustSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: ZoneMixers.
    ZoneMixers zoneMixer() const;

    // TODO: Check return type. From object lists, some candidates are: FansSystemModel, FansComponentModel.
    FansSystemModel fan() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: ZoneMixers.
    bool setZoneMixer(const ZoneMixers& zoneMixers);

    // TODO: Check argument type. From object lists, some candidates are: FansSystemModel, FansComponentModel.
    bool setFan(const FansSystemModel& fansSystemModel);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACExhaustSystem_Impl;

    explicit AirLoopHVACExhaustSystem(std::shared_ptr<detail::AirLoopHVACExhaustSystem_Impl> impl);

    friend class detail::AirLoopHVACExhaustSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACExhaustSystem");
  };

  /** \relates AirLoopHVACExhaustSystem*/
  using OptionalAirLoopHVACExhaustSystem = boost::optional<AirLoopHVACExhaustSystem>;

  /** \relates AirLoopHVACExhaustSystem*/
  using AirLoopHVACExhaustSystemVector = std::vector<AirLoopHVACExhaustSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACEXHAUSTSYSTEM_HPP