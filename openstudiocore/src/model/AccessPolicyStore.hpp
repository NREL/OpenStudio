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
#ifndef MODEL_ACCESSPOLICYSTORE_HPP
#define MODEL_ACCESSPOLICYSTORE_HPP

#include "ModelAPI.hpp"

#include <map>

#include <QFile>
#include <QString>

#include "../utilities/idd/IddObject.hpp"

#include "../utilities/core/Path.hpp"

namespace openstudio
{
  namespace model
  {

    class AccessParser;

    /*! Access Policy will tell ModelObjects witch fields to expose
     *
     * The Policy will restrict your access to ModelObject data.
     * Workspace objects have fields. you can index into those fields and
     * get the value, change the value, ect. ModelObjects may not want to
     * allow you to change things, or even show them. The policy is used to
     * build GUIs, it is not really something for general use in the backend
     * data manipulation side of things. The idea is that each program might have its
     * own XML file that tells the AccessPolicy how to display fields.
     */
    class MODEL_API AccessPolicy
    {
      friend class AccessParser;

    public:

      /*! tells the ModelObect how to sort its fields for display in a GUI*/
      enum ACCESS_LEVEL
        { FREE, /*!< Show this field, allow edits */
          LOCKED, /*!< Show this field, do not allow edits */
          HIDDEN };/*!< Do not show this field*/

      AccessPolicy();

      /*! gets the level of access
        \param index the index into the field vector
        \return defaults to FREE
        *
        * If you send in an erronious index, or the policy didn't
        * load information for that index, you will get FREE back.
        * (You might get suprising results when you ask a workspace object
        * for that bogus index though. :) )
        */
      ACCESS_LEVEL getAccess(unsigned int index) const;

    private:
      std::map<unsigned int, ACCESS_LEVEL> m_accessMap;
      unsigned int m_numNormalFields;
      unsigned int m_extensibleSize;
      std::map<unsigned int, ACCESS_LEVEL> m_extensibleAccessMap;
    };


  /*! This class is a simple singleton that stores AccessPolicy for ModelObjects
   *
   *
   */
    class MODEL_API AccessPolicyStore
    {
      friend class AccessParser;

    public:
      static AccessPolicyStore& Instance();

      /*! loads an xml with the policy rules
       */
      bool loadFile( QFile& file );
      bool loadFile( const openstudio::path& path );

      /*!Each IddObjectType has a uniqueAcessPolicy. This function will retrieve it*/
      const AccessPolicy* getPolicy( const openstudio::IddObjectType& )const;
    private:
      AccessPolicyStore();
      AccessPolicyStore(const AccessPolicyStore&);
      AccessPolicyStore& operator=(const AccessPolicyStore&);

      std::map<openstudio::IddObjectType,AccessPolicy*> m_policyMap;

      static AccessPolicyStore* s_instance;
      REGISTER_LOGGER("model.AccessPolicyStore");
    };

  }
}
#endif
