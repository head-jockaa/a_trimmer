#ifndef IMAGE_SEARCH__
#define IMAGE_SEARCH__

#include "util.h"

#include <iostream>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#ifdef __WIN32__
#include <winsock2.h>
#endif
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "ws2_32.lib")

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
#define BUF_LEN 2048

// Google
#define TABLE_PREFIX "<div data-async-context="
#define URL_PREFIX "\"ou\":\""
#define URL_SURFIX '"'
// Bing
//#define TABLE_PREFIX "<div class=\"dg_u\""
//#define URL_PREFIX "imgurl:&quot;"
//#define URL_SURFIX '&'

//int OPENSSL_init_ssl(uint64_t opts, const OPENSSL_INIT_SETTINGS *settings);


struct ThreadManager {
	int selected, timeout, idx, halt;
	TCPsocket tcpsock;
	FILE *file;
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

extern SDL_Thread *thread;
int TestThread(void *ptr);
int TestThread2(void *ptr);
void TCPshutdown();
void parseHTML(int n, const char *table_prefix, const char *url_prefix, char url_surfix);

#endif