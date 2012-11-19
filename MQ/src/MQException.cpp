/*
 * Copyright 2010 MQWeb - Franky Braem
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they
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
#include "cmqc.h"

#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>

#include <MQ/MQException.h>

namespace MQ
{

  MQException::MQException(const std::string& object,
                           const std::string& function,
                           long code, 
                           long reason) : Poco::Exception(reason)
                                        , _object(object)
                                        , _function(function)
                                        , _code(code)
                                        , _reason(reason)
  {
    std::string msg;

    if ( ! _object.empty() )
    {
      msg.append(_object);
      msg.append(" - ");
    }
    msg.append(_function);
    msg.append(" : RC=");

    Poco::NumberFormatter::append(msg, _reason);
    message(msg);
  }

  void MQException::log(Poco::Logger& logger) const
  {
    if ( _code == MQCC_WARNING )
    {
      logger.warning(message());
    }
    else
    {
      logger.error(message());
    }
  }

  const char* MQException::name() const throw()
  {
    return "MQException";
  }

  const char* MQException::what() const throw()
  {
    return "MQException";
  }

}
