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
#include <MQ/MQFunctions.h>
#include <MQ/MQException.h>

#include <Poco/ScopedLock.h>
#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>

#ifdef __linux__
#include <string.h>
#endif

namespace MQ
{

Poco::NamedMutex MQFunctions::_MQIMutex("MQIMutex");

MQFunctions::MQFunctions() :   
    _connFn(NULL)
  , _connxFn(NULL)
  , _openFn(NULL)
  , _closeFn(NULL)
  , _putFn(NULL)
  , _getFn(NULL)
  , _discFn(NULL)
  , _inqFn(NULL)
{
}

MQFunctions::~MQFunctions()
{
  if ( _dll.isLoaded() )
  {
    _dll.unload();
  }
}

void MQFunctions::loadLibrary(const std::string& mqLibrary)
{
  if ( ! _dll.isLoaded() )
  {
     _dll.load(mqLibrary);
  }

  _connFn = (ConnFn) _dll.getSymbol("MQCONN");
  _connxFn = (ConnxFn) _dll.getSymbol("MQCONNX");
  _openFn = (OpenFn) _dll.getSymbol("MQOPEN");
  _closeFn = (CloseFn) _dll.getSymbol("MQCLOSE");
  _putFn = (PutFn) _dll.getSymbol("MQPUT");
  _getFn = (GetFn) _dll.getSymbol("MQGET");
  _discFn = (DiscFn) _dll.getSymbol("MQDISC");
  _inqFn = (InqFn) _dll.getSymbol("MQINQ");
}

void MQFunctions::conn(char* qmgrName, MQHCONN* hconn, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_connFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _connFn(qmgrName, hconn, cc, rc);

  trace(qmgrName, "MQCONN", cc, rc);
}


MQHCONN MQFunctions::conn(const std::string& qmgrName)
{
  MQCHAR48 name;
  strncpy(name, qmgrName.c_str(), sizeof(MQCHAR48));

  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;
  MQHCONN qmgr = 0;
  
  conn(name, &qmgr, &cc, &rc);
  if ( cc != MQCC_OK )
  {
    throw MQException(qmgrName, "MQCONN", cc, rc);
  }

  return qmgr;
}


void MQFunctions::connx(char* qmgrName, MQCNO* connectOpts, MQHCONN* hconn, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_connxFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _connxFn(qmgrName, connectOpts, hconn, cc, rc);

  trace(qmgrName, "MQCONNX", cc, rc);
}

MQHCONN MQFunctions::connx(const std::string& qmgrName, MQCNO* connectOpts)
{
  MQCHAR48 name;
  memset(name, '0', sizeof(MQCHAR48));
  strncpy(name, qmgrName.c_str(), sizeof(MQCHAR48));

  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;
  MQHCONN qmgr = 0;
  
  connx(name, connectOpts, &qmgr, &cc, &rc);
  if ( cc != MQCC_OK )
  {
    throw MQException(qmgrName, "MQCONNX", cc, rc);
  }

  return qmgr;
}

void MQFunctions::open(MQHCONN conn, MQOD* od, MQLONG options, MQHOBJ* obj, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_openFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _openFn(conn, od, options, obj, cc, rc);

  trace(od->ObjectName, "MQOPEN", cc, rc);
}

MQHOBJ MQFunctions::open(MQHCONN conn, MQOD* od, MQLONG options)
{
  MQHOBJ obj = 0;
  
  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;
  open(conn, od, options, &obj, &cc, &rc);

  if ( cc != MQCC_OK )
  {
    std::string name(od->ObjectName, MQ_OBJECT_NAME_LENGTH);
    name.erase(name.find_last_not_of(" \n\r\t")+1); // trim

    throw MQException(name, "MQOPEN", cc, rc);
  }

  return obj;
}

void MQFunctions::put(MQHCONN conn, MQHOBJ obj, MQMD* md, MQPMO* options, MQLONG size, MQBYTE* buffer, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_putFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _putFn(conn, obj, md, options, size, buffer, cc, rc);

  trace("", "MQPUT", cc, rc);
}

void MQFunctions::put(MQHCONN conn, MQHOBJ obj, MQMD* md, MQPMO* options, MQLONG size, MQBYTE* buffer)
{
  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;

  put(conn, obj, md, options, size, buffer, &cc, &rc);

  if ( cc != MQCC_OK )
  {
    throw MQException("", "MQPUT", cc, rc);
  }
}


void MQFunctions::get(MQHCONN conn, MQHOBJ obj, MQMD* md, MQGMO* options, MQLONG size, MQBYTE* buffer, PMQLONG dataLength, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_getFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _getFn(conn, obj, md, options, size, buffer, dataLength, cc, rc);

  trace("", "MQGET", cc, rc);
}

void MQFunctions::get(MQHCONN conn, MQHOBJ obj, MQMD* md, MQGMO* options, MQLONG size, MQBYTE* buffer, PMQLONG dataLength)
{
  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;

  get(conn, obj, md, options, size, buffer, dataLength, &cc, &rc);

  if ( cc != MQCC_OK )
  {
    throw MQException("", "MQGET", cc, rc);
  }
}

void MQFunctions::close(MQHCONN conn, MQHOBJ* obj, MQLONG options, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_closeFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _closeFn(conn, obj, options, cc, rc);

  trace("", "MQCLOSE", cc, rc);
}

void MQFunctions::close(MQHCONN conn, MQHOBJ* obj, MQLONG options)
{
  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;

  close(conn, obj, options, &cc, &rc);
  if ( cc != MQCC_OK )
  {
    throw MQException("", "MQCLOSE", cc, rc);
  }
}

void MQFunctions::disc(PMQHCONN conn, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_discFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _discFn(conn, cc, rc);
}

void MQFunctions::disc(PMQHCONN conn)
{
  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;

  disc(conn, &cc, &rc);
  if ( cc != MQCC_OK )
  {
    throw MQException("", "MQDISC", cc, rc);
  }
}

void MQFunctions::inq(MQHCONN conn, MQHOBJ obj, MQLONG selectorCount, PMQLONG selectors, MQLONG intAttrCount, PMQLONG intAttrs, MQLONG charAttrLength, PMQCHAR charAttrs, PMQLONG cc, PMQLONG rc)
{
  poco_assert_dbg(_inqFn != NULL);

  Poco::ScopedLock<Poco::NamedMutex> lock(_MQIMutex);
  _inqFn(conn, obj, selectorCount, selectors, intAttrCount, intAttrs, charAttrLength, charAttrs, cc, rc);
}

void MQFunctions::inq(MQHCONN conn, MQHOBJ obj, MQLONG selectorCount, PMQLONG selectors, MQLONG intAttrCount, PMQLONG intAttrs, MQLONG charAttrLength, PMQCHAR charAttrs)
{
  MQLONG cc = MQCC_OK;
  MQLONG rc = MQRC_NONE;

  inq(conn, obj, selectorCount, selectors, intAttrCount, intAttrs, charAttrLength, charAttrs, &cc, &rc);
  if ( cc != MQCC_OK )
  {
    throw MQException("", "MQINQ", cc, rc);
  }
}

void MQFunctions::inq(MQHCONN conn, MQHOBJ obj, const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult)
{
  std::vector<MQLONG> selectors;
  for(std::vector<int>::const_iterator it = intSelectors.begin(); it != intSelectors.end(); it++)
  {
    selectors.push_back(*it);
  }

  std::vector<MQLONG> intAttrs(intSelectors.size());

  MQLONG charAttrLength = 0;
  for(std::map<int, int>::const_iterator it = charSelectors.begin(); it != charSelectors.end(); it++)
  {
    selectors.push_back(it->first);
    charAttrLength += it->second;
  }
  std::vector<MQCHAR> charAttrs(charAttrLength);

  inq(conn, obj, selectors.size(), selectors.size() == 0 ? NULL : &selectors[0], 
                 intAttrs.size(), intAttrs.size() == 0 ? NULL : &intAttrs[0], 
                 charAttrs.size(), charAttrs.size() == 0 ? NULL : &charAttrs[0]);

  for(int i = 0; i < intAttrs.size(); i++)
  {
    intResult[intSelectors[i]] = intAttrs[i];
  }

  std::string result(charAttrs.begin(), charAttrs.end());
  for(std::map<int, int>::const_iterator it = charSelectors.begin(); it != charSelectors.end(); it++)
  {
    std::string value = result.substr(0, it->second);
    result.erase(0, it->second);
    value.erase(value.find_last_not_of(" \n\r\t")+1); // trim

    charResult[it->first] = value;
  }
}

void MQFunctions::trace(const std::string& subject, const std::string& function, PMQLONG cc, PMQLONG rc)
{
  Poco::Logger& logger = Poco::Logger::get("mq");
  if ( logger.trace() )
  {
    std::string msg;
    if ( ! subject.empty() )
    {
      msg.append(subject);
      msg.append(" - ");
    }
    msg.append(function);
    msg.append(" : CC=");
    Poco::NumberFormatter::append(msg, *cc);
    msg.append(" : RC=");
    Poco::NumberFormatter::append(msg, *rc);

    logger.trace(msg);
  }
}

} // Namespace MQWeb
