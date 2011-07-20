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

    File:        EnumBase.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 05 27

    Description: Base class for enums.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_ENUM_BASE_H
#define CEDAR_AUX_ENUM_BASE_H

// LOCAL INCLUDES
#include "auxiliaries/namespace.h"
#include "auxiliaries/Enum.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <map>
#include <vector>
#include <string>

/*!@brief A base class for making enums that are more flexible than the standard ones.
 *
 * To make an enum using this class, create a new class that has a static construct function. In the construct function,
 * declare all the enum values belonging to your enum.
 * @todo explain this better and add example code
 */
template <class T>
class cedar::aux::EnumBase
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  EnumBase(const std::string& prefix = "")
  :
  mUndefined (Enum::UNDEFINED, prefix + "UNDEFINED")
  {
    T::construct();
    this->def(mUndefined);
  }

  //!@brief Destructor
  virtual ~EnumBase()
  {
  }

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*! @brief Define a new enum value.
   */
  void def(const Enum& rEnum)
  {
    //! @todo check for duplicate values
    this->mEnumFromId[rEnum.id()] = rEnum;
    this->mEnumFromString[rEnum.name()] = rEnum;
    this->mEnumList.push_back(rEnum);
  }

  /*! @brief Retrieve the enum value corresponding to id.
   */
  const Enum& get(EnumId id) const
  {
    std::map<EnumId, Enum>::const_iterator it;
    it = this->mEnumFromId.find(id);
    if (it != this->mEnumFromId.end())
    {
      return it->second;
    }
    else
    {
      return this->mUndefined;
    }
  }

  /*! @brief Retrieve the enum value corresponding to a string.
   *
   * @returns cedar::aux::Enum::UNDEFINED if there is no value corresponding to the string.
   */
  const Enum& get(const std::string& id) const
  {
    std::map<std::string, Enum>::const_iterator it;
    it = this->mEnumFromString.find(id);
    if (it != this->mEnumFromString.end())
    {
      return it->second;
    }
    else
    {
      return this->mUndefined;
    }
  }

  /*! @brief Returns a list containing all enum values in this class (including cedar::aux::Enum::UNDEFINED!).
   */
  const std::vector<Enum>& list() const
  {
    return this->mEnumList;
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
  // none yet
private:
  //! Map from EnumIds to enum values.
  std::map<cedar::aux::EnumId, cedar::aux::Enum> mEnumFromId;

  //! Map from strings to enum values.
  std::map<std::string, cedar::aux::Enum> mEnumFromString;

  //! List of all enums in this class.
  std::vector<cedar::aux::Enum> mEnumList;

  //! Value for undefined/invalid enums (needed for returning a reference).
  cedar::aux::Enum mUndefined;

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet

private:
  // none yet

}; // class cedar::aux::EnumBase

#endif // CEDAR_AUX_ENUM_BASE_H
