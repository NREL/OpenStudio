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

#ifndef OPENSTUDIO_STARTUPVIEW_HPP
#define OPENSTUDIO_STARTUPVIEW_HPP

#include "../openstudio_lib/SOConstants.hpp"
#include <boost/smart_ptr.hpp>
#include <QWidget>
#include <QPoint>
#include <QAbstractListModel>

class QListView;

namespace openstudio {

class TemplateListModel;

class StartupView : public QWidget
{
  Q_OBJECT

  public:

  StartupView(QWidget * parent = nullptr);

  virtual ~StartupView() {}

  QSize sizeHint() const override;

  signals:

  void newFromTemplate( NewFromTemplateEnum );

  void openClicked();

  void importClicked();

  void importSDDClicked();

  protected:

  void mouseMoveEvent(QMouseEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;

  void resizeEvent(QResizeEvent * event) override;

  void paintEvent(QPaintEvent *event) override;

  private slots:

  void newFromTemplateSlot();

  void showDetailsForItem( const QModelIndex & index );

  private:

  QPoint dragPosition;

  bool _move;

  std::shared_ptr<TemplateListModel> m_templateListModel;

  QWidget * m_projectDetailView;

  QListView * m_listView;

};

class TemplateListModel : public QAbstractListModel
{
  public:

  TemplateListModel(QObject * parent = nullptr);

  int rowCount( const QModelIndex & parent = QModelIndex() ) const override;

  QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  private:

};

} // openstudio

#endif // OPENSTUDIO_STARTUPVIEW_HPP

