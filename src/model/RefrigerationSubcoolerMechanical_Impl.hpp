/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_IMPL_HPP
#define MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;

  namespace detail {

    /** RefrigerationSubcoolerMechanical_Impl is a ModelObject_Impl that is the implementation class for RefrigerationSubcoolerMechanical.*/
    class MODEL_API RefrigerationSubcoolerMechanical_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationSubcoolerMechanical_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationSubcoolerMechanical_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationSubcoolerMechanical_Impl(const RefrigerationSubcoolerMechanical_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationSubcoolerMechanical_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<RefrigerationSystem> capacityProvidingSystem() const;

      boost::optional<double> outletControlTemperature() const;

      //@}
      /** @name Setters */
      //@{

      bool setCapacityProvidingSystem(const boost::optional<RefrigerationSystem>& refrigerationSystem);

      void resetCapacityProvidingSystem();

      bool setOutletControlTemperature(boost::optional<double> outletControlTemperature);

      void resetOutletControlTemperature();

      //@}
      /** @name Other */
      //@{

      boost::optional<RefrigerationSystem> system() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationSubcoolerMechanical");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_IMPL_HPP
