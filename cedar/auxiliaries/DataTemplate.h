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

    File:        DataTemplate.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 05 23

    Description: Header for the cedar::aux::DataTemplate.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_DATA_TEMPLATE_H
#define CEDAR_AUX_DATA_TEMPLATE_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/Data.h"

// SYSTEM INCLUDES
#include <QReadWriteLock>

/*!@brief   A templated version of cedar::aux::Data.
 *
 * @tparam  T Type stored in the data object.
 *
 * @remarks Usually, you should not include the header for this class directly. Rather, include the headers of its
 *          instantiations: e.g., for cedar::aux::MatData, include cedar/auxiliaries/MatData.h.
 */
template <typename T>
class cedar::aux::DataTemplate : public cedar::aux::Data
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // typedefs
  //--------------------------------------------------------------------------------------------------------------------
public:
  //! The type being stored in this data object.
  typedef T DataType;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  DataTemplate()
  {
  }

  //!@brief This constructor initializes the internal data to a value.
  DataTemplate(const T& value)
  {
    this->mData = value;
  }

  //!@brief Destructor
  virtual ~DataTemplate()
  {
  }

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:

  DataPtr clone()
  {
	  boost::shared_ptr<DataTemplate<T>> neu(new DataTemplate<T>());
	  neu->mData = this->mData;
	  return neu;
  }

  //!@brief returns the internal data
  T& getData()
  {
    return this->mData;
  }

  //!@brief returns the internal data as const
  const T& getData() const
  {
    return this->mData;
  }

  //!@brief sets the internal data to the given data/value
  void setData(const T& data)
  {
    this->mData = data;
  }

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
  //!@brief the internal data
  T mData;

private:
  // none yet

}; // class cedar::aux::DataTemplate

#endif // CEDAR_AUX_DATA_TEMPLATE_H

