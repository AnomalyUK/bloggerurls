#include "server_http.hpp"
#include <fstream>

using namespace std;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

std::map<string,string> lookups;
bool debug;

int read_rewrites(string filename) {
  std::ifstream f(filename);
  string line;
  while (getline(f, line)) {
    auto space = line.find_first_of(" ");
    auto oldpath = line.substr(0, space);
    auto newpath = line.substr(space+1);
    lookups[oldpath] = newpath;
  }
  return lookups.size();
}
  
void handle(shared_ptr<HttpServer::Response> response,
            shared_ptr<HttpServer::Request> request)
{
  if (debug) cout << "handling\n";
  auto fields = request->parse_query_string();
  auto blogger = fields.find("blogger");
  if (blogger == fields.end()) {
    if (debug) cout << "no query string\n";
    response->write(SimpleWeb::StatusCode::client_error_not_found);
    return;
  }
  auto key = blogger->second;
  if (debug) cout << "key " << key << "\n";
  auto found = lookups.find(key);
  if (found == lookups.end()) {
    response->write(SimpleWeb::StatusCode::client_error_not_found, key);
    if (debug) cout << "no match for query string\n";
    return;
  }
  SimpleWeb::CaseInsensitiveMultimap location;
  location.insert(pair<string,string>{string("Location"), found->second});
  response->write(SimpleWeb::StatusCode::redirection_found, location);
  if (debug) cout << "redirected to " << found->second << "\n";
}

int main(int argc, char **argv) {
  string filename = "rewrites.txt";
  int opt;
  debug = false;

  while ((opt = getopt(argc, argv, "df:")) != EOF)
    switch(opt) {
    case 'd': debug = true; break;
    case 'f': filename = optarg; break;
    }
  
  auto count = read_rewrites(filename);
  cerr << "Read " << count << " rewrites\n";
  HttpServer server;

  server.config.port = 2000;
  server.config.thread_pool_size = 2;
  server.resource["^/(.*)$"]["GET"] = handle;

  server.start();
  return 0;
}
