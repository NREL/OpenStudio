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

#ifndef MODEL_CLIMATEZONES_HPP
#define MODEL_CLIMATEZONES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

class Site;

namespace detail {
  class ClimateZones_Impl;
} // detail

/** ClimateZone is a ModelExtensibleGroup that represents a single climate zone definition
 *  contained in the unique ClimateZones ModelObject. */
class MODEL_API ClimateZone : public ModelExtensibleGroup {
 public:
  /** @name Getters */
  //@{

  /** The institution that defines this climate zone designation. Typically this is
   *  "ASHRAE" or "CEC". */
  std::string institution() const;

  /** The name of the document that defines this climate zone designation. "Standard 169",
   *  etc. */
  std::string documentName() const;

  /** The year of the document. Together with institution and documentName, this provides
   *  a fairly complete citation, which implicitly defines the allowable values. */
  unsigned year() const;

  /** The actual climate zone designation. "4C", "15", etc. */
  std::string value() const;

  //@}
  /** @name Setters */
  //@{

  bool setValue(const std::string& value);

  bool setType(const std::string& institution,const std::string& documentName, unsigned year);

  bool setTypeAndValue(const std::string& institution,
                       const std::string& documentName,
                       unsigned year,
                       const std::string& value);
  //@}
 protected:
  /// @cond
  typedef detail::ClimateZones_Impl ImplType;

  friend class IdfExtensibleGroup;
  friend class detail::ClimateZones_Impl;

  ClimateZone(std::shared_ptr<detail::ClimateZones_Impl> impl,unsigned index);

  /// @endcond
 private:

  /** Private default constructor. */
  ClimateZone();

  REGISTER_LOGGER("openstudio.model.ClimateZones");
};

/** \relates ClimateZone */
typedef std::vector<ClimateZone> ClimateZoneVector;

/** ClimateZones is a unque ModelObject that wraps the OpenStudio IDD object 'OS:ClimateZones'.
 *  As a location identifier, it is a child of Site. ClimateZones was added to support building
 *  energy standards like Title 24 and ASHRAE 90.1, which list different requirements based on the
 *  climate zone locations of buildings. Since a given building may be evaluated against multiple
 *  standards, multiple climate zones may be specified for a given model.
 *
 *  \todo Provide methods for setting the ClimateZones for a model based on the data in Site and/or
 *  WeatherFile, and for checking whether the ClimateZones object is consistent with the geospatial
 *  location(s) called out in Site and/or WeatherFile. */
class MODEL_API ClimateZones : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ClimateZones() {}

  //@}
  /** @name Static Methods */
  //@{

  static std::string ashraeInstitutionName();

  static std::string ashraeDocumentName();

  static unsigned ashraeDefaultYear();

  static std::string cecInstitutionName();

  static std::string cecDocumentName();

  static unsigned cecDefaultYear();

  static std::string getDefaultDocumentName(const std::string& institution);

  static unsigned getDefaultYear(const std::string& institution);

  static std::vector<std::string> validClimateZoneValues(const std::string& institution,
                                                         unsigned year);

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  /** Site parents the unique ClimateZones object. */
  boost::optional<Site> site() const;

  /** Return the individual climate zone definitions, with each one wrapped up as a ClimateZone
   *  ModelExtensibleGroup. */
  std::vector<ClimateZone> climateZones() const;

  /** Return the ClimateZone definition at extensible group index. */
  ClimateZone getClimateZone(unsigned index) const;

  // ETH@20110615 Multiple definitions for a particular institution, year combination is not
  // handled well. Either return a vector or disallow duplicates at this level. Will have
  // implications for some setters.

  /** Return the ClimateZone definition for institution and year. The return value will be .empty()
   *  if no such ClimateZone is found. */
  ClimateZone getClimateZone(const std::string& institution,unsigned year) const;

  /** Return the ClimateZone defintions for institution (e.g. "ASHRAE" or "CEC"). */
  std::vector<ClimateZone> getClimateZones(const std::string& institution) const;

  /** Return the active ClimateZone definition. If no ClimateZone is designated as active, the
   *  return value will be .empty(). The active climate zone is the one that will be accessed
   *  by the standardsinterface::OpenStudioStandardsInterface, perhaps in the course of applying a
   *  ruleset::StandardsRuleset using a rulesengine::OpenStudioRulesEngine. */
  //ClimateZone activeClimateZone() const;

  /** Return the active ClimateZone value. If no ClimateZone is designated as active, the return
   *  value will be .empty(). The active climate zone is the one that will be accessed by the
   *  standardsinterface::OpenStudioStandardsInterface, perhaps in the course of applying a
   *  ruleset::StandardsRuleset using a rulesengine::OpenStudioRulesEngine. */
  //std::string activeClimateZoneValue() const;

  //@}
  /** @name Setters */
  //@{

  /** Clear all ClimateZone definitions. */
  bool clear();

  /** Sets the active ClimateZone to be one defined by institution. If there are no such
   *  definitions, this method is equivalent to appendClimateZone(institution). If there are
   *  multiple climate zones associated with institution, the first one is set as active and
   *  a warning is logged. The return value of this method should never be .empty(). */
  //ClimateZone setActiveClimateZone(const std::string& institution);

  /** Sets the active ClimateZone to be one defined by institution and year. If there are no such
   *  definitions, this method is equivalent to appendClimateZone(institution,year). The return
   *  value of this method should never be .empty(). */
  //ClimateZone setActiveClimateZone(const std::string& institution,unsigned year);

  /** Sets the ClimateZone definition for institution to value and returns the modified (or
   *  created) ClimateZone. If there are no definitions for institution, this method is equivalent
   *  to appendClimateZone(institution,value). If there are multiple definitions for institution,
   *  the return value is .empty() and a warning is logged. */
  ClimateZone setClimateZone(const std::string& institution,
                             const std::string& value);


  /** Sets the ClimateZone definition for institution, year to value and returns the modified (or
   *  created) ClimateZone. If there are no definitions for the given combination of institution
   *  and year, this method is equivalent to appendClimateZone(institution,year,value). */
  ClimateZone setClimateZone(const std::string& institution,
                             unsigned year,
                             const std::string& value);

  /** Creates and returns a new climate zone definition for institution. Attempts to provide a
   *  default document name and year, leaves the value blank. */
  ClimateZone appendClimateZone(const std::string& institution);

  /** Creates and returns a new climate zone definition for institution, with value. Attempts to
   *  provide a default document name and year. */
  ClimateZone appendClimateZone(const std::string& institution,
                                const std::string& value);

  /** Creates and returns a new climate zone definition. Attempts to provide a default document
   *  name. */
  ClimateZone appendClimateZone(const std::string& institution,
                                unsigned year,
                                const std::string& value);

  /** Creates and returns a new climate zone definition. */
  ClimateZone appendClimateZone(const std::string& institution,
                                const std::string documentName,
                                unsigned year,
                                const std::string& value);

  //@}
  /** @name Queries */
  //@{

  /** Returns the number of climate zone designations registered with this object. */
  unsigned numClimateZones() const;

  //@}

 protected:
  /// @cond
  typedef detail::ClimateZones_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit ClimateZones(Model& model);

  explicit ClimateZones(std::shared_ptr<detail::ClimateZones_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ClimateZones");
};

/** \relates ClimateZones */
typedef boost::optional<ClimateZones> OptionalClimateZones;

} // model
} // openstudio

#endif // MODEL_CLIMATEZONES_HPP
