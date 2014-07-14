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

#ifndef SHAREDGUICOMPONENTS_OSDIALOG_HPP
#define SHAREDGUICOMPONENTS_OSDIALOG_HPP

#include <QDialog>

class QMargins;

class QPoint;

class QPushButton;

class QSize;

class QVBoxLayout;

namespace openstudio {

class OSDialog : public QDialog
{
  Q_OBJECT

public:

  OSDialog(bool isIP = false,
           QWidget * parent = nullptr);

  virtual ~OSDialog() {}

  //QSize sizeHint() const;

  //void setSizeHint(const QSize & sizeHint);

  QMargins layoutContentsMargins() const;

  void setLayoutContentsMargins(const QMargins & layoutContentsMargins);

  QVBoxLayout * upperLayout();

  QPushButton * backButton();

  QPushButton * cancelButton();

  QPushButton * okButton();

protected:

  void setOkButtonAsDefault(bool isDefault);

  void mouseMoveEvent(QMouseEvent *event);

  void mouseReleaseEvent(QMouseEvent *event);
  
  void mousePressEvent(QMouseEvent *event);

  void resizeEvent(QResizeEvent * event);

  void paintEvent(QPaintEvent *event);

  bool m_isIP;

  QPushButton * m_backButton;

  QPushButton * m_cancelButton;

  QPushButton * m_okButton;

private:

  virtual void createLayout();

  QVBoxLayout * m_upperLayout;

  QSize m_sizeHint;

  QMargins m_layoutContentsMargins;

  QPoint dragPosition;

  bool _move;

signals:

  void backButtonClicked(bool checked);

  void cancelButtonClicked(bool checked);

  void okButtonClicked(bool checked);

  void toggleUnitsClicked(bool displayIP);

protected slots:

  virtual void on_backButton(bool checked); 
  
  virtual void on_cancelButton(bool checked);

  virtual void on_okButton(bool checked);

  virtual void toggleUnits(bool displayIP);

};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSDIALOG_HPP
