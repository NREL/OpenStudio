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

#ifndef OPENSTUDIO_COLLAPSIBLEINSPECTOR_HPP
#define OPENSTUDIO_COLLAPSIBLEINSPECTOR_HPP

#include <QAbstractButton>
#include <QWidget>

class QLabel;
class QPaintEvent;

namespace openstudio {

class CollapsibleInspectorHeader;

class CollapsibleInspector : public QWidget
{
  Q_OBJECT

public:

  CollapsibleInspector(QString text,
    QWidget * inspector,
    QWidget * parent = 0);

  virtual ~CollapsibleInspector() {}

private:

  void createLayout();

  CollapsibleInspectorHeader * m_header;

  QWidget * m_inspector;

public slots:

  void on_headerToggled(bool checked);

};

class CollapsibleInspectorHeader : public QAbstractButton
{
  Q_OBJECT

public:

  CollapsibleInspectorHeader(QString text,
    QWidget * parent = 0);

  void setChecked(bool isChecked);

protected:

  void paintEvent(QPaintEvent * event);

  QSize sizeHint() const;

private:

  void createLayout();

  void setImage(bool isChecked);

  QString m_text;

  QLabel * m_arrowLabel;

private slots:

  void on_toggled(bool checked);

};

} // openstudio

#endif // OPENSTUDIO_COLLAPSIBLEINSPECTOR_HPP

