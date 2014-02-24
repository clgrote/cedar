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

    File:        ArithmeticExpressionEvaluator.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2014 02 24

    Description: Header file for the class cedar::aux::ArithmeticExpressionEvaluator.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_ARITHMETIC_EXPRESSION_EVALUATOR_H
#define CEDAR_AUX_ARITHMETIC_EXPRESSION_EVALUATOR_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES

// FORWARD DECLARATIONS
#include "cedar/auxiliaries/ArithmeticExpressionEvaluator.fwd.h"

// SYSTEM INCLUDES
#include <map>


/*!@brief A simple class for evaluating arithmetic expressions.
 */
class cedar::aux::ArithmeticExpressionEvaluator
{
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
private:

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  ArithmeticExpressionEvaluator();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  double evaluate(const std::string& expression, const std::map<std::string, double>& variables = (std::map<std::string, double>())) const;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

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
//  CalculatorGrammarPtr mGrammar;

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  // none yet

}; // class cedar::aux::ArithmeticExpressionEvaluator

#endif // CEDAR_AUX_ARITHMETIC_EXPRESSION_EVALUATOR_H

