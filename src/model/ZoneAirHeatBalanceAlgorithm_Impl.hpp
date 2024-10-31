/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEAIRHEATBALANCEALGORITHM_IMPL_HPP
#define MODEL_ZONEAIRHEATBALANCEALGORITHM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ZoneAirHeatBalanceAlgorithm_Impl is a ModelObject_Impl that is the implementation class for ZoneAirHeatBalanceAlgorithm.*/
    class MODEL_API ZoneAirHeatBalanceAlgorithm_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ZoneAirHeatBalanceAlgorithm_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ZoneAirHeatBalanceAlgorithm_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ZoneAirHeatBalanceAlgorithm_Impl(const ZoneAirHeatBalanceAlgorithm_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ZoneAirHeatBalanceAlgorithm_Impl() override = default;

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

      bool doSpaceHeatBalanceforSizing() const;
      bool isDoSpaceHeatBalanceforSizingDefaulted() const;

      bool doSpaceHeatBalanceforSimulation() const;
      bool isDoSpaceHeatBalanceforSimulationDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAlgorithm(const std::string& algorithm);
      void resetAlgorithm();

      bool setDoSpaceHeatBalanceforSizing(bool doSpaceHeatBalanceforSizing);
      void resetDoSpaceHeatBalanceforSizing();

      bool setDoSpaceHeatBalanceforSimulation(bool doSpaceHeatBalanceforSimulation);
      void resetDoSpaceHeatBalanceforSimulation();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneAirHeatBalanceAlgorithm");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEAIRHEATBALANCEALGORITHM_IMPL_HPP
