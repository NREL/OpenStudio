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

#ifndef MODEL_PORTLIST_HPP
#define MODEL_PORTLIST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Node;

  class ThermalZone;

  class HVACComponent;

  namespace detail {

    class PortList_Impl;

    class ThermalZone_Impl;

  }  // namespace detail

  /** PortList is a ModelObject that wraps the OpenStudio IDD object 'OS:PortList'. */
  class MODEL_API PortList : public ModelObject
  {
   public:
    explicit PortList(const HVACComponent& comp);

    virtual ~PortList() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PortList(const PortList& other) = default;
    PortList(PortList&& other) = default;
    PortList& operator=(const PortList&) = default;
    PortList& operator=(PortList&&) = default;

    static IddObjectType iddObjectType();

    /** Returns the port for portIndex.  Ports are consequtively
   *  indexed starting from 0.
   */
    unsigned port(unsigned portIndex) const;

    /** Given a port, return the port index.
   *  Consider that a "port" is simply an unsigned, so
   *  port and port index share the same type, but they do not share the same value.
   */
    unsigned portIndex(unsigned port) const;

    /** Returns the next available port.  This will be the first port
   *  with no connected objects */
    unsigned nextPort() const;

    /** Returns the optional ModelObject connected to the port designated by portIndex.
   *  If there is no connected object then the optional will be false.
   */
    boost::optional<ModelObject> modelObject(unsigned portIndex) const;

    /** Returns the optional ModelObject connected to the last port of the PortList.
   *  If there are no connections to the list's outlet ports, then the
   *  optional will be false.
   */
    boost::optional<ModelObject> lastModelObject();

    /** Returns a vector of all objects connected to the PortList's ports.
   *  If no objects are connected to the PortList then an empty vector will be returned.
   */
    std::vector<ModelObject> modelObjects() const;

    /** Returns the port index for the ModelObject specified by modelObject.
   */
    unsigned portIndexForModelObject(ModelObject& modelObject) const;

    /** Returns the index of the next available port */
    unsigned nextPortIndex() const;

    ThermalZone thermalZone() const;

    /** Returns the port connected to an AirLoopHVAC object.
   *  If there is no AirLoopHVAC object, then the next available port is returned
   */
    unsigned airLoopHVACPort() const;

    std::vector<unsigned> airLoopHVACPorts() const;

    /** Returns the port Index connected to an AirLoopHVAC object.
   *  If there is no AirLoopHVAC object, then the next available portIndex is returned
   */
    unsigned airLoopHVACPortIndex() const;

    std::vector<unsigned> airLoopHVACPortIndexes() const;

    /** Returns the object connected to an AirLoopHVAC object. */
    boost::optional<ModelObject> airLoopHVACModelObject() const;

    std::vector<ModelObject> airLoopHVACModelObjects() const;

   protected:
    /// @cond
    using ImplType = detail::PortList_Impl;

    explicit PortList(std::shared_ptr<detail::PortList_Impl> impl);

    friend class detail::PortList_Impl;
    friend class Model;
    friend class IdfObject;
    friend class ThermalZone;
    friend class detail::ThermalZone_Impl;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.PortList");
  };

  /** \relates PortList*/
  using OptionalPortList = boost::optional<PortList>;

  /** \relates PortList*/
  using PortListVector = std::vector<PortList>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PORTLIST_HPP
