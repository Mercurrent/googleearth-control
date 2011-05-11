#include "libs.h"

extern xn::UserGenerator g_UserGenerator;
extern xn::DepthGenerator g_DepthGenerator;

char buf[256];
int server = -1;
bool confidence = false;

int connect_to_server(const char *address, int portno);

const char *SERVER_ADDRESS = "dn3";
const int SERVER_PORT = 24567;



//called on startup
void init_control(){   
    
    server = connect_to_server(SERVER_ADDRESS, SERVER_PORT);
    
    if(server == -1){
        printf("error connecting\n");
        exit(1);
    }
}

//Get a vec3 of a specified joint position
XnPoint3D get_joint(XnUserID player, XnSkeletonJoint jointid){
    
	XnSkeletonJointPosition joint1;
	g_UserGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, jointid, joint1);

    //blah. Is 0.5 right?
	if (joint1.fConfidence < 0.5){
	    confidence = false;		
	}

	return joint1.position;
}

//this is the interesting bit...
void update_control(XnUserID player){

    //difference between lefthand and righthand Y rotates view left and right
    //difference between lefthand and leftsholder in Y rotates view up and down
    //difference between lefthand and leftsholder in Z accels and brakes
    
    confidence = true; //reset for this frame
        
    XnPoint3D lefthand = get_joint(player, XN_SKEL_LEFT_HAND);    
    XnPoint3D righthand = get_joint(player, XN_SKEL_RIGHT_HAND);        
    XnPoint3D leftsholder = get_joint(player, XN_SKEL_LEFT_SHOULDER);        
    
    //printf("left: %f/%f/%f\n", left.X, left.Y, left.Z);    
    //printf("right: %f/%f/%f\n", right.X, right.Y, right.Z);
    
    float scale = 0.5f;
    float horiz = (lefthand.Y - righthand.Y) * scale;
    float vert = (lefthand.Y - leftsholder.Y) * scale;
    float accel = (lefthand.Z - leftsholder.Z) * scale;
            
    //we want to fly faster
    accel *= 2;
    
    //If we're not 'confident', then it's best not to do anything
    //usually someone has walked in front of the camera, or something unhelpful
    if(!confidence){
        accel = horiz = vert = 0.0f;
    }
            
    //x,y,z,yaw,pitch,roll
    sprintf(buf, "%f, %f, %f, %f, %f, %f", 
            0.0f, accel, 0.0f, horiz, vert, 0.0f);
        
    if(server > 0 && write(server, buf, 256) != 256){
        printf("error writing\n");
        exit(1);
    }
}





int connect_to_server(const char *address, int portno){
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
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
        printf("ERROR connecting");
        exit(0);
    }
    
    return sockfd;
}


