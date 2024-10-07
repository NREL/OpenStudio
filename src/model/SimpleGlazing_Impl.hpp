/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIMPLEGLAZING_IMPL_HPP
#define MODEL_SIMPLEGLAZING_IMPL_HPP

#include "ModelAPI.hpp"
#include "Glazing_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SimpleGlazing_Impl is a Glazing_Impl that is the implementation class for SimpleGlazing.*/
    class MODEL_API SimpleGlazing_Impl : public Glazing_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SimpleGlazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SimpleGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SimpleGlazing_Impl(const SimpleGlazing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SimpleGlazing_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<double> getVisibleTransmittance() const override;

      //@}
      /** @name Getters */
      //@{

      double uFactor() const;

      double solarHeatGainCoefficient() const;

      boost::optional<double> visibleTransmittance() const;

      //@}
      /** @name Setters */
      //@{

      bool setUFactor(double uFactor);

      bool setSolarHeatGainCoefficient(double solarHeatGainCoefficient);

      bool setVisibleTransmittance(boost::optional<double> visibleTransmittance);

      void resetVisibleTransmittance();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SimpleGlazing");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIMPLEGLAZING_IMPL_HPP
