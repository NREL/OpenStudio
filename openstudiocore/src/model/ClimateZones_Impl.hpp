/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
    virtual boost::optional<ParentObject> parent() const override;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

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
    virtual bool setParent(ParentObject& newParent) override;

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
