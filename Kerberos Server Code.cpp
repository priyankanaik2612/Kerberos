// Server side C/C++ program to demonstrate Socket programming 
#include<iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
using namespace std;
#include<iostream>
#include<bits/stdc++.h>
using namespace std;
class Base_Encryption{
	public: 
		int mapping_encrypt(char a)
		{
			return int(a);
		}
		int mapping_decrypt(int a)
		{
			return char(a);
		}
		
		string decrypt(string message,int key)
		{
			string ct;
				for(int i=0;i<message.size();i++)
				{
					int num=mapping_encrypt(message[i]);
					num=(num-key);
					char c=mapping_decrypt(num);
					ct=ct+c;
				}
				return ct;
		}
			string encrypt(string message,int key)
		{
			string ct;
				for(int i=0;i<message.size();i++)
				{
					int num=mapping_encrypt(message[i]);
					num=(num+key);
					char c=mapping_decrypt(num);
					ct=ct+c;
				}
				return ct;
		}
	
};
class Real_Server: public Base_Encryption{
	string name;
	public:
		void set_name(string a)
		{
			this->name=a;
		}
		string getname()
		{
			return name;
		}
		string generate_session_key()
	{
		string str;
			for(int i=0;i<5;i++)
			{
				str+=rand()%26 + '0'+49;
			}
		//this->session_key=str;
			return str;
		
	}

		string get_acknowledgement(string Ticket_B, string encrypted_timestamp,time_t &now,int Session_AB)
		{
			string ts=decrypt(encrypted_timestamp,Session_AB);
			ts=ts+"1";// B sends acknowledgment by incrementing timestamp by 1
			now=now+1;
			string result=encrypt(ts,Session_AB);
			return result;
		}
	
};
class Password_Authenticator{
	public:
		int check(int userid, int &p, string password)
		{
			int authenticator=userid*321;
			//cout<<"\nEnter symmetric password(client): ";
			//string password;
			//cin>>password;
			for(int i=0;i<password.size();i++)
			{
				if(int(password[i]-'0'-48)!=authenticator%10)
				{
					return -1;
				}
				authenticator=authenticator/10;
				
			}
			p=(userid*321)%6;
			return p;
			
		}
		~Password_Authenticator()
		{
			cout<<"The password is being destroyed immediately by calling destructor for safety.";
		}
};

class Ticket_Granting_Server: public Base_Encryption{
	int ticket;
	int tgs_symmetric_key=5;
	public:
		string get_encrypted_ticket()
		{string str;
			for(int i=0;i<5;i++)
			{
				str+=rand()%26 + '0'+49;
			}
			
			return str;
		}
		int request(string Session_Key_A, Real_Server B, string Timestamp)
		{
			//Generate Session Key between A & B
			int key=rand()%3 + 1;
			return key;
			
		}
			int mapping_encrypt(char a)
		{
			return int(a-'0'-49);
		}
		int mapping_decrypt(int a)
		{
			return char(a+'0'+49);
		}
		
		string encrypt(string message,int key)
		{
			string ct;
				for(int i=0;i<message.size();i++)
				{
					int num=mapping_encrypt(message[i]);
					num=(num+key)%26;
					char c=mapping_decrypt(num);
					ct=ct+c;
				}
				return ct;
		}
		
	vector<string> get_response(int Key_AB,string A_sess_key, Real_Server B)
		{
			vector<string> result;
			string B_sess_key=B.generate_session_key();
			string b=encrypt(B_sess_key,Key_AB);
			string a=encrypt(A_sess_key,Key_AB);
			result.push_back(a);
			result.push_back(b);
			return result;
			
						
		}
		
	
};
class Authentication_Server: public Base_Encryption{
	
	public:
		int request(int id, string password)
		{
			int key;
			Password_Authenticator p;
			int valid=p.check(id, key,password);
			if(valid==-1)
			{
				cout<<"\nPassword invalid, try again!";
				return -1;
			}
			else{
				cout<<"\nThe password is valid"<<endl;
				//A.generate_permanent_symmetric_key(valid);
				cout<<"\nPermanent Symmetric key is generated based on password & algo."<<endl;
				//p.~Password_Authenticator(); 
				return valid;
				
			}
				
		}
	string generate_session_key()
	{
		string str;
			for(int i=0;i<5;i++)
			{
				str+=rand()%26 + '0'+49;
			}
		//this->session_key=str;
			return str;
		
	}

		string get_response(int ps_key)
		{	Ticket_Granting_Server tgs;
			string message;
			string ticket=tgs.get_encrypted_ticket();  // Ticket for TGS, encrypted with TGS symmetric key;
			string session_key=generate_session_key(); // K-A_TGS
			message=ticket+session_key;
			cout<<"\n\nAS Message before encryption: "<<message;
			string encrypted_message=encrypt(message,ps_key);
			cout<<"\nEncrypted AS message: "<<encrypted_message;
			return encrypted_message;
		}	
};


time_t get_currtime()
{
	time_t now = time(0);

return now;
}
	int mapping_encrypt(char a)
		{
			return int(a-'0'-49);
		}
	int mapping_decrypt(int a)
		{
			return char(a+'0'+49);
		}
	string encrypt(string message,int key)
		{
			string ct;
				for(int i=0;i<message.size();i++)
				{
					
					int num=mapping_encrypt(message[i]);
					num=(num+key);
					char c=mapping_decrypt(num);
				//	cout<<"\n"<<message[i]<<"  "<<c;
					ct=ct+c;
				}
				return ct;
		}
	string decrypt(string ct,int key)
	{
		string pt;
		for(int i=0;i<ct.size();i++)
		{
			int num=mapping_encrypt(ct[i]);
			num=(num-key);
			char c=mapping_decrypt(num);
			pt=pt+c;
		//	cout<<"\n"<<ct[i]<<"  "<<c;
		}
		
		return pt;
	}

Authentication_Server AS;
Ticket_Granting_Server TGS;
Real_Server B;
int main()
{
	
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Request accepted from Auth Server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

//get 1
valread = read( new_socket , buffer, 1024); 
	cout<<"\n****From Client: "<<buffer<<endl ; 
// send 1
	send(new_socket , hello , strlen(hello) , 0 ); 
	cout<<"\nRequest from client recieved  by Auth Server\n"; 

	fflush(stdin);
	srand(time(0)); 
	//User A=User("Alice",1);
	B.set_name("Bob");

	cout<<"\n\n************************************";
	cout<<"\nVerification by Authentication Server";
	cout<<"\n************************************";  


//get 2
memset(buffer,0,sizeof(buffer));
valread = read( new_socket , buffer, 1024); 
	cout<<buffer<<endl ; 
	int userid= atoi(buffer);
cout<<"\n****From Client: User ID for connection "<< userid;

cout<<"\n Asking client for password";
//get 3
memset(buffer,0,sizeof(buffer));
valread = read( new_socket , buffer, 1024); 
	cout<<"\n****From Client: password is "<<buffer;
string password=buffer;
	int k=AS.request(userid,password);
	if(k==-1)
{
cout<<"\nIncorrect password. Authentication not possible.";
exit(0);
}
// send 5
	std::string s_int = std::to_string(k);
char *s5 = new char[s_int.length() + 1];
strcpy(s5, s_int.c_str());
send(new_socket , s5 , strlen(s5) , 0 ); 
	cout<<"\nPermanent symmetric key generated. \n"; 
//send 2
	cout<<"\nUsing this key to encrypt session key and ticket";

	string AS_message=AS.get_response(k);
	char *s2 = new char[AS_message.length() + 1];
	strcpy(s2, AS_message.c_str());
	send(new_socket , s2 , strlen(s2) , 0 ); 
cout<<"\nSending ticket+sessionkey back to Client";
	
cout<<"\n\n************************************";
cout<<"\nTicket Granting Server generating Sessionkey & Ticket";
cout<<"\n************************************";

//get 4 -timestamp
	memset(buffer,0,sizeof(buffer));
	valread = read( new_socket , buffer, 1024); 
	string timestamp=buffer;
	cout<<"\n****From Client: Timestamp sent to TGS "<< timestamp;
//get 6 -session key (A)
	memset(buffer,0,sizeof(buffer));
	valread = read( new_socket , buffer, 1024); 
	string SessionKey_A=buffer;
	cout<<"\n****From Client: Encrypted Session Key of Client: "<< SessionKey_A;

	int Session_Key_AB=TGS.request(SessionKey_A,B,timestamp);
vector<string> tickets=TGS.get_response(Session_Key_AB,SessionKey_A,B);
	cout<<"\nTicket of User A encrypted with session_key_of_ticket_generating_service_for_A : "<<tickets[0];
	cout<<"\nTicket of Real Server B encrypted with session_key_of_ticket_generating_service_for_B : "<<tickets[1];
// send 3
	std::string s = std::to_string(Session_Key_AB);
	char *s3 = new char[s.length() + 1];
	strcpy(s3, s.c_str());	
	send(new_socket , s3 , strlen(s3) , 0 ); 
cout<<"\nSending Session key of AB to Client";	


time_t now =get_currtime();
	//string timestamp = ctime(&now);
		cout<<"\n\n************************************";
	cout<<"\nAcknowledgement by Real Server by sending encrypted message to client";
	cout<<"\n************************************";
//get 5
memset(buffer,0,sizeof(buffer));
valread = read( new_socket , buffer, 1024); 
string encrypted_timestamp=buffer;
string msg=B.get_acknowledgement(tickets[1], encrypted_timestamp,now,Session_Key_AB);
	cout<<"\nAcknowledegment sent by B by incrementing timestamp: "<<ctime(&now);

// send 4
	string returntime = ctime(&now);
	char *s4 = new char[returntime.length() + 1];
	strcpy(s4, returntime.c_str());
	send(new_socket , s4 , strlen(s4) , 0 ); 
cout<<"\nSending incremented timestamp back to Client";
	
cout<<"\n\n-----------------------------";
cout<<"\nConnection established securely.";
cout<<"\n\n-----------------------------";
	
	
}

