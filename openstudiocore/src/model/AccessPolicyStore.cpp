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

#include <iostream>
#include <sstream>

#include <QXmlDefaultHandler>
#include <QXmlReader>

#include "AccessPolicyStore.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/idd/IddFileAndFactoryWrapper.hpp"

using std::map;
using std::stringstream;

namespace openstudio
{
  namespace model
  {
    AccessPolicyStore* AccessPolicyStore::s_instance=nullptr;


    //XML Parser
    class AccessParser:public QXmlDefaultHandler
    {
    public:
      AccessParser();

    protected:
      bool startElement(const QString& namespaceURI,
                        const QString& localName,
                        const QString& qName,
                        const QXmlAttributes& atts  ) override;

      bool endElement  ( const QString& namespaceURI,
                         const QString& localName,
                         const QString& qName ) override;

      bool error(QXmlParseException&);
      bool fatalError(QXmlParseException&);
    private:
      AccessPolicy* m_curPolicy;
      IddObjectType m_curType;
      IddFileAndFactoryWrapper m_factory;
      REGISTER_LOGGER("AccessParser");
    };

    AccessParser::AccessParser():
      m_curPolicy(nullptr),
      m_curType("Catchall")
    {
    }

    bool AccessParser::error(QXmlParseException& e)
    {
      stringstream s;
      s<<"Error line:"
       <<e.lineNumber()
       <<"\ncolumn:"
       <<e.columnNumber()
       <<"\n"
       <<e.message().toStdString()
       <<"\n";
      LOG(Debug,s.str());
      return false;
    }
    bool AccessParser::fatalError(QXmlParseException& e)
    {
      LOG(Debug,"Error line:"<<e.lineNumber()<<"\ncolumn:"<<e.columnNumber()<<"\n"<<e.message().toStdString()<<"\n");
      return false;
    }

    bool AccessParser::startElement(const QString& /*namespaceURI*/,
                                    const QString& /*localName*/,
                                    const QString& qName,
                                    const QXmlAttributes& atts  )
    {
      if( !qName.compare("policy",Qt::CaseInsensitive))
      {
        if( m_curPolicy )
        {
          LOG(Debug,"parse error, new policy started before old one ended\n");
          return false;
        }

        for( int i=0,iend=atts.length();i<iend;++i)
        {
          QString name = atts.qName( i );
          if(!name.compare("IddObjectType",Qt::CaseInsensitive))
          {
            QString val = atts.value(i);
            try{
              m_curType = IddObjectType(val.toStdString());
            }
            catch(...)
            {
              LOG(Debug,"IddObjectType failed conversion:"<<val.toStdString()<<"\n");
              return false;//Bad IddObjectType
            }
            auto exists = AccessPolicyStore::Instance().m_policyMap.find( m_curType );
            if( exists != AccessPolicyStore::Instance().m_policyMap.end() )
            {
              LOG(Warn,"2 entries of same type found in policy xml. Later entries will obscure previous entires:"<<val.toStdString()<<"\n");
              delete exists->second;
              AccessPolicyStore::Instance().m_policyMap.erase( exists );
              // DLM: return false?
              OS_ASSERT(false);
            }
            m_curPolicy = new AccessPolicy();

            OptionalIddObject opObj = m_factory.getObject(m_curType);
            if (opObj)
            {
              // initialize here in case there are no rules
              m_curPolicy->m_numNormalFields = opObj->numFields();
              m_curPolicy->m_extensibleSize = opObj->properties().numExtensible;
            }

            AccessPolicyStore::Instance().m_policyMap[m_curType] = m_curPolicy;
            return true;//I don't care about any other attributes!

          }
        }
        return false;//NO IddObjectType!!!!
      }
      else if(!qName.compare("rule",Qt::CaseInsensitive))
      {
        if( !m_curPolicy )
        {
          LOG(Debug,"parse error, rule started before a policy is started");
          return false;
        }
        QString fieldName;
        QString accessRule;
        for( int i=0,iend=atts.length();i<iend;++i)
        {
          QString name = atts.qName( i );
          if(!name.compare("IddField",Qt::CaseInsensitive))
          {
            fieldName = atts.value(i);
          }
          else if(!name.compare("access",Qt::CaseInsensitive))
          {
            accessRule=atts.value(i);
          }
        }
        if( fieldName.size() && accessRule.size() )
        {
          AccessPolicy::ACCESS_LEVEL level=AccessPolicy::FREE;
          if(!accessRule.compare("locked",Qt::CaseInsensitive))
          {
            level = AccessPolicy::LOCKED;
          }
          else if(!accessRule.compare("hidden",Qt::CaseInsensitive))
          {
            level = AccessPolicy::HIDDEN;
          }

          OptionalIddObject opObj=m_factory.getObject( m_curType );
          if( !opObj )
          {
            LOG(Debug,"IddObject not found in factory!!!\n");
            return true;//keep going
          }
          IddObject obj = *opObj;

          bool foundInFields=false;
           for( unsigned int i=0,iend=obj.numFields();i<iend;++i)
           {
             openstudio::OptionalIddField f  = obj.getField(i);
             QString fieldName2(f->name().c_str());
             if( !fieldName.compare(fieldName2,Qt::CaseInsensitive) )
             {
               m_curPolicy->m_accessMap[i]=level;
               foundInFields=true;
               break;
             }
           }
           m_curPolicy->m_numNormalFields = obj.numFields();
           m_curPolicy->m_extensibleSize = obj.properties().numExtensible;
           for (unsigned int i = obj.numFields(),iend=obj.properties().numExtensible+obj.numFields(); i<iend && !foundInFields;++i)
           {
               openstudio::OptionalIddField f  = obj.getField(i);
               QString fieldName2(f->name().c_str());
               if( !fieldName.compare(fieldName2,Qt::CaseInsensitive) )
               {
                 m_curPolicy->m_extensibleAccessMap[i-obj.numFields()]=level;
                 foundInFields=true;
                 break;
               }
           }

          return true;
        }
        else
        {
          LOG(Debug,"Parse error in <rule> need both IddField and Access attribute\n");
          return true;
        }
      }
      return true;
    }


    bool AccessParser::endElement  ( const QString& /*namespaceURI*/,
                                     const QString& /*localName*/,
                                     const QString& qName )
    {
      if( !qName.compare("policy",Qt::CaseInsensitive))
      {
        m_curPolicy = nullptr;
      }
      return true;
    }


    AccessPolicy::AccessPolicy()
      : m_numNormalFields(std::numeric_limits<unsigned>::max()),
        m_extensibleSize(std::numeric_limits<unsigned>::max())
    {
    }



    AccessPolicy::ACCESS_LEVEL AccessPolicy::getAccess(unsigned int index )const
    {
      if (m_numNormalFields == std::numeric_limits<unsigned>::max()){
        OS_ASSERT(false);
      }
      if (m_extensibleSize == std::numeric_limits<unsigned>::max()){
        OS_ASSERT(false);
      }

      if(index<m_numNormalFields)
      {
        auto i = m_accessMap.find(index);
        if( i != m_accessMap.end() )
        {
          return (*i).second;
        }
      }
      else
      {
        index-=m_numNormalFields;
        index = index % m_extensibleSize;
        auto i = m_extensibleAccessMap.find(index);
        if( i != m_extensibleAccessMap.end() )
        {
          return (*i).second;
        }
      }
      return FREE;
    }


    AccessPolicyStore::AccessPolicyStore()
    {
    }

    AccessPolicyStore::~AccessPolicyStore()
    {
      clear();
    }

    AccessPolicyStore& AccessPolicyStore::Instance()
    {
      if(!s_instance)
      {
        s_instance = new AccessPolicyStore();
      }
      return *s_instance;
    }

    bool AccessPolicyStore::loadFile( const QByteArray &data)
    {
      QXmlSimpleReader xmlReader;
      AccessParser ap;
      xmlReader.setContentHandler( &ap );

      QXmlInputSource source;
      source.setData( data );
      //LER:: add error handler
      if(!xmlReader.parse(source))
      {
        LOG(Debug,"xml parse error in AccessPolicyStore::loadFile\n");
        return false;
      }
      return true;
    }

    bool AccessPolicyStore::loadFile( openstudio::filesystem::ifstream& file)
    {
      QXmlSimpleReader xmlReader;
      AccessParser ap;
      xmlReader.setContentHandler( &ap );

      if(!file.is_open())
      {
        LOG(Debug,"file was not found\n");
        return false;
      }

      return loadFile(openstudio::filesystem::read_as_QByteArray(file));
    }

    bool AccessPolicyStore::loadFile(const openstudio::path& path)
    {
      openstudio::filesystem::ifstream file(path);
      return loadFile(file);
    }

    const AccessPolicy* AccessPolicyStore::getPolicy( const openstudio::IddObjectType& type)const
    {
      auto i = m_policyMap.find(type);
      if(i!=m_policyMap.end())
      {
        return (*i).second;
      }

      return nullptr;

    }

    void AccessPolicyStore::clear()
    {
      for (auto& policyPair : m_policyMap) {
        if (policyPair.second) {
          delete policyPair.second;
        }
      }
      m_policyMap.clear();
    }

  }//model
}//openstudio
