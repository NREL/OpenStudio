/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEFUNCTIONALPRESSUREDROP_IMPL_HPP
#define MODEL_CURVEFUNCTIONALPRESSUREDROP_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveFunctionalPressureDrop_Impl is a Curve_Impl that is the implementation class for CurveFunctionalPressureDrop.*/
    class MODEL_API CurveFunctionalPressureDrop_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveFunctionalPressureDrop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveFunctionalPressureDrop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveFunctionalPressureDrop_Impl(const CurveFunctionalPressureDrop_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveFunctionalPressureDrop_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual int numVariables() const override;

      virtual double evaluate(const std::vector<double>& independentVariables) const override;

      //@}
      /** @name Getters */
      //@{

      double diameter() const;

      boost::optional<double> minorLossCoefficient() const;

      boost::optional<double> length() const;

      boost::optional<double> roughness() const;

      boost::optional<double> fixedFrictionFactor() const;

      //@}
      /** @name Setters */
      //@{

      bool setDiameter(double diameter);

      bool setMinorLossCoefficient(boost::optional<double> minorLossCoefficient);

      void resetMinorLossCoefficient();

      bool setLength(boost::optional<double> length);

      void resetLength();

      bool setRoughness(boost::optional<double> roughness);

      void resetRoughness();

      bool setFixedFrictionFactor(boost::optional<double> fixedFrictionFactor);

      void resetFixedFrictionFactor();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CurveFunctionalPressureDrop");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEFUNCTIONALPRESSUREDROP_IMPL_HPP
