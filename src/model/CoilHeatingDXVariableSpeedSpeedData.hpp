/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_HPP
#define MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilHeatingDXVariableSpeedSpeedData_Impl;

  }  // namespace detail

  /** CoilHeatingDXVariableSpeedSpeedData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:VariableSpeed:SpeedData'. */
  class MODEL_API CoilHeatingDXVariableSpeedSpeedData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDXVariableSpeedSpeedData(const Model& model);

    explicit CoilHeatingDXVariableSpeedSpeedData(const Model& model, const Curve& heatingCapacityFunctionofTemperatureCurve,
                                                 const Curve& totalHeatingCapacityFunctionofAirFlowFractionCurve,
                                                 const Curve& energyInputRatioFunctionofTemperatureCurve,
                                                 const Curve& energyInputRatioFunctionofAirFlowFractionCurve);

    virtual ~CoilHeatingDXVariableSpeedSpeedData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDXVariableSpeedSpeedData(const CoilHeatingDXVariableSpeedSpeedData& other) = default;
    CoilHeatingDXVariableSpeedSpeedData(CoilHeatingDXVariableSpeedSpeedData&& other) = default;
    CoilHeatingDXVariableSpeedSpeedData& operator=(const CoilHeatingDXVariableSpeedSpeedData&) = default;
    CoilHeatingDXVariableSpeedSpeedData& operator=(CoilHeatingDXVariableSpeedSpeedData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double referenceUnitGrossRatedHeatingCapacity() const;

    double referenceUnitGrossRatedHeatingCOP() const;

    double referenceUnitRatedAirFlowRate() const;

    double ratedSupplyFanPowerPerVolumeFlowRate2017() const;

    double ratedSupplyFanPowerPerVolumeFlowRate2023() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

    bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

    bool setReferenceUnitRatedAirFlowRate(double referenceUnitRatedAirFlowRate);

    bool setRatedSupplyFanPowerPerVolumeFlowRate2017(double ratedSupplyFanPowerPerVolumeFlowRate2017);

    bool setRatedSupplyFanPowerPerVolumeFlowRate2023(double ratedSupplyFanPowerPerVolumeFlowRate2023);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDXVariableSpeedSpeedData_Impl;

    explicit CoilHeatingDXVariableSpeedSpeedData(std::shared_ptr<detail::CoilHeatingDXVariableSpeedSpeedData_Impl> impl);

    friend class detail::CoilHeatingDXVariableSpeedSpeedData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableSpeedSpeedData");
  };

  /** \relates CoilHeatingDXVariableSpeedSpeedData*/
  using OptionalCoilHeatingDXVariableSpeedSpeedData = boost::optional<CoilHeatingDXVariableSpeedSpeedData>;

  /** \relates CoilHeatingDXVariableSpeedSpeedData*/
  using CoilHeatingDXVariableSpeedSpeedDataVector = std::vector<CoilHeatingDXVariableSpeedSpeedData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLESPEEDSPEEDDATA_HPP
