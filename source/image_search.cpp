#include "image_search.h"

ThreadManager tm;
NetworkStatus ns;
char endHTML1[7]={'<','/','H','T','M','L','>'};
char endHTML2[7]={'<','/','h','t','m','l','>'};

struct ImageFormatReader {
    bool jpgStart, pngStart, gifStart;
    bool jpgEnd, pngEnd, gifEnd;
    int jpgPointer, jpgBytes;
    bool jpgImageDataStart;
    int pngPointer1, pngPointer2;
    int gifPointer, gifBytes, gifField;
    void reset();
    void checkJPG(char c);
    void checkPNG(char c);
    void checkGIF(char c);
};
ImageFormatReader ifr;
#define GIF_HEADER 1
#define GIF_GLOBAL_COLOR_TABLES 2
#define GIF_GLAPHIC_CONTROL_EXTENSION_BLOCK 3
#define GIF_COMMENT_EXTENSION_BLOCK 4
#define GIF_PLAIN_TEXT_EXTENSION_BLOCK 5
#define GIF_APPLICATION_EXTENSION_BLOCK 6
#define GIF_IMAGE_DATA_HEADER 7
#define GIF_IMAGE_DATA_BLOCK 8


void getBingImageSearch(const char *query, const char *safe);
void getAskImageSearch(const char *query);
void getGoogleImageSearch(const char *query);
void getTargetImage();
SDL_Texture *createSprite(SDL_Surface *surface);
SDL_Thread *thread;


void TCPshutdown(){
#ifdef __WIN32__
shutdown(tm.tcpsock->channel,SD_RECEIVE);
closesocket(tm.tcpsock->channel);
#else
SDLNet_TCP_Close(tm.tcpsock);
#endif
tm.tcpsock=NULL;
}


int TestThread(void *ptr)
{
	tm.file=NULL;
    tm.running = true;
    tm.halt = 0;
	tm.tcpsock=NULL;
//    getBingImageSearch(query, "OFF");
    getGoogleImageSearch(tm.query);
    parseHTML(tm.idx, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
    getTargetImage();

    if(tm.halt){
        std::cout << "thread: start the next thread\n";
    }else{
        std::cout << "thread: end successfully\n";
        tm.halt = THREAD_SUCCESS;
    }

    tm.running = false;
    return 0;
}

int TestThread2(void *ptr)
{
	tm.file=NULL;
    tm.running = true;
    tm.halt = 0;
	tm.failure=false;
	tm.tcpsock=NULL;
    getTargetImage();

    if(tm.halt){
        std::cout << "thread: 次のスレッドを開始するフラグ\n";
    }else{
        std::cout << "thread: 正常終了フラグ\n";
        tm.halt = THREAD_SUCCESS;
    }

    tm.running = false;
    return 0;
}


void imageSearch_https(const char *host, int port, const char *request){
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

	std::cout << request;

	//IPアドレス取得
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, host, port);
	if(res){
		ns.status=NS_IPADDRESS_FAILURE;
		ns.display=300;
		return;
	}

	//通信開始
	while(tm.tcpsock){}
	tm.tcpsock = SDLNet_TCP_Open(&ipaddress);
	if(!tm.tcpsock) {
		ns.status=NS_CONNECT_FAILURE;
		ns.display=300;
		return;
	}

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

	ret = SSL_write(ssl, request, strlen(request));
	if (ret < 1){
		ERR_print_errors_fp(stderr);
		return;
	}

	char fn[100];

	//受け取る
	char get_msg[BUF_LEN];
	int length = 0;
	int htmlCount1 = 0, htmlCount2 = 0;
	ns.receiveLength=0;
	ns.receiveCounter=0;
	for(int i=0 ; i<1000 ; i++){
		//強制終了
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

		//ファイルサイズ取得
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
				for(int j=0 ; j<2048 ; j++){
					if(result[j]==10 || result[j]==13){
						break;
					}
					ns.contentLength *= 10;
					ns.contentLength += result[j]-48;
				}
			}

			while(tm.file){}
			sprintf_s(fn,"save/tmp_search/%d.html",tm.selected);
			tm.file = fopen(fn, "w");
			if(!tm.file){
				break;
			}
		}
		for( int j=0; j<length; j++ ){
			fputc(get_msg[j], tm.file);
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

	if(tm.file){
		fclose(tm.file);
		tm.file = NULL;
	}

	ret = SSL_shutdown(ssl);
	if (ret != 1){
		ERR_print_errors_fp(stderr);
	}

	if(tm.tcpsock){
		SDLNet_TCP_Close(tm.tcpsock);
		tm.tcpsock = NULL;
	}

	SSL_free(ssl);
	SSL_CTX_free(ctx);
	ERR_free_strings();
}

char* getHostName(const char *url){
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

char* getPath(const char *url){
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

/** 画像検索結果HTMLから、n番目のURLを取り出す */
void parseHTML(int n, const char *table_prefix, const char *url_prefix, char url_surfix){
    char fn[100];
    sprintf_s(fn, "save/tmp_search/%d.html", tm.selected);
    loadFile(fn);
    if(!fsize){
        return;
    }
    char *result = NULL, *url = NULL;

    //URL文字列の取り出し
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
    size_t size = 0;
    for(size_t i=0 ; i<strlen(result) ; i++){
        if(result[i] == url_surfix){
            break;
        }
        size++;
    }

    //url変数に入れる
    url = new char[size+1];
	n=0;
    for(size_t i=0 ; i<size ; i++){
		if(i<=size-3 && result[i]=='%' && result[i+1]=='2' && result[i+2]=='5'){
			url[n] = '%';
			i+=2;
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

    strcpy_s(tm.targetURL, url);
	FILE *hFile;
	sprintf_s(fn,"save/tmp_url/%d.dat",tm.selected);
	fopen_s(&hFile, fn, "wb");
	fwrite(tm.targetURL, sizeof(tm.targetURL[0]), n/sizeof(tm.targetURL[0]), hFile);
	fclose(hFile);
}

/** URL先の画像を取得する */
void getTargetImage(){
	if(tm.targetURL[0] == 0){
		return;
	}
	ns.status=NS_GET_IMAGE;

	char *host = getHostName(tm.targetURL);
	char *path = getPath(tm.targetURL);

	//IPアドレス取得
	IPaddress ipaddress;
	int res = SDLNet_ResolveHost(&ipaddress, host, 80);

	if(res){
		//失敗したら次の画像
		delete host;
		delete path;
		tm.idx++;
		tm.timeout = 0;
		ns.status=NS_IPADDRESS_FAILURE;
		parseHTML(tm.idx, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		return;
	}

	//通信開始
	while(tm.tcpsock){}
	tm.tcpsock = SDLNet_TCP_Open(&ipaddress);

	if(!tm.tcpsock) {
		//失敗したら次の画像
		delete host;
		delete path;
		tm.idx++;
		tm.timeout = 0;
		ns.status=NS_CONNECT_FAILURE;
		parseHTML(tm.idx, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
		tm.halt = RESTART_GETIMAGE;
		return;
	}

	//リクエストする
	int msg_size = strlen(path)+strlen(host)+30;
	char *msg = new char[msg_size];
	sprintf_s(msg, msg_size, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, host);
	SDLNet_TCP_Send(tm.tcpsock, msg, (int)strlen(msg)+1);
	delete msg;

	char get_msg[BUF_LEN];
	int length = 0;
	char fn[100];

	tm.file=NULL;
	ns.receiveCounter=0;
	ns.receiveLength=0;
	ifr.reset();

	//受け取る
	for(int i=0 ; i<10000 ; i++){
		//強制終了
		if(tm.halt){
			break;
		}
		length = SDLNet_TCP_Recv(tm.tcpsock, get_msg, 2048);

		if(!ns.contentLength && !length){
			ns.status=NS_RCV_ZERO_LENGTH;
			ns.display=300;
			break;
		}
		if(length){
			tm.timeout = 0;
			ns.receiveCounter++;
		}

		if(i == 0){
			//httpsは保留
			char *result = strstr(get_msg, "Location: https");
			if(result){
				tm.idx++;
				tm.timeout = 0;
				parseHTML(tm.idx, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
				tm.halt = RESTART_GETIMAGE;
				break;
			}
			//エラーは飛ばす
			result = strstr(get_msg, "200 OK");
			if(!result){
				tm.idx++;
				tm.timeout = 0;
				parseHTML(tm.idx, TABLE_PREFIX, URL_PREFIX, URL_SURFIX);
				tm.halt = RESTART_GETIMAGE;
				break;
			}

			//リダイレクト
			result = strstr(get_msg, "Location:");
			if(result){
				std::cout << "thread: リダイレクト発生\n";
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
				strcpy_s(tm.targetURL, url2);
				delete url2;
				break;
			}

			//ファイルサイズ取得
			ns.contentLength = 0;
			result = strstr(get_msg, "Content-Length:");
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
				for(int j=0 ; j<2048 ; j++){
					if(result[j]==10 || result[j]==13){
						break;
					}
					ns.contentLength *= 10;
					ns.contentLength += result[j]-48;
				}
			}
		}

		for( int j=0 ; j<length ; j++ ){
			//画像ファイルの開始点
            if(!ifr.jpgStart && !ifr.pngStart && !ifr.gifStart){
                ifr.checkPNG(get_msg[j]);
                ifr.checkJPG(get_msg[j]);
                ifr.checkGIF(get_msg[j]);

                if(ifr.jpgStart || ifr.pngStart || ifr.gifStart){
                    //画像ファイルを保存
                    if(tm.file){
                        std::cout << "thread: ファイルオープン 待機\n";
                    }
                    while(tm.file){}
                    std::cout << "thread: ファイルオープン\n";
                    sprintf(fn, "save/tmp_image/%d.jpg", tm.selected);
                    tm.file = fopen(fn, "wb");
                    if(!tm.file){
                        std::cout << "thread: ファイルオープン失敗\n";
                        tm.halt = THREAD_END;
                        break;
                    }
                    if(ifr.jpgStart){
                        fputc(-1, tm.file);
                        fputc(-40, tm.file);
						ns.receiveLength = 2;
						ns.status = NS_RCV_JPEG;
                    }
                    else if(ifr.pngStart){
                        fputc(-119, tm.file);
                        fputc('P', tm.file);
                        fputc('N', tm.file);
                        fputc('G', tm.file);
						ns.receiveLength = 4;
						ns.status = NS_RCV_PNG;
                    }
                    else if(ifr.gifStart){
                        fputc('G', tm.file);
                        fputc('I', tm.file);
                        fputc('F', tm.file);
                        fputc('8', tm.file);
						ns.receiveLength = 4;
						ns.status = NS_RCV_GIF;
                    }
                    continue;
                }
            }


            //ファイル書き込み
            if(ifr.jpgStart || ifr.pngStart || ifr.gifStart){
                fputc(get_msg[j], tm.file);
				ns.receiveLength++;

                //ファイルの終了点
                if(!ns.contentLength){
                    ifr.checkPNG(get_msg[j]);
                    ifr.checkJPG(get_msg[j]);
                    ifr.checkGIF(get_msg[j]);
                }
            }

			//書き込み完了
			if((ns.contentLength && ns.receiveLength>=ns.contentLength) || ifr.jpgEnd || ifr.pngEnd || ifr.gifEnd){
				tm.finish=true;
				break;
			}
		}

		if(tm.finish){
			break;
		}
	}

	if(tm.file){
		fclose(tm.file);
		tm.file = NULL;
	}

	if(tm.tcpsock){
		SDLNet_TCP_Close(tm.tcpsock);
		tm.tcpsock = NULL;
	}

	ns.status=NULL;
	delete host;
	delete path;
}

/** 画像検索結果HTMLを取得 */
void imageSearch(const char *host, int port, const char *request){
    //IPアドレス取得
    IPaddress ipaddress;
    std::cout << "thread: IPアドレス取得開始\n";
    int res = SDLNet_ResolveHost(&ipaddress, host, port);
    if(res){
        std::cout << "thread: IPアドレス取得失敗\n";
        return;
    }

    //通信開始
    if(tm.tcpsock){
        std::cout << "thread: TCP開始 待機\n";
    }
    while(tm.tcpsock){}
    std::cout << "thread: TCP開始\n";
    tm.tcpsock = SDLNet_TCP_Open(&ipaddress);
    if(!tm.tcpsock) {
        std::cout << "thread: TCP失敗\n";
        return;
    }

    //リクエストする
    SDLNet_TCP_Send(tm.tcpsock, request, (int)strlen(request)+1);

    int contentLength = 0, written = 0;
    char fn[100];

    //受け取る
    char get_msg[2048];
    int length = 0;
    int htmlCount1 = 0, htmlCount2 = 0;
    for(int i=0 ; i<1000 ; i++){
        //強制終了
        if(tm.halt){
            std::cout << "thread: TCP受信 強制終了\n";
            break;
        }
        std::cout << "*";
        length = SDLNet_TCP_Recv(tm.tcpsock, get_msg, 2048);

        if(!contentLength && !length){
            std::cout << "thread: TCP受信ゼロ Content-Length不明で終了\n";
            break;
        }

        //ファイルサイズ取得
        if(i == 0){
            contentLength = 0;
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
                for(int j=0 ; j<2048 ; j++){
                    if(result[j]==10 || result[j]==13){
                        break;
                    }
                    contentLength *= 10;
                    contentLength += result[j]-48;
                }
            }else{
                std::cout << "thread: Content-Length なし\n";
            }

            if(tm.file){
                std::cout << "thread: ファイルオープン 待機\n";
            }
            while(tm.file){}
            std::cout << "thread: ファイルオープン\n";
            sprintf_s(fn, "save/tmp_search/output%d.html", tm.selected);
            tm.file = fopen(fn, "w");
            if(!tm.file){
                std::cout << "thread: ファイルオープン失敗\n";
                break;
            }
        }

        for( int j=0; j<length; j++ ){
            fputc(get_msg[j], tm.file);
            written++;

            if(contentLength && written>=contentLength){
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
            std::cout << "thread: HTML終了タグ到達 受信完了\n";
            break;
        }
        if(contentLength && written>=contentLength){
            std::cout << "thread: contentLength到達 受信完了\n";
            break;
        }
    }

    if(tm.file){
        std::cout << "thread: ファイルクローズ\n";
        fclose(tm.file);
        tm.file = NULL;
    }

    if(tm.tcpsock){
        std::cout << "thread: TCP終了\n";
        SDLNet_TCP_Close(tm.tcpsock);
        tm.tcpsock = NULL;
    }
}


void getGoogleImageSearch(const char *query){
	int request_size = strlen(query)+100+120;
    char *request = new char[request_size];
    sprintf_s(request, request_size, "GET /search?q=%s&source=lnms&tbm=isch HTTP/1.1\nHost: www.google.co.jp\nUser-Agent: %s\n\n", query, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36");
    std::cout << "thread: send TCP \n";
    imageSearch_https("www.google.co.jp", 443, request);
    delete request;
}


void getBingImageSearch(const char *query, const char *safe){
    //リクエストする
	int request_size = strlen(query)+100;
    char *request = new char[request_size];
    sprintf_s(request, request_size, "GET /images/search?q=%s HTTP/1.1\nHost: www.bing.com\nCookie: SRCHHPGUSR=ADLT=%s\n\n", query, safe);
    std::cout << "thread: TCP送信\n";
    imageSearch("www.bing.com", 80, request);
    delete request;
}

void ImageFormatReader::reset(){
    jpgStart = false;pngStart = false;gifStart = false;
    jpgEnd = false;pngEnd = false;gifEnd = false;
    jpgPointer = 0;jpgBytes = 0;
    jpgImageDataStart = false;
    pngPointer1 = 0;pngPointer2 = 0;
    gifPointer = 0;gifBytes = 0;gifField = 0;
}

void ImageFormatReader::checkPNG(char c){
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
                std::cout << "PNG START\n";
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
            std::cout << "PNG END\n";
        }else{
            pngPointer2 = 0;
        }
    }
}

void ImageFormatReader::checkJPG(char c){
    if(jpgImageDataStart){
        if(c == -1){
            jpgPointer = 0;
        }
        else if(jpgPointer == 1 && c == -39){
            jpgPointer = 0;
            jpgEnd = true;
            std::cout << "JPG END\n";
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
            std::cout << "JPG START\n";
            return;
        }
        else if(c == -38){
            jpgImageDataStart = true;
            std::cout << "IMAGE DATA START\n";
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
            std::cout << "END CHUNK\n";
            return;
        }
    }
    jpgPointer++;
}

void ImageFormatReader::checkGIF(char c){
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
                std::cout << "GIF START\n";
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
                std::cout << "IMAGE DATA HEADER\n";
                return;
            }
            else if(c == 59){
                gifEnd = true;
                std::cout << "GIF END\n";
            }
        }
        else if(gifPointer == 1){
            if(c == -7){
                gifPointer = 2;
                gifField = GIF_GLAPHIC_CONTROL_EXTENSION_BLOCK;
                std::cout << "GLAPHIC CONTROL EXTENSION BLOCK\n";
                return;
            }
            else if(c == -2){
                gifPointer = 2;
                gifField = GIF_COMMENT_EXTENSION_BLOCK;
                gifBytes = 0;
                std::cout << "COMMENT EXTENSION BLOCK\n";
                return;
            }
            else if(c == 1){
                gifPointer = 2;
                gifField = GIF_PLAIN_TEXT_EXTENSION_BLOCK;
                gifBytes = 0;
                std::cout << "PLAIN TEXT EXTENSION BLOCK\n";
                return;
            }
            else if(c == -1){
                gifPointer = 2;
                gifField = GIF_APPLICATION_EXTENSION_BLOCK;
                gifBytes = 0;
                std::cout << "APPLICATION EXTENSION BLOCK\n";
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
            std::cout << "GLOBAL COLOR TABLES: ";
            std::cout << gifBytes;
            std::cout << "\n";
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
                std::cout << "COMMENT: ";
                std::cout << a;
                std::cout << "\n";
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
                std::cout << "COMMENT: ";
                std::cout << a;
                std::cout << "\n";
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
                std::cout << "DATA: ";
                std::cout << a;
                std::cout << "\n";
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
                std::cout << "DATA: ";
                std::cout << a;
                std::cout << "\n";
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
                std::cout << "DATA: ";
                std::cout << a;
                std::cout << "\n";
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
                std::cout << "DATA: ";
                std::cout << a;
                std::cout << "\n";
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
                std::cout << "LOCAL COLOR TABLES: ";
                std::cout << gifBytes;
                std::cout << "\n";
            }else{
                gifBytes = 0;
                std::cout << "LOCAL COLOR TABLES: none\n";
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
                std::cout << "IMAGE DATA BLOCK: ";
                std::cout << gifBytes;
                std::cout << "\n";
            }
        }
        else if(gifPointer == gifBytes){
            gifPointer = 0;
            return;
        }
    }
    gifPointer++;
}
