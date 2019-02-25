
#include "COMMON/wrapper.h"

#define CYCLE_TIME 100
//#define QUEUE_NAME "/mq1"

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

void addToList(listArgs *planetArgs){
    listArgs *args = planetArgs;
    planet_type *tempo = args->headOfList; //temporary pointer


    while(tempo->next != NULL){
        printf("NODE: %s\n", tempo->next->name);
        tempo = tempo->next;
    }
    planet_type *temp1 = malloc(sizeof(planet_type));
    if(temp1 == NULL){
        printf("COULD NOT ALLOCATE MAMORY\n");
        return;
    }
    *temp1 = args->planetToCreate;
    tempo->next = temp1;
}

void removeFromList(void *planetArgs) {
    listArgs *args = planetArgs;
    planet_type *tempo = args->headOfList; //temporary pointer

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


void *planet(listArgs *arguments){
    //traverse list until next=0, insert planet
    //while(1): calculate, update database
    //life = 0, call removeFromList()

    listArgs args;
    args.planetToCreate = arguments->planetToCreate;
    args.headOfList = arguments->headOfList;
    if(args.planetToCreate.name[0] != 0){
        printf("MY NAME IS: %s\n", args.planetToCreate.name);
        addToList(&args);

    }
    else{
        printf("HEADNODE RECIEIIEVCJ\n");
    }
    while(1){
        if(args.planetToCreate.life == 0) { //dead planet condition
            removeFromList(&args);
            break;
        }

        printf("PLANET: %s\n", args.planetToCreate.name); //test print
        sleep(1); //så att min dator inte brinner upp
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
    mq_unlink(QUEUE_NAME);
    mqd_t mq;
    planet_type msg;
    sem_t *sem_empty = sem_open(SEM_EMPTY, O_CREAT, 0644, BUFFER_SIZE);
    sem_t *sem_full = sem_open(SEM_FULL, O_CREAT, 0644, 0);
    sem_t *sem_mutex = sem_open(SEM_MUTEX, O_CREAT, 0644, 1);
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
    int x = 0;
    while(1){
        sem_wait(sem_full);
        sem_wait(sem_mutex);
        if (MQread(&mq, (void*)&msg) >= 1){
            //x++;
            printf("SERVER: Request recieved, planet.name: %s\n", msg.name);
            //create planet thread
            msg.next = NULL;
            createPlanetArg.planetToCreate = msg;

            pthread_create(&pl, NULL, (void *(*)(void *)) &planet, (void *)&createPlanetArg);
            //if(x == 3)
            //removeFromList((void *)&createPlanetArg);
        }
        sem_post(sem_mutex);
        sem_post(sem_empty);
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


    //-------------------------------Insert code for pthreads above------------------------------------------------


    gtk_main();//Call gtk_main which handles basic GUI functionality
    return 0;
}