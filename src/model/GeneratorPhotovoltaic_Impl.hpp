/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORPHOTOVOLTAIC_IMPL_HPP
#define MODEL_GENERATORPHOTOVOLTAIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

  class AllShadingAndHTSurfName;
  class PVModules;
  class Schedule;
  class PlanarSurface;
  class PhotovoltaicPerformance;

  namespace detail {

    /** GeneratorPhotovoltaic_Impl is a Generator_Impl that is the implementation class for GeneratorPhotovoltaic.*/
    class MODEL_API GeneratorPhotovoltaic_Impl : public Generator_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorPhotovoltaic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorPhotovoltaic_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorPhotovoltaic_Impl(const GeneratorPhotovoltaic_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorPhotovoltaic_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      /// returns PhotovoltaicPerformance
      virtual std::vector<ModelObject> children() const override;

      /// remove the object from the model's workspace
      virtual std::vector<openstudio::IdfObject> remove() override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      /// returns deletes reference to Surface from clone
      virtual ModelObject clone(Model model) const override;

      virtual std::string generatorObjectType() const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<PlanarSurface> surface() const;

      PhotovoltaicPerformance photovoltaicPerformance() const;

      std::string heatTransferIntegrationMode() const;

      bool isHeatTransferIntegrationModeDefaulted() const;

      double numberOfModulesInParallel() const;

      bool isNumberOfModulesInParallelDefaulted() const;

      double numberOfModulesInSeries() const;

      bool isNumberOfModulesInSeriesDefaulted() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setSurface(const PlanarSurface& surface);

      void resetSurface();

      bool setHeatTransferIntegrationMode(const std::string& heatTransferIntegrationMode);

      void resetHeatTransferIntegrationMode();

      bool setNumberOfModulesInParallel(double numberOfModulesInParallel);

      void resetNumberOfModulesInParallel();

      bool setNumberOfModulesInSeries(double numberOfModulesInSeries);

      void resetNumberOfModulesInSeries();

      bool setRatedElectricPowerOutput(double ratedElectricPowerOutput);

      void resetRatedElectricPowerOutput();

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorPhotovoltaic");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORPHOTOVOLTAIC_IMPL_HPP
