/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_IMPL_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** SurfacePropertyConvectionCoefficientsMultipleSurface_Impl is a ModelObject_Impl that is the implementation class for SurfacePropertyConvectionCoefficientsMultipleSurface.*/
    class MODEL_API SurfacePropertyConvectionCoefficientsMultipleSurface_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                bool keepHandle);

      SurfacePropertyConvectionCoefficientsMultipleSurface_Impl(const SurfacePropertyConvectionCoefficientsMultipleSurface_Impl& other,
                                                                Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyConvectionCoefficientsMultipleSurface_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<std::string> surfaceType() const;

      boost::optional<std::string> convectionCoefficient1Location() const;

      boost::optional<std::string> convectionCoefficient1Type() const;

      boost::optional<double> convectionCoefficient1() const;

      boost::optional<Schedule> convectionCoefficient1Schedule() const;

      //boost::optional<UserConvectionModels> convectionCoefficient1UserCurve() const;

      boost::optional<std::string> convectionCoefficient2Location() const;

      boost::optional<std::string> convectionCoefficient2Type() const;

      boost::optional<double> convectionCoefficient2() const;

      boost::optional<Schedule> convectionCoefficient2Schedule() const;

      //boost::optional<UserConvectionModels> convectionCoefficient2UserCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setSurfaceType(const std::string& surfaceType);

      void resetSurfaceType();

      bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

      void resetConvectionCoefficient1Location();

      bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

      void resetConvectionCoefficient1Type();

      bool setConvectionCoefficient1(double convectionCoefficient1);

      void resetConvectionCoefficient1();

      bool setConvectionCoefficient1Schedule(Schedule& schedule);

      void resetConvectionCoefficient1Schedule();

      //bool setConvectionCoefficient1UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

      //void resetConvectionCoefficient1UserCurve();

      bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

      void resetConvectionCoefficient2Location();

      bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

      void resetConvectionCoefficient2Type();

      bool setConvectionCoefficient2(double convectionCoefficient2);

      void resetConvectionCoefficient2();

      bool setConvectionCoefficient2Schedule(Schedule& schedule);

      void resetConvectionCoefficient2Schedule();

      //bool setConvectionCoefficient2UserCurve(const boost::optional<UserConvectionModels>& userConvectionModels);

      //void resetConvectionCoefficient2UserCurve();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficientsMultipleSurface");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_IMPL_HPP
