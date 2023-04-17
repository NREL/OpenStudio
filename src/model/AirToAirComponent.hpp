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

#ifndef MODEL_AIRTOAIRCOMPONENT_HPP
#define MODEL_AIRTOAIRCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class AirToAirComponent_Impl;
  }

  class AirLoopHVAC;

  /** AirToAirComponent is the base class for model objects which interact with
 *  two air streams.
 *
 *  These objects are typically heat recovery devices such as the EnergyPlus IDD object
 *  named HeatExchanger:AirToAir:SensibleAndLatent.
 */
  class MODEL_API AirToAirComponent : public HVACComponent
  {
   public:
    AirToAirComponent(IddObjectType type, const Model& model);

    virtual ~AirToAirComponent() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirToAirComponent(const AirToAirComponent& other) = default;
    AirToAirComponent(AirToAirComponent&& other) = default;
    AirToAirComponent& operator=(const AirToAirComponent&) = default;
    AirToAirComponent& operator=(AirToAirComponent&&) = default;

    /** Returns the primary air stream inlet port.
   *
   *  In the conext of air to air heat recovery devices in EnergyPlus,
   *  the primary air stream is often thought of as the supply side air stream
   *  which is the supply of outdoor air into the system.
   */
    unsigned primaryAirInletPort() const;

    /** Returns the primary air stream outlet port. **/
    unsigned primaryAirOutletPort() const;

    /** Returns the optional ModelObject connected to the primary air stream inlet port. **/
    boost::optional<ModelObject> primaryAirInletModelObject() const;

    /** Returns the optional ModelObject connected to the primary air stream outlet port. **/
    boost::optional<ModelObject> primaryAirOutletModelObject() const;

    /** Returns the secondary air stream inlet port.
   *
   *  In the context of air to air heat recovery devices in EnergyPlus,
   *  the secondary air stream is often the exhaust air stream leaving the system.
   */
    unsigned secondaryAirInletPort() const;

    /** Returns the secondary air stream outlet port. **/
    unsigned secondaryAirOutletPort() const;

    /** Returns the optional ModelObject connected to the secondary air stream inlet port. **/
    boost::optional<ModelObject> secondaryAirInletModelObject() const;

    /** Returns the optional ModelObject connected to the secondary air stream outlet port. **/
    boost::optional<ModelObject> secondaryAirOutletModelObject() const;

    bool addToNode(Node& node);

    std::vector<openstudio::IdfObject> remove();

    ModelObject clone(Model model) const;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirToAirComponent_Impl;

    explicit AirToAirComponent(std::shared_ptr<detail::AirToAirComponent_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirToAirComponent");

    /// @endcond
  };

  using OptionalAirToAirComponent = boost::optional<AirToAirComponent>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTOAIRCOMPONENT_HPP
