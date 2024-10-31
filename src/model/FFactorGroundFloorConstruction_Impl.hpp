/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP
#define MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP

#include "ConstructionBase_Impl.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class MODEL_API FFactorGroundFloorConstruction_Impl : public ConstructionBase_Impl
    {

     public:
      // constructor
      FFactorGroundFloorConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      FFactorGroundFloorConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FFactorGroundFloorConstruction_Impl(const FFactorGroundFloorConstruction_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~FFactorGroundFloorConstruction_Impl() override = default;

      /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Queries */
      //@{

      virtual bool isOpaque() const override;

      virtual bool isFenestration() const override;

      virtual bool isSolarDiffusing() const override;

      virtual bool isModelPartition() const override;

      virtual int renderingColorIndex() const override;

      /// Returns fFactor.
      double fFactor() const;
      bool setFFactor(double fFactor);

      /// Returns area.
      double area() const;
      bool setArea(double area);

      /// Returns perimeterExposed.
      double perimeterExposed() const;
      bool setPerimeterExposed(double perimeterExposed);

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.FFactorGroundFloorConstruction");
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP
