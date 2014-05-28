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

#ifndef SHAREDGUICOMPONENTS_TIDITEMMODEL_HPP
#define SHAREDGUICOMPONENTS_TIDITEMMODEL_HPP

#include <QAbstractItemModel>
#include <QDomDocument>
#include <map>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

class TIDItem;

class TIDItemModel : public QAbstractItemModel
{
  Q_OBJECT

  public:

  TIDItemModel(const QDomDocument & document, QObject * parent = nullptr);

  virtual ~TIDItemModel();

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

  Qt::ItemFlags flags(const QModelIndex & index) const;

  QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;

  QModelIndex parent(const QModelIndex & index) const;

  int rowCount(const QModelIndex & parent = QModelIndex()) const;

  int columnCount(const QModelIndex & parent = QModelIndex()) const;

  QModelIndex indexForTID(int tid) const;

  static const std::map<int,std::vector<IddObjectType> > tidToOSTypeMap;

  private:

  QDomDocument m_document;

  TIDItem * m_rootItem;

  static const std::map<int,std::vector<IddObjectType> > createTidToOSTypeMap();
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_TIDITEMMODEL_HPP
