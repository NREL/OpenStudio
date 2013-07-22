/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_UTILITYBILLSTABVIEW_H
#define OPENSTUDIO_UTILITYBILLSTABVIEW_H

#include <openstudio_lib/MainTabView.hpp>

#include "../shared_gui_components/OSDialog.hpp"

#include <model/Model.hpp>

#include <QWidget>

class QButtonGroup;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QPushButton;
class QScrollArea;

namespace openstudio {

class OSDoubleEdit;
class OSLineEdit;
class OSIntegerEdit;

class BillFormatDialog;

namespace model {
  class Model;
}

enum BillFormat{
  STARTDATE_ENDDATE,
  STARTDATE_NUMDAYS,
  ENDDATE_NUMDAYS
};

class UtilityBillsView : public QWidget
{
  Q_OBJECT

public:

  UtilityBillsView(const model::Model & model);
  
  virtual ~UtilityBillsView() {}

private:

  void createWidgets();
  void showBillFormatDialog();
  void showGroupBoxIfBills();
  void getSortedBills();
  void showAddButton();
  void hideAddButton();
  void enableAddButton();
  void disableAddButton();

  model::Model m_model;

  BillFormatDialog * m_billFormatDialog;

  BillFormat m_billFormat;

  bool m_showPeak;
  
  QButtonGroup * m_buttonGroup;

  OSLineEdit * m_name;
  OSLineEdit * m_consumptionUnits;
  OSLineEdit * m_energyDemandUnits;
  OSLineEdit * m_weatherFile;

  QPushButton * m_addNewBill;

  QGridLayout * m_beforeRangeGridLayout;
  QGridLayout * m_inRangeGridLayout;
  QGridLayout * m_afterRangeGridLayout;

  QGroupBox * m_groupBox;

  QScrollArea * m_scrollArea;

  std::vector<double> m_sortedBills;
  
private slots:
  
  void addBill(bool checked);
  void deleteBill(int index);
  void setBillFormat(BillFormat billFormat);

};

class UtilityBillsTabView : public MainTabView
{

  Q_OBJECT

public:

  UtilityBillsTabView(const model::Model & model,
                        QWidget * parent = 0);

  virtual ~UtilityBillsTabView() {}

private:

  UtilityBillsView * m_utilityBillsView;

};

class UtilityBillWidget : public QWidget
{

  Q_OBJECT

public:

  UtilityBillWidget(QGridLayout * gridLayout,
    BillFormat billFormat,
    bool showPeak,
    QWidget * parent = 0);

  virtual ~UtilityBillWidget() {}

  void createWidgets(QGridLayout * gridLayout,
    BillFormat billFormat,
    bool showPeak);

  void getStartDate(QGridLayout * gridLayout, int & rowIndex, int & columnIndex);
  void getEndDate(QGridLayout * gridLayout, int & rowIndex, int & columnIndex);
  void getBillingPeriod(QGridLayout * gridLayout, int & rowIndex, int & columnIndex);

private:

  QLabel * m_startDateLbl;
  QDateEdit * m_startDateEdit;

  QLabel * m_billingPeriodIntLbl;
  OSIntegerEdit * m_billingPeriodIntEdit;

  QLabel * m_endDateLbl;
  QDateEdit * m_endDateEdit;

  OSDoubleEdit * m_energyUseDoubleEdit;
  OSDoubleEdit * m_peaklDoubleEdit;
  OSDoubleEdit * m_costDoubleEdit;
  QPushButton * m_deleteBillWidget;

private slots:

  void setDstStartDate(const QDate & newdate);
  void setDstEndDate(const QDate & newdate);

};

class BillFormatDialog : public OSDialog
{
  Q_OBJECT

public:

  BillFormatDialog(QWidget * parent = 0);

  virtual ~BillFormatDialog() {}

  BillFormat billFormat(){return m_billFormat;}

private:

  virtual void createLayout();

  BillFormat m_billFormat;

signals:

  void billFormatSignal(BillFormat);

private slots:

  void on_okButton(bool checked);

  void setBillFormat(int index);

};

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLSTABVIEW_H
