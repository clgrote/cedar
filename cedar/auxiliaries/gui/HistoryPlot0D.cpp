/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        HistoryPlot0D.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 14

    Description:

    Credits:

======================================================================================================================*/
#include "cedar/configuration.h"

#ifdef CEDAR_USE_QWT

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/HistoryPlot0D.h"
#include "cedar/auxiliaries/gui/exceptions.h"
#include "cedar/auxiliaries/annotation/Dimensions.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/auxiliaries/DoubleData.h"
#include "cedar/auxiliaries/MatData.h"
#include "cedar/auxiliaries/UnitData.h"
#include "cedar/auxiliaries/assert.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/GlobalClock.h"
#include "cedar/units/Time.h"
#include "cedar/units/prefixes.h"

// SYSTEM INCLUDES
#include <qwt_legend.h>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QMenu>
#include <QThread>
#include <iostream>
#include <limits.h>

// MACROS
// Enable to show information on locking/unlocking
// #define DEBUG_LOCKS

#ifdef DEBUG_LOGS
#  include "auxiliaries/System.h"
#endif

//----------------------------------------------------------------------------------------------------------------------
// static members
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::gui::HistoryPlot0D::HistoryPlot0D(QWidget *pParent)
:
cedar::aux::gui::MultiPlotInterface(pParent),
mpHistoryPlot(nullptr)
{
  this->init();
}

cedar::aux::gui::HistoryPlot0D::HistoryPlot0D(cedar::aux::ConstDataPtr data, const std::string& title, QWidget *pParent)
:
cedar::aux::gui::MultiPlotInterface(pParent),
mpHistoryPlot(nullptr)
{
  this->init();
  this->plot(data, title);
}

cedar::aux::gui::HistoryPlot0D::~HistoryPlot0D()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

bool cedar::aux::gui::HistoryPlot0D::canAppend(cedar::aux::ConstDataPtr data) const
{
  auto mat_data = boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(data);

  if (!mat_data)
  {
    return false;
  }

  QReadLocker locker(&mat_data->getLock());
  if (mat_data->getDimensionality() != 0)
  {
    return false;
  }

  return true;
}

bool cedar::aux::gui::HistoryPlot0D::canDetach(cedar::aux::ConstDataPtr data) const
{
  for (const auto& plot_data : this->mPlotData)
  {
    if (plot_data.mData == data)
    {
      return true;
    }
  }
  return false;
}

void cedar::aux::gui::HistoryPlot0D::init()
{
  mpHistoryPlot = new cedar::aux::gui::QwtLinePlot();
  auto p_layout = new QVBoxLayout();
  p_layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(p_layout);
#ifdef CEDAR_USE_QWT
  mpHistoryPlot = new cedar::aux::gui::QwtLinePlot();
#endif // CEDAR_USE_QWT
  p_layout->addWidget(this->mpHistoryPlot);

  mMaxHistorySize = 500;
}

void cedar::aux::gui::HistoryPlot0D::advanceHistory()
{
  cedar::unit::Time time_now = cedar::aux::GlobalClockSingleton::getInstance()->getTime();
  for (auto& plot_data : this->mPlotData)
  {
    QReadLocker data_locker(&plot_data.mData->getLock());

    if (plot_data.mData->getDimensionality() != 0)
    {
      data_locker.unlock();
      emit dataChanged();
      return;
    }

    cv::Mat now = plot_data.mData->getData().clone();
    data_locker.unlock();

    QWriteLocker hist_locker(&plot_data.mHistory->getLock());
    const cv::Mat& current_hist = plot_data.mHistory->getData();

    cv::Mat new_hist
       = cv::Mat::zeros
         (
           std::min(current_hist.rows + 1, static_cast<int>(this->mMaxHistorySize)),
           1,
           now.type()
         );

    int start = 0;
    int end = current_hist.rows;

    if (current_hist.rows >= static_cast<int>(this->mMaxHistorySize))
    {
      start = 1;
    }

    cv::Mat slice = new_hist(cv::Range(0, end - start), cv::Range::all());
    current_hist(cv::Range(start, end), cv::Range::all()).copyTo(slice);
    cv::Mat slice2 = new_hist(cv::Range(new_hist.rows - 1, new_hist.rows), cv::Range::all());
    now.copyTo(slice2);

    plot_data.mHistory->setData(new_hist);

    plot_data.mXLabels.push_back(time_now);

    while (plot_data.mXLabels.size() > this->mMaxHistorySize)
    {
      plot_data.mXLabels.pop_front();
    }

    std::vector<double> time_values;
    time_values.resize(plot_data.mXLabels.size());
    for (size_t i = 0; i < plot_data.mXLabels.size(); ++i)
    {
      time_values.at(i) = (plot_data.mXLabels.at(i) - time_now) / (1.0 * cedar::unit::second);
    }
    auto time_annotation = plot_data.mHistory->getAnnotation<cedar::aux::annotation::Dimensions>();
    time_annotation->setSamplingPositions(0, time_values);

    CEDAR_DEBUG_NON_CRITICAL_ASSERT(static_cast<unsigned int>(new_hist.rows) == plot_data.mXLabels.size());
  }
}

void cedar::aux::gui::HistoryPlot0D::clear()
{
  for (const auto& plot_data : this->mPlotData)
  {
    if (this->mpHistoryPlot->canDetach(plot_data.mData))
    {
      this->mpHistoryPlot->detach(plot_data.mData);
    }
  }
  this->mPlotData.clear();
}

void cedar::aux::gui::HistoryPlot0D::plot(cedar::aux::ConstDataPtr data, const std::string& title)
{
  if (!this->canAppend(data))
  {
    CEDAR_THROW(cedar::aux::gui::InvalidPlotData, "Cannot plot this data.");
  }

  this->clear();

  this->append(data, title);

  this->startTimer(30);
}

void cedar::aux::gui::HistoryPlot0D::doAppend(cedar::aux::ConstDataPtr data, const std::string& title)
{
  this->mPlotData.push_back(PlotData());
  auto& plot_data = this->mPlotData.back();
  plot_data.mData = boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(data);
  plot_data.mHistory = cedar::aux::MatDataPtr(new cedar::aux::MatData(cv::Mat()));

  // initialize the new history to a 1x1 matrix.
  QReadLocker locker(&plot_data.mData->getLock());
  cv::Mat hist = plot_data.mData->getData().clone();
  locker.unlock();

  cv::Mat new_hist = cv::Mat::zeros(1, 1, hist.type());

  hist.copyTo(new_hist);

  QWriteLocker hlocker(&plot_data.mHistory->getLock());
  plot_data.mHistory->setData(new_hist);
  hlocker.unlock();

  cedar::aux::annotation::DimensionsPtr time_annotation(new cedar::aux::annotation::Dimensions(1));
  time_annotation->setLabel(0, "time [s]");
  plot_data.mXLabels.clear();
  plot_data.mXLabels.push_back(cedar::aux::GlobalClockSingleton::getInstance()->getTime());
  plot_data.mHistory->setAnnotation(time_annotation);

  this->mpHistoryPlot->append(plot_data.mHistory, title);
}

void cedar::aux::gui::HistoryPlot0D::doDetach(cedar::aux::ConstDataPtr data)
{
  for (auto iter = mPlotData.begin(); iter != mPlotData.end(); )
  {
    if (iter->mData == data)
    {
      this->mpHistoryPlot->detach(iter->mHistory);
      iter = mPlotData.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}

void cedar::aux::gui::HistoryPlot0D::timerEvent(QTimerEvent* /* pEvent */)
{
  if (!this->isVisible())
  {
    return;
  }

  this->advanceHistory();
}

#endif // CEDAR_USE_QWT
