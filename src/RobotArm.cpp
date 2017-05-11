#include "RobotArm.h"

using namespace std;
#define INIT_X 300.0f
#define INIT_Y 300.0f
#define M_Pi 3.1415926536f

Boll::Boll(GLfloat r, GLfloat x, GLfloat y) {
	radius = r;
	center_location_x = x;
	center_location_y = y;
}

Arm::Arm(GLfloat l_b_x, GLfloat l_b_y, GLfloat r_t_x, GLfloat r_t_y) {
	left_botton_location_x = l_b_x;
	left_botton_location_y = l_b_y;
	right_top_location_x = r_t_x;
	right_top_location_y = r_t_y;
}

RobotArm::~RobotArm() {
	if (RobotArm::upper_arm != NULL)
	{
		delete upper_arm;
	}
	if (upper_arm != NULL)
	{
		delete under_arm;
	}
	if (upper_arm != NULL)
	{
		delete reach_boll;
	}
}

//consturctor of class RobotArm
RobotArm::RobotArm(GLfloat x, GLfloat y,
	GLfloat origin_x, GLfloat origin_y,
	GLfloat len_up, GLfloat len_under,
	GLfloat widht, GLfloat r) {
	mouse_location_x = x;
	mouse_location_y = y;
	upper_arm_lenth = len_up;
	under_arm_lenth = len_under;
	origin_location_x = origin_x;
	origin_location_y = origin_y;
	arm_widht = widht;
	boll_r = r;
}

//caculate robot arm with the location of mouse
void RobotArm::locationCaculate(GLfloat x, GLfloat y) {
	GLfloat cal_x, cal_y;
	GLfloat temp_c;
	GLfloat theta, theta_temp,bata;
    GLfloat theta_x_y,theta_y_x;
	GLfloat alpha_upper_arm, alpha_under_arm;

	temp_c = sqrt(pow(x - origin_location_x, 2) + pow(y - origin_location_y, 2));
	bata = acos((pow(upper_arm_lenth, 2) + pow(temp_c, 2) - pow((under_arm_lenth + boll_r), 2)) /(2*upper_arm_lenth*temp_c));

    theta_x_y = (x - origin_location_x)/(y - origin_location_y);
    if((y - origin_location_y) < 0){
        theta_temp = M_PI*1.5 - atan(theta_x_y);
    }else if((y - origin_location_y) > 0){
        theta_temp = M_PI*0.5 - atan(theta_x_y);
    }else if((y - origin_location_y) == 0){
        theta_temp = M_PI - atan(theta_x_y);
    }	
    
    if(theta < 0){
        theta_temp = theta_temp + 2*M_PI;
    }

    theta = theta_temp -bata;
    cal_x = origin_location_x + upper_arm_lenth*cos(theta);
	cal_y = origin_location_y + upper_arm_lenth*sin(theta);
	alpha_upper_arm = atan((cal_y - origin_location_y) / (cal_x - origin_location_x));
	alpha_under_arm = atan((y - cal_y) / (x - cal_x));

    upper_arm = new Arm(origin_location_x,origin_location_y,
            cal_x,cal_y);
    under_arm = new Arm(cal_x,cal_y,x,y);
	reach_boll = new Boll(boll_r, mouse_location_x, mouse_location_y);
	cout << "caculta success" << endl;
}

//handle the event of mouse out of range
void RobotArm::outRangeHandle(GLfloat x ,GLfloat y){
    GLfloat theta;
    GLfloat cal_x,cal_y;
    GLfloat max_x,max_y;
    if((y - origin_location_y) < 0){
        theta = atan((x - origin_location_x)/(y - origin_location_y));
    }else{
        theta = M_PI+atan((x - origin_location_x)/(y - origin_location_y));
    }
    cout<<theta<<endl;
    cal_x = origin_location_x - upper_arm_lenth*sin(theta);
    cal_y = origin_location_y - upper_arm_lenth*cos(theta);
    max_x = origin_location_x - (upper_arm_lenth + under_arm_lenth + boll_r)*sin(theta);
    max_y = origin_location_y - (upper_arm_lenth + under_arm_lenth + boll_r)*cos(theta);
    upper_arm = new Arm(origin_location_x,origin_location_y,cal_x,cal_y);
    under_arm = new Arm(cal_x,cal_y,max_x,max_y);
	reach_boll = new Boll(boll_r, max_x, max_y);
}

//caculate robot arm with input angles
void RobotArm::angleCaculate(GLfloat x, GLfloat y) {
    GLfloat cal_x,cal_y;
    GLfloat boll_x,boll_y;
    GLfloat theta;
    GLfloat x_angle = (M_PI/180)*x;
    GLfloat y_angle = (M_PI/180)*y;

    cal_x = origin_location_x + upper_arm_lenth*cos(x_angle);
    cal_y = origin_location_y + upper_arm_lenth*sin(x_angle);
    
    theta = M_PI - (x_angle + y_angle);
    boll_x = cal_x + under_arm_lenth*cos(y_angle);
    boll_y = cal_y + under_arm_lenth*sin(y_angle);

    upper_arm = new Arm(origin_location_x,origin_location_y,cal_x,cal_y);
    under_arm = new Arm(cal_x,cal_y,boll_x,boll_y);
    reach_boll = new Boll(boll_r,boll_x,boll_y);
}
