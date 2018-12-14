#include <drive_control.h>
#include <drive_delay.h> 
#include <drive_imu.h>

#include <motor_chassis.h>
#include <motor_cradle_head.h>

#include <robotstatus.h>
 
#define CHASSIS_W   198.785   //mm
#define CHASSIS_L   175.375

infantry_control_t g_infc;

object_t g_speed_target;
object_t g_angle_target;

static object_t s_angle_contorl_out;
static object_t s_speed_contorl_out;


static void PID_Reset(void)
{
    PID_SetParam(&g_infc.pid[SHOOT_SPEED],3.43,38.15,0.02,1000,5000,14.2);
    PID_SetParam(&g_infc.pid[SHOOT_ANGLE],100,0,0,0,0,   0);
	
    PID_SetParam(&g_infc.pid[PITCH_ANGLE],120.5,865 ,0.03,8,3000,18.73);
	//PID_SetParam(&g_infc.pid[YAW_ANGLE],  50,0 ,0,0,0,0);
	PID_SetParam(&g_infc.pid[YAW_ANGLE],120.5,865 ,0.03,8,3000,18.73);
	
	
	PID_SetParam(&g_infc.pid[LF_SPEED],   3.26, 26.64,0.06,1000,5000,61.3);
	PID_SetParam(&g_infc.pid[LA_SPEED],   3.26, 26.64,0.06,1000,5000,61.3);
	PID_SetParam(&g_infc.pid[RF_SPEED],   3.26, 26.64,0.06,1000,5000,61.3);
	PID_SetParam(&g_infc.pid[RA_SPEED],   3.26, 26.64,0.06,1000,5000,61.3);
}
void Infan_Control_Init(void)
{
	
	g_angle_target.pitch=263;
	g_angle_target.yaw=154;//88
	
	
	
//	g_speed_target.lf=4000;
//	g_speed_target.la=4000;
//	g_speed_target.rf=4000;
//	g_speed_target.ra=4000;

	PID_Reset();
}
void Angle_6623_Control(object_t target)
{
	static u8 error_temp=0;
	
	if(robot_status.imu_data==DATA_FALSE&&robot_status.imu_status==NO_CORRECT)
	{
		//函数运算间隔计算
		static uint64_t previousT;
		float deltaT = (Get_SysTimeUs() - previousT) * 1e-6;
		previousT = Get_SysTimeUs();
		
		g_infc.angle_outer_target.pitch		= target.pitch;
		g_infc.angle_outer_target.yaw		= target.yaw;
		
		//计算角度控制误差
		g_infc.angle_outer_error.pitch  	= g_infc.angle_outer_target.pitch - g_data_6623.angle[PITCH];
		g_infc.angle_outer_error.yaw  	    = g_infc.angle_outer_target.yaw   - g_data_6623.angle[YAW];
		
		//死区控制
		g_infc.angle_outer_error.pitch  	= ApplyDeadbandFloat(g_infc.angle_outer_error.pitch,0.05);
		g_infc.angle_outer_error.yaw  	    = ApplyDeadbandFloat(g_infc.angle_outer_error.yaw,  0.05);
		
		if(abs(g_infc.angle_outer_error.pitch)<0.5f)//&&abs((g_infc.angle_outer_error.yaw)<0.5f//))
		{
			error_temp++;
			if(error_temp>=80)
			{
				//robot_status.imu_status=CORRECT_START;
				error_temp=0;
			}
			
		}
		//PID算法，计算出角度环的控制量
		s_angle_contorl_out.pitch         	= PID_GetPID(&g_infc.pid[PITCH_ANGLE],g_infc.angle_outer_error.pitch,deltaT);
		s_angle_contorl_out.yaw         	= PID_GetPID(&g_infc.pid[YAW_ANGLE],  g_infc.angle_outer_error.yaw,  deltaT);
		
		//输出限幅
		s_angle_contorl_out.pitch         	= ConstrainFloat(s_angle_contorl_out.pitch,-3000,3000);
		s_angle_contorl_out.yaw         	= ConstrainFloat(s_angle_contorl_out.yaw,  -3000,3000);
		
		Cmd_6623_ESC(-s_angle_contorl_out.yaw,-s_angle_contorl_out.pitch);
    }
	//if(robot_status.imu_data==DATA_TRUE)
	   //Cmd_6623_ESC(0,0);
}

void Speed_3510_Control(object_t target)
{
	//函数运算间隔计算
   	static uint64_t previousT;
    float deltaT = (Get_SysTimeUs() - previousT) * 1e-6;
    previousT = Get_SysTimeUs();
	
	//速度环目标赋值
	g_infc.speed_inner_target.lf    = target.lf;
	g_infc.speed_inner_target.la    = target.la;
	g_infc.speed_inner_target.rf    = target.rf;
	g_infc.speed_inner_target.ra    = target.ra;
	
	//计算速度环控制误差
	g_infc.speed_inner_error.lf     = g_infc.speed_inner_target.lf      - g_data_3510.speed[LF];
	g_infc.speed_inner_error.la     = g_infc.speed_inner_target.la      - g_data_3510.speed[LA];
	g_infc.speed_inner_error.rf     = g_infc.speed_inner_target.rf      - g_data_3510.speed[RF];
	g_infc.speed_inner_error.ra     = g_infc.speed_inner_target.ra      - g_data_3510.speed[RA];
	
	//死区控制
	g_infc.speed_inner_error.lf     = ApplyDeadbandFloat( g_infc.speed_inner_error.lf,5);
	g_infc.speed_inner_error.la     = ApplyDeadbandFloat( g_infc.speed_inner_error.la,5);
	g_infc.speed_inner_error.rf     = ApplyDeadbandFloat( g_infc.speed_inner_error.rf,5);
	g_infc.speed_inner_error.ra     = ApplyDeadbandFloat( g_infc.speed_inner_error.ra,5);
	
	//PID算法，计算出速度环的控制量
	s_speed_contorl_out.lf          = PID_GetPID(&g_infc.pid[LF_SPEED],g_infc.speed_inner_error.lf,deltaT);
	s_speed_contorl_out.la          = PID_GetPID(&g_infc.pid[LA_SPEED],g_infc.speed_inner_error.la,deltaT);
	s_speed_contorl_out.rf          = PID_GetPID(&g_infc.pid[RF_SPEED],g_infc.speed_inner_error.rf,deltaT);
	s_speed_contorl_out.ra          = PID_GetPID(&g_infc.pid[RA_SPEED],g_infc.speed_inner_error.ra,deltaT);
	
	//输出限幅
	s_speed_contorl_out.lf          = ConstrainFloat(s_speed_contorl_out.lf,-6000,6000);
	s_speed_contorl_out.la          = ConstrainFloat(s_speed_contorl_out.la,-6000,6000);
	s_speed_contorl_out.rf          = ConstrainFloat(s_speed_contorl_out.rf,-6000,6000);
	s_speed_contorl_out.ra          = ConstrainFloat(s_speed_contorl_out.ra,-6000,6000);
	
	//can发送控制量
	Cmd_3510_ESC(s_speed_contorl_out.lf,s_speed_contorl_out.la,s_speed_contorl_out.rf,s_speed_contorl_out.ra);
}

void Speed_2006_Control(object_t target)
{
	//函数运算间隔计算
	static uint64_t previousT;
    float deltaT = (Get_SysTimeUs() - previousT) * 1e-6;
    previousT = Get_SysTimeUs();
	
	//速度环目标赋值
    g_infc.speed_inner_target.shoot		= target.shoot;
	
	//计算速度环控制误差
    g_infc.speed_inner_error.shoot  	= g_infc.speed_inner_target.shoot - g_data_2006.speed;
	
	//死区控制
    g_infc.speed_inner_error.shoot  	= ApplyDeadbandFloat( g_infc.speed_inner_error.shoot,10);
	
	//PID算法，计算出速度环的控制量
    s_speed_contorl_out.shoot         	= PID_GetPID(&g_infc.pid[SHOOT_SPEED],g_infc.speed_inner_error.shoot,deltaT);
	
	//输出限幅
    s_speed_contorl_out.shoot         	= ConstrainFloat(s_speed_contorl_out.shoot,-6000,6000);
	
	//can发送控制量
	Cmd_2006_ESC(s_speed_contorl_out.shoot);
	//Cmd_2006_ESC(0);
}

void Angle_2006_Control(object_t target)
{
	//函数运算间隔计算
    static uint64_t previousT;
    float deltaT = (Get_SysTimeUs() - previousT) * 1e-6;
    previousT = Get_SysTimeUs();
	//角度环目标赋值
	g_infc.angle_outer_target.shoot		= target.shoot;
	
	//计算角度控制误差
	g_infc.angle_outer_error.shoot  	= g_infc.angle_outer_target.shoot-g_data_2006.angle;
	
	//死区控制
    g_infc.angle_outer_error.shoot  	= ApplyDeadbandFloat(g_infc.angle_outer_error.shoot,0.1);
	
	//PID算法，计算出角度环的控制量
    s_angle_contorl_out.shoot         	= PID_GetPID(&g_infc.pid[SHOOT_ANGLE],g_infc.angle_outer_error.shoot,deltaT);
	
	//输出限幅
    s_angle_contorl_out.shoot         	= ConstrainFloat(s_angle_contorl_out.shoot,-6000,6000);
	
	//将角度外环控制量作为速度内环的控制目标
	Speed_2006_Control(s_angle_contorl_out);
}
/*
*Y W
*|<-
*| <-
*|__________x
*/
void Speed_Chassis_Control(float xspeed,float yspeed,float yawspeed)    //底盘速度控制
{
	g_speed_target.rf=-(yspeed-xspeed+yawspeed*(CHASSIS_L+CHASSIS_W)/2.0f); 
	g_speed_target.lf=yspeed+xspeed+yawspeed*(CHASSIS_L-CHASSIS_W)/2.0f;
	g_speed_target.la=yspeed-xspeed+yawspeed*(CHASSIS_L-CHASSIS_W)/2.0f;
	g_speed_target.ra=-(yspeed+xspeed+yawspeed*(CHASSIS_L+CHASSIS_W)/2.0f);
	Speed_3510_Control(g_speed_target);
}

void Speed_Rotate_Control(object_t target)       //自旋speed
{
	static uint64_t previousT;
    float deltaT = (Get_SysTimeUs() - previousT) * 1e-6;
    previousT = Get_SysTimeUs();
	
	//速度环目标赋值
	g_infc.speed_inner_target.ch_rotate=target.ch_rotate;
	
	//计算速度环控制误差
    g_infc.speed_inner_error.ch_rotate  	= g_infc.speed_inner_target.ch_rotate-g_imu_data.yaw_speed;
	
	//死区控制
    g_infc.speed_inner_error.ch_rotate  	= ApplyDeadbandFloat( g_infc.speed_inner_error.ch_rotate,10);
	
	//PID算法，计算出速度环的控制量
    s_speed_contorl_out.ch_rotate         	= PID_GetPID(&g_infc.pid[CH_ROTATE_SPEED],g_infc.speed_inner_error.ch_rotate,deltaT);
	
	//输出限幅
    s_speed_contorl_out.ch_rotate         	= ConstrainFloat(s_speed_contorl_out.ch_rotate,-6000,6000);
	
	Speed_Chassis_Control(0,0,s_speed_contorl_out.ch_rotate);
}

void Angle_Rotate_Control(object_t target)         //
{
    static uint64_t previousT;
    float deltaT = (Get_SysTimeUs() - previousT) * 1e-6;
    previousT = Get_SysTimeUs();
	//角度环目标赋值
	g_infc.angle_outer_target.ch_rotate		= target.ch_rotate;
	
	//计算角度控制误差
	g_infc.angle_outer_error.ch_rotate  	= g_infc.angle_outer_target.ch_rotate-g_imu_data.yaw;
	
	//死区控制
    g_infc.angle_outer_error.ch_rotate  	= ApplyDeadbandFloat(g_infc.angle_outer_error.ch_rotate,0.1);
	
	//PID算法，计算出角度环的控制量
    s_angle_contorl_out.ch_rotate         	= PID_GetPID(&g_infc.pid[SHOOT_ANGLE],g_infc.angle_outer_error.ch_rotate,deltaT);
	
	//输出限幅
    s_angle_contorl_out.ch_rotate         	= ConstrainFloat(s_angle_contorl_out.ch_rotate,-6000,6000);
	
	//将角度外环控制量作为速度内环的控制目标
	Speed_Rotate_Control(s_angle_contorl_out);
}
