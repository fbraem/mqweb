/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or  as soon they
 * will be approved by the European Commission - subsequent
 * versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 * Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://joinup.ec.europa.eu/software/page/eupl
 *
 * Unless required by applicable law or agreed to in
 * writing, software distributed under the Licence is
 * distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the Licence for the specific language governing
 * permissions and limitations under the Licence.
 */
#ifndef _MQWeb_Mapper_H
#define _MQWeb_Mapper_H

#include "Poco/JSON/Object.h"

namespace MQ {
namespace Web {


class Mapper
  /// Interface for mapping objects (Data Mapper pattern)
{
public:

  virtual void change(const Poco::JSON::Object::Ptr&obj) = 0;


  virtual void create(const Poco::JSON::Object::Ptr& obj, bool replace = false) = 0;


  virtual void copy(const Poco::JSON::Object::Ptr& obj, bool replace = false) = 0;


  virtual Poco::JSON::Array::Ptr inquire(const Poco::JSON::Object::Ptr& filter) = 0;
};


}} // Namespace MQ::Web

#endif // _MQWeb_Mapper_H
