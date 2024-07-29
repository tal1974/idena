#include "../idena.h"

using json = nlohmann::json;
using namespace std;
// using namespace string_literals;
class utl {
 public:
  utl(string url, string key) : m_url(url), m_key(key) {}
  const json post(json&);

 private:
  const cpr::Url m_url;
  const string m_key;
};

int main(int argc, char* argv[]) {
  if (argc != 5) {
    cout << "Usage: idena_replenish <address:port> <api.key> <min start "
            "balance> <balance remain after replenish>"
         << endl;
    cout << "Example: idena_replenish http://127.0.0.1:9119 123apikey 10 2"
         << endl;
    return 0;
  }
  utl l_utl(argv[1], argv[2]);

  auto ls_body =
      R"({"method":"dna_getCoinbaseAddr","params":[],"id":1,"key":""})"_json;
  string l_addr = l_utl.post(ls_body)["result"];
  cout << "Address is " << l_addr << endl;
  ls_body = R"({"method":"dna_getBalance","params":[],"id":2,"key":""})"_json;
  ls_body["params"][0] = l_addr;
  double replenish_sum = stod(string(l_utl.post(ls_body)["result"]["balance"]));
  cout << "Balance is " << replenish_sum << endl;
  if (replenish_sum < stod(argv[3])) {
    cout << "Balance is less than " << argv[3] << endl;
    return 0;
  }
  replenish_sum -= stod(argv[4]);
  ls_body =
      R"({"method":"dna_sendTransaction","params":[{"type":22,"from":"","to":"","amount":0}],"id":1,"key":""})"_json;
  ls_body["params"][0]["from"] = l_addr;
  ls_body["params"][0]["to"] = l_addr;
  ls_body["params"][0]["amount"] = replenish_sum;
  cout << "Transaction " << l_utl.post(ls_body);
  return 0;
}
const json utl::post(json& i_json) {
  i_json["key"] = m_key;
  auto lPost = cpr::Post(m_url, cpr::Body{i_json.dump()},
                         cpr::Header{{"Content-Type", "application/json"}});
  if (lPost.status_code != 200) {
    cout << lPost.error.message << endl;
    exit(-1);
  }
  auto jRes = json::parse(lPost.text);
  if (jRes.find("error") != jRes.end()) {
    cout << jRes["error"]["message"] << endl;
    exit(-1);
  }
  return jRes;
}