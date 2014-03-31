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

#include <shared_gui_components/TIDItemModel.hpp>
#include <utilities/core/Compare.hpp>

namespace openstudio {

class TIDItem
{
public:

  TIDItem(const QDomElement & element, TIDItem * parent = 0)
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
          TIDItem * childItem = new TIDItem(childElement,this); 

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
    return 0;
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

