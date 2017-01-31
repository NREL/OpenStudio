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

#ifndef SHAREDGUICOMPONENTS_TIDITEMMODEL_HPP
#define SHAREDGUICOMPONENTS_TIDITEMMODEL_HPP

#include <QAbstractItemModel>
#include <QDomDocument>
#include <map>

namespace openstudio {
struct IddObjectType;
class TIDItem;

class TIDItemModel : public QAbstractItemModel
{
  Q_OBJECT

  public:

  TIDItemModel(const QDomDocument & document, QObject * parent = nullptr);

  virtual ~TIDItemModel();

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex & index) const override;

  QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;

  QModelIndex parent(const QModelIndex & index) const override;

  int rowCount(const QModelIndex & parent = QModelIndex()) const override;

  int columnCount(const QModelIndex & parent = QModelIndex()) const override;

  QModelIndex indexForTID(int tid) const;

  static const std::map<int,std::vector<IddObjectType> > tidToOSTypeMap;

  private:

  QDomDocument m_document;

  TIDItem * m_rootItem;

  static const std::map<int,std::vector<IddObjectType> > createTidToOSTypeMap();
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_TIDITEMMODEL_HPP
