/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CENTRALHEATPUMPSYSTEMMODULE_HPP
#define MODEL_CENTRALHEATPUMPSYSTEMMODULE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  class ChillerHeaterPerformanceElectricEIR;
  class CentralHeatPumpSystem;
  class Schedule;

  namespace detail {

    class CentralHeatPumpSystemModule_Impl;

  }  // namespace detail

  /** CentralHeatPumpSystemModule is a ParentObject that wraps the OpenStudio IDD object 'OS:CentralHeatPumpSystem:Module'. */
  class MODEL_API CentralHeatPumpSystemModule : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CentralHeatPumpSystemModule(const Model& model);

    virtual ~CentralHeatPumpSystemModule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CentralHeatPumpSystemModule(const CentralHeatPumpSystemModule& other) = default;
    CentralHeatPumpSystemModule(CentralHeatPumpSystemModule&& other) = default;
    CentralHeatPumpSystemModule& operator=(const CentralHeatPumpSystemModule&) = default;
    CentralHeatPumpSystemModule& operator=(CentralHeatPumpSystemModule&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ChillerHeaterPerformanceElectricEIR chillerHeaterModulesPerformanceComponent() const;

    Schedule chillerHeaterModulesControlSchedule() const;

    int numberofChillerHeaterModules() const;

    //@}
    /** @name Setters */
    //@{

    bool setChillerHeaterModulesPerformanceComponent(const ChillerHeaterPerformanceElectricEIR& chillerHeaterPerformanceElectricEIR);

    bool setChillerHeaterModulesControlSchedule(Schedule& schedule);

    bool setNumberofChillerHeaterModules(int numberofChillerHeaterModules);

    //@}
    /** @name Other */
    //@{

    // Convenience function to return parent CentralHeatPumpSystem
    boost::optional<CentralHeatPumpSystem> centralHeatPumpSystem() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CentralHeatPumpSystemModule_Impl;

    explicit CentralHeatPumpSystemModule(std::shared_ptr<detail::CentralHeatPumpSystemModule_Impl> impl);

    friend class detail::CentralHeatPumpSystemModule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CentralHeatPumpSystemModule");
  };

  /** \relates CentralHeatPumpSystemModule*/
  using OptionalCentralHeatPumpSystemModule = boost::optional<CentralHeatPumpSystemModule>;

  /** \relates CentralHeatPumpSystemModule*/
  using CentralHeatPumpSystemModuleVector = std::vector<CentralHeatPumpSystemModule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CENTRALHEATPUMPSYSTEMMODULE_HPP
