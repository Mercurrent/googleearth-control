#include "sdlcontrol.h"

//keyboard and joystick button state
bool key_down[512];
bool joy_down[32];

char buffer[256];

//blah. This seemed about right
int SPEED = 200;

//global orientations
float roll, pitch, yaw, x, y, z = 0;

//server connection
int server = -1;

//entry
int main(int argc, char **argv){

    memset(&key_down, 0, sizeof(key_down));
    memset(&joy_down, 0, sizeof(joy_down));

    SDL_Surface *screen;
    SDL_Event event;
    
    printf("Connecting to server %s:%d\n", SERVER_ADDRESS, SERVER_PORT);
    
    server = connect_to_server(SERVER_ADDRESS, SERVER_PORT);
    
    if(server < 0){
        printf("Couldn't connect\n");
        return 1;
    }
    
    printf("connected. Opening window\n");
      
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 ){
        return 1;
    }
   
    //brings up a little 128x128 window, just for sending events to
    if (!(screen = SDL_SetVideoMode(128, 128, 16, 0))){        
        SDL_Quit();
        return 1;
    }
        
    SDL_JoystickEventState(SDL_ENABLE);
    
    SDL_Joystick *joystick = NULL;
    
    printf("%d joysticks\n", SDL_NumJoysticks());
  
    if(SDL_NumJoysticks() > 0){
          
        for( int i=0; i < SDL_NumJoysticks(); i++ )   {
            printf("Joystick: %s\n", SDL_JoystickName(i));
        }

        SDL_JoystickEventState(SDL_ENABLE);
        joystick = SDL_JoystickOpen(0);
    }
    
    bool loop = true;
    
    int constant_speed = 0;
    
    while(loop){
    
        int keyid = 0;
        int joyid = 0;

        while(SDL_PollEvent(&event)){      
            switch (event.type){
              case SDL_QUIT:
                loop = false;
              break;
              case SDL_KEYDOWN:
                   keyid = event.key.keysym.sym;
                   key_down[keyid] = true;     
                   //printf("%d %d down\n", keyid, SDLK_UP);                                    
                   break;
             case SDL_KEYUP:
                   keyid = event.key.keysym.sym;
                   key_down[keyid] = false;                       
                   break;
              case SDL_JOYBUTTONDOWN:
                   joyid = event.jbutton.button;
                   joy_down[joyid] = true;
                   break;

            case SDL_JOYBUTTONUP:
                   joyid = event.jbutton.button;
                   joy_down[joyid] = false;
                   break;

            }
        }      
        
        
        update(SDLK_UP, SDLK_DOWN, 2, 1, &pitch);
        update(SDLK_LEFT, SDLK_RIGHT, 0, 3, &yaw);
        update(SDLK_a, SDLK_d, &x);
        update(SDLK_w, SDLK_s, &y);
        update(SDLK_q, SDLK_e, &z);
        update(SDLK_PAGEUP, SDLK_PAGEDOWN, &roll);
        
        
        if(key_down[SDLK_z]){        
            if(constant_speed == 0.0f){
                constant_speed = 1.0f;
            }else{
                constant_speed *= 1.5f;
            }
        }

        if(key_down[SDLK_x]){
            constant_speed /= 1.5f;
        }

        if(key_down[SDLK_c]){
            constant_speed = 0;
        }

        y -= constant_speed;
        
        //prepare string and send to server
        sprintf(buffer, "%f, %f, %f, %f, %f, %f\n", x, y, z, yaw, pitch, roll);    
        //printf("%s\n", buffer);    
        write(server, buffer, strlen(buffer));
        
        //limit to 20fps        
        SDL_Delay(1.0f/20.0f * 1024);        
    }
    
    return 0;        
}


void update(int k_add, int k_sub, float *mod){
    update(k_add, k_sub, -1, -1, mod);
}

void update(int k_add, int k_sub, int j_add, int j_sub, float *mod){

    int amount = SPEED;

    if(key_down[k_add] || (j_add >= 0 && joy_down[j_add]) ){
        *mod = amount;
    }else if(key_down[k_sub] || (j_sub >= 0 && joy_down[j_sub]) ){
        *mod = -amount;
    }else{
        *mod = 0.0f;
    }
}
    
int connect_to_server(const char *address, int portno){
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\n");
        exit(0);
    }
    server = gethostbyname(address);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        printf("ERROR connecting\n");
        exit(0);
    }
    
    return sockfd;
}

