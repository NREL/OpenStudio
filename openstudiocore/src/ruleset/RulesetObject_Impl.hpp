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

#ifndef RULESET_RULESETOBJECT_IMPL_HPP
#define RULESET_RULESETOBJECT_IMPL_HPP

#include <ruleset/RulesetAPI.hpp>

#include <utilities/core/UUID.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/enable_shared_from_this.hpp>

class QDomElement;
class QDomDocument;

namespace openstudio {
namespace ruleset {

class RulesetObject;

namespace detail {

  class RULESET_API RulesetObject_Impl : public QObject, 
                                         public boost::enable_shared_from_this<RulesetObject_Impl> 
  {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    RulesetObject_Impl();

    RulesetObject_Impl(const UUID& uuid, const UUID& versionUUID);

    RulesetObject_Impl(const QDomElement& element);

    // creates new UUID
    RulesetObject_Impl(const RulesetObject_Impl& other);

    /** Get an object that wraps this impl. */
    template<typename T>
    T getObject() const
    {
      T result(boost::dynamic_pointer_cast<typename T::ImplType>(
                 boost::const_pointer_cast<RulesetObject_Impl>(shared_from_this())));
      return result;
    }

    virtual ~RulesetObject_Impl() {}

    //@}
    /** @name Getters */
    //@{

    UUID uuid() const;

    UUID versionUUID() const;

    //@}
    /** @name Serialization */
    //@{

    /// name of this xml element
    virtual std::string xmlElementName() const = 0;

    /// write object and all children to xml
    QDomDocument toXml() const;

    virtual void writeValues(QDomDocument& doc, QDomElement& element) const;

    //@}
    /** @name Queries */
    //@{

    /** Equality as determined by deep comparison of data members. */
    virtual bool equivalentData(const RulesetObject& other) const;

    //@}

   signals:
     void changed();

   public slots:
     void onChildChanged();

   protected:
     std::string printUUID() const;

     void onChange();

     void connectChild(const RulesetObject& child) const;

     void disconnectChild(const RulesetObject& child) const;
   private:
    UUID m_uuid;
    UUID m_versionUUID;

    REGISTER_LOGGER("Ruleset.RulesetObject");
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_RULESETOBJECT_IMPL_HPP
