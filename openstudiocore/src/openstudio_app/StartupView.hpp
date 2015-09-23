/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

