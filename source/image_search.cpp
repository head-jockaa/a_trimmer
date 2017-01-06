#include "image_search.h"

// to solve an error of no OpenSSL_AppLink
#ifdef __WIN32__
#include <openssl/applink.c>
#endif

ThreadManager tm;
NetworkStatus ns;
ImageFormatReader ifr;
char endHTML1[7]={'<','/','H','T','M','L','>'};
char endHTML2[7]={'<','/','h','t','m','l','>'};

void getGoogleImageSearch(int id, char *query);
void getTargetImage(int id, char *url);
void receivingImageFile(int id, char *url, SSL *ssl);
SDL_Texture *createSprite(SDL_Surface *surface);
SDL_Thread *thread;

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
		networkLog(id, "shuted down");
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
    tm.halt = 0;
	tm.tcpsock=NULL;
	getGoogleImageSearch(id, tm.query);
	if(!tm.halt)parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
	if(!tm.halt)getTargetImage(id, tm.targetURL);

    if(tm.halt){
		networkLog(id, "ImageSearchThread is halted");
    }else{
		networkLog(id, "ImageSearchThread ended successfully");
    }

    tm.running = false;
//	tm.threadID--;
	return 0;
}

int AnotherThread(void *ptr)
{
	tm.threadID++;
	int id=tm.threadID;
	networkLog(id, "AnotherThread()");
    tm.running = true;
    tm.halt = 0;
	tm.failure=false;
	tm.tcpsock=NULL;
    getTargetImage(id, tm.targetURL);

    if(tm.halt){
		networkLog(id, "AnotherThread is halted");
    }else{
		networkLog(id, "AnothrtThread ended successfully");
        tm.halt = THREAD_SUCCESS;
    }

    tm.running = false;
//	tm.threadID--;
    return 0;
}


void imageSearch_https(int id, const char *host, int port, const char *request){
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
		return;
	}

	ssl = SSL_new(ctx);
	if (!ssl){
		ERR_print_errors_fp(stderr);
		return;
	}

	//getting IP address
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, host, port);
	if(res){
		ns.status=NS_IPADDRESS_FAILURE;
		ns.display=300;
		return;
	}

	networkLog(id, "resolved host google.co.jp");

	//start comm.
	if(tm.tcpsock){
		networkLog(id, "Wait: another thread is opening socket");
	}
	while(tm.tcpsock){}
	tm.tcpsock = SDLNet_TCP_Open(&ipaddress);
	if(!tm.tcpsock) {
		ns.status=NS_CONNECT_FAILURE;
		ns.display=300;
		return;
	}

	networkLog(id, "google.co.jp tcp opened");

	ret = SSL_set_fd(ssl, tm.tcpsock->channel);
	if (ret == 0){
		ERR_print_errors_fp(stderr);
		return;
	}

	RAND_poll();
	while (RAND_status() == 0){
		unsigned short rand_ret = rand() % 65536;
		RAND_seed(&rand_ret, sizeof(rand_ret));
	}

	ret = SSL_connect(ssl);
	if (ret != 1){
		ERR_print_errors_fp(stderr);
		return;
	}

	ret = SSL_write(ssl, request, (int)strlen(request));
	if (ret < 1){
		ERR_print_errors_fp(stderr);
		return;
	}

	char fn[100];

	//receive
	FILE *file=NULL;
	char get_msg[BUF_LEN];
	int length = 0;
	int htmlCount1 = 0, htmlCount2 = 0;
	ns.receiveLength=0;
	ns.receiveCounter=0;
	for(int i=0 ; i<1000 ; i++){
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
			tm.timeout=0;
			ns.receiveCounter++;
		}

		//getting filesize info
		if(i == 0){
			ns.contentLength = 0;
			char *result = strstr(get_msg, "Content-Length:");
			if(result){
				result += 16;
			}
			if(!result){
				result = strstr(get_msg, "x-oct-size:");
				if(result){
					result += 12;
				}
			}
			if(result){
				for(int j=0 ; j<BUF_LEN ; j++){
					if(result[j]==10 || result[j]==13){
						break;
					}
					ns.contentLength *= 10;
					ns.contentLength += result[j]-48;
				}
			}

			sprintf_s(fn,"save/tmp_search/%d.html",tm.selected);
			fopen_s(&file, fn, "w");
			networkLog(id, fn);
		}
		if(!file){
			break;
		}
		for( int j=0; j<length; j++ ){
			fputc(get_msg[j], file);
			ns.receiveLength++;

			if(ns.contentLength && ns.receiveLength>=ns.contentLength){
				break;
			}

			if(htmlCount1 < 7){
				if(get_msg[j] == endHTML1[htmlCount1]){
					htmlCount1++;
				}else{
					htmlCount1 = 0;
				}
			}

			if(htmlCount2 < 7){
				if(get_msg[j] == endHTML2[htmlCount2]){
					htmlCount2++;
				}else{
					htmlCount2 = 0;
				}
			}
		}

		if(htmlCount1==7 || htmlCount2==7){
			break;
		}
		if(ns.contentLength && ns.receiveLength>=ns.contentLength){
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

	if(tm.tcpsock && tm.halt!=THREAD_SHUTDOWN){
		TCPshutdown(id);
		networkLog(id, "close tcp socket (https google)");
	}

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	ERR_free_strings();
}

char* getHostName(int id, const char *url){
    int size = (int)strlen(url);
    char* host = new char[size];
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

char* getPath(int id, const char *url){
    int size = (int)strlen(url);
    char* path = new char[size];
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
void parseHTML(int id, int n, const char *table_prefix, const char *url_prefix, const char *url_surfix){
	networkLog(id, "parseHTML()");
    char fn[100];
    sprintf_s(fn, "save/tmp_search/%d.html", tm.selected);
    loadFile(fn);
    if(!fsize){
        return;
    }
    char *result = NULL, *url = NULL;

    //fetch the nth URL string
    result = strstr(fstr, table_prefix);
    if(result){
        for(int i=0 ; i<n ; i++){
            result = strstr(result, url_prefix);
            if(result){
                result += strlen(url_prefix);
            }else{
                tm.targetURL[0] = 0;
                return;
            }
        }
    }else{
        tm.targetURL[0] = 0;
        return;
    }
	char *result2 = NULL;
    result2 = strstr(result, url_surfix);
    size_t size = result2-result;

    //put into the variable "url"
    url = new char[size+1];
	n=0;
    for(size_t i=0 ; i<size ; i++){
		if(i<=size-3 && result[i]=='%' && result[i+1]=='2' && result[i+2]=='5'){
			url[n] = '%';
			i+=2;
		}
		else if(i<=size-6 && result[i]=='\\' && result[i+1]=='u' && result[i+2]=='0' && result[i+3]=='0' && result[i+4]=='2' && result[i+5]=='6'){
			url[n] = '&';
			i+=5;
		}
		else if(i<=size-6 && result[i]=='\\' && result[i+1]=='u' && result[i+2]=='0' && result[i+3]=='0' && result[i+4]=='3' && result[i+5]=='d'){
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
    for(size_t i=0 ; i<size ; i++){
		if(i<=size-3 && url[i]=='%' && (url[i+1]=='2' || url[i+1]=='3')){
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

	if(strstr(url, "images-amazon.")){
		std::cout << "skip images-amazon.\n";
		tm.which++;
		parseHTML(id, tm.which, table_prefix, url_prefix, url_surfix);
		return;
	}

    strcpy_s(tm.targetURL, 1000, url);
	FILE *hFile;
	sprintf_s(fn,"save/tmp_url/%d.dat",tm.selected);
	fopen_s(&hFile, fn, "wb");
	networkLog(id, fn);
	fwrite(tm.targetURL, sizeof(tm.targetURL[0]), n/sizeof(tm.targetURL[0]), hFile);
	fclose(hFile);
}

/** get an image file from the URL */
void getTargetImage_http(int id, char *url){
	networkLog(id, "getTargetImage_http()");
	if(url[0] == 0){
		return;
	}
	ns.status=NS_GET_IMAGE;

	char *host = getHostName(id, url);
	char *path = getPath(id, url);

	networkLog(id, "resolve host %s",host);

	//getting IP address
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, host, 80);

	if(res){
		//go to the next image file if failed
		delete host;
		delete path;
		tm.which++;
		tm.timeout = 0;
		ns.status=NS_IPADDRESS_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		networkLog(id, "resolving host failed");
		return;
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
		delete host;
		delete path;
		tm.which++;
		tm.timeout = 0;
		ns.status=NS_CONNECT_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		networkLog(id, "opening tcp failed");
		return;
	}

	networkLog(id, "connected");

	//send request
	long msg_size = strlen(path)+strlen(host)+strlen(USER_AGENT)+50;
	char *msg = new char[msg_size];
	sprintf_s(msg, msg_size, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n", path, host, USER_AGENT);

	networkLog_noparam(id, msg);

	SDLNet_TCP_Send(tm.tcpsock, msg, (int)strlen(msg)+1);
	networkLog(id, SDLNet_GetError());
	delete[] msg;

	receivingImageFile(id, url, NULL);

	if(tm.tcpsock && tm.halt!=THREAD_SHUTDOWN){
		TCPshutdown(id);
		networkLog(id, "closed tcp socket");
	}

	ns.status=NULL;
	delete host;
	delete path;
}

void getTargetImage_https(int id, char *url){
	networkLog(id, "getTargetImage_https()");
	if(url[0] == 0){
		return;
	}

	int ret;
	SSL *ssl;
	SSL_CTX *ctx;

	SSL_load_error_strings();
	SSL_library_init();
	ctx = SSL_CTX_new(SSLv23_client_method());
	if (!ctx){
		ERR_print_errors_fp(stderr);
		return;
	}

	ssl = SSL_new(ctx);
	if (!ssl){
		ERR_print_errors_fp(stderr);
		return;
	}

	ns.status=NS_GET_IMAGE;

	char *host = getHostName(id, url);
	char *path = getPath(id, url);

	networkLog(id, "resolve host %s", host);

	//getting IP address
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, host, 443);

	if(res){
		//go to the next image file if failed
		delete host;
		delete path;
		tm.which++;
		tm.timeout = 0;
		ns.status=NS_IPADDRESS_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		networkLog(id, "resolving host failed");
		return;
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
		delete host;
		delete path;
		tm.which++;
		tm.timeout = 0;
		ns.status=NS_CONNECT_FAILURE;
		parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		networkLog(id, "opening tcp failed");
		return;
	}

	networkLog(id, "connected");

	ret = SSL_set_fd(ssl, tm.tcpsock->channel);
	if (ret == 0){
		networkLog(id, "SSL_set_fd() failed");
		ERR_print_errors_fp(stderr);
		return;
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
		tm.timeout = 0;
		parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		return;
	}

	//send request
	long msg_size = strlen(path)+strlen(host)+strlen(USER_AGENT)+50;
	char *msg = new char[msg_size];
	sprintf_s(msg, msg_size, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n", path, host, USER_AGENT);

	networkLog_noparam(id, msg);

	ret = SSL_write(ssl, msg, (int)strlen(msg));
	if (ret < 1){
		ERR_print_errors_fp(stderr);
		return;
	}
	delete[] msg;

	receivingImageFile(id, url, ssl);

	ret = SSL_shutdown(ssl);
	if (ret != 1){
		ERR_print_errors_fp(stderr);
	}

	if(tm.tcpsock && tm.halt!=THREAD_SHUTDOWN){
		TCPshutdown(id);
		networkLog(id, "closed tcp socket");
	}

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	ERR_free_strings();
	ns.status=NULL;
	delete host;
	delete path;
}

void getTargetImage(int id, char *url){
	if(strstr(url, "https://")){
		getTargetImage_https(id, url);
	}else{
		getTargetImage_http(id, url);
	}
}

void receivingImageFile(int id, char *url, SSL *ssl){
	networkLog(id, "receivingImageFile()");
	FILE *file=NULL;
	char get_msg[BUF_LEN];
	int length = 0;
	char fn[100];

	ns.receiveCounter=0;
	ns.receiveLength=0;
	ifr.reset();

	//receive
	for(int i=0 ; i<10000 ; i++){
		//forced termination
		if(tm.halt){
			networkLog(id, "halted");
			break;
		}
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
		if(length){
			tm.timeout = 0;
			ns.receiveCounter++;
		}

		if(i == 0){
			//error
			if(length==-1){
				networkLog(id, "error code -1 caused by shut-down");
				break;
			}

			//this means that the url is wrong
			char *result = strstr(get_msg, "200 OK");
			if(!result){
				tm.which++;
				tm.timeout = 0;
				networkLog(id, "get text 200 OK (pass it)");
				parseHTML(id, tm.which, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
				tm.halt = RESTART_GETIMAGE;
				break;
			}

			//to redirect
			result = strstr(get_msg, "Location:");
			if(result){
				networkLog(id, "redirect to another url");
				result += 10;
				int size = 0;
				for(size_t j=0 ; j<strlen(result) ; j++){
					if(result[j]==10 || result[j]==13){
						break;
					}
					size++;
				}
				char *url2 = new char[size+1];
				for(int j=0 ; j<size ; j++){
					url2[i] = result[j];
				}
				url2[size] = 0;
				tm.timeout = 0;
				tm.halt = RESTART_GETIMAGE;
				strcpy_s(url, 1000, url2);
				delete[] url2;
				break;
			}

			//getting file size info
			ns.contentLength = 0;
			result = strstr(get_msg, "Content-Length:");
			if(result){
				networkLog(id, "response header has Content-Length");
				result += 16;
			}
			if(!result){
				result = strstr(get_msg, "x-oct-size:");
				if(result){
					networkLog(id, "response header has x-oct-size");
					result += 12;
				}
			}
			if(result){
				for(int j=0 ; j<BUF_LEN ; j++){
					if(result[j]==10 || result[j]==13){
						break;
					}
					ns.contentLength *= 10;
					ns.contentLength += result[j]-48;
				}
				networkLog(id, "content length is %d\n", ns.contentLength);
			}
		}

		for( int j=0 ; j<length ; j++ ){
			//check the startpoint of the image file
			if(!ifr.jpgStart && !ifr.pngStart && !ifr.gifStart){
				ifr.checkPNG(id, get_msg[j]);
				ifr.checkJPG(id, get_msg[j]);
				ifr.checkGIF(id, get_msg[j]);

				if(ifr.jpgStart || ifr.pngStart || ifr.gifStart){
					//save the image file
					sprintf_s(fn, "save/tmp_image/%d.jpg", tm.selected);
					fopen_s(&file, fn, "wb");
					networkLog(id, fn);
					if(!file){
						networkLog(id, "failed to create new img file");
						tm.halt = THREAD_END;
						break;
					}
					if(ifr.jpgStart){
						fputc(-1, file);
						fputc(-40, file);
						ns.receiveLength = 2;
						networkLog(id, "passed JPEG head");
						ns.status = NS_RCV_JPEG;
					}
					else if(ifr.pngStart){
						fputc(-119, file);
						fputc('P', file);
						fputc('N', file);
						fputc('G', file);
						ns.receiveLength = 4;
						networkLog(id, "passed PNG head");
						ns.status = NS_RCV_PNG;
					}
					else if(ifr.gifStart){
						fputc('G', file);
						fputc('I', file);
						fputc('F', file);
						fputc('8', file);
						ns.receiveLength = 4;
						networkLog(id, "passed GIF head");
						ns.status = NS_RCV_GIF;
					}
					continue;
				}
			}

			//writing new image file
			if(ifr.jpgStart || ifr.pngStart || ifr.gifStart){
				fputc(get_msg[j], file);
				ns.receiveLength++;
				
				//check the endpoint of the image file
				if(!ns.contentLength){
					ifr.checkPNG(id, get_msg[j]);
					ifr.checkJPG(id, get_msg[j]);
					ifr.checkGIF(id, get_msg[j]);
				}
			}

			//finish writing a file
			if((ns.contentLength && ns.receiveLength>=ns.contentLength) || ifr.jpgEnd || ifr.pngEnd || ifr.gifEnd){
				tm.finish=true;
				break;
			}
		}

		if(tm.finish){
			break;
		}
	}

	if(file){
		networkLog(id, "finished writing img file");
		fclose(file);
	}
}

void getGoogleImageSearch(int id, char *query){
	networkLog(id, "getGoogleImageSearch()");
	long request_size = strlen(query)+100+120;
    char *request = new char[request_size];
    sprintf_s(request, request_size, "GET /search?q=%s&source=lnms&tbm=isch HTTP/1.1\r\nHost: www.google.co.jp\r\nUser-Agent: %s\r\n\r\n", query, USER_AGENT);
	networkLog_noparam(id, request);
    imageSearch_https(id, "www.google.co.jp", 443, request);
    delete[] request;
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
			networkLog(id, "END CHUNK");
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

void networkLog_noparam(int id, const char *log) {
	if(!OUTPUT_NETWORK_LOG)return;
	for(int i=0 ; i<1000 ; i++){
		if(log[i] == 0) {
			str[i] = 13;
			str[i+1] = 10;
			str[i+2] = 0;
			break;
		}
		str[i] = log[i];
	}

	FILE* hFile;
	char fn[100];
	sprintf_s(fn,"log%d.txt",id);
	fopen_s(&hFile, fn, "ab");
	fwrite(str, sizeof(str[0]), strlen(str)/sizeof(str[0]), hFile);
	fclose(hFile);
}

void networkLog(int id, const char *log, ...) {
	if(!OUTPUT_NETWORK_LOG)return;
	va_list c2;
	va_start(c2, log);
	#ifdef __WIN32__
	vsprintf_s(str,log,c2);
	#else
	vsprintf(str,log,c2);
	#endif
	sprintf_s(str,"%s\n",str);

	FILE* hFile;
	char fn[100];
	sprintf_s(fn,"log%d.txt",id);
	fopen_s(&hFile, fn, "ab");
	fwrite(str, sizeof(fstr[0]), strlen(str)/sizeof(str[0]), hFile);
	fclose(hFile);
}
