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

#ifndef RULESET_MODELRULESET_IMPL_HPP
#define RULESET_MODELRULESET_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>
#include <ruleset/Ruleset_Impl.hpp>
#include <ruleset/ModelRule.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {
namespace model {
  class Model;
}
namespace ruleset {

class ModelRuleset;

namespace detail {

  class RULESET_API ModelRuleset_Impl : public Ruleset_Impl {

    Q_OBJECT;

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Creates empty ruleset. */
    ModelRuleset_Impl(const std::string& name);

    ModelRuleset_Impl(const std::string& name, const UUID& uuid, const UUID& versionUUID,
                      const std::vector<ModelRule>& rules);

    ModelRuleset_Impl(const QDomElement& element);
    
    virtual ~ModelRuleset_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::vector<ModelRule> rules() const;

    //@}
    /** @name Setters */
    //@{

    void add(const ModelRule& rule);

    //@}
    /** @name Serialization */
    //@{

    virtual std::string xmlElementName() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}
    /** @name Actions */
    //@{

    bool apply(openstudio::model::Model& model);

    //@}
   protected:
    friend class openstudio::ruleset::Ruleset;

   private:
    std::vector<ModelRule> m_rules;

    REGISTER_LOGGER("Ruleset.ModelRuleset");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_MODELRULESET_IMPL_HPP
