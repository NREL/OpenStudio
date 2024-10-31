/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DUCT_IMPL_HPP
#define MODEL_DUCT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** Duct_Impl is a StraightComponent_Impl that is the implementation class for Duct.*/
    class MODEL_API Duct_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      Duct_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Duct_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Duct_Impl(const Duct_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Duct_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.Duct");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DUCT_IMPL_HPP
