#include <cairo.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include "wrapper.h"
/* #include "wrapper.c" */

#define CYCLE_TIME 100
//#define QUEUE_NAME "/mq1"
#define MASS_MAX 100

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

    if(args->headOfList->next != NULL){
        printf("NODE: %s\n", args->headOfList->name);
        args->headOfList = args->headOfList->next;
        addToList(args);
    }
    else{
        printf("END OF LIST FOUND\n");
        args->headOfList->next = &(args->planetToCreate);
    }
}

void *planet(void *arguments){
    //traverse list until next=0, insert planet
    //while(1): calculate, update database
    //life = 0, call remove planet

    listArgs *args = arguments;

    if(args->planetToCreate.name[0] != 0){
        printf("MY NAME IS: %s\n", args->planetToCreate.name);
        addToList(args);
    }
    else{
        printf("HEADNODE RECIEIIEVCJ\n");
    }
    while(1){

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

    mqd_t mq;
    planet_type msg;
    //try to open mailslot
    if(MQcreate(&mq, QUEUE_NAME) != 1){
        printf("SERVER: Could not create mailslot!\n");
        return NULL;
    }
    printf("SERVER: Mailslot created!\n");

    //init semaphores
    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT | O_RDWR, 0, BUFFER_SIZE);
    sem_t *full = sem_open(SEM_FULL, O_CREAT | O_RDWR, 0, 0);
    /* sem_init(bEmpty, 0, BUFFER_SIZE); */

    if(empty == SEM_FAILED || full == SEM_FAILED){
        printf("SERVER: HAH SEM_FAILED U FKN N00B!!!\n");
    }
    int semValue;
    sem_getvalue(empty, &semValue);
    printf("empty_sem: %d\n", semValue);
    counter = 0;

    //init linked list
    planet_type * head = initList();

    //Create struct to send both head of list and planet to pthread_create
    listArgs createPlanetArg;
    createPlanetArg.planetToCreate.name[0] = 0;
    createPlanetArg.headOfList = head;

    pthread_t pl;
    //Continously read mailslot
    while(1){
        printf("SERVER: Waiting for request...\n");
        sem_wait(full);
        int mqRet = MQread(&mq, (void*)&msg);
        if (mqRet >= 1){
            printf("SERVER: Request recieved, planet.name: %s\n", msg.name);
            //create planet thread
            msg.next = NULL;
            createPlanetArg.planetToCreate = msg;

            pthread_create(&pl, NULL, &planet, (void *)&createPlanetArg);
        }
        else if (mqRet == 0){
            printf("SERVER: ERROR = %d\n", errno);
        }
        sem_post(empty);
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


