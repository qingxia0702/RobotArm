#pragma once
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <math.h>

class Arm {
public:
	GLfloat rotate_angle = 0.0f;
	GLfloat left_botton_location_x;
	GLfloat left_botton_location_y;
	GLfloat right_top_location_x;
	GLfloat right_top_location_y;
	
	Arm(GLfloat l_b_x, GLfloat l_b_y, GLfloat r_t_x, GLfloat r_t_y);
};

class Boll {
public:
	GLfloat radius;
	GLfloat center_location_x;
	GLfloat center_location_y;
	
	Boll(GLfloat r, GLfloat x, GLfloat y);
};

class RobotArm {
private:
	GLfloat upper_arm_lenth;
	GLfloat under_arm_lenth;
	GLfloat arm_widht;
	GLfloat origin_location_x;
	GLfloat origin_location_y;
	GLfloat boll_r;
public:
	GLfloat mouse_location_x;
	GLfloat mouse_location_y;
	Arm* upper_arm = NULL;
	Arm* under_arm = NULL;
	Boll* reach_boll = NULL;

	//RobotArm consturted by the location of mouse
	RobotArm(GLfloat x, GLfloat y,
		GLfloat origin_x, GLfloat origin_y,
		GLfloat len_up, GLfloat len_under,
		GLfloat widht, GLfloat r);
	void locationCaculate(GLfloat x, GLfloat y);
	void angleCaculate(GLfloat x, GLfloat y);
    void outRangeHandle(GLfloat x ,GLfloat y);
    ~RobotArm();

};
