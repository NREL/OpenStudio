/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Construction;

  namespace detail {

    class EnergyManagementSystemConstructionIndexVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemConstructionIndexVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:ConstructionIndexVariable'. */
  class MODEL_API EnergyManagementSystemConstructionIndexVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemConstructionIndexVariable(const Model& model, const Construction& construction);

    explicit EnergyManagementSystemConstructionIndexVariable(const Model& model);

    virtual ~EnergyManagementSystemConstructionIndexVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemConstructionIndexVariable(const EnergyManagementSystemConstructionIndexVariable& other) = default;
    EnergyManagementSystemConstructionIndexVariable(EnergyManagementSystemConstructionIndexVariable&& other) = default;
    EnergyManagementSystemConstructionIndexVariable& operator=(const EnergyManagementSystemConstructionIndexVariable&) = default;
    EnergyManagementSystemConstructionIndexVariable& operator=(EnergyManagementSystemConstructionIndexVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject constructionObject() const;

    //@}
    /** @name Setters */
    //@{

    bool setConstructionObject(const ModelObject& construction);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemConstructionIndexVariable_Impl;

    explicit EnergyManagementSystemConstructionIndexVariable(std::shared_ptr<detail::EnergyManagementSystemConstructionIndexVariable_Impl> impl);

    friend class detail::EnergyManagementSystemConstructionIndexVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemConstructionIndexVariable");
  };

  /** \relates EnergyManagementSystemConstructionIndexVariable*/
  using OptionalEnergyManagementSystemConstructionIndexVariable = boost::optional<EnergyManagementSystemConstructionIndexVariable>;

  /** \relates EnergyManagementSystemConstructionIndexVariable*/
  using EnergyManagementSystemConstructionIndexVariableVector = std::vector<EnergyManagementSystemConstructionIndexVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_HPP
