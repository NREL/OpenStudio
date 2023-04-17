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

#ifndef MODEL_ZONEHVACCOMPONENT_HPP
#define MODEL_ZONEHVACCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Node;
  class AirLoopHVAC;
  class AirLoopHVACReturnPlenum;

  namespace detail {

    class ZoneHVACComponent_Impl;

  };

  /** ZoneHVACComponent is the base class for HVAC related objects that exclusively condition a single zone.
 */
  class MODEL_API ZoneHVACComponent : public HVACComponent
  {
   public:
    virtual ~ZoneHVACComponent() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACComponent(const ZoneHVACComponent& other) = default;
    ZoneHVACComponent(ZoneHVACComponent&& other) = default;
    ZoneHVACComponent& operator=(const ZoneHVACComponent&) = default;
    ZoneHVACComponent& operator=(ZoneHVACComponent&&) = default;

    /** Returns the inlet port.  For a ZoneHVACComponent this port is typically connected
   *  to a zone exhaust node. **/
    unsigned inletPort() const;

    /** Returns the optional node connected to the inletPort(). **/
    boost::optional<Node> inletNode() const;

    /** Returns the outlet port. For a ZoneHVACComponent this port is typically connected
   *  to a zone air inlet node **/
    unsigned outletPort() const;

    /** Returns the optional node connected to the outletPort(). **/
    boost::optional<Node> outletNode() const;

    /** Returns the optional ThermalZone that this ZoneHVACComponent is attached to
   **/
    virtual boost::optional<ThermalZone> thermalZone() const;

    /** Adds this ZoneHVACComponent to the thermal zone while managing all
   *  node connections automatically.  Returns true if the operation was
   *  successful.
   **/
    bool addToThermalZone(ThermalZone& thermalZone);

    /** Detaches this ZoneHVACComponent from the associated ThermalZone.
   *  If there is no attached ThermalZone there is no effect.
   *  If the ThermalZone is attached via an AirLoopHVAC object (as a result of addToNode()),
   *  then this method will reverse the effects of addToNode, and also detach the ZoneHVACComponent
   *  from the AirLoopHVAC object.
   **/
    void removeFromThermalZone();

    /** Establish plenumZone as the return plenum for this ZoneHVACComponent.
   *  ZoneHVACComponent must already be attached to a ThermalZone
  *   The method canBePlenum called on plenumZone must return true.
  */
    bool setReturnPlenum(const ThermalZone& plenumZone);

    /** Remove any return plenum attached to this ZoneHVACComponent
  */
    void removeReturnPlenum();

    boost::optional<AirLoopHVACReturnPlenum> returnPlenum() const;

    /** Adds this ZoneHVACComponent to a node on an AirLoopHVAC object.
   *  The node must be located between a ThermalZone and a AirTerminalSingleDuctInletSideMixer object.
   *  This is used to feed an AirLoopHVAC structure (such as a DOAS, or any built up system) into a ZoneHVACComponent.
   *  If the ZoneHVACComponent object is already attached to a thermalZone() then it will first be detached using removeFromThermalZone().
   *
   *  Certain OpenStudio objects derived from the ZoneHVACComponent type,
   *  can be used as AirLoopHVAC supply components.
   *  One example is the AirLoopHVACUnitarySystem, which can be used as both
   *  a ZoneHVACComponent and a AirLoopHVAC component. In these cases
   *  addToNode can be used to add those components to the supply side of an
   *  AirLoopHVAC system.
   **/
    bool addToNode(Node& node);

    /** Returns the AirLoopHVAC attached to this ZoneHVACComponent.
   *  The AirLoopHVAC object would have been attached via addToNode */
    boost::optional<AirLoopHVAC> airLoopHVAC() const;

    /** If this ZoneHVACComponent is used as a supply component
   *  on an AirLoopHVAC system, then this method will detach
   *  this ZoneHVACComponent from the AirLoopHVAC system, otherwise
   *  this method does nothing.
   *
   *  Certain OpenStudio objects derived from the ZoneHVACComponent type,
   *  can be used as AirLoopHVAC supply components.
   *  One example is the AirLoopHVACUnitarySystem, which can be used as both
   *  a ZoneHVACComponent and a AirLoopHVAC component.
   *
   *  If this ZoneHVACComponent is on the demand side of AirLoopHVAC,
   *  between a ThermalZone and a AirTerminalSingleDuctInletSideMixer,
   *  then use the method removeFromThermalZone, instead of removeFromAirLoopHVAC
   *  to detach this ZoneHVACComponent from the zone and air system.
   **/
    bool removeFromAirLoopHVAC();

    /** Provided for backwards compatibility, use inletNode instead **/
    boost::optional<ModelObject> airInletModelObject() const;

    /** Provided for backwards compatibility, use outletNode instead **/
    boost::optional<ModelObject> airOutletModelObject() const;

   protected:
    ZoneHVACComponent(IddObjectType type, const Model& model);

    using ImplType = detail::ZoneHVACComponent_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    explicit ZoneHVACComponent(std::shared_ptr<ImplType> impl);

    virtual std::vector<ModelObject> children() const override;

   private:
    REGISTER_LOGGER("openstudio.model.HVACComponent");
  };

  using OptionalZoneHVACComponent = boost::optional<ZoneHVACComponent>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_ZONEHVACCOMPONENT_HPP
