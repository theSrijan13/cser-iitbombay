#include "server.hpp"
#include "bbl.pb.h"

#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::fstream;
using std::ios;
using std::string;
using std::stringstream;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#define PREFIX "[SERV] "

namespace bbl {

int Server::_static_handle(	void * cls, // Server*
				struct MHD_Connection * connection,
				const char * url,
				const char * method,
				const char * version,
				const char * upload_data,
				size_t * upload_data_size,
				void ** ptr ) {
	Server* server = (Server*) cls;
	return server->_handle(connection, url, method, version, upload_data, upload_data_size, ptr);
}

int Server::_handle(	struct MHD_Connection * connection,
			const char * url,
			const char * method,
			const char * version,
			const char * upload_data,
			size_t * upload_data_size,
			void ** ptr ) {
	static int dummy;

	// PUT and POST data must first be gathered
	// But not yet implemented => 404
	if (strcmp("PUT", method) == 0 || strcmp("POST", method) == 0) {
		// Not yet handled => 404
		cerr << PREFIX << method << " not yet handled..." << endl;
		return MHD_NO;
	}
	
	if (&dummy != *ptr) {
		/* The first time only the headers are valid,
		 do not respond in the first round... */
		*ptr = &dummy;
		return MHD_YES;
	}

	*ptr = NULL; /* clear context pointer */
	result r = this->_handler->handle(method, url);
	
	if (r.httpCode == MHD_HTTP_OK) {
		struct MHD_Response * response;
		int ret;
		response = MHD_create_response_from_data( r.data.size(),
						   (void*) r.data.data(),
						   MHD_NO,
						   MHD_YES); // Ask MHD to copy data
		MHD_add_response_header( response, "Content-Type", r.mimeType != "" ? r.mimeType.c_str() : "text/html");
		if (r.encoding != "")
			MHD_add_response_header( response, "Content-Encoding", r.encoding.c_str());
		ret = MHD_queue_response(connection,
				   MHD_HTTP_OK, // 202
				   response);
		MHD_destroy_response(response);
		return ret;
	} else {
		return MHD_queue_response(connection, r.httpCode, NULL);
	}
}

int Server::_static_filter(
		void *cls, // Server*
		const struct sockaddr * addr,
		socklen_t addrlen) {
	Server* server = (Server*) cls;
	return server->_filter(addr, addrlen) ? MHD_YES : MHD_NO;
}

bool Server::_filter(const struct sockaddr * addr, socklen_t addrlen) {
	// TODO filter non local addresses.
	return true;
}

bool Server::start() {
	cout << PREFIX << "Starting..." << endl;
	this->_daemon = MHD_start_daemon(this->_threadPoolSize >0 ? MHD_USE_SELECT_INTERNALLY : MHD_USE_THREAD_PER_CONNECTION,
		this->_port,
		& (Server::_static_filter), this,
		& (Server::_static_handle), this,
		MHD_OPTION_THREAD_POOL_SIZE, this->_threadPoolSize,
		MHD_OPTION_END);
	if (this->_daemon == NULL)
		return false;
	cout << PREFIX << "port: " << this->_port << endl;
	cout << PREFIX << "threads: " << this->_threadPoolSize << endl;
	return true;
}

void Server::stop() {
	MHD_stop_daemon(this->_daemon);
}

} // namespace ::bbl

class DefaultHandler : public bbl::Handler {
	public:
	  DefaultHandler(const std::string& chroot) : _chroot(chroot), prefix("[DH  ] ") {}
	  ~DefaultHandler() {}

	public:
	  bbl::result handle(const string& method, const string& url) {
		cout << prefix << method << ":" << url << endl;
		bbl::result r;
		fs::path pPath( _chroot / url);
		if ( !fs::exists( pPath ) ) {
			cout << prefix << "  Not found " << pPath.file_string() << endl;
			r.httpCode = MHD_HTTP_NOT_FOUND;
			return r;
		}
		if ( fs::is_directory( pPath ) && fs::exists( pPath / "index.html") ) {
			pPath = pPath / "index.html";
		}

		const string sPath = pPath.file_string();
		if ( fs::is_directory ( pPath ) ) {
			// list files
			cout << prefix << "  Listing directory" << endl;
			fs::directory_iterator end_iter;
			stringstream buffer;
			buffer << "<html><head><title>" << pPath.file_string() << "</title></head>" << endl;
			buffer << "<body>" << endl;
			buffer << "<h3>" << pPath.file_string() << "</h3>" << endl;
			buffer << "</p>" << endl;
			for ( fs::directory_iterator dir_itr( pPath );
				dir_itr != end_iter;
				++dir_itr ) {
				string link = dir_itr->path().filename();
				try {
					buffer << "<a href='" << link << "'>" << link << "</a><br/>" << endl;
				} catch ( const std::exception & ex ) {
					cout << link << " " << ex.what() << endl;
				}
			}
			buffer << "</p>" << endl;
			buffer << "</body>" << endl;
			buffer << "</html>" << endl;
			r.httpCode = MHD_HTTP_OK;
			r.mimeType = MIME_HTML;
			r.data = buffer.str();
		} else {
			fstream file(sPath.c_str(), ios::binary | ios::ate | ios::in);
			int size = file.tellg();
			cout << prefix << "  Serving " << sPath << " (" << size << " bytes)" << endl;
			char* memblock = new char [size];
			file.seekg (0, std::ios::beg);
			file.read (memblock, size);
			file.close();
	
			r.httpCode = MHD_HTTP_OK;
			r.data = string(memblock, size);
		}
		return r;
	  };

	private:
	  string prefix;
	  fs::path _chroot;
};

int main(int argc, char** argv) {
	if (argc != 4) {
		cerr << "Usage : server port threadPoolSize webroot" << endl;
		return 1;
	}
	bbl::Server s(atoi(argv[1]), atoi(argv[2]), new DefaultHandler(argv[3]));
	s.start();
	char c;
	cin >> c;
	s.stop();
}