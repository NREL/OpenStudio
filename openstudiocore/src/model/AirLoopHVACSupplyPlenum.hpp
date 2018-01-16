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

#ifndef MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP
#define MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {
namespace model {

class ThermalZone;

namespace detail {

  class AirLoopHVACSupplyPlenum_Impl;

} // detail

/** AirLoopHVACSupplyPlenum is a Splitter that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:SupplyPlenum'. */
class MODEL_API AirLoopHVACSupplyPlenum : public Splitter {
 public:

  explicit AirLoopHVACSupplyPlenum(const Model& model);

  virtual ~AirLoopHVACSupplyPlenum() {}

  static IddObjectType iddObjectType();

  boost::optional<ThermalZone> thermalZone() const;

  /** Establish thermalZone as the plenum zone.
   *  If thermalZone is already conditioned by ThermalZone::equipment(),
   *  then this method will return false.  A ThermalZone cannot be conditioned
   *  by equipment and and be a plenum zone.  Remove any existing ZoneHVACComponent objects,
   *  and associated AirLoopHVAC objects before using this method on thermalZone.
  **/
  bool setThermalZone(const ThermalZone& thermalZone);

  void resetThermalZone();

  unsigned inletPort() override;

  unsigned outletPort(unsigned branchIndex) override;

  unsigned nextOutletPort() override;

  /** Insert this AirLoopHVACSupplyPlenum into HVAC system at node.
   *  Node must be immediatly after AirLoopHVACZoneSplitter and there
   *  cannot be an existing AirLoopHVACSupplyPlenum on the node's branch
   */
  bool addToNode(Node & node);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone, HVACComponent & terminal);

 protected:
  /// @cond
  typedef detail::AirLoopHVACSupplyPlenum_Impl ImplType;

  explicit AirLoopHVACSupplyPlenum(std::shared_ptr<detail::AirLoopHVACSupplyPlenum_Impl> impl);

  friend class detail::AirLoopHVACSupplyPlenum_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirLoopHVACSupplyPlenum");
};

/** \relates AirLoopHVACSupplyPlenum*/
typedef boost::optional<AirLoopHVACSupplyPlenum> OptionalAirLoopHVACSupplyPlenum;

/** \relates AirLoopHVACSupplyPlenum*/
typedef std::vector<AirLoopHVACSupplyPlenum> AirLoopHVACSupplyPlenumVector;

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP

