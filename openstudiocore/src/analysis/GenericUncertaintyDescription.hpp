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

#ifndef ANALYSIS_GENERICUNCERTAINTYDESCRIPTION_HPP
#define ANALYSIS_GENERICUNCERTAINTYDESCRIPTION_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/UncertaintyDescription.hpp>

namespace openstudio {

class Attribute;
struct AttributeDescription;

namespace analysis {

/** GenericUncertaintyDescription is an UncertaintyDescription interface for use by
 *  APIs and for serialization/deserialization. It works equally well for all 
 *  \link UncertaintyDescriptionType UncertaintyDescriptionTypes \endlink.  */
class ANALYSIS_API GenericUncertaintyDescription : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  GenericUncertaintyDescription(const UncertaintyDescriptionType& type);

  /** Constructor provided for deserialization; not for general use. */
  GenericUncertaintyDescription(const UncertaintyDescriptionType& type,
                                const std::vector<Attribute>& attributes);

  virtual ~GenericUncertaintyDescription() {}

  //@}

  /** Returns the interface UncertaintyDescriptionType, that is,
   *  UncertaintyDescriptionType::Generic */
  static UncertaintyDescriptionType type();

  /** @name Getters and Queries */
  //@{

  /** Returns the UncertaintyDescriptionType that was used to construct this object, that
   *  is, the underlying, actual, type. */
  UncertaintyDescriptionType actualType() const;

  /** Returns all options that are set. */
  std::vector<Attribute> attributes() const;

  /** Describes all options for this UncertaintyDescription::type(), including whether they 
   *  are required. */
  std::vector<AttributeDescription> attributeDescriptions() const;

  /** Returns Attribute attributeName by value (returned object does not share data with 
   *  private member). Throws if not isSet(attributeName). */
  Attribute getAttribute(const std::string& attributeName) const;

  bool isSet(const std::string& attributeName) const;

  /** Returns true if all required attributes are set. */
  bool isComplete() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets candidate Attribute if possible. Must match one of the attributeDescriptions(). 
   *  \post candidate does not share data with any member variables */
  bool setAttribute(const Attribute& candidate);

  /** Clears Attribute attributeName if that Attribute is currently set and is not required. */
  bool clearAttribute(const std::string& attributeName);

  //@}
 protected:
  explicit GenericUncertaintyDescription(boost::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.GenericUncertaintyDescription");
};

/** \relates GenericUncertaintyDescription */
typedef std::vector<GenericUncertaintyDescription> GenericUncertaintyDescriptionVector;

} // analysis
} // openstudio

#endif // ANALYSIS_GENERICUNCERTAINTYDESCRIPTION_HPP
