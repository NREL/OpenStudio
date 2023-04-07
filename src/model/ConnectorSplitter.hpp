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

#ifndef MODEL_CONNECTORSPLITTER_HPP
#define MODEL_CONNECTORSPLITTER_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {

namespace model {

  namespace detail {
    class ConnectorSplitter_Impl;
  }  // namespace detail

  /** ConnectorSplitter is an interface to the EnergyPlus IDD object named "OS:Connector:Splitter"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of splitter objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the splitter.  Branch indexes
 *  are used to refer to the many outlet ports of the mixer
 */
  class MODEL_API ConnectorSplitter : public Splitter
  {
   public:
    /** Constructs a new Splitter object and places it inside the model. */
    explicit ConnectorSplitter(const Model& model);

    virtual ~ConnectorSplitter() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConnectorSplitter(const ConnectorSplitter& other) = default;
    ConnectorSplitter(ConnectorSplitter&& other) = default;
    ConnectorSplitter& operator=(const ConnectorSplitter&) = default;
    ConnectorSplitter& operator=(ConnectorSplitter&&) = default;

    /** Returns the inlet port to the splitter. */
    unsigned inletPort() const override;

    /** Returns the outlet port for branchIndex.  Branches consequtively
   *  indexed starting from 0.
   */
    unsigned outletPort(unsigned branchIndex) const override;

    /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
    unsigned nextOutletPort() const override;

    virtual bool addToNode(Node& node);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::ConnectorSplitter_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit ConnectorSplitter(std::shared_ptr<detail::ConnectorSplitter_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ConnectorSplitter");
  };

  /** \relates ConnectorSplitter */
  using OptionalConnectorSplitter = boost::optional<ConnectorSplitter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONNECTORSPLITTER_HPP
