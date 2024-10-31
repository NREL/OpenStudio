/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    class ZoneHVACLowTempRadiantVarFlow_Impl;

  }  // namespace detail

  /** ZoneHVACLowTempRadiantVarFlow is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:LowTemperatureRadiant:VariableFlow'. */
  class MODEL_API ZoneHVACLowTempRadiantVarFlow : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ZoneHVACLowTempRadiantVarFlow(const Model& model, Schedule& availabilitySchedule, HVACComponent& heatingCoil,
                                           HVACComponent& coolingCoil);

    // This constructor defaults the availabilitySchedule to alwaysOnDiscrete, and **does not set any heating nor cooling coil**
    explicit ZoneHVACLowTempRadiantVarFlow(const Model& model);

    virtual ~ZoneHVACLowTempRadiantVarFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneHVACLowTempRadiantVarFlow(const ZoneHVACLowTempRadiantVarFlow& other) = default;
    ZoneHVACLowTempRadiantVarFlow(ZoneHVACLowTempRadiantVarFlow&& other) = default;
    ZoneHVACLowTempRadiantVarFlow& operator=(const ZoneHVACLowTempRadiantVarFlow&) = default;
    ZoneHVACLowTempRadiantVarFlow& operator=(ZoneHVACLowTempRadiantVarFlow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> radiantSurfaceTypeValues();

    static std::vector<std::string> temperatureControlTypeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<HVACComponent> heatingCoil() const;

    boost::optional<HVACComponent> coolingCoil() const;

    boost::optional<std::string> radiantSurfaceType() const;

    std::vector<Surface> surfaces() const;

    std::string fluidtoRadiantSurfaceHeatTransferModel() const;

    bool isFluidtoRadiantSurfaceHeatTransferModelDefaulted() const;

    double hydronicTubingInsideDiameter() const;

    bool isHydronicTubingInsideDiameterDefaulted() const;

    double hydronicTubingOutsideDiameter() const;

    bool isHydronicTubingOutsideDiameterDefaulted() const;

    boost::optional<double> hydronicTubingLength() const;

    bool isHydronicTubingLengthDefaulted() const;

    bool isHydronicTubingLengthAutosized() const;

    double hydronicTubingConductivity() const;

    bool isHydronicTubingConductivityDefaulted() const;

    std::string temperatureControlType() const;

    bool isTemperatureControlTypeDefaulted() const;

    std::string setpointControlType() const;

    bool isSetpointControlTypeDefaulted() const;

    std::string numberofCircuits() const;

    double circuitLength() const;

    boost::optional<Schedule> changeoverDelayTimePeriodSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRadiantSurfaceType(const std::string& radiantSurfaceType);

    void resetRadiantSurfaceType();

    bool setFluidtoRadiantSurfaceHeatTransferModel(const std::string& fluidtoRadiantSurfaceHeatTransferModel);

    void resetFluidtoRadiantSurfaceHeatTransferModel();

    bool setHeatingCoil(HVACComponent& heatingCoil);
    void resetHeatingCoil();

    bool setCoolingCoil(HVACComponent& coolingCoil);
    void resetCoolingCoil();

    bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

    void resetHydronicTubingInsideDiameter();

    bool setHydronicTubingOutsideDiameter(double hydronicTubingOutsideDiameter);

    void resetHydronicTubingOutsideDiameter();

    bool setHydronicTubingLength(double hydronicTubingLength);

    void resetHydronicTubingLength();

    void autosizeHydronicTubingLength();

    bool setHydronicTubingConductivity(double hydronicTubingConductivity);

    void resetHydronicTubingConductivity();

    bool setTemperatureControlType(const std::string& temperatureControlType);

    void resetTemperatureControlType();

    bool setSetpointControlType(const std::string& setpointControlType);

    void resetSetpointControlType();

    bool setNumberofCircuits(const std::string& numberofCircuits);

    bool setCircuitLength(double circuitLength);

    bool setChangeoverDelayTimePeriodSchedule(Schedule& schedule);

    void resetChangeoverDelayTimePeriodSchedule();

    boost::optional<ThermalZone> thermalZone() const override;

    bool addToThermalZone(ThermalZone& thermalZone);

    void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHydronicTubingLength() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneHVACLowTempRadiantVarFlow_Impl;

    explicit ZoneHVACLowTempRadiantVarFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantVarFlow_Impl> impl);

    friend class detail::ZoneHVACLowTempRadiantVarFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantVarFlow");
  };

  /** \relates ZoneHVACLowTempRadiantVarFlow*/
  using OptionalZoneHVACLowTempRadiantVarFlow = boost::optional<ZoneHVACLowTempRadiantVarFlow>;

  /** \relates ZoneHVACLowTempRadiantVarFlow*/
  using ZoneHVACLowTempRadiantVarFlowVector = std::vector<ZoneHVACLowTempRadiantVarFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_HPP
