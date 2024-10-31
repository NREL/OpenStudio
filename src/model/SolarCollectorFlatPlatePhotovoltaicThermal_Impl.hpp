/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_IMPL_HPP
#define MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class GeneratorPhotovoltaic;
  class PlanarSurface;
  class SolarCollectorPerformancePhotovoltaicThermalBIPVT;
  class SolarCollectorPerformancePhotovoltaicThermalSimple;

  namespace detail {

    /** SolarCollectorFlatPlatePhotovoltaicThermal_Impl is a StraightComponent_Impl that is the implementation class for SolarCollectorFlatPlatePhotovoltaicThermal.*/
    class MODEL_API SolarCollectorFlatPlatePhotovoltaicThermal_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SolarCollectorFlatPlatePhotovoltaicThermal_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SolarCollectorFlatPlatePhotovoltaicThermal_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SolarCollectorFlatPlatePhotovoltaicThermal_Impl(const SolarCollectorFlatPlatePhotovoltaicThermal_Impl& other, Model_Impl* model,
                                                      bool keepHandle);

      virtual ~SolarCollectorFlatPlatePhotovoltaicThermal_Impl() override = default;

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

      boost::optional<PlanarSurface> surface() const;

      boost::optional<GeneratorPhotovoltaic> generatorPhotovoltaic() const;

      ModelObject solarCollectorPerformance() const;

      boost::optional<double> designFlowRate() const;

      bool isDesignFlowRateAutosized() const;

      boost::optional<double> autosizedDesignFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalBIPVT& performance);
      bool setSolarCollectorPerformance(const SolarCollectorPerformancePhotovoltaicThermalSimple& performance);

      void resetSolarCollectorPerformance();

      bool setSurface(const PlanarSurface& surface);

      void resetSurface();

      bool setGeneratorPhotovoltaic(const GeneratorPhotovoltaic& generatorPhotovoltaic);

      void resetGeneratorPhotovoltaic();

      bool setDesignFlowRate(double designFlowRate);

      void resetDesignFlowRate();

      void autosizeDesignFlowRate();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SolarCollectorFlatPlatePhotovoltaicThermal");

      bool setSolarCollectorPerformanceNoClone(const ModelObject& performance);

      friend class openstudio::model::SolarCollectorFlatPlatePhotovoltaicThermal;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORFLATPLATEPHOTOVOLTAICTHERMAL_IMPL_HPP
