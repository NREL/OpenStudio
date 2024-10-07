/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMALSTORAGEICEDETAILED_HPP
#define MODEL_THERMALSTORAGEICEDETAILED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    class ThermalStorageIceDetailed_Impl;

  }  // namespace detail

  /** ThermalStorageIceDetailed is a StraightComponent that wraps the OpenStudio IDD object 'OS:ThermalStorage:Ice:Detailed'. */
  class MODEL_API ThermalStorageIceDetailed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ThermalStorageIceDetailed(const Model& model);

    virtual ~ThermalStorageIceDetailed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ThermalStorageIceDetailed(const ThermalStorageIceDetailed& other) = default;
    ThermalStorageIceDetailed(ThermalStorageIceDetailed&& other) = default;
    ThermalStorageIceDetailed& operator=(const ThermalStorageIceDetailed&) = default;
    ThermalStorageIceDetailed& operator=(ThermalStorageIceDetailed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> thawProcessIndicatorValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double capacity() const;

    std::string dischargingCurveVariableSpecifications() const;

    Curve dischargingCurve() const;

    std::string chargingCurveVariableSpecifications() const;

    Curve chargingCurve() const;

    double timestepoftheCurveData() const;

    double parasiticElectricLoadDuringDischarging() const;

    double parasiticElectricLoadDuringCharging() const;

    double tankLossCoefficient() const;

    double freezingTemperatureofStorageMedium() const;

    std::string thawProcessIndicator() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCapacity(double capacity);

    bool setDischargingCurveVariableSpecifications(const std::string& dischargingCurveVariableSpecifications);

    bool setDischargingCurve(const Curve& dischargingCurve);

    bool setChargingCurveVariableSpecifications(const std::string& chargingCurveVariableSpecifications);

    bool setChargingCurve(const Curve& chargingCurve);

    bool setTimestepoftheCurveData(double timestepoftheCurveData);

    bool setParasiticElectricLoadDuringDischarging(double parasiticElectricLoadDuringDischarging);

    bool setParasiticElectricLoadDuringCharging(double parasiticElectricLoadDuringCharging);

    bool setTankLossCoefficient(double tankLossCoefficient);

    bool setFreezingTemperatureofStorageMedium(double freezingTemperatureofStorageMedium);

    bool setThawProcessIndicator(const std::string& thawProcessIndicator);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ThermalStorageIceDetailed_Impl;

    explicit ThermalStorageIceDetailed(std::shared_ptr<detail::ThermalStorageIceDetailed_Impl> impl);

    friend class detail::ThermalStorageIceDetailed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ThermalStorageIceDetailed");
  };

  /** \relates ThermalStorageIceDetailed*/
  using OptionalThermalStorageIceDetailed = boost::optional<ThermalStorageIceDetailed>;

  /** \relates ThermalStorageIceDetailed*/
  using ThermalStorageIceDetailedVector = std::vector<ThermalStorageIceDetailed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMALSTORAGEICEDETAILED_HPP
