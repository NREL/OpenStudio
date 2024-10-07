/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMSUBROUTINE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMSUBROUTINE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class EnergyManagementSystemSubroutine_Impl;

  }  // namespace detail

  /** EnergyManagementSystemSubroutine is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Subroutine'. */
  class MODEL_API EnergyManagementSystemSubroutine : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemSubroutine(const Model& model);

    virtual ~EnergyManagementSystemSubroutine() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemSubroutine(const EnergyManagementSystemSubroutine& other) = default;
    EnergyManagementSystemSubroutine(EnergyManagementSystemSubroutine&& other) = default;
    EnergyManagementSystemSubroutine& operator=(const EnergyManagementSystemSubroutine&) = default;
    EnergyManagementSystemSubroutine& operator=(EnergyManagementSystemSubroutine&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string body() const;

    std::vector<std::string> lines() const;

    std::vector<ModelObject> referencedObjects() const;

    std::vector<std::string> invalidReferencedObjects() const;

    //@}
    /** @name Setters */
    //@{

    bool setBody(const std::string& body);

    bool resetBody();

    bool addLine(const std::string& body);

    bool setLines(const std::vector<std::string>& body);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemSubroutine_Impl;

    explicit EnergyManagementSystemSubroutine(std::shared_ptr<detail::EnergyManagementSystemSubroutine_Impl> impl);

    friend class detail::EnergyManagementSystemSubroutine_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemSubroutine");
  };

  /** \relates EnergyManagementSystemSubroutine*/
  using OptionalEnergyManagementSystemSubroutine = boost::optional<EnergyManagementSystemSubroutine>;

  /** \relates EnergyManagementSystemSubroutine*/
  using EnergyManagementSystemSubroutineVector = std::vector<EnergyManagementSystemSubroutine>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMSUBROUTINE_HPP
