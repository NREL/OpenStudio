/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~PortList() override = default;
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
