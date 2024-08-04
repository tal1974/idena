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
  CLI::App app{"Idena replenish"};
  argv = app.ensure_utf8(argv);
  std::string l_url = "http://127.0.0.1:9119", l_apikey;
  app.add_option("-u,--url", l_url,
                 "URL for node. Default is http://127.0.0.1:9119");
  app.add_option("-a,--apikey", l_apikey, "API key for yours node.")
      ->required();
  int l_balthres = 10, l_balremain = 2;
  app.add_option("-t,--threshold", l_balthres, "Balance threshold. Default 10");
  app.add_option("-r,--remain", l_balremain,
                 "Balance remain after stake replenish. Default 2");
  CLI11_PARSE(app, argc, argv);

  utl l_utl(l_url, l_apikey);

  auto ls_body =
      R"({"method":"dna_getCoinbaseAddr","params":[],"id":1,"key":""})"_json;
  string l_addr = l_utl.post(ls_body)["result"];
  cout << "Address is " << l_addr << endl;
  ls_body = R"({"method":"dna_getBalance","params":[],"id":2,"key":""})"_json;
  ls_body["params"][0] = l_addr;
  double replenish_sum = stod(string(l_utl.post(ls_body)["result"]["balance"]));
  cout << "Balance is " << replenish_sum << endl;
  if (replenish_sum < l_balthres) {
    cout << "Balance is less than " << l_balthres << endl;
    return 0;
  }
  replenish_sum -= l_balremain;
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