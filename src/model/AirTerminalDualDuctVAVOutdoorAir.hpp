/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP
#define MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Node;
  class DesignSpecificationOutdoorAir;

  namespace detail {

    class AirTerminalDualDuctVAVOutdoorAir_Impl;

  }  // namespace detail

  /** AirTerminalDualDuctVAVOutdoorAir is a Mixer that wraps the OpenStudio IDD object 'OS:AirTerminal:DualDuct:VAV:OutdoorAir'. */
  class MODEL_API AirTerminalDualDuctVAVOutdoorAir : public Mixer
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalDualDuctVAVOutdoorAir(const Model& model);

    virtual ~AirTerminalDualDuctVAVOutdoorAir() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalDualDuctVAVOutdoorAir(const AirTerminalDualDuctVAVOutdoorAir& other) = default;
    AirTerminalDualDuctVAVOutdoorAir(AirTerminalDualDuctVAVOutdoorAir&& other) = default;
    AirTerminalDualDuctVAVOutdoorAir& operator=(const AirTerminalDualDuctVAVOutdoorAir&) = default;
    AirTerminalDualDuctVAVOutdoorAir& operator=(AirTerminalDualDuctVAVOutdoorAir&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> perPersonVentilationRateModeValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> maximumTerminalAirFlowRate() const;

    bool isMaximumTerminalAirFlowRateAutosized() const;

    std::string perPersonVentilationRateMode() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setMaximumTerminalAirFlowRate(double maximumTerminalAirFlowRate);

    void autosizeMaximumTerminalAirFlowRate();

    /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
    bool controlForOutdoorAir() const;
    bool setControlForOutdoorAir(bool controlForOutdoorAir);

    bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);

    /** This corresponds to Mixer::inletModelObject(0), as well as the system's branch 0
    * (ie. AirLoopHVAC::demandInletNodes()[0] and AirLoopHVAC::supplyOutletNodes()[0])
    * In OpenStudio we avoid using hot / cold nomenclature in the dual duct system api,
    * but here in the terminal interface we use the language for transparency with the idf.
    */
    boost::optional<Node> outdoorAirInletNode() const;

    /** This corresponds to Mixer::inletModelObject(1), as well as the system's branch 1
    * (ie. AirLoopHVAC::demandInletNodes()[1] and AirLoopHVAC::supplyOutletNodes()[1])
    */
    boost::optional<Node> recirculatedAirInletNode() const;

    //@}
    /** @name Other */
    //@{

    /* Queries the autosized maximum terminal air flow rate from the SQL file */
    boost::optional<double> autosizedMaximumTerminalAirFlowRate() const;

    /* Autosize all autosizeable fields */
    // void autosize();

    /* Hard applies the autosized values from SQL file */
    // void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalDualDuctVAVOutdoorAir_Impl;

    explicit AirTerminalDualDuctVAVOutdoorAir(std::shared_ptr<detail::AirTerminalDualDuctVAVOutdoorAir_Impl> impl);

    friend class detail::AirTerminalDualDuctVAVOutdoorAir_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalDualDuctVAVOutdoorAir");
  };

  /** \relates AirTerminalDualDuctVAVOutdoorAir*/
  using OptionalAirTerminalDualDuctVAVOutdoorAir = boost::optional<AirTerminalDualDuctVAVOutdoorAir>;

  /** \relates AirTerminalDualDuctVAVOutdoorAir*/
  using AirTerminalDualDuctVAVOutdoorAirVector = std::vector<AirTerminalDualDuctVAVOutdoorAir>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALDUALDUCTVAVOUTDOORAIR_HPP
