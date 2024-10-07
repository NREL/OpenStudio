/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class EnergyManagementSystemGlobalVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemGlobalVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:GlobalVariable'. */
  class MODEL_API EnergyManagementSystemGlobalVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemGlobalVariable(const Model& model, const std::string& variableName);

    virtual ~EnergyManagementSystemGlobalVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemGlobalVariable(const EnergyManagementSystemGlobalVariable& other) = default;
    EnergyManagementSystemGlobalVariable(EnergyManagementSystemGlobalVariable&& other) = default;
    EnergyManagementSystemGlobalVariable& operator=(const EnergyManagementSystemGlobalVariable&) = default;
    EnergyManagementSystemGlobalVariable& operator=(EnergyManagementSystemGlobalVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    bool exportToBCVTB() const;

    bool isExportToBCVTBDefaulted() const;

    bool setExportToBCVTB(bool exportToBCVTB);

    void resetExportToBCVTB();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemGlobalVariable_Impl;

    explicit EnergyManagementSystemGlobalVariable(std::shared_ptr<detail::EnergyManagementSystemGlobalVariable_Impl> impl);

    friend class detail::EnergyManagementSystemGlobalVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemGlobalVariable");
  };

  /** \relates EnergyManagementSystemGlobalVariable*/
  using OptionalEnergyManagementSystemGlobalVariable = boost::optional<EnergyManagementSystemGlobalVariable>;

  /** \relates EnergyManagementSystemGlobalVariable*/
  using EnergyManagementSystemGlobalVariableVector = std::vector<EnergyManagementSystemGlobalVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMGLOBALVARIABLE_HPP
