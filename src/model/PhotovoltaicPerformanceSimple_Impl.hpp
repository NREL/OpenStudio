/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_IMPL_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PhotovoltaicPerformance_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** PhotovoltaicPerformanceSimple_Impl is a PhotovoltaicPerformance_Impl that is the implementation class for PhotovoltaicPerformanceSimple.*/
    class MODEL_API PhotovoltaicPerformanceSimple_Impl : public PhotovoltaicPerformance_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PhotovoltaicPerformanceSimple_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PhotovoltaicPerformanceSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PhotovoltaicPerformanceSimple_Impl(const PhotovoltaicPerformanceSimple_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PhotovoltaicPerformanceSimple_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      double fractionOfSurfaceAreaWithActiveSolarCells() const;

      bool isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted() const;

      std::string conversionEfficiencyInputMode() const;

      boost::optional<double> fixedEfficiency() const;

      boost::optional<Schedule> efficiencySchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setFractionOfSurfaceAreaWithActiveSolarCells(double fractionOfSurfaceAreaWithActiveSolarCells);

      void resetFractionOfSurfaceAreaWithActiveSolarCells();

      bool setConversionEfficiencyInputMode(const std::string& conversionEfficiencyInputMode);

      bool setFixedEfficiency(double fixedEfficiency);

      void resetFixedEfficiency();

      bool setEfficiencySchedule(Schedule& schedule);

      void resetEfficiencySchedule();

      bool setDefaultFixedEfficiency();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceSimple");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_IMPL_HPP
