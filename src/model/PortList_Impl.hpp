/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PORTLIST_IMPL_HPP
#define MODEL_PORTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;

  class ThermalZone;

  class HVACComponent;

  namespace detail {

    /** PortList_Impl is a ModelObject_Impl that is the implementation class for PortList.*/
    class MODEL_API PortList_Impl : public ModelObject_Impl
    {
     public:
      PortList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PortList_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PortList_Impl(const PortList_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PortList_Impl() override = default;

      unsigned port(unsigned portIndex) const;

      unsigned portIndex(unsigned port) const;

      unsigned nextPort() const;

      boost::optional<ModelObject> modelObject(unsigned portIndex) const;

      boost::optional<ModelObject> lastModelObject();

      std::vector<ModelObject> modelObjects() const;

      unsigned newPortAfterIndex(unsigned portIndex);

      unsigned portIndexForModelObject(ModelObject& modelObject, bool* ok = nullptr) const;

      unsigned nextPortIndex() const;

      void removePort(unsigned port);

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      ThermalZone thermalZone() const;

      std::vector<openstudio::IdfObject> remove() override;

      unsigned airLoopHVACPort() const;

      std::vector<unsigned> airLoopHVACPorts() const;

      unsigned airLoopHVACPortIndex() const;

      std::vector<unsigned> airLoopHVACPortIndexes() const;

      boost::optional<ModelObject> airLoopHVACModelObject() const;

      std::vector<ModelObject> airLoopHVACModelObjects() const;

      // Return the HVACComponent that the port list is associated with.
      HVACComponent hvacComponent() const;

      // Set the hvacComponent that the port list is associated with.
      bool setHVACComponent(const HVACComponent& hvacComponent);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PortList");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PORTLIST_IMPL_HPP
