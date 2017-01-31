/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "TIDItemModel.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

class TIDItem
{
public:

  TIDItem(const QDomElement & element, TIDItem * parent = nullptr)
  {
    m_parentItem = parent;

    QString tagName = element.tagName();

    if( istringEqual(tagName.toStdString(),"term") )
    {
      QString tidName = element.firstChildElement("name").text();

      m_itemData.push_back(tidName);

      int tid = element.firstChildElement("tid").text().toInt();

      m_itemData.push_back(tid);
    }
    else if( istringEqual(tagName.toStdString(),"taxonomy") )
    {
      m_itemData.push_back("Taxonomy");

      m_itemData.push_back(0);
    }

    QDomNodeList children = element.childNodes();

    for( int i = 0;
         i < children.length();
         i++ )
    {
      QDomNode childNode = children.at(i); 

      if( childNode.isElement() )
      {
        QDomElement childElement = childNode.toElement();

        if( istringEqual(childElement.tagName().toStdString(),"term") )
        {
          auto childItem = new TIDItem(childElement,this); 

          m_childItems.append(childItem);
        }
      }
    }
  }

  ~TIDItem()
  {
    qDeleteAll(m_childItems);
  }

  TIDItem * child(int row)
  {
    return m_childItems.value(row);
  }

  int childCount() const
  {
    return m_childItems.count();
  }

  int columnCount() const
  {
    return m_itemData.count();
  }

  QVariant data(int column) const
  {
    return m_itemData.value(column);
  }

  int row() const
  {
    if( m_parentItem )
    {
      return m_parentItem->m_childItems.indexOf(const_cast<TIDItem *>(this));
    }
  
    return 0;
  }

  TIDItem * parent()
  {
    return m_parentItem;
  }

private:

  QList<TIDItem *> m_childItems;

  QList<QVariant> m_itemData;

  TIDItem * m_parentItem;
};

TIDItemModel::TIDItemModel(const QDomDocument & document, QObject * parent)
  : QAbstractItemModel(parent),
    m_document(document)
{
  m_rootItem = new TIDItem(m_document.documentElement());
}

TIDItemModel::~TIDItemModel()
{
  delete m_rootItem;
}

QVariant TIDItemModel::data(const QModelIndex & index, int role) const
{
  if( ! index.isValid() )
  {
    return QVariant();
  }

  if( role != Qt::DisplayRole )
  {
    return QVariant();
  }

  TIDItem * item = static_cast<TIDItem *>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags TIDItemModel::flags(const QModelIndex & index) const
{
  if( ! index.isValid() )
  {
    return nullptr;
  }

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex TIDItemModel::index(int row, int column, const QModelIndex & parent) const
{
  if( ! hasIndex(row, column, parent) )
  {
    return QModelIndex();
  }

  TIDItem * parentItem;

  if( ! parent.isValid() )
  {
    parentItem = m_rootItem;
  }
  else
  {
    parentItem = static_cast<TIDItem *>(parent.internalPointer());
  }

  TIDItem * childItem = parentItem->child(row);

  if( childItem )
  {
    return createIndex(row,column,childItem);
  }
  else
  {
    return QModelIndex();
  }
}

QModelIndex TIDItemModel::parent(const QModelIndex & index) const
{
  if( ! index.isValid() )
  {
    return QModelIndex();
  }

  TIDItem * childItem = static_cast<TIDItem *>(index.internalPointer());

  TIDItem * parentItem = childItem->parent();

  if( parentItem == m_rootItem )
  {
    return QModelIndex();
  }

  return createIndex(parentItem->row(),0,parentItem);
}

int TIDItemModel::rowCount(const QModelIndex & parent) const
{
  TIDItem * parentItem;

  if( parent.column() > 0 )
  {
    return 0;
  }

  if( ! parent.isValid() )
  {
    parentItem = m_rootItem;
  }
  else
  {
    parentItem = static_cast<TIDItem *>(parent.internalPointer());
  }

  return parentItem->childCount();
}

int TIDItemModel::columnCount(const QModelIndex & parent) const
{
  if( parent.isValid() )
  {
    return static_cast<TIDItem *>(parent.internalPointer())->columnCount();
  }
  else
  {
    return m_rootItem->columnCount();
  }
}

QModelIndex TIDItemModel::indexForTID(int tid) const
{
  QModelIndexList list = match(index(0,1),Qt::DisplayRole,tid);

  if( ! list.empty() )
  {
    QModelIndex index = list.first();

    return index.sibling(index.row(),0);
  }

  return QModelIndex();
}

const std::map<int,std::vector<IddObjectType> > TIDItemModel::createTidToOSTypeMap()
{
  std::map<int,std::vector<IddObjectType> > map;

  // 127 Construction Assembly
  std::vector<IddObjectType> tid127_types;
  tid127_types.push_back(IddObjectType::OS_Construction); 
  map[127] = tid127_types;

  // KSB: Should we have lower tids in the map like these?
  // If not then programmer will need to walk up the tid tree.

  // 152 Floor
  std::vector<IddObjectType> tid152_types;
  tid152_types.push_back(IddObjectType::OS_Construction); 
  map[152] = tid152_types;

  // 134 Roof
  std::vector<IddObjectType> tid134_types;
  tid134_types.push_back(IddObjectType::OS_Construction); 
  map[134] = tid134_types;

  // 129 Wall
  std::vector<IddObjectType> tid129_types;
  tid129_types.push_back(IddObjectType::OS_Construction); 
  map[129] = tid129_types;

  return map;
}

const std::map<int,std::vector<IddObjectType> > TIDItemModel::tidToOSTypeMap = TIDItemModel::createTidToOSTypeMap();

} // openstudio

