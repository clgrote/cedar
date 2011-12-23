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

    File:        EnumParameter.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 28

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_ENUM_PARAMETER_H
#define CEDAR_AUX_ENUM_PARAMETER_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/Parameter.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/auxiliaries/EnumBase.h"

// SYSTEM INCLUDES
#include <set>

/*!@brief A parameter storing an enum value.
 *
 * More detailed description of the class coming soon.
 */
class cedar::aux::EnumParameter : public cedar::aux::Parameter
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  EnumParameter(cedar::aux::Configurable *pOwner,
                const std::string& name,
                boost::shared_ptr<cedar::aux::EnumBase> enumBase);

  //!@brief The standard constructor, with an additional default value.
  EnumParameter(cedar::aux::Configurable *pOwner,
                const std::string& name,
                boost::shared_ptr<cedar::aux::EnumBase> enumBase,
                cedar::aux::EnumId defaultValue);

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief read from a configuration node
  void setTo(const cedar::aux::ConfigurationNode& root);
  //!@brief write to a configuration node
  void putTo(cedar::aux::ConfigurationNode& root) const;
  //!@brief set enum value to default
  void makeDefault();
  //!@brief return the enum value
  cedar::aux::Enum getValue() const;
  //!@brief set enum value to a specified id
  void set(const std::string& enumId);
  //!@brief get the enum from which this parameter represents an entry
  const cedar::aux::EnumBase& getEnumDeclaration()
  {
    return *(this->mEnumDeclaration);
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
  //!@brief the enum value of this parameter
  cedar::aux::EnumId mValue;
  //!@brief the default value of this parameter
  cedar::aux::EnumId mDefault;
  //!@brief a pointer to the enum used by this parameter
  boost::shared_ptr<cedar::aux::EnumBase> mEnumDeclaration;
}; // class cedar::aux::VectorParameter

#endif // CEDAR_AUX_ENUM_PARAMETER_H
