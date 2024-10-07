/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_HPP
#define MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class AirTerminalSingleDuctInletSideMixer_Impl;

  }  // namespace detail

  /** AirTerminalSingleDuctInletSideMixer is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:InletSideMixer'.
 *  As of EnergyPlus version 8.6 this object maps to AirTerminal:SingleDuct:Mixer. **/
  class MODEL_API AirTerminalSingleDuctInletSideMixer : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirTerminalSingleDuctInletSideMixer(const Model& model);

    virtual ~AirTerminalSingleDuctInletSideMixer() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirTerminalSingleDuctInletSideMixer(const AirTerminalSingleDuctInletSideMixer& other) = default;
    AirTerminalSingleDuctInletSideMixer(AirTerminalSingleDuctInletSideMixer&& other) = default;
    AirTerminalSingleDuctInletSideMixer& operator=(const AirTerminalSingleDuctInletSideMixer&) = default;
    AirTerminalSingleDuctInletSideMixer& operator=(AirTerminalSingleDuctInletSideMixer&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> perPersonVentilationRateModeValues();
    /** @name Getters */
    //@{

    /** If true, OpenStudio will attach the DesignSpecificationOutdoorAir object associated
    * with the terminal's zone on export to EnergyPlus idf format.
    * This field replaces the functionality of the EnergyPlus field: Design Specification Outdoor Air Object Name.*/
    bool controlForOutdoorAir() const;

    std::string perPersonVentilationRateMode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlForOutdoorAir(bool controlForOutdoorAir);

    bool setPerPersonVentilationRateMode(const std::string& perPersonVentilationRateMode);
    //@}
    /** @name Other */
    //@{

    unsigned secondaryAirInletPort() const;

    boost::optional<Node> secondaryAirInletNode() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirTerminalSingleDuctInletSideMixer_Impl;

    explicit AirTerminalSingleDuctInletSideMixer(std::shared_ptr<detail::AirTerminalSingleDuctInletSideMixer_Impl> impl);

    friend class detail::AirTerminalSingleDuctInletSideMixer_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctInletSideMixer");
  };

  /** \relates AirTerminalSingleDuctInletSideMixer*/
  using OptionalAirTerminalSingleDuctInletSideMixer = boost::optional<AirTerminalSingleDuctInletSideMixer>;

  /** \relates AirTerminalSingleDuctInletSideMixer*/
  using AirTerminalSingleDuctInletSideMixerVector = std::vector<AirTerminalSingleDuctInletSideMixer>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTINLETSIDEMIXER_HPP
