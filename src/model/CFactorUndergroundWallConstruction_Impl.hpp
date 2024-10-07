/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_IMPL_HPP
#define MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_IMPL_HPP

#include "ConstructionBase_Impl.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class MODEL_API CFactorUndergroundWallConstruction_Impl : public ConstructionBase_Impl
    {

     public:
      // constructor
      CFactorUndergroundWallConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      CFactorUndergroundWallConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CFactorUndergroundWallConstruction_Impl(const CFactorUndergroundWallConstruction_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~CFactorUndergroundWallConstruction_Impl() override = default;

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

      /// Returns cFactor.
      double cFactor() const;
      bool setCFactor(double cFactor);

      /// Returns height.
      double height() const;
      bool setHeight(double height);

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.CFactorUndergroundWallConstruction");
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CFACTORUNDERGROUNDWALLCONSTRUCTION_IMPL_HPP
