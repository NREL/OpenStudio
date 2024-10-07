/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTENERGYMANAGEMENTSYSTEM_HPP
#define MODEL_OUTPUTENERGYMANAGEMENTSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputEnergyManagementSystem_Impl;

  }  // namespace detail

  /** OutputEnergyManagementSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:EnergyManagementSystem'. */
  class MODEL_API OutputEnergyManagementSystem : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputEnergyManagementSystem() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputEnergyManagementSystem(const OutputEnergyManagementSystem& other) = default;
    OutputEnergyManagementSystem(OutputEnergyManagementSystem&& other) = default;
    OutputEnergyManagementSystem& operator=(const OutputEnergyManagementSystem&) = default;
    OutputEnergyManagementSystem& operator=(OutputEnergyManagementSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> actuatorAvailabilityDictionaryReportingValues();

    static std::vector<std::string> internalVariableAvailabilityDictionaryReportingValues();

    static std::vector<std::string> eMSRuntimeLanguageDebugOutputLevelValues();

    /** @name Getters */
    //@{

    std::string actuatorAvailabilityDictionaryReporting() const;

    bool isActuatorAvailabilityDictionaryReportingDefaulted() const;

    std::string internalVariableAvailabilityDictionaryReporting() const;

    bool isInternalVariableAvailabilityDictionaryReportingDefaulted() const;

    std::string eMSRuntimeLanguageDebugOutputLevel() const;

    bool isEMSRuntimeLanguageDebugOutputLevelDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setActuatorAvailabilityDictionaryReporting(const std::string& actuatorAvailabilityDictionaryReporting);

    void resetActuatorAvailabilityDictionaryReporting();

    bool setInternalVariableAvailabilityDictionaryReporting(const std::string& internalVariableAvailabilityDictionaryReporting);

    void resetInternalVariableAvailabilityDictionaryReporting();

    bool setEMSRuntimeLanguageDebugOutputLevel(const std::string& eMSRuntimeLanguageDebugOutputLevel);

    void resetEMSRuntimeLanguageDebugOutputLevel();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputEnergyManagementSystem_Impl;

    explicit OutputEnergyManagementSystem(std::shared_ptr<detail::OutputEnergyManagementSystem_Impl> impl);

    friend class detail::OutputEnergyManagementSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputEnergyManagementSystem(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputEnergyManagementSystem");
  };

  /** \relates OutputEnergyManagementSystem*/
  using OptionalOutputEnergyManagementSystem = boost::optional<OutputEnergyManagementSystem>;

  /** \relates OutputEnergyManagementSystem*/
  using OutputEnergyManagementSystemVector = std::vector<OutputEnergyManagementSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTENERGYMANAGEMENTSYSTEM_HPP
