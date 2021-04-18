#include "stdafx.h"

using namespace std;
using json = nlohmann::json;

/** Sample Call Back class inheriting SensorCallback, 
    associated with the hall effect sensor
 */
class hallSampleCallback : public SensorCallback{
    /** Call back functin associated with hall effect sensor to return the velocity readiings.
    */
    /** @param v - velocity of bicycle wheel reading from hall sensor
    */
  virtual void dataIn(double v, bool isInterrupt = false){
    conds.bike_velocity = v;
    if(isInterrupt == false){
      int sock = 0, conn_status;
      auto time_now = chrono::system_clock::now();
      time_t timestamp = chrono::system_clock::to_time_t(time_now);
      char time_data[20];
      strftime(time_data, 20, "%H:%M:%S",localtime(&timestamp));
      json json_data;
      json_data["distance"] = (double)-1.0;
      json_data["velocity"] = v;
      json_data["timestamp"] = time_data;
      string data = json_data.dump();
      const char *buffer_data = data.c_str();
      /** Send sensor readings to app */
      struct sockaddr_in server_addr;
      sock = socket (AF_INET, SOCK_STREAM, 0);
      if(inet_pton(AF_INET, hostIp, &server_addr.sin_addr) <= 0){
	  printf("\nInvalid address/ Address not supported \n");
      }
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(PORT_HALL);
      conn_status = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
      if(conn_status < 0){
	   perror("ERROR connecting\n");
      }
      else {
	   send(sock, buffer_data, strlen(buffer_data), 0);
	   close(sock);
      }
    }
    else{
		if(upcoming_car == 1 && v >= VT && bike_flag == 0){
			bike_flag = 1;
		}
    }
  }
};
