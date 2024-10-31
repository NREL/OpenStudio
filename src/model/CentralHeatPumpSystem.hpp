/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CENTRALHEATPUMPSYSTEM_HPP
#define MODEL_CENTRALHEATPUMPSYSTEM_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ModelObjectList;
  class CentralHeatPumpSystemModule;

  namespace detail {

    class CentralHeatPumpSystem_Impl;

  }  // namespace detail

  /** CentralHeatPumpSystem is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:CentralHeatPumpSystem'. */
  class MODEL_API CentralHeatPumpSystem : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /* This constructor adds a chiller heater module list to store your CentralHeatPumpSystemModules, but it doesn't add a CentralHeatPumpSystemModule
   * At least one CentralHeatPumpSystemModule is needed for this object to work correctly (and be translated) to EnergyPlus.
   */
    explicit CentralHeatPumpSystem(const Model& model);

    virtual ~CentralHeatPumpSystem() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CentralHeatPumpSystem(const CentralHeatPumpSystem& other) = default;
    CentralHeatPumpSystem(CentralHeatPumpSystem&& other) = default;
    CentralHeatPumpSystem& operator=(const CentralHeatPumpSystem&) = default;
    CentralHeatPumpSystem& operator=(CentralHeatPumpSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlMethodValues();

    /** @name Getters */
    //@{

    std::string controlMethod() const;

    double ancillaryPower() const;

    boost::optional<Schedule> ancillaryOperationSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlMethod(const std::string& controlMethod);

    bool setAncillaryPower(double ancillaryPower);

    bool setAncillaryOperationSchedule(Schedule& schedule);

    void resetAncillaryOperationSchedule();

    //@}
    /** @name Other */
    //@{

    bool addModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule);

    void removeModule(const CentralHeatPumpSystemModule& centralHeatPumpSystemModule);

    void removeAllModules();

    std::vector<CentralHeatPumpSystemModule> modules() const;

    /** Convenience Function to return the Cooling Loop **/
    boost::optional<PlantLoop> coolingPlantLoop() const;

    /** Convenience Function to return the Source Loop **/
    boost::optional<PlantLoop> sourcePlantLoop() const;

    /** Convenience Function to return the Heating Loop **/
    boost::optional<PlantLoop> heatingPlantLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CentralHeatPumpSystem_Impl;

    explicit CentralHeatPumpSystem(std::shared_ptr<detail::CentralHeatPumpSystem_Impl> impl);

    friend class detail::CentralHeatPumpSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystem");
  };

  /** \relates CentralHeatPumpSystem*/
  using OptionalCentralHeatPumpSystem = boost::optional<CentralHeatPumpSystem>;

  /** \relates CentralHeatPumpSystem*/
  using CentralHeatPumpSystemVector = std::vector<CentralHeatPumpSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CENTRALHEATPUMPSYSTEM_HPP
