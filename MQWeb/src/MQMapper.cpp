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
#include <MQ/Web/MQMapper.h>

namespace MQ {
namespace Web {

MQMapper::MQMapper(CommandServer& commandServer) : _commandServer(commandServer)
{
}

MQMapper::~MQMapper()
{
}

void MQMapper::mapNumberToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int parameter)
{
  if ( pcf->hasParameter(parameter) )
  {
    json->set(name, pcf->getParameterNum(parameter));
  }
}


void MQMapper::mapStringToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int parameter)
{
  if (    pcf->hasParameter(parameter) )
  {
    json->set(name, pcf->getParameterString(parameter));
  }
}


void MQMapper::mapDateToJSON(const PCF::Ptr& pcf, Poco::JSON::Object::Ptr& json, const std::string& name, int dateParameter, int timeParameter)
{
  if (    pcf->hasParameter(dateParameter) 
       && pcf->hasParameter(timeParameter) )
  {
    Poco::DateTime dateTime = pcf->getParameterDate(dateParameter, timeParameter);
    json->set(name, Poco::DateTimeFormatter::format(dateTime, "%d-%m-%Y %H:%M:%S"));
  }
}


}} //  Namespace MQ::Web
