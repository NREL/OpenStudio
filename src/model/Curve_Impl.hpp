/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVE_IMPL_HPP
#define MODEL_CURVE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** Curve_Impl is a ModelObject_Impl that is the implementation class for Curve. */
    class MODEL_API Curve_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // Construct completely new object.
      Curve_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      Curve_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      Curve_Impl(const Curve_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Curve_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Other */
      //@{

      virtual int numVariables() const = 0;

      virtual double evaluate(const std::vector<double>& x) const = 0;

      double evaluate(double x) const;

      double evaluate(double x, double y) const;

      double evaluate(double x, double y, double z) const;

      double evaluate(double w, double x, double y, double z) const;

      double evaluate(double v, double w, double x, double y, double z) const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.Curve");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVE_IMPL_HPP
