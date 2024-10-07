/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

  namespace detail {

    class EnergyManagementSystemInternalVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemInternalVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:InternalVariable'. */
  class MODEL_API EnergyManagementSystemInternalVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    explicit EnergyManagementSystemInternalVariable(const Model& model, const std::string& internalDataType);

    virtual ~EnergyManagementSystemInternalVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemInternalVariable(const EnergyManagementSystemInternalVariable& other) = default;
    EnergyManagementSystemInternalVariable(EnergyManagementSystemInternalVariable&& other) = default;
    EnergyManagementSystemInternalVariable& operator=(const EnergyManagementSystemInternalVariable&) = default;
    EnergyManagementSystemInternalVariable& operator=(EnergyManagementSystemInternalVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string internalDataIndexKeyName() const;

    std::string internalDataType() const;

    //@}
    /** @name Setters */
    //@{

    bool setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName);

    void resetInternalDataIndexKeyName();

    bool setInternalDataType(const std::string& internalDataType);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemInternalVariable_Impl;

    explicit EnergyManagementSystemInternalVariable(std::shared_ptr<detail::EnergyManagementSystemInternalVariable_Impl> impl);

    friend class detail::EnergyManagementSystemInternalVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemInternalVariable");
    explicit EnergyManagementSystemInternalVariable(const Model& model);
  };

  /** \relates EnergyManagementSystemInternalVariable*/
  using OptionalEnergyManagementSystemInternalVariable = boost::optional<EnergyManagementSystemInternalVariable>;

  /** \relates EnergyManagementSystemInternalVariable*/
  using EnergyManagementSystemInternalVariableVector = std::vector<EnergyManagementSystemInternalVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP
