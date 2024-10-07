/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilCoolingDXVariableSpeedSpeedData_Impl;

  }  // namespace detail

  /** CoilCoolingDXVariableSpeedSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableSpeed:SpeedData'. */
  class MODEL_API CoilCoolingDXVariableSpeedSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXVariableSpeedSpeedData(const Model& model);

    explicit CoilCoolingDXVariableSpeedSpeedData(const Model& model, const Curve& totalCoolingCapacityFunctionofTemperatureCurve,
                                                 const Curve& totalCoolingCapacityFunctionofAirFlowFractionCurve,
                                                 const Curve& energyInputRatioFunctionofTemperatureCurve,
                                                 const Curve& energyInputRatioFunctionofAirFlowFractionCurve);

    virtual ~CoilCoolingDXVariableSpeedSpeedData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXVariableSpeedSpeedData(const CoilCoolingDXVariableSpeedSpeedData& other) = default;
    CoilCoolingDXVariableSpeedSpeedData(CoilCoolingDXVariableSpeedSpeedData&& other) = default;
    CoilCoolingDXVariableSpeedSpeedData& operator=(const CoilCoolingDXVariableSpeedSpeedData&) = default;
    CoilCoolingDXVariableSpeedSpeedData& operator=(CoilCoolingDXVariableSpeedSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double referenceUnitGrossRatedTotalCoolingCapacity() const;

    double referenceUnitGrossRatedSensibleHeatRatio() const;

    double referenceUnitGrossRatedCoolingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate2017() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate2023() const;

    boost::optional<double> referenceUnitRatedCondenserAirFlowRate() const;

    boost::optional<double> referenceUnitRatedPadEffectivenessofEvapPrecooling() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofAirFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedTotalCoolingCapacity(double referenceUnitGrossRatedTotalCoolingCapacity);

    bool setReferenceUnitGrossRatedSensibleHeatRatio(double referenceUnitGrossRatedSensibleHeatRatio);

    bool setReferenceUnitGrossRatedCoolingCOP(double referenceUnitGrossRatedCoolingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedEvaporatorFanPowerPerVolumeFlowRate2017);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedEvaporatorFanPowerPerVolumeFlowRate2023);

    bool setReferenceUnitRatedCondenserAirFlowRate(double referenceUnitRatedCondenserAirFlowRate);

    void resetReferenceUnitRatedCondenserAirFlowRate();

    bool setReferenceUnitRatedPadEffectivenessofEvapPrecooling(double referenceUnitRatedPadEffectivenessofEvapPrecooling);

    void resetReferenceUnitRatedPadEffectivenessofEvapPrecooling();

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXVariableSpeedSpeedData_Impl;

    explicit CoilCoolingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilCoolingDXVariableSpeedSpeedData_Impl> impl);

    friend class detail::CoilCoolingDXVariableSpeedSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeedSpeedData");
  };

  /** \relates CoilCoolingDXVariableSpeedSpeedData*/
  using OptionalCoilCoolingDXVariableSpeedSpeedData = boost::optional<CoilCoolingDXVariableSpeedSpeedData>;

  /** \relates CoilCoolingDXVariableSpeedSpeedData*/
  using CoilCoolingDXVariableSpeedSpeedDataVector = std::vector<CoilCoolingDXVariableSpeedSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLESPEEDSPEEDDATA_HPP
