/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COOLINGTOWERPERFORMANCEYORKCALC_HPP
#define MODEL_COOLINGTOWERPERFORMANCEYORKCALC_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoolingTowerPerformanceYorkCalc_Impl;

  }  // namespace detail

  /** CoolingTowerPerformanceYorkCalc is a ModelObject that wraps the OpenStudio IDD object 'OS:CoolingTowerPerformance:YorkCalc'. */
  class MODEL_API CoolingTowerPerformanceYorkCalc : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoolingTowerPerformanceYorkCalc(const Model& model);

    virtual ~CoolingTowerPerformanceYorkCalc() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoolingTowerPerformanceYorkCalc(const CoolingTowerPerformanceYorkCalc& other) = default;
    CoolingTowerPerformanceYorkCalc(CoolingTowerPerformanceYorkCalc&& other) = default;
    CoolingTowerPerformanceYorkCalc& operator=(const CoolingTowerPerformanceYorkCalc&) = default;
    CoolingTowerPerformanceYorkCalc& operator=(CoolingTowerPerformanceYorkCalc&&) = default;

    //@}

    static IddObjectType iddObjectType();

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
    /// @cond
    using ImplType = detail::CoolingTowerPerformanceYorkCalc_Impl;

    explicit CoolingTowerPerformanceYorkCalc(std::shared_ptr<detail::CoolingTowerPerformanceYorkCalc_Impl> impl);

    friend class detail::CoolingTowerPerformanceYorkCalc_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoolingTowerPerformanceYorkCalc");
  };

  /** \relates CoolingTowerPerformanceYorkCalc*/
  using OptionalCoolingTowerPerformanceYorkCalc = boost::optional<CoolingTowerPerformanceYorkCalc>;

  /** \relates CoolingTowerPerformanceYorkCalc*/
  using CoolingTowerPerformanceYorkCalcVector = std::vector<CoolingTowerPerformanceYorkCalc>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COOLINGTOWERPERFORMANCEYORKCALC_HPP
