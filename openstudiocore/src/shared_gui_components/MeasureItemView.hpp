/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef SHAREDGUICOMPONENTS_MEASUREITEMVIEW_HPP
#define SHAREDGUICOMPONENTS_MEASUREITEMVIEW_HPP

#include "OSCollapsibleView.hpp"
#include "OSListView.hpp"
#include "HeaderViews.hpp"
#include <QAbstractButton>

class QLineEdit;
class QLabel;
class QPushButton;

namespace openstudio {

class MeasureType;

namespace measuretab {

class MeasuresView;
class VariableItemView;
class NewGroupDropZone;
class NewGroupView;
class VariableGroupContentView;
class MeasureDropZone;
class VariableHeaderView;
class VariableContentView;
class MeasureDropZone;
class MeasureItemButton;

} // measuretab

namespace measuretab {

// RectangularDropZone styles a rectangular drop zone
class RectangularDropZone : public QWidget
{
  Q_OBJECT
  
  public:

  RectangularDropZone();

  virtual ~RectangularDropZone() {}

  void setAcceptedMimeType(const QString & type);

  QLabel * nameLabel;

  signals:

  void dataDropped(QDropEvent * event);

  protected:

  void paintEvent(QPaintEvent *) override;

  void dropEvent(QDropEvent * event) override;

  void dragEnterEvent(QDragEnterEvent * event) override;

  private:

  QString m_acceptedMimeType;
};

// MeasureItemView displays a MeasureItem (individual measure)
// It is configured by MeasureItemDelegate
class MeasureItemView : public QWidget
{
  Q_OBJECT

public:

  MeasureItemView(bool t_fixed);

  virtual ~MeasureItemView() {}

  MeasureItemButton * measureItemButton;

  QPushButton * duplicateButton;

  QPushButton * removeButton;

protected:

  void paintEvent(QPaintEvent *) override;

};

class MeasureDropZone : public RectangularDropZone
{
  Q_OBJECT

  public:

  MeasureDropZone();

  virtual ~MeasureDropZone() {}
};

class MeasureItemButton : public QAbstractButton
{
  Q_OBJECT

  public:

  MeasureItemButton(bool t_fixed);

  virtual ~MeasureItemButton() {}

  QLabel * nameLabel;

  QLabel * cautionLabel;

  public slots:

  void setHasEmphasis(bool hasEmphasis);

  protected:

  void paintEvent(QPaintEvent * e) override;
};


} // measuretab

} // openstudio

#endif // SHAREDGUICOMPONENTS_MEASUREITEMVIEW_HPP
