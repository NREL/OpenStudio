/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKEXTERNALNODE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKEXTERNALNODE_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** AirflowNetworkExternalNode_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkExternalNode.*/
    class MODEL_API AirflowNetworkExternalNode_Impl : public AirflowNetworkNode_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkExternalNode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkExternalNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkExternalNode_Impl(const AirflowNetworkExternalNode_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkExternalNode_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double externalNodeHeight() const;

      bool isExternalNodeHeightDefaulted() const;

      Curve windPressureCoefficientCurve() const;

      bool symmetricWindPressureCoefficientCurve() const;

      bool isSymmetricWindPressureCoefficientCurveDefaulted() const;

      std::string windAngleType() const;

      bool isWindAngleTypeDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      void setExternalNodeHeight(double externalNodeHeight);

      void resetExternalNodeHeight();

      bool setWindPressureCoefficientCurve(const Curve& wPCValue);

      void setSymmetricWindPressureCoefficientCurve(bool symmetricWindPressureCoefficientCurve);

      void resetSymmetricWindPressureCoefficientCurve();

      bool setWindAngleType(const std::string& windAngleType);

      void resetWindAngleType();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkExternalNode");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalWindPressureCoefficientCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEXTERNALNODE_IMPL_HPP
