#include <stdio.h>
#include <winsock2.h>
#include <stdint.h>
#include <string.h>

#define VELKOST_HLAVICKY 9
#define MAX_DATA 1463

int chyba = 5;

//filter (ip.src == 192.168.0.248 && ip.dst == 192.168.0.45) || (ip.dst == 192.168.0.45 && ip.src == 192.168.0.248)
//filter (ip.src == 127.0.0.1 && ip.dst == 127.0.0.1)

//cd "c:\Users\Admin\Desktop\pks2\" ; if ($?) { gcc main.c -lws2_32 -o main } ; if ($?) { .\main }
//cd "c:\Users\Marek\Desktop\pks2\" && gcc main.c -lws2_32 -o main && "c:\Users\Marek\Desktop\pks2\"main
//-lws2_32

//chcel som mat moju hlavicu v strukture, ale stretol som sa so zaokruhlovanim struktur v C
//co by pri pouziti struktury vyeskalovalo do pamatovej neefektivnosti

//PREVZATY KOD ZO STRANKY OD manjunath
//https://stackoverflow.com/questions/21939392/crc-16-program-to-calculate-check-sum
#define CRC16 0x8005
uint16_t gen_crc16(const uint8_t *data, uint16_t size){
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

    return crc;
}
//KONIEC PREVZATEHO KODU

char *priprav_spravu(int ID_fragmentu, short velkost, char typ_spravy){
	char *sprava_na_poslanie = (char *)malloc(velkost + VELKOST_HLAVICKY);
	
	sprava_na_poslanie[0] = typ_spravy;
	*((int *)(sprava_na_poslanie + 1)) = ID_fragmentu;
	*((short *)(sprava_na_poslanie + 5)) = velkost;

	return sprava_na_poslanie;
}

void vloz_CRC_do_spravy(char *sprava_na_poslanie){   //potom, ako vlozim data do spravy, mozem vypocitat CRC, ktore sa vlozi do hlavicky fragmentu
	short crc_vysledok = gen_crc16(sprava_na_poslanie + VELKOST_HLAVICKY, *((short *)(sprava_na_poslanie + 5)));
	*((short *)(sprava_na_poslanie + 7)) = crc_vysledok;
}

int skontroluj_CRC_v_sprave(char *sprava){
	//SIMULACIA CHYBY
	if(*((int *)(sprava + 1)) == chyba){
		chyba = -1;
		(*(sprava + 9))++;
	}
	//SIMULACIA CHYBY

	unsigned short CRC_z_hlavicky = *((short *)(sprava + 7)), velkost_dat = *((short *)(sprava + 5)), pocitane_CRC;
	pocitane_CRC = gen_crc16(sprava + 9, velkost_dat);
	//printf("pocital som %d  a prislo %d\n", pocitane_CRC, CRC_z_hlavicky);
	if(pocitane_CRC == CRC_z_hlavicky)
		return 1;
	return 0;
}

void dostan_meno_suboru_z_cesty(char *buffer){
	int dlzka = strlen(buffer), posledne_lomitko, i;
	for(i = 0; i < dlzka; i++)
		if(buffer[i] == '\\')
			posledne_lomitko = i;
	for(i = posledne_lomitko + 1; i < dlzka; i++)
		buffer[i - posledne_lomitko - 1] = buffer[i];
	buffer[i - posledne_lomitko - 1] = 0;
}

void posli(SOCKET moj_socket, struct sockaddr *adresa, int size_of_adresa){
	int max_velkost_uzivatela = 1000, volba_spravy = 1, ID_fragmentu = 0, counter = 0;
	char *sprava_na_poslanie, recieve_buffer[1500], buffer[1500];
	while(1){
		printf("Zadaj maximalnu velkost dat fragmentu z rozmedzia 1 - %d\n", MAX_DATA);
		scanf("%d", &max_velkost_uzivatela);
		if(max_velkost_uzivatela < 1 || max_velkost_uzivatela > MAX_DATA){}
		else
			break;
	}
	while(1){
		printf("Chces poslat spravu alebo subor?\nsprava  ->  1\nsubor  ->  2\n", MAX_DATA);
		scanf("%d", &volba_spravy);
		if(volba_spravy == 1 || volba_spravy == 2)
			break;
	}
	
	if(volba_spravy == 1){
		//dam vediet, ze pojde o spravy a pockam na potvrdenie
		sprava_na_poslanie = priprav_spravu(0, 0, 12);
		sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
		printf("Poslal som info, ze pojde o spravy\n");
		recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
		printf("Dostal som potvrdenie\n");
		free(sprava_na_poslanie);

		char jeden_char, sprava[15000];
		int main_counter, dlzka;
		while(1){
			scanf("\n");
			gets(sprava);
			main_counter = 0;
			dlzka = strlen(sprava);
			while(main_counter != dlzka){
				counter = 0;
				while(counter != max_velkost_uzivatela && main_counter != dlzka){
					buffer[counter++] = sprava[main_counter++];
				}
				sprava_na_poslanie = priprav_spravu(++ID_fragmentu, counter, 2);
				memcpy(sprava_na_poslanie + VELKOST_HLAVICKY, buffer, counter);
				vloz_CRC_do_spravy(sprava_na_poslanie);

				sendto(moj_socket, sprava_na_poslanie, counter + VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
				printf("Poslal som %dB dat v sprave s ID %d\n", counter, ID_fragmentu);
				while(1){
					recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
					if(*recieve_buffer == 3){
						printf("Prislo potvrdenie o spravnom prijati spravy s ID: %d\n", *((int *)(recieve_buffer + 1)));
						counter = 0;
						break;
					}
					else if(*recieve_buffer == 4){
						printf("Prislo potvrdenie o chybnom prijati spravy s ID: %d, posielam ju este raz\n", *((int *)(recieve_buffer + 1)));
						vloz_CRC_do_spravy(sprava_na_poslanie);
						sendto(moj_socket, sprava_na_poslanie, counter + VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
						continue;
					}
					else{
						printf("Dosiel iny typ spravy, konkretne %d\n", *recieve_buffer);
						break;
					}
				}
				free(sprava_na_poslanie);
			}

			
			sprava_na_poslanie = priprav_spravu(++ID_fragmentu, counter, 14);
			sendto(moj_socket, sprava_na_poslanie, counter + VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
			recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
			break;
		}
	}
	else if(volba_spravy == 2){
		//dam vediet, ze pojde o subory a pockam na potvrdenie
		sprava_na_poslanie = priprav_spravu(0, 0, 13);
		sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
		printf("Poslal som info, ze pojde o subor\n");
		recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
		printf("Dostal som potvrdenie\n");
		free(sprava_na_poslanie);

		while(1){
			//najskor si vypytam a poslem informacie o subore
			printf("Zadaj absolutnu cestu k suboru, co chces poslat\n");
			scanf("\n");
			gets(buffer);
			FILE *otvoreny_subor;
			while(!(otvoreny_subor = fopen(buffer, "rb"))){
				printf("Subor sa nepodarilo otvorit, skus znovu zadat cestu\n");
				scanf("\n");
				gets(buffer);
			}
			dostan_meno_suboru_z_cesty(buffer);
			printf("Subor %s sa podarilo otvorit\n", buffer);

			//najskor poslem fragment(y) s nazvom suboru
			char pomocny_buffer[400];
			int dlzka_nazvu = strlen(buffer);
			int main_counter = 0, counter;
			while(main_counter != dlzka_nazvu){
				counter = 0;
				while(counter != max_velkost_uzivatela && main_counter != dlzka_nazvu){
					pomocny_buffer[counter++] = buffer[main_counter++];
				}
				sprava_na_poslanie = priprav_spravu(++ID_fragmentu, counter, 5);
				memcpy(sprava_na_poslanie + VELKOST_HLAVICKY, buffer + main_counter - counter, counter);
				
				while(1){
					vloz_CRC_do_spravy(sprava_na_poslanie);
					sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY + counter, 0, adresa, size_of_adresa);
					printf("Posielam fragment s datami o subore s ID %d o velkosti %d B\n", *((int *)(sprava_na_poslanie + 1)), *((short *)(sprava_na_poslanie + 5)));
					recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
					if(*recieve_buffer == 4){	//resend pri chybe
						printf("Dosla mi info o chybnom fragmente s ID %d, posielam znovu\n", *((int *)(recieve_buffer + 1)));
						continue;
					}
					else if(*recieve_buffer == 5){
						printf("Dostal som potvrdenie o spravnosti prijatych dat o subore vo fragmente s ID %d\n", *((int *)(recieve_buffer + 1)));
						free(sprava_na_poslanie);
						break;
					}
				}
			}

			sprava_na_poslanie = priprav_spravu(++ID_fragmentu, counter, 9);
			sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
			printf("Poslal som vsetky info o subore a potvrdenie s ID %d a cakam na zadanie cesty na druhej strane...\n", *((int *)(sprava_na_poslanie + 1)));
			while(1){
				recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
				if(*recieve_buffer == 9){
					printf("Prislo potvrdenie o spravnom prijati vsetkych info o subore s ID: %d\n", *((int *)(recieve_buffer + 1)));
					break;
				}
				else{
					printf("Dosiel iny typ spravy, konkretne %d\n", *recieve_buffer);
					break;
				}
			}
			free(sprava_na_poslanie);


			unsigned long long int pocet_dat_v_B = 0;
			int pocet_odoslanych_fragmentov_s_datami = 0, end;
			char jeden_char;
			counter = 0;
			//samotny prenos suboru
			while(1){
				end = fread(&jeden_char, 1, 1, otvoreny_subor);
				if(counter < max_velkost_uzivatela && end == 1){
					buffer[counter++] = jeden_char;
					pocet_dat_v_B++;
				}
				if(counter == max_velkost_uzivatela || (end != 1 && counter != 0)){
					sprava_na_poslanie = priprav_spravu(++ID_fragmentu, counter, 2);
					memcpy(sprava_na_poslanie + VELKOST_HLAVICKY, buffer, counter);
					vloz_CRC_do_spravy(sprava_na_poslanie);
					pocet_odoslanych_fragmentov_s_datami++;
					sendto(moj_socket, sprava_na_poslanie, counter + VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
					printf("Poslal som %dB dat v sprave s ID %d\n", *((short *)(sprava_na_poslanie + 5)), ID_fragmentu);
					while(1){
						recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
						if(*recieve_buffer == 3){
							printf("Prislo potvrdenie o spravnom prijati spravy s ID: %d\n", *((int *)(recieve_buffer + 1)));
							counter = 0;
							break;
						}
						else if(*recieve_buffer == 4){
							printf("Prislo potvrdenie o chybnom prijati spravy s ID: %d, posielam ju este raz\n", *((int *)(recieve_buffer + 1)));
							pocet_odoslanych_fragmentov_s_datami++;
							vloz_CRC_do_spravy(sprava_na_poslanie);
							sendto(moj_socket, sprava_na_poslanie, counter + VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
							continue;
						}
						else{
							printf("Dosiel iny typ spravy, konkretne %d\n", *recieve_buffer);
							break;
						}
					}
					free(sprava_na_poslanie);
				}
				if(end != 1)
					break;
			}

			//fragment s info o uzavreti suboru
			sprava_na_poslanie = priprav_spravu(++ID_fragmentu, 0, 9);
			sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
			printf("Posielam info o uzavreti suboru v sprave s ID %d\n", *((int *)(sprava_na_poslanie + 1)));
			recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
			if(*recieve_buffer == 9){
				fclose(otvoreny_subor);
				printf("Doslo potvrdenie s ID %d, ze subor sa podarilo uspesne uzavriet na oboch stranach\n", *((int *)(recieve_buffer + 1)));
			}
			else{
				printf("Prisiel necakany fragment typu %d a subor sa nepodarilo uzavriet\n", *recieve_buffer);
			}

			printf("Subor bol uspesne poslany v %d fragmentoch (chybne sa pocitaju tiez)\nCelkove data suboru cinia %llu B\n", pocet_odoslanych_fragmentov_s_datami, pocet_dat_v_B);
			break;
		}
	}
}

void prijmi(SOCKET moj_socket, struct sockaddr *adresa, int size_of_adresa){
	int volba_spravy;
	char *sprava_na_poslanie, recieve_buffer[1500];;

	int doimplementacia = 0, doimplementacia_chybne = 0;

	recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
	sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, *recieve_buffer);
	sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
	free(sprava_na_poslanie);

	if(*recieve_buffer == 12){
		printf("Pojde o posielanie sprav\n");
		int dlzka_spravy;
		while(1){
			recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);

			if(*recieve_buffer == 2){


				doimplementacia++;


				if(skontroluj_CRC_v_sprave(recieve_buffer) != 0){
					//printf("CRC hodnoty sedia\n");
				}
				else{
					printf("CRC nesedia, posielam ziadost o znovuposlanie fragmentu cislo %d\n", *((int *)(recieve_buffer + 1)));

					doimplementacia_chybne++;

					sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 4);
					sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
					continue;
				}
				dlzka_spravy = *((short *)(recieve_buffer + 5));
				for(int i = 0; i < dlzka_spravy; i++)
					printf("%c", recieve_buffer[VELKOST_HLAVICKY + i]);

				printf("\nPrijal som %dB dat v sprave s ID %d a posielam spat potvrdenie o prijati\n", dlzka_spravy, *((int *)(recieve_buffer + 1)));
				sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 3);	//typ spravy je 3, teda potvrdenie
				sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
				free(sprava_na_poslanie);
			}
			else if(*recieve_buffer == 14){
				sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 14);	//typ spravy je 14, teda stopka
				sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
				printf("Dostal som informaciu o konci aktualneho posielania, ukoncujem a posielam potvrdenie\n");
				free(sprava_na_poslanie);
				printf("Pocet datovych fragmentov je %d, z toho %d chybnych\n", doimplementacia, doimplementacia_chybne);
				return;
			}
		}
	}
	else if(*recieve_buffer == 13){
		printf("Pojde o posielanie suborov\n");
		int dlzka_prijatej_spravy;
		while(1){
			//najskor prijmem fragment(y) s nazvom suboru
			recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
			char buffer_na_cestu_ulozenia[800], buffer_na_nazov_suboru[400];
			int index = 0;
			short velkost_dat;

			if(*recieve_buffer == 5){
				//mozno bude treba vyskladat nazov z viacerych fragmentov
				while(*recieve_buffer == 5){
					if(skontroluj_CRC_v_sprave(recieve_buffer) != 0){
						//printf("CRC hodnoty sedia\n");
					}
					else{
						printf("CRC nesedia, posielam ziadost o znovuposlanie fragmentu cislo %d\n", *((int *)(recieve_buffer + 1)));
						sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 4);
						sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
						recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
						continue;
					}


					velkost_dat = *((short *)(recieve_buffer + 5));
					for(int i = 0; i < velkost_dat; i++){
						buffer_na_nazov_suboru[index++] = recieve_buffer[VELKOST_HLAVICKY + i];
					}
					sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), VELKOST_HLAVICKY, 5);
					sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
					free(sprava_na_poslanie);

					recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
				}
				buffer_na_nazov_suboru[index] = 0;
				printf("Nazov suboru je %s\nZadaj cestu priecinku a zahajim prenos\n", buffer_na_nazov_suboru, recieve_buffer + VELKOST_HLAVICKY);
				scanf("\n");
				gets(buffer_na_cestu_ulozenia);
				if(buffer_na_cestu_ulozenia[strlen(buffer_na_cestu_ulozenia) - 1] == '\\')
					sprintf(buffer_na_cestu_ulozenia, "%s%s", buffer_na_cestu_ulozenia, buffer_na_nazov_suboru);
				else
					sprintf(buffer_na_cestu_ulozenia, "%s\\%s", buffer_na_cestu_ulozenia, buffer_na_nazov_suboru);
				//printf("Absolutna cesta ulozenia je %s\n", buffer_na_cestu_ulozenia);
				sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), VELKOST_HLAVICKY, 9);	//posielam potvrdenie o doruceni udajov o subore
				sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
				free(sprava_na_poslanie);
			}
			else{
				printf("Chyba, dostal som neocakavany fragment s inym typom\n");
			}


			//samotny prenos suboru
			unsigned long long int pocet_dat_v_B = 0;
			int pocet_prijatych_fragmentov_s_datami = 0;

			FILE *subor_na_zapis = fopen(buffer_na_cestu_ulozenia, "wb");
			if(subor_na_zapis)
				printf("Subor sa podarilo otvorit\n");
			else
				printf("Subor sa nepodarilo otvorit\n");
			while(1){
				recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
				if(*recieve_buffer == 2){
					pocet_prijatych_fragmentov_s_datami++;
					if(skontroluj_CRC_v_sprave(recieve_buffer) != 0){
						//printf("CRC hodnoty sedia\n");
					}
					else{
						printf("CRC nesedia, posielam ziadost o znovuposlanie fragmentu cislo %d\n", *((int *)(recieve_buffer + 1)));

						doimplementacia_chybne++;

						sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 4);
						sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
						continue;
					}
						
					dlzka_prijatej_spravy = *((short *)(recieve_buffer + 5));
					pocet_dat_v_B += dlzka_prijatej_spravy;
					for(int i = 0; i < dlzka_prijatej_spravy; i++)
						fwrite(&(recieve_buffer[VELKOST_HLAVICKY + i]), 1, 1, subor_na_zapis);

					printf("\nPrijal som %dB dat v sprave s ID %d a posielam spat potvrdenie o prijati\n", dlzka_prijatej_spravy, *((int *)(recieve_buffer + 1)));
					sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 3);	//typ spravy je 3, teda potvrdenie
					sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
					free(sprava_na_poslanie);
				}
				else if(*recieve_buffer == 9){
					sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer + 1)), 0, 9);	//typ spravy je 9, teda potvrdenie zavretia suboru
					sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
					fclose(subor_na_zapis);
					printf("Dostal som ziadost o zavretie suboru, zatvaram a posielam potvrdenie\n");
					free(sprava_na_poslanie);
					printf("Subor prisiel v %d fragmentoch, z toho %d chybne, obsahujucich dokopy %llu B dat\n", pocet_prijatych_fragmentov_s_datami, doimplementacia_chybne, pocet_dat_v_B);


					printf("Absolutna cesta ulozenia je %s\n", buffer_na_cestu_ulozenia);
					return;
				}
			}
		}
	}
	else{
		printf("Prisiel necakany fragment\n");
	}

}

void menu(int mod_programu, SOCKET moj_socket, struct sockaddr *adresa, int size_of_adresa){
	char *sprava_na_poslanie, recieve_buffer[1500];
	int ID_fragmentu = 0;

	while(1){
		if(mod_programu == 1){
			printf("Kto bude odosielatel?\n1  ->  ja\n2  ->  druha strana\n3  ->  koniec spojenia\n");
			scanf("%d", &mod_programu);
			if(mod_programu == 1)
				sprava_na_poslanie = priprav_spravu(++ID_fragmentu, 0, 10);
			else if(mod_programu == 2)
				sprava_na_poslanie = priprav_spravu(++ID_fragmentu, 0, 11);
			else if(mod_programu == 3)
				sprava_na_poslanie = priprav_spravu(++ID_fragmentu, 0, 7);
			else{
				mod_programu = 1;
				continue;
			}
			sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
			recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
			free(sprava_na_poslanie);
			if(mod_programu == 3){
				closesocket(moj_socket);
				return;
			}
		}
		else if(mod_programu == 2){
			//pocuvam, aby som vedel, kto bude odosielatel
			recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
			if(*recieve_buffer == 10){
				mod_programu = 2;
				printf("Budem prijimat, posielam spat potvrdenie\n");
			}
			else if(*recieve_buffer == 11){
				mod_programu = 1;
				printf("Budem odosielat, posielam spat potvrdenie\n");
			}
			else if(*recieve_buffer == 7){
				printf("Ziada sa ukoncenie spojenia, posielam odpoved a ukoncujem\n");
			}
			else{
				printf("Dosiel neocakavany fragment\n");
				continue;
			}
			sprava_na_poslanie = priprav_spravu(++ID_fragmentu, 0, *recieve_buffer);
			sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, adresa, size_of_adresa);
			free(sprava_na_poslanie);
			if(*recieve_buffer == 7){
				closesocket(moj_socket);
				return;
			}
		}
		if(mod_programu == 1)
			posli(moj_socket, adresa, size_of_adresa);
		else if(mod_programu == 2)
			prijmi(moj_socket, adresa, size_of_adresa);
	}
}

void funkcia_na_klient(){
	SOCKET moj_socket;
    // 2 predstavuje IPv4, AF_INET family
    // 2 pre udp protocol
    moj_socket = socket(2, 2, 17);
    if(moj_socket == INVALID_SOCKET){
        printf("Chybny socket\n");
        exit(2);
    }

	struct sockaddr_in server;

	char ip_adresa[50];
	printf("Zadaj IP adresu serveru (napr. 192.168.0.248)\n");
	scanf("%s", ip_adresa);

	server.sin_addr.S_un.S_addr = inet_addr(ip_adresa);	//IP SA BUDE MUSIET DAT VYBERAT
	server.sin_family = 2;

	int port;
	printf("Zadaj port serveru (napr. 5002)\n");
	scanf("%d", &port);
	server.sin_port = htons(port);		//PORT SA BUDE MUSIET DAT VYBERAT

	int  max_velkost_uzivatela;

	//vytvorim spojenie
	if(connect(moj_socket, (struct sockaddr *)&server , sizeof(server))){
		printf("Connect error\n");
		exit(1);
	}
	bind(moj_socket, (struct sockaddr *)&server , sizeof(server));
	char *sprava_na_poslanie, recieve_buffer[1500];
	struct sockaddr adresa = *((struct sockaddr *)&server);
	int ID_fragmentu = 0, size_of_adresa = sizeof(adresa);
	sprava_na_poslanie = priprav_spravu(++ID_fragmentu, 0, 1);
	send(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0);
	recv(moj_socket, recieve_buffer, sizeof(recieve_buffer), 0);
	free(sprava_na_poslanie);
	if(*recieve_buffer == 1)
		printf("Spojenie bolo inicializovane\n");
	else
		printf("Dosiel fragment neocakavaneho typu\n");


	int mod_programu = 1;

	menu(mod_programu, moj_socket, &adresa, size_of_adresa);
}

void funkcia_na_server(){
	SOCKET moj_socket;
    // 2 predstavuje IPv4, AF_INET family
    // 2 pre udp protocol
    moj_socket = socket(2, 2, 17);
    if(moj_socket == INVALID_SOCKET){
        printf("Chybny socket\n");
        exit(2);
    }

	struct sockaddr_in server;

	char ip_adresa[50];
	printf("Nastav IP adresu serveru (napr. 192.168.0.248)\n");
	scanf("%s", ip_adresa);
	server.sin_addr.S_un.S_addr = inet_addr(ip_adresa);	//IP SA BUDE MUSIET DAT VYBERAT
	server.sin_family = 2;

	int port;
	printf("Nastav port serveru (napr. 5002)\n");
	scanf("%d", &port);
	server.sin_port = htons(5002);		//PORT SA BUDE MUSIET DAT VYBERAT
	bind(moj_socket, (struct sockaddr *)&server , sizeof(server));

	char recieve_buffer[1500], *sprava_na_poslanie;
	struct sockaddr adresa;
	int size_of_adresa = sizeof(adresa), ID_fragmentu;
	recvfrom(moj_socket, recieve_buffer, 1500, 0, &adresa, &size_of_adresa);
	if(*recieve_buffer == 1)
		printf("Prisiel pociatocny fragment, robim spojenie a posielam odpoved\n");
	else
		printf("Prisiel fragment, avsak nie startovny\n");
	sprava_na_poslanie = priprav_spravu(*((int *)(recieve_buffer)), 0, 1);
	sendto(moj_socket, sprava_na_poslanie, VELKOST_HLAVICKY, 0, &adresa, size_of_adresa);
	free(sprava_na_poslanie);

	int mod_programu = 2;
	menu(mod_programu, moj_socket, &adresa, size_of_adresa);
}

int main()
{
	WSADATA kniznica;
	if (WSAStartup(MAKEWORD(2,2), &kniznica) != 0){
		printf("Problem s kniznicou\n");
		return 1;
	}

	char mod_programu;
	printf("Vyber si stranu programu:\n1  ->  Server\n2  ->  Klient\n");
	scanf("%d", &mod_programu);



	if(mod_programu == 1){
		funkcia_na_server();
	}
	else if(mod_programu == 2){
		funkcia_na_klient();
	}

	printf("Koooniec\n");
	return 0;
}