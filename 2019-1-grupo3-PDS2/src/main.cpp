#include <iostream>
#include <string>
#include "../lib/nlohmann/json.hpp"
#include "../lib/user.hpp"
#include "../lib/hashtag.hpp"
#include "../lib/tweet.hpp"
#include "../lib/interface.hpp"

using Json = nlohmann::json;

int main() {
  int op = -1;

  while(op != 0){
    Twitter::Interface::splash();
    op = Twitter::Interface::initialize();
    delete Twitter::logged_user;
  }
  std::cout << "Bye!" << std::endl;
  return 0;
}
