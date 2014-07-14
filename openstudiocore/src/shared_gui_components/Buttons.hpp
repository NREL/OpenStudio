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

#ifndef SHAREDGUICOMPONENTS_BUTTONS_HPP
#define SHAREDGUICOMPONENTS_BUTTONS_HPP

#include <QPushButton>

#include <boost/smart_ptr.hpp>

class QPaintEvent;

class QPixmap;

class QTimer;

namespace openstudio {

class ZoomOutButton : public QPushButton
{
  Q_OBJECT

  public:

  ZoomOutButton(QWidget * parent = nullptr);
  virtual ~ZoomOutButton() {}
};

class OrangeButton : public QPushButton
{
  Q_OBJECT

  public:

  OrangeButton(QWidget * parent = nullptr);
  virtual ~OrangeButton() {}
};

class GrayButton : public QPushButton
{
  Q_OBJECT

  public:

  GrayButton(QWidget * parent = nullptr);
  virtual ~GrayButton() {}
};

class BlueButton : public QPushButton
{
  Q_OBJECT

  public:

  BlueButton(QWidget * parent = nullptr);
  virtual ~BlueButton() {}
};

class RemoveButton : public QPushButton
{
  Q_OBJECT
  
  public:

  RemoveButton(QWidget * parent = nullptr);
  virtual ~RemoveButton() {}
};

class SofterRemoveButton : public QPushButton
{
  Q_OBJECT
  
  public:

  SofterRemoveButton(QWidget * parent = nullptr);
  virtual ~SofterRemoveButton() {}
};


class AddScriptButton : public QPushButton
{
  Q_OBJECT
  
  public:

  AddScriptButton(QWidget * parent = nullptr);
  virtual ~AddScriptButton() {}
};

class DuplicateButton : public QPushButton
{
  Q_OBJECT

  public:

  DuplicateButton(QWidget * parent = nullptr);
  virtual ~DuplicateButton() {}
};

class SofterDuplicateButton : public QPushButton
{
  Q_OBJECT
 
 public:
  
  SofterDuplicateButton(QWidget * parent = nullptr);
  virtual ~SofterDuplicateButton() {}
};

class UpButton : public QPushButton
{
  Q_OBJECT

  public:

  UpButton(QWidget * parent = nullptr);
  virtual ~UpButton() {}
};

class DownButton : public QPushButton
{
  Q_OBJECT

  public:

  DownButton(QWidget * parent = nullptr);
  virtual ~DownButton() {}
};

class MyMeasuresFolderButton : public QPushButton
{
  Q_OBJECT

  public:

  MyMeasuresFolderButton(QWidget * parent = nullptr);
  virtual ~MyMeasuresFolderButton() {}
};

class OpenDirectoryButton : public QPushButton
{
  Q_OBJECT

  public:

  OpenDirectoryButton(QWidget * parent = nullptr);
  virtual ~OpenDirectoryButton() {}
};

class CreateOneWithSelectedMeasuresButton : public GrayButton
{
  Q_OBJECT

  public:

  CreateOneWithSelectedMeasuresButton();
  virtual ~CreateOneWithSelectedMeasuresButton() {}
};

class CreateOneForEachSelectedMeasureButton : public GrayButton
{
  Q_OBJECT

  public:

  CreateOneForEachSelectedMeasureButton();
  virtual ~CreateOneForEachSelectedMeasureButton() {}
};

class CreateFromFileButton : public GrayButton
{
  Q_OBJECT

  public:

  CreateFromFileButton();
  virtual ~CreateFromFileButton() {}
};

class CloudLostConnectionButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudLostConnectionButton(QWidget * parent = nullptr);
  virtual ~CloudLostConnectionButton() {}
};

class CloudOffButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudOffButton(QWidget * parent = nullptr);
  virtual ~CloudOffButton() {}
};

class CloudOnButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudOnButton(QWidget * parent = nullptr);
  virtual ~CloudOnButton() {}
};

class CloudStartingButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudStartingButton(QWidget * parent = nullptr);
  virtual ~CloudStartingButton() {}

  public slots:

  void rotate();

  protected:

  void paintEvent ( QPaintEvent * event );

  private:

  QPixmap * m_background;

  QPixmap * m_arrow;

  float m_rotation;

  QTimer * m_timer;
};

class CloudStoppingButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudStoppingButton(QWidget * parent = nullptr);
  virtual ~CloudStoppingButton() {}

  public slots:

  void rotate();

  protected:

  void paintEvent ( QPaintEvent * event );

  private:

  QPixmap * m_background;

  QPixmap * m_arrow;

  float m_rotation;

  QTimer * m_timer;

};

class PlayButton : public QPushButton
{
  Q_OBJECT

  public:

  enum Status { IDLE, IDLEDISABLED, STARTING, RUNNING, STOPPING, ERROR };

  PlayButton(QWidget * parent = nullptr);
  virtual ~PlayButton() {}

  Status status() const;
  void setStatus(const Status & status);

  private:

  Status m_status;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_BUTTONS_HPP

