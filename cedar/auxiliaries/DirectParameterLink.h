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

    File:        DirectParameterLink.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2014 02 18

    Description: Header file for the class cedar::aux::DirectParameterLink.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_DIRECT_PARAMETER_LINK_H
#define CEDAR_AUX_DIRECT_PARAMETER_LINK_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/ParameterLink.h"

// FORWARD DECLARATIONS
#include "cedar/auxiliaries/DirectParameterLink.fwd.h"

// SYSTEM INCLUDES


/*!@brief A parameter link that sets the linked parameters to the same value.
 */
class cedar::aux::DirectParameterLink : public cedar::aux::ParameterLink
{
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  DirectParameterLink();

  //!@brief Destructor
  virtual ~DirectParameterLink();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  void leftChanged();

  void rightChanged();

  bool checkIfLinkable(cedar::aux::ConstParameterPtr left, cedar::aux::ConstParameterPtr right) const;

  void assign(cedar::aux::ConstParameterPtr source, cedar::aux::ParameterPtr target) const;

  bool isNumeric(cedar::aux::ConstParameterPtr parameter) const;

  std::string getNumericType(cedar::aux::ConstParameterPtr parameter) const;

  double getValue(cedar::aux::ConstParameterPtr source) const;

  void setValue(cedar::aux::ParameterPtr target, double value) const;

  template <typename T> double getNumericValue(cedar::aux::ConstParameterPtr source) const;

  template <typename T> void setNumericValue(cedar::aux::ParameterPtr source, double value) const;


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

}; // class cedar::aux::DirectParameterLink

#endif // CEDAR_AUX_DIRECT_PARAMETER_LINK_H

