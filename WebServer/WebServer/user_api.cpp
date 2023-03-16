#include "user_api.h"
#include "quine_mccluskey.h"

using namespace std;

bool handle_api(str &header,str &outbuf,const str& filename )
{
    header += "HTTP/1.1 200 OK\r\n"
              "Content-Type: application/text\r\n"
              "Connection: Close\r\n";
    if(filename.find("quine_mccluskey") != string::npos)
    {
        size_t pos = filename.find('&');
        if (pos != string::npos) {
            string request = filename.substr(pos + 1);
            string response = Quine_McCluskey (request);
            header += "Content-Length:"+ to_string(response.length())+
                    "\r\n"
                    "Server: Lambert-API Server\r\n\r\n";
            outbuf += header;
            outbuf += response;
            return true;
        }
    }
    return false;
}