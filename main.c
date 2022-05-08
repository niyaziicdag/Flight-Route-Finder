#include <stdio.h>
#include <stdlib.h>

struct sehir{
	char isim[20];
	struct sehir* gidilenSehir;
	int dk;
	int fiyat;
	struct sehir* next;
};
struct ucuslar{
	struct sehir baslangic;
	struct sehir hedef;
	int dk;
	int fiyat;
};
struct node{
	struct sehir city;
	struct node* next;
};
int sehirBul(struct sehir*, struct node*);
struct node* aktifSehir(struct node*, struct ucuslar*, int);
void hedefEkle(struct sehir*, struct sehir*, int, int);
void rotaBul(struct node*, struct ucuslar*, int);
void ekranTemizle();
void bekle();
void menu();
struct ucuslar* ucuslarOku(FILE *f,char file_name[256], struct ucuslar* u_liste){
	int ch;
	int tmp;
	int i=0;
	int j=0;
	if ((f = fopen(file_name, "r")) == NULL) {
		printf("Dosya okunamadi.\n");
		exit(EXIT_FAILURE);
	}
	ch = fgetc(f);
	while (ch != EOF){
		tmp=0;
		while(ch!=' '){//BAÞLANGIÇ ÞEHRÝ
			u_liste[i].baslangic.isim[j]=ch;
			ch = fgetc(f);
			j++;
		}
		u_liste[i].baslangic.isim[j]='\0';
		ch = fgetc(f);
		j=0;
		while(ch!=' '){//HEDEF ÞEHRÝ
			u_liste[i].hedef.isim[j]=ch;
			ch=fgetc(f);
			j++;
		}
		u_liste[i].hedef.isim[j]='\0';
		ch = fgetc(f);
		j=0;
		while(ch!=' '){//SAAT
			tmp = tmp * 10 + (ch-48);
			ch=fgetc(f);	
		}
		ch=fgetc(f);
		u_liste[i].dk=tmp*60;
		tmp=0;
		while(ch!=' '){//DAKIKA
			tmp = tmp * 10 + (ch-48);
			ch=fgetc(f);
		}
		ch=fgetc(f);
		u_liste[i].dk = u_liste[i].dk+tmp;
		tmp=0;
		while(ch!=' ' && ch!='\n' && ch!=EOF){//FÝYAT
			tmp=tmp*10+(ch-48);
			ch=fgetc(f);
		}
		u_liste[i].fiyat=tmp;
		ch=fgetc(f);
		i++;
	}
	fclose(f);
	return u_liste;
}
int ucusBul(char kalkis[20],char varis[20],int kriter,int aktarma,int sehirAdet,int toplam_ucus,struct ucuslar* u_liste){
	
	struct node* tmp;
	struct node* sehirler;

	sehirler = (struct node*)malloc(sizeof(struct node));
	sehirler = aktifSehir(sehirler, u_liste, toplam_ucus);
	tmp = sehirler;

	while(tmp->next!=NULL){
		sehirAdet++;
		tmp = tmp->next;
	}
	sehirAdet++;

	rotaBul(sehirler, u_liste, toplam_ucus);
	tmp=sehirler;
	struct sehir* tmpCity;
	int i=0,bulundu=0;
	while(i<sehirAdet){
		tmpCity = tmp->city.gidilenSehir;
		while(tmpCity->gidilenSehir!=NULL){
			printf("ic=%s - %s\n",tmp->city.isim,tmpCity->isim);
			if(strcmp(tmp->city.isim,kalkis)==0 && strcmp(tmpCity->isim,varis)==0){
				printf("Ucus Bulundu.\n %s --> %s = Sure: %d dakika, Fiyat : %d TL\n",tmp->city.isim,tmpCity->isim,tmpCity->dk,tmpCity->fiyat);
				bulundu=1;
				menu();
			}
			else if(strcmp(tmp->city.isim,varis)==0 && strcmp(tmpCity->isim,kalkis)==0){
				printf("Ucus Bulundu.\n %s --> %s = Sure: %d dakika, Fiyat : %d TL\n",tmpCity->isim,tmp->city.isim,tmpCity->dk,tmpCity->fiyat);
				bulundu=1;	
				menu();
			}
			tmpCity = tmpCity->gidilenSehir;
		}
		i++;
		tmp = tmp->next;
	}
	return bulundu;
}
int main()
{
	 while (1) menu();
	return 0;
}

void menu() {
	FILE *f;
	char file_name[256]="sample.txt";
	int toplam_ucus;
	int ch,i;
	if ((f = fopen(file_name, "r")) == NULL) {
		printf("Dosya okunamadi.\n");
		exit(EXIT_FAILURE);
	}
	while ((ch = fgetc(f)) != EOF)//Toplam uçuþ adedi alýnýr.
		if(ch==10)
			toplam_ucus++;
	toplam_ucus++;
	fclose(f);

	struct ucuslar* u_liste;
	struct node* tmp;
	struct node* sehirler;

	u_liste = (struct ucuslar*)malloc(toplam_ucus*sizeof(struct ucuslar));//DOSYADAN UÇUÞ LÝSTESÝ UÇUÞLAR STRUCTINA EKLENÝR
	u_liste = ucuslarOku(f,file_name, u_liste);
	for(i=0;i<toplam_ucus;i++){
		printf("%s %s %d %d\n", u_liste[i].baslangic.isim, u_liste[i].hedef.isim, u_liste[i].dk, u_liste[i].fiyat);
	}
	sehirler = (struct node*)malloc(sizeof(struct node));
	sehirler = aktifSehir(sehirler, u_liste, toplam_ucus);//UÇUÞ LÝSTESÝNDEN ÞEHÝRLER BÝR STRUCTA ÇEKÝLÝR

	tmp = sehirler;
	printf("\n");
	int sehirAdet=0;
	while(tmp->next!=NULL){
		sehirAdet++;
		tmp = tmp->next;
	}
	sehirAdet++;

	rotaBul(sehirler, u_liste, toplam_ucus);
	tmp=sehirler;
	struct sehir* tmpCity;
	i=0;
	while(i<sehirAdet){
		printf("Kalkis: %s \n",tmp->city.isim);
		tmpCity = tmp->city.gidilenSehir;
		while(tmpCity->gidilenSehir!=NULL){
			printf("%s Sure: %d dk, Fiyat: %d TL\n", tmpCity->isim, tmpCity->dk, tmpCity->fiyat);
			tmpCity = tmpCity->gidilenSehir;
		}
		printf("%s Sure: %d dk, Fiyat: %d TL", tmpCity->isim, tmpCity->dk, tmpCity->fiyat);
		printf("\n\n");
		i++;
		tmp = tmp->next;
	}
    int secim,kriter,aktarma;
    char kalkis[20],varis[20];

    printf("1-Arama yapma\n");
    printf("2-Cikis\n");
    printf("Lutfen Secim Yapiniz --> ");
    scanf("%d", &secim);
	int bulundu=0;
    switch (secim) {
        case 1:
            //ekranTemizle();
			printf("Kalkis noktasi giriniz :");
			scanf("%s",kalkis);
			printf("Varis noktasi giriniz :");
			scanf("%s",varis);
			printf("Aktarma sayisi giriniz (Aktarmasiz :0) :");
			scanf("%d",&aktarma);
			printf("Arama kriteri giriniz (Sure :0 - Fiyat :1) :");
			scanf("%d",&kriter);
			printf("\n");
			bulundu=ucusBul(kalkis,varis,kriter,aktarma,sehirAdet,toplam_ucus,u_liste);
			if(bulundu==0){
				printf("Aranilan ucus bulunamadi.\n");	
			}
            bekle();
            break;
        case 2:
            ekranTemizle();
            printf("Program Kapatildi!\n");
            exit(0);
            break;
        default:
            ekranTemizle();
            printf("Hatali Secim\n");
            bekle();
    }
}
void ekranTemizle() {
    system("cls");
}
void bekle() {
    char temp;
    printf("Devam etmek icin Enter'a basiniz!\n");
    temp = getchar();
    temp = getchar();
    ekranTemizle();
 
}
int sehirBul(struct sehir* city, struct node* cities){
	int i;
	struct node* tmp=cities;
	while(tmp->next!=NULL){
		if(strcmp(city->isim,tmp->city.isim)==0){
			return 0;
		}
		tmp=tmp->next;
	}
	if(strcmp(city->isim,tmp->city.isim)==0){
			return 0;
	}
	else{
		struct node* newNode = (struct node*)malloc(sizeof(struct node));
		newNode->city = *city;
		newNode->city.gidilenSehir = NULL;
		newNode->next = NULL;
		tmp->next = newNode;
	return 1;
	}
}

struct node* aktifSehir(struct node* cities, struct ucuslar* u_liste, int ucus_N){
	int i;
	for(i=0;i<ucus_N;i++){
		if(i==0){
			cities->city = u_liste[i].baslangic;
			cities->city.gidilenSehir = NULL;
			struct node* newNode = (struct node*)malloc(sizeof(struct node));
			newNode->city = u_liste[i].hedef;
			newNode->next = NULL;
			cities->next = newNode;
			cities->next->city.gidilenSehir = NULL;
		}
		else{
			sehirBul(&u_liste[i].baslangic, cities);
			sehirBul(&u_liste[i].hedef, cities);
		}
	}
	return cities;
}

void hedefEkle(struct sehir* source, struct sehir* dest, int dk, int fiyat){
	struct sehir* tmp = source;
	while(tmp->gidilenSehir!=NULL){
		tmp = tmp->gidilenSehir;
	}
	tmp->gidilenSehir=dest;
	tmp->gidilenSehir->gidilenSehir=NULL;
	dest->dk = dk;
	dest->fiyat = fiyat;
	return;
}

void rotaBul(struct node* cities, struct ucuslar* u_liste, int ucus_N){
	int i;
	struct node* tmp;
	for(i=0; i<ucus_N; i++){
		tmp=cities;
		while(tmp->next!=NULL){
			if(strcmp(u_liste[i].baslangic.isim,tmp->city.isim)==0){
				hedefEkle(&tmp->city, &u_liste[i].hedef, u_liste[i].dk, u_liste[i].fiyat);
			}	
			tmp = tmp->next;
		}
		if(strcmp(u_liste[i].baslangic.isim,tmp->city.isim)==0){
				hedefEkle(&tmp->city, &u_liste[i].hedef, u_liste[i].dk, u_liste[i].fiyat);
		}
		tmp=cities;
		while(tmp->next!=NULL){
			if(strcmp(u_liste[i].hedef.isim,tmp->city.isim)==0){
				hedefEkle(&tmp->city, &u_liste[i].baslangic, u_liste[i].dk, u_liste[i].fiyat);
			}
			tmp = tmp->next;
		}
		if(strcmp(u_liste[i].hedef.isim,tmp->city.isim)==0){
				hedefEkle(&tmp->city, &u_liste[i].baslangic,  u_liste[i].dk, u_liste[i].fiyat);
		}
	}
}
