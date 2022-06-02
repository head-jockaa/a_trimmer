#include "image_search.h"

// to solve an error of no OpenSSL_AppLink
#ifdef __WIN32__
#include <openssl/applink.c>
#endif

#ifdef __WIN32__
// TODO
#else
#include <fcntl.h>
#endif

ThreadManager tm;
NetworkStatus ns;
ImageFormatReader ifr;
char endHTML1[7]={'<','/','H','T','M','L','>'};
char endHTML2[7]={'<','/','h','t','m','l','>'};

bool getGoogleImageSearch(int id, char *query);
bool getTargetImage(int id, char *url);
bool receivingImageFile(int id, char *url, SSL *ssl);
bool readHttpHeader(int id, char *headerLine, char *url);
SDL_Texture *createSprite(SDL_Surface *surface);
SDL_Thread *thread;
char thread_str[10000], *thread_fstr;
size_t thread_fsize;
bool thread_loadFile(const char* fn);

/** SDLNet_TCP_Close causes trouble */
void TCPshutdown(int id){
	networkLog(id, "TCPshutdown()");
	if(tm.tcpsock){
		#ifdef __WIN32__
			shutdown(tm.tcpsock->channel,SD_RECEIVE);
			closesocket(tm.tcpsock->channel);
		#else
			SDLNet_TCP_Close(tm.tcpsock);
		#endif
		networkLog(id, "TCP shuted down");
	}else{
		networkLog(id, "tcpsock is null");
	}
	tm.tcpsock=NULL;
}


int ImageSearchThread(void *ptr)
{
	tm.threadID++;
	int id=tm.threadID;
	networkLog(id, "ImageSearchThread()");
	tm.running = true;
	tm.halt = false;
	tm.tcpsock=NULL;
	tm.failure = !getGoogleImageSearch(id, tm.query);
	if(!tm.failure) {
		parseHTML(id, tm.which, TABLE_PREFIX);
		bool finish=false;
		for(int i=0 ; i<10 ; i++){
			if(!tm.halt)finish=getTargetImage(id, tm.targetURL);
			if(finish)break;
			TCPshutdown(id);
		}
	}

	if(tm.halt){
		networkLog(id, "ImageSearchThread is halted");
	}
	else if(!tm.failure){
		networkLog(id, "ImageSearchThread ended successfully");
	}

	tm.running = false;
	return 0;
}

bool imageSearch_https(int id, const char *host, int port, const char *request){
	networkLog(id, "imageSearch_https()");
	ns.status=NS_IMAGE_SEARCH;
	int ret;
	SSL *ssl;
	SSL_CTX *ctx;

	SSL_load_error_strings();
	SSL_library_init();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (!ctx){
		ERR_print_errors_fp(stderr);
		return false;
	}

	ssl = SSL_new(ctx);
	if (!ssl){
		ERR_print_errors_fp(stderr);
		return false;
	}

	//getting IP address
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, host, port);
	if(res){
		ns.status=NS_IPADDRESS_FAILURE;
		ns.display=300;
		return false;
	}

	networkLog(id, "resolved host google.co.jp");

	//to avoid this error "SSL23_GET_SERVER_HELLO:tlsv1 alert internal error"
	SSL_set_tlsext_host_name(ssl, host);

	//start comm.
	if(tm.tcpsock){
		networkLog(id, "Wait: another thread is opening socket");
	}
	while(tm.tcpsock){}
	tm.tcpsock = SDLNet_TCP_Open(&ipaddress);
	if(!tm.tcpsock) {
		ns.status=NS_CONNECT_FAILURE;
		ns.display=300;
		return false;
	}

	networkLog(id, "google.co.jp tcp opened");

	ret = SSL_set_fd(ssl, tm.tcpsock->channel);
	if (ret == 0){
		ERR_print_errors_fp(stderr);
		return false;
	}

	RAND_poll();
	while (RAND_status() == 0){
		unsigned short rand_ret = rand() % 65536;
		RAND_seed(&rand_ret, sizeof(rand_ret));
	}

	ret = SSL_connect(ssl);
	if (ret != 1){
		ERR_print_errors_fp(stderr);
		return false;
	}

	ret = SSL_write(ssl, request, (int)strlen(request));
	if (ret < 1){
		ERR_print_errors_fp(stderr);
		return false;
	}

	char fn[100];

	//receive
	FILE *file=NULL;
	char get_msg[BUF_LEN], headerLine[BUF_LEN];
	int length = 0, headerPos = 0, readmode = HTTPGET_HEADER;
	ns.receiveLength=0;
	ns.contentLength=0;
	ns.receiveCounter=0;
	ns.chunksize=0;

	for(int i=0 ; i<2000 ; i++){
		//forced termination
		if(tm.halt){
			break;
		}
		length = SSL_read(ssl, get_msg, sizeof(get_msg));

		if(!ns.contentLength && !length){
			ns.status=NS_RCV_ZERO_LENGTH;
			ns.display=300;
			break;
		}
		
		if(length){
			ns.receiveCounter++;
		}

		for( int j=0 ; j<length ; j++ ){
			if(readmode==HTTPGET_HEADER){
				if(get_msg[j]==13)readmode=HTTPGET_HEADER_R;
				else if(get_msg[j]==10)readmode=HTTPGET_HEADER_N;
				if(get_msg[j]==13 || get_msg[j]==10){
					for(int k=headerPos; k<BUF_LEN ; k++){
						headerLine[k]=0;
					}
					readHttpHeader(id,headerLine,tm.targetURL);
					headerPos=0;
				}else{
					headerLine[headerPos]=get_msg[j];
					headerPos++;
				}
			}
			else if(readmode==HTTPGET_HEADER_R){
				if(get_msg[j]==13){
					if(ns.chunked){
						readmode=HTTPGET_CHUNK_SIZE;
					}
					else readmode=HTTPGET_DATA;
				}
				else if(get_msg[j]==10)readmode=HTTPGET_HEADER_RN;
				else{
					readmode=HTTPGET_HEADER;
					headerLine[0]=get_msg[j];
					headerPos=1;
				}
			}
			else if(readmode==HTTPGET_HEADER_N){
				if(get_msg[j]==10){
					if(ns.chunked)readmode=HTTPGET_CHUNK_SIZE;
					else readmode=HTTPGET_DATA;
				}else{
					readmode=HTTPGET_HEADER;
					headerLine[0]=get_msg[j];
					headerPos=1;
				}
			}
			else if(readmode==HTTPGET_HEADER_RN){
				if(get_msg[j]==13)readmode=HTTPGET_HEADER_RNR;
				else{
					readmode=HTTPGET_HEADER;
					headerLine[0]=get_msg[j];
					headerPos=1;
				}
			}
			else if(readmode==HTTPGET_HEADER_RNR){
				if(get_msg[j]==10){
					if(ns.chunked)readmode=HTTPGET_CHUNK_SIZE;
					else readmode=HTTPGET_DATA;
				}
				else readmode=HTTPGET_HEADER;
			}
			else if(readmode==HTTPGET_CHUNK_SIZE){
				if(get_msg[j]==13 || get_msg[j]==10){
					networkLog(id, "Receiving chunk %d bytes", ns.chunksize);
					if(ns.chunksize==0){
						readmode=HTTPGET_DATA;
						break;
					}
				}
				if(get_msg[j]==13){
					readmode=HTTPGET_CHUNK_R;
				}
				else if(get_msg[j]==10){
					readmode=HTTPGET_DATA;
				}else{
					ns.chunksize *= 16;
					if(get_msg[j]>='a' && get_msg[j]<='f'){
						ns.chunksize += get_msg[j]-87;
					}
					else if(get_msg[j]>='A' && get_msg[j]<='F'){
						ns.chunksize += get_msg[j]-55;
					}else{
						ns.chunksize += get_msg[j]-48;
					}
				}
			}
			else if(readmode==HTTPGET_CHUNK_R){
				if(get_msg[j]==10){
					readmode=HTTPGET_DATA;
				}
			}
			else if(readmode==HTTPGET_END_CHUNK){
				ns.chunksize=0;
				ns.receiveLength=0;
				if(get_msg[j]==13)readmode=HTTPGET_END_CHUNK_R;
				else if(get_msg[j]==10)readmode=HTTPGET_CHUNK_SIZE;
			}
			else if(readmode==HTTPGET_END_CHUNK_R){
				readmode=HTTPGET_CHUNK_SIZE;
				if(get_msg[j]!=10){
					if(get_msg[j]>='a' && get_msg[j]<='f'){
						ns.chunksize = get_msg[j]-87;
					}
					else if(get_msg[j]>='A' && get_msg[j]<='F'){
						ns.chunksize = get_msg[j]-55;
					}else{
						ns.chunksize = get_msg[j]-48;
					}
				}
			}
			else if(readmode==HTTPGET_DATA){
				//start to save a html file
				if(file==NULL){
					sprintf_s(fn,"save/tmp_google_search_%d_%d_%d.html",animebook[tm.selected].category,animebook[tm.selected].year,animebook[tm.selected].serial);
					fopen_s(&file, fn, "w");
					networkLog(id, fn);
					if(!file){
						networkLog(id, "failed to create new html file");
						tm.halt = true;
						break;
					}
				}

				//writing new image file
				fputc(get_msg[j], file);
				ns.receiveLength++;

				if(ns.chunked && ns.receiveLength==ns.chunksize){
					readmode=HTTPGET_END_CHUNK;
				}
			}
		}

		if((ns.contentLength && ns.receiveLength>=ns.contentLength) || (readmode==HTTPGET_DATA && ns.chunksize==0)){
			break;
		}
	}

	if(file){
		fclose(file);
		file = NULL;
	}

	ret = SSL_shutdown(ssl);
	if (ret != 1){
		ERR_print_errors_fp(stderr);
	}

	TCPshutdown(id);

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	ERR_free_strings();
	return true;
}

char* getHostName(int id, const char *url, char *host){
	int size = BUF_LEN;
	for(int i=0 ; i<size ; i++){
		host[i] = 0;
	}
	int startFrom = 0;
	for(int i=0 ; i<size ; i++){
		if(!startFrom){
			if(url[i]=='/' && url[i+1]=='/'){
				startFrom = i+2;
				i++;
				continue;
			}
		}else{
			if(url[i] == '/'){
				host[i-startFrom] = 0;
				break;
			}
			host[i-startFrom] = url[i];
		}
	}
	return host;
}

char* getPath(int id, const char *url, char *path){
	int size = BUF_LEN;
	for(int i=0 ; i<size ; i++){
		path[i] = 0;
	}
	int slashNum = 0;
	int startFrom = 0;
	for(int i=0 ; i<size ; i++){
		if(!startFrom){
			if(url[i] == '/'){
				slashNum++;
			}
			if(slashNum == 3){
				startFrom = i;
				i--;
				continue;
			}
		}else{
			path[i-startFrom] = url[i];
		}
	}
	return path;
}

/** fetch the nth URL from an HTML of image search result */
void parseHTML(int id, int n, const char *table_prefix){
	networkLog(id, "parseHTML()");
	char fn[100];
	sprintf_s(fn, "save/tmp_google_search_%d_%d_%d.html",animebook[tm.selected].category,animebook[tm.selected].year,animebook[tm.selected].serial);
	thread_loadFile(fn);
	if(!thread_fsize){
		return;
	}
	char *result = NULL, *url = NULL;

	//fetch the nth URL string
	result = strstr(thread_fstr, table_prefix);
	size_t size = 0, start = 0, end = 0;
	if(result){
		int pointer = 0, which_number = 1;
		while(pointer < strlen(result)-3){
			if(
			   (result[pointer] == '.' && result[pointer+1] == 'j' && result[pointer+2] == 'p' && result[pointer+3] == 'g') ||
			   (result[pointer] == '.' && result[pointer+1] == 'j' && result[pointer+2] == 'p' && result[pointer+3] == 'e') ||
			   (result[pointer] == '.' && result[pointer+1] == 'p' && result[pointer+2] == 'n' && result[pointer+3] == 'g') ||
			   (result[pointer] == '.' && result[pointer+1] == 'g' && result[pointer+2] == 'i' && result[pointer+3] == 'f')
			   ){
				if(which_number == n){
					for(int i=pointer ; i>=0 ; i--){
						if(result[i] == '"'){
							start = i+1;
							break;
						}
					}
					for(int i=pointer ; i<strlen(result) ; i++){
						if(result[i] == '"'){
							end = i-1;
							break;
						}
					}
					size = end-start-1;
					break;
				}
				which_number++;
			}
			pointer++;
		}
	}else{
		tm.targetURL[0] = 0;
		return;
	}

	//put into the variable "url"
	url = new char[size+1];
	n=0;
	for(size_t i=start ; i<=end ; i++){
		if(i<=end-3 && result[i]=='%' && result[i+1]=='2' && result[i+2]=='5'){
			url[n] = '%';
			i+=2;
		}
		else if(i<=end-6 && result[i]=='\\' && result[i+1]=='u' && result[i+2]=='0' && result[i+3]=='0' && result[i+4]=='2' && result[i+5]=='6'){
			url[n] = '&';
			i+=5;
		}
		else if(i<=end-6 && result[i]=='\\' && result[i+1]=='u' && result[i+2]=='0' && result[i+3]=='0' && result[i+4]=='3' && result[i+5]=='d'){
			url[n] = '=';
			i+=5;
		}else{
	        url[n] = result[i];
		}
		n++;
	}
	size = n;
	url[n] = 0;
	n=0;
	bool queryFound=false, queryFoundDouble=false;
	for(size_t i=0 ; i<size ; i++){
		if(i<size-2 && url[i]=='%' && url[i+1]=='3' && url[i+2]=='F'){
			if(queryFound){
				// there's only one query symbol '?' in a URL path
				queryFoundDouble=true;
			}else{
				queryFound=true;
			}
		}
		if(!queryFoundDouble && i<size-2 && url[i]=='%' && (url[i+1]=='2' || url[i+1]=='3')){
			for(int j=0 ; j<16 ; j++){
				if(url[i+1] == hex[j]){
					url[n] = j*16;
				}
			}
			for(int j=0 ; j<16 ; j++){
				if(url[i+2] == hex[j]){
					url[n] += j;
				}
			}
			i+=2;
		}else{
	        url[n] = url[i];
		}
		n++;
	}
	url[n] = 0;

	strcpy_s(tm.targetURL, 1000, url);
	FILE *hFile;
	sprintf_s(fn,"save/tmp_url/%d_%d_%d.txt",animebook[tm.selected].category,animebook[tm.selected].year,animebook[tm.selected].serial);
	fopen_s(&hFile, fn, "wb");
	networkLog(id, fn);
	fwrite(tm.targetURL, sizeof(tm.targetURL[0]), n/sizeof(tm.targetURL[0]), hFile);
	fclose(hFile);
}

/** get an image file from the URL */
bool getTargetImage_http(int id, char *url){
	networkLog(id, "getTargetImage_http()");
	if(url[0] == 0){
		return false;
	}
	ns.status=NS_GET_IMAGE;

	getHostName(id, url, tm.host);
	getPath(id, url, tm.path);

	networkLog(id, "resolve host %s",tm.host);

	//getting IP address
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, tm.host, 80);

	if(res){
		//go to the next image file if failed
		tm.which++;
		ns.status=NS_IPADDRESS_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX);
		networkLog(id, "resolving host failed: %s", tm.host);
		char *result = strstr(tm.host, "%");
		if(result){
			networkLog(id, "punycode is not supported");
		}
		return false;
	}

	networkLog(id, "resolved");

	//start comm.
	if(tm.tcpsock){
		networkLog(id, "Wait: another thread is opening socket");
	}
	while(tm.tcpsock){}
	tm.tcpsock = SDLNet_TCP_Open(&ipaddress);

	if(!tm.tcpsock) {
		//go to the next image file if failed
		tm.which++;
		ns.status=NS_CONNECT_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX);
		networkLog(id, "opening tcp failed");
		return false;
	}

	networkLog(id, "connected");

	//send request
	long msg_size = strlen(tm.path)+strlen(tm.host)+strlen(USER_AGENT)+50;
	char *msg = new char[msg_size];
	sprintf_s(msg, msg_size, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n", tm.path, tm.host, USER_AGENT);

	networkLog(id, "Request header is below");
	networkLog_noparam(id, msg);

	// Don't mistake the length of data because you'll fail to send to some servers
	SDLNet_TCP_Send(tm.tcpsock, msg, (int)strlen(msg));

	networkLog(id, SDLNet_GetError());
	delete[] msg;

	if(!receivingImageFile(id, url, NULL))return false;

	TCPshutdown(id);

	ns.status=NULL;
	return true;
}

bool getTargetImage_https(int id, char *url){
	networkLog(id, "getTargetImage_https()");
	if(url[0] == 0){
		return false;
	}

	int ret;
	SSL *ssl;
	SSL_CTX *ctx;

	SSL_load_error_strings();
	SSL_library_init();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (!ctx){
		ERR_print_errors_fp(stderr);
		return false;
	}

	ssl = SSL_new(ctx);
	if (!ssl){
		ERR_print_errors_fp(stderr);
		return false;
	}

	ns.status=NS_GET_IMAGE;

	getHostName(id, url, tm.host);
	getPath(id, url, tm.path);
	networkLog(id, "resolve host %s", tm.host);

	//getting IP address
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, tm.host, 443);

	if(res){
		//go to the next image file if failed
		tm.which++;
		ns.status=NS_IPADDRESS_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX);
		networkLog(id, "resolving host failed: %s", tm.host);
		char *result = strstr(tm.host, "%");
		if(result){
			networkLog(id, "punycode is not supported");
		}
		return false;
	}

	networkLog(id, "resolved");

	//to avoid this error "SSL23_GET_SERVER_HELLO:tlsv1 alert internal error"
	SSL_set_tlsext_host_name(ssl, tm.host);

	//start comm.
	if(tm.tcpsock){
		networkLog(id, "Wait: another thread is opening socket");
	}
	while(tm.tcpsock){}
	tm.tcpsock = SDLNet_TCP_Open(&ipaddress);
	if(!tm.tcpsock) {
		//go to the next image file if failed
		tm.which++;
		ns.status=NS_CONNECT_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX);
		networkLog(id, "opening tcp failed");
		return false;
	}

	networkLog(id, "connected");

	ret = SSL_set_fd(ssl, tm.tcpsock->channel);
	if (ret == 0){
		networkLog(id, "SSL_set_fd() failed");
		ERR_print_errors_fp(stderr);
		return false;
	}

	RAND_poll();
	while (RAND_status() == 0){
		unsigned short rand_ret = rand() % 65536;
		RAND_seed(&rand_ret, sizeof(rand_ret));
	}

	ret = SSL_connect(ssl);
	if (ret != 1){
		networkLog(id, "SSL error occurred");
		ERR_print_errors_fp(stderr);
		tm.which++;
		parseHTML(id, tm.which, TABLE_PREFIX);
		return false;
	}

	//send request
	long msg_size = strlen(tm.path)+strlen(tm.host)+strlen(USER_AGENT)+50;
	char *msg = new char[msg_size];
	sprintf_s(msg, msg_size, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n", tm.path, tm.host, USER_AGENT);

	networkLog(id, "Request header is below");
	networkLog_noparam(id, msg);

	ret = SSL_write(ssl, msg, (int)strlen(msg));
	if (ret < 1){
		ERR_print_errors_fp(stderr);
		return false;
	}
	delete[] msg;

	if(!receivingImageFile(id, url, ssl))return false;

	ret = SSL_shutdown(ssl);
	if (ret != 1){
		ERR_print_errors_fp(stderr);
	}

	TCPshutdown(id);

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	ERR_free_strings();
	ns.status=NULL;
	return true;
}

bool getTargetImage(int id, char *url){
	/* TODO
	char *checkurl = strstr(url, "amazon.com");
	if(checkurl){
		networkLog(id, "skip %s", "amazon.com");
		tm.which++;
		parseHTML(id, tm.which, TABLE_PREFIX);
		return false;
	}
	else
	 */
	if(strstr(url, "https://")){
		return getTargetImage_https(id, url);
	}else{
		return getTargetImage_http(id, url);
	}
}

void toLowerCase(char *headerLine){
	for(int i=0 ; i<BUF_LEN ; i++){
		if(headerLine[i]==0)break;
		if(headerLine[i]>='A' && headerLine[i]<='Z'){
			headerLine[i]+=32;
		}
	}
}

bool checkHeaderName(char *headerLine, const char *name){
	int namePos=0;
	for(int i=0 ; i<BUF_LEN ; i++){
		if(headerLine[i]==0)break;
		if(headerLine[i]==':')break;
		if(headerLine[i]==' ')continue;
		if(headerLine[i]==name[namePos]){
			namePos++;
		}else{
			break;
		}
	}
	return name[namePos]==0;
}

bool checkHeaderValue(char *headerLine, const char *name){
	int namePos=0;
	bool pasColon=false;
	for(int i=0 ; i<BUF_LEN ; i++){
		if(headerLine[i]==0)break;
		if(headerLine[i]==':')pasColon=true;
		else if(pasColon){
			if(headerLine[i]==0)break;
			if(headerLine[i]==' ')continue;
			if(headerLine[i]==name[namePos]){
				namePos++;
			}else{
				break;
			}
		}
	}
	return name[namePos]==0;
}

/* returns true when retrying */
bool readHttpHeader(int id, char *headerLine, char*url){
	networkLog_noparam(id, headerLine);

	toLowerCase(headerLine);

	//getting file size info
	bool result = checkHeaderName(headerLine, "content-length");
	if(!result){
		result = checkHeaderName(headerLine, "x-oct-size");
	}
	if(result){
		ns.contentLength=0;
		bool passColon=false;
		for(int i=0 ; i<BUF_LEN ; i++){
			if(passColon){
				if(headerLine[i]==0){
					networkLog(id, "content length has been found above: %d", ns.contentLength);
					break;
				}
				else if(headerLine[i]>='0' && headerLine[i]<='9'){
					ns.contentLength *= 10;
					ns.contentLength += headerLine[i]-48;
				}
			}
			else if(headerLine[i]==':'){
				passColon=true;
			}
		}
	}

	// ckeck if chunked or not
	result = checkHeaderName(headerLine, "transfer-encoding");
	if(result){
		result = checkHeaderValue(headerLine, "chunked");
	}
	if(result){
		networkLog(id, "the mode chunked has been found above");
		ns.chunked = true;
	}

	//to redirect
	result = checkHeaderName(headerLine, "location");
	if(result){
		networkLog(id, "redirect to another url");
		bool passColon=false;
		char *url2 = new char[BUF_LEN];
		int urlPos = 0;
		for(int i=0 ; i<BUF_LEN ; i++){
			if(passColon){
				if(headerLine[i]!=' '){
					url2[urlPos]=headerLine[i];
					urlPos++;
				}
				if(headerLine[i]==0)break;
			}
			else if(headerLine[i]==':'){
				passColon=true;
			}
		}

		if(strcmp(url,url2)==0){
			// avoid an infinite loop (rarely happens though)
			tm.which++;
			parseHTML(id, tm.which, TABLE_PREFIX);
		}else{
			strcpy_s(url, BUF_LEN, url2);
		}
		delete[] url2;
		return true;
	}

	//Content-Encoding gzip is not supported
	result = checkHeaderName(headerLine, "content-encoding");
	if(result){
		result = checkHeaderValue(headerLine, "gzip");
		if(result){
			tm.which++;
			networkLog(id, "Content-Encoding gzip is not supported");
			parseHTML(id, tm.which, TABLE_PREFIX);
			return true;
		}
	}

	//if not 200, it would mean that the url is wrong
	char *result2 = strstr(headerLine, "200 ok");
	if(result2){
		ns.found200=true;
	}
	return false;
}

bool receivingImageFile(int id, char *url, SSL *ssl){
	// to avoid the blocking of recv()
#ifdef __WIN32__
	unsigned long value = 1;
	ioctlsocket(tm.tcpsock->channel, FIONBIO, &value);
#else
	int flags;
	flags = fcntl(tm.tcpsock->channel, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(tm.tcpsock->channel, F_SETFL, flags);
#endif

	networkLog(id, "receivingImageFile()");
	FILE *file=NULL;
	char get_msg[BUF_LEN];
	int length = 0, headerPos = 0, readmode = HTTPGET_HEADER;
	char fn[100], headerLine[BUF_LEN];

	for(int i=0 ; i<BUF_LEN ; i++){
		get_msg[i]=0;
		headerLine[i]=0;
	}

	ns.found200=false;
	ns.chunked=false;
	ns.chunksize=0;
	ns.receiveCounter=0;
	ns.receiveLength=0;
	ifr.reset();

	bool failure = false;

	//receive
	for(int i=0 ; i<=10000 ; i++){
		if(i==10000){
			tm.which++;
			networkLog(id, "recv() won't be ready forever (pass it)");
			parseHTML(id, tm.which, TABLE_PREFIX);
			failure = true;
			break;
		}

		//forced termination
		if(tm.halt){
			networkLog(id, "halted");
			break;
		}
		get_msg[0]=0;
		if(ssl){
			length = SSL_read(ssl, get_msg, BUF_LEN);
		}else{
			length = SDLNet_TCP_Recv(tm.tcpsock, get_msg, BUF_LEN);
		}
		if(!ns.contentLength && !length){
			ns.status=NS_RCV_ZERO_LENGTH;
			ns.display=300;
			networkLog(id, "received zero length");
			break;
		}
		if(length==-1){
			// message is not ready
			SDL_Delay(1);
			continue;
		}else{
			i=0;
		}
		ns.receiveCounter++;

		for( int j=0 ; j<length ; j++ ){
			if(readmode==HTTPGET_HEADER){
				if(get_msg[j]==13)readmode=HTTPGET_HEADER_R;
				else if(get_msg[j]==10)readmode=HTTPGET_HEADER_N;
				if(get_msg[j]==13 || get_msg[j]==10){
					for(int k=headerPos; k<BUF_LEN ; k++){
						headerLine[k]=0;
					}
					if(readHttpHeader(id,headerLine,url)){
						// redirect
						failure = true;
						break;
					}
					headerPos=0;
				}else{
					headerLine[headerPos]=get_msg[j];
					headerPos++;
				}
			}
			else if(readmode==HTTPGET_HEADER_R){
				if(get_msg[j]==13){
					if(ns.chunked)readmode=HTTPGET_CHUNK_SIZE;
					else readmode=HTTPGET_DATA;
				}
				else if(get_msg[j]==10)readmode=HTTPGET_HEADER_RN;
				else{
					readmode=HTTPGET_HEADER;
					headerLine[0]=get_msg[j];
					headerPos=1;
				}
			}
			else if(readmode==HTTPGET_HEADER_N){
				if(get_msg[j]==10){
					if(ns.chunked)readmode=HTTPGET_CHUNK_SIZE;
					else readmode=HTTPGET_DATA;
				}else{
					readmode=HTTPGET_HEADER;
					headerLine[0]=get_msg[j];
					headerPos=1;
				}
			}
			else if(readmode==HTTPGET_HEADER_RN){
				if(get_msg[j]==13)readmode=HTTPGET_HEADER_RNR;
				else{
					readmode=HTTPGET_HEADER;
					headerLine[0]=get_msg[j];
					headerPos=1;
				}
			}
			else if(readmode==HTTPGET_HEADER_RNR){
				if(get_msg[j]==10){
					if(ns.chunked)readmode=HTTPGET_CHUNK_SIZE;
					else readmode=HTTPGET_DATA;
				}
				else readmode=HTTPGET_HEADER;
			}
			else if(readmode==HTTPGET_CHUNK_SIZE){
				if(get_msg[j]==13 || get_msg[j]==10){
					networkLog(id, "Receiving chunk %d bytes", ns.chunksize);
					if(ns.chunksize==0){
						tm.finish=true;
						break;
					}
				}
				if(get_msg[j]==13){
					readmode=HTTPGET_CHUNK_R;
				}
				else if(get_msg[j]==10){
					readmode=HTTPGET_DATA;
				}else{
					ns.chunksize *= 16;
					if(get_msg[j]>='a' && get_msg[j]<='f'){
						ns.chunksize += get_msg[j]-87;
					}
					else if(get_msg[j]>='A' && get_msg[j]<='F'){
						ns.chunksize += get_msg[j]-55;
					}else{
						ns.chunksize += get_msg[j]-48;
					}
				}
			}
			else if(readmode==HTTPGET_CHUNK_R){
				if(get_msg[j]==10){
					readmode=HTTPGET_DATA;
				}
			}
			else if(readmode==HTTPGET_END_CHUNK){
				ns.chunksize=0;
				ns.receiveLength=0;
				if(get_msg[j]==13)readmode=HTTPGET_END_CHUNK_R;
				else if(get_msg[j]==10)readmode=HTTPGET_CHUNK_SIZE;
			}
			else if(readmode==HTTPGET_END_CHUNK_R){
				readmode=HTTPGET_CHUNK_SIZE;
				if(get_msg[j]!=10){
					if(get_msg[j]>='a' && get_msg[j]<='f'){
						ns.chunksize = get_msg[j]-87;
					}
					else if(get_msg[j]>='A' && get_msg[j]<='F'){
						ns.chunksize = get_msg[j]-55;
					}else{
						ns.chunksize = get_msg[j]-48;
					}
				}
			}
			else if(readmode==HTTPGET_DATA){
				if(!ns.found200){
					tm.which++;
					networkLog(id, "get an error code (pass it)");
					parseHTML(id, tm.which, TABLE_PREFIX);
					failure = true;
					break;
				}

				//start to save an image file
				if(file==NULL){
					sprintf_s(fn, "save/tmp_image/%d_%d_%d.jpg",animebook[tm.selected].category,animebook[tm.selected].year,animebook[tm.selected].serial);
					fopen_s(&file, fn, "wb");
					networkLog(id, fn);
					if(!file){
						networkLog(id, "failed to create new img file");
						tm.halt = true;
						failure = true;
						break;
					}
				}

				ifr.checkPNG(id, get_msg[j]);
				ifr.checkJPG(id, get_msg[j]);
				ifr.checkGIF(id, get_msg[j]);

				//writing new image file
				fputc(get_msg[j], file);
				ns.receiveLength++;

				if(ns.chunked && ns.receiveLength==ns.chunksize){
					readmode=HTTPGET_END_CHUNK;
				}
			}

			//finish writing a file
			if(!ns.chunked && ns.contentLength && ns.receiveLength>=ns.contentLength){
				tm.finish=true;
				break;
			}
			if(!ns.contentLength && (ifr.jpgEnd|| ifr.pngEnd || ifr.gifEnd)){
				tm.finish=true;
				break;
			}
		}

		if(failure || tm.finish){
			break;
		}
	}

	if(file){
		networkLog(id, "finished writing img file");
		fclose(file);
		sprintf_s(thread_str,"save/tmp_google_search_%d_%d_%d.html",animebook[tm.selected].category,animebook[tm.selected].year,animebook[tm.selected].serial);
		remove(thread_str);
	}

	return !failure;
}

bool getGoogleImageSearch(int id, char *query){
	networkLog(id, "getGoogleImageSearch()");
	long request_size = strlen(query)+strlen(USER_AGENT)+100;
    char *request = new char[request_size];
    sprintf_s(request, request_size, "GET /search?q=%s&source=lnms&tbm=isch HTTP/1.1\r\nHost: www.google.co.jp\r\nUser-Agent: %s\r\n\r\n", query, USER_AGENT);
	networkLog(id, "Request header is below");
	networkLog_noparam(id, request);
    bool result = imageSearch_https(id, "www.google.co.jp", 443, request);
    delete[] request;
	return result;
}

void ImageFormatReader::reset(){
    jpgStart = false;pngStart = false;gifStart = false;
    jpgEnd = false;pngEnd = false;gifEnd = false;
    jpgPointer = 0;jpgBytes = 0;
    jpgImageDataStart = false;
    pngPointer1 = 0;pngPointer2 = 0;
    gifPointer = 0;gifBytes = 0;gifField = 0;
}

void ImageFormatReader::checkPNG(int id, char c){
    if(!pngStart){
        if(pngPointer1 == 0){
            if(c == -119){
                pngPointer1++;
            }
        }
        else if(pngPointer1 == 1){
            if(c == 'P'){
                pngPointer1++;
            }else{
                pngPointer1 = 0;
            }
        }
        else if(pngPointer1 == 2){
            if(c == 'N'){
                pngPointer1++;
            }else{
                pngPointer1 = 0;
            }
        }
        else if(pngPointer1 == 3){
            if(c == 'G'){
                pngStart = true;
				networkLog(id, "found PNG data");
				ns.status = NS_RCV_PNG;
            }else{
                pngPointer1 = 0;
            }
        }
        return;
    }
    if(pngPointer2 == 0){
        if(c == 'I'){
            pngPointer2++;
        }
    }
    else if(pngPointer2 == 1){
        if(c == 'E'){
            pngPointer2++;
        }else{
            pngPointer2 = 0;
        }
    }
    else if(pngPointer2 == 2){
        if(c == 'N'){
            pngPointer2++;
        }else{
            pngPointer2 = 0;
        }
    }
    else if(pngPointer2 == 3){
        if(c == 'D'){
            pngPointer2++;
        }else{
            pngPointer2 = 0;
        }
    }
    else if(pngPointer2 == 4){
        if(c == -82){
            pngPointer2++;
        }else{
            pngPointer2 = 0;
        }
    }
    else if(pngPointer2 == 5){
        if(c == 66){
            pngPointer2++;
        }else{
            pngPointer2 = 0;
        }
    }
    else if(pngPointer2 == 6){
        if(c == 96){
            pngPointer2++;
        }else{
            pngPointer2 = 0;
        }
    }
    else if(pngPointer2 == 7){
        if(c == -126){
            pngEnd = true;
			networkLog(id, "PNG END");
        }else{
            pngPointer2 = 0;
        }
    }
}

void ImageFormatReader::checkJPG(int id, char c){
    if(jpgImageDataStart){
        if(c == -1){
            jpgPointer = 0;
        }
        else if(jpgPointer == 1 && c == -39){
            jpgPointer = 0;
            jpgEnd = true;
			networkLog(id, "JPG END");
            return;
        }
    }
    else if(jpgPointer == 0){
        if(c != -1){
            return;
        }
    }
    else if(jpgPointer == 1){
        if(c == -40){
            jpgPointer = 0;
            jpgStart = true;
			networkLog(id, "found JPG data");
			ns.status = NS_RCV_JPEG;
            return;
        }
        else if(c == -38){
            jpgImageDataStart = true;
			networkLog(id, "came into JPG data part");
            return;
        }
    }
    else if(jpgPointer == 2){
        int a = c;
        if(a < 0){
            a += 256;
        }
        jpgBytes = a*256;
    }
    else if(jpgPointer == 3){
        int a = c;
        if(a < 0){
            a += 256;
        }
        jpgBytes += a;
    }
    else if(jpgPointer >= 4){
        if(!jpgImageDataStart && jpgPointer-1 == jpgBytes){
            jpgPointer = 0;
			networkLog(id, "END CHUNK: received %d bytes", jpgBytes);
            return;
        }
    }
    jpgPointer++;
}

void ImageFormatReader::checkGIF(int id, char c){
    if(!gifStart){
        if(gifPointer == 0){
            if(c == 'G'){
                gifPointer++;
            }
        }
        else if(gifPointer == 1){
            if(c == 'I'){
                gifPointer++;
            }else{
                gifPointer = 0;
            }
        }
        else if(gifPointer == 2){
            if(c == 'F'){
                gifPointer++;
            }else{
                gifPointer = 0;
            }
        }
        else if(gifPointer == 3){
            if(c == '8'){
                gifStart = true;
                gifPointer = 4;
                gifField = GIF_HEADER;
				networkLog(id, "found GIF data");
				ns.status = NS_RCV_GIF;
            }else{
                gifPointer = 0;
            }
        }
        return;
    }

    if(gifField == 0){
        if(gifPointer == 0){
            if(c == 44){
                gifPointer = 1;
                gifField = GIF_IMAGE_DATA_HEADER;
				networkLog(id, "came into GIF header part");
                return;
            }
            else if(c == 59){
                gifEnd = true;
				networkLog(id, "GIF END");
            }
        }
        else if(gifPointer == 1){
            if(c == -7){
                gifPointer = 2;
                gifField = GIF_GLAPHIC_CONTROL_EXTENSION_BLOCK;
				networkLog(id, "GLAPHIC CONTROL EXTENSION BLOCK");
                return;
            }
            else if(c == -2){
                gifPointer = 2;
                gifField = GIF_COMMENT_EXTENSION_BLOCK;
                gifBytes = 0;
				networkLog(id, "COMMENT EXTENSION BLOCK");
                return;
            }
            else if(c == 1){
                gifPointer = 2;
                gifField = GIF_PLAIN_TEXT_EXTENSION_BLOCK;
                gifBytes = 0;
				networkLog(id, "PLAIN TEXT EXTENSION BLOCK");
                return;
            }
            else if(c == -1){
                gifPointer = 2;
                gifField = GIF_APPLICATION_EXTENSION_BLOCK;
                gifBytes = 0;
				networkLog(id, "APPLICATION EXTENSION BLOCK");
                return;
            }
        }
    }
    else if(gifField == GIF_HEADER){
        if(gifPointer == 10){
            int a = c & 15;
            gifBytes = 2;
            for(int j=0 ; j<a ; j++){
                gifBytes *= 2;
            }
            gifBytes *= 3;
        }
        else if(gifPointer == 12){
            gifField = GIF_GLOBAL_COLOR_TABLES;
            gifPointer = 0;
			networkLog(id, "GLOBAL COLOR TABLES: %d", gifBytes);
            return;
        }
    }
    else if(gifField == GIF_GLOBAL_COLOR_TABLES){
        if(gifPointer == gifBytes-1){
            gifField = 0;
            gifPointer = 0;
            return;
        }
    }
    else if(gifField == GIF_GLAPHIC_CONTROL_EXTENSION_BLOCK){
        if(gifPointer == 7){
            gifField = 0;
            gifPointer = 0;
            return;
        }
    }
    else if(gifField == GIF_COMMENT_EXTENSION_BLOCK){
        if(gifPointer == 2){
            int a = c;
            if(a < 0){
                a += 256;
            }
            if(a == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "COMMENT: %d", a);
            }
            gifBytes = a+3;
        }
        else if(gifPointer == gifBytes && gifBytes){
            int a = c;
            if(a < 0){
                a += 256;
            }
            if(a == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "COMMENT: %d", a);
            }
            gifBytes += a+1;
        }
    }
    else if(gifField == GIF_PLAIN_TEXT_EXTENSION_BLOCK){
        if(gifPointer == 15){
            int a = c;
            if(a < 0){
                a += 256;
            }
            if(a == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "DATA: %d", a);
			}
            gifBytes = a+16;
        }
        else if(gifPointer == gifBytes && gifBytes){
            int a = c;
            if(a < 0){
                a += 256;
            }
            if(a == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "DATA: %d", a);
            }
            gifBytes += a+1;
        }
    }
    else if(gifField == GIF_APPLICATION_EXTENSION_BLOCK){
        if(gifPointer == 14){
            int a = c;
            if(a < 0){
                a += 256;
            }
            if(a == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "DATA: %d", a);
            }
            gifBytes = a+15;
        }
        else if(gifPointer == gifBytes && gifBytes){
            int a = c;
            if(a < 0){
                a += 256;
            }
            if(a == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "DATA: %d", a);
			}
            gifBytes += a+1;
        }
    }
    else if(gifField == GIF_IMAGE_DATA_HEADER){
        if(gifPointer == 9){
            int a = (c >> 7) & 1;
            if(a == 1){
                gifBytes = c & 7;
                for(int j=0 ; j<a ; j++){
                    gifBytes *= 2;
                }
                gifBytes *= 3;
				networkLog(id, "LOCAL COLOR TABLES: %d", gifBytes);
            }else{
                gifBytes = 0;
				networkLog(id, "LOCAL COLOR TABLES: none");
            }
        }
        else if(gifPointer == 10+gifBytes){
            gifField = GIF_IMAGE_DATA_BLOCK;
            gifPointer = 0;
            return;
        }
    }
    else if(gifField == GIF_IMAGE_DATA_BLOCK){
        if(gifPointer == 0){
            gifBytes = c;
            if(gifBytes < 0){
                gifBytes += 256;
            }
            if(gifBytes == 0){
                gifField = 0;
                gifPointer = 0;
                return;
            }else{
				networkLog(id, "IMAGE DATA BLOCK: %d", gifBytes);
            }
        }
        else if(gifPointer == gifBytes){
            gifPointer = 0;
            return;
        }
    }
    gifPointer++;
}

/* to avoid a confusion with a '%' in URL strings */
void networkLog_noparam(int id, const char *log) {
	if(!OUTPUT_NETWORK_LOG)return;
	for(int i=0 ; i<10000 ; i++){
		if(log[i] == 0) {
			thread_str[i] = 13;
			thread_str[i+1] = 10;
			thread_str[i+2] = 0;
			break;
		}
		thread_str[i] = log[i];
	}

	FILE* hFile;
	char fn[100];
	sprintf_s(fn,"log%d.txt",id);
	fopen_s(&hFile, fn, "ab");
	fwrite(thread_str, sizeof(thread_str[0]), strlen(thread_str)/sizeof(thread_str[0]), hFile);
	fclose(hFile);
}

void networkLog(int id, const char *log, ...) {
	if(!OUTPUT_NETWORK_LOG)return;
	va_list c2;
	va_start(c2, log);
	#ifdef __WIN32__
	vsprintf_s(thread_str,log,c2);
	#else
	vsprintf(thread_str,log,c2);
	#endif
	sprintf_s(thread_str,"%s\n",thread_str);

	FILE* hFile;
	char fn[100];
	sprintf_s(fn,"log%d.txt",id);
	fopen_s(&hFile, fn, "ab");
	fwrite(thread_str, sizeof(thread_str[0]), strlen(thread_str)/sizeof(thread_str[0]), hFile);
	fclose(hFile);
}

bool thread_loadFile(const char* fn){
	fpos_t a=0,b=0;
	FILE* hFile;

	if(thread_fstr)delete [] thread_fstr;
	thread_fsize=0;
	thread_fstr=NULL;
	fopen_s(&hFile,fn,"rb");
	if(hFile==NULL){
		thread_fstr=NULL;
		return false;
	}

	b=fseek(hFile,0,SEEK_END);
	fgetpos(hFile,&a);
	thread_fsize=(size_t)a;
	fseek(hFile,(long)b,SEEK_SET);

	thread_fstr=new char[thread_fsize+1];
	fread(thread_fstr, sizeof(thread_fstr[0]), thread_fsize/sizeof(thread_fstr[0]), hFile);
	fclose(hFile);
	thread_fstr[thread_fsize]=0;
	return true;
}
