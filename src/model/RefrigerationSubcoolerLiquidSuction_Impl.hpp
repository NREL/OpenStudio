/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_IMPL_HPP
#define MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;

  namespace detail {

    /** RefrigerationSubcoolerLiquidSuction_Impl is a ModelObject_Impl that is the implementation class for RefrigerationSubcoolerLiquidSuction.*/
    class MODEL_API RefrigerationSubcoolerLiquidSuction_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationSubcoolerLiquidSuction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationSubcoolerLiquidSuction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationSubcoolerLiquidSuction_Impl(const RefrigerationSubcoolerLiquidSuction_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationSubcoolerLiquidSuction_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> liquidSuctionDesignSubcoolingTemperatureDifference() const;

      boost::optional<double> designLiquidInletTemperature() const;

      boost::optional<double> designVaporInletTemperature() const;

      //@}
      /** @name Setters */
      //@{

      bool setLiquidSuctionDesignSubcoolingTemperatureDifference(boost::optional<double> liquidSuctionDesignSubcoolingTemperatureDifference);

      void resetLiquidSuctionDesignSubcoolingTemperatureDifference();

      bool setDesignLiquidInletTemperature(boost::optional<double> designLiquidInletTemperature);

      void resetDesignLiquidInletTemperature();

      bool setDesignVaporInletTemperature(boost::optional<double> designVaporInletTemperature);

      void resetDesignVaporInletTemperature();

      //@}
      /** @name Other */
      //@{

      boost::optional<RefrigerationSystem> system() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationSubcoolerLiquidSuction");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_IMPL_HPP
