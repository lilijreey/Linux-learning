getifaddr: getifaddr.c
	gcc -Wall -o $@ $^

unix: unixSvr unixCli

unixSvr: unix_sock_svr.c unixSocket.c
	gcc -Wall -o $@ $^ 

unixCli: unix_sock_cli.c unixSocket.c
	gcc -Wall -o $@ $^ 

clean:
	rm unixSvr unixClient
