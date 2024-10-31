/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP
#define MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class PlanarSurface;
  class SolarCollectorPerformanceIntegralCollectorStorage;

  namespace detail {

    /** SolarCollectorIntegralCollectorStorage_Impl is a StraightComponent_Impl that is the implementation class for SolarCollectorIntegralCollectorStorage.*/
    class MODEL_API SolarCollectorIntegralCollectorStorage_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SolarCollectorIntegralCollectorStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SolarCollectorIntegralCollectorStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SolarCollectorIntegralCollectorStorage_Impl(const SolarCollectorIntegralCollectorStorage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SolarCollectorIntegralCollectorStorage_Impl() override = default;

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

      SolarCollectorPerformanceIntegralCollectorStorage solarCollectorPerformance() const;

      boost::optional<PlanarSurface> surface() const;

      std::string bottomSurfaceBoundaryConditionsType() const;

      bool isBottomSurfaceBoundaryConditionsTypeDefaulted() const;

      boost::optional<double> maximumFlowRate() const;

      //@}
      /** @name Setters */
      //@{

      bool setSolarCollectorPerformance(const SolarCollectorPerformanceIntegralCollectorStorage& solarCollectorPerformance);

      void resetSolarCollectorPerformance();

      bool setSurface(const PlanarSurface& surface);

      void resetSurface();

      bool setMaximumFlowRate(double maximumFlowRate);

      void resetMaximumFlowRate();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SolarCollectorIntegralCollectorStorage");

      bool setSolarCollectorPerformanceNoClone(const SolarCollectorPerformanceIntegralCollectorStorage& solarCollectorPerformance);

      friend class openstudio::model::SolarCollectorIntegralCollectorStorage;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP
