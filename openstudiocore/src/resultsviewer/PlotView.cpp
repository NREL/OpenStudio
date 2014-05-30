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

#include "PlotView.hpp"
#include "PlotViewProperties.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/data/Vector.hpp"

#include <qwt/qwt_symbol.h>
#include <qwt/qwt_data.h>
#include <qwt/qwt_scale_engine.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <cfloat>
#include <QPrinter>
#include <QPrintDialog>
#include <QMessageBox>
#include <QCursor>

using namespace openstudio;

namespace resultsviewer{


  LinePlotCurve::LinePlotCurve(QString& title, openstudio::TimeSeriesLinePlotData& data)
  {
    setTitle(title);
    m_yType = resultsviewer::unScaledY;
    setLinePlotStyle(resultsviewer::smoothLinePlot);
    setLinePlotData(data);
  }


  void LinePlotCurve::setLinePlotData(const openstudio::LinePlotData& data)
  {
    if (data.size() <=0) return;

    size_t i;
    m_xValues.resize(data.size());
    m_yUnscaled.resize(data.size());
    for(i=0; i<data.size(); i++)
    {
      m_xValues[i] = data.x(i);
      m_yUnscaled[i] = data.y(i);
    }
    setLinePlotStyle(resultsviewer::smoothLinePlot);
  }


  void LinePlotCurve::setDataMode(YValueType yType)
  {
    switch (yType)
    {
    case resultsviewer::unScaledY:
      setData(m_xValues, m_yUnscaled);
      m_yType = yType;
      break;
    case resultsviewer::scaledY:
      setData(m_xValues, m_yScaled);
      m_yType = yType;
      break;
    }
  }

  void LinePlotCurve::setLinePlotStyle(LinePlotStyleType lineStyle)
  {
    switch (lineStyle)
    {
    case smoothLinePlot:
      setDataMode(m_yType);
      setStyle(QwtPlotCurve::Lines);
      m_linePlotStyle = lineStyle;
      break;
    case stairLinePlot:
      setDataMode(m_yType);
      setStyle(QwtPlotCurve::Steps);
      m_linePlotStyle = lineStyle;
      break;
    case barLinePlot:
      setDataMode(m_yType);
      setStyle(QwtPlotCurve::Sticks);
      m_linePlotStyle = lineStyle;
      break;
    }
  }

  PlotLegend::PlotLegend(PlotView *parent): QWidget(parent)
  {
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_plotView = parent;
    m_mainLayout = new QVBoxLayout;
    m_width = 50; // default starting width
    setLayout(m_mainLayout);
    if (m_plotView)
    {
      m_x = m_plotView->plot()->canvas()->x() + 100;
      m_y = m_plotView->plot()->canvas()->y() + 100;
      updateLegend();
    }
  }


  void PlotLegend::clearLegend(QLayout *li)
  {
    QLayoutItem *child;
    while ((child = li->takeAt(0)) != nullptr)
    {
      if (child->widget())
        delete child->widget();
      else
        clearLegend((QLayout *)child);
      delete child;
    }
  }

  void PlotLegend::paintEvent(QPaintEvent *evt)
  {
    int h = sizeHint().height();
    int w = sizeHint().width();
    adjustSize();
    if (sizeHint().height() > h) h = sizeHint().height();
    if (sizeHint().width() > w) w = sizeHint().width();
    setGeometry(m_x,m_y,w,h);
  }


  void PlotLegend::refreshLegend()
  {
    clearLegend(m_mainLayout);
    updateLegend();
  }


  void PlotLegend::updateLegend()
  {
    if (m_plotView)
    {
      /// multiple curves based on units
      const QwtPlotItemList &listPlotItem = m_plotView->plot()->itemList();
      QwtPlotItemIterator itPlotItem;
      for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
      {
        QwtPlotItem *plotItem = *itPlotItem;
        if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
          addLegendItem((LinePlotCurve*)plotItem);
      }
    }
  }

  void PlotLegend::addLegendItem(resultsviewer::LinePlotCurve *curve)
  {
    // update with paint event and curve styles
    auto curveStyle = new QLabel(this);
    auto curveLabel = new QLabel(this);
    QPen pen(curve->pen());
    QPalette p(curveStyle->palette());
    p.setColor(QPalette::WindowText, pen.color());
    p.setColor(QPalette::Foreground, pen.color());
    curveStyle->setPalette(p);
    curveStyle->setMaximumWidth(fontMetrics().width("_____"));
    curveStyle->setAlignment(Qt::AlignTop);
    curveStyle->setText("_____");
    curveLabel->setText(curve->title().text());
    int titleWidth = curveStyle->width() + curveLabel->width();
    if (titleWidth > m_width) m_width = titleWidth;
    auto legendItem = new QHBoxLayout;
    legendItem->addWidget(curveStyle);
    legendItem->addWidget(curveLabel);
    m_mainLayout->addLayout(legendItem);
  }

  void PlotLegend::mousePressEvent(QMouseEvent *e)
  {
    if (e->button() == Qt::LeftButton) m_startDragPos = e->pos();
  }

  void PlotLegend::mouseMoveEvent(QMouseEvent *e)
  {
    if (e->buttons() & Qt::LeftButton)
    {
      int distance = (e->pos() - m_startDragPos).manhattanLength();
      if (distance >= QApplication::startDragDistance()) performDrag(e->globalPos());
    }
  }

  void PlotLegend::performDrag(QPoint pos)
  {
    QPoint posWidget = parentWidget()->mapFromGlobal(QCursor::pos());
    m_x = posWidget.x();
    m_y = posWidget.y() - height()/2;
    move(m_x,m_y);
  }

  PlotViewMimeData::PlotViewMimeData(std::vector<PlotViewData>& _plotViewDataVec)
  {
    m_plotViewDataVec = _plotViewDataVec;
  }

  PlotViewToolbar::PlotViewToolbar(const QString &title, QWidget *parent):QToolBar(title, parent)
  {
  }


  void PlotViewToolbar::mouseDoubleClickEvent(QMouseEvent *evt)
  {
    emit(signalDoubleClick());
  }

  /// PlotView - results viewer container for plots
  PlotView::PlotView(int plotType, QWidget* parent): QWidget(parent),
    m_illuminanceMapRefPt1(nullptr),
    m_illuminanceMapRefPt2(nullptr),
    m_lastImageSavedPath(QApplication::applicationDirPath()),
    m_plot(nullptr),
    m_menuBar(nullptr),
    m_toolBar(nullptr),
    m_centerSlider(nullptr),
    m_centerDate(nullptr),
    m_spanSlider(nullptr),
    m_centerSpinBox(nullptr),
    m_spanSpinBox(nullptr),
    m_plotType(plotType),
    m_picker(nullptr),
    m_valueInfo(nullptr),
    m_valueInfoMarker(nullptr),
    m_panner(nullptr),
    m_plotViewTimeAxis(nullptr),
    m_grid(nullptr),
    m_legend(nullptr),
    m_spectrogram(nullptr),
    m_rightAxis(nullptr),
    m_spinDay(nullptr),
    m_spinHour(nullptr),
    m_noData(nullptr)
  {
    init();
  }

  PlotView::PlotView(QString& path, int plotType, QWidget* parent): QWidget(parent),
    m_illuminanceMapRefPt1(nullptr),
    m_illuminanceMapRefPt2(nullptr),
    m_lastImageSavedPath(path.isEmpty()?QApplication::applicationDirPath():path),
    m_plot(nullptr),
    m_menuBar(nullptr),
    m_toolBar(nullptr),
    m_centerSlider(nullptr),
    m_centerDate(nullptr),
    m_spanSlider(nullptr),
    m_centerSpinBox(nullptr),
    m_spanSpinBox(nullptr),
    m_plotType(plotType),
    m_picker(nullptr),
    m_valueInfo(nullptr),
    m_valueInfoMarker(nullptr),
    m_panner(nullptr),
    m_plotViewTimeAxis(nullptr),
    m_grid(nullptr),
    m_legend(nullptr),
    m_spectrogram(nullptr),
    m_rightAxis(nullptr),
    m_spinDay(nullptr),
    m_spinHour(nullptr),
    m_noData(nullptr)
  {
    init();
  }


  void PlotView::init()
  {
    setAcceptDrops(true);
    setAttribute(Qt::WA_DeleteOnClose);

    // createMenuBar();
    createQwtPlot();

    createToolBar();

    createLayout();

    m_plotViewTimeAxis = nullptr;

    m_leftAxisUnits = "NONE SPECIFIED";
    m_rightAxisUnits = "NONE SPECIFIED";

    // autoscaling on by default
    m_yLeftAutoScale = true;
    m_yRightAutoScale = true;

    m_xAxisMin = DBL_MAX;
    m_xAxisMax = -DBL_MAX;


    m_colorVec.push_back(Qt::black);
    m_colorVec.push_back(Qt::blue);
    m_colorVec.push_back(Qt::cyan);
    m_colorVec.push_back(Qt::magenta);
    m_colorVec.push_back(Qt::yellow);
    m_colorVec.push_back(Qt::darkBlue);
    m_colorVec.push_back(Qt::darkCyan);
    m_colorVec.push_back(Qt::darkMagenta);
    m_colorVec.push_back(Qt::darkYellow);


    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::white);
    m_plot->setPalette(p);

    QPen gridPen(Qt::gray);

    switch (m_plotType)
    {
    case RVPV_LINEPLOT:
      m_legend = new resultsviewer::PlotLegend(this);
      m_grid = new QwtPlotGrid();
      gridPen.setCosmetic(true);
      gridPen.setWidth(1);
      gridPen.setStyle(Qt::DotLine);
      m_grid->setPen(gridPen);
      m_grid->attach(m_plot);
      break;
    case RVPV_FLOODPLOT:
      m_floodPlotAutoScale = true;
      m_floodPlotMax = 0;
      m_floodPlotMin = 0;
      m_floodPlotYearlyMax = 0;
      m_floodPlotYearlyMin = 0;
      m_spectrogram = new QwtPlotSpectrogram();
      m_colorMapType = openstudio::FloodPlotColorMap::Jet;
      m_colorMapLength = 64;
      m_spectrogram->attach(m_plot);
      m_plot->plotLayout()->setAlignCanvasToScales(true);
      break;
    case RVPV_ILLUMINANCEPLOT:
      m_floodPlotAutoScale = true;
      m_floodPlotMax = 0;
      m_floodPlotMin = 0;
      m_floodPlotYearlyMax = 0;
      m_floodPlotYearlyMin = 0;
      m_spectrogram = new QwtPlotSpectrogram();
      m_colorMapType = openstudio::FloodPlotColorMap::Jet;
      m_colorMapLength = 64;
      m_spectrogram->attach(m_plot);
      m_plot->plotLayout()->setAlignCanvasToScales(true);
      break;
    }
  }

  void PlotView::createMenuBar()
  {
    m_menuBar = new QMenuBar(this);
    QAction *test = new QAction(tr("test"), this);
    m_menuBar->addAction(test);

  }

  void PlotView::createToolBar()
  {
    m_toolBar = new PlotViewToolbar("PlotToolBar",this);

    connect(m_toolBar, SIGNAL(signalDoubleClick()), this, SLOT(slotFloatOrDock()));

    auto exclusiveButtons = new QButtonGroup(this);
    exclusiveButtons->setExclusive(true);

    auto pointer = new QToolButton(this);
    pointer->setIcon(QIcon(":/images/select_tool.png"));
    pointer->setText("Select Mode");
    pointer->setToolTip("Select Mode");
    pointer->setCheckable(true);
    m_toolBar->addWidget(pointer);
    exclusiveButtons->addButton(pointer);
    connect(pointer, SIGNAL(toggled(bool)), this, SLOT(slotPointerMode(bool)));

    m_selectCursor = QCursor(Qt::ArrowCursor);

    auto pan = new QToolButton(this);
    pan->setIcon(QIcon(":/images/pan.png"));
    pan->setText("Pan Mode");
    pan->setToolTip("Pan Mode");
    pan->setCheckable(true);
    m_toolBar->addWidget(pan);
    exclusiveButtons->addButton(pan);
    connect(pan, SIGNAL(toggled(bool)), this, SLOT(slotPanMode(bool)));

    m_panCursor = QCursor(Qt::OpenHandCursor);

    auto valueInfo = new QToolButton(this);
    valueInfo->setIcon(QIcon(":/images/value_info.png"));
    valueInfo->setText("Value Info Mode");
    valueInfo->setToolTip("Value Info Mode");
    valueInfo->setCheckable(true);
    m_toolBar->addWidget(valueInfo);
    exclusiveButtons->addButton(valueInfo);
    connect(valueInfo, SIGNAL(toggled(bool)), this, SLOT(slotValueInfoMode(bool)));

    m_valueInfoCursor = QCursor(Qt::ArrowCursor);

    auto zoom = new QToolButton(this);
    zoom->setIcon(QIcon(":/images/rubberband_zoom.png"));
    zoom->setText("Zoom Mode");
    zoom->setToolTip("Zoom Mode");
    zoom->setCheckable(true);
    m_toolBar->addWidget(zoom);
    exclusiveButtons->addButton(zoom);
    connect(zoom, SIGNAL(toggled(bool)), this, SLOT(slotZoomMode(bool)));

    m_zoomCursor = QCursor(QPixmap(":/images/zoom_cursor.png"),0,0);

    QAction *zoomIn = new QAction(QIcon(":/images/zoom_in.png"),tr("Zoom In"), this);
    zoomIn->setToolTip("Zoom In");
    m_toolBar->addAction(zoomIn);
    connect(zoomIn, SIGNAL(triggered()), this, SLOT(slotZoomIn()));

    QAction *zoomOut = new QAction(QIcon(":/images/zoom_out.png"),tr("Zoom Out"), this);
    zoomOut->setToolTip("Zoom Out");
    m_toolBar->addAction(zoomOut);
    connect(zoomOut, SIGNAL(triggered()), this, SLOT(slotZoomOut()));


    auto saveMenu = new QMenu(this);
    QAction *saveImageScreenSize = new QAction(tr("Save Image (Screen Size)"), this);
    connect(saveImageScreenSize, SIGNAL(triggered()), this, SLOT(slotSaveImageScreenSize()));
    QAction *saveImage800x600 = new QAction(tr("Save Image (800x600)"), this);
    connect(saveImage800x600, SIGNAL(triggered()), this, SLOT(slotSaveImage800x600()));
    QAction *saveImage400x300 = new QAction(tr("Save Image (400x300)"), this);
    connect(saveImage400x300, SIGNAL(triggered()), this, SLOT(slotSaveImage400x300()));
    saveMenu->addAction(saveImageScreenSize);
    saveMenu->addAction(saveImage800x600);
    saveMenu->addAction(saveImage400x300);
    auto save = new QToolButton(this);
    save->setIcon(QIcon(":/images/save_plot_image.png"));
    save->setText("Save Image");
    save->setToolTip("Save Image");
    save->setMenu(saveMenu);
    save->setPopupMode(QToolButton::InstantPopup);
    m_toolBar->addWidget(save);

    QAction *print = new QAction(QIcon(":/images/print_plot.png"),tr("Print"), this);
    print->setToolTip("Print Image");
    m_toolBar->addAction(print);
    connect(print, SIGNAL(triggered()), this, SLOT(slotPrint()));

    QAction *properties = new QAction(QIcon(":/images/plot_preferences.png"),tr("Properties"), this);
    properties->setToolTip("Plot Properties");
    m_toolBar->addAction(properties);
    connect(properties, SIGNAL(triggered()), this, SLOT(slotProperties()));

    // default select tool
    pointer->setChecked(true);
  }

  void PlotView::createQwtPlot()
  {
    m_plot = new QwtPlot(this);

    QFont font;
    font.setFamily("Arial");
    font.setPointSize(12);

    QwtText title;
    title.setFont(font);
    m_plot->setTitle(title);

    font.setPointSize(12);
    title.setFont(font);
    m_plot->setAxisTitle(QwtPlot::xBottom, title);
    m_plot->setAxisTitle(QwtPlot::xTop, title);
    m_plot->setAxisTitle(QwtPlot::yLeft, title);
    m_plot->setAxisTitle(QwtPlot::yRight, title);

    m_plot->setMargin(5);


    m_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft, m_plot->canvas());
    m_zoomer[0]->setRubberBand(QwtPicker::RectRubberBand);
    m_zoomer[0]->setRubberBandPen(QColor(Qt::green));
    m_zoomer[0]->setTrackerMode(QwtPicker::ActiveOnly);
    m_zoomer[0]->setTrackerPen(QColor(Qt::blue));

    // trac 349 - zoomer double zooms when second zoomer created
    m_zoomer[1] = new Zoomer(QwtPlot::xTop, QwtPlot::yRight, m_plot->canvas());
    m_zoomer[1]->setEnabled(false);

    connect(m_zoomer[0], SIGNAL(zoomed(const QwtDoubleRect &)), this, SLOT(slotZoomed(const QwtDoubleRect &)));



    m_panner = new QwtPlotPanner(m_plot->canvas());
    m_panner->setMouseButton(Qt::LeftButton);

    m_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
      QwtPicker::PointSelection,
      QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly,
      m_plot->canvas());
    m_picker->setRubberBandPen(QColor(Qt::gray));
    m_picker->setRubberBand(QwtPicker::CrossRubberBand);
    m_picker->setTrackerPen(QColor(Qt::white));

    connect(m_picker, SIGNAL(appended(const QPoint &)), this,  SLOT(slotValueInfo(const QPoint &)));

    m_valueInfo = new QTextEdit(this);
    m_valueInfo->setReadOnly(true);
    m_valueInfo->setVisible(false);
    m_valueInfo->setMinimumWidth(fontMetrics().width("x=mm/yy hh::mm::ss, y = 999,999,999,999") + 20);
    m_valueInfo->setMinimumHeight(fontMetrics().height() * 3);


    m_valueInfoMarker = new QwtPlotMarker();
    m_valueInfoMarker->attach(m_plot);
    m_valueInfoMarker->setSymbol(QwtSymbol(QwtSymbol::Hexagon, QBrush(Qt::white), QPen(Qt::black,3), QSize(10,10)));
    m_valueInfoMarker->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);
    m_valueInfoMarker->hide();
    m_illuminanceMapRefPt1 = new QwtPlotMarker();
    m_illuminanceMapRefPt1->attach(m_plot);
    m_illuminanceMapRefPt1->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);
    m_illuminanceMapRefPt1->setLabel(QwtText("Ref1"));
    m_illuminanceMapRefPt1->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    m_illuminanceMapRefPt1->setSymbol(QwtSymbol(QwtSymbol::XCross, QBrush(Qt::white), QPen(Qt::black,2), QSize(7,7)));
    m_illuminanceMapRefPt1->hide();
    m_illuminanceMapRefPt2 = new QwtPlotMarker();
    m_illuminanceMapRefPt2->attach(m_plot);
    m_illuminanceMapRefPt2->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);
    m_illuminanceMapRefPt2->setLabel(QwtText("Ref2"));
    m_illuminanceMapRefPt2->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    m_illuminanceMapRefPt2->setSymbol(QwtSymbol(QwtSymbol::XCross, QBrush(Qt::white), QPen(Qt::black,2), QSize(7,7)));
    m_illuminanceMapRefPt2->hide();


    slotZoomMode(false);
    slotPanMode(false);
    slotValueInfoMode(false);

  }


  void PlotView::createLayout()
  {
    auto mainLayout = new QVBoxLayout;

    mainLayout->addWidget(m_toolBar);

    mainLayout->addWidget(m_plot);

    m_noData = new QLabel(this);
    m_noData->setGeometry(m_plot->x(),m_plot->y(),m_plot->width(),m_plot->height());
    QFont font(m_noData->font());
    font.setPointSize(20);
    m_noData->setFont(font);
    m_noData->setWordWrap(true);
    m_noData->setAlignment(Qt::AlignCenter);
    m_noData->setText("NO DATA");
    m_noData->hide();

    if (m_plotType == RVPV_ILLUMINANCEPLOT)
    {
      auto centerBoxLayout = new QHBoxLayout;
      QLabel *centerLabel = new QLabel(tr("Center:"), this);
      m_centerSlider = new QSlider(Qt::Horizontal, this);
      m_centerDate = new QLineEdit(tr("00/00 00:00:00"),this);
      m_centerDate->setInputMask("99/99 99:99:99");
      int boxWidth = fontMetrics().width(tr("00/00 00:00:00")) + 10;
      m_centerDate->setMaximumWidth(boxWidth);
      m_centerDate->setMinimumWidth(boxWidth);

      m_spinDay = new QSpinBox(this);
      m_spinDay->setMaximumWidth(15);
      m_spinDay->setFrame(false);
      m_spinDay->setRange(0,10000);
      m_spinDayPreviousValue = 500;
      m_spinDay->setValue(m_spinDayPreviousValue);
      m_spinHour = new QSpinBox(this);
      m_spinHour->setMaximumWidth(15);
      m_spinHour->setFrame(false);
      m_spinHour->setRange(0,10000);
      m_spinHourPreviousValue = 500;
      m_spinHour->setValue(m_spinHourPreviousValue);

      centerBoxLayout->addWidget(centerLabel);
      centerBoxLayout->addWidget(m_centerSlider);
      centerBoxLayout->addWidget(m_spinDay,0,Qt::AlignRight);
      centerBoxLayout->addWidget(m_centerDate);
      centerBoxLayout->addWidget(m_spinHour,0,Qt::AlignLeft);

      mainLayout->addLayout(centerBoxLayout);
      connect(m_centerSlider, SIGNAL(valueChanged(int)), this, SLOT(slotCenterIlluminance(int)));
      connect(m_centerDate, SIGNAL(editingFinished()), this, SLOT(slotIlluminanceDateChanged()));
      connect(m_spinDay, SIGNAL(valueChanged(int)), this, SLOT(slotSpinDayChanged(int)));
      connect(m_spinHour, SIGNAL(valueChanged(int)), this, SLOT(slotSpinHourChanged(int)));
    }
    else
    {
      auto centerBoxLayout = new QHBoxLayout;
      QLabel *centerLabel = new QLabel(tr("Center:"), this);
      m_centerSlider = new QSlider(Qt::Horizontal, this);
      m_centerSpinBox = new QDoubleSpinBox(this);

      centerBoxLayout->addWidget(centerLabel);
      centerBoxLayout->addWidget(m_centerSlider);
      centerBoxLayout->addWidget(m_centerSpinBox);


      mainLayout->addLayout(centerBoxLayout);
      connect(m_centerSlider, SIGNAL(valueChanged(int)), this, SLOT(slotCenterSliderToDouble(int)));
      connect(m_centerSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotCenterSpinBoxToInt(double)));
      connect(m_centerSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotCenterValue(double)));

      auto spanBoxLayout = new QHBoxLayout;

      QLabel *spanLabel = new QLabel(tr("Span:"), this);
      m_spanSlider = new QSlider(Qt::Horizontal, this);
      m_spanSpinBox = new QDoubleSpinBox(this);

      spanBoxLayout->addWidget(spanLabel);
      spanBoxLayout->addWidget(m_spanSlider);
      spanBoxLayout->addWidget(m_spanSpinBox);

      connect(m_spanSlider, SIGNAL(valueChanged(int)), this, SLOT(slotSpanSliderToDouble(int)));
      connect(m_spanSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotSpanSpinBoxToInt(double)));
      connect(m_spanSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotSpanValue(double)));

      mainLayout->addLayout(spanBoxLayout);
    }

    setLayout(mainLayout);

  }


  void PlotView::closeEvent(QCloseEvent *evt)
  {
    QString qstr(this->windowTitle());
    emit(signalClose(qstr));
  }


  void PlotView::plotViewData(PlotViewData &_plotViewData, const std::function<bool ()> &t_workCanceled)
  {
    switch(m_plotType)
    {
    case RVPV_LINEPLOT:
      if ((_plotViewData.ts) && (_plotViewData.ts->values().size() > 0))
        linePlotItem(_plotViewData, t_workCanceled);
      break;
    case RVPV_FLOODPLOT:
      if ((_plotViewData.ts) && (_plotViewData.ts->values().size() > 0))
        floodPlotItem(_plotViewData);
      break;
    case RVPV_ILLUMINANCEPLOT:
      illuminancePlotItem(_plotViewData);
      break;
    }

  }

  void PlotView::plotViewDataDifference(PlotViewData &_plotViewData1, PlotViewData &_plotViewData2)
  {
    switch(m_plotType)
    {
    case RVPV_ILLUMINANCEPLOT:
      illuminancePlotItemsDifference(_plotViewData1, _plotViewData2);
      break;
    }
  }


  void PlotView::illuminancePlotItemsDifference(PlotViewData &_plotViewData1, PlotViewData &_plotViewData2)
  {
    // clear items as necessary
    m_valueInfo->hide();
    m_valueInfoMarker->hide();

    m_legendName = _plotViewData1.legendName + "-" + _plotViewData2.legendName;
    m_alias = _plotViewData1.alias;
    m_alias.append(_plotViewData2.alias);
    m_plotSource = _plotViewData1.plotSource;
    m_plotSource.append(_plotViewData2.plotSource);
    QString title = updateLabelString( m_legendName, m_alias, m_plotSource);
    m_plot->setTitle(title);

    m_plot->setAxisTitle(QwtPlot::xBottom, "x (m)");
    m_plot->setAxisTitle(QwtPlot::yLeft, "y (m)");

    openstudio::SqlFile sqlFile1(openstudio::toPath(_plotViewData1.plotSource[0]));
    openstudio::SqlFile sqlFile2(openstudio::toPath(_plotViewData2.plotSource[0]));


    // list of hourly reports for the illuminance map
    std::vector< std::pair<int, openstudio::DateTime> > reportIndicesDates1 = sqlFile1.illuminanceMapHourlyReportIndicesDates(openstudio::toString(_plotViewData1.dbIdentifier));
    if (reportIndicesDates1.size() <= 0)
    {
      //    LOG(Error, "no report indices for illuminance map '" << openstudio::toString(_plotViewData1.legendName) << "'");
      return;
    }
    std::vector< std::pair<int, openstudio::DateTime> > reportIndicesDates2 = sqlFile2.illuminanceMapHourlyReportIndicesDates(openstudio::toString(_plotViewData2.dbIdentifier));
    if (reportIndicesDates2.size() <= 0)
    {
      //    LOG(Error, "no report indices for illuminance map '" << openstudio::toString(_plotViewData2.legendName) << "'");
      return;
    }

    // use common dates
    m_illuminanceMapDifferenceReportIndices.clear();

    std::vector< std::pair<int, openstudio::DateTime> >::iterator it1;
    std::vector< std::pair<int, openstudio::DateTime> >::iterator it2;
    int count=0;

    for (it1 = reportIndicesDates1.begin(); it1 != reportIndicesDates1.end(); ++it1)
    {
      bool bFound = false;
      for (it2 = reportIndicesDates2.begin(); it2 != reportIndicesDates2.end(); ++it2)
      {
        if ((*it2).second == (*it1).second)
        {
          bFound = true;
          break;
        }
      }
      if (bFound)
      {
        std::pair< int, int > pairIndices( (*it1).first,  (*it2).first );
        m_illuminanceMapDifferenceReportIndices.push_back( pairIndices );
        std::pair< int, openstudio::DateTime > pairIndexDate( count,  (*it1).second );
        m_illuminanceMapReportIndicesDates.push_back( pairIndexDate );
        count++;
      }
    }

    m_illuminanceMapData.resize(m_illuminanceMapReportIndicesDates.size());

    // assuming x and y the same
    m_centerSlider->setRange(0,m_illuminanceMapReportIndicesDates.size()-1);

    std::vector<double> x1,x2;
    std::vector<double> y1,y2;
    std::vector<double> illuminance1, illuminance2,illuminanceDiff;
    sqlFile1.illuminanceMap(m_illuminanceMapDifferenceReportIndices[0].first,x1,y1,illuminance1);
    sqlFile2.illuminanceMap(m_illuminanceMapDifferenceReportIndices[0].second,x2,y2,illuminance2);
    unsigned i=0;
    while ( (i<illuminance1.size()) && (i<illuminance2.size()) )
    {// can check if x1=x2 and y1=y2
      illuminanceDiff.push_back(illuminance1[i]-illuminance2[i]);
      i++;
    }
    openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(x1,y1,illuminanceDiff,openstudio::LinearInterp);



    m_illuminanceMapData[0] = data;

    m_yAxisMin = data->minY();
    m_yAxisMax = data->maxY();
    m_plot->setAxisScale(QwtPlot::xBottom, data->minX(), data->maxX());
    m_plot->setAxisScale(QwtPlot::yLeft, data->minY(), data->maxY());

    m_centerSlider->setValue(0);
    QString centerDate(tr("Unknown"));
    centerDate.sprintf("%02d/%02d %02d:%02d:%02d", openstudio::month((m_illuminanceMapReportIndicesDates[0].second).date().monthOfYear()), (m_illuminanceMapReportIndicesDates[0].second).date().dayOfMonth(), (m_illuminanceMapReportIndicesDates[0].second).time().hours(), 0,0);
    m_centerDate->setText(centerDate);

    m_floodPlotData = data;

    m_spectrogram->setData(*m_floodPlotData);

    setDataRange();

    m_spectrogramOn = true;
    m_contourOn = false;

    m_plot->replot();

    updateZoomBase(m_plot->canvas()->rect(), true);
  }




  void PlotView::illuminancePlotItem(resultsviewer::PlotViewData &_plotViewData)
  {
    // clear items as necessary
    m_valueInfo->hide();
    m_valueInfoMarker->hide();

    m_dbIdentifier = _plotViewData.dbIdentifier;
    m_legendName = _plotViewData.legendName;
    m_alias = _plotViewData.alias;
    m_plotSource = _plotViewData.plotSource;
    QString title = updateLabelString( m_legendName, m_alias, m_plotSource);
    m_plot->setTitle(title);

    m_plot->setAxisTitle(QwtPlot::xBottom, "x (m)");
    m_plot->setAxisTitle(QwtPlot::yLeft, "y (m)");
    m_plot->setAxisTitle(QwtPlot::yRight, "(lux)");

    openstudio::SqlFile sqlFile(openstudio::toPath(_plotViewData.plotSource[0]));


    // yearly min and max
    sqlFile.illuminanceMapMaxValue(openstudio::toString(m_dbIdentifier),m_floodPlotYearlyMin,m_floodPlotYearlyMax);
    m_floodPlotMin = m_floodPlotYearlyMin;
    m_floodPlotMax = m_floodPlotYearlyMax;


    // reference points
    boost::optional<std::string> refPt;
    refPt = sqlFile.illuminanceMapRefPt(openstudio::toString(m_dbIdentifier),1);
    if (refPt)
    {
      QString str = openstudio::toQString(*refPt);
      str.remove("RefPt1=");
      str.remove("(");
      str.remove(")");
      QStringList list = str.split(":");
      if (list.count() == 3)
      {
        m_illuminanceMapRefPt1->setValue(list[0].toDouble(), list[1].toDouble());
        m_illuminanceMapRefPt1->show();
      }
    }
    refPt = sqlFile.illuminanceMapRefPt(openstudio::toString(m_dbIdentifier),2);
    if (refPt)
    {
      QString str = openstudio::toQString(*refPt);
      str.remove("RefPt2=");
      str.remove("(");
      str.remove(")");
      QStringList list = str.split(":");
      if (list.count() == 3)
      {
        m_illuminanceMapRefPt2->setValue(list[0].toDouble(), list[1].toDouble());
        m_illuminanceMapRefPt2->show();
      }
    }

    // list of hourly reports for the illuminance map
    m_illuminanceMapReportIndicesDates = sqlFile.illuminanceMapHourlyReportIndicesDates(openstudio::toString(m_dbIdentifier));

    if (m_illuminanceMapReportIndicesDates.size() <= 0)
    {
      //    LOG(Error, "no report indices for illuminance map '" << openstudio::toString(_plotViewData.legendName) << "'");
      return;
    }
    m_illuminanceMapData.resize(m_illuminanceMapReportIndicesDates.size());

    m_centerSlider->setRange(0,m_illuminanceMapReportIndicesDates.size()-1);

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> illuminance;
    sqlFile.illuminanceMap(m_illuminanceMapReportIndicesDates[0].first,x,y,illuminance);
    openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(x,y,illuminance,openstudio::LinearInterp);

    m_illuminanceMapData[0] = data;

    m_yAxisMin = data->minY();
    m_yAxisMax = data->maxY();
    m_plot->setAxisScale(QwtPlot::xBottom, data->minX(), data->maxX());
    m_plot->setAxisScale(QwtPlot::yLeft, data->minY(), data->maxY());

    m_centerSlider->setValue(0);
    QString centerDate(tr("Unknown"));
    centerDate.sprintf("%02d/%02d %02d:%02d:%02d", openstudio::month((m_illuminanceMapReportIndicesDates[0].second).date().monthOfYear()), (m_illuminanceMapReportIndicesDates[0].second).date().dayOfMonth(), (m_illuminanceMapReportIndicesDates[0].second).time().hours(), 0,0);
    m_centerDate->setText(centerDate);

    m_floodPlotData = data;

    m_spectrogram->setData(*m_floodPlotData);

    setDataRange();
    m_spectrogramOn = true;
    m_contourOn = false;

    m_plot->replot();

    updateZoomBase(m_plot->canvas()->rect(), true);
  }



  void PlotView::floodPlotItem(resultsviewer::PlotViewData &_plotViewData)
  {
    // clear items as necessary
    m_valueInfo->hide();
    m_valueInfoMarker->hide();

    m_legendName = _plotViewData.legendName;
    m_alias = _plotViewData.alias;
    m_plotSource = _plotViewData.plotSource;
    QString title = updateLabelString( m_legendName, m_alias, m_plotSource);
    m_plot->setTitle(title);

    if (_plotViewData.interval == "HOURLY")
    {
      m_plot->setAxisTitle(QwtPlot::xBottom, "Day of Simulation");
      m_plot->setAxisTitle(QwtPlot::yLeft, "Hour of Day");
    }
    else if (_plotViewData.interval == "DAILY")
    {
      m_plot->setAxisTitle(QwtPlot::xBottom, "Day of Simulation");
      m_plot->setAxisTitle(QwtPlot::yLeft, "");
    }
    else if (_plotViewData.interval == "MONTHLY")
    {
      m_plot->setAxisTitle(QwtPlot::xBottom, "Month of Simulation");
      m_plot->setAxisTitle(QwtPlot::yLeft, "");
    }
    else
    {
      m_plot->setAxisTitle(QwtPlot::xBottom, "");
      m_plot->setAxisTitle(QwtPlot::yLeft, "");
    }


    m_startDateTime =  _plotViewData.ts->firstReportDateTime();
    m_endDateTime = _plotViewData.ts->firstReportDateTime() + Time(_plotViewData.ts->daysFromFirstReport(_plotViewData.ts->daysFromFirstReport().size()-1));
    m_xAxisMin = m_startDateTime.date().dayOfYear();
    m_xAxisMax = m_xAxisMin + (m_endDateTime - m_startDateTime).totalDays();
    if (m_plotViewTimeAxis == nullptr)
    {
      m_plotViewTimeAxis = new PlotViewTimeAxis(RVPV_FLOODPLOT);
      m_plot->setAxisTitle(QwtPlot::xBottom, " Simulation Time");
      m_plot->setAxisScaleDraw(QwtPlot::xBottom, m_plotViewTimeAxis);
      m_plot->setAxisLabelRotation(QwtPlot::xBottom, -90.0);
      m_plot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
    }
    else
    {
      m_plotViewTimeAxis->startDateTime(m_startDateTime);
    }

    openstudio::TimeSeriesFloodPlotData::Ptr data = openstudio::TimeSeriesFloodPlotData::create(*_plotViewData.ts);
    m_centerSlider->setRange(100.0*data->minX(),100.0*data->maxX());
    m_spanSlider->setRange(0,50*(data->maxX()-data->minX()));

    m_centerSpinBox->setRange(data->minX(),data->maxX());
    m_spanSpinBox->setRange(0,0.5*(data->maxX()-data->minX()));

    m_yAxisMin = data->minY();
    m_yAxisMax = data->maxY();
    m_plot->setAxisScale(QwtPlot::xBottom, data->minX(), data->maxX());
    m_plot->setAxisScale(QwtPlot::yLeft, data->minY(), data->maxY());

    if (!m_floodPlotData)
    {
      m_centerSpinBox->setValue(0.5 * (data->maxX() - data->minX()));
      m_spanSpinBox->setValue(0.5 * (data->maxX() - data->minX()));
    }
    else
    {
      m_centerSpinBox->setValue(m_centerSpinBox->value());
      m_spanSpinBox->setValue(m_spanSpinBox->value());
    }

    m_floodPlotData = data;

    rightAxisTitleFromUnits(openstudio::toQString(m_floodPlotData->units()));
    m_spectrogram->setData(*m_floodPlotData);

    setDataRange();

    m_spectrogramOn = true;
    m_contourOn = false;

    m_plot->replot();

    updateZoomBase(m_plot->canvas()->rect(), true);
  }

  void PlotView::contourLevels(openstudio::Vector& contourValues)
  {
    QwtValueList contours;
    for( unsigned int level = 0; level < contourValues.size(); level ++ )
      contours += contourValues[level];

    m_spectrogram->setContourLevels(contours);
    m_plot->replot();
  }

  void PlotView::colorLevels(openstudio::Vector& colorLevels)
  {
    // update color stops based on input vector

    m_colorLevels = colorLevels;

    openstudio::FloodPlotColorMap colorMap = openstudio::FloodPlotColorMap(m_colorLevels, m_colorMapType);

    m_spectrogram->setColorMap(colorMap);
    m_plot->setAxisScale(QwtPlot::yRight, openstudio::minimum(colorLevels), openstudio::maximum(colorLevels)); // legend numbers
    m_rightAxis->setColorMap(QwtDoubleInterval(openstudio::minimum(colorLevels), openstudio::maximum(colorLevels)), m_spectrogram->colorMap()); // legend colors
    m_plot->replot();
  }

  void PlotView::colorMapRange(double min, double max)
  {
    if (min >= max)
      return;
    if (!m_floodPlotData)
      return;

    QwtDoubleInterval colorMap = QwtDoubleInterval(min, max);
    m_floodPlotData->colorMapRange(colorMap); // color range applied to plot data
    m_spectrogram->setData(*m_floodPlotData);
    m_plot->setAxisScale(QwtPlot::yRight, min, max); // legend numbers
    m_rightAxis->setColorMap(colorMap, m_spectrogram->colorMap()); // legend colors
    m_plot->replot();
  }

  void PlotView::showContour(bool on)
  {
    m_spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, on);
    m_contourOn = on;
  }

  void PlotView::showSpectrogram(bool on)
  {
    m_spectrogram->setDisplayMode(QwtPlotSpectrogram::ImageMode, on);
    m_spectrogram->setDefaultContourPen(on ? QPen() : QPen(Qt::NoPen));
    m_spectrogramOn = on;
  }

  void PlotView::setColorMap(QString& clrMap)
  {
    openstudio::FloodPlotColorMap::ColorMapList clrMapType;
    if (clrMap.toUpper() == "GRAY")
    {
      clrMapType = openstudio::FloodPlotColorMap::Gray;
      setColorMap(clrMapType);
    }
    else if (clrMap.toUpper() == "JET")
    {
      clrMapType = openstudio::FloodPlotColorMap::Jet;
      setColorMap(clrMapType);
    }

  }

  void PlotView::setColorMap(openstudio::FloodPlotColorMap::ColorMapList clrMap)
  {
    m_colorMapType = clrMap;
    initColorMap();
    initColorBar();
  }

  void PlotView::initColorMap()
  {
    m_colorMap = openstudio::FloodPlotColorMap(m_colorLevels, m_colorMapType);
    m_spectrogram->setColorMap(m_colorMap);
    m_spectrogram->setData(*m_floodPlotData);
  }

  void PlotView::setDataRange()
  {
    if (m_floodPlotAutoScale)
      m_dataRange = QwtDoubleInterval(m_floodPlotData->minValue(), m_floodPlotData->maxValue());
    else
      m_dataRange = QwtDoubleInterval(m_floodPlotMin, m_floodPlotMax);

    if (m_dataRange.minValue() == m_dataRange.maxValue())
      m_dataRange = QwtDoubleInterval(m_dataRange.minValue(), m_dataRange.minValue() + 1.0);

    m_colorLevels=openstudio::linspace(m_dataRange.minValue(), m_dataRange.maxValue(),m_colorMapLength);

    m_floodPlotData->colorMapRange(m_dataRange);

    setColorMap(m_colorMapType);
    /// default contour levels - 10
    QwtValueList contourLevels;
    double interval = (m_dataRange.maxValue() - m_dataRange.minValue())/10.0;
    for ( double level = m_dataRange.minValue(); level < m_dataRange.maxValue(); level += interval )
      contourLevels += level;

    m_spectrogram->setContourLevels(contourLevels);

  }

  void PlotView::initColorBar()
  {
    QwtScaleWidget *rightAxis = m_plot->axisWidget(QwtPlot::yRight);
    rightAxis->setColorBarEnabled(true);
    rightAxis->setColorMap(m_dataRange, m_spectrogram->colorMap());

    m_plot->setAxisScale(QwtPlot::yRight, m_dataRange.minValue(), m_dataRange.maxValue() );
    m_plot->enableAxis(QwtPlot::yRight);

  }



  void PlotView::linePlotItem(resultsviewer::PlotViewData &_plotViewData, const std::function<bool ()> &t_workCanceled)
  {
    if (m_plotViewTimeAxis == nullptr)
    {
      m_startDateTime = _plotViewData.ts->firstReportDateTime();
      m_endDateTime = _plotViewData.ts->firstReportDateTime() + Time(_plotViewData.ts->daysFromFirstReport(_plotViewData.ts->daysFromFirstReport().size()-1));
      m_plotViewTimeAxis = new PlotViewTimeAxis(RVPV_LINEPLOT);
      m_plot->setAxisTitle(QwtPlot::xBottom, " Simulation Time");
      m_plot->setAxisScaleDraw(QwtPlot::xBottom, m_plotViewTimeAxis);
      m_xAxisMin = m_startDateTime.date().dayOfYear() + m_startDateTime.time().totalDays();
      m_xAxisMax = m_xAxisMin + (m_endDateTime - m_startDateTime).totalDays();
      m_plot->setAxisLabelRotation(QwtPlot::xBottom, -90.0);
      m_plot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
    }
    else
    {
      double offset = _plotViewData.ts->firstReportDateTime().date().dayOfYear() + _plotViewData.ts->firstReportDateTime().time().totalDays();
      if (offset < m_xAxisMin) m_xAxisMin = offset;

      double endOffset = ((_plotViewData.ts->firstReportDateTime() + Time(_plotViewData.ts->daysFromFirstReport(_plotViewData.ts->daysFromFirstReport().size()-1))) - _plotViewData.ts->firstReportDateTime()).totalDays() + offset;
      if ( endOffset > m_xAxisMax ) m_xAxisMax = endOffset;
    }

    openstudio::TimeSeriesLinePlotData::Ptr data = openstudio::TimeSeriesLinePlotData::create(*_plotViewData.ts);

    if (data)
    {
      m_centerSlider->setRange(100*m_xAxisMin, 100*m_xAxisMax);
      m_spanSlider->setRange(0, 50*(m_xAxisMax - m_xAxisMin));
      m_centerSpinBox->setRange(m_xAxisMin, m_xAxisMax);
      m_spanSpinBox->setRange(0, 0.5*(m_xAxisMax - m_xAxisMin));
      if (numberOfCurves() == 0)
      {
        m_centerSpinBox->setValue(0.5 * (m_xAxisMax - m_xAxisMin));
        m_spanSpinBox->setValue(0.5 * (m_xAxisMax - m_xAxisMin));
        m_lineThickness = 2; // default
        m_yAxisMin = data->minY();
        m_yAxisMax = data->maxY();
      }
      else
      {
        if (data->minY() < m_yAxisMin) m_yAxisMin = data->minY();
        if (data->maxY() > m_yAxisMax) m_yAxisMax = data->maxY();
      }


      QString legendLabel = updateLabelString(_plotViewData.legendName, _plotViewData.alias, _plotViewData.plotSource);
      auto curve = new LinePlotCurve(legendLabel,*data);
      curve->setLegend(_plotViewData.legendName);
      curve->setAlias(_plotViewData.alias);
      curve->setPlotSource(_plotViewData.plotSource);
      QColor color = curveColor(m_lastColor);
      m_lastColor = color;
      curve->setPen(curvePen(color));
      curve->attach(m_plot);

      // check for number of different units (std::string  based)
      QString curveUnits = openstudio::toQString(data->units());
      if (m_leftAxisUnits == "NONE SPECIFIED")
      {
        m_leftAxisUnits = curveUnits;
        leftAxisTitleFromUnits(m_leftAxisUnits);
        curve->setYAxis(QwtPlot::yLeft);
        m_plot->enableAxis(QwtPlot::yRight, false);
        m_zoomer[1]->setEnabled(false);
      }
      else if (m_leftAxisUnits.toUpper() == curveUnits.toUpper())
      {
        curve->setYAxis(QwtPlot::yLeft);
      }
      else if (m_rightAxisUnits == "NONE SPECIFIED")
      {
        m_rightAxisUnits = curveUnits;
        rightAxisTitleFromUnits(m_rightAxisUnits);
        curve->setYAxis(QwtPlot::yRight);
        m_plot->enableAxis(QwtPlot::yRight, true);
        m_zoomer[1]->setEnabled(true);
      }
      else if (m_rightAxisUnits.toUpper() == curveUnits.toUpper())
      {
        curve->setYAxis(QwtPlot::yRight);
      }
      else // more than 2 units - scale all curves
      {
        scaleCurves(curve, t_workCanceled);
        m_plot->enableAxis(QwtPlot::yRight, false);
        m_zoomer[1]->setEnabled(false);
        m_plot->setAxisTitle(QwtPlot::yLeft,"Scaled");
      }
      /// update legend and replot
      showCurve(curve, true);

      // update zoom base rect
      updateZoomBase(m_plot->canvas()->rect(), true);
    }

  }





  QColor PlotView::curveColor(QColor &lastColor)
  {
    auto colorIt = std::find(m_colorVec.begin(), m_colorVec.end(), lastColor);
    if ( (colorIt == m_colorVec.end()) ||  (*colorIt == m_colorVec.back()) ) {
      return (m_colorVec.at(0));
    } else {
      ++colorIt;
      return *colorIt;
    }
  }


  QPen PlotView::curvePen(QColor &color)
  {
    // trac ticket 170
    // same thickness per email from Kyle 3/31/10
    QPen pen(color);
    pen.setCosmetic(true);
    /*
    if (m_lineThickness > 2)
    m_lineThickness-=2;
    else
    m_lineThickness=8;
    int numCurves = numberOfCurves();
    if (numCurves < 4)
    pen.setStyle(Qt::SolidLine);
    else if (numCurves < 8)
    pen.setStyle(Qt::DotLine);
    else if (numCurves < 12)
    pen.setStyle(Qt::DashLine);
    else
    pen.setStyle(Qt::DashDotLine);
    */
    pen.setWidth(m_lineThickness);
    return pen;
  }


  int PlotView::numberOfCurves()
  {
    /// multiple curves based on units
    const QwtPlotItemList &listPlotItem = m_plot->itemList();
    QwtPlotItemIterator itPlotItem;
    int curveCount = 0;
    for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
    {
      QwtPlotItem *plotItem = *itPlotItem;
      if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
      {
        curveCount++;
      }
    }
    return curveCount;
  }


  void PlotView::scaleCurves(LinePlotCurve *curve, const std::function<bool ()> &t_workCanceled)
  {

    /// multiple curves based on units
    const QwtPlotItemList &listPlotItem = m_plot->itemList();
    LinePlotCurve *plotCurve;
    QwtPlotItemIterator itPlotItem;
    int curveCount = numberOfCurves();

    switch (curveCount)
    {
    case 0:
      {
        curve->setYAxis(QwtPlot::yLeft);
        m_plot->enableAxis(QwtPlot::yRight, false);
        m_zoomer[1]->setEnabled(false);
        break;
      }
    case 1:
      {
        curve->setYAxis(QwtPlot::yRight);
        m_plot->enableAxis(QwtPlot::yRight, true);
        m_zoomer[1]->setEnabled(true);
        break;
      }
    default: //scale
      m_plot->enableAxis(QwtPlot::yRight, false);
      m_zoomer[1]->setEnabled(false);
      // find min, max of all curves
      // scale
      int i;
      for ( itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
      {
        QApplication::processEvents();
        if ( (*itPlotItem)->rtti() == QwtPlotItem::Rtti_PlotCurve)
        {
          plotCurve = (LinePlotCurve*) (*itPlotItem);

          if ((plotCurve->minYValue() != 0) || (plotCurve->maxYValue() != 1))
          {

            //          QwtArray<double> xData(plotCurve->dataSize());
            QwtArray<double> yData(plotCurve->dataSize());
            for (i = 0; i < plotCurve->dataSize(); ++i)
            {
              if (i % 1000 == 0)
              {
                QApplication::processEvents();
                if (t_workCanceled && t_workCanceled())
                {
                  return;
                }
              }
              //        xData[i] = plotCurve->x(i);
              yData[i] = (plotCurve->y(i) - plotCurve->minYValue())/ (plotCurve->maxYValue() - plotCurve->minYValue());
            }
            // reset data
            plotCurve->setTitle(plotCurve->title().text() + "[" + QString::number(plotCurve->minYValue()) + ", "  + QString::number(plotCurve->maxYValue()) + "]");
            plotCurve->setYScaled(yData);
            plotCurve->setDataMode(resultsviewer::scaledY);
          }
        }
      }
      break;
    }

  }

  void PlotView::showCurve(QwtPlotItem *item, bool on)
  {
    /// update curve visibility
    item->setVisible(on);
    m_legend->refreshLegend();
    m_plot->replot();
  }


  void PlotView::setPlotCursor(const QCursor& _cursor)
  {
    //  setCursor(_cursor); // this widget
    m_plot->canvas()->setCursor(_cursor); // override crosshairs
  }


  void PlotView::updateZoomBase(const QwtDoubleRect& base, bool reset)
  {
    m_zoomer[0]->setZoomBase();
    if ( m_zoomer[1]->isEnabled() ) m_zoomer[1]->setZoomBase();
  }


  void PlotView::slotZoomMode(bool on)
  {
    if (on) setPlotCursor(m_zoomCursor);
    m_zoomer[0]->setEnabled(on);
    if ( m_zoomer[1]->isEnabled() ) m_zoomer[1]->setEnabled(on);
  }

  void PlotView::slotPanMode(bool on)
  {
    if (on) setPlotCursor(m_panCursor);
    m_panner->setEnabled(on);
  }

  void PlotView::slotValueInfoMode(bool on)
  {
    if (on)
    {
      setPlotCursor(m_valueInfoCursor);
    }
    else
    {
      m_valueInfo->hide();
      m_valueInfoMarker->hide();
      m_plot->replot(); // to remove marker from canvas
    }
    m_picker->setEnabled(on);
  }

  void PlotView::slotZoomIn()
  {
    m_zoomer[0]->zoom(1);
    if (m_zoomer[1]->isEnabled()) m_zoomer[1]->zoom(1);
  }

  void PlotView::slotZoomOut()
  {
    m_zoomer[0]->zoom(-1);
    if (m_zoomer[1]->isEnabled()) m_zoomer[1]->zoom(-1);
  }


  void PlotView::slotPointerMode(bool on)
  {
    // may have selection for various plot items here
    // holder to turn other exclusive buttons off
    if (on) setPlotCursor(m_selectCursor);
  }

  void PlotView::slotPrint()
  {
    QPrinter printer;

    QString docName = m_plot->title().text();
    if ( !docName.isEmpty() )
    {
      docName.replace (QRegExp (QString::fromLatin1 ("\n")), tr (" -- "));
      printer.setDocName (docName);
    }

    printer.setCreator("Results Viewer");
    printer.setOrientation(QPrinter::Landscape);

    QPrintDialog dialog(&printer);
    if ( dialog.exec() )
    {
      QwtPlotPrintFilter filter;
      if ( printer.colorMode() == QPrinter::GrayScale )
      {
        int options = QwtPlotPrintFilter::PrintAll;
        options &= ~QwtPlotPrintFilter::PrintBackground;
        options |= QwtPlotPrintFilter::PrintFrameWithScales;
        filter.setOptions(options);
      }
      QPainter p;
      QRect r(0,0,printer.width(),printer.height());
      p.begin(&printer);
      m_plot->print(&p, r, filter);
      printLegend(&p, r);
      p.end();
    }

  }


  void PlotView::dropEvent(QDropEvent *e)
  {
    const PlotViewMimeData *mimeData = qobject_cast<const PlotViewMimeData *>(e->mimeData());
    if (mimeData)
    {
      std::vector<PlotViewData> plotViewDataVec = mimeData->plotViewDataVec();
      std::vector<PlotViewData>::iterator plotViewDataVecIt;
      if (plotViewDataVec.size() > 0)
      {
        switch (m_plotType)
        {
        case RVPV_LINEPLOT:
          for (plotViewDataVecIt = plotViewDataVec.begin(); plotViewDataVecIt != plotViewDataVec.end(); ++plotViewDataVecIt)
          {
            plotViewData((*plotViewDataVecIt), std::function<bool ()>());
          }
          m_legend->update();
          break;
        case RVPV_FLOODPLOT:
          // replace floodplot data with last selected
          plotViewData(plotViewDataVec.back(), std::function<bool()>());
          break;
        }
      }
    }
  }


  void PlotView::slotCenterSliderToDouble(int center)
  {
    m_centerSpinBox->setValue(center / 100.0);
  }

  void PlotView::slotSpanSliderToDouble(int span)
  {
    m_spanSpinBox->setValue(span / 100.0);
  }

  void PlotView::slotCenterSpinBoxToInt(double center)
  {
    m_centerSlider->setValue(100*center);
  }

  void PlotView::slotSpanSpinBoxToInt(double span)
  {
    m_spanSlider->setValue(100*span);
  }


  void PlotView::slotCenterValue(double center)
  {
    xCenterSpan(center, m_spanSpinBox->value());
  }

  void PlotView::slotSpanValue(double span)
  {
    xCenterSpan(m_centerSpinBox->value(), span);
  }

  void PlotView::xCenterSpan(double center, double span)
  {
    /// min = maximum(center - span, min(x data))
    /// max = minimum(center + span, max(x data))
    double minX = center - span;
    double maxX = center + span;
    if (minX < m_xAxisMin)  {
      minX = m_xAxisMin;
      maxX = minX + 2.0 * span;
    } else if (maxX > m_xAxisMax) {
      maxX = m_xAxisMax;
      minX = maxX - 2.0 * span;
    }
    m_plot->setAxisScale(QwtPlot::xBottom, minX, maxX);

    if (m_yLeftAutoScale || m_yRightAutoScale)  AutoScaleY(minX, maxX);

    m_valueInfo->hide();
    m_valueInfoMarker->hide();
    m_plot->replot();
  }


  void PlotView::AutoScaleY(double minX, double maxX)
  { // ticket 346
    /// find min and max for x range for all curves
    if (maxX <= minX) return;
    const QwtPlotItemList &listPlotItem = m_plot->itemList();
    LinePlotCurve *linePlotCurve;
    QwtPlotItemIterator itPlotItem;
    double yLeftMin = DBL_MAX;
    double yLeftMax = -DBL_MAX;
    double yRightMin = DBL_MAX;
    double yRightMax = -DBL_MAX;
    int minXIndex, maxXIndex, i;

    for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
    {
      QwtPlotItem *plotItem = *itPlotItem;
      if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
      {
        linePlotCurve = (LinePlotCurve *)plotItem;
        minXIndex = 0;
        maxXIndex = linePlotCurve->dataSize()-1;

        for (minXIndex=0;minXIndex<linePlotCurve->dataSize();minXIndex++)
          if (minX < linePlotCurve->x(minXIndex)) break;

        for (maxXIndex=linePlotCurve->dataSize()-1;maxXIndex > minXIndex;maxXIndex--)
          if (maxX > linePlotCurve->x(maxXIndex)) break;

        for(i=minXIndex;i<=maxXIndex;i++)
        {
          switch (linePlotCurve->yAxis())
          {
          case QwtPlot::yLeft:
            if (linePlotCurve->y(i) < yLeftMin) yLeftMin = linePlotCurve->y(i);
            if (linePlotCurve->y(i) > yLeftMax) yLeftMax = linePlotCurve->y(i);
            break;
          case QwtPlot::yRight:
            if (linePlotCurve->y(i) < yRightMin) yRightMin = linePlotCurve->y(i);
            if (linePlotCurve->y(i) > yRightMax) yRightMax = linePlotCurve->y(i);
            break;
          }
        }
      }
    }
    if (m_yLeftAutoScale && (yLeftMin < yLeftMax)) m_plot->setAxisScale(QwtPlot::yLeft, yLeftMin, yLeftMax);
    if (m_yRightAutoScale && (yRightMin < yRightMax)) m_plot->setAxisScale(QwtPlot::yRight, yRightMin, yRightMax);
  }

  QStringList PlotView::colorMapList()
  {
    QStringList colorMaps(QStringList() << tr("Gray") << tr("Jet"));
    return colorMaps;
  }


  void PlotView::printLegend(QPainter *p, const QRect &rect)
  {

    if ((p) && (m_legend) && (m_legend->isVisible()))
    {
      // scale
      int w = rect.width();
      int h = rect.height();
      double scaleW = double(rect.width()) / double(m_plot->width());
      double scaleH = double(rect.height()) / double(m_plot->height());
      // translation
      QPoint posLegend = m_legend->pos();
      QPoint posPlot = m_plot->pos();
      double dx = double(posLegend.x() - posPlot.x());
      double dy = double(posLegend.y() - posPlot.y());
      p->save();
      p->setWindow(rect.x(), rect.y(), w, h);
      p->translate(dx*scaleW, dy*scaleH);
      m_legend->render(p, QPoint(), QRegion(), QWidget::DrawChildren);
      p->restore();
    }
  }

  void PlotView::generateImage(QString& file, int w, int h)
  {
    if ((w*h)==0)
    {
      QPixmap pixmap(size());
      pixmap.fill(Qt::white);
      QPainter p(&pixmap);
      m_plot->print(&p, rect());
      printLegend(&p, rect());
      p.end();
      pixmap.save(file, nullptr, -1);
    }
    else
    {
      QPixmap pixmap(w, h);
      pixmap.fill(Qt::white);
      QRect r(0,0,w,h);
      QPainter p(&pixmap);
      m_plot->print(&p, r);
      printLegend(&p, r);
      p.end();
      pixmap.save(file, nullptr, -1);
    }
  }

  void PlotView::exportImageToFile(int w, int h)
  {
    QString filename = QFileDialog::getSaveFileName(this,
      tr("Export Image"), // caption
      m_lastImageSavedPath, // folder
      tr( "PNG (*.png)\n" // filters
      "Bitmap (*.bmp)"));
    if (!filename.isEmpty())
    {
      m_lastImageSavedPath = QFileInfo(filename).absoluteFilePath();
      emit(signalLastImageSavedPath(m_lastImageSavedPath));
      generateImage(filename, w, h);
    }
  }

  void PlotView::slotSaveImageScreenSize()
  {
    exportImageToFile(0,0);
  }

  void PlotView::slotSaveImage800x600()
  {
    exportImageToFile(800,600);
  }

  void PlotView::slotSaveImage400x300()
  {
    exportImageToFile(400,300);
  }


  void PlotView::slotProperties()
  {
    PlotViewProperties dialog(this);
    dialog.setPlotView(this);
    if (dialog.exec())
    {
    }
  }


  void PlotView::slotValueInfo(const QPoint& pos)
  {
    switch (m_plotType)
    {
    case resultsviewer::RVPV_LINEPLOT:
      valueInfoLinePlot(pos);
      break;
    case resultsviewer::RVPV_FLOODPLOT:
      valueInfoFloodPlot(pos);
      break;
    case resultsviewer::RVPV_ILLUMINANCEPLOT:
      valueInfoIlluminanceMap(pos);
      break;
    }
  }

  void PlotView::valueInfoLinePlot(const QPoint& pos)
  {
    // ticket #146 - find closest point and then mark and display information
    const QwtPlotItemList &listPlotItem = m_plot->itemList();
    QwtPlotItemIterator itPlotItem;
    double minDist = DBL_MAX;
    double dist;
    int minIndex = -1;
    int index;
    LinePlotCurve *minCurve=nullptr;
    for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
    {
      QwtPlotItem *plotItem = *itPlotItem;
      if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
      {
        index = ((LinePlotCurve *)plotItem)->closestPoint(pos, &dist);
        if (dist < minDist)
        {
          minIndex = index;
          minDist = dist;
          minCurve = (LinePlotCurve *)plotItem;
        }
      }
    }

    if (minCurve && (minIndex > -1)) //&& (minDist < 20))
    {
      double x = minCurve->x(minIndex);
      double y = minCurve->y(minIndex);

      openstudio::Time timeFromFracDays(x);
      //    openstudio::Date date = openstudio::Date::fromDayOfYear(timeFromFracDays.days()); Date issue with day 366 in year 2009
      //  Fix this - what class
      int dayOfYear = timeFromFracDays.days();
      while (dayOfYear > 365) dayOfYear -=365;
      openstudio::Date date = openstudio::Date::fromDayOfYear(dayOfYear);

      unsigned day = date.dayOfMonth();
      unsigned month = openstudio::month(date.monthOfYear());
      int hour = timeFromFracDays.hours();
      int minutes = timeFromFracDays.minutes();
      int seconds = timeFromFracDays.seconds();


      QString s;
      s.sprintf("%02d/%02d %02d:%02d:%02d", month, day, hour, minutes, seconds);


      QString info(tr("x=%1, y=%2\ntime=%3, y=%4").arg(x).arg(y).arg(s).arg(y));


      int xPos = m_plot->transform(QwtPlot::xBottom,x) + m_plot->canvas()->x() + 20;
      int yPos;
      if (minCurve->yAxis() == QwtPlot::yRight)
      {
        yPos = m_plot->transform(QwtPlot::yRight,y) + m_plot->canvas()->y();
        m_valueInfoMarker->setAxis(QwtPlot::xBottom, QwtPlot::yRight);
      }
      else
      {
        yPos = m_plot->transform(QwtPlot::yLeft,y) + m_plot->canvas()->y();
        m_valueInfoMarker->setAxis(QwtPlot::xBottom, QwtPlot::yLeft);
      }
      m_valueInfoMarker->setValue(x,y);
      m_valueInfoMarker->show();

      m_valueInfo->setText(info);
      m_valueInfo->setGeometry(xPos, yPos, m_valueInfo->minimumWidth(), m_valueInfo->minimumHeight());
      m_valueInfo->show();

      m_plot->replot(); // update marker location
    }
  }

  void PlotView::valueInfoFloodPlot(const QPoint& pos)
  {
    double x = m_plot->invTransform(QwtPlot::xBottom,pos.x());
    double y = m_plot->invTransform(QwtPlot::yLeft,pos.y());
    double value = m_floodPlotData->value(x,y);

    openstudio::Time timeFromFracDays(floor(x) + y/24.0);

    int dayOfYear = timeFromFracDays.days();
    while (dayOfYear > 365) dayOfYear -=365;
    openstudio::Date date = openstudio::Date::fromDayOfYear(dayOfYear);

    unsigned day = date.dayOfMonth();
    unsigned month = openstudio::month(date.monthOfYear());
    int hour = timeFromFracDays.hours();
    int minutes = timeFromFracDays.minutes();
    int seconds = timeFromFracDays.seconds();


    QString s;
    s.sprintf("%02d/%02d %02d:%02d:%02d", month, day, hour, minutes, seconds);


    QString info(tr("x=%1, y=%2 value=%3\ntime=%4, value=%5").arg(x).arg(y).arg(value).arg(s).arg(value));


    m_valueInfoMarker->setValue(x,y);
    int xPos = m_plot->transform(QwtPlot::xBottom,x) + m_plot->canvas()->x() + 20;
    int yPos = m_plot->transform(QwtPlot::yLeft,y) + m_plot->canvas()->y();
    m_valueInfoMarker->show();


    m_valueInfo->setText(info);
    m_valueInfo->setGeometry(xPos, yPos, m_valueInfo->minimumWidth(), m_valueInfo->minimumHeight());
    m_valueInfo->show();

    m_plot->replot(); // update marker location
  }


  void PlotView::valueInfoIlluminanceMap(const QPoint& pos)
  {
    // ticket #146 - find closest point and then mark and display information
    double x = m_plot->invTransform(QwtPlot::xBottom,pos.x());
    double y = m_plot->invTransform(QwtPlot::yLeft,pos.y());
    valueInfoIlluminanceMap(x, y);
    m_plot->replot(); // update marker location
  }

  void PlotView::valueInfoIlluminanceMap(const double& x, const double& y)
  {
    double value = m_floodPlotData->value(x,y);

    QString info(tr("x=%1, y=%2 value=%3").arg(x).arg(y).arg(value));

    m_valueInfoMarker->setValue(x,y);
    int xPos = m_plot->transform(QwtPlot::xBottom,x) + m_plot->canvas()->x() + 20;
    int yPos = m_plot->transform(QwtPlot::yLeft,y) + m_plot->canvas()->y();
    m_valueInfoMarker->show();


    m_valueInfo->setText(info);
    m_valueInfo->setGeometry(xPos, yPos, m_valueInfo->minimumWidth(), m_valueInfo->minimumHeight());
    m_valueInfo->show();
  }



  void PlotView::slotZoomed(const QwtDoubleRect& rect)
  {
    // if rect changes hide the value info
    m_valueInfo->hide();
    m_valueInfoMarker->hide();
    m_plot->replot(); // remove marker
  }


  void PlotView::leftAxisTitleFromUnits(const QString& units)
  {
    m_plot->setAxisTitle(QwtPlot::yLeft, axisTitleFromUnits(units));
  }

  void PlotView::rightAxisTitleFromUnits(const QString& units)
  {
    m_plot->setAxisTitle(QwtPlot::yRight, axisTitleFromUnits(units));
  }

  QString PlotView::axisTitleFromUnits(const QString& units)
  {
    QString axisTitle = units;

    if (axisTitle.toUpper() == "W")
    {
      axisTitle = "Power (W)";
    }
    else if (axisTitle.toUpper() == "J")
    {
      axisTitle = "Energy (J)";
    }
    else if (axisTitle.toUpper() == "C")
    {
      axisTitle = "Temperature (C)";
    }

    return axisTitle;
  }

  void PlotView::updateAlias(QString &alias, QString &plotSource)
  {
    bool found = false;


    const QwtPlotItemList &listPlotItem = m_plot->itemList();
    LinePlotCurve *linePlotCurve;
    QwtPlotItemIterator itPlotItem;
    switch (m_plotType)
    {
    case resultsviewer::RVPV_LINEPLOT: // update every line plot curve and refresh legend
      for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
      {
        QwtPlotItem *plotItem = *itPlotItem;
        if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
        {
          found = false;
          linePlotCurve = (LinePlotCurve *)plotItem;
          for (int i = 0; i < linePlotCurve->plotSource().count(); i++)
          {
            if (linePlotCurve->plotSource()[i] == plotSource)
            {
              linePlotCurve->alias()[i] = alias;
              found = true;
            }
          }
          if (found)
          {
            QString legendName = linePlotCurve->legend();
            QString title = updateLabelString( legendName, linePlotCurve->alias(), linePlotCurve->plotSource());
            linePlotCurve->setTitle(title);
          }
        }
      }
      m_legend->refreshLegend();
      m_legend->update();

      break;
    case resultsviewer::RVPV_FLOODPLOT: // update title - update all values
      for (int i = 0; i < m_plotSource.count(); i++)
      {
        if (m_plotSource[i] == plotSource)
        {
          m_alias[i] = alias;
          found = true;
        }
      }
      if (found)
      {
        QString title = updateLabelString( m_legendName, m_alias, m_plotSource);
        m_plot->setTitle(title);
      }
      break;
    }
  }

  QString PlotView::updateLabelString( QString& label, QStringList& alias, QStringList& plotSource)
  {
    QString updatedLabel = "";
    int count = alias.count();
    if ( alias.count() == plotSource.count() )
    {
      switch (count)
      {
      case 1: // usual single alias and plotsource
        updatedLabel = label.arg(alias[0]);
        break;
      case 2: // differences - involves two aliases and plotsources
        updatedLabel = label.arg(alias[0]).arg(alias[1]);
        break;
      }
    }
    return updatedLabel;
  }

  void PlotView::slotFloatOrDock()
  {
    emit(signalFloatOrDockMe(this));
  }


  void PlotView::slotCenterIlluminance(int reportIndex)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString centerDate(tr("Unknown"));
    centerDate.sprintf("%02d/%02d %02d:%02d:%02d", openstudio::month((m_illuminanceMapReportIndicesDates[reportIndex].second).date().monthOfYear()), (m_illuminanceMapReportIndicesDates[reportIndex].second).date().dayOfMonth(), (m_illuminanceMapReportIndicesDates[reportIndex].second).time().hours(), 0,0);
    m_centerDate->setText(centerDate);

    plotDataAvailable(true);
    // illuminance map caching
    if (m_illuminanceMapData[reportIndex])
      m_floodPlotData = m_illuminanceMapData[reportIndex];
    else
    {
      //  set report index and replot
      if (m_illuminanceMapDifferenceReportIndices.size() > 0)
      {
        openstudio::SqlFile sqlFile1(openstudio::toPath(m_plotSource[0]));
        openstudio::SqlFile sqlFile2(openstudio::toPath(m_plotSource[1]));
        // assuming x and y the same
        std::vector<double> x1,x2;
        std::vector<double> y1,y2;
        std::vector<double> illuminance1, illuminance2,illuminanceDiff;
        sqlFile1.illuminanceMap(m_illuminanceMapDifferenceReportIndices[reportIndex].first,x1,y1,illuminance1);
        sqlFile2.illuminanceMap(m_illuminanceMapDifferenceReportIndices[reportIndex].second,x2,y2,illuminance2);
        unsigned i=0;
        while ( (i<illuminance1.size()) && (i<illuminance2.size()) )
        {// can check if x1=x2 and y1=y2
          illuminanceDiff.push_back(illuminance1[i]-illuminance2[i]);
          i++;
        }
        openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(x1,y1,illuminanceDiff,openstudio::LinearInterp);


        m_floodPlotData = data;
        m_illuminanceMapData[reportIndex] = data;
      }
      else
      {
        openstudio::SqlFile sqlFile(openstudio::toPath(m_plotSource[0]));
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> illuminance;
        sqlFile.illuminanceMap(m_illuminanceMapReportIndicesDates[reportIndex].first,x,y,illuminance);
        openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(x,y,illuminance,openstudio::LinearInterp);

        m_floodPlotData = data;
        m_illuminanceMapData[reportIndex] = data;
      }
    }
    m_spectrogram->setData(*m_floodPlotData);

    setDataRange();
    // replot too slow - see http://www.qtcentre.org/threads/17892-Spectrogram-too-slow
    // test simplest "value" for raster data - a little faster than interpolation but not much - see double MatrixFloodPlotData::value(double x, double y) const

    if (m_valueInfo->isVisible())
      valueInfoIlluminanceMap(m_valueInfoMarker->xValue(),m_valueInfoMarker->yValue());

    m_plot->replot();

    QApplication::restoreOverrideCursor();
  }


  void PlotView::slotIlluminanceDateChanged()
  {
    // check if date within illuminance plot dates

    QStringList centerDate = m_centerDate->text().split(" ");
    if (centerDate.count() != 2) return;

    QStringList monthDay = centerDate[0].split("/");
    if (monthDay.count() != 2) return;

    QStringList hourMinuteSecond = centerDate[1].split(":");
    if (hourMinuteSecond.count() != 3) return;

    int reportIndex = -1;

    try
    {
      openstudio::DateTime dt(openstudio::DateTime(openstudio::Date(openstudio::monthOfYear(monthDay[0].toUInt()),monthDay[1].toUInt()), openstudio::Time(0,hourMinuteSecond[0].toUInt(), hourMinuteSecond[1].toUInt(), hourMinuteSecond[2].toDouble())));
      for( unsigned i=0; i < m_illuminanceMapReportIndicesDates.size(); i++)
        if (m_illuminanceMapReportIndicesDates[i].second == dt)
        {
          reportIndex = i;
          break;
        }
    }
    catch (...)
    {
      // invalid date specified - can add correction here
    }

    if (reportIndex > -1)
    {
      plotDataAvailable(true);
      m_centerSlider->setValue(reportIndex);
    }
    else
      plotDataAvailable(false);
  }

  void PlotView::plotDataAvailable(bool available)
  {
    if (available)
    {
      showContour(m_contourOn);
      showSpectrogram(m_spectrogramOn);
      m_noData->hide();
      m_plot->replot();
    }
    else
    {
      m_spectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, false);
      m_spectrogram->setDisplayMode(QwtPlotSpectrogram::ImageMode, false);
      // hide markers and show NO DATA
      //    int xPos = m_plot->transform(QwtPlot::xBottom, m_xAxisMax/2) + m_plot->canvas()->x() + 40;
      //    int yPos = m_plot->transform(QwtPlot::yLeft,m_yAxisMax/2) + m_plot->canvas()->y();
      int xPos = m_plot->canvas()->x() + 40;
      int yPos = m_plot->canvas()->y() + 120;
      m_noData->setGeometry(xPos, yPos, m_noData->sizeHint().width(), m_noData->sizeHint().height());
      m_noData->show();
      m_plot->replot();
    }
  }

  void PlotView::slotSpinDayChanged(int i)
  {
    setCenterDayHour( i-m_spinDayPreviousValue ,0);
    m_spinDayPreviousValue = i;
  }

  void PlotView::slotSpinHourChanged(int i)
  {
    setCenterDayHour(0, i-m_spinHourPreviousValue );
    m_spinHourPreviousValue = i;
  }

  void PlotView::setCenterDayHour(int day, int hour)
  {
    QStringList centerDate = m_centerDate->text().split(" ");
    if (centerDate.count() != 2) return;

    QStringList monthDay = centerDate[0].split("/");
    if (monthDay.count() != 2) return;

    QStringList hourMinuteSecond = centerDate[1].split(":");
    if (hourMinuteSecond.count() != 3) return;

    try
    {
      openstudio::DateTime dtCurrent( openstudio::Date(openstudio::monthOfYear(monthDay[0].toInt()), monthDay[1].toInt()),
        openstudio::Time(0, hourMinuteSecond[0].toInt(), hourMinuteSecond[1].toInt(), hourMinuteSecond[2].toInt()) );
      openstudio::Time dtDelta( day, hour, 0, 0 );

      dtCurrent = dtCurrent + dtDelta;
      QString strCenterDate(tr("Unknown"));
      strCenterDate.sprintf("%02d/%02d %02d:%02d:%02d", openstudio::month( dtCurrent.date().monthOfYear() ), dtCurrent.date().dayOfMonth(), dtCurrent.time().hours(), 0,0);
      m_centerDate->setText(strCenterDate);

      slotIlluminanceDateChanged();
    }
    catch (...)
    {
      // invalid date specified - can add correction here
    }
  }

  void PlotView::mousePressEvent(QMouseEvent *e)
  {
    if (e->button() == Qt::RightButton)
      slotProperties();
    QWidget::mousePressEvent(e);
  }


};
