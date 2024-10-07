/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_IMPL_HPP
#define MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Surface;

  namespace detail {

    /** SurfacePropertyExposedFoundationPerimeter_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyExposedFoundationPerimeter.*/
    class MODEL_API SurfacePropertyExposedFoundationPerimeter_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyExposedFoundationPerimeter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyExposedFoundationPerimeter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyExposedFoundationPerimeter_Impl(const SurfacePropertyExposedFoundationPerimeter_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyExposedFoundationPerimeter_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      Surface surface() const;

      std::string exposedPerimeterCalculationMethod() const;

      boost::optional<double> totalExposedPerimeter();

      double exposedPerimeterFraction() const;

      bool isExposedPerimeterFractionDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setExposedPerimeterCalculationMethod(const std::string& exposedPerimeterCalculationMethod);

      bool setTotalExposedPerimeter(double totalExposedPerimeter);

      bool setExposedPerimeterFraction(double exposedPerimeterFraction);

      void resetExposedPerimeterFraction();

      virtual bool setParent(ParentObject& newParent) override;

      virtual boost::optional<ParentObject> parent() const override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyExposedFoundationPerimeter");

      boost::optional<Surface> optionalSurface() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_IMPL_HPP
