#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>

//cd "c:\Users\Admin\Desktop\Adamovic_PKS1\" ; if ($?) { gcc main.c -lwpcap -o main } ; if ($?) { .\main }

//struktura obsahujuca informacie z externych suborov
typedef struct externy{
    unsigned short value;
    char name[50];
    struct externy* dalsi;
} EXTERNY;

//struktura pocitajuca, kolkokrat sa vyskytla IP adresa medzi destination
typedef struct destination_adresa{
    unsigned char adresa[4];
    int vyskyt;
    struct destination_adresa* dalsi;
} DESTINATION_ADRESA;

typedef struct komunikacie{
    unsigned char adresa1[4];
    unsigned char adresa2[4];

    //podla indexov(value 0-1) 0, 1 = syn, ack     2, 3 = syn, ack
    //podla indexov(value 2-3) 0, 1 = fin, ack     2, 3 = fin, ack
    unsigned char stav[4];

    char uzavreta;
    short zoznam_ramcov[5000];
    int pocet_ramcov;

    struct komunikacie* dalsi;
} KOMUNIKACIE;

int tftp_port = -1;


//funkcia sluziaca na nacitanie pcap suboru
pcap_t *nacitaj_PCAP_file(char *name, char *buffer){
    pcap_t *subor = pcap_open_offline(name, buffer);
    if(subor == NULL){
        printf("Subor sa nepodarilo nacitat z nasledujuceho dovodu:\n");
        int i = 0;
        while(buffer[i] != 0)
            printf("%c", buffer[i++]);
        printf("\n");
        return 0;
    }
    else{
        //printf("Subor sa podarilo otvorit\n");
    }
    return subor;
}

void vypis_adresy(FILE *vystupny_subor, const u_char *data_ramca){
    fprintf(vystupny_subor, "MAC adresa prijimatela je:  %.2X %.2X %.2X %.2X %.2X %.2X\n", data_ramca[0], data_ramca[1], data_ramca[2], data_ramca[3], data_ramca[4], data_ramca[5]);
    fprintf(vystupny_subor, "MAC adresa odosielatela je: %.2X %.2X %.2X %.2X %.2X %.2X\n", data_ramca[6], data_ramca[7], data_ramca[8], data_ramca[9], data_ramca[10], data_ramca[11]);
}

void *flags(int x, char *vystup){
    if(x / 128 > 0)
        x -= 128;
    if(x / 64 > 0)
        x -= 64;
    if(x / 32 > 0)
        x -= 32;
    if(x / 16 > 0){
        vystup[0] = 1;
        x -= 16;
    }
    if(x / 8 > 0){
        vystup[1] = 1;
        x -= 8;
    }
    if(x / 4 > 0){
        vystup[2] = 1;
        x -= 4;
    }
    if(x / 2 > 0){
        vystup[3] = 1;
        x -= 2;
    }
    if(x / 1 > 0){
        vystup[4] = 1;
        x -= 1;
    }
}

KOMUNIKACIE *najdi_komunikaciu(KOMUNIKACIE *start, unsigned char *adresa1, unsigned char *adresa2, char *flagy){
    while(start != NULL){
        if((start->adresa1[0] == adresa1[0] && start->adresa1[1] == adresa1[1] && start->adresa1[2] == adresa1[2] && start->adresa1[3] == adresa1[3] &&
        start->adresa2[0] == adresa2[0] && start->adresa2[1] == adresa2[1] && start->adresa2[2] == adresa2[2] && start->adresa2[3] == adresa2[3]) ||
        (start->adresa1[0] == adresa2[0] && start->adresa1[1] == adresa2[1] && start->adresa1[2] == adresa2[2] && start->adresa1[3] == adresa2[3] &&
        start->adresa2[0] == adresa1[0] && start->adresa2[1] == adresa1[1] && start->adresa2[2] == adresa1[2] && start->adresa2[3] == adresa1[3])){
            if(start->uzavreta == 1 ||
            (start->stav[0] == 2 && start->stav[1] == 1 && start->stav[2] == 2 && start->stav[3] == 2 && *(int *)adresa1 == *(int *)start->adresa2) ||
            (start->stav[0] == 2 && start->stav[1] == 2 && start->stav[2] == 2 && start->stav[3] == 1 && *(int *)adresa1 == *(int *)start->adresa1) ||
            (start->stav[0] > 0 && flagy[3] && *(int *)adresa1 == *(int *)start->adresa1) ||
            (start->stav[2] > 0 && flagy[3] && *(int *)adresa1 == *(int *)start->adresa2)){
                //ak je uz uzavreta, tak hladam dalej
                start = start->dalsi;
                continue;
            }
            return start;
        }
        start = start->dalsi;
    }
    return start;
}

void vytvor_komunikaciu(KOMUNIKACIE *zaciatok_komunikacii, unsigned char *adresa1, unsigned char *adresa2, int cislo_ramca){
    KOMUNIKACIE *nova = (KOMUNIKACIE *)malloc(sizeof(KOMUNIKACIE));
    nova->dalsi = NULL;
    nova->uzavreta = 0;

    nova->stav[0] = 1;
    nova->stav[1] = 0;
    nova->stav[2] = 0;
    nova->stav[3] = 0;

    nova->adresa1[0] = adresa1[0];
    nova->adresa1[1] = adresa1[1];
    nova->adresa1[2] = adresa1[2];
    nova->adresa1[3] = adresa1[3];

    nova->adresa2[0] = adresa2[0];
    nova->adresa2[1] = adresa2[1];
    nova->adresa2[2] = adresa2[2];
    nova->adresa2[3] = adresa2[3];

    nova->pocet_ramcov = 1;
    nova->zoznam_ramcov[0] = (short)cislo_ramca;

    while(zaciatok_komunikacii->dalsi != NULL)
        zaciatok_komunikacii = zaciatok_komunikacii->dalsi;

    zaciatok_komunikacii->dalsi = nova;
}

void komunikacia_tcp(const u_char *data_ramca, const u_char *vnorene_data_ramca, FILE *vystupny_subor, int cislo_ramca, char *vnoreny_v_tcp, int velkost, KOMUNIKACIE *zaciatok_komunikacii){
    int source = vnorene_data_ramca[0] * 256 + vnorene_data_ramca[1], destination = vnorene_data_ramca[2] * 256 + vnorene_data_ramca[3];
    KOMUNIKACIE *akt;
    unsigned char adresa1[4] = {data_ramca[26], data_ramca[27], data_ramca[28], data_ramca[29]}, adresa2[4] = {data_ramca[30], data_ramca[31], data_ramca[32], data_ramca[33]};
    char flagy[5] = {0};
    flags(vnorene_data_ramca[13], flagy);

    //skusim najst komunikaciu v zozname (este neukoncenu)
    akt = najdi_komunikaciu(zaciatok_komunikacii, adresa1, adresa2, flagy);

    //ak som nasiel, tak upravim strukturu, ak bude co upravovat
    if(akt){
        //printf("cisielko %d\n", cislo_ramca);
        akt->zoznam_ramcov[akt->pocet_ramcov++] = cislo_ramca;

        //nie je reset?
        if(flagy[2] == 1){
            akt->stav[0] = 2;
            akt->stav[1] = 2;
            akt->stav[2] = 2;
            akt->stav[3] = 2;
            akt->uzavreta = 1;
            //printf("Zatvaram komunikaciu, kvoli resetu, zacinajucu na porte %d a konciacu na porte %d\n", akt->zoznam_ramcov[0], cislo_ramca);
        }

        //syn ack combo
        else if(akt->stav[0] == 1 && akt->stav[1] == 0 && akt->stav[2] == 0 && akt->stav[3] == 0 && flagy[0] == 1 && flagy[3] == 1 && *(int *)adresa1 == *(int *)akt->adresa2){
            akt->stav[2] = 1;
            akt->stav[3] = 1;
            //printf("Vypis 1 = som na ramci %d\n", cislo_ramca);
        }
        //ack solo
        else if(akt->stav[0] == 1 && akt->stav[1] == 0 && akt->stav[2] == 1 && akt->stav[3] == 1 && flagy[0] == 1 && *(int *)adresa1 == *(int *)akt->adresa1){
            akt->stav[1] = 1;
            //printf("Vypis 2 = som na ramci %d\n", cislo_ramca);
        }

        //zacnem zatvaranie, prve fin
        else if(akt->stav[0] == 1 && akt->stav[1] == 1 && akt->stav[2] == 1 && akt->stav[3] == 1 && flagy[4] == 1){
            if(*(int *)adresa1 == *(int *)akt->adresa1)
                akt->stav[0] = 2;
            else if(*(int *)adresa1 == *(int *)akt->adresa2)
                akt->stav[2] = 2;
            //printf("Vypis 3 = som na ramci %d info: %d %d %d %d\n", cislo_ramca, akt->stav[0], akt->stav[1], akt->stav[2], akt->stav[3]);
        }
        //pokracujem v zatvarani, druhe fin ack combo, part 1
        else if(akt->stav[0] == 2 && akt->stav[1] == 1 && akt->stav[2] == 1 && akt->stav[3] == 1 && flagy[0] == 1 && flagy[4] == 1 && *(int *)adresa1 == *(int *)akt->adresa2){
            akt->stav[2] = 2;
            akt->stav[3] = 2;
            //printf("Vypis 4_1 = som na ramci %d\n", cislo_ramca);
        }
        //part 2
        else if(akt->stav[0] == 1 && akt->stav[1] == 1 && akt->stav[2] == 2 && akt->stav[3] == 1 && flagy[0] == 1 && flagy[4] == 1 && *(int *)adresa1 == *(int *)akt->adresa1){
            akt->stav[0] = 2;
            akt->stav[1] = 2;
            //printf("Vypis 4_2 = som na ramci %d\n", cislo_ramca);
        }
        //ukoncujem zatvaranie, solo ack, part 1
        else if(akt->stav[0] == 2 && akt->stav[1] == 2 && akt->stav[2] == 2 && akt->stav[3] == 1 && flagy[0] == 1 && flagy[3] == 0 && *(int *)adresa1 == *(int *)akt->adresa2){
            akt->stav[3] = 2;
            akt->uzavreta = 1;
            /*
            printf("Zatvaram komunikaciu zacinajucu na porte %d a konciacu na porte %d, obsahuje ramce:\n", akt->zoznam_ramcov[0], cislo_ramca);
            for(int i = 0; i < akt->pocet_ramcov; i++)
                printf("%d ", akt->zoznam_ramcov[i]);
            printf("\n");
            */
        }
        //ukoncujem zatvaranie, solo ack, part 2
        else if(akt->stav[0] == 2 && akt->stav[1] == 1 && akt->stav[2] == 2 && akt->stav[3] == 2 && flagy[0] == 1 && flagy[3] == 0 && *(int *)adresa1 == *(int *)akt->adresa1){
            akt->stav[1] = 2;
            akt->uzavreta = 1;
            /*
            printf("Zatvaram komunikaciu zacinajucu na porte %d a konciacu na porte %d, obsahuje ramce:\n", akt->zoznam_ramcov[0], cislo_ramca);
            for(int i = 0; i < akt->pocet_ramcov; i++)
                printf("%d ", akt->zoznam_ramcov[i]);
            printf("\n");
            */
        }
        
    }
    //ak som nenasiel, tak vytvorim novu a pridam ju na koniec zoznamu
    else if(flagy[3] == 1){
        //printf("Vytvaram komunikaciu, ramec %d\n", cislo_ramca);
        vytvor_komunikaciu(zaciatok_komunikacii, adresa1, adresa2, cislo_ramca);
    }
    else{
        //printf("prebieha nezacata komunikacia, cislo ramca %d\n", cislo_ramca);
    }
}

void ries_udp(const u_char *data_ramca, const u_char *vnorene_data_ramca, FILE *vystupny_subor, EXTERNY *udp, char filter, int cislo_ramca, int velkost){
    int source = vnorene_data_ramca[0] * 256 + vnorene_data_ramca[1], destination = vnorene_data_ramca[2] * 256 + vnorene_data_ramca[3];

    while(udp != NULL){
        if(source == udp->value || destination == udp->value)
            break;
        udp = udp->dalsi;
    }

    if(filter == 7){
        if(tftp_port == -1 && destination == 69){
            tftp_port = source;
            fprintf(vystupny_subor, "KOMUNIKACIA\n");
        }
        else if(tftp_port > -1 && destination == 69){
            tftp_port = -2;
        }
        if(source == tftp_port || destination == tftp_port){
            fprintf(vystupny_subor, "\n***********************************************\nCislo ramca je: %d\nVelkost ramca podla API je: %dB\n", cislo_ramca, velkost);
            if(velkost + 4 < 64)
                fprintf(vystupny_subor, "Velkost prenasanych dat je 64B\n");
            else
                fprintf(vystupny_subor, "Velkost prenasanych dat je %dB\n", velkost + 4);

            fprintf(vystupny_subor, "Jedna sa o ethernet II\n");
            vypis_adresy(vystupny_subor, data_ramca);
            fprintf(vystupny_subor, "Internet_IPv4\n");
            fprintf(vystupny_subor, "Zdrojova IP adresa je: %d.%d.%d.%d\n", data_ramca[26], data_ramca[27], data_ramca[28], data_ramca[29]);
            fprintf(vystupny_subor, "Cielova IP adresa je:  %d.%d.%d.%d\n", data_ramca[30], data_ramca[31], data_ramca[32], data_ramca[33]);

            //teraz vypisem zdrojovy a cielovy port
            fprintf(vystupny_subor, "zdrojovy port: %d\ncielovy port: %d\n", source, destination);
            fprintf(vystupny_subor, "Protokol je UDP\nTFTP\n");
            fprintf(vystupny_subor, "Vypis ramca v hexadecimalnom tvare:");
            for(int i = 0; i < velkost; i++){
                if(i % 16 == 0)
                    fprintf(vystupny_subor, "\n");
                fprintf(vystupny_subor, "%.2X ", data_ramca[i]);
            }
            fprintf(vystupny_subor, "\n***********************************************\n\n");
        }
    }
    
}

void ries_icmp(const u_char *data_ramca, const u_char *vnorene_data_ramca, FILE *vystupny_subor, EXTERNY *icmp, char filter, int cislo_ramca, int velkost){
    fprintf(vystupny_subor, "\n***********************************************\nCislo ramca je: %d\nVelkost ramca podla API je: %dB\n", cislo_ramca, velkost);
    if(velkost + 4 < 64)
        fprintf(vystupny_subor, "Velkost prenasanych dat je 64B\n");
    else
        fprintf(vystupny_subor, "Velkost prenasanych dat je %dB\n", velkost + 4);

    fprintf(vystupny_subor, "Jedna sa o ethernet II\n");
    vypis_adresy(vystupny_subor, data_ramca);
    fprintf(vystupny_subor, "Internet_IPv4\n");
    fprintf(vystupny_subor, "Zdrojova IP adresa je: %d.%d.%d.%d\n", data_ramca[26], data_ramca[27], data_ramca[28], data_ramca[29]);
    fprintf(vystupny_subor, "Cielova IP adresa je:  %d.%d.%d.%d\n", data_ramca[30], data_ramca[31], data_ramca[32], data_ramca[33]);

    while(icmp != NULL){
        if(vnorene_data_ramca[0] == icmp->value)
            break;
        icmp = icmp->dalsi;
    }
    if(icmp == NULL)
        fprintf(vystupny_subor, "Protokol je ICMP\nNEZNAMY\n");
    else
        fprintf(vystupny_subor, "Protokol je ICMP\n%s\n", icmp->name);
    fprintf(vystupny_subor, "Vypis ramca v hexadecimalnom tvare:");
    for(int i = 0; i < velkost; i++){
        if(i % 16 == 0)
            fprintf(vystupny_subor, "\n");
        fprintf(vystupny_subor, "%.2X ", data_ramca[i]);
    }
    fprintf(vystupny_subor, "\n***********************************************\n\n");
}

void ries_tcp(const u_char *data_ramca, const u_char *vnorene_data_ramca, FILE *vystupny_subor, EXTERNY *tcp, char filter, int cislo_ramca, int velkost, KOMUNIKACIE *zaciatok_komunikacii){
    // +13 z vnorene_data_ramca, aby som sa dostal na flag byte

    int source = vnorene_data_ramca[0] * 256 + vnorene_data_ramca[1], destination = vnorene_data_ramca[2] * 256 + vnorene_data_ramca[3];
    EXTERNY *pomocna = tcp;
    while(pomocna != NULL){
        if(destination == pomocna->value){
            tcp = pomocna;
            break;
        }
        pomocna = pomocna->dalsi;
    }
    if(pomocna == NULL){
        while(tcp != NULL){
            if(source == tcp->value)
                break;
            tcp = tcp->dalsi;
        }
    }   

        
    if(tcp == NULL)
        return;
    else{
        if(filter == 1 && tcp->value == 80)
            komunikacia_tcp(data_ramca, vnorene_data_ramca, vystupny_subor, cislo_ramca, tcp->name, velkost, zaciatok_komunikacii);
        else if(filter == 2 && tcp->value == 443)
            komunikacia_tcp(data_ramca, vnorene_data_ramca, vystupny_subor, cislo_ramca, tcp->name, velkost, zaciatok_komunikacii);
        else if(filter == 3 && tcp->value == 23)
            komunikacia_tcp(data_ramca, vnorene_data_ramca, vystupny_subor, cislo_ramca, tcp->name, velkost, zaciatok_komunikacii);
        else if(filter == 4 && tcp->value == 22)
            komunikacia_tcp(data_ramca, vnorene_data_ramca, vystupny_subor, cislo_ramca, tcp->name, velkost, zaciatok_komunikacii);
        else if(filter == 5 && tcp->value == 21)
            komunikacia_tcp(data_ramca, vnorene_data_ramca, vystupny_subor, cislo_ramca, tcp->name, velkost, zaciatok_komunikacii);
        else if(filter == 6 && tcp->value == 20)
            komunikacia_tcp(data_ramca, vnorene_data_ramca, vystupny_subor, cislo_ramca, tcp->name, velkost, zaciatok_komunikacii);
    }
    
}

void ries_ipv4(const u_char *data_ramca, FILE *vystupny_subor, EXTERNY *ipv4, DESTINATION_ADRESA *destination_adresy, EXTERNY *tcp, EXTERNY *icmp,
EXTERNY *udp, char filter, int cislo_ramca, int velkost, KOMUNIKACIE *zaciatok_komunikacii){
    DESTINATION_ADRESA *predosli = destination_adresy;
    int velkost_ip_headeru = (data_ramca[14] % 16) * 4;
    int ipv4_value = data_ramca[23];

    while(ipv4 != NULL){
        if(ipv4_value == ipv4->value)
            break;
        ipv4 = ipv4->dalsi;
    }

    if(!filter){
        fprintf(vystupny_subor, "Zdrojova IP adresa je: %d.%d.%d.%d\n", data_ramca[26], data_ramca[27], data_ramca[28], data_ramca[29]);
        fprintf(vystupny_subor, "Cielova IP adresa je:  %d.%d.%d.%d\n", data_ramca[30], data_ramca[31], data_ramca[32], data_ramca[33]);

        if(ipv4 == NULL)
            fprintf(vystupny_subor, "Protokol je NEZNAMY\n");
        else{
            fprintf(vystupny_subor, "Protokol je %s\n", ipv4->name);
            if(ipv4->value == 6 || ipv4->value == 17){
                int source = data_ramca[14 + velkost_ip_headeru] * 256 + data_ramca[15 + velkost_ip_headeru], destination = data_ramca[16 + velkost_ip_headeru] * 256 + data_ramca[17 + velkost_ip_headeru];
                fprintf(vystupny_subor, "zdrojovy port: %d\ncielovy port: %d\n", source, destination);
            }
        }
            
    }
    

    //teraz pridam destination ip adresu do zoznamu .. ak tam je, tak inkrementuje jej vyskyt

    if(!filter){
        while(destination_adresy != NULL){
            if(destination_adresy->vyskyt < 0){
                destination_adresy = destination_adresy->dalsi;
                continue;
            }

            if(destination_adresy->adresa[0] == data_ramca[30] && destination_adresy->adresa[1] == data_ramca[31] && destination_adresy->adresa[2] == data_ramca[32] && destination_adresy->adresa[3] == data_ramca[33]){
                destination_adresy->vyskyt++;
                break;
            }

            predosli = destination_adresy;
            destination_adresy = destination_adresy->dalsi;
        }

        //vytvorim novy zaznam, ak pridem na koniec zoznamu
        if(destination_adresy == NULL){
            predosli->dalsi = (DESTINATION_ADRESA *)malloc(sizeof(DESTINATION_ADRESA));
            predosli->dalsi->vyskyt = 1;
            predosli->dalsi->dalsi = NULL;
            predosli->dalsi->adresa[0] = data_ramca[30];
            predosli->dalsi->adresa[1] = data_ramca[31];
            predosli->dalsi->adresa[2] = data_ramca[32];
            predosli->dalsi->adresa[3] = data_ramca[33];
        }
    }
    

    //tu idem riesit posledny bod, filtrovane komunikacie

    if(ipv4 != NULL){
        if(ipv4->value == 6 && filter > 0 && filter < 7)
            ries_tcp(data_ramca, data_ramca + 14 + velkost_ip_headeru, vystupny_subor, tcp, filter, cislo_ramca, velkost, zaciatok_komunikacii);
        if(ipv4->value == 17 && filter == 7)
            ries_udp(data_ramca, data_ramca + 14 + velkost_ip_headeru, vystupny_subor, udp, filter, cislo_ramca, velkost);
        if(ipv4->value == 1 && filter == 8)
            ries_icmp(data_ramca, data_ramca + 14 + velkost_ip_headeru, vystupny_subor, icmp, filter, cislo_ramca, velkost);
    }

}

//funkcia riesiaca eth2 ramce, rozlisuje ci sa jedna o eth2 v snape
void ries_eth2_1(const u_char *data_ramca, FILE *vystupny_subor, EXTERNY *eth, int snap, EXTERNY *ipv4,
DESTINATION_ADRESA *destination_adresy, EXTERNY *tcp, EXTERNY *icmp, EXTERNY *udp, char filter, int cislo_ramca, int velkost, KOMUNIKACIE *zaciatok_komunikacii){
    int eth_value;
    if(snap)
        eth_value = data_ramca[20] * 256 + data_ramca[21];
    else
        eth_value = data_ramca[12] * 256 + data_ramca[13];

    while(eth != NULL){
        if(eth_value == eth->value)
            break;
        eth = eth->dalsi;
    }

    if(!snap && !filter)
        vypis_adresy(vystupny_subor, data_ramca);

    if(!filter)
        if(eth == NULL)
            fprintf(vystupny_subor, "NEZNAMY\n");
        else
            fprintf(vystupny_subor, "%s\n", eth->name);

    if(eth != NULL)
        if(eth->value == 2048 && snap != 1)
            ries_ipv4(data_ramca, vystupny_subor, ipv4, destination_adresy, tcp, icmp, udp, filter, cislo_ramca, velkost, zaciatok_komunikacii);
}

//funkcia riesiaca skupiny 802
void ries_802_1(const u_char *data_ramca, FILE *vystupny_subor, EXTERNY *ieee, EXTERNY *eth){
    while(ieee != NULL){
        if(data_ramca[14] == ieee->value)
            break;
        ieee = ieee->dalsi;
    }

    if(ieee == NULL){
        fprintf(vystupny_subor, "LLC\nNEZNAMY\n");
        vypis_adresy(vystupny_subor, data_ramca);
    }
    else{
        if(ieee->value == 255){
            fprintf(vystupny_subor, "RAW\n");
            vypis_adresy(vystupny_subor, data_ramca);
        }
        else if(ieee->value == 170){      //ak je SNAP, tak idem najst eth type
            fprintf(vystupny_subor, "LLC + SNAP\n");
            vypis_adresy(vystupny_subor, data_ramca);
            ries_eth2_1(data_ramca, vystupny_subor, eth, 1, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, NULL);
        }
        else{
            fprintf(vystupny_subor, "LLC\n%s\n", ieee->name);
            vypis_adresy(vystupny_subor, data_ramca);
        }
    }
}

void ries_ramec_1(int cislo_ramca, struct pcap_pkthdr *hlavicka_ramca, const u_char *data_ramca, FILE *vystupny_subor, EXTERNY *ieee, EXTERNY *eth, EXTERNY *ipv4,
DESTINATION_ADRESA *destination_adresy, EXTERNY *tcp, EXTERNY *icmp, EXTERNY *udp, char filter, KOMUNIKACIE *zaciatok_komunikacii){
    if(!filter){
        fprintf(vystupny_subor, "\n***********************************************\nCislo ramca je: %d\nVelkost ramca podla API je: %dB\n", cislo_ramca, hlavicka_ramca->len);
        if(hlavicka_ramca->len + 4 < 64)
            fprintf(vystupny_subor, "Velkost prenasanych dat je 64B\n");
        else
            fprintf(vystupny_subor, "Velkost prenasanych dat je %dB\n", hlavicka_ramca->len + 4);
    } 

    int kontrolna_velkost = data_ramca[12] * 256 + data_ramca[13];
    
    //printf("kontrolka %d\n", kontrolna_velkost);
    if(kontrolna_velkost < 1536){              //zistujem podla toho, ci je velkost ramca na 12. a 13. bite, co je standard pre IEEE
        if(filter)
            return;
        fprintf(vystupny_subor, "Jedna sa o skupinu 802.3 ");
        ries_802_1(data_ramca, vystupny_subor, ieee, eth);
    }
    else{
        if(!filter)
            fprintf(vystupny_subor, "Jedna sa o ethernet II\n");
        ries_eth2_1(data_ramca, vystupny_subor, eth, 0, ipv4, destination_adresy, tcp, icmp, udp, filter, cislo_ramca, hlavicka_ramca->len, zaciatok_komunikacii);
    }

    
    if(!filter){
        fprintf(vystupny_subor, "Vypis ramca v hexadecimalnom tvare:");
        for(int i = 0; i < hlavicka_ramca->len; i++){
            if(i % 16 == 0)
                fprintf(vystupny_subor, "\n");
            fprintf(vystupny_subor, "%.2X ", data_ramca[i]);
        }
        fprintf(vystupny_subor, "\n***********************************************\n\n");
    }
}

EXTERNY *init(char *nazov){
    EXTERNY *start = (EXTERNY *)malloc(sizeof(EXTERNY)), *akt;
    FILE *subor = fopen(nazov, "r");
    if(subor == NULL){
        printf("Nepodarilo sa otvorit externy subor\n");
        return NULL;
    }
    int val;
    if(fscanf(subor, "%x", &val) > 0){
        start->value = val;
        fscanf(subor, "%s", start->name);
        start->dalsi = NULL;
    }

    akt = start;
    while(fscanf(subor, "%x", &val) > 0){
        akt->dalsi = (EXTERNY *)malloc(sizeof(EXTERNY));
        akt = akt->dalsi;
        akt->dalsi = NULL;
        akt->value = val;
        fscanf(subor, "%s", akt->name);
    }

    fclose(subor);
    return start;
}

DESTINATION_ADRESA *init_adresy(){
    DESTINATION_ADRESA *start = (DESTINATION_ADRESA *)malloc(sizeof(DESTINATION_ADRESA));
    start->dalsi = NULL;

    //tento zaciatok nebudeme kontrolovat, preto vyskyt -1
    start->vyskyt = -1;

    return start;
}

KOMUNIKACIE *init_komunikacii(){
    KOMUNIKACIE *start = (KOMUNIKACIE *)malloc(sizeof(KOMUNIKACIE));
    start->dalsi = NULL;

    //tento zaciatok nebudeme kontrolovat, preto vyskyt -1
    start->uzavreta = -1;

    return start;
}

void vypis_ip_adresy(FILE *vystupny_subor, DESTINATION_ADRESA *destination_adresy){
    DESTINATION_ADRESA *start = destination_adresy;
    int max_vyskyt = 0;
    unsigned char max_vyskyt_adresa[4];
    
    fprintf(vystupny_subor, "Zoznam destination IP adries:\n");

    while(destination_adresy != NULL){
        if(destination_adresy->vyskyt != -1)
            fprintf(vystupny_subor, "%d.%d.%d.%d\n", destination_adresy->adresa[0], destination_adresy->adresa[1], destination_adresy->adresa[2], destination_adresy->adresa[3]);

        if(destination_adresy->vyskyt > max_vyskyt){
            max_vyskyt = destination_adresy->vyskyt;
            max_vyskyt_adresa[0] = destination_adresy->adresa[0];
            max_vyskyt_adresa[1] = destination_adresy->adresa[1];
            max_vyskyt_adresa[2] = destination_adresy->adresa[2];
            max_vyskyt_adresa[3] = destination_adresy->adresa[3];
        }
        destination_adresy = destination_adresy->dalsi;
    }

    if(max_vyskyt)
        fprintf(vystupny_subor, "\nIP adresa s najvacsim poctom prijatych paketov:\n%d.%d.%d.%d       %d paketovv\n", max_vyskyt_adresa[0], max_vyskyt_adresa[1], max_vyskyt_adresa[2], max_vyskyt_adresa[3], max_vyskyt);
    else
        fprintf(vystupny_subor, "V subore sa nenachadzali IPv4 pakety\n");

}

void vypis_komunikacny_ramec(pcap_t *subor, struct pcap_pkthdr *hlavicka_ramca, const u_char *data_ramca, int filter, FILE *vystupny_subor, int cislo_ramca){
    char protokol[15];
    int velkost_ip_headeru = (data_ramca[14] % 16) * 4;
    const u_char *vnorene_data_ramca = data_ramca + 14 + velkost_ip_headeru;
    int source = vnorene_data_ramca[0] * 256 + vnorene_data_ramca[1], destination = vnorene_data_ramca[2] * 256 + vnorene_data_ramca[3];
    if(filter == 1)
        sprintf(protokol, "HTTP");
    else if(filter == 2)
        sprintf(protokol, "HTTPS");
    else if(filter == 3)
        sprintf(protokol, "TELLNET");
    else if(filter == 4)
        sprintf(protokol, "SSH");
    else if(filter == 5)
        sprintf(protokol, "FTP_riadiace");
    else if(filter == 6)
        sprintf(protokol, "FTP_datove");

    fprintf(vystupny_subor, "\n***********************************************\nCislo ramca je: %d\nVelkost ramca podla API je: %dB\n", cislo_ramca, hlavicka_ramca->len);
    if(hlavicka_ramca->len + 4 < 64)
        fprintf(vystupny_subor, "Velkost prenasanych dat je 64B\n");
    else
        fprintf(vystupny_subor, "Velkost prenasanych dat je %dB\n", hlavicka_ramca->len + 4);

    fprintf(vystupny_subor, "Jedna sa o ethernet II\n");
    vypis_adresy(vystupny_subor, data_ramca);
    fprintf(vystupny_subor, "Internet_IPv4\n");
    fprintf(vystupny_subor, "Zdrojova IP adresa je: %d.%d.%d.%d\n", data_ramca[26], data_ramca[27], data_ramca[28], data_ramca[29]);
    fprintf(vystupny_subor, "Cielova IP adresa je:  %d.%d.%d.%d\n", data_ramca[30], data_ramca[31], data_ramca[32], data_ramca[33]);

    //teraz vypisem zdrojovy a cielovy port
    fprintf(vystupny_subor, "zdrojovy port: %d\ncielovy port: %d\n", source, destination);

    fprintf(vystupny_subor, "Protokol je TCP\n%s\n", protokol);
    fprintf(vystupny_subor, "Vypis ramca v hexadecimalnom tvare:");
    for(int i = 0; i < hlavicka_ramca->len; i++){
        if(i % 16 == 0)
            fprintf(vystupny_subor, "\n");
        fprintf(vystupny_subor, "%.2X ", data_ramca[i]);
    }
    fprintf(vystupny_subor, "\n***********************************************\n\n");

}

void vypis_komunikacii(pcap_t *subor, struct pcap_pkthdr *hlavicka_ramca, const u_char *data_ramca, int filter, KOMUNIKACIE *zaciatok_komunikacii, int komplet, FILE *vystupny_subor){
    //najskor najdem kompletnu uzatvorenu komunikaciu cez zaciatok komunikacii
    while(zaciatok_komunikacii->uzavreta != komplet){
        zaciatok_komunikacii = zaciatok_komunikacii->dalsi;
        if(zaciatok_komunikacii == NULL){
            fprintf(vystupny_subor, "Neexistuje tento typ komunikacie (%d) v tomto subore\n", komplet);
            return;
        }
    }

    //zistim, ci nema viacej ako 20 ramcov
    //vypisem jednotlive ramce
    int cislo_ramca = 0, poradie_v_komunikacii = 0;

    if(komplet)
        fprintf(vystupny_subor, "KOMPLETNA KOMUNIKACIA\n");
    else
        fprintf(vystupny_subor, "NEKOMPLETNA KOMUNIKACIA\n");
    while(poradie_v_komunikacii < zaciatok_komunikacii->pocet_ramcov && pcap_next_ex(subor, &hlavicka_ramca, &data_ramca) > 0){      //dokym nacitavam ramce, tak ich budem riesit
        cislo_ramca++;
        if(cislo_ramca == zaciatok_komunikacii->zoznam_ramcov[poradie_v_komunikacii]){
            if(poradie_v_komunikacii < 10 || poradie_v_komunikacii > zaciatok_komunikacii->pocet_ramcov - 11)
                vypis_komunikacny_ramec(subor, hlavicka_ramca, data_ramca, filter, vystupny_subor, cislo_ramca);
            else
                fprintf(vystupny_subor, ".\n");
            poradie_v_komunikacii++;
        }
    }
}

int main(){
    
    /*
    //cyklus pre vypisanie cisiel na vsetky testy
    for(int i = 0; i < 9; i++){
        for(int y = 0; y < 37; y++)
            printf("%d %d %d ", y + 1, i, 1);
    }
    */
    


    int typ_suboru = 1, cislo_ramca;
    EXTERNY *ieee = init("externe_subory/802.txt"), *eth = init("externe_subory/eth.txt"), *ipv4 = init("externe_subory/ipv4.txt"),
    *tcp = init("externe_subory/tcp.txt"), *icmp = init("externe_subory/icmp.txt"), *udp = init("externe_subory/udp.txt");
    pcap_t *subor;      //pointer na subor pcap, co budem otvarat
    struct pcap_pkthdr *hlavicka_ramca;
    const u_char *data_ramca;
    FILE *vystupny_subor;

    //tu su vlozene nazvy pcap suborov pre nasledne jednoduche pracovanie s nimi
    char nazvy_suborov[37][20] = {"eth-1", "eth-2", "eth-3", "eth-4", "eth-5", "eth-6", "eth-7", "eth-8", "eth-9", "trace_ip_nad_20_B", "trace-1", 
    "trace-2", "trace-3", "trace-4", "trace-5", "trace-6", "trace-7", "trace-8", "trace-9", "trace-10", "trace-11", "trace-12", "trace-13", 
    "trace-14", "trace-15", "trace-16", "trace-17", "trace-18", "trace-19", "trace-20", "trace-21", "trace-22", "trace-23", "trace-24", "trace-25", 
    "trace-26", "trace-27"};
    char pomocna[50], pomocna2[50], filter;
    for(int i = 0; i < 37; i++){
        printf("%-2d  =>  %-25s", i + 1, nazvy_suborov[i]);
        if((i + 1) % 3 == 0)
            printf("\n");
    }

    //cyklus pre subory
    while(typ_suboru){
        printf("\nNapis cislo suboru, ktory chces analyzovat:\n");
        scanf("%d", &typ_suboru);
        if(typ_suboru < 1 || typ_suboru > 37){
            printf("Zadal si zlu hodnotu!\n");
            typ_suboru = 1;
            continue;
        }
        sprintf(pomocna, "pcap/%s.pcap", nazvy_suborov[typ_suboru - 1]);

        
        char *buffer = (char *)malloc(PCAP_ERRBUF_SIZE);
        subor = nacitaj_PCAP_file(pomocna, buffer);   //nacitam subor podla nazvu zo zlozky pcap
        if(subor == NULL){              //ak sa mi nepodari otvorit subor, idem na zaciatok cyklu
            free(buffer);
            continue;
        }

        //kontrolny vypis spajanych zoznamov vytvorenych z externych suborov
        /*
        while(tcp != NULL){
            printf("%.4X %s\n", tcp->value, tcp->name);
            tcp = tcp->dalsi;
        }
        */

       printf("Aky filter z bodu 4 chces pouzit na subor?\n0  =>  Nechcem filter\n1  =>  HTTP%-10c2  =>  HTTPS%-10c3  =>  TELLNET\n4  =>  SSH%-11c5  =>  FTP riadiace%-3c6  =>  FTP datove\n7  =>  TFTP%-10c8  =>  ICMP%-11c\n", ' ', ' ', ' ', ' ', ' ', ' ');
       scanf("%d", &filter);
       if(filter < 0 || filter > 8){
            printf("Zadal si zlu hodnotu!\n");
            typ_suboru = 1;
            continue;
        }
        
        DESTINATION_ADRESA *destination_adresy = init_adresy();
        KOMUNIKACIE *zaciatok_komunikacii = init_komunikacii();
        cislo_ramca = 0;
        strcpy(pomocna2, pomocna);
        sprintf(pomocna, "vystupne_subory/%d%s.txt", filter, nazvy_suborov[typ_suboru - 1]);
        vystupny_subor = fopen(pomocna, "w");

        //cyklus pre ramce v subore
        while(pcap_next_ex(subor, &hlavicka_ramca, &data_ramca) > 0){      //dokym nacitavam ramce, tak ich budem riesit
            ries_ramec_1(++cislo_ramca, hlavicka_ramca, data_ramca, vystupny_subor, ieee, eth, ipv4, destination_adresy, tcp, icmp, udp, filter, zaciatok_komunikacii);
            
            //obmedzenie na pocet ramcov
            //if(cislo_ramca > 4)
            //    return 0;
        }

        if(!filter)
            vypis_ip_adresy(vystupny_subor, destination_adresy);
        
        //teraz uvolnim pamat, kedze som skoncil so suborom
        DESTINATION_ADRESA *start;
        while(destination_adresy != NULL){
            start = destination_adresy->dalsi;
            free(destination_adresy);
            destination_adresy = start;
        }

        if(filter > 0 && filter < 7){
            pcap_close(subor);
            subor = nacitaj_PCAP_file(pomocna2, buffer);
            vypis_komunikacii(subor, hlavicka_ramca, data_ramca, filter, zaciatok_komunikacii, 1, vystupny_subor);
            pcap_close(subor);
            subor = nacitaj_PCAP_file(pomocna2, buffer);
            vypis_komunikacii(subor, hlavicka_ramca, data_ramca, filter, zaciatok_komunikacii, 0, vystupny_subor);
        }

        tftp_port = -1;
        fclose(vystupny_subor);
        free(buffer);
        pcap_close(subor);
        printf("Chces zvolit dalsi subor?\n0 -> nie\n1 -> ano\n");
        scanf("%d", &typ_suboru);
    }

    printf("Koniec");
    return 0;
}


// pre test vsetkych suborov
/*
1 0 1 2 0 1 3 0 1 4 0 1 5 0 1 6 0 1 7 0 1 8 0 1 9 0 1 10 0 1 11 0 1 12 0 1 13 0 1 14 0 1 15 0 1 16 0 1 17 0 1 18 0 1 19 0 1 20 0 1 21 0 1
22 0 1 23 0 1 24 0 1 25 0 1 26 0 1 27 0 1 28 0 1 29 0 1 30 0 1 31 0 1 32 0 1 33 0 1 34 0 1 35 0 1 36 0 1 37 0 1 1 1 1 2 1 1 3 1 1 4 1 1 5
1 1 6 1 1 7 1 1 8 1 1 9 1 1 10 1 1 11 1 1 12 1 1 13 1 1 14 1 1 15 1 1 16 1 1 17 1 1 18 1 1 19 1 1 20 1 1 21 1 1 22 1 1 23 1 1 24 1 1 25 1
1 26 1 1 27 1 1 28 1 1 29 1 1 30 1 1 31 1 1 32 1 1 33 1 1 34 1 1 35 1 1 36 1 1 37 1 1 1 2 1 2 2 1 3 2 1 4 2 1 5 2 1 6 2 1 7 2 1 8 2 1 9 2
1 10 2 1 11 2 1 12 2 1 13 2 1 14 2 1 15 2 1 16 2 1 17 2 1 18 2 1 19 2 1 20 2 1 21 2 1 22 2 1 23 2 1 24 2 1 25 2 1 26 2 1 27 2 1 28 2 1 29
2 1 30 2 1 31 2 1 32 2 1 33 2 1 34 2 1 35 2 1 36 2 1 37 2 1 1 3 1 2 3 1 3 3 1 4 3 1 5 3 1 6 3 1 7 3 1 8 3 1 9 3 1 10 3 1 11 3 1 12 3 1 13
3 1 14 3 1 15 3 1 16 3 1 17 3 1 18 3 1 19 3 1 20 3 1 21 3 1 22 3 1 23 3 1 24 3 1 25 3 1 26 3 1 27 3 1 28 3 1 29 3 1 30 3 1 31 3 1 32 3 1
33 3 1 34 3 1 35 3 1 36 3 1 37 3 1 1 4 1 2 4 1 3 4 1 4 4 1 5 4 1 6 4 1 7 4 1 8 4 1 9 4 1 10 4 1 11 4 1 12 4 1 13 4 1 14 4 1 15 4 1 16 4 1
17 4 1 18 4 1 19 4 1 20 4 1 21 4 1 22 4 1 23 4 1 24 4 1 25 4 1 26 4 1 27 4 1 28 4 1 29 4 1 30 4 1 31 4 1 32 4 1 33 4 1 34 4 1 35 4 1 36 4
1 37 4 1 1 5 1 2 5 1 3 5 1 4 5 1 5 5 1 6 5 1 7 5 1 8 5 1 9 5 1 10 5 1 11 5 1 12 5 1 13 5 1 14 5 1 15 5 1 16 5 1 17 5 1 18 5 1 19 5 1 20 5
1 21 5 1 22 5 1 23 5 1 24 5 1 25 5 1 26 5 1 27 5 1 28 5 1 29 5 1 30 5 1 31 5 1 32 5 1 33 5 1 34 5 1 35 5 1 36 5 1 37 5 1 1 6 1 2 6 1 3 6
1 4 6 1 5 6 1 6 6 1 7 6 1 8 6 1 9 6 1 10 6 1 11 6 1 12 6 1 13 6 1 14 6 1 15 6 1 16 6 1 17 6 1 18 6 1 19 6 1 20 6 1 21 6 1 22 6 1 23 6 1
24 6 1 25 6 1 26 6 1 27 6 1 28 6 1 29 6 1 30 6 1 31 6 1 32 6 1 33 6 1 34 6 1 35 6 1 36 6 1 37 6 1 1 7 1 2 7 1 3 7 1 4 7 1 5 7 1 6 7 1 7 7
1 8 7 1 9 7 1 10 7 1 11 7 1 12 7 1 13 7 1 14 7 1 15 7 1 16 7 1 17 7 1 18 7 1 19 7 1 20 7 1 21 7 1 22 7 1 23 7 1 24 7 1 25 7 1 26 7 1 27 7
1 28 7 1 29 7 1 30 7 1 31 7 1 32 7 1 33 7 1 34 7 1 35 7 1 36 7 1 37 7 1 1 8 1 2 8 1 3 8 1 4 8 1 5 8 1 6 8 1 7 8 1 8 8 1 9 8 1 10 8 1 11 8
1 12 8 1 13 8 1 14 8 1 15 8 1 16 8 1 17 8 1 18 8 1 19 8 1 20 8 1 21 8 1 22 8 1 23 8 1 24 8 1 25 8 1 26 8 1 27 8 1 28 8 1 29 8 1 30 8 1 31
8 1 32 8 1 33 8 1 34 8 1 35 8 1 36 8 1 37 8 0
*/