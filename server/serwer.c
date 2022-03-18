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

#define PORT htons(21212)

int logowanie(char* loginpass){
    char buf[60]="";
    FILE *fp;
    fp = fopen("users.txt", "r");
    strcat(loginpass, "\n");
    while (fgets(buf, sizeof(buf), fp)!=0)
    {
        if (strcmp(buf, loginpass)==0) return 1;
    }
    return 0;
}

void ObsluzPolaczenie(int gn, struct sockaddr_in adr)
{
    FILE *fp;
    fp = fopen("log.txt", "a+");
    char buf2[100];
    char *buf = "Polaczenie od :";
    fputs(buf,fp);
    char* inet = inet_ntoa(adr.sin_addr);
    fputs(inet,fp);
    fputs(":",fp);
    short unsigned int nto = ntohs(adr.sin_port);
    sprintf(buf2, "%d", nto);
    fputs(buf2, fp);
    char sciezka[512];
    long dl_pliku, wyslano, wyslano_razem, przeczytano;
    struct stat fileinfo;
    FILE* plik;
    unsigned char bufor[1024];
    memset(sciezka, 0, 512);
    DIR *dir;
    struct dirent *entry;
    int i = 1;
    int j;
    int l;
    char cwd[100];
    char message[1024];
    memset(message,0,1024);
    char str[3];
    char loginpass[60];
    recv(gn, loginpass, sizeof(loginpass), 0);
    char * p = strchr( loginpass, ' ' );
    int login_len = p-loginpass;
    char login[login_len+1];
    for (i=0; i<login_len; i++) {
    	login[i] = loginpass[i];
    }
    login[login_len] = '\0';
    i = 1;
    l = logowanie(loginpass);
    if (l == 1){
        char log[30] = "Pomyslnie zalogowano!\n";
        send(gn, log, sizeof(log), 0);
        fputs(" (",fp);
    	fputs(login, fp);
    	fputs(")\n",fp);
    } else {
        char log[30] = "Blad przy logowaniu!\n";
        send(gn, log, sizeof(log), 0);
        fputs(" (uzytkownik nie zalogowal sie)\n",fp);
        return;
    }
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/files");
    char info[1024] = "Dostepne do pobrania pliki:\n";
    if ((dir = opendir(cwd)) == NULL)
        perror("Error opendir() function");
    else
    {
        while ((entry = readdir(dir)) != NULL) {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            sprintf(str, "%d", i);
            strcat(message, str);
            strcat(message, ". ");
            strcat(message, entry->d_name);
            strcat(message, "\n");
            i++;
        }
        strcat(info, message);
        send(gn, info, sizeof(info), 0);
        closedir(dir);
    }
    recv(gn, str,sizeof(str), 0);
    fputs(login, fp);
    fputs(" chce plik nr ", fp);
    fputs(str,fp);
    fputs("\n",fp);
    sscanf(str, "%d", &j);
    i = 1;
    if ((dir = opendir(cwd)) == NULL)
        perror("Error opendir() function");
    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if(j == i) {
        	fputs(login, fp);
            fputs(" chce plik ",fp);
            fputs(entry->d_name, fp);
            fputs("\n", fp);
            strcat(sciezka, entry->d_name);
        }
        i++;
    }
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/files/");
    strcat(cwd, sciezka);
    memset(sciezka, 0, 512);
    strcat(sciezka, cwd);
    if (stat(sciezka, &fileinfo) < 0)
    {
        fputs("Nie moge pobrac informacji o pliku\n",fp);
        return;
    }

    if (fileinfo.st_size == 0)
    {
        fputs("Rozmiar pliku 0\n",fp);
        return;
    }
    char fsize [16];
    sprintf(fsize, "%ld", fileinfo.st_size);
    fputs("Dlugosc pliku: ",fp);
    fputs(fsize,fp);
    fputs("\n",fp);

    dl_pliku = htonl((long) fileinfo.st_size);

    if (send(gn, &fsize, sizeof(fsize), 0) != sizeof(fsize))
    {
        fputs("Blad przy wysylaniu wielkosci pliku\n",fp);
        return;
    }

    dl_pliku = fileinfo.st_size;
    wyslano_razem = 0;
    
    plik = fopen(sciezka, "rb");
    if (plik == NULL)
    {
        fputs("Blad przy otwarciu pliku\n",fp);
        return;
    }
    while (wyslano_razem < dl_pliku)
    {
        przeczytano = fread(bufor, 1, 1024, plik);
        wyslano = send(gn, bufor, przeczytano, 0);
        if (przeczytano != wyslano)
            break;
        wyslano_razem += wyslano;
    }
    if (wyslano_razem == dl_pliku) {
        fputs("Plik wyslany poprawnie\n",fp);
    }
    else
        fputs("Blad przy wysylaniu pliku\n",fp);
    fclose(plik);
    exit(0);
    return;
}

void calculator(){
    int choice;
    do {
        printf("1 - dodawanie\n2 - exit\n");
        scanf("%d", &choice);
        if (choice == 1) {
            int a;
            int b;
            printf("Podaj pierwsza liczbe: ");
            scanf("%d", &a);
            printf("Podaj druga liczbe: ");
            scanf("%d", &b);
            printf("Wynik: %d\n", a+b);
        }
    } while (choice != 2);
    return;
}

int menu() {
    int choice = 0;
    char buf[100];
    while (choice != 4){
        printf("1 - pokaz logi\n2 - czysc logi\n3 - kalkulator\n4 - exit\n");
        scanf("%d", &choice);
        if (choice == 1) {
            FILE *fp;
            fp = fopen("log.txt", "r");
            while (fgets(buf, sizeof(buf), fp)!=0)
            {
                printf("%s", buf);
            }
        }
        if (choice == 2) {
            FILE *fp;
            fp = fopen("log.txt", "w");
            fputs("", fp);
        }
        if (choice == 3){
            calculator();
        }
    }
    return 1;
}



void con(){
    FILE *fp;
    fp = fopen("log.txt", "a+");
    int gn_nasluch, gn_klienta;
    struct sockaddr_in adr;
    socklen_t dladr = sizeof(struct sockaddr_in);
    int i;
    char buf2[100];
    gn_nasluch = socket(PF_INET, SOCK_STREAM, 0);
    adr.sin_family = AF_INET;
    adr.sin_port = PORT;
    adr.sin_addr.s_addr = INADDR_ANY;
    memset(adr.sin_zero, 0, sizeof(adr.sin_zero));

    if (bind(gn_nasluch, (struct sockaddr*) &adr, dladr) < 0)
    {
        printf("Bind nie powiodl sie\n");
        exit(0);
    }

    listen(gn_nasluch, 10);

    while(1)
    {
        dladr = sizeof(struct sockaddr_in);
        gn_klienta = accept(gn_nasluch, (struct sockaddr*) &adr, &dladr);
        if (gn_klienta < 0)
        {
            fputs("Accept zwrocil blad\n",fp);
            continue;
        }
        if (fork() == 0)
        {
            /* proces potomny */

            ObsluzPolaczenie(gn_klienta, adr);

            close(gn_klienta);

            exit(0);
        }
        else
        {
            /* proces macierzysty */
            continue;
        }
    }
}

int main(void)
{
	int main_count = 0;
	int fork_count = 0;
	pid_t x = fork();
	int status;
    while (1) {
        if (x == 0 && fork_count == 0) {
            /* proces potomny */
			fork_count++;
            con();
        } else {
        	if (main_count == 0) {
        		main_count++;
            	status = menu();
            	if (status == 1) exit(0);
            }
        }
    }
    return 0;
}
