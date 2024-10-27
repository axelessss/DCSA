#include "Base64.hpp"
#include "../contrib/argparse.hpp"
#include <hv/HttpClient.h>
#include <iostream>

enum class Action {
  Add,
  Delete,
  Get,
  Update,
  Invalid
};

const std::string BASE_URL = "http://0.0.0.0:7777/user";

Action resolveAction(const std::string& actionStr) {
  if (actionStr == "add") return Action::Add;
  if (actionStr == "delete") return Action::Delete;
  if (actionStr == "get") return Action::Get;
  if (actionStr == "update") return Action::Update;
  return Action::Invalid;
}

void setupHttpClient(hv::HttpClient& client, HttpRequest& req, argparse::ArgumentParser& program, const Action action) {
  using json = nlohmann::json;
  switch (action) {
    case Action::Add: {
      std::string login = program.get<std::string>("--login");
      std::string password = program.get<std::string>("--password");
      req.method = HTTP_POST;
      req.url = BASE_URL;
      req.headers["Content-Type"] = "application/json";
      json data = {{"Login", login}, {"Password", password}};
      req.body = data.dump();
      break;
    }
    case Action::Delete: {
      std::string login = program.get<std::string>("--login");
      std::string password = program.get<std::string>("--password");
      std::string id = program.get<std::string>("--id");
      req.method = HTTP_DELETE;
      req.url = BASE_URL + "/" + id;
      std::string auth_header = "Basic " + base64::Encode(login + ":" + password);
      req.headers["Authorization"] = auth_header;
      break;
    }
    case Action::Get:
      req.method = HTTP_GET;
      req.url = BASE_URL + "s";
      break;
    case Action::Update: {
      std::string id = program.get<std::string>("--id");
      std::string login = program.get<std::string>("--login");
      std::string password = program.get<std::string>("--password");
      std::string newLogin = program.get<std::string>("--login2");
      std::string newPassword = program.get<std::string>("--password2");
      req.url = BASE_URL + "/" + id;
      req.headers["Content-Type"] = "application/json";
      json data = {{"Login", newLogin}, {"Password", newPassword}};
      req.body = data.dump();
      std::string auth_header = "Basic " + base64::Encode(login + ":" + password);
      req.headers["Authorization"] = auth_header;
      break;
    }
    default:
      throw std::invalid_argument("Invalid action");
  }
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("HTTP Client");
  program.add_argument("action").help("Action to perform (get, add, delete, update)");
  program.add_argument("--login2").help("New User login");
  program.add_argument("--password2").help("New User password");
  program.add_argument("--login").help("User login");
  program.add_argument("--password").help("User password");
  program.add_argument("--id").help("User ID");

  try {
    program.parse_args(argc, argv);
    std::string actionStr = program.get<std::string>("action");
    Action action = resolveAction(actionStr);

    hv::HttpClient client;
    HttpRequest req;
    setupHttpClient(client, req, program, action);

    HttpResponse resp;
    int code = client.send(&req, &resp);
    std::cout << "Request successful, response body: " << resp.body << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
