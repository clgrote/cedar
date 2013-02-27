/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        MatDataPlot.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 08 28

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/MatDataPlot.h"
#include "cedar/auxiliaries/gui/PlotManager.h"
#include "cedar/auxiliaries/gui/PlotDeclaration.h"
#include "cedar/auxiliaries/gui/ImagePlot.h"
#include "cedar/auxiliaries/gui/MatrixPlot.h"
#include "cedar/auxiliaries/gui/exceptions.h"
#include "cedar/auxiliaries/annotation/ColorSpace.h"
#include "cedar/auxiliaries/MatData.h"

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <QLayout>

//----------------------------------------------------------------------------------------------------------------------
// type registration
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registerPlot()
  {
    using cedar::aux::MatData;
    using cedar::aux::gui::MatDataPlot;

    typedef cedar::aux::gui::PlotDeclarationTemplate<MatData, MatDataPlot> DeclarationType;

    boost::shared_ptr<DeclarationType> declaration(new DeclarationType());
    declaration->declare();
    cedar::aux::gui::PlotManagerSingleton::getInstance()->setDefault<MatData, MatDataPlot>();
    return true;
  }

  bool registered = registerPlot();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gui::MatDataPlot::MatDataPlot(QWidget *pParent)
:
cedar::aux::gui::MultiPlotInterface(pParent),
mpCurrentPlotWidget(NULL)
{
  QVBoxLayout *p_layout = new QVBoxLayout();
  this->setLayout(p_layout);

  this->setContentsMargins(0, 0, 0, 0);
  p_layout->setContentsMargins(0, 0, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

bool cedar::aux::gui::MatDataPlot::canAppend(cedar::aux::ConstDataPtr data) const
{
  if (this->mpCurrentPlotWidget == NULL)
  {
    return false;
  }
  else if
  (
    cedar::aux::gui::MultiPlotInterface *p_multi_plot
      = dynamic_cast<cedar::aux::gui::MultiPlotInterface*>(this->mpCurrentPlotWidget)
  )
  {
    return p_multi_plot->canAppend(data);
  }
  else
  {
    return false;
  }
}

void cedar::aux::gui::MatDataPlot::doAppend(cedar::aux::ConstDataPtr data, const std::string& title)
{
  CEDAR_DEBUG_ASSERT(this->mpCurrentPlotWidget != NULL);
  cedar::aux::gui::MultiPlotInterface *p_multi_plot
    = dynamic_cast<cedar::aux::gui::MultiPlotInterface*>(this->mpCurrentPlotWidget);

  CEDAR_DEBUG_ASSERT(p_multi_plot != NULL);
  p_multi_plot->append(data, title);
}

void cedar::aux::gui::MatDataPlot::plot(cedar::aux::ConstDataPtr data, const std::string& title)
{
  this->mData= boost::shared_dynamic_cast<cedar::aux::ConstMatData>(data);
  if (!this->mData)
  {
    CEDAR_THROW(cedar::aux::gui::InvalidPlotData,
                "Cannot cast to cedar::aux::MatData in cedar::aux::gui::MatrixPlot::display.");
  }

  if (this->mpCurrentPlotWidget)
  {
    delete this->mpCurrentPlotWidget;
    this->mpCurrentPlotWidget = NULL;
  }

  try
  {
    // data should be plotted as an image
    cedar::aux::annotation::ConstColorSpacePtr color_space = this->mData->getAnnotation<cedar::aux::annotation::ColorSpace>();
    cedar::aux::gui::ImagePlot* p_plot = new cedar::aux::gui::ImagePlot();
    p_plot->plot(this->mData, title);
    this->mpCurrentPlotWidget = p_plot;
  }
  catch(cedar::aux::AnnotationNotFoundException&)
  {
    // data should be plotted as a matrix
    cedar::aux::gui::MatrixPlot* p_plot = new cedar::aux::gui::MatrixPlot();
    p_plot->plot(this->mData, title);
    this->mpCurrentPlotWidget = p_plot;
  }
  this->layout()->addWidget(this->mpCurrentPlotWidget);
}
