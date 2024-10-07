/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORFLATPLATEWATER_IMPL_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEWATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorPerformanceFlatPlate;
  class PlanarSurface;
  //class Connection;

  namespace detail {

    /** SolarCollectorFlatPlateWater_Impl is a StraightComponent_Impl that is the implementation class for SolarCollectorFlatPlateWater.*/
    class MODEL_API SolarCollectorFlatPlateWater_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SolarCollectorFlatPlateWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SolarCollectorFlatPlateWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SolarCollectorFlatPlateWater_Impl(const SolarCollectorFlatPlateWater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SolarCollectorFlatPlateWater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

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

      SolarCollectorPerformanceFlatPlate solarCollectorPerformance() const;

      boost::optional<PlanarSurface> surface() const;

      //Connection inletNode() const;

      //Connection outletNode() const;

      boost::optional<double> maximumFlowRate() const;

      //@}
      /** @name Setters */
      //@{

      bool setSolarCollectorPerformance(const SolarCollectorPerformanceFlatPlate& solarCollectorPerformanceFlatPlate);

      void resetSolarCollectorPerformance();

      bool setSurface(const PlanarSurface& surface);

      void resetSurface();

      //bool setInletNode(const Connection& connection);

      //bool setOutletNode(const Connection& connection);

      bool setMaximumFlowRate(double maximumFlowRate);

      void resetMaximumFlowRate();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlateWater");

      bool setSolarCollectorPerformanceNoClone(const SolarCollectorPerformanceFlatPlate& parameters);

      friend class openstudio::model::SolarCollectorFlatPlateWater;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORFLATPLATEWATER_IMPL_HPP
