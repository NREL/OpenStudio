/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_TEMPERINGVALVE_HPP
#define MODEL_TEMPERINGVALVE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

  class TemperingValve_Impl;

} // detail

/** TemperingValve is a StraightComponent that wraps the OpenStudio IDD object 'OS:TemperingValve'. */
class MODEL_API TemperingValve : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit TemperingValve(const Model& model);

  virtual ~TemperingValve() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Node> stream2SourceNode() const;

  boost::optional<Node> temperatureSetpointNode() const;

  boost::optional<Node> pumpOutletNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setStream2SourceNode(const Node & stream2SourceNode);

  void resetStream2SourceNode();

  bool setTemperatureSetpointNode(const Node & temperatureSetpointNode);

  void resetTemperatureSetpointNode();

  bool setPumpOutletNode(const Node & pumpOutletNode);

  void resetPumpOutletNode();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::TemperingValve_Impl ImplType;

  explicit TemperingValve(std::shared_ptr<detail::TemperingValve_Impl> impl);

  friend class detail::TemperingValve_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.TemperingValve");
};

/** \relates TemperingValve*/
typedef boost::optional<TemperingValve> OptionalTemperingValve;

/** \relates TemperingValve*/
typedef std::vector<TemperingValve> TemperingValveVector;

} // model
} // openstudio

#endif // MODEL_TEMPERINGVALVE_HPP

