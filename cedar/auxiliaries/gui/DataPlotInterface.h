/*======================================================================================================================

    Copyright 2011 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        DataPlotInterface.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 28

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GUI_DATA_PLOT_INTERFACE_H
#define CEDAR_AUX_GUI_DATA_PLOT_INTERFACE_H

// LOCAL INCLUDES
#include "cedar/auxiliaries/gui/namespace.h"
#include "cedar/auxiliaries/namespace.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QWidget>

/*!@brief A unified interface for widgets that plot instances of cedar::proc::Data.
 */
class cedar::aux::gui::DataPlotInterface : public QWidget
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------
  Q_OBJECT
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  DataPlotInterface(QWidget *pParent = NULL);

  //!@brief Destructor
  virtual ~DataPlotInterface();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Displays the data in the plot widget.
   *
   *        This is the main method that is called whenever a data plotter is requested to plot a different data.
   *
   * @param data The data to display.
   */
  virtual void display(cedar::aux::DataPtr data) = 0;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
signals:
  //! Signal that is emitted when the plotter detects a change in data that it cannot handle.
  void dataChanged();

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  // none yet

}; // class cedar::aux::gui::DataPlotInterface

#endif // CEDAR_AUX_GUI_DATA_PLOT_INTERFACE_H

