#include <cairo.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "wrapper.h"
#include <unistd.h>
/* #include "wrapper.c" */

#define CYCLE_TIME 100
//#define QUEUE_NAME "/mq1"
#define MASS_MAX 100
#define DT 5

double G_CONST = 0.000667259;

static void do_drawing(cairo_t *);

int x = 0;
int y = 0;
int x2 = 0;
GtkWidget *window;
GtkWidget *darea;

typedef struct arg_struct{
    planet_type planetToCreate;
    planet_type *headOfList;
}listArgs;

void addToList(void *planetArgs){
    listArgs *args = planetArgs;
    planet_type *tempo = args->headOfList;

    while(tempo->next != NULL){
        printf("NODE: %s", args->headOfList->next->name);
        tempo = tempo->next;
    }
    planet_type *temp1 = malloc(sizeof(planet_type));

    if(temp1 == NULL){
        printf("COULD NOT ALLOCATE MARMARMOY!!\n");
        return;
    }
    *temp1 = args->planetToCreate;
    tempo->next = temp1;
}

void removeFromList(void *planetArgs) {
    listArgs *args = planetArgs;
    planet_type *tempo = args->headOfList; //keep track of where list starts

    while(strcmp(tempo->next->name, args->planetToCreate.name) != 0) {
        if(tempo->next == NULL) {
            printf("REMOVE ITEM NOT IN LIST\n");
            return;
        }
        tempo = tempo->next;
    }
    planet_type *temp1 = tempo->next;
    tempo->next = tempo->next->next;
    free(temp1);
}

void *planet(void *arguments){
    //traverse list until next=0, insert planet
    //while(1): calculate, update database
    //life = 0, call remove planet

    listArgs *args = arguments;

    if(args->planetToCreate.name[0] != 0){
        printf("MY NAME IS: %s\n", args->planetToCreate.name);
        //Lock DB
        pthread_mutex_lock(&mutex);
        addToList(args);
        pthread_mutex_unlock(&mutex);
    }
    else{
        printf("HEADNODE RECIEIIEVCJ\n");
    }
    while(1){
        //Do calculations
        double ax = 0;
        double ay = 0;
        planet_type *temp = args->headOfList;

        while(temp->next != NULL){
            pthread_mutex_lock(&mutex);
            if(strcmp(temp->next->name, args->planetToCreate.name) != 0){
                double dist = sqrt(pow(temp->next->sx - args->planetToCreate.sx, 2.0) + pow(temp->next->sy - args->planetToCreate.sy, 2));
                double a = G_CONST*(temp->next->mass/(dist*dist));

                ax += a*((temp->next->sx - args->planetToCreate.sx)/dist);
                ay += a*((temp->next->sy - args->planetToCreate.sy)/dist);
            }
            temp->next = temp->next->next;
        }

        args->planetToCreate.vx += ax*DT;
        args->planetToCreate.vy += ay*DT;

        args->planetToCreate.sx += args->planetToCreate.vx*DT;
        args->planetToCreate.sy += args->planetToCreate.vy*DT;

        if(args->planetToCreate.life <= 0){
            removeFromList(arguments);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

planet_type * initList(){
    //Init database
    planet_type *head = NULL;
    planet_type *temp = (planet_type*)malloc(sizeof(planet_type));

    if(temp==NULL){
        printf("SERVER: Could not allocate memory for head!\n");
        return 0;
    }
    head = temp;
    return head;
}

void *manageMail(void * u){
    //Open mailslot and return handle to client
    //Listen to mailslot for planet_create
    //Start planet_threads accordingly

    //Init mutex for linked list
    pthread_mutex_init(&mutex, NULL);

    //mq_unlink(QUEUE_NAME);//???
    mqd_t mq;
    planet_type msg;
    //try to open mailslot
    if(MQcreate(&mq, QUEUE_NAME) != 1){
        printf("SERVER: Could not create mailslot!\n");
        return NULL;
    }
    printf("SERVER: Mailslot created!\n");

    //init linked list
    planet_type * head = initList();

    //Create struct to send both head of list and planet to pthread_create
    listArgs createPlanetArg;
    createPlanetArg.planetToCreate.name[0] = 0;
    createPlanetArg.headOfList = head;

    pthread_t pl;
    //Continously read mailslot
    while(1){
        int mqRet;
        mqRet = MQread(&mq, (void*)&msg);
        if (mqRet >= 1){
            printf("SERVER: Request recieved, planet.name: %s\n", msg.name);
            if(strcmp(msg.name, "END") == 0){
                return MQclose(&mq, QUEUE_NAME);
            }
            //create planet thread
            msg.next = NULL;
            createPlanetArg.planetToCreate = msg;
            pthread_create(&pl, NULL, (void *(*)(void *)) &planet, (void *)&createPlanetArg);
        }
        else if (mqRet == 0){
            printf("SERVER: ERROR=%d\n", errno);
        }
    }
}

void*more_thread(void*args)
{

}
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, //Draw event for cairo, will be triggered each time a draw event is executed
                              gpointer user_data)
{
    do_drawing(cr); //Launch the actual draw method
    return FALSE; //Return something
}

planet_type testPlanet;

void drawPlanet(cairo_t *cr, planet_type *planet){

    //populate points
    size_t size = sizeof(planet->points)/sizeof(vector2D);

    for(int k = 0; k < size; k++){
        planet->points[k].x = planet->sx - planet->radius*cos((double)k/size*3.1415*2);
        //printf("x - %f\n", planet.radius*cos(k/size*3.1415*2));
        planet->points[k].y = planet->sy + planet->radius*sin((double)k/size*3.1415*2);
        //printf("coord of point %d: %f | %f \n", k, planet.points[k].x, planet.points[k].y);
    }
    //generate colors
    srand((unsigned int)time(NULL));

    float R = 0.0;//rand()/(float)RAND_MAX;
    float G = planet->mass / (float)MASS_MAX;
    float B = 1.0 - planet->mass / (float)MASS_MAX;

    //Cairo settings
    cairo_set_source_rgb(cr, R, G, B);
    cairo_set_line_width(cr, 1.2);

    //increment mult
    planet->mult += 0.02;
    if(planet->mult >size)
        planet->mult = 0.00;

    //draw circle then mult-table
    for(int i = 0; i < size; i++){
        cairo_move_to(cr, planet->points[i].x, planet->points[i].y);
        cairo_line_to(cr, planet->points[(i+1)%size].x, planet->points[(i+1)%size].y);
        int j = ((int)((i*planet->mult)) % size);
        cairo_move_to(cr, planet->points[i].x, planet->points[i].y);
        cairo_line_to(cr, planet->points[j].x, planet->points[j].y);
    }
    cairo_stroke(cr);

}

static void do_drawing(cairo_t *cr) //Do the drawing against the cairo surface area cr
{

    cairo_set_source_rgb(cr, 0, 0, 0); //Set RGB source of cairo, 0,0,0 = black
    x++; //dummy calculation
    y++;
    x2++;
    cairo_select_font_face(cr, "Purisa",
          CAIRO_FONT_SLANT_NORMAL,
          CAIRO_FONT_WEIGHT_BOLD);
    cairo_move_to(cr, 20, 30);
    cairo_show_text(cr, "You probably do not want to debug using text output, but you can");
    cairo_arc(cr, x,y,50,0,2*3.1415); //Create cairo shape: Parameters: Surface area, x pos, y pos, radius, Angle 1, Angle 2
    cairo_fill(cr);
    cairo_arc(cr, x2+100,0,25,0,2*3.1415); //These drawings are just examples, remove them once you understood how to draw your planets
    cairo_fill(cr);
    //Printing planets should reasonably be done something like this:
    // --------- for all planets in list:
    // --------- cairo_arc(cr, planet.xpos, planet.ypos, 10, 0, 2*3.1415)
    // --------- cairo_fill(cr)
    //------------------------------------------Insert planet drawings below-------------------------------------------
    //sleep(1);
    drawPlanet(cr, &testPlanet);
    //------------------------------------------Insert planet drawings Above-------------------------------------------

}
GtkTickCallback on_frame_tick(GtkWidget * widget, GdkFrameClock * frame_clock, gpointer user_data) //Tick handler to update the frame
{
    gdk_frame_clock_begin_updating (frame_clock); //Update the frame clock
    gtk_widget_queue_draw(darea); //Queue a draw event
    gdk_frame_clock_end_updating (frame_clock); //Stop updating frame clock
}

int main(int argc, char *argv[]) //Main function
{
    //----------------------------------------Variable declarations should be placed below---------------------------------
	pthread_t i_am_thread;
    pthread_t mailServer;
    //----------------------------------------Variable declarations should be placed Above---------------------------------

    //GUI stuff, don't touch unless you know what you are doing, or if you talked to me
    gtk_init(&argc, &argv); //Initialize GTK environment
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //Create a new window which will serve as your top layer
    darea = gtk_drawing_area_new(); //Create draw area, which will be used under top layer window
    gtk_container_add(GTK_CONTAINER(window), darea); //add draw area to top layer window
    g_signal_connect(G_OBJECT(darea), "draw",
                     G_CALLBACK(on_draw_event), NULL); //Connect callback function for the draw event of darea
    g_signal_connect(window, "destroy", //Destroy event, not implemented yet, altough not needed
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); //Set position of window
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600); //Set size of window
    gtk_window_set_title(GTK_WINDOW(window), "GTK window"); //Title
    gtk_widget_show_all(window); //Show window
    gtk_widget_add_tick_callback(darea, on_frame_tick, NULL, 1); //Add timer callback functionality for darea
    //GUI stuff, don't touch unless you know what you are doing, or if you talked to me


    //-------------------------------Insert code for pthreads below------------------------------------------------
    int ec = pthread_create(&i_am_thread, NULL, more_thread, (void*)0);

    int mS = pthread_create(&mailServer, NULL, manageMail, (void*)0);


    strcpy(testPlanet.name, "TestPlanet");
    testPlanet.sx = 400.0;
    testPlanet.sy = 400.0;
    testPlanet.vx = 0.0;
    testPlanet.vy = 0.0;
    testPlanet.mass = 80;
    testPlanet.life = 500;
    testPlanet.radius = 100;
    testPlanet.mult = 2.0;

    //-------------------------------Insert code for pthreads above------------------------------------------------


    gtk_main();//Call gtk_main which handles basic GUI functionality
    return 0;
}


