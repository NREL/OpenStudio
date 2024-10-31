/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class EnergyManagementSystemProgram_Impl;

  }  // namespace detail

  /** EnergyManagementSystemProgram is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Program'. */
  class MODEL_API EnergyManagementSystemProgram : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemProgram(const Model& model);

    virtual ~EnergyManagementSystemProgram() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemProgram(const EnergyManagementSystemProgram& other) = default;
    EnergyManagementSystemProgram(EnergyManagementSystemProgram&& other) = default;
    EnergyManagementSystemProgram& operator=(const EnergyManagementSystemProgram&) = default;
    EnergyManagementSystemProgram& operator=(EnergyManagementSystemProgram&&) = default;

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
    using ImplType = detail::EnergyManagementSystemProgram_Impl;

    explicit EnergyManagementSystemProgram(std::shared_ptr<detail::EnergyManagementSystemProgram_Impl> impl);

    friend class detail::EnergyManagementSystemProgram_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgram");
  };

  /** \relates EnergyManagementSystemProgram*/
  using OptionalEnergyManagementSystemProgram = boost::optional<EnergyManagementSystemProgram>;

  /** \relates EnergyManagementSystemProgram*/
  using EnergyManagementSystemProgramVector = std::vector<EnergyManagementSystemProgram>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP
