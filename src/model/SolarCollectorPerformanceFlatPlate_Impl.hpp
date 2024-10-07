/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_IMPL_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SolarCollectorPerformanceFlatPlate_Impl is a ModelObject_Impl that is the implementation class for SolarCollectorPerformanceFlatPlate.*/
    class MODEL_API SolarCollectorPerformanceFlatPlate_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      SolarCollectorPerformanceFlatPlate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SolarCollectorPerformanceFlatPlate_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SolarCollectorPerformanceFlatPlate_Impl(const SolarCollectorPerformanceFlatPlate_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SolarCollectorPerformanceFlatPlate_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double grossArea() const;

      std::string testFluid() const;

      bool isTestFluidDefaulted() const;

      double testFlowRate() const;

      std::string testCorrelationType() const;

      double coefficient1ofEfficiencyEquation() const;

      double coefficient2ofEfficiencyEquation() const;

      boost::optional<double> coefficient3ofEfficiencyEquation() const;

      boost::optional<double> coefficient2ofIncidentAngleModifier() const;

      boost::optional<double> coefficient3ofIncidentAngleModifier() const;

      //@}
      /** @name Setters */
      //@{

      bool setGrossArea(double grossArea);

      bool setTestFluid(const std::string& testFluid);

      void resetTestFluid();

      bool setTestFlowRate(double testFlowRate);

      bool setTestCorrelationType(const std::string& testCorrelationType);

      bool setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation);

      bool setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation);

      bool setCoefficient3ofEfficiencyEquation(boost::optional<double> coefficient3ofEfficiencyEquation);

      void resetCoefficient3ofEfficiencyEquation();

      bool setCoefficient2ofIncidentAngleModifier(boost::optional<double> coefficient2ofIncidentAngleModifier);

      void resetCoefficient2ofIncidentAngleModifier();

      bool setCoefficient3ofIncidentAngleModifier(boost::optional<double> coefficient3ofIncidentAngleModifier);

      void resetCoefficient3ofIncidentAngleModifier();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceFlatPlate");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_IMPL_HPP
