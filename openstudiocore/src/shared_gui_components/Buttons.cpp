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

#include "Buttons.hpp"
#include <QString>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QTimer>

namespace openstudio {

ZoomOutButton::ZoomOutButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(20,20);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':images/zoom-out.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':images/zoom-out-over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':images/zoom-out-press.png');"
               "}");

  setStyleSheet(style);
}

OrangeButton::OrangeButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  QString style;
  style.append("QPushButton { ");
  style.append("border-radius: 4px; ");
  style.append("border: 1px solid #949393; ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #FDBC3B, "); 
  style.append("                             stop: 1 #F47920 ); ");
  style.append("height: 22px; ");
  style.append("font-size: 12px; ");
  style.append("margin: 0; ");
  style.append("padding-left: 10px; ");
  style.append("padding-right: 10px; ");
  style.append("} ");

  setStyleSheet(style);
}

GrayButton::GrayButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  QString style;
  style.append("QPushButton { ");
  style.append("border-radius: 4px; ");
  style.append("border: 1px solid #949393; ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #F2F2F2, "); 
  style.append("                             stop: 1 #A5A5A5 ); ");
  style.append("height: 22px; ");
  style.append("font-size: 12px; ");
  style.append("margin: 0; ");
  style.append("padding-left: 10px; ");
  style.append("padding-right: 10px; ");
  style.append("} ");

  style.append("QPushButton:pressed { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  style.append("QPushButton:checked { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  setStyleSheet(style);
}

BlueButton::BlueButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  QString style;
  style.append("QPushButton { ");
  style.append("border-radius: 4px; ");
  style.append("border: 1px solid #093A07; ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #C8DAF1, "); 
  style.append("                             stop: 1 #39679E ); ");
  style.append("height: 22px; ");
  style.append("font-size: 12px; ");
  style.append("margin: 0; ");
  style.append("padding-left: 10px; ");
  style.append("padding-right: 10px; ");
  style.append("} ");

  style.append("QPushButton:pressed { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  style.append("QPushButton:checked { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  setStyleSheet(style);
}

SofterRemoveButton::SofterRemoveButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(16,16);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/delete_softer.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/delete_softer_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/delete_softer_press.png');"
               "}");

  setStyleSheet(style);
}

AddScriptButton::AddScriptButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(24,24);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/add_script_icon.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/add_script_icon_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/add_script_icon_press.png');"
               "}");

  setStyleSheet(style);
}

DuplicateButton::DuplicateButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(24,24);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/duplicate_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/duplicate_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/duplicate_press.png');"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/duplicate_disabled.png');"
               "}");

  setStyleSheet(style);
}

SofterDuplicateButton::SofterDuplicateButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(16,16);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_press.png');"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_disabled.png');"
               "}");

  setStyleSheet(style);
}


UpButton::UpButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(22,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/arrow_up.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/arrow_up_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/arrow_up_press.png');"
               "}");

  setStyleSheet(style);
}

DownButton::DownButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(22,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/arrow_down.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/arrow_down_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/arrow_down_press.png');"
               "}");

  setStyleSheet(style);
}

MyMeasuresFolderButton::MyMeasuresFolderButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(31,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/open_my_measures.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/open_my_measures_press.png');"
               "}");

  setStyleSheet(style);
}

OpenDirectoryButton::OpenDirectoryButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(31,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/dir.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/dir_press.png');"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/dir_disabled.png');"
               "}");

  setStyleSheet(style);
}

CreateOneWithSelectedMeasuresButton::CreateOneWithSelectedMeasuresButton()
  : GrayButton()
{
  setText("Create One With Selected Measures");
}

CreateOneForEachSelectedMeasureButton::CreateOneForEachSelectedMeasureButton()
  : GrayButton()
{
  setText("Create One For Each Selected Measure");
}

CreateFromFileButton::CreateFromFileButton()
  : GrayButton()
{
  setText("Create From External File");
}

CloudLostConnectionButton::CloudLostConnectionButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/cloud_lost_connection.png');"
               "  border:none;"
               "}");

  setStyleSheet(style);

}
CloudOffButton::CloudOffButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/cloud_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/cloud_off_disabled.png');"
               "}");

  setStyleSheet(style);
}

CloudOnButton::CloudOnButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/cloud_on.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/cloud_on_disabled.png');"
               "}");

  setStyleSheet(style);
}

CloudStartingButton::CloudStartingButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  border:none;"
               "}");

  setStyleSheet(style);

  m_background = new QPixmap(":/shared_gui_components/images/cloud_starting.png");
  m_arrow = new QPixmap(":/shared_gui_components/images/cloud_arrow.png");

  m_rotation = 0.0;

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &CloudStartingButton::rotate);
  m_timer->start(50);
}

void CloudStartingButton::rotate()
{
  m_rotation += 10.0;

  update();
}

void CloudStartingButton::paintEvent ( QPaintEvent * event )
{
  QPainter painter(this);

  double transX = m_arrow->width()/2.0;
  double transY = m_arrow->height()/2.0;

  QTransform t;
  t.translate(transX, transY);
  t.rotate(m_rotation);
  t.translate(-transX, -transY);

  QPixmap rotatedArrow = QPixmap(m_arrow->transformed(t, Qt::SmoothTransformation));

  double transX2 = rotatedArrow.width()/2.0;
  double transY2 = rotatedArrow.height()/2.0;

  painter.drawPixmap(0, 0, *m_background);
  painter.drawPixmap(12+transX-transX2, 5+transY-transY2, rotatedArrow);
}

CloudStoppingButton::CloudStoppingButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  border:none;"
               "}");

  setStyleSheet(style);

  m_background = new QPixmap(":/shared_gui_components/images/cloud_stopping.png");
  m_arrow = new QPixmap(":/shared_gui_components/images/cloud_arrow_magenta.png");

  m_rotation = 0.0;

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &CloudStoppingButton::rotate);
  m_timer->start(50);
}

void CloudStoppingButton::rotate()
{
  m_rotation += 10.0;

  update();
}

void CloudStoppingButton::paintEvent ( QPaintEvent * event )
{
  QPainter painter(this);

  double transX = m_arrow->width()/2.0;
  double transY = m_arrow->height()/2.0;

  QTransform t;
  t.translate(transX, transY);
  t.rotate(m_rotation);
  t.translate(-transX, -transY);

  QPixmap rotatedArrow = QPixmap(m_arrow->transformed(t, Qt::SmoothTransformation));

  double transX2 = rotatedArrow.width()/2.0;
  double transY2 = rotatedArrow.height()/2.0;

  painter.drawPixmap(0, 0, *m_background);
  painter.drawPixmap(12+transX-transX2, 5+transY-transY2, rotatedArrow);
}

PlayButton::PlayButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);
  setFixedSize(35,35);

  setStatus(IDLE);
}

PlayButton::Status PlayButton::status() const
{
  return m_status;
}

void PlayButton::setStatus(const Status & status)
{
  QString style;

  switch (status)
  {
    case IDLE:
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_button.png');"
                   "  border:none;"
                   "}");
      style.append("QPushButton:hover {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_over.png');"
                   "}");
      style.append("QPushButton:pressed {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_press.png');"
                   "}");
      setStyleSheet(style);
      setEnabled(true);
      break;
    case IDLEDISABLED:
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_button_disabled.png');"
                   "  border:none;"
                   "}");
      setStyleSheet(style);
      setEnabled(false);
      break;
    case STARTING:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/starting_simulation_button.png');"
                   "  border:none;"
                   "}");

      setStyleSheet(style);
      setEnabled(false);
      break;
    case RUNNING:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_cancel.png');"
                   "  border:none;"
                   "}");
      style.append("QPushButton:hover {"
                   "  background-image:url(':/shared_gui_components/images/run_cancel_over.png');"
                   "}");
      style.append("QPushButton:pressed {"
                   "  background-image:url(':/shared_gui_components/images/run_cancel_press.png');"
                   "}");

      setStyleSheet(style);
      setEnabled(true);
      break;
    case STOPPING:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/stopping_simulation_button.png');"
                   "  border:none;"
                   "}");

      setStyleSheet(style);
      setEnabled(false);
      break;
    case ERROR:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_button.png');"
                   "  border:none;"
                   "}");
      style.append("QPushButton:hover {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_over.png');"
                   "}");
      style.append("QPushButton:pressed {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_press.png');"
                   "}");
      setStyleSheet(style);
      setEnabled(true);
      break;
  }

  m_status = status;
}


} // openstudio

