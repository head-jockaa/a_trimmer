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

#define BUF_LEN 2048
#define OUTPUT_NETWORK_LOG false

// Google
#define TABLE_PREFIX "[\"GRID_STATE0"
#define USER_AGENT "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.5005.61 Safari/537.36"

struct ThreadManager {
	bool halt;
	int selected, which, threadID;
	TCPsocket tcpsock;
	char targetURL[BUF_LEN];
	char query[BUF_LEN], host[BUF_LEN], path[BUF_LEN];
	bool running, finish, failure, hasCacheImage;
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
	int status, contentLength, receiveLength, receiveCounter, display, chunksize;
	bool chunked, found200;
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

#define HTTPGET_HEADER 0
#define HTTPGET_HEADER_R 1
#define HTTPGET_HEADER_N 2
#define HTTPGET_HEADER_RN 3
#define HTTPGET_HEADER_RNR 4
#define HTTPGET_CHUNK_SIZE 5
#define HTTPGET_CHUNK_R 6
#define HTTPGET_END_CHUNK 7
#define HTTPGET_END_CHUNK_R 8
#define HTTPGET_DATA 9
#define HTTPGET_RESTART 10

extern SDL_Thread *thread;
int ImageSearchThread(void *ptr);
void TCPshutdown(int id);
void parseHTML(int id, int n, const char *table_prefix);
void networkLog_noparam(int id, const char *log);
void networkLog(int id, const char *log, ...);

#endif
