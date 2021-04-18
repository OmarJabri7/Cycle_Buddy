#include "stdafx.h"

using namespace std;
using json = nlohmann::json;

/** Sample Call Back class inheriting SensorCallback, 
    associated with the sonar sensor
 */
class sonarCallback : public SensorCallback{
    /** Call back functin associated with sonar sensor to return the velocity readiings.
    */
    /** @param t - time of echo heading t obstacle and coming back as reading from sonar sensor
    */
  virtual void dataIn(double t, bool isInterrupt = false){
        auto time_now = chrono::system_clock::now();
        time_t timestamp = chrono::system_clock::to_time_t(time_now);
		double distance = t/58;
		double v = abs((old_distance - (distance)))/10; //speed of incoming item
		conds.car_distance = distance;
		conds.car_velocity = v;
		char time_data[20];
		strftime(time_data, 20, "%H:%M:%S",localtime(&timestamp));
		json json_data;
		json_data["distance"] = distance;
		json_data["velocity"] = v;
		json_data["timestamp"] = time_data;
        string data = json_data.dump();
        const char *buffer_data = data.c_str();
        /** Send sensor readings to app */
        int sock = 0, conn_status;
        struct sockaddr_in server_addr;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(inet_pton(AF_INET, hostIp, &server_addr.sin_addr) <= 0) {
			printf("\nInvalid address/ Address not supported \n");
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT_SONAR);
        conn_status = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
        if(conn_status < 0){
			perror("ERROR connecting\n");
        }
        else {
			send(sock, buffer_data, strlen(buffer_data), 0);
			close(sock);
          }
		old_distance = distance;
    }
};
