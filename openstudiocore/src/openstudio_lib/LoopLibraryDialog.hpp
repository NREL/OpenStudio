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

#ifndef OPENSTUDIO_LOOPLIBRARYDIALOG_HPP
#define OPENSTUDIO_LOOPLIBRARYDIALOG_HPP

#include "SOConstants.hpp"
#include <QDialog>
#include <QPixmap>
#include <QString>
#include <boost/optional.hpp>

class QScrollArea;

namespace openstudio {

class LoopLibraryDialog : public QDialog
{
  Q_OBJECT

  public:

  LoopLibraryDialog(QWidget * parent = 0);

  virtual ~LoopLibraryDialog() {}

  void newItem( const AddToModelEnum & addToModelEnum,
                const QString & detailedText,
                const QPixmap & pixmap );

  boost::optional<AddToModelEnum> addToModelEnum() const;

  protected:

  void paintEvent ( QPaintEvent * event );

  private slots:

  void onAddToModelClicked( AddToModelEnum addToModelEnum );

  private:

  QScrollArea * m_scrollArea;

  boost::optional<AddToModelEnum> m_addToModelEnum;
};

class LoopItemView : public QWidget
{
  Q_OBJECT

  public:

  LoopItemView( const AddToModelEnum & addToModelEnum,
                const QString & detailedText,
                const QPixmap & pixmap,
                QWidget * parent = 0 );

  virtual ~LoopItemView() {}

  //void paintEvent ( QPaintEvent * event );

  signals:

  void addToModelClicked( AddToModelEnum );

  private slots:

  void emitAddToModelClicked();
  
  private:

  AddToModelEnum m_addToModelEnum;

  QString m_detailedText;

  QPixmap m_pixmap;
};

} // namespace openstudio

#endif // OPENSTUDIO_LOOPLIBRARYDIALOG_HPP

