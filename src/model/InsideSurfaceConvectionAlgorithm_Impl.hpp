/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INSIDESURFACECONVECTIONALGORITHM_IMPL_HPP
#define MODEL_INSIDESURFACECONVECTIONALGORITHM_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API InsideSurfaceConvectionAlgorithm_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      InsideSurfaceConvectionAlgorithm_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      InsideSurfaceConvectionAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      InsideSurfaceConvectionAlgorithm_Impl(const InsideSurfaceConvectionAlgorithm_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~InsideSurfaceConvectionAlgorithm_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string algorithm() const;

      bool isAlgorithmDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAlgorithm(const std::string& algorithm);

      void resetAlgorithm();

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.InsideSurfaceConvectionAlgorithm");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INSIDESURFACECONVECTIONALGORITHM_IMPL_HPP
