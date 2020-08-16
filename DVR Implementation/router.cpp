#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define INF 99999

using namespace std;
int sockfd;
int countClock=0;
struct adjacentEdge
{
	string adjacent;
    	int cost;
	
};

struct routingTable
{
	string dest;
    	string nextHop;
    	int cost;
};

vector<adjacentEdge>Edge;
set<string>ipAddresses;
vector<string>adjacentNode;
vector<routingTable>table;
bool flagChange=false;

string destination="192.168.0.";

bool checkAdjacent(string ip)
{
	for(int i=0;i<Edge.size();i++)
	{
		if(!ip.compare(Edge[i].adjacent))
			return true;
	}
	return false;
}

void printTable(string ipAddress)
{
	cout<<"Routing Table for "<<ipAddress<<" : "<<endl;
	cout<<"Destination  \tNext Hop \tCost"<<endl;
	cout<<endl;
  
	for(int i=0;i<table.size();i++)
	{
		if(!table[i].dest.compare(ipAddress)) continue;
            	cout<<table[i].dest<<"\t"<<table[i].nextHop<<"\t"<<table[i].cost<<endl;
    	}
    	cout<<endl;
}

void updatedTable(string ownip,string ip,vector<routingTable>rt)
{
	cout<<"enter"<<endl;
	int tempCost;
	/*for(int i=0;i<rt.size();i++){
		cout<<rt[i].dest<<" "<<rt[i].cost<<" "<<rt[i].nextHop<<endl;
	}*/
	/*for(int i=0;i<ipAddresses.size();i++)
    	{
    		for(int j=0;j<Edge.size();j++)
        	{
        		if(!ip.compare(Edge[j].adjacent))
        	    	{
		    		tempCost=Edge[j].cost+rt[i].cost;
        	        	if(!ip.compare(table[i].nextHop)||(tempCost<table[i].cost && ownip.compare(rt[i].nextHop)!=0))
                		{
					if(table[i].cost!=tempCost)
					{
						table[i].nextHop=ip;
						table[i].cost=tempCost;
                    				flagChange=true;
					}
					//break;
                		}
            		}
        	}
    	}*/
	//cout<<ip<<endl;
	for(int i=0;i<rt.size();i++){
			
	}
	
    	if(flagChange==true){
		cout<<"After updating"<<endl;
        	printTable(ownip);
	}
    	flagChange=false;
}

int findAdjacent(string ipAddress, string fileName){
	

	string routerOne, routerTwo;
    	int distance;
	ifstream getFileValue(fileName.c_str());
    	//cout<<ipAddress<endl;
	
	while(!getFileValue.eof()){
        	
		getFileValue>>routerOne>>routerTwo>>distance;
		ipAddresses.insert(routerOne);
        	ipAddresses.insert(routerTwo);
		struct adjacentEdge edge;
		
		//cout<<"Value"<<endl;
		//cout<<routerOne<<endl;
		//cout<<routerTwo<<endl;
		//cout<<distance<<endl;
		if(!routerOne.compare(ipAddress)){
			
			if(!checkAdjacent(routerTwo)){
            			
				adjacentNode.push_back(routerTwo);
            			edge.adjacent=routerTwo;
            			edge.cost=distance;
				//e.status = 1;
				//e.recvClock = 0;
            			Edge.push_back(edge);
			}
        	}
        	
		else if(!routerTwo.compare(ipAddress))
	        {
			if(!checkAdjacent(routerOne)){
		        
				adjacentNode.push_back(routerOne);
			        edge.adjacent=routerOne;
			        edge.cost=distance;
				//e.status = 1;
				//e.recvClock = 0;
			        Edge.push_back(edge);
			}
        	}

		
	}
	getFileValue.close();
	//for(int i=0;i<adjacentNode.size();i++)
	//	cout<<adjacentNode[i]<<endl;

}

void updateTableCost(string ip,string adj,int prevCost,int newCost){
	
	for(int i=0;i<ipAddresses.size();i++){
		if(!adj.compare(table[i].nextHop)){
			
			if(!adj.compare(table[i].dest)){
				table[i].cost=newCost;	
				//cout<<"found1"<<endl;		
			}
			else{
				table[i].cost=table[i].cost-prevCost+newCost;	
				//cout<<"found2"<<endl;		
			}
		}
		else{
			if(!adj.compare(table[i].dest)){
				if(table[i].cost>newCost){
					table[i].cost=newCost;
					table[i].nextHop=adj;	
					//cout<<"found3"<<endl;			
				}			
			}		
	
		}
		//cout<<"print"<<endl;
	}
	cout<<"After chaning cost :"<<endl;
	printTable(ip);
}

void sendTabletoAdjacents(string ip){

	string datagram="fooo"+ip;
    	for(int i=0;i<table.size();i++){

        	datagram=datagram+":"+table[i].dest+"-"+table[i].nextHop+"-"+to_string(table[i].cost);
    	}
	for(int i=0;i<adjacentNode.size();i++){
	        struct sockaddr_in otherNetwork;
		otherNetwork.sin_family = AF_INET;
        	otherNetwork.sin_port = htons(4747);
        	inet_pton(AF_INET,adjacentNode[i].c_str(),&otherNetwork.sin_addr);
		int sent_bytes = sendto(sockfd,datagram.c_str(), 1024, 0, (struct sockaddr*) &otherNetwork, sizeof(sockaddr_in));
		
	}

}

struct routingTable getRouterTable(string value,string delimiter)
{
	char *tokn=new char[value.length()+1];
	strcpy(tokn,value.c_str());
	//int i=0;
	//cout<<tokn<<endl;
	struct routingTable rt;
	vector<string>temp;
	char *tok=strtok(tokn,delimiter.c_str());
	/*while(tok!=NULL)
	{
		cout<<tok[i]<<endl;
		i++;
	}*/

	while(tok!=NULL)
	{
		temp.push_back(tok);
		tok=strtok(NULL,delimiter.c_str());
	}

	rt.dest=temp[0];
	//cout<<rte.dest<<endl;
	rt.nextHop=temp[1];
	//cout<<rte.nextHop<<endl;
	rt.cost=atoi(temp[2].c_str());
	//cout<<rte.cost<<endl;
	temp.clear();
	return rt;
}

vector<routingTable>fromPacketToTableConvert(string pckt)
{
	char *str=new char[pckt.length()+1];
	strcpy(str,pckt.c_str());
    	char *token=strtok(str,":");
	vector<routingTable>tbList;
    	vector<string>wholeTable;
	struct routingTable tempTable;
    	while(token!=NULL)
    	{
		string c(token);
        	wholeTable.push_back(c);
		token=strtok(NULL,":");
	}
	
	for(int i=0;i<wholeTable.size();i++)
	{
		tempTable=getRouterTable(wholeTable[i],"-");
		tbList.push_back(tempTable);
		//cout<<"dest : "<<rte.destination<<"  next : "<<rte.nextHop<<"  cost : "<<rte.cost<<endl;
	}
    return tbList;
}

void sendMessage(string ip,string msgSize,string message,string destinationIP){
	
	struct sockaddr_in otherNetwork;
	string instruction="frwd-"+destinationIP+"-"+msgSize+"-"+message;
	string nextStoppage;
	
	for(int i=0;i<ipAddresses.size();i++){
		if(!destinationIP.compare(table[i].dest))
		{
			nextStoppage=table[i].nextHop;
			break;
		}	
	}

	otherNetwork.sin_family = AF_INET;
    	otherNetwork.sin_port = htons(4747);
    	inet_pton(AF_INET,nextStoppage.c_str(),&otherNetwork.sin_addr);

    	int sent_bytes=sendto(sockfd,instruction.c_str(),1024,0,(struct sockaddr*) &otherNetwork,sizeof(sockaddr_in));
	//cout<<sent_bytes<<endl;
	cout<<message<<" packet forwarded to "<<nextStoppage.c_str()<<" (printed by "<<ip<<")\n";
}

int makeRoutingTable(string ipAddress){

	struct routingTable dvrRoute;
	//cout<<"Making Ip"<<endl;
	string destTemp;
	for(int i=0;i<ipAddresses.size();i++){
		
		char x=i+1+'0';
        	destTemp=destination+x;
		//cout<<temp<<endl;

		if(!ipAddress.compare(destTemp)){
			
			dvrRoute.dest=destTemp;
           		dvrRoute.nextHop=destTemp;
            		dvrRoute.cost=0;		
		}
		else if(find(adjacentNode.begin(),adjacentNode.end(),destTemp)!=adjacentNode.end()){
			
			for(int j=0;j<Edge.size();j++){
                		if(!Edge[j].adjacent.compare(destTemp))
                		{
                    			dvrRoute.dest=destTemp;
                    			dvrRoute.nextHop=destTemp;
                    			dvrRoute.cost=Edge[j].cost;
					continue;
                		}
            		}		
		}
		else
        	{
            		dvrRoute.dest=destTemp;
            		dvrRoute.nextHop="\t-";
            		dvrRoute.cost=INF;
        	}
		table.push_back(dvrRoute);
	}
	printTable(ipAddress);

}



void getCommandFromDriver(string ipAddress){
	
	struct sockaddr_in networkaddress;
   	socklen_t addrlen;
	string commandType;
	int bytes_received;
	struct sockaddr_in client_address;
	char buffer[1024];
	int flag=0;
	//sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	//bind(sockfd, (struct sockaddr*) &client_address, sizeof(sockaddr_in));
	while(true){
		//char buffer[1024];
		bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &networkaddress, &addrlen);


			string command(buffer);
			//cout<<bytes_received<<endl;
			//string command=(string)buffer;	
			commandType=command.substr(0,4);
			//cout<<commandType<<endl;
			//cout<<(int)command[5]<<endl;
			//cout<<buffer[5]<<endl;l
			//printf("%s\n",buffer);
			/*for(int i = 4 ; i < bytes_received ; i++){
				cout << "Ba: " << endl ;
				int segment=(int)((unsigned char)buffer[i]);
				cout<<segment;
			}*/
			if(!command.substr(0,3).compare("clk")){
				//cout<<"dhuksi"<<endl;
				
				countClock++;
				if(countClock%3==0){
					//cout<<"time to update"<<endl;
					//flag==1;
					//countClock--;
					sendTabletoAdjacents(ipAddress);
				}		
			}

			else if(!commandType.compare("fooo")){
				//cout<<"paisi"<<endl;
				string b(buffer);
				string ip=b.substr(4,12);
				//cout<<b.substr(4,12)<<endl;
				int len=b.length()-15;
				char tablepacket[len];
                		for (int i=0;i<len;i++){
                    			tablepacket[i]=buffer[16+i];
                		}
				string packet(tablepacket);
				//cout<<packet<<endl;
				fromPacketToTableConvert(packet);
				vector<routingTable>tbl=fromPacketToTableConvert(packet);
                		updatedTable(ipAddress,ip,tbl);
				cout<<"Done"<<endl;
				
			}

			else if(!commandType.compare("show"))
			{
				string ipAddressOne="";
				printTable(ipAddress);
				for(int i = 4 ; i < bytes_received ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					//cout<<segment<<endl;
					if(i<7) ipAddressOne=ipAddressOne+to_string(segment)+".";
					else ipAddressOne=ipAddressOne+to_string(segment);
					
				}
				cout<<ipAddressOne<<endl;
			}
			else if(!commandType.compare("cost")){

				string ipAddressOne="";
				string ipAddressTwo="";
				string newCost="";
				unsigned char x;
				int temp;
				
				for(int i = 4 ; i < 8 ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					//cout<<segment<<endl;
					if(i<7) ipAddressOne=ipAddressOne+to_string(segment)+".";
					else ipAddressOne=ipAddressOne+to_string(segment);
					
				}
				for(int i = 8 ; i < 12 ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					//cout<<segment<<endl;
					if(i<11) ipAddressTwo=ipAddressTwo+to_string(segment)+".";
					else ipAddressTwo=ipAddressTwo+to_string(segment);
					
				}
				for(int i = 12 ; i < 14 ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					if(i==13 && segment==1){
						segment=256+temp;
						newCost=to_string(segment);
					}
					else if(i==13 && segment==0) continue;
					else{
						temp=segment;
					//cout<<segment<<endl;
						
						newCost=newCost+to_string(segment);
					}
					//else ipAddressTwo=ipAddressTwo+to_string(segment);
					
				}
				int number=stoi(newCost);
				//printf("%d\n",number);
				
				//cout<<ipAddressOne<<endl;
				//cout<<ipAddressTwo<<endl;	
				//cout<<newCost<<endl;
				int prevNumber;
				string adjacent;
				
				for(int i=0;i<Edge.size();i++){
					if(!ipAddressOne.compare(Edge[i].adjacent)){
						prevNumber=Edge[i].cost;
						Edge[i].cost=number;	
						adjacent=ipAddressOne;			
					}
					else if(!ipAddressTwo.compare(Edge[i].adjacent)){
						prevNumber=Edge[i].cost;
						Edge[i].cost=number;	
						adjacent=ipAddressTwo;					
					}	
								
				}
				updateTableCost(ipAddress,adjacent,prevNumber,number);
					

						
			}
			else if(!commandType.compare("send")){
				string ipAddressOne="";
				string ipAddressTwo="";
				string newCost="";
				unsigned char x;
				int temp;
				
				for(int i = 4 ; i < 8 ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					//cout<<segment<<endl;
					if(i<7) ipAddressOne=ipAddressOne+to_string(segment)+".";
					else ipAddressOne=ipAddressOne+to_string(segment);
					
				}
				for(int i = 8 ; i < 12 ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					//cout<<segment<<endl;
					if(i<11) ipAddressTwo=ipAddressTwo+to_string(segment)+".";
					else ipAddressTwo=ipAddressTwo+to_string(segment);
					
				}
				for(int i = 12 ; i < 14 ; i++){
				//cout << "Ba: " << endl ;
					int segment=(int)((unsigned char)buffer[i]);
					if(i==13 && segment==1){
						segment=256+temp;
						newCost=to_string(segment);
					}
					else if(i==13 && segment==0) continue;
					else{
						temp=segment;
					//cout<<segment<<endl;
						
						newCost=newCost+to_string(segment);
					}
					//else ipAddressTwo=ipAddressTwo+to_string(segment);
					
				}
				int number=stoi(newCost);
				char msg[number+1];
				for (int i=0;i<number;i++){
                   			msg[i]=buffer[14+i];
                		}
				msg[number]='\0';
				string message(msg);
				//cout<<message<<endl;
				sendMessage(ipAddress,newCost,message,ipAddressTwo);		
			}	
			else if(!commandType.compare("frwd")){
				//char *m=new char[bytes_received+ 1];
				//cout<<to_string(buffer)<<endl;
				string b(buffer);
				//cout<<b<<endl;
				//cout<<buffer.substr(0,6);
				/*for(int i=0;i<bytes_received;i++){
					m[i]=buffer[i];				
				}*/
				string ip="";
				int index;
				string len="";
				string msg="";
				for(int i=5;b[i]!='-';i++){
					ip=ip+b[i];
					index=i;
				}
				//cout<<ip<<endl;
				for(int i=index+2;b[i]!='-';i++){
					len=len+b[i];
					index=i;
				}
				for(int i=index+2;i<index+2+stoi(len);i++){
					msg=msg+b[i];
					//index=i;
				}
				//cout<<ip<<endl;
				//cout<<len<<endl;
				//cout<<msg<<endl;
				//string ip(token[1]);
				//cout<<ip<<endl;
				//cout<<to_string(token[0])<<endl;
				//cout<<to_string(token[1])<<endl;
				//cout<<to_string(token[2])<<endl;
				//cout<<to_string(token[3])<<endl;*/
				if(!ip.compare(ipAddress))
				{
					cout<<msg<<" packet reached destination (printed by "<<ip<<")\n";
				}
				else
					sendMessage(ipAddress,len,msg,ip);
				
							
			}		
		}
	
}


int main(int argc, char *argv[]) {

	if (argc != 3) {
	        cout<<"router : "<<argv[1]<<"<ip address>\n";
	        exit(1);
	}

	//cout<<argv[0]<<endl;
	//cout<<argv[1]<<endl;
	//cout<<argv[2]<<endl;
	//int sockfd;
	int bind_flag;
	int sent_bytes;
	struct sockaddr_in client_address;

	client_address.sin_family = AF_INET;
    	client_address.sin_port = htons(4747);
	client_address.sin_addr.s_addr = inet_addr(argv[1]);
	//inet_pton(AF_INET, argv[1], &client_address.sin_addr);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind_flag = bind(sockfd, (struct sockaddr*) &client_address, sizeof(sockaddr_in));
	//printf("%d\n", bind_flag);
	if(bind_flag==0)printf("successful bind\n");
	else cout<<"Unsuccessful bind"<<endl;

	findAdjacent(argv[1],argv[2]);
	makeRoutingTable(argv[1]);
	getCommandFromDriver(argv[1]);
}
