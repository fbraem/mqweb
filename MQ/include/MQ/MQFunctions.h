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
#include <cmqc.h>
#include <cmqxc.h>

#include <string>
#include <vector>
#include <map>

#include <Poco/SharedLibrary.h>
#include <Poco/Mutex.h>

#ifndef _MQ_MQFunctions_H
#define _MQ_MQFunctions_H

namespace MQ {


typedef void (*ConnFn)(PMQCHAR, PMQHCONN, PMQLONG, PMQLONG);


typedef void (*ConnxFn)(PMQCHAR, PMQCNO, PMQHCONN, PMQLONG, PMQLONG);


typedef void (*OpenFn)(MQHCONN, PMQOD, MQLONG, PMQHOBJ, PMQLONG, PMQLONG);


typedef void (*PutFn) (MQHCONN, MQHOBJ, PMQMD, PMQPMO, MQLONG, PMQBYTE, PMQLONG, PMQLONG);


typedef void (*GetFn) (MQHCONN, MQHOBJ, PMQMD, PMQGMO, MQLONG, PMQBYTE, PMQLONG, PMQLONG, PMQLONG);


typedef void (*CloseFn)(MQHCONN, PMQHOBJ, MQLONG, PMQLONG, PMQLONG);


typedef void (*DiscFn)(PMQHCONN, PMQLONG, PMQLONG);


typedef void (*InqFn)(MQHCONN, MQHOBJ, MQLONG, PMQLONG, MQLONG, PMQLONG, MQLONG, PMQCHAR, PMQLONG, PMQLONG);

class MQFunctions
{
public:
	MQFunctions();


	~MQFunctions();


	bool isOk() const;


	void loadLibrary(const std::string& mqLibrary);


	MQHCONN conn(const std::string& qmgrName);


	void conn(PMQCHAR qmgrName, PMQHCONN hconn, PMQLONG cc, PMQLONG rc);

	void connx(PMQCHAR qmgrName, PMQCNO connectOpts, PMQHCONN hconn, PMQLONG cc, PMQLONG rc);


	MQHCONN connx(const std::string& qmgrName, PMQCNO connectOpts);


	void open(MQHCONN conn, PMQOD od, MQLONG options, PMQHOBJ obj, PMQLONG cc, PMQLONG rc);


	MQHOBJ open(MQHCONN conn, PMQOD od, MQLONG options);


	void put(MQHCONN conn, MQHOBJ obj, PMQMD md, PMQPMO options, MQLONG size, PMQBYTE buffer, PMQLONG cc, PMQLONG rc);


	void put(MQHCONN conn, MQHOBJ obj, PMQMD md, PMQPMO options, MQLONG size, PMQBYTE buffer);


	void get(MQHCONN conn, MQHOBJ obj, PMQMD md, PMQGMO options, MQLONG size, PMQBYTE buffer, PMQLONG dataLength, PMQLONG cc, PMQLONG rc);


	void get(MQHCONN conn, MQHOBJ obj, PMQMD md, PMQGMO options, MQLONG size, PMQBYTE buffer, PMQLONG dataLength);


	void close(MQHCONN conn, MQHOBJ* obj, MQLONG options, PMQLONG cc, PMQLONG rc);


	void close(MQHCONN conn, MQHOBJ* obj, MQLONG options);

	void disc(PMQHCONN conn, PMQLONG cc, PMQLONG rc);


	void disc(PMQHCONN conn);


	void inq(MQHCONN conn, MQHOBJ obj, MQLONG selectorCount, PMQLONG selectors, MQLONG intAttrCount, PMQLONG intAttrs, MQLONG charAttrLength, PMQCHAR charAttrs, PMQLONG cc, PMQLONG rc);


	void inq(MQHCONN conn, MQHOBJ obj, MQLONG selectorCount, PMQLONG selectors, MQLONG intAttrCount, PMQLONG intAttrs, MQLONG charAttrLength, PMQCHAR charAttrs);


	void inq(MQHCONN conn, MQHOBJ obj, const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult);


private:
	Poco::SharedLibrary _dll;


	ConnFn _connFn;


	ConnxFn _connxFn;


	OpenFn _openFn;


	PutFn _putFn;


	GetFn _getFn;


	CloseFn _closeFn;


	DiscFn _discFn;


	InqFn _inqFn;


	static Poco::Mutex _MQIMutex;


	void trace(const std::string& subject, const std::string& function, PMQLONG cc, PMQLONG rc);
};

inline bool MQFunctions::isOk() const
{
	return _dll.isLoaded();
}

} // namespace MQ

#endif // _MQ_MQFunctions_H
