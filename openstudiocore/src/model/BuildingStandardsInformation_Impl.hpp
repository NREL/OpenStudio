/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef MODEL_BUILDINGSTANDARDSINFORMATION_IMPL_HPP
#define MODEL_BUILDINGSTANDARDSINFORMATION_IMPL_HPP

#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

class BuildingStandardsInformation;

namespace detail {

  class MODEL_API BuildingStandardsInformation_Impl : public ModelObject_Impl {
   public:
 
    /** Constructors and Destructors */
    //@{

    // constructor
    BuildingStandardsInformation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    BuildingStandardsInformation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    BuildingStandardsInformation_Impl(const BuildingStandardsInformation_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    // virtual destructor
    virtual ~BuildingStandardsInformation_Impl(){}

    //@}
    /** Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const;

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<int> numberOfStories() const;

    boost::optional<int> numberOfAboveGroundStories() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberOfStories(boost::optional<int> value);

    bool setNumberOfAboveGroundStories(boost::optional<int> value);

    //@}
    /** @name Queries */
    //@{
    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.BuildingStandardsInformation");
  };

} // detail

} // model 
} // openstudio

#endif // MODEL_BUILDINGSTANDARDSINFORMATION_IMPL_HPP
