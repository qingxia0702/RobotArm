#include <iostream>
#include <GL/glut.h>
#include <string>
#include "RobotArm.h"
using namespace std;

#define M_Pi 3.1415926536f
#define BOLL_INIT_X  550.0
#define BOLL_INIT_Y  -300.0

//init the origin location
#define ORIGIN_X  300.0
#define ORIGIN_Y  -300.0

//init lenght and width of arms
#define UPPER_LENTH  120.0
#define UNDER_LENTH  120.0
#define ARM_WIDTH  10.0

//init the radius of reach boll
#define BOLL_R  10.0

//the init location of mouse
GLfloat mouselocation_x = BOLL_INIT_X;
GLfloat mouselocation_y = BOLL_INIT_Y;
GLfloat mouse_distance;

/*
    set the origin robot arm as:
	reach boll center: (550,-295)
	reach boll redius:10
	origin location: (300,-300)
	upper arm lenght: 120
	under arm lenght 120
	arm width 10
*/
RobotArm* robot_arm = new RobotArm(mouselocation_x, mouselocation_y,
	ORIGIN_X, ORIGIN_Y,
	UPPER_LENTH, UNDER_LENTH,
	ARM_WIDTH, BOLL_R);

//state of mouse
bool mouseisdown = false;
bool loopr = false;
int ry = 30;
int rx = 30;
//the angle input by user
int upper_angle;
int under_angle;

static GLfloat red[] = { 1, 0, 0 }, green[] = { 0, 1, 0 }, blue[] = { 0, 0, 1 };
static GLfloat yellow[] = { 0,92, 0.89, 1 };

void timer(int p)
{
	ry -= 5;
	glutPostRedisplay();       //marks the current window as needing to be redisplayed.
	if (loopr)
		glutTimerFunc(200, timer, 0);
}

//draw a cylinder between two points 
//points' location as input paramenters
void CylinderDraw(GLfloat x0, GLfloat y0,GLfloat z0, GLfloat x1 ,GLfloat y1,GLfloat z1){
    GLdouble  dir_x = x1 - x0;  
    GLdouble  dir_y = y1 - y0;  
    GLdouble  dir_z = z1 - z0;  
    GLdouble  bone_length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z  );  
    static GLUquadricObj *  quad_obj = NULL;  
    if ( quad_obj == NULL  )  
        quad_obj = gluNewQuadric();  
    gluQuadricDrawStyle( quad_obj, GLU_FILL  );  
    gluQuadricNormals( quad_obj, GLU_SMOOTH  );  

    glPushMatrix();
    glTranslated( x0, y0, z0  );  

    double  length;  
    length = sqrt( dir_x*dir_x + dir_y*dir_y + dir_z*dir_z  );  
    if ( length < 0.0001  ) {   
        dir_x = 0.0; dir_y = 0.0; dir_z = 1.0;  length = 1.0;  
    }  
    dir_x /= length;  dir_y /= length;  dir_z /= length;  
    GLdouble  up_x, up_y, up_z;  
    up_x = 0.0;  
    up_y = 1.0;  
    up_z = 0.0;  
    double  side_x, side_y, side_z;  
    side_x = up_y * dir_z - up_z * dir_y;  
    side_y = up_z * dir_x - up_x * dir_z;  
    side_z = up_x * dir_y - up_y * dir_x;  
    length = sqrt( side_x*side_x + side_y*side_y + side_z*side_z  );  
    if ( length < 0.0001  ) {  
        side_x = 1.0; side_y = 0.0; side_z = 0.0;  length = 1.0;  
    }  
    side_x /= length;  side_y /= length;  side_z /= length;  
    up_x = dir_y * side_z - dir_z * side_y;  
    up_y = dir_z * side_x - dir_x * side_z;  
    up_z = dir_x * side_y - dir_y * side_x;  

    GLdouble  m[16] = { side_x, side_y, side_z, 0.0,  
        up_x,   up_y,   up_z,   0.0,  
        dir_x,  dir_y,  dir_z,  0.0,  
        0.0,    0.0,    0.0,    1.0 };  
    glMultMatrixd( m  );  

    GLdouble radius= BOLL_R;       
    GLdouble slices = 8.0;      
    GLdouble stack = 3.0;        
    gluCylinder( quad_obj, radius, radius, bone_length, slices, stack  );   
    glPopMatrix();
}

void drawSphere(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat radius, GLfloat M, GLfloat N)  
{  
 float step_z = M_PI/M;  
 float step_xy = 2*M_PI/N;  
 float x[4],y[4],z[4];  
  
 float angle_z = 0.0;  
 float angle_xy = 0.0;  
 int i=0, j=0;  
 glBegin(GL_QUADS);  
  for(i=0; i<M; i++)  
  {  
     angle_z = i * step_z;  
       
     for(j=0; j<N; j++)  
     {  
         angle_xy = j * step_xy;  
       
         x[0] = radius * sin(angle_z) * cos(angle_xy);  
         y[0] = radius * sin(angle_z) * sin(angle_xy);  
         z[0] = radius * cos(angle_z);  
       
         x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);  
         y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);  
         z[1] = radius * cos(angle_z + step_z);  
       
         x[2] = radius*sin(angle_z + step_z)*cos(angle_xy + step_xy);  
         y[2] = radius*sin(angle_z + step_z)*sin(angle_xy + step_xy);  
         z[2] = radius*cos(angle_z + step_z);  
       
         x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);  
         y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);  
         z[3] = radius * cos(angle_z);  
       
         for(int k=0; k<4; k++)  
         {  
              glVertex3f(xx+x[k], yy+y[k],zz+z[k]);  
             }  
        }  
    }  
 glEnd();  
} 

//display function of cylinders and spheres
void DrawRobotArmCylinders(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    {
        //draw a spheres on origin location 
        glColor3f(0.89f,0.92f,1.0f);
        drawSphere(robot_arm->upper_arm->left_botton_location_x,
                robot_arm->upper_arm->left_botton_location_y,
                0.0,
                BOLL_R*2,
                50,50);  
       
        //draw a cylinders for upper arm
        glColor3f(0.0f,1.0f,1.0f);
        CylinderDraw(robot_arm->upper_arm->left_botton_location_x,
                robot_arm->upper_arm->left_botton_location_y,
                0.0,
                robot_arm->upper_arm->right_top_location_x,
                robot_arm->upper_arm->right_top_location_y,
                0.0);
        
       //draw a sphere on link location 
        glColor3f(0.89f,0.92f,1.0f);
        drawSphere(robot_arm->upper_arm->right_top_location_x,
                robot_arm->upper_arm->right_top_location_y,
                0.0,
                BOLL_R*2,
                50,50);  
       
        //draw a cylinder for under arm
            glColor3f(1.0f,0.0f,0.0f);
        CylinderDraw(robot_arm->under_arm->left_botton_location_x,
                robot_arm->under_arm->left_botton_location_y,
                0.0,
                robot_arm->under_arm->right_top_location_x,
                robot_arm->under_arm->right_top_location_y,
                0.0);
       
        //draw a sphere for reach boll
        glColor3f(0.89f,0.92f,1.0f);
        drawSphere(robot_arm->reach_boll->center_location_x,
                robot_arm->reach_boll->center_location_y,
                0.0,
                BOLL_R*2,
                50,50); 
        
        //draw a range of mouse
        glColor3fv(green);
        glBegin(GL_LINE_STRIP);
        int m = 200;
        for (int i = 0;i<=m;i++) 
            glVertex2f(ORIGIN_X + (UPPER_LENTH + UNDER_LENTH + BOLL_R)*cos(2 * M_Pi / m*i),
                ORIGIN_Y + (UPPER_LENTH + UNDER_LENTH + BOLL_R)*sin(2 * M_Pi / m*i));
        glEnd();
       
    }glPopMatrix();
    glFlush();
}

//display function of lines
void DrawRobotArmLines() {	
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    
    //draw a line for upper arm
    glColor3fv(blue);
    glBegin(GL_LINES);
    GLfloat up_arm_begin[] = {robot_arm->upper_arm->left_botton_location_x, robot_arm->upper_arm->left_botton_location_y};
    GLfloat up_arm_end[] = {robot_arm->under_arm->left_botton_location_x,robot_arm->under_arm->left_botton_location_y};
    glVertex2fv(up_arm_begin);
    glVertex2fv(up_arm_end);
    glEnd();

    //draw a line for under arm
    glColor3fv(red);
    glBegin(GL_LINES);
    GLfloat un_arm_begin[] = {robot_arm->under_arm->left_botton_location_x, robot_arm->under_arm->left_botton_location_y};
    GLfloat un_arm_end[] = {robot_arm->reach_boll->center_location_x,robot_arm->reach_boll->center_location_y};
    glVertex2fv(un_arm_begin);
    glVertex2fv(un_arm_end);
    glEnd();

    //draw the reach boll
    glColor3fv(green);
    glBegin(GL_TRIANGLE_FAN);
    int n = 20;
    for (int i = 0;i<n;i++)
        glVertex2f(robot_arm->reach_boll->center_location_x + (robot_arm->reach_boll->radius)*cos(2 * M_Pi / n*i),
                robot_arm->reach_boll->center_location_y + (robot_arm->reach_boll->radius)*sin(2 * M_Pi / n*i));
    glEnd();

    //draw a range of mouse
    glColor3fv(green);
    glBegin(GL_LINE_STRIP);
    int m = 200;
	for (int i = 0;i<=m;i++) 
		glVertex2f(ORIGIN_X + (UPPER_LENTH + UNDER_LENTH + BOLL_R)*cos(2 * M_Pi / m*i),
			ORIGIN_Y + (UPPER_LENTH + UNDER_LENTH + BOLL_R)*sin(2 * M_Pi / m*i));
	glEnd();

	glPopMatrix();
	glFlush();
}

//function of mouse callback
void MouseLocation(int button, int state,int x, int y) {
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseisdown = true;
            mouselocation_x = x;
            mouselocation_y = -600+y;
            mouse_distance = sqrt(pow((mouselocation_x-ORIGIN_X),2)+pow((mouselocation_y - ORIGIN_Y),2));
            cout << "location of mouse is :(" << x << "," << y << ")" << endl;
            //mouse is out of range
            if(mouse_distance > (UPPER_LENTH + UNDER_LENTH + BOLL_R)){
                cout << "the location is outof rangle"<<endl;
                robot_arm->mouse_location_x = mouselocation_x;
                robot_arm->mouse_location_y = mouselocation_y;
                robot_arm->outRangeHandle(mouselocation_x ,mouselocation_y);
                glutPostRedisplay();
            }else{
                robot_arm->mouse_location_x = mouselocation_x;
                robot_arm->mouse_location_y = mouselocation_y;
                robot_arm->locationCaculate(mouselocation_x, mouselocation_y);
                glutPostRedisplay();
                cout << "mouse location changed" << endl;
            }
		}
		else
			mouseisdown = false;
	}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN)
		{
			loopr = true;
			glutTimerFunc(200, timer, 0);
		}
}

//function of keyboard callback
void KeyBoardHandle(unsigned char key,int x,int y){
    switch(key){
        case 'k':
            cout<<"Please input the angles of robot arm!"<<endl;
            cout<<"Please input the angle of upper arm!"<<endl;
            cin >> upper_angle;
            cout<<"Please imput the angle of under arm!"<<endl;
            cin >> under_angle;
            robot_arm->angleCaculate(upper_angle,under_angle);
            glutPostRedisplay();
            break;
    }
}

void reshape(GLint w, GLint h)
{
	cout <<"get glint:"<< w << " " << h << endl;
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    //reshape the location,
    //with this ,we can caculate locations with mouse location.
	glOrtho(0.0f, w, 0.0f, -h, -500, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init (void)  {   
    glClearColor (0.0, 0.0, 0.0, 0.0);  
    glClearDepth(1);  
    glShadeModel(GL_SMOOTH);  
    GLfloat _ambient[]={0.0,0.0,0.0,0.0};  
    GLfloat _diffuse[]={1.0,1.0,0.0,1.0};  
    GLfloat _specular[]={1.0,1.0,1.0,1.0};  
    GLfloat _position[]={100,100,100,0.0};  
    glLightfv(GL_LIGHT0,GL_AMBIENT,_ambient);  
    glLightfv(GL_LIGHT0,GL_DIFFUSE,_diffuse);  
    glLightfv(GL_LIGHT0,GL_SPECULAR,_specular);  
    glLightfv(GL_LIGHT0,GL_POSITION,_position);  
    glEnable(GL_TEXTURE_2D);  
    glEnable(GL_LIGHTING);  
    glEnable(GL_LIGHT0);  
    glEnable(GL_COLOR_MATERIAL);
} 



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(600, 600);
	glutCreateWindow("RobotArm");
    init();
	cout << "Please chose the model of display" << endl;
    robot_arm->locationCaculate(mouselocation_x, mouselocation_y);
    glutMouseFunc(MouseLocation);
    //glutDisplayFunc(DrawRobotArmLines);
    glutDisplayFunc(DrawRobotArmCylinders);
    glutKeyboardFunc(KeyBoardHandle);		
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

