
#include <iostream>
#include <fstream>
#include <string>

int main()
{



	int clients = 200;
	int waves = clients / 50;

	std::ofstream myfile;
	std::string fileName = "configRes.txt";
	myfile.open (fileName.c_str());


    	for (int i = 0; i < clients; i++)
	{
		myfile << "    link [ attach 0(";
		myfile << i + 3;
		myfile << ") attach ";
		myfile << i + 3;
		myfile << "(0) min_delay 256e-6 prop_delay 0.1 ]\n";
	}


	myfile << "\n\n\n<-------------Client instantiation------------------>\n\n";


    	for (int i = 0; i < waves; i++)
	{
		int min = (i * 50) + 3;
		int max = min + 49;


	myfile << "   host [ idrange [from ";
	myfile << min;
	myfile << " to ";
	myfile << max;
	myfile << "]\n";
	myfile << "     graph [\n";
	myfile << "	ProtocolSession [ name app use \"S3F.OS.UDP.test.UDPClient\" _extends .dict.udp_client_setup_";
	myfile << i;
	myfile << " ]\n";

	myfile << "	ProtocolSession [ name socket use \"S3F.OS.Socket.SocketMaster\" ]\n";
	myfile << "	ProtocolSession [ name udp use \"S3F.OS.UDP.udpSessionMaster\" _find .dict.udpinit ]\n";
	myfile << "        ProtocolSession [ name ip use \"S3F.OS.IP\" ]\n";
	myfile << "      ]\n";
	myfile << "      interface [ id 0 _extends .dict.iface ]\n";
	myfile << "    ]\n";

	myfile << "\n\n\n";

	}


	myfile << "\n\n\n<-------------Client setup------------------>\n\n";


    	for (int i = 0; i < waves; i++)
	{


		myfile << "  udp_client_setup_";
		myfile << i;
		myfile << " [\n";
		myfile << "    start_time ";
		myfile << i+1;
		myfile << "\n";    			
		myfile << "    start_window 0\n";   			
		    					
		myfile << "    file_size 200 \n"; 			
		myfile << "    off_time 10  \n";     			
		myfile << "    off_time_exponential false\n"; 		
		myfile << "    off_time_run_first  false \n"; 		
		myfile << "    user_timeout 10	   \n";   		
		myfile << "    #fixed_server false\n"; 			

		myfile << "    #request_size 4 \n"; 
					
		myfile << "    #server_list forUDP\n"; 
					
		myfile << "    #start_port\n"; 
					
		myfile << "    show_report true  \n"; 
		myfile << "  ]\n\n\n"; 

	}


    myfile.close();




  return 0;
}


