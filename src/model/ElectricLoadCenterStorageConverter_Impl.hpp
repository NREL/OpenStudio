/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;  //UniVariateCurves;
  class ThermalZone;
  class ElectricLoadCenterDistribution;

  namespace detail {

    /** ElectricLoadCenterStorageConverter_Impl is a ParentObject_Impl that is the implementation class for ElectricLoadCenterStorageConverter.*/
    class MODEL_API ElectricLoadCenterStorageConverter_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricLoadCenterStorageConverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterStorageConverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterStorageConverter_Impl(const ElectricLoadCenterStorageConverter_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricLoadCenterStorageConverter_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

      Schedule availabilitySchedule() const;
      bool isAvailabilityScheduleDefaulted() const;

      std::string powerConversionEfficiencyMethod() const;

      // Required if powerConversionEfficiencyMethod == "SimpleFixed"
      boost::optional<double> simpleFixedEfficiency() const;

      // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
      boost::optional<double> designMaximumContinuousInputPower() const;

      // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
      // TODO: Later may need to add support for Tables. (UniVariateCurves, UniVariateTables.)
      boost::optional<Curve> efficiencyFunctionofPowerCurve() const;

      // Defaults to 0.0
      double ancillaryPowerConsumedInStandby() const;

      bool isAncillaryPowerConsumedInStandbyDefaulted() const;

      boost::optional<ThermalZone> thermalZone() const;

      // Defualts to 0.0, because E+ doc says that if omitted, all heat gains are conductive
      double radiativeFraction() const;

      bool isRadiativeFractionDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Schedule.
      // Note Schedules are passed by reference, not const reference.
      bool setAvailabilitySchedule(Schedule& schedule);
      void resetAvailabilitySchedule();

      // No Setter, will be done when calling one of the three following functions
      //bool setPowerConversionEfficiencyMethod(const std::string& powerConversionEfficiencyMethod);

      bool setSimpleFixedEfficiency(double simpleFixedEfficiency);

      bool setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower);

      bool setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve);

      bool setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby);

      void resetAncillaryPowerConsumedInStandby();

      bool setThermalZone(const ThermalZone& thermalZone);

      void resetThermalZone();

      bool setRadiativeFraction(double radiativeFraction);

      void resetRadiativeFraction();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageConverter");
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP
