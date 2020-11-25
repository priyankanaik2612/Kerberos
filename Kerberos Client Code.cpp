

// Client side C/C++ program to demonstrate Socket programming 
#include<iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
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
class User: public Base_Encryption{
	string name;
	int userid;
	string plaintext;
	string session_key; //K-A_TGS
	int permanent_symmetric_key; //K-A_AS
	public:
		User(string name,int id)
		{
			this->name=name;
			this->userid=id;
		}
	string getname()
	{
		return name;
	}
	void set_plaintext(string p)
	{
		plaintext=p;
	}
		string get_plaintext()
	{
		return plaintext;
	}
	int get_permanent_symmetric_key()
	{
		return permanent_symmetric_key;
	}
	void generate_permanent_symmetric_key(int key)
	{
		permanent_symmetric_key=key;
	
	}
	string get_session_key()
	{
		return session_key;
	}
	void update_session_key(string k)
	{
		session_key=k;
	}
	string generate_session_key()
	{
		string str;
			for(int i=0;i<5;i++)
			{
				str+=rand()%26 + '0'+49;
			}
		this->session_key=str;
			return str;
		
	}

		int get_userid()
	{
		return userid;
	}

	vector<string> extract(string s)
	{
		vector<string> result;
		string a,b;
		int i;
		for(i=0;i<5;i++)
		{
			a+=s[i];
		}
		for(;i<10;i++)
		{
			b+=s[i];
		}
		cout<<"\n\nThe Process uses permanent symmetric key to decrypt message:";
		cout<<"\nEncrypted ticket and session_key: "<<a<<" "<<b;
		int p=this->permanent_symmetric_key;
		a=decrypt(a,p);
		b=decrypt(b,p);
		result.push_back(a);
		result.push_back(b);
		cout<<"\nTicket and session_key after decryption:"<<a<<b;
		return result;
	}

		string get_acknowledgement(string Ticket_B, string encrypted_timestamp,time_t &now,int Session_AB)
		{
			string ts=Base_Encryption::decrypt(encrypted_timestamp,Session_AB);
			ts=ts+"1";// B sends acknowledgment by incrementing timestamp by 1
			now=now+1;
			string result=encrypt(ts,Session_AB);
			return result;
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

Real_Server B;

int main()
{
	int sock = 0, valread; 
	B.set_name("Bob");
	struct sockaddr_in serv_addr; 
	char *hello = "Request from client"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		cout<<"\n Socket creation error \n"; 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		cout<<"\nInvalid address/ Address not supported \n"; 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		cout<<"\nConnection Failed \n"; 
		return -1; 
	} 
//send 1
	send(sock , hello , strlen(hello) , 0 ); 
	cout<<"Request sent to Auth Server\n"; 
//get 1
	valread = read( sock , buffer, 1024); 
	cout<<"****From Server: "<<buffer<<endl ; 

	fflush(stdin);
	srand(time(0)); 
	
	User A=User("Alice",1);

	

	cout<<"\n\n************************************";
	cout<<"\nVerification of User by Authentication Server, issue session key for client & Ticket for TGS";
	cout<<"\n************************************";
	cout<<"\nRequesting the Authentication Server";
	A.set_plaintext("Request to Real Server(Bob)");
//send 2

	std::string s = std::to_string(A.get_userid());
char *s2 = new char[s.length() + 1];
strcpy(s2, s.c_str());
//const char *s2=s.c_str();

	send(sock , s2, strlen(s2) , 0 ); 
	cout<<"\nRequest sent to Auth Server with user ID\n"; 
//send 3
	cout<<"\nEnter symmetric password(client): ";
	string pass;
	cin>>pass;
	char *s3 = new char[pass.length() + 1];
	strcpy(s3, pass.c_str());
	send(sock , s3, strlen(s3) , 0 ); 
	
//get 2 a

cout<<"\n****Permanent Symmetric key generated From AS";
memset(buffer,0,sizeof(buffer));
	valread = read( sock , buffer, 1024); 
	int pskey=atoi(buffer);
	A.generate_permanent_symmetric_key(pskey);
if(pskey==-1) 
{
cout<<"\nIncorrect password. Authentication not possible.";
exit(0);
}
//get 2 b
	memset(buffer,0,sizeof(buffer));
	valread = read( sock , buffer, 1024); 
	string AS_message=buffer;
	cout<<"\n****Permanent Symmetric key generated From AS";
	vector<string> Ticket_sessionkey=A.extract(AS_message); 
cout<<"\n****From Server: Ticket+Session key extracted From AS msg:"<<buffer;
	string sess_key=Ticket_sessionkey[0];
	A.update_session_key(sess_key);
	
cout<<"\n\n************************************";
cout<<"\nClient contacting Ticket Granting Server";
cout<<"\n************************************";
	time_t now =get_currtime();
	string timestamp = ctime(&now);
//send 4
	cout<<"\nSending foll. parameters to TGS to get Tickets of A & B: ";
	cout<<"\n1. Session Key of A: "<<Ticket_sessionkey[0];
	cout<<"\n2. Name of Real server: "<<B.getname();
	cout<<"\n3. Timestamp to prevent replay: "<<timestamp;
	cout<<"\n\nResponse from Ticket Granting Server: ";
	char *s4 = new char[timestamp.length() + 1];
	strcpy(s4, timestamp.c_str());
	send(sock , s4, strlen(s4) , 0 ); 
//sending timespan prevents another client from accessing
//send 6
char *s6 = new char[sess_key.length() + 1];
	strcpy(s6, sess_key.c_str());
	send(sock , s6, strlen(s4) , 0 ); 
 

cout<<"\n****From server: received Tickets of A & B, i.e. session key of AB encrypted with session key of TGS";
	
//get 3
	memset(buffer,0,sizeof(buffer));
	valread = read( sock , buffer, 1024); 
	int Session_Key_AB= atoi(buffer);
	cout<<"\n****From server: session key(AB) received:";


	cout<<"\n\n************************************";
	cout<<"\nContacting Real Server for Acknowledgement";
	cout<<"\n************************************";

cout<<"\nSending timestamp encrypted with session_key_AB to Real server";
	
//send 5 
	string encrypted_timestamp=encrypt(timestamp,Session_Key_AB);
	char *s5 = new char[encrypted_timestamp.length() + 1];
	strcpy(s5, encrypted_timestamp.c_str());
	send(sock , s5, strlen(s5) , 0 ); 

//get 4
	memset(buffer,0,sizeof(buffer));
	valread = read( sock , buffer, 1024); 
	string new_timestamp=buffer;
	cout<<"\n****From Server: Incremented timestamp: "<<new_timestamp;
	
	
cout<<"\n\n-----------------------------";
cout<<"\nConnection established securely.";
cout<<"\n\n-----------------------------";
	
}

