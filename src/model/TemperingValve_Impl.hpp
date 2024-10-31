/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TEMPERINGVALVE_IMPL_HPP
#define MODEL_TEMPERINGVALVE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** TemperingValve_Impl is a StraightComponent_Impl that is the implementation class for TemperingValve.*/
    class MODEL_API TemperingValve_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      TemperingValve_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      TemperingValve_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      TemperingValve_Impl(const TemperingValve_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~TemperingValve_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> stream2SourceNode() const;

      boost::optional<Node> temperatureSetpointNode() const;

      boost::optional<Node> pumpOutletNode() const;

      //@}
      /** @name Setters */
      //@{

      bool setStream2SourceNode(const boost::optional<Node>& stream2SourceNode);

      void resetStream2SourceNode();

      bool setTemperatureSetpointNode(const boost::optional<Node>& temperatureSetpointNode);

      void resetTemperatureSetpointNode();

      bool setPumpOutletNode(const boost::optional<Node>& pumpOutletNode);

      void resetPumpOutletNode();

      //@}
      /** @name Other */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      // Set pumpOutletNode, temperatureSetpointNode, and stream2SourceNode to reasonable values if they are not set
      bool setControlNodes();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.TemperingValve");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Connection> optionalInletNode() const;
      boost::optional<Connection> optionalOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TEMPERINGVALVE_IMPL_HPP
