#include <unistd.h> 
#include "MTRcmd.h"   
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using boost::shared_ptr;

using namespace std;
/*
int main(int argc, char **argv)
{
	string host = argv[1];
	string cmd = argv[2];
	
	boost::shared_ptr<TSocket> socket(new TSocket(host, 3344));

	//	对接nonblockingServer时必须的，对普通server端时用boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	//boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));

	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	MTRcmdClient client(protocol);

	//设置发送、接收、连接超时
	socket->setConnTimeout(2000);
	socket->setRecvTimeout(2000);
	socket->setSendTimeout(2000);

	transport->open();

	//insert your code here 
	MTRcmdReq query;
	query.__set_cmd(cmd);
	
	MTRcmdRes res;

	client.mtrcmd(res, query);

	transport->close();

	if (res.status == 0) {
		fprintf(stdout, "%s\n", res.info.c_str());
	}
	return 0;
}
*/

int work(const string &host, const string &cmd)
{
	assert((false == host.empty()) && (false == cmd.empty()));
	//fprintf(stdout, "dest host: %s\tcmd: %s\n", host.c_str(), cmd.c_str());
	fprintf(stdout, "dest host: \033[47;31m%s\033[0m\tcmd: \033[47;31m%s\033[0m\n", host.c_str(), cmd.c_str());

	boost::shared_ptr<TSocket> socket(new TSocket(host, 3344));

	//对接nonblockingServer时必须的，对普通server端时用boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	//boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));

	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	MTRcmdClient client(protocol);

	//设置发送、接收、连接超时
	socket->setConnTimeout(1000*60*5);
	socket->setRecvTimeout(1000*60*5);
	socket->setSendTimeout(1000*60*5);

	transport->open();

	//insert your code here 
	MTRcmdReq query;
	query.__set_cmd(cmd);

	MTRcmdRes res;

	client.mtrcmd(res, query);

	transport->close();

	if (res.status == 0) {
		fprintf(stdout, "%s\n", res.info.c_str());
	}
	return 0;
}


int usage(char *exe)
{
	fprintf(stdout, "%s [option]\n", exe);
	fprintf(stdout, "\t-s\thostname\n");
	fprintf(stdout, "\t-f\thostlist file\n");
	fprintf(stdout, "\t-c\tcmd\n");
	fprintf(stdout, "\t-h\thelp\n");
	
	return 0;
}

int main(int argc, char **argv)
{
	bool bhost = false;
	bool bfile = false;
	string hostname;
	string hostfile;
	string cmd;	
	
	int ch;
	while ((ch = getopt(argc, argv, "hs:f:c:")) != -1) {
		switch (ch) {
			case 's':
				bhost = true;
				hostname = optarg;
				break;
			case 'f':
				bfile = true;
				hostfile = optarg;
				break;
			case 'c':
				cmd = optarg;
				break;
			case 'h':
			default:
				usage(argv[0]);
				exit(1);
		}
	}
	if ((bhost && bfile) || (!bhost && !bfile)) {
		fprintf(stderr, "para is not correct!\n");
		exit(1);
	}
	if (cmd.empty()) {
		fprintf(stderr, "cmd is null!\n");
		exit(1);
	}
	
	if (bhost) {
		work(hostname, cmd);
	} else if (bfile) {
		FILE *fp = fopen(hostfile.c_str(), "r");
		if (!fp) {
			fprintf(stderr, "open file %s fail!\n", hostfile.c_str());
			return -1;
		}
		char dest[256];
		while (fgets(dest, 256, fp) != NULL) {
			// 去除结尾换行符
			while (dest[strlen(dest) - 1] == '\n') {
				dest[strlen(dest) - 1] = '\0';
			}
			// 跳过空行
			if (dest[0] == '\0') {
				continue;
			}
			int len = 0;
			while (len < (strlen(dest))) {
				if (dest[len] != ' ' && dest[len] != '\t') {
					break;
				}
				len ++;
			}
			if (len == strlen(dest)) {
				continue;
			}
			
			work(dest, cmd);
		}
		fclose(fp);
	} else {
		return -1;
	}
	
	return 0;
}

