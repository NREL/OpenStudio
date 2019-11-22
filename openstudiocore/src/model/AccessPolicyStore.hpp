/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_ACCESSPOLICYSTORE_HPP
#define MODEL_ACCESSPOLICYSTORE_HPP

#include "ModelAPI.hpp"

#include <map>

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
     * get the value, change the value, etc. ModelObjects may not want to
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
        * If you send in an erroneous index, or the policy didn't
        * load information for that index, you will get FREE back.
        * (You might get surprising results when you ask a workspace object
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
      bool loadFile( openstudio::filesystem::ifstream & file );
      bool loadFile( const openstudio::path& path );
      bool loadFile( const QByteArray &data );

      /*!Each IddObjectType has a uniqueAcessPolicy. This function will retrieve it*/
      const AccessPolicy* getPolicy( const openstudio::IddObjectType& )const;

      /* clear the map*/
      void clear();

    private:
      AccessPolicyStore();
      ~AccessPolicyStore();
      AccessPolicyStore(const AccessPolicyStore&);
      AccessPolicyStore& operator=(const AccessPolicyStore&);

      std::map<openstudio::IddObjectType,AccessPolicy*> m_policyMap;

      static AccessPolicyStore* s_instance;
      REGISTER_LOGGER("model.AccessPolicyStore");
    };

  }
}
#endif
