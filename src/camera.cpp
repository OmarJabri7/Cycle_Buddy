#include "stdafx.h"

using namespace std;
using json = nlohmann::json;

/** mutex obj defined to enable usage of mutex lock to prevent conflict */
std::mutex mtx;

/** function for Photo capture and car plate recognization */
void camera_process(){
	/** initialize the camera */
	raspicam::RaspiCam Camera;
    if(!Camera.open()){
      cerr << "Error opening camera" << endl;
    }
    cout << "###### Stabilizing camera... #######" << endl;
    cout << "###### Camera configured ######" << endl;
    int captureCount = 0;
    /** looping and waiting for incoming photo timing */
	while(true){
		double car_distance = 0;
		double car_velocity = 0;
		double bike_velocity = 0;
		
		/** when all conditions for taking photo fullfills, trigger the camera to take one */
		
		if(conds.car_distance <= DT && conds.car_velocity >= VT && conds.bike_velocity >= VT){
			captureCount++;
			car_distance = conds.car_distance;
			car_velocity = conds.car_velocity;
			bike_velocity = conds.bike_velocity;
			mtx.lock();
			cout << "####### CAPTURING IMAGE #######" << endl;
			Camera.grab();
			//allocate memory
			unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
			//extract the image in rgb format
			Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB ); //get camera image
			//save
			ofstream outFile ( "src/img.jpg",ios::binary );
			outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
			outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
			cout<<"Image saved at src/img.jpg"<<endl;
			
			/** upload the photo to the online API for plate recognization */
			string res = exec("alpr -c gb src/img.jpg");
			cout << "Capture No. " << captureCount << endl;
			string car_plate = getLicensePlate(res);
			cout << "RESULT: " << car_plate << endl;
			delete data;
			exec("python3 src/upload.py");
			//Send Data
			json json_data;
			json_data["car_plate"] = car_plate;
			json_data["car_distance"] = car_distance;
			json_data["car_velocity"] = car_velocity;
			json_data["bike_velocity"] = bike_velocity;
			string result_data = json_data.dump();
			const char *buffer_data = result_data.c_str();
			/** Send sensor readings to app */
			int sock = 0, conn_status;
			struct sockaddr_in server_addr;
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if(inet_pton(AF_INET, hostIp, &server_addr.sin_addr) <= 0) {
				printf("\nInvalid address/ Address not supported \n");
			}
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(PORT_DATA);
			conn_status = connect(sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
			if(conn_status < 0){
				perror("ERROR connecting\n");
			}
			else {
				send(sock, buffer_data, strlen(buffer_data), 0);
				close(sock);
			}
			mtx.unlock();
        }
    }
}
	
