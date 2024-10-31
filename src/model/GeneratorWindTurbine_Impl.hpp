/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORWINDTURBINE_IMPL_HPP
#define MODEL_GENERATORWINDTURBINE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** GeneratorWindTurbine_Impl is a Generator_Impl that is the implementation class for GeneratorWindTurbine.*/
    class MODEL_API GeneratorWindTurbine_Impl : public Generator_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorWindTurbine_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorWindTurbine_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorWindTurbine_Impl(const GeneratorWindTurbine_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorWindTurbine_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::string generatorObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      //@}
      /** @name Getters */
      //@{

      std::string rotorType() const;

      std::string powerControl() const;

      double ratedRotorSpeed() const;

      double rotorDiameter() const;

      double overallHeight() const;

      int numberofBlades() const;

      double ratedPower() const;

      double ratedWindSpeed() const;

      double cutInWindSpeed() const;

      double cutOutWindSpeed() const;

      double fractionSystemEfficiency() const;

      double maximumTipSpeedRatio() const;

      double maximumPowerCoefficient() const;

      boost::optional<double> annualLocalAverageWindSpeed() const;

      double heightforLocalAverageWindSpeed() const;

      double bladeChordArea() const;

      double bladeDragCoefficient() const;

      double bladeLiftCoefficient() const;

      double powerCoefficientC1() const;

      double powerCoefficientC2() const;

      double powerCoefficientC3() const;

      double powerCoefficientC4() const;

      double powerCoefficientC5() const;

      double powerCoefficientC6() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRotorType(const std::string& rotorType);

      bool setPowerControl(const std::string& powerControl);

      bool setRatedRotorSpeed(double ratedRotorSpeed);

      bool setRotorDiameter(double rotorDiameter);

      bool setOverallHeight(double overallHeight);

      bool setNumberofBlades(int numberofBlades);

      bool setRatedPower(double ratedPower);

      bool setRatedWindSpeed(double ratedWindSpeed);

      bool setCutInWindSpeed(double cutInWindSpeed);

      bool setCutOutWindSpeed(double cutOutWindSpeed);

      bool setFractionSystemEfficiency(double fractionSystemEfficiency);

      bool setMaximumTipSpeedRatio(double maximumTipSpeedRatio);

      bool setMaximumPowerCoefficient(double maximumPowerCoefficient);

      bool setAnnualLocalAverageWindSpeed(double annualLocalAverageWindSpeed);

      void resetAnnualLocalAverageWindSpeed();

      bool setHeightforLocalAverageWindSpeed(double heightforLocalAverageWindSpeed);

      bool setBladeChordArea(double bladeChordArea);

      bool setBladeDragCoefficient(double bladeDragCoefficient);

      bool setBladeLiftCoefficient(double bladeLiftCoefficient);

      bool setPowerCoefficientC1(double powerCoefficientC1);

      bool setPowerCoefficientC2(double powerCoefficientC2);

      bool setPowerCoefficientC3(double powerCoefficientC3);

      bool setPowerCoefficientC4(double powerCoefficientC4);

      bool setPowerCoefficientC5(double powerCoefficientC5);

      bool setPowerCoefficientC6(double powerCoefficientC6);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorWindTurbine");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORWINDTURBINE_IMPL_HPP
