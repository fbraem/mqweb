/*
* Copyright 2017 - KBC Group NV - Franky Braem - The MIT license
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <cmqc.h>
#include <cmqxc.h>

#include <string>
#include <vector>
#include <map>

#include "Poco/SharedLibrary.h"

#ifndef _MQ_MQFunctions_H
#define _MQ_MQFunctions_H

namespace MQ {


typedef void (*ConnFn)(MQCHAR*, MQHCONN*, MQLONG*, MQLONG*);


typedef void (*ConnxFn)(MQCHAR*, MQCNO*, MQHCONN*, MQLONG*, MQLONG*);


typedef void (*OpenFn)(MQHCONN, MQOD*, MQLONG, MQHOBJ*, MQLONG*, MQLONG*);


typedef void (*PutFn) (MQHCONN, MQHOBJ, MQMD*, MQPMO*, MQLONG, MQBYTE*, MQLONG*, MQLONG*);


typedef void (*GetFn) (MQHCONN, MQHOBJ, MQMD*, MQGMO*, MQLONG, MQBYTE*, MQLONG*, MQLONG*, MQLONG*);


typedef void (*CloseFn)(MQHCONN, MQHOBJ*, MQLONG, MQLONG*, MQLONG*);


typedef void (*DiscFn)(MQHCONN*, MQLONG*, MQLONG*);


typedef void (*InqFn)(MQHCONN, MQHOBJ, MQLONG, MQLONG*, MQLONG, MQLONG*, MQLONG, MQCHAR*, MQLONG*, MQLONG*);


typedef void (*CbFn)(MQHCONN, MQLONG, MQCBD*, MQHOBJ, MQMD*, MQGMO*, MQLONG*, MQLONG*);


typedef void (*CtlFn)(MQHCONN, MQLONG, MQCTLO*, MQLONG*, MQLONG*);


typedef void (*CrtMhFn)(MQHCONN, MQCMHO*, MQHMSG*, MQLONG*, MQLONG*);


typedef void (*DltMhFn)(MQHCONN, MQHMSG*, MQDMHO*, MQLONG*, MQLONG*);


typedef void (*SetMpFn)(MQHCONN, MQHMSG, MQSMPO*, MQCHARV*, MQPD*, MQLONG, MQLONG, MQBYTE*, MQLONG*, MQLONG*);


typedef void (*DltMpFn)(MQHCONN, MQHMSG, MQDMPO*, MQCHARV*, MQLONG*, MQLONG*);


typedef void (*InqMpFn)(MQHCONN, MQHMSG, MQIMPO*, MQCHARV*, MQPD*, MQLONG*, MQLONG, MQBYTE*, MQLONG*, MQLONG*, MQLONG*);

class MQFunctions
	/// Helper class for calling Websphere MQ functions dynamically. Depending on the loaded library
	/// (binding or client) the correct function will be called.
{
public:
	MQFunctions();
		/// Constructor

	~MQFunctions();
		/// Destructor

	bool isOk() const;
		/// Returns true when the library was loaded successfully.


	void loadLibrary(const std::string& mqLibrary);
		/// Loads the library and attaches our methods to the corresponding Websphere MQ function.


	MQHCONN conn(const std::string& qmgrName);
		/// Calls MQCONN. Can throw an MQException.

	void conn(MQCHAR* qmgrName, MQHCONN* hconn, MQLONG* cc, MQLONG* rc);
		/// Calls MQCONN. Doesn't throw an MQException.

	void connx(MQCHAR* qmgrName, MQCNO* connectOpts, MQHCONN* hconn, MQLONG* cc, MQLONG* rc);
		/// Calls MQCONNX. Doesn't throw an MQException.

	MQHCONN connx(const std::string& qmgrName, MQCNO* connectOpts);
		/// Calls MQCONNX. Can throw an MQException.

	void open(MQHCONN conn, MQOD* od, MQLONG options, MQHOBJ* obj, MQLONG* cc, MQLONG* rc);
		/// Calls MQOPEN. Doesn't throw an MQException.

	MQHOBJ open(MQHCONN conn, MQOD* od, MQLONG options);
		/// Calls MQOPEN. Can throw an MQException.

	void put(MQHCONN conn, MQHOBJ obj, MQMD* md, MQPMO* options, MQLONG size, MQBYTE* buffer, MQLONG* cc, MQLONG* rc);
		/// Calls MQPUT. Doesn't throw an MQException.

	void put(MQHCONN conn, MQHOBJ obj, MQMD* md, MQPMO* options, MQLONG size, MQBYTE* buffer);
		/// Calls MQPUT. Can throw an MQException.

	void get(MQHCONN conn, MQHOBJ obj, MQMD* md, MQGMO* options, MQLONG size, MQBYTE* buffer, MQLONG* dataLength, MQLONG* cc, MQLONG* rc);
		/// Calls MQGET. Doesn't throw an MQException.

	void get(MQHCONN conn, MQHOBJ obj, MQMD* md, MQGMO* options, MQLONG size, MQBYTE* buffer, MQLONG* dataLength);
		/// Calls MQGET. Can throw an MQException.

	void close(MQHCONN conn, MQHOBJ* obj, MQLONG options, MQLONG* cc, MQLONG* rc);
		/// Calls MQCLOSE. Doesn't throw an MQException.

	void close(MQHCONN conn, MQHOBJ* obj, MQLONG options);
		/// Calls MQCLOSE. Can throw an MQException.

	void disc(MQHCONN* conn, MQLONG* cc, MQLONG* rc);
		/// Calls MQDISC. Doesn't throw an MQException.

	void disc(MQHCONN* conn);
		/// Calls MQDISC. Can throw an MQException.

	void inq(MQHCONN conn, MQHOBJ obj, MQLONG selectorCount, MQLONG* selectors, MQLONG intAttrCount, MQLONG* intAttrs, MQLONG charAttrLength, MQCHAR* charAttrs, MQLONG* cc, MQLONG* rc);
		/// Calls MQINQ. Doesn't throw an MQException.

	void inq(MQHCONN conn, MQHOBJ obj, MQLONG selectorCount, MQLONG* selectors, MQLONG intAttrCount, MQLONG* intAttrs, MQLONG charAttrLength, MQCHAR* charAttrs);
		/// Calls MQCLOSE. Can throw an MQException.

	void inq(MQHCONN conn, MQHOBJ obj, const std::vector<int>& intSelectors, const std::map<int, int>& charSelectors, std::map<int, int>& intResult, std::map<int, std::string>& charResult);
		/// Calls MQCLOSE. Can throw an MQException.

	void cb(MQHCONN conn, MQLONG operation, MQCBD* callbackDesc, MQHOBJ obj, MQMD* md, MQGMO* gmo, MQLONG* cc, MQLONG* rc);
		/// Calls MQCB. Doesn't throw an MQException.

	void cb(MQHCONN conn, MQLONG operation, MQCBD* callbackDesc, MQHOBJ obj, MQMD* md, MQGMO* gmo);
		/// Calls MQCB. Can throw an MQException.

	void ctl(MQHCONN conn, MQLONG operation, MQCTLO* options, MQLONG* cc, MQLONG* rc);
		/// Calls MQCTL. Doesn't throw an MQException.

	void ctl(MQHCONN conn, MQLONG operation, MQCTLO* options);
		/// Calls MQCTL. Can throw an MQException.

	void crtmh(MQHCONN conn, MQCMHO* options, MQHMSG* hmsg, MQLONG* cc, MQLONG* rc);

	void crtmh(MQHCONN conn, MQCMHO* options, MQHMSG* hmsg);

	void dltmh(MQHCONN conn, MQHMSG* hmsg, MQDMHO* options, MQLONG* cc, MQLONG* rc);

	void dltmh(MQHCONN conn, MQHMSG* hmsg, MQDMHO* options);

	void setmp(MQHCONN conn, MQHMSG hmsg, MQSMPO* options, MQCHARV* name, MQPD* propDesc, MQLONG type, MQLONG valueLength, MQBYTE* value, MQLONG* cc, MQLONG* rc);

	void setmp(MQHCONN conn, MQHMSG hmsg, MQSMPO* options, MQCHARV* name, MQPD* propDesc, MQLONG type, MQLONG valueLength, MQBYTE* value);

	void dltmp(MQHCONN conn, MQHMSG hmsg, MQDMPO* options, MQCHARV* name, MQLONG* cc, MQLONG* rc);

	void dltmp(MQHCONN conn, MQHMSG hmsg, MQDMPO* options, MQCHARV* name);

	void inqmp(MQHCONN conn, MQHMSG, MQIMPO* options, MQCHARV* name, MQPD* propDesc, MQLONG* type, MQLONG valueLength, MQBYTE* value, MQLONG* dataLength, MQLONG* cc, MQLONG* rc);

	void inqmp(MQHCONN conn, MQHMSG, MQIMPO* options, MQCHARV* name, MQPD* propDesc, MQLONG* type, MQLONG valueLength, MQBYTE* value, MQLONG* dataLength);
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

	CbFn _cbFn;

	CtlFn _ctlFn;

	CrtMhFn _crtMhFn;

	DltMhFn _dltMhFn;

	SetMpFn _setMpFn;

	DltMpFn _dltMpFn;

	InqMpFn _inqMpFn;

	void trace(const std::string& subject, const std::string& function, MQLONG* cc, MQLONG* rc);
};

inline bool MQFunctions::isOk() const
{
	return _dll.isLoaded();
}

} // namespace MQ

#endif // _MQ_MQFunctions_H
