/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "EnergyManagementSystemProgram.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class EnergyManagementSystemProgramCallingManager_Impl;

  }  // namespace detail

  /** EnergyManagementSystemProgramCallingManager is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:ProgramCallingManager'. */
  class MODEL_API EnergyManagementSystemProgramCallingManager : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemProgramCallingManager(const Model& model);

    virtual ~EnergyManagementSystemProgramCallingManager() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemProgramCallingManager(const EnergyManagementSystemProgramCallingManager& other) = default;
    EnergyManagementSystemProgramCallingManager(EnergyManagementSystemProgramCallingManager&& other) = default;
    EnergyManagementSystemProgramCallingManager& operator=(const EnergyManagementSystemProgramCallingManager&) = default;
    EnergyManagementSystemProgramCallingManager& operator=(EnergyManagementSystemProgramCallingManager&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validCallingPointValues();

    /** @name Getters */
    //@{

    std::string callingPoint() const;

    std::vector<EnergyManagementSystemProgram> programs() const;

    boost::optional<EnergyManagementSystemProgram> getProgram(unsigned index) const;

    //@}
    /** @name Setters */
    //@{

    bool setCallingPoint(const std::string& callingPoint);

    bool eraseProgram(unsigned index);

    void erasePrograms();

    bool addProgram(const EnergyManagementSystemProgram& program);

    bool setProgram(const EnergyManagementSystemProgram& program, unsigned index);

    bool setPrograms(const std::vector<EnergyManagementSystemProgram>& programs);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemProgramCallingManager_Impl;

    explicit EnergyManagementSystemProgramCallingManager(std::shared_ptr<detail::EnergyManagementSystemProgramCallingManager_Impl> impl);

    friend class detail::EnergyManagementSystemProgramCallingManager_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgramCallingManager");
  };

  /** \relates EnergyManagementSystemProgramCallingManager*/
  using OptionalEnergyManagementSystemProgramCallingManager = boost::optional<EnergyManagementSystemProgramCallingManager>;

  /** \relates EnergyManagementSystemProgramCallingManager*/
  using EnergyManagementSystemProgramCallingManagerVector = std::vector<EnergyManagementSystemProgramCallingManager>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_HPP
