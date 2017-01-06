#ifndef IMAGE_SEARCH__
#define IMAGE_SEARCH__

#include "util.h"

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#ifdef __WIN32__
//#include <winsock2.h>
#define SD_RECEIVE 0
#endif
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "ws2_32.lib")

// expose a variable "channel" in SDL_net
#define SOCKET  int
struct _TCPsocket {
	int ready;
	SOCKET channel;
	IPaddress remoteAddress;
	IPaddress localAddress;
	int sflag;
};

#define START_FROM 0
#define RESTART_SEARCH 1
#define RESTART_GETIMAGE 2
#define THREAD_SUCCESS 3
#define THREAD_END 4
#define THREAD_SHUTDOWN 5
#define BUF_LEN 2048

#define OUTPUT_NETWORK_LOG false

// Google
#define TABLE_PREFIX "<div data-async-context="
#define URL_PREFIX "\"ou\":\""
#define URL_SURFIX "\""
#define USER_AGENT "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36"

struct ThreadManager {
	int selected, timeout, which, halt, threadID;
	TCPsocket tcpsock;
	char targetURL[1000];
	char query[300];
	bool running, finish, failure;
};
extern ThreadManager tm;

#define NS_IPADDRESS_FAILURE 1
#define NS_CONNECT_FAILURE 2
#define NS_RCV_ZERO_LENGTH 3
#define NS_IMAGE_SEARCH 4
#define NS_GET_IMAGE 5
#define NS_RCV_JPEG 6
#define NS_RCV_PNG 7
#define NS_RCV_GIF 8
struct NetworkStatus {
	int status, contentLength, receiveLength, receiveCounter, display;
	bool timeoutIcon;
};
extern NetworkStatus ns;

struct ImageFormatReader {
	bool jpgStart, pngStart, gifStart;
	bool jpgEnd, pngEnd, gifEnd;
	int jpgPointer, jpgBytes;
	bool jpgImageDataStart;
	int pngPointer1, pngPointer2;
	int gifPointer, gifBytes, gifField;
	void reset();
	void checkJPG(int id, char c);
	void checkPNG(int id, char c);
	void checkGIF(int id, char c);
};
extern ImageFormatReader ifr;
#define GIF_HEADER 1
#define GIF_GLOBAL_COLOR_TABLES 2
#define GIF_GLAPHIC_CONTROL_EXTENSION_BLOCK 3
#define GIF_COMMENT_EXTENSION_BLOCK 4
#define GIF_PLAIN_TEXT_EXTENSION_BLOCK 5
#define GIF_APPLICATION_EXTENSION_BLOCK 6
#define GIF_IMAGE_DATA_HEADER 7
#define GIF_IMAGE_DATA_BLOCK 8


extern SDL_Thread *thread;
int ImageSearchThread(void *ptr);
int AnotherThread(void *ptr);
void TCPshutdown(int id);
void parseHTML(int id, int n, const char *table_prefix, const char *url_prefix, const char *url_surfix);
void networkLog_noparam(int id, const char *log);
void networkLog(int id, const char *log, ...);

#endif
