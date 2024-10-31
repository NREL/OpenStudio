/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COOLINGTOWERPERFORMANCEYORKCALC_IMPL_HPP
#define MODEL_COOLINGTOWERPERFORMANCEYORKCALC_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoolingTowerPerformanceYorkCalc_Impl is a ModelObject_Impl that is the implementation class for CoolingTowerPerformanceYorkCalc.*/
    class MODEL_API CoolingTowerPerformanceYorkCalc_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CoolingTowerPerformanceYorkCalc_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoolingTowerPerformanceYorkCalc_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoolingTowerPerformanceYorkCalc_Impl(const CoolingTowerPerformanceYorkCalc_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoolingTowerPerformanceYorkCalc_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double minimumInletAirWetBulbTemperature() const;

      double maximumInletAirWetBulbTemperature() const;

      double minimumRangeTemperature() const;

      double maximumRangeTemperature() const;

      double minimumApproachTemperature() const;

      double maximumApproachTemperature() const;

      double minimumWaterFlowRateRatio() const;

      double maximumWaterFlowRateRatio() const;

      double maximumLiquidtoGasRatio() const;

      double coefficient1() const;

      double coefficient2() const;

      double coefficient3() const;

      double coefficient4() const;

      double coefficient5() const;

      double coefficient6() const;

      double coefficient7() const;

      double coefficient8() const;

      double coefficient9() const;

      double coefficient10() const;

      double coefficient11() const;

      double coefficient12() const;

      double coefficient13() const;

      double coefficient14() const;

      double coefficient15() const;

      double coefficient16() const;

      double coefficient17() const;

      double coefficient18() const;

      double coefficient19() const;

      double coefficient20() const;

      double coefficient21() const;

      double coefficient22() const;

      double coefficient23() const;

      double coefficient24() const;

      double coefficient25() const;

      double coefficient26() const;

      double coefficient27() const;

      //@}
      /** @name Setters */
      //@{

      bool setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature);

      bool setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature);

      bool setMinimumRangeTemperature(double minimumRangeTemperature);

      bool setMaximumRangeTemperature(double maximumRangeTemperature);

      bool setMinimumApproachTemperature(double minimumApproachTemperature);

      bool setMaximumApproachTemperature(double maximumApproachTemperature);

      bool setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio);

      bool setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio);

      bool setMaximumLiquidtoGasRatio(double maximumLiquidtoGasRatio);

      bool setCoefficient1(double coefficient1);

      bool setCoefficient2(double coefficient2);

      bool setCoefficient3(double coefficient3);

      bool setCoefficient4(double coefficient4);

      bool setCoefficient5(double coefficient5);

      bool setCoefficient6(double coefficient6);

      bool setCoefficient7(double coefficient7);

      bool setCoefficient8(double coefficient8);

      bool setCoefficient9(double coefficient9);

      bool setCoefficient10(double coefficient10);

      bool setCoefficient11(double coefficient11);

      bool setCoefficient12(double coefficient12);

      bool setCoefficient13(double coefficient13);

      bool setCoefficient14(double coefficient14);

      bool setCoefficient15(double coefficient15);

      bool setCoefficient16(double coefficient16);

      bool setCoefficient17(double coefficient17);

      bool setCoefficient18(double coefficient18);

      bool setCoefficient19(double coefficient19);

      bool setCoefficient20(double coefficient20);

      bool setCoefficient21(double coefficient21);

      bool setCoefficient22(double coefficient22);

      bool setCoefficient23(double coefficient23);

      bool setCoefficient24(double coefficient24);

      bool setCoefficient25(double coefficient25);

      bool setCoefficient26(double coefficient26);

      bool setCoefficient27(double coefficient27);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoolingTowerPerformanceYorkCalc");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COOLINGTOWERPERFORMANCEYORKCALC_IMPL_HPP
