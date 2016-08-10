#include <libwebsockets.h>

using namespace std;

#define ECO_DRIVE_WS_DATA_SIZE    256
#define WAIT_DELAY      50

#include <stdint.h>
#include <map>
#include <string>
#include <list>
#include <stdio.h>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <sstream>
#include <libwebsockets.h>

#include "WebSocketServer.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/// WebSocketServer
/// ---------

#define MAX_BUFFER_SIZE 0

#define ECO_DRIVE_WS_DATA_SIZE    256
char resource_path[40] = "./src/www";

WebSocketServer *self;

/**-------------------------------------------------------------------------
 @name        get_mimetype
 @param[in]   :filename
 @return      :mimetype
 @details     :
 --------------------------------------------------------------------------*/
const char * get_mimetype(const char *file) {
	int n = strlen(file);

	if (n < 5)
		return NULL;

	if (!strcmp(&file[n - 4], ".ico"))
		return "image/x-icon";

	if (!strcmp(&file[n - 4], ".png"))
		return "image/png";

	if (!strcmp(&file[n - 5], ".html"))
		return "text/html";

	if (!strcmp(&file[n - 4], ".css"))
		return "text/css";

	if (!strcmp(&file[n - 4], ".gif"))
		return "image/gif";

	if (!strcmp(&file[n - 4], ".jpg"))
		return "image/jpeg";

	if (!strcmp(&file[n - 3], ".js"))
		return "text/javascript";

	if (!strcmp(&file[n - 5], ".json"))
		return "application/json";

	if (!strcmp(&file[n - 4], ".log"))
		return "text/plain";

	if (!strcmp(&file[n - 4], ".xml"))
		return "text/xml";

	if (!strcmp(&file[n - 4], ".svg"))
		return "image/svg+xml";

	if (!strcmp(&file[n - 4], ".ttf"))
		return "application/x-font-ttf";

	if (!strcmp(&file[n - 5], ".woff"))
		return "application/font-woff";

	if (n < 11)
		return NULL;

	if (!strcmp(&file[n - 11], ".properties"))
		return "text/plain";


	return NULL;
}

int callback_http(
		struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len) {

	char buf[256];

	int n = 0;

	char *other_headers;

	const char *mimetype;

	switch (reason) {
	case LWS_CALLBACK_HTTP: {

		if (len < 1) {
			lws_return_http_status(wsi,400, NULL);
			goto try_to_reuse;
		}

		if (lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI))
			return 0;

		/* if not, send a file the easy way */
		strcpy(buf, resource_path);
		if (strcmp((const char *) in, "/")) {
			if (*((const char *) in) != '/') {
				strcat(buf, "/");
			}
			strncat(buf, (const char *) in,sizeof(buf) - strlen(resource_path));
		} else /* default file to serve */
		{
			strcat(buf, "/index.html");
		}
		buf[sizeof(buf) - 1] = '\0';

		/* refuse to serve files we don't understand */
		mimetype = get_mimetype(buf);
		if (!mimetype) {

			lws_return_http_status( wsi,415, NULL);
			return -1;
		}


		other_headers = NULL;

		n = lws_serve_http_file(wsi, buf, mimetype,other_headers, n);
		if (n < 0 || ((n > 0) && lws_http_transaction_completed(wsi)))
			return -1; /* error or can't reuse connection: close the socket */

		/*
		 * notice that the sending of the file completes asynchronously,
		 * we'll get a LWS_CALLBACK_HTTP_FILE_COMPLETION callback when
		 * it's done
		 */
	}
		break;

	case LWS_CALLBACK_HTTP_BODY:

		strncpy(buf, (const char *) in, 20);
		buf[20] = '\0';
		if (len < 20)
			buf[len] = '\0';
		lwsl_notice("LWS_CALLBACK_HTTP_BODY: %s... len %d\n",(const char * )buf, (int )len);
		break;

	case LWS_CALLBACK_HTTP_BODY_COMPLETION:

		lwsl_notice("LWS_CALLBACK_HTTP_BODY_COMPLETION\n");
		/* the whole of the sent body arrived, close or reuse the connection */
		lws_return_http_status( wsi, 200, NULL);
		goto try_to_reuse;

	case LWS_CALLBACK_HTTP_FILE_COMPLETION:

		//log_debug("LWS_CALLBACK_HTTP_FILE_COMPLETION seen");
		/* kill the connection after we sent one file */
		goto try_to_reuse;
		break;



	default:

		break;
	}

	return 0;

	try_to_reuse: if (lws_http_transaction_completed(wsi))
		return -1;

	return 0;
}




static int websocket_callback(
		struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len) {
	int fd;

	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		self->onConnectWrapper(lws_get_socket_fd(wsi));

		break;
	case LWS_CALLBACK_RECEIVE:
		self->onMessage(lws_get_socket_fd(wsi),
				std::string((const char *) in));
		lws_callback_on_writable( wsi);
		break;

	case LWS_CALLBACK_CLOSED:
		self->onDisconnectWrapper(lws_get_socket_fd(wsi));
		break;

	case LWS_CALLBACK_SERVER_WRITEABLE:
		fd = lws_get_socket_fd(wsi);
		while (!self->connections[fd]->buffer.empty()) {
			std::string message = self->connections[fd]->buffer.front();
			int charsSent =lws_write(wsi, (unsigned char*) message.c_str(),message.length(), LWS_WRITE_TEXT);

			if (unsigned (charsSent) < message.length())
			 self->onErrorWrapper(fd, std::string("Error writing to socket"));
			else
			// Only pop the message if it was sent successfully.
			self->connections[fd]->buffer.pop_front();
		}
		break;
     	default:

		break;
	}
	return 0;
}

static struct lws_protocols protocols[] = {
		{ "http-only",   // name
		callback_http, // callback
		0             // maximum frame size
		},
		{ "myprotocol",
				websocket_callback,
				0,      // user data struct not used
				16384,
		},
		{ NULL, NULL, 0, 0 } };

// Constructor / Destructor
WebSocketServer::WebSocketServer(int port, const std::string certPath,
		const std::string& keyPath) {
	this->_port = port;
	this->_certPath = certPath;
	this->_keyPath = keyPath;

	struct lws_context_creation_info info;
	memset(&info, 0, sizeof info);
	info.port = port;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;
/*
	// keep alive
	info.ka_time = 60; // 60 seconds until connection is suspicious
	info.ka_probes = 10; // 10 probes after ^ time
	info.ka_interval = 10; // 10s interval for sending probes*/
	struct lws_context *context = lws_create_context(&info);
	if (context == NULL) {
		fprintf(stderr, "libwebsocket init failed\n");

	}
	this->_context = context;
	self = this;

};


WebSocketServer::~WebSocketServer() {
	// Free up some memory
	for (std::map<int, Connection*>::const_iterator it =
			this->connections.begin(); it != this->connections.end(); ++it) {
		Connection* c = it->second;
		this->connections.erase(it->first);
		delete c;
	}
}
;


void WebSocketServer::run(uint64_t timeout) {
	while (1) {
		this->wait(timeout);
			}
};
void WebSocketServer::wait(uint64_t timeout) {

	if (lws_service(this->_context, timeout) < 0)
	{
		throw "Error polling for socket activity.";
		lws_context_destroy(this->_context);
	}
};

void WebSocketServer::send(int socketID, std::string data) {
	this->connections[socketID]->buffer.push_back(data);
};

void WebSocketServer::broadcast(std::string data) {
	for (std::map<int, Connection*>::const_iterator it =
			this->connections.begin();
			it != this->connections.end(); ++it)
		this->send(it->first, data);
};

// Key => value storage for each connection
std::string WebSocketServer::getValue(int socketID, const std::string& name) {
	return this->connections[socketID]->keyValueMap[name];
};
void WebSocketServer::setValue(int socketID, const std::string& name,
		const std::string& value) {
	this->connections[socketID]->keyValueMap[name] = value;
};
int WebSocketServer::getNumberOfConnections() {
	return this->connections.size();;
};

void WebSocketServer::_removeConnection(int socketID) {
	Connection* c = this->connections[socketID];
	this->connections.erase(socketID);
	delete c;
};

// Wrappers, so we can take care of some maintenance
void WebSocketServer::onConnectWrapper(int socketID) {
	Connection* c = new Connection;
	c->createTime = time(0);
	this->connections[socketID] = c;
	this->onConnect(socketID);

};
void WebSocketServer::onDisconnectWrapper(int socketID) {
	this->onDisconnect(socketID);
	this->_removeConnection(socketID);
};
void WebSocketServer::onErrorWrapper(int socketID, const std::string& message) {
	this->onError(socketID, message);
	this->_removeConnection(socketID);
};

