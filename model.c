#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int shoot[25][25];
GtkWidget *win;
GtkWidget *buttons[16][16],*buttonsvt[16][16], *battlebuttons[25][25];
int whichpick=0;
int posl=14;
int ptsl=14;
int player=1;
char t[]= "Now turn Player 1";
char tt[]="Player 1 won!";
char ttt[]="Player 1";
// Fieldstate {shot, miss};
int orientation=-1;
int okpick;
static gint windows_counter;
int shiplength=-1;
struct pair{
    int first;
    int second;
    char third;
    int fourth;
};
char* who="1";
struct pairbat{
    int first;
    int second;
    char third;
};
void chooseship( GtkWidget *widget, struct pair *mydata);
void sprawdz_traf( GtkWidget *widget, struct pair *mydata);
void destro(GtkWidget *widget,gpointer data);
int lastcallx,lastcally;
//GtkWidget* buttons[10][10];
void destcount(GtkWidget* window, gpointer data);
void dalej( GtkWidget *widget, gpointer data);

void winner(char* pl){
	GtkWidget* dow =gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dow),"Congratulation!");
    gtk_window_set_position(GTK_WINDOW(dow),GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(dow),"destroy",G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(dow), 10);
    GtkWidget *box1 =gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(dow), box1);
	tt[7]=*pl;
    GtkWidget *label = gtk_label_new (tt);
    g_object_set (label, "margin", 20, NULL);
    gtk_box_pack_start (GTK_BOX(box1), label,TRUE, TRUE,0);
    GtkWidget *button=gtk_button_new_with_label("Close");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(gtk_main_quit),NULL);
	g_object_set (button, "margin", 20, NULL);
    gtk_box_pack_start (GTK_BOX(box1), button,TRUE, TRUE,0);
    gtk_widget_show_all(dow);
}

void cantpick(char* mess);
void whonext(char* pla){
	windows_counter++;
	GtkWidget* dow =gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dow),"Turn");
    gtk_window_set_position(GTK_WINDOW(dow),GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(dow),"destroy",G_CALLBACK(destro), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(dow), 10);
    GtkWidget *box1 =gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(dow), box1);
	t[16]=*pla;
    GtkWidget *label = gtk_label_new (t);
    g_object_set (label, "margin", 20, NULL);
    gtk_box_pack_start (GTK_BOX(box1), label,TRUE, TRUE,0);
    GtkWidget *button=gtk_button_new_with_label("Next!");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(destro),NULL);
	g_object_set (button, "margin", 20, NULL);
    gtk_box_pack_start (GTK_BOX(box1), button,TRUE, TRUE,0);
    gtk_widget_show_all(dow);
}

void createbattle(){
	windows_counter++;
	GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"Battleship");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 0);
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER(window), grid);
	GtkWidget* name=gtk_label_new ("Player 2");
	gtk_grid_attach(GTK_GRID(grid), name, 16, 0, 4, 1);
	GtkWidget* namet=gtk_label_new ("Player 1");
	gtk_grid_attach(GTK_GRID(grid), namet, 4, 0, 4, 1);
	for(int i=1;i<=10;i++){
		for(int j=1;j<=10;j++){
			battlebuttons[i][j]=gtk_button_new_with_label("");
			struct pairbat* para =malloc(sizeof(struct pairbat));
			para->first=i;
			para->second=j;
			//para->third='';
			g_signal_connect(G_OBJECT(battlebuttons[i][j]), "clicked",G_CALLBACK(sprawdz_traf),para);
			gtk_grid_attach(GTK_GRID(grid), battlebuttons[i][j], j, i, 1, 1);
		}
	}
	for(int i=11;i<=12;i++){
		for(int j=1;j<=10;j++){
			GtkWidget* row=gtk_label_new("|");
			gtk_grid_attach(GTK_GRID(grid), row, i, j, 1, 1);
		}
	}
	char letter[]="A";
	for(int j=1;j<=10;j++){
		GtkWidget *lab=gtk_label_new (letter);
		gtk_grid_attach(GTK_GRID(grid),lab,0,j,1,1);
		lab=gtk_label_new (letter);
		gtk_grid_attach(GTK_GRID(grid),lab,13,j,1,1);
		++(letter[0]);	
	}
	char digit[]="1";
	for(int j=1;j<=9;j++){
		GtkWidget *lab=gtk_label_new (digit);
		gtk_grid_attach(GTK_GRID(grid),lab,j,11,1,1);
		lab=gtk_label_new (digit);
		gtk_grid_attach(GTK_GRID(grid),lab,13+j,11,1,1);
		++(digit[0]);
	}
	GtkWidget *lab=gtk_label_new ("10");
	gtk_grid_attach(GTK_GRID(grid),lab,10,11,1,1);
	lab=gtk_label_new ("10");
	gtk_grid_attach(GTK_GRID(grid),lab,23,11,1,1);
	for(int i=1;i<=10;i++){
		for(int j=14;j<=23;j++){
			battlebuttons[i][j]=gtk_button_new_with_label("");
			struct pairbat* para =malloc(sizeof(struct pairbat));
			para->first=i;
			para->second=j;
			//para->third='';
			g_signal_connect(G_OBJECT(battlebuttons[i][j]), "clicked",G_CALLBACK(sprawdz_traf),para);
			gtk_grid_attach(GTK_GRID(grid), battlebuttons[i][j], j, i, 1, 1);
		}
	}


	gtk_widget_show_all(window);
	//
}

void createpick(GtkWidget *window){
	windows_counter++;
	orientation=-1;
	okpick=0;
	shiplength=-1;
	lastcallx=-1;
	lastcally=-1;
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"pickship");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destcount), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 0);
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER(window), grid);
	ttt[7]=player+'0';
	GtkWidget* name=gtk_label_new (ttt);
	gtk_grid_attach(GTK_GRID(grid), name, 8, 0, 4, 1);
	/*struct butto{
		gchar *opis; 
		gint posX, lenX, posY, lenY;
	}tab[];
	*/
	for(int i=6;i<=15;i++){
		for(int j=1;j<=10;j++){
			struct pair* para =malloc(sizeof(struct pair));
			para->first=i;
			para->second=j;
			//para->third="";
			para->fourth=player;
			if(player==1){
				buttons[i][j]=gtk_button_new_with_label("");
				g_signal_connect(G_OBJECT(buttons[i][j]), "clicked",G_CALLBACK(chooseship),para);
				gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 1);
			}
			else{
				buttonsvt[i][j]=gtk_button_new_with_label("");
				g_signal_connect(G_OBJECT(buttonsvt[i][j]), "clicked",G_CALLBACK(chooseship),para);
				gtk_grid_attach(GTK_GRID(grid), buttonsvt[i][j], i, j, 1, 1);
			}
		}
	}
	for(int i=0;i<=3;i++){
		for(int j=i;j<=i+1;j++){
			int mom=5-i;
			char res[]= {mom+'0', '\0'};
			struct pair* para =malloc(sizeof(struct pair));
			para->first=i;
			para->second=j;
			para->third=res[0];
			para->fourth=player;
			if(player==1){
				buttons[i][j]=gtk_button_new_with_label(res);
				g_signal_connect(G_OBJECT(buttons[i][j]), "clicked",G_CALLBACK(chooseship),para);
				if(i==j){
					gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 5-i, 1);
				}
				else{
					gtk_grid_attach(GTK_GRID(grid), buttons[i][j], i, j, 1, 5-i);	
				}
			}
			else{
				buttonsvt[i][j]=gtk_button_new_with_label(res);
				g_signal_connect(G_OBJECT(buttonsvt[i][j]), "clicked",G_CALLBACK(chooseship),para);
				if(i==j){
					gtk_grid_attach(GTK_GRID(grid), buttonsvt[i][j], i, j, 5-i, 1);
				}
				else{
					gtk_grid_attach(GTK_GRID(grid), buttonsvt[i][j], i, j, 1, 5-i);	
				}
			}

		}
	}	
	GtkWidget *butt=gtk_button_new_with_label("Dalej");
	//int *point=player;
	char playe[]={player+'0'};
	g_signal_connect(G_OBJECT(butt), "clicked",G_CALLBACK(dalej),NULL);
	gtk_grid_attach(GTK_GRID(grid), butt, 9, 12, 3, 1);
	char letter[]="A";
	for(int i=1;i<=10;i++){
		GtkWidget *lab=gtk_label_new (letter);
		gtk_grid_attach(GTK_GRID(grid),lab,5,i,1,1);
		++(letter[0]);
	}
	char digit[]="1";
	for(int j=1;j<=9;j++){
		GtkWidget *lab=gtk_label_new (digit);
		gtk_grid_attach(GTK_GRID(grid),lab,j+5,11,1,1);
		++(digit[0]);
	}
	GtkWidget *lab=gtk_label_new ("10");
	gtk_grid_attach(GTK_GRID(grid),lab,15,11,1,1);
	gtk_widget_show_all(window);
}

int main(int argc, char *argv[]){
	gtk_init(&argc, &argv);
	createpick(NULL);
		//createpickvt();
	gtk_main();
}

void chooseship( GtkWidget *widget, struct pair *mydata){

	if(mydata->first<5 && mydata->first==mydata->second){
		gchar* porownaj="X";
		if(mydata->fourth==1){
			const gchar *moment=gtk_button_get_label(GTK_BUTTON(buttons[mydata->first][mydata->second]));
			if(*porownaj==*moment){
				cantpick("You can't pick that ship again");
				return;
			}
		}
		else{
			const gchar *momen=gtk_button_get_label(GTK_BUTTON(buttonsvt[mydata->first][mydata->second]));
			if(*porownaj==*momen){
				cantpick("You can't pick that ship again");
				return;
			}
		}

		orientation=0;
		lastcally=mydata->second;
		lastcallx=mydata->first;
		shiplength=mydata->third-'0';

	}
	if(mydata->first<5 && mydata->first!=mydata->second){
		gchar* porownaj="X";
		if(mydata->fourth==1){
			const gchar *moment=gtk_button_get_label(GTK_BUTTON(buttons[mydata->first][mydata->second]));
			if(*porownaj==*moment){
				cantpick("You can't pick that ship again");
				return;
			}
		}
		else{
			const gchar *momen=gtk_button_get_label(GTK_BUTTON(buttonsvt[mydata->first][mydata->second]));
			if(*porownaj==*momen){
				cantpick("You can't pick that ship again");
				return;
			}
		}
		orientation=1;
		lastcally=mydata->second;
		lastcallx=mydata->first;
		shiplength=mydata->third-'0';
	}
	if(mydata->first>=6){

		if(orientation==0){
			if(mydata->first+shiplength-1<=15){
				int ok=1;
				for(int i=0;i<shiplength;i++){
						//gchar* moment;
					if(mydata->fourth==1){
						gchar* por="X";
						const gchar* moment=gtk_button_get_label(GTK_BUTTON(buttons[mydata->first+i][mydata->second]));
						if(*moment==*por){
							ok=0;
							break;
						}
					}
					else{
						gchar* por="X";
						const gchar* moment=gtk_button_get_label(GTK_BUTTON(buttonsvt[mydata->first+i][mydata->second]));
						if(*moment==*por){
							ok=0;
							break;
						}
					}
					
					//printf("%s\n",moment);
									
				}
				if(ok==1){
					for(int i=0;i<shiplength;i++){
						if(mydata->fourth==1){
							gtk_button_set_label(GTK_BUTTON(buttons[mydata->first+i][mydata->second]),"X");
							shoot[mydata->second][mydata->first+i-5]=1;				
						}
						else{
							gtk_button_set_label(GTK_BUTTON(buttonsvt[mydata->first+i][mydata->second]),"X");
							shoot[mydata->second][mydata->first+i+8]=1;		
						}
					}
					okpick++;
					if(mydata->fourth==1){
						gtk_button_set_label(GTK_BUTTON(buttons[lastcallx][lastcally]),"X");
						gtk_button_set_label(GTK_BUTTON(buttons[lastcallx][lastcally+1]),"X");
					}
					else{
						gtk_button_set_label(GTK_BUTTON(buttonsvt[lastcallx][lastcally]),"X");
						gtk_button_set_label(GTK_BUTTON(buttonsvt[lastcallx][lastcally+1]),"X");
					}
					lastcally=-1;
					lastcallx=-1;
					orientation=-1;			
					shiplength=-1;
				}
				else
					cantpick("You can't pick ship in that way");
			}
			else{
				cantpick("You can't pick ship in that way");
			}
		}
		if(orientation==1){
			if(mydata->second+shiplength-1<=10){
				int ok=1;
				for(int i=0;i<shiplength;i++){
					if(mydata->fourth==1){
						gchar* por="X";
						const gchar* moment=gtk_button_get_label(GTK_BUTTON(buttons[mydata->first][mydata->second+i]));
						if(*moment==*por){
							ok=0;
							break;
						}
					}
					else{
						gchar* por="X";
						const gchar* moment=gtk_button_get_label(GTK_BUTTON(buttonsvt[mydata->first][mydata->second+i]));
						if(*moment==*por){
							ok=0;
							break;
						}
					}
					
					
				}
				if(ok==1){
					for(int i=0;i<shiplength;i++){
						if(mydata->fourth==1){
							gtk_button_set_label(GTK_BUTTON(buttons[mydata->first][mydata->second+i]),"X");
							shoot[mydata->second+i][mydata->first-5]=1;
						}
						else{
							gtk_button_set_label(GTK_BUTTON(buttonsvt[mydata->first][mydata->second+i]),"X");
							shoot[mydata->second+i][mydata->first+8]=1;
						}
					}
					okpick++;
					if(mydata->fourth==1){
						gtk_button_set_label(GTK_BUTTON(buttons[lastcallx][lastcally]),"X");
						gtk_button_set_label(GTK_BUTTON(buttons[lastcallx][lastcally-1]),"X");
					}
					else{
						gtk_button_set_label(GTK_BUTTON(buttonsvt[lastcallx][lastcally]),"X");
						gtk_button_set_label(GTK_BUTTON(buttonsvt[lastcallx][lastcally-1]),"X");
					}
					lastcally=-1;
					lastcallx=-1;
					orientation=-1;
					shiplength=-1;
				}
				else{
					cantpick("You can't pick ship in that way");
				}
			}
			else{
				cantpick("You can't pick ship in that way");
			}
		}
	}
		
}
void sprawdz_traf( GtkWidget *widget, struct pair *mydata){
	if(who=="1"){
		if(mydata->second<11){
			cantpick("Not here!");
		}
		else{
			if(shoot[mydata->first][mydata->second]==0){
				gtk_button_set_label(GTK_BUTTON(battlebuttons[mydata->first][mydata->second]),"N");
				whonext("2");
				who="2";
			}
			if(shoot[mydata->first][mydata->second]==1){
				gchar* por="X";
				const gchar* moment=gtk_button_get_label(GTK_BUTTON(battlebuttons[mydata->first][mydata->second]));
				if(*moment==*por){
					cantpick("You've already selected it!");
					whonext("1");	
				}
				else{	
					gtk_button_set_label(GTK_BUTTON(battlebuttons[mydata->first][mydata->second]),"X");
					posl--;
					if(posl==0){
						winner("2");
					}
					else
						whonext("1");
				}
			}
		}
	}
	else{
		if(mydata->second>13){
			cantpick("Not here!");
		}
		else{
			if(shoot[mydata->first][mydata->second]==0){
				gtk_button_set_label(GTK_BUTTON(battlebuttons[mydata->first][mydata->second]),"N");
				whonext("1");
				who="1";
			}
			if(shoot[mydata->first][mydata->second]==1){
				gchar* por="X";
				const gchar* mom=gtk_button_get_label(GTK_BUTTON(battlebuttons[mydata->first][mydata->second]));
				if(*mom==*por){
					cantpick("You've already selected it!");
					whonext("2");	
				}
				else{	
					gtk_button_set_label(GTK_BUTTON(battlebuttons[mydata->first][mydata->second]),"X");
					posl--;
					if(posl==0){
						winner("2");
					}
					else
						whonext("2");
				}
			}
		}
	}
		
}

void dalej(GtkWidget *widget, gpointer data){
	if(okpick==4){
		if(player==1){
			player++;
			createpick(NULL);
			GtkWidget *toplev=gtk_widget_get_toplevel (widget);   
			gtk_window_close(GTK_WINDOW(toplev));
		}
		else{
			createbattle(NULL);
			whonext(who);
		}
	}
	else{
		cantpick("Pick all ships!");
	}
}

void cantpick(char* mess){
	windows_counter++;
	GtkWidget *dow =gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dow),"Error");
    gtk_window_set_position(GTK_WINDOW(dow),GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(dow),"destroy",G_CALLBACK(destcount), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(dow), 10);
    GtkWidget *box1 =gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(dow), box1);
    GtkWidget *label = gtk_label_new ("Error");
    g_object_set (label, "margin", 20, NULL);
    gtk_box_pack_start (GTK_BOX(box1), label,TRUE, TRUE,0);
    GtkWidget *button=gtk_button_new_with_label(mess);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(destro),NULL);
	g_object_set (button, "margin", 20, NULL);
    gtk_box_pack_start (GTK_BOX(box1), button,TRUE, TRUE,0);
    gtk_widget_show_all(dow);
}
void destro(GtkWidget *widget,gpointer data){
	GtkWidget *toplev=gtk_widget_get_toplevel (widget);   
	gtk_window_close(GTK_WINDOW(toplev));
}
void destcount(GtkWidget* window, gpointer data){
	windows_counter--;
	if(windows_counter==0)
		gtk_main_quit();

}