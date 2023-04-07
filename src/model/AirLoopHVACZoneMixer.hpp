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

#ifndef MODEL_AIRLOOPHVACZONEMIXER_HPP
#define MODEL_AIRLOOPHVACZONEMIXER_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {
namespace model {

  class AirflowNetworkDistributionNode;

  namespace detail {
    class AirLoopHVACZoneMixer_Impl;
  }  // namespace detail

  /** AirLoopHVACZoneMixer is an interface to the EnergyPlus IDD object named "AirLoopHVAC:ZoneMixer"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of zone mixer objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the mixer.  Branch indexes
 *  are used to refer to the many inlet ports of the mixer
 */
  class MODEL_API AirLoopHVACZoneMixer : public Mixer
  {

   public:
    /** Constructs a new AirLoopHVACZoneMixer object and places it inside the model. */
    explicit AirLoopHVACZoneMixer(const Model& model);

    virtual ~AirLoopHVACZoneMixer() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACZoneMixer(const AirLoopHVACZoneMixer& other) = default;
    AirLoopHVACZoneMixer(AirLoopHVACZoneMixer&& other) = default;
    AirLoopHVACZoneMixer& operator=(const AirLoopHVACZoneMixer&) = default;
    AirLoopHVACZoneMixer& operator=(AirLoopHVACZoneMixer&&) = default;

    unsigned outletPort() const override;

    unsigned inletPort(unsigned branchIndex) const override;

    unsigned nextInletPort() const override;

    bool addToNode(Node& node);

    std::vector<openstudio::IdfObject> remove();

    ModelObject clone(Model model) const;

    void disconnect();

    AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

    boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

    static IddObjectType iddObjectType();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirLoopHVACZoneMixer_Impl;

    explicit AirLoopHVACZoneMixer(std::shared_ptr<detail::AirLoopHVACZoneMixer_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneMixer");

    AirLoopHVACZoneMixer(const Handle& handle, const Model& model);

    /// @endcond
  };

  /** \relates AirLoopHVACZoneMixer */
  using OptionalAirLoopHVACZoneMixer = boost::optional<AirLoopHVACZoneMixer>;

  /** \relates AirLoopHVACZoneMixer */
  using AirLoopHVACZoneMixerVector = std::vector<AirLoopHVACZoneMixer>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACZONEMIXER_HPP
