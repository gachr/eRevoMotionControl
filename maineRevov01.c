#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

void error(const char *msg)
{
	perror(msg);
}

int main(int argc, char *argv[])
{
	int n;
	int muxval;
	int dutycycle;
	int periodfreq;
	int run;
	int rot;
	int speed;
	FILE * fdmux;
	FILE * fdrun;
	FILE * fdperiodfreq;
	FILE * fddutycycle; 
	FILE * frotdutycycle;
	FILE * fspeeddutycycle;
	struct termios termAttr;
	speed_t baudRate;
	char buffer[256];
	char varaux[50];

	fdmux = fopen("/sys/kernel/debug/omap_mux/gpmc_a2","w");
	if (fdmux!=NULL){
		printf("enter 1.0\n");
		muxval = 6;
		fprintf(fdmux,"%d",muxval);
		pclose(fdmux);

		printf("enter 1.1\n");
		
		sprintf(varaux,"/sys/class/pwm/ehrpwm.1:0/duty_ns");
		fddutycycle = fopen(varaux,"w");
		if (fddutycycle!=NULL){
			dutycycle = 0;
			fprintf(fddutycycle,"%d",dutycycle);
			pclose(fddutycycle);

			printf("enter 2.1\n");

			fdperiodfreq = fopen("/sys/class/pwm/ehrpwm.1:0/period_freq","w");
			if (fdperiodfreq != NULL){
				periodfreq = 50;
				fprintf(fdperiodfreq,"%d",periodfreq);
				pclose(fdperiodfreq);

				printf("enter 3.1\n");

				fddutycycle = fopen("/sys/class/pwm/ehrpwm.1:0/duty_ns","w");
				if (fddutycycle != NULL){
					dutycycle = 1500000;
					fprintf(fddutycycle,"%d",dutycycle);
					pclose(fddutycycle);

					printf("enter 4.1\n");

					fdrun = fopen("/sys/class/pwm/ehrpwm.1:0/run","w");
					if (fdrun != NULL){
						run = 1;
						fprintf(fdrun,"%d",run);
						pclose(fdrun);

						printf("enter 5.1\n");	
					}
				}
			
			}	

		}
	}

	fdmux = fopen("/sys/kernel/debug/omap_mux/gpmc_a3","w");
	if (fdmux!=NULL){
		printf("enter 1.0\n");
		muxval = 6;
		fprintf(fdmux,"%d",muxval);
		pclose(fdmux);

		printf("enter 1.2\n");
		
		sprintf(varaux,"/sys/class/pwm/ehrpwm.1:1/duty_ns");
		fddutycycle = fopen(varaux,"w");
		if (fddutycycle!=NULL){
			dutycycle = 0;
			fprintf(fddutycycle,"%d",dutycycle);
			pclose(fddutycycle);

			printf("enter 2.2\n");

			fdperiodfreq = fopen("/sys/class/pwm/ehrpwm.1:1/period_freq","w");
			if (fdperiodfreq != NULL){
				periodfreq = 50;
				fprintf(fdperiodfreq,"%d",periodfreq);
				pclose(fdperiodfreq);

				printf("enter 3.2\n");

				fddutycycle = fopen("/sys/class/pwm/ehrpwm.1:1/duty_ns","w");
				if (fddutycycle != NULL){
					dutycycle = 0;
					fprintf(fddutycycle,"%d",dutycycle);
					pclose(fddutycycle);

					printf("enter 4.2\n");

					fdrun = fopen("/sys/class/pwm/ehrpwm.1:1/run","w");
					if (fdrun != NULL){
						run = 1;
						fprintf(fdrun,"%d",run);
						pclose(fdrun);

						printf("enter 5.2\n");	
					}
				}
			
			}	

		}
	}


	
	while (1) {
		int closeSocket;
		int sockfd, newsockfd, portno;
		socklen_t clilen;
		struct sockaddr_in serv_addr, cli_addr;
		int true = 1;

        	sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) { 
			error("ERROR opening socket"); 
			exit(1);
		}
		if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1){
			perror("SetsockOpt");
			exit(1);
		}
		//bzero((char *) &serv_addr, sizeof(serv_addr));
		portno = 55000;
        	serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);
		bzero(&(serv_addr.sin_zero),8);
		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { 
			error("ERROR on binding"); 
			close(sockfd);
			exit(1);
		}
		listen(sockfd,1);
		clilen = sizeof(cli_addr);
		printf("Waiting for connections...\n");
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) { 
			error("ERROR on accept");
			close(sockfd);	
			exit(1);
		}
		printf("Connection established...\n");
		
		/*frotdutycycle = fopen("/sys/class/pwm/ehrpwm.1:0/duty_ns","w");
		if (frotdutycycle == NULL){
			closeSocket = 1;
		}
		fspeeddutycycle = fopen("/sys/class/pwm/ehrpwm.1:1/duty_ns","w");
		if (fspeeddutycycle == NULL){
			closeSocket = 1;
		}*/
		sleep(1);
		
		while (closeSocket == 0)
		{
			bzero(buffer,256);
			n = recv(newsockfd,buffer,2,MSG_DONTWAIT);
			if (n > 0) { 
				if (buffer[0] != 'f') {
					if (buffer[0] == '2'){
						rot = buffer[1]*10000 + 1000000;
						printf("rot = %d\n",rot);
						frotdutycycle = fopen("/sys/class/pwm/ehrpwm.1:0/duty_ns","w");
						if (frotdutycycle != NULL){
							fprintf(frotdutycycle,"%d",rot);
							printf("rot = %d\n",rot);
							pclose(frotdutycycle);
						}
					}
					if (buffer[0] == '1'){
						speed = buffer[1]*100000;
						printf("speed = %d\n",speed);
						fspeeddutycycle = fopen("/sys/class/pwm/ehrpwm.1:1/duty_ns","w");
						if (fspeeddutycycle != NULL){
							fprintf(fspeeddutycycle,"%d",speed);
							printf("speed = %d\n",speed);
							pclose(fspeeddutycycle);
						}
					}
				}
				else {
					printf("closing socket...\n");
					closeSocket = 1;
					rot = 1500000;
					speed = 0;
					frotdutycycle = fopen("/sys/class/pwm/ehrpwm.1:0/duty_ns","w");
					if (frotdutycycle != NULL) {
						fprintf(frotdutycycle,"%d",rot);
						pclose(frotdutycycle);
					}
					fspeeddutycycle = fopen("/sys/class/pwm/ehrpwm.1:1/duty_ns","w");
					if (fspeeddutycycle != NULL) {
						fprintf(fspeeddutycycle,"%d",speed);
						pclose(fspeeddutycycle);
					}
				}
			}
		}
		//pclose(frotdutycycle);
		//pclose(fspeeddutycycle);
		close(sockfd);
		close(newsockfd);
		printf("Connection closed...\n");
	}
	return 0;
}
