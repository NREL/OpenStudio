/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PIPEOUTDOOR_IMPL_HPP
#define MODEL_PIPEOUTDOOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Construction;
  class Node;

  namespace detail {

    /** PipeOutdoor_Impl is a StraightComponent_Impl that is the implementation class for PipeOutdoor.*/
    class MODEL_API PipeOutdoor_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PipeOutdoor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PipeOutdoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PipeOutdoor_Impl(const PipeOutdoor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PipeOutdoor_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Construction> construction() const;

      boost::optional<Node> ambientTemperatureOutdoorAirNode() const;

      double pipeInsideDiameter() const;

      double pipeLength() const;

      //@}
      /** @name Setters */
      //@{

      bool setConstruction(const boost::optional<Construction>& construction);

      void resetConstruction();

      bool setAmbientTemperatureOutdoorAirNode(const boost::optional<Node>& node);

      void resetAmbientTemperatureOutdoorAirNode();

      bool setPipeInsideDiameter(double pipeInsideDiameter);

      bool setPipeLength(double pipeLength);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PipeOutdoor");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PIPEOUTDOOR_IMPL_HPP
