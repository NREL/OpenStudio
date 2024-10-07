/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CENTRALHEATPUMPSYSTEMMODULE_IMPL_HPP
#define MODEL_CENTRALHEATPUMPSYSTEMMODULE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ChillerHeaterPerformanceElectricEIR;
  class CentralHeatPumpSystem;
  class Schedule;

  namespace detail {

    /** CentralHeatPumpSystemModule_Impl is a ParentObject_Impl that is the implementation class for CentralHeatPumpSystemModule.*/
    class MODEL_API CentralHeatPumpSystemModule_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CentralHeatPumpSystemModule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CentralHeatPumpSystemModule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CentralHeatPumpSystemModule_Impl(const CentralHeatPumpSystemModule_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CentralHeatPumpSystemModule_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      ChillerHeaterPerformanceElectricEIR chillerHeaterModulesPerformanceComponent() const;

      Schedule chillerHeaterModulesControlSchedule() const;

      int numberofChillerHeaterModules() const;

      //@}
      /** @name Setters */
      //@{

      bool setChillerHeaterModulesPerformanceComponent(const ChillerHeaterPerformanceElectricEIR& chillerHeaterPerformanceElectricEIR);

      bool setChillerHeaterModulesControlSchedule(Schedule& schedule);

      bool setNumberofChillerHeaterModules(int numberofChillerHeaterModules);

      //@}
      /** @name Other */
      //@{

      // Convenience function to return parent CentralHeatPumpSystem
      boost::optional<CentralHeatPumpSystem> centralHeatPumpSystem() const;

      // Override the clone method to clone the ChillerHeaterPerformanceElectricEIR as well
      ModelObject clone(Model model) const override;

      // Treat the ChillerHeaterPerformanceElectricEIR as a child
      std::vector<IddObjectType> allowableChildTypes() const override;
      std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystemModule");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<ChillerHeaterPerformanceElectricEIR> optionalChillerHeaterModulesPerformanceComponent() const;
      boost::optional<Schedule> optionalChillerHeaterModulesControlSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CENTRALHEATPUMPSYSTEMMODULE_IMPL_HPP
