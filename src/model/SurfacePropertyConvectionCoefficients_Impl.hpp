/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_IMPL_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** SurfacePropertyConvectionCoefficients_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyConvectionCoefficients.*/
    class MODEL_API SurfacePropertyConvectionCoefficients_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyConvectionCoefficients_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyConvectionCoefficients_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyConvectionCoefficients_Impl(const SurfacePropertyConvectionCoefficients_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyConvectionCoefficients_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      ModelObject surfaceAsModelObject() const;

      boost::optional<Surface> surfaceAsSurface() const;

      boost::optional<SubSurface> surfaceAsSubSurface() const;

      boost::optional<InternalMass> surfaceAsInternalMass() const;

      boost::optional<std::string> convectionCoefficient1Location() const;

      boost::optional<std::string> convectionCoefficient1Type() const;

      boost::optional<double> convectionCoefficient1() const;

      boost::optional<Schedule> convectionCoefficient1Schedule() const;

      boost::optional<std::string> convectionCoefficient2Location() const;

      boost::optional<std::string> convectionCoefficient2Type() const;

      boost::optional<double> convectionCoefficient2() const;

      boost::optional<Schedule> convectionCoefficient2Schedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSurface(const ModelObject& surface);

      bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

      void resetConvectionCoefficient1Location();

      bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

      void resetConvectionCoefficient1Type();

      bool setConvectionCoefficient1(double convectionCoefficient1);

      void resetConvectionCoefficient1();

      bool setConvectionCoefficient1Schedule(Schedule& schedule);

      void resetConvectionCoefficient1Schedule();

      bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

      void resetConvectionCoefficient2Location();

      bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

      void resetConvectionCoefficient2Type();

      bool setConvectionCoefficient2(double convectionCoefficient2);

      void resetConvectionCoefficient2();

      bool setConvectionCoefficient2Schedule(Schedule& schedule);

      void resetConvectionCoefficient2Schedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficients");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTS_IMPL_HPP
