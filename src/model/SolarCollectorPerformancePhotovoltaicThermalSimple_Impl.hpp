/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_IMPL_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;

  namespace detail {

    /** SolarCollectorPerformancePhotovoltaicThermalSimple_Impl is a ModelObject_Impl that is the implementation class for SolarCollectorPerformancePhotovoltaicThermalSimple.*/
    class MODEL_API SolarCollectorPerformancePhotovoltaicThermalSimple_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SolarCollectorPerformancePhotovoltaicThermalSimple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SolarCollectorPerformancePhotovoltaicThermalSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                              bool keepHandle);

      SolarCollectorPerformancePhotovoltaicThermalSimple_Impl(const SolarCollectorPerformancePhotovoltaicThermalSimple_Impl& other, Model_Impl* model,
                                                              bool keepHandle);

      virtual ~SolarCollectorPerformancePhotovoltaicThermalSimple_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      double fractionOfSurfaceAreaWithActiveThermalCollector() const;

      std::string thermalConversionEfficiencyInputModeType() const;

      boost::optional<double> thermalConversionEfficiency() const;

      boost::optional<Schedule> thermalConversionEfficiencySchedule() const;

      double frontSurfaceEmittance() const;

      bool isFrontSurfaceEmittanceDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setFractionOfSurfaceAreaWithActiveThermalCollector(double fractionofSurfaceAreawithActiveThermalCollector);

      bool setThermalConversionEfficiency(double fixedThermalConversionEfficiency);

      void resetThermalConversionEfficiency();

      bool setThermalConversionEfficiencySchedule(Schedule& schedule);

      void resetThermalConversionEfficiencySchedule();

      bool setFrontSurfaceEmittance(double frontSurfaceEmittance);

      void resetFrontSurfaceEmittance();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SolarCollectorPerformancePhotovoltaicThermalSimple");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEPHOTOVOLTAICTHERMALSIMPLE_IMPL_HPP
