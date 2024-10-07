/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** EnergyManagementSystemCurveOrTableIndexVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemCurveOrTableIndexVariable.*/
    class MODEL_API EnergyManagementSystemCurveOrTableIndexVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemCurveOrTableIndexVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemCurveOrTableIndexVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemCurveOrTableIndexVariable_Impl(const EnergyManagementSystemCurveOrTableIndexVariable_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~EnergyManagementSystemCurveOrTableIndexVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemCurveOrTableIndexVariable");

      boost::optional<ModelObject> optionalCurveOrTableObject() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMCURVEORTABLEINDEXVARIABLE_IMPL_HPP
