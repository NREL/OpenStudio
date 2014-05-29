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
#include <iostream>
#include <sstream>

#include <QXmlDefaultHandler>
#include <QXmlReader>

#include "AccessPolicyStore.hpp"

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
                        const QXmlAttributes& atts  );

      bool endElement  ( const QString& namespaceURI,
                         const QString& localName,
                         const QString& qName );

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
      m_curType(IddObjectType::Catchall)
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

            }
            m_curPolicy = new AccessPolicy();
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
    {
    }



    AccessPolicy::ACCESS_LEVEL AccessPolicy::getAccess(unsigned int index )const
    {
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

    AccessPolicyStore& AccessPolicyStore::Instance()
    {
      if(!s_instance)
      {
        s_instance = new AccessPolicyStore();
      }
      return *s_instance;
    }

    bool AccessPolicyStore::loadFile( QFile& file)
    {
      QXmlSimpleReader xmlReader;
      AccessParser ap;
      xmlReader.setContentHandler( &ap );

      if(!file.exists())
      {
        LOG(Debug,"file:"<<file.fileName().toStdString()<<" was not found\n");
        return false;
      }
      auto source = new QXmlInputSource( &file );
      //LER:: add error handler
      if(!xmlReader.parse(source))
      {
        LOG(Debug,"xml parse error in AccessPolicyStore::loadFile\n");
        return false;
      }
      return true;

    }

    bool AccessPolicyStore::loadFile(const openstudio::path& path)
    {
      QFile file(toQString(path));
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

  }//model
}//openstudio
