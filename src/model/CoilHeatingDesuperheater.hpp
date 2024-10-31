/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDESUPERHEATER_HPP
#define MODEL_COILHEATINGDESUPERHEATER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ModelObject;
  class AirflowNetworkEquivalentDuct;

  namespace detail {

    class CoilHeatingDesuperheater_Impl;

  }  // namespace detail

  /** CoilHeatingDesuperheater is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Desuperheater'.

  This coil can be used in air loop simulations but can’t be used by certain compound objects
  (e.g., AirLoopHVACUnitaryHeatPumpAirToAir, AirLoopHVACUnitaryHeatPumpWaterToAir, or DehumidifierDesiccantNoFans)
  or any air distribution equipment (e.g., AirTerminalSingleDuctConstantVolumeReheat, AirTerminalSingleDuctVAVReheat,
  or AirTerminalSingleDuctSeriesPIUReheat).
*/
  class MODEL_API CoilHeatingDesuperheater : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDesuperheater(const Model& model);

    virtual ~CoilHeatingDesuperheater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDesuperheater(const CoilHeatingDesuperheater& other) = default;
    CoilHeatingDesuperheater(CoilHeatingDesuperheater&& other) = default;
    CoilHeatingDesuperheater& operator=(const CoilHeatingDesuperheater&) = default;
    CoilHeatingDesuperheater& operator=(CoilHeatingDesuperheater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double heatReclaimRecoveryEfficiency() const;

    bool isHeatReclaimRecoveryEfficiencyDefaulted() const;

    boost::optional<ModelObject> heatingSource() const;

    OS_DEPRECATED(3, 7, 0) double parasiticElectricLoad() const;

    double onCycleParasiticElectricLoad() const;

    OS_DEPRECATED(3, 7, 0) bool isParasiticElectricLoadDefaulted() const;

    bool isOnCycleParasiticElectricLoadDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setHeatReclaimRecoveryEfficiency(double heatReclaimRecoveryEfficiency);

    void resetHeatReclaimRecoveryEfficiency();

    /** \fn bool setHeatingSource(const ModelObject& heatingSource)
      \brief Sets the heating source from which the desuperheater heating coil recovers energy.
      \param[in] heatingSource The heating source.

      Valid inputs: \n
      \ref CoilCoolingDXSingleSpeed \n
      \ref CoilCoolingDXTwoSpeed \n
      \ref RefrigerationCompressorRack \n
      \ref RefrigerationCondenserAirCooled \n
      \ref RefrigerationCondenserEvaporativeCooled \n
      \ref RefrigerationCondenserWaterCooled \n
  */
    bool setHeatingSource(const ModelObject& modelObject);

    void resetHeatingSource();

    OS_DEPRECATED(3, 7, 0) bool setParasiticElectricLoad(double parasiticElectricLoad);

    bool setOnCycleParasiticElectricLoad(double onCyclearasiticElectricLoad);

    OS_DEPRECATED(3, 7, 0) void resetParasiticElectricLoad();

    void resetOnCycleParasiticElectricLoad();

    //@}
    /** @name Other */
    //@{

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDesuperheater_Impl;

    explicit CoilHeatingDesuperheater(std::shared_ptr<detail::CoilHeatingDesuperheater_Impl> impl);

    friend class detail::CoilHeatingDesuperheater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDesuperheater");
  };

  /** \relates CoilHeatingDesuperheater*/
  using OptionalCoilHeatingDesuperheater = boost::optional<CoilHeatingDesuperheater>;

  /** \relates CoilHeatingDesuperheater*/
  using CoilHeatingDesuperheaterVector = std::vector<CoilHeatingDesuperheater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDESUPERHEATER_HPP
