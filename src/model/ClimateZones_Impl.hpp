/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CLIMATEZONES_IMPL_HPP
#define MODEL_CLIMATEZONES_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Site;
  class ClimateZone;

  namespace detail {

    class MODEL_API ClimateZones_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ClimateZones_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ClimateZones_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ClimateZones_Impl(const ClimateZones_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ClimateZones_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      /** Site parents the unique ClimateZones object. */
      boost::optional<Site> site() const;

      std::vector<ClimateZone> climateZones() const;

      ClimateZone getClimateZone(unsigned index) const;

      ClimateZone getClimateZone(const std::string& institution, unsigned year) const;

      std::vector<ClimateZone> getClimateZones(const std::string& institution) const;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Setters */
      //@{

      bool clear();

      ClimateZone setClimateZone(const std::string& institution, const std::string& value);

      ClimateZone setClimateZone(const std::string& institution, unsigned year, const std::string& value);

      ClimateZone appendClimateZone(const std::string& institution);

      ClimateZone appendClimateZone(const std::string& institution, const std::string& value);

      ClimateZone appendClimateZone(const std::string& institution, unsigned year, const std::string& value);

      ClimateZone appendClimateZone(const std::string& institution, const std::string documentName, unsigned year, const std::string& value);

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      //@}
      /** @name Queries */
      //@{

      unsigned numClimateZones() const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ClimateZones");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CLIMATEZONES_IMPL_HPP
