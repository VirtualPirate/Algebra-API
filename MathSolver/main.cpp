#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "nlohmann/json.hpp"


#include "Substitutor.hpp"
#include "Operand.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "OperandList.hpp"
#include "Term.hpp"
#include "Expression.hpp"
#include "Operation_Functions.hpp"
#include "Parser.hpp"

#include "Tests.hpp"

using json = nlohmann::json;

const int PORT = 8080;
const int MAX_CONNECTIONS = 5;
const int BUFFER_SIZE = 1024;

struct Operand_And_SubtitutorList {
  Operand operand;
  Variable_Subtitutor_List subtitute_list;
};

Operand_And_SubtitutorList parseJsonToOperand_And_SubtitutorList(json& data) {
  Variable_Subtitutor_List subtitute_list;
  std::string expr{data["expression"]};
  for(auto& e: data["variables"].items()) {
    
    const char* key = e.key().c_str();
    double value = std::stod(e.value().get<std::string>());
    subtitute_list.append(Variable_Subtitutor{*key, value} );
  }

  return Operand_And_SubtitutorList{Operand{expr}, subtitute_list};
}

int main() {


	    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Set socket options
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	// Bind the socket to an address and port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);;
    address.sin_port = htons(PORT);
    bind(server_socket, (struct sockaddr *)&address, sizeof(address));

    // Listen for incoming connections
    listen(server_socket, MAX_CONNECTIONS);

	while(true){
        

        // Accept a connection
        int client_socket = accept(server_socket, nullptr, nullptr);

        // Receive data from the client
        char buffer[BUFFER_SIZE];

        while(true){
          int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
          buffer[bytes_received] = '\0';

          // Parse JSON
          json data = json::parse(buffer);

          // Parse data
          Variable_Subtitutor_List subtitute_list;
          std::string expr{data["expression"]};
          for(auto& e: data["variables"].items()) {
    
            const char* key = e.key().c_str();
            double value = std::stod(e.value().get<std::string>());
            subtitute_list.append(Variable_Subtitutor{*key, value} );
          }


          Operand operand{expr};
          // Variable_Subtitutor_List subtitute_list{parsed_data.subtitute_list};
          std::vector<std::string> simplified_data{};

          //Calculate data
          if(operand){
            if(operand.is_expression()){
                Expression exp{operand.get<Expression>()};
		            exp.subtitute(subtitute_list).get<Expression>().simplify(simplified_data);
            }
            else{
                Operand result{operand.subtitute(subtitute_list).simplify()};
                simplified_data.push_back(get_cout_string(result));
            }
          }

          json send_data;
          send_data["steps"] = json::array();
          for (const auto& str : simplified_data) {
            send_data["steps"].push_back(str);
          }
          const auto json_send = send_data.dump();
          const char* json_send_ = json_send.c_str();


          //Send JSON to client
          send(client_socket, json_send_, strlen(json_send_), 0);
        }


    }

    // Close the socket
    close(server_socket);
    
	return 0;
}


