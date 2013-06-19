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

#ifndef RULESET_RULESET_IMPL_HPP
#define RULESET_RULESET_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/RulesetObject_Impl.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {
namespace ruleset {

class Ruleset;

namespace detail {

  class RULESET_API Ruleset_Impl : public RulesetObject_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Creates empty ruleset. */

    Ruleset_Impl(const std::string& name);

    Ruleset_Impl(const std::string& name, const UUID& uuid, const UUID& versionUUID);

    Ruleset_Impl(const QDomElement& element);

    virtual ~Ruleset_Impl();

    /** Returns public object associated with this Impl. */
    Ruleset ruleset() const;

    //@}
    /** @name Getters */
    //@{

    std::string name() const;

    //@}
    /** @name Setters */
    //@{

    bool setName(const std::string& name);

    //@}
    /** @name Serialization */
    //@{

    bool saveToXml(const openstudio::path& path) const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
   protected:
    friend class openstudio::ruleset::Ruleset;

   private:

    REGISTER_LOGGER("Ruleset.Ruleset");

    std::string m_name;

  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_RULESET_IMPL_HPP
