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

#ifndef MODELEDITOR_LISTWIDGET_HPP
#define MODELEDITOR_LISTWIDGET_HPP

#include <QListWidget>

namespace modeleditor
{

class ListWidget : public QListWidget
{
  Q_OBJECT

public:
  ListWidget(QWidget * parent = nullptr);
  virtual ~ListWidget();

  static const QString& getMimeType(){return mMimeType;}

public slots:

signals:

protected:
  virtual QMimeData * mimeData(const QList<QListWidgetItem *> items) const;

private:
  static QString mMimeType;

};

} // namespace modeleditor

#endif // MODELEDITOR_LISTWIDGET_HPP
