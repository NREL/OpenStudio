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

#ifndef MODEL_AIRFLOWNETWORKNODE_HPP
#define MODEL_AIRFLOWNETWORKNODE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class AirflowNetworkNode_Impl;
  }

  /** AirflowNetworkNode is the base class for AirflowNetwork objects that are nodes in the pressure network.
 *
 */
  class MODEL_API AirflowNetworkNode : public ModelObject
  {

   public:
    AirflowNetworkNode(IddObjectType type, const Model& model);

    virtual ~AirflowNetworkNode() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkNode(const AirflowNetworkNode& other) = default;
    AirflowNetworkNode(AirflowNetworkNode&& other) = default;
    AirflowNetworkNode& operator=(const AirflowNetworkNode&) = default;
    AirflowNetworkNode& operator=(AirflowNetworkNode&&) = default;

    //std::vector<openstudio::IdfObject> remove();

    //bool removeFromLoop();

    //** Returns the inlet port. **/
    //unsigned inletPort() const;

    //** Returns the outlet port. **/
    //unsigned outletPort() const;

    //** Returns the optional ModelObject connected to the inlet port. **/
    //boost::optional<ModelObject> inletModelObject() const;

    //** Returns the optional ModelObject connected to the outlet port. **/
    //boost::optional<ModelObject> outletModelObject() const;

    //** Returns the optional AirLoopHVAC object that this AirToAirComponent is attached to.
    // *
    // *  Reimplemented from HVACComponent.
    // */
    //boost::optional<AirLoopHVAC> airLoopHVAC() const;

    //bool addToNode(Node & node);

    //ModelObject clone(Model model) const;

    //void disconnect();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirflowNetworkNode_Impl;

    explicit AirflowNetworkNode(std::shared_ptr<detail::AirflowNetworkNode_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkNode");

    /// @endcond
  };

  using OptionalAirflowNetworkNode = boost::optional<AirflowNetworkNode>;

  using AirflowNetworkNodeVector = std::vector<AirflowNetworkNode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKNODE_HPP
