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

#ifndef _MQWeb_QueueController_h
#define _MQWeb_QueueController_h

#include <MQ/Web/Controller.h>

namespace MQ {
namespace Web {

class QueueController : public Controller
  /// Controller that shows the details of a queue
{
public:
  QueueController(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
    /// Constructor


  virtual ~QueueController();
    /// Destructor

  void handle();
    /// Sends a MQCMD_INQUIRE_Q command to get the details of the queue

private:
};


} } // Namespace MQ::Web

#endif // _MQWeb_QueueController_h
