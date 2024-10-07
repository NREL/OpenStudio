/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    class EnergyManagementSystemCurveOrTableIndexVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemCurveOrTableIndexVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:CurveOrTableIndexVariable'. */
  class MODEL_API EnergyManagementSystemCurveOrTableIndexVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemCurveOrTableIndexVariable(const Model& model, const Curve& curve);

    explicit EnergyManagementSystemCurveOrTableIndexVariable(const Model& model);

    virtual ~EnergyManagementSystemCurveOrTableIndexVariable() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemCurveOrTableIndexVariable(const EnergyManagementSystemCurveOrTableIndexVariable& other) = default;
    EnergyManagementSystemCurveOrTableIndexVariable(EnergyManagementSystemCurveOrTableIndexVariable&& other) = default;
    EnergyManagementSystemCurveOrTableIndexVariable& operator=(const EnergyManagementSystemCurveOrTableIndexVariable&) = default;
    EnergyManagementSystemCurveOrTableIndexVariable& operator=(EnergyManagementSystemCurveOrTableIndexVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject curveOrTableObject() const;

    //@}
    /** @name Setters */
    //@{

    bool setCurveOrTableObject(const Curve& allCurves);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl;

    explicit EnergyManagementSystemCurveOrTableIndexVariable(std::shared_ptr<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl> impl);

    friend class detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemCurveOrTableIndexVariable");
  };

  /** \relates EnergyManagementSystemCurveOrTableIndexVariable*/
  using OptionalEnergyManagementSystemCurveOrTableIndexVariable = boost::optional<EnergyManagementSystemCurveOrTableIndexVariable>;

  /** \relates EnergyManagementSystemCurveOrTableIndexVariable*/
  using EnergyManagementSystemCurveOrTableIndexVariableVector = std::vector<EnergyManagementSystemCurveOrTableIndexVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_HPP
