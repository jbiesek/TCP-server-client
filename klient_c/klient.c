#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <netdb.h>
#include <unistd.h>

#define IP(H) *((unsigned long*) (H)->h_addr_list[0])

void logowanie(int gn){
	char loginpass[60];
	printf("Podaj login: ");
	scanf("%s", &loginpass);
	char *password = getpass("Podaj haslo: ");
	strcat(loginpass, " ");
	strcat(loginpass, password);
	send(gn, loginpass, sizeof(loginpass), 0);
}

int main(void)
{
    int gn;
    struct sockaddr_in adr;
    int port;
    struct hostent *h;
    char nazwa[512];
    char bufor[1025];
    char sciezka[512];
    long odebrano, odebrano_razem;
    int i;

    printf("Nazwa hosta / adres IP: ");
    scanf("%s", nazwa);
    h = gethostbyname(nazwa);
    if (h == NULL)
    {
        printf("Nieznany host\n");
        return 1;
    }
    printf("Numer portu: ");
    scanf("%d", &port);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = IP(h);
    printf("Lacze sie z %s:%d\n",
           inet_ntoa(adr.sin_addr),
           port);
    gn = socket(PF_INET, SOCK_STREAM, 0);
    if (connect(gn, (struct sockaddr*) &adr, sizeof(adr))<0)
    {
        printf("Nawiazanie polaczenia nie powiodlo sie\n");
        close(gn);
        return 1;
    }
    printf("Polaczenie nawiazane\n");
	logowanie(gn);
	memset(bufor, 0, 1025);
	recv(gn, bufor, sizeof(bufor), 0);
    printf("%s", bufor);
    if (strcmp(bufor, "Blad przy logowaniu!\n") == 0) exit(0);
    memset(bufor, 0, 1025);
    recv(gn, bufor, sizeof(bufor), 0);
    printf("%s", bufor);
    printf("Wybierz plik z listy: \n");
    int k;
    scanf("%d", &k);
    char str[3];
    memset(str, 0, 3);
    sprintf(str, "%d", k);
    char filename[50];
    memset(filename, 0, 50);
    char sstr[5];
    strcat(sstr, str);
    strcat(sstr, ".");
    strcat(sstr, " ");
    int j=0;
    char *wsk = strstr(bufor, sstr);
    for(i=0; i<1025;i++){
        if (bufor[i] == sstr[0] && bufor[i+1] == sstr[1] && bufor[i+2] == sstr[2] && k < 10 ||
        	bufor[i] == sstr[0] && bufor[i+1] == sstr[1] && bufor[i+2] == sstr[2] && bufor[i+3] == sstr[3] && k >= 10 && k <100){
            i+=3;
            while (bufor[i] != '\n'){
                filename[j] = bufor[i];
                i++;
                j++;
            }
        }
    }
    send(gn, str, sizeof(str), 0);
    char filsize[16];
    char fs[16];
    if (recv(gn, &filsize, sizeof(filsize), 0) != sizeof(filsize))
    {
        printf("Blad przy odbieraniu dlugosci\n");
        printf("Moze plik nie istnieje?\n");
        close(gn);
        return 1;
    }
    j = 0;
    for (i = 0; i < 16; i++){
    	if (filsize[i]!= '0' && filsize[i]!= '1' && filsize[i]!= '2' && filsize[i]!= '3' && filsize[i]!= '4' && filsize[i]!= '5' && filsize[i]!= '6' && filsize[i]!= '7' && filsize[i]!= '8' && filsize[i]!= '9' ){
    		continue;
    	} else {
    		fs[j] = filsize[i];
    		j++;
    	}
    }
    printf("Wybrano plik: %s\n", filename);
    printf("Dlugosc pliku: ");
    char fsize[j];
    for (i = 0; i < j; i++){
    	fsize[i] = fs[i];
    }
    int dl_pliku = atoi(fsize);
    printf("%d\n", dl_pliku);
    FILE *fp;
	fp = fopen(filename, "wb");
    odebrano_razem = 0;
    while (odebrano_razem < dl_pliku)
    {
        memset(bufor, 0, 1025);
        odebrano = recv(gn, bufor, 1024, 0);
        if (odebrano < 0)
            break;
        odebrano_razem += odebrano;
        int loading[10];
    	int dl = dl_pliku/10;
        fwrite(bufor, sizeof(char), odebrano, fp);
        if(odebrano_razem>dl && odebrano_razem<2*dl && loading[0]!=-1){
        	printf("[*_________] 10\% \n");
        	loading[0]=-1;
        }
        if(odebrano_razem>2*dl && odebrano_razem<3*dl && loading[1]!=-1){
        	printf("[**________] 20\% \n");
        	loading[1]=-1;
        }
        if(odebrano_razem>3*dl && odebrano_razem<4*dl && loading[2]!=-1){
        	printf("[***_______] 30\% \n");
        	loading[2]=-1;
        }
        if(odebrano_razem>4*dl && odebrano_razem<5*dl && loading[3]!=-1){
        	printf("[****______] 40\% \n");
        	loading[3]=-1;
        }
        if(odebrano_razem>5*dl && odebrano_razem<6*dl && loading[4]!=-1){
        	printf("[*****_____] 50\% \n");
        	loading[4]=-1;
        }
        if(odebrano_razem>6*dl && odebrano_razem<7*dl && loading[5]!=-1){
        	printf("[******____] 60\% \n");
        	loading[5]=-1;
        }
        if(odebrano_razem>7*dl && odebrano_razem<8*dl && loading[6]!=-1){
        	printf("[*******___] 70\% \n");
        	loading[6]=-1;
        }
        if(odebrano_razem>8*dl && odebrano_razem<9*dl && loading[7]!=-1){
        	printf("[********__] 80\% \n");
        	loading[7]=-1;
        }
        if(odebrano_razem>9*dl && odebrano_razem<10*dl && loading[8]!=-1){
        	printf("[*********_] 90\% \n");
        	loading[8]=-1;
        }
        
    }
    close(gn);
    if (odebrano_razem != dl_pliku)
        printf("Blad w odbiorze pliku!\n");
    else
        printf("[**********] 100\% \n");
        printf("Plik odebrany poprawnie!\n");
    return 0;
}
