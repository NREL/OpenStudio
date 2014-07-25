/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_CLIMATEZONES_IMPL_HPP
#define MODEL_CLIMATEZONES_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Site;
class ClimateZone;

namespace detail {

  class MODEL_API ClimateZones_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ClimateZones_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ClimateZones_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                      Model_Impl* model, 
                      bool keepHandle);

    // clone copy constructor
    ClimateZones_Impl(const ClimateZones_Impl& other, Model_Impl* model, bool keepHandle);

    // virtual destructor
    virtual ~ClimateZones_Impl(){}

    //@}
    /** @name Getters */
    //@{

    /** Site parents the unique ClimateZones object. */
    boost::optional<Site> site() const;

    std::vector<ClimateZone> climateZones() const;

    ClimateZone getClimateZone(unsigned index) const;

    ClimateZone getClimateZone(const std::string& institution,unsigned year) const;

    std::vector<ClimateZone> getClimateZones(const std::string& institution) const;

    //ClimateZone activeClimateZone() const;

    //std::string activeClimateZoneValue() const;

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;
    
    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Setters */
    //@{

    bool clear();

    //ClimateZone setActiveClimateZone(const std::string& institution);

    //ClimateZone setActiveClimateZone(const std::string& institution,unsigned year);

    ClimateZone setClimateZone(const std::string& institution,
                               const std::string& value);

    ClimateZone setClimateZone(const std::string& institution,
                               unsigned year, 
                               const std::string& value);

    ClimateZone appendClimateZone(const std::string& institution);

    ClimateZone appendClimateZone(const std::string& institution,
                                  const std::string& value);

    ClimateZone appendClimateZone(const std::string& institution,
                                  unsigned year, 
                                  const std::string& value);

    ClimateZone appendClimateZone(const std::string& institution, 
                                  const std::string documentName, 
                                  unsigned year, 
                                  const std::string& value);

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    //@}
    /** @name Queries */
    //@{

    unsigned numClimateZones() const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.ClimateZones");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CLIMATEZONES_IMPL_HPP
