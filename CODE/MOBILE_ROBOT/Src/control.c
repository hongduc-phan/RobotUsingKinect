/**
  ******************************************************************************
  * @file    control.c 
  * @author  
  * @version
  * @date
  * @brief	Control robot
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "control.h"

#include "uart.h"
#include "motor.h"
#include "buzzer.h"
#include "adc.h"
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int8_t sen_weight[8] = { -9, -7, -3, -1, 1, 3, 7, 9 };
float E = 0, E_old = 0;
float left_control = 0, right_control = 0;
float err_cur = 0, err_old = 0, err_del = 0, err_sum = 0;
float Kp_LINE[3] = {400, 600, 1000};
float Ki_LINE[3] = {5, 12, 22};
float Kd_LINE[3] = {10, 100, 120};

uint8_t PID_flag = 0;
int16_t v_trackline = 0;
uint8_t mode_run = 0;
// mode_run = 0 MANUAL
// mode_run = 1 AUTO

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
Function name: Robot_Control_Via_UART
Decription:
*******************************************************************************/
void Robot_Control_Via_UART(void)
{
	if (RxPackedIncomeFlag)
	{
        RxPackedIncomeFlag = 0;
        
        switch (RxBuffer[2])
        {
            case 0:
            {
                break;
            }
            
            case 1:     // User control
            {
                int16_t l_speed = 0, r_speed = 0;
                l_speed = ((RxBuffer[3] << 24) | (RxBuffer[4] << 16) | (RxBuffer[5] << 8) | RxBuffer[6]) * DUTY_MAX / 100;
                r_speed = ((RxBuffer[7] << 24) | (RxBuffer[8] << 16) | (RxBuffer[9] << 8) | RxBuffer[10]) * DUTY_MAX / 100;
                
                Motor_Speed(l_speed, r_speed);
                PID_flag = 0;
                
                break;
            }
            
            case 2:     // Auto run trackline
            {
                PID_flag = 1;
                v_trackline = ((RxBuffer[3] << 24) | (RxBuffer[4] << 16) | (RxBuffer[5] << 8) | RxBuffer[6]) * DUTY_MAX / 100;
                
                break;
            }
            
            default: break;
        }	
	}
}

/*******************************************************************************
Function name: Calc_E
Decription:
*******************************************************************************/
float Calc_E(void)
{
	if (sen_sum == 0)
	{
		E = E_old;
	}
	else
	{
		E = l4*sen_weight[0] + l3*sen_weight[1] + l2*sen_weight[2] + l1*sen_weight[3];
		E += r1*sen_weight[4] + r2*sen_weight[5] + r3*sen_weight[6] + r4*sen_weight[7];
		E = E/sen_sum;
		
		if ((E > 4) && (E_old <= -7))
			E = E_old;
		else if ((E < -4) && (E_old >= 7))
			E = E_old;
		else E_old = E;
	}
	
	return E;
}

/*******************************************************************************
Function name: TrackLinePID
Decription:
*******************************************************************************/
void TrackLinePID(uint8_t enable, int16_t err_set, int16_t speed)
{
    if (enable)
    {
        uint8_t step = 0;
        
        Read_Sensor();
        // Calculate new error
        Calc_E();
        
        if (sen_sum < 5)    // Cross not detected
        {
            int16_t l_speed = 0, r_speed = 0;
            float err_control = 0;
            
            // Proportional part
            err_cur = err_set - E;
            
            // Derivative part
            err_del = err_cur - err_old;
            
            // Intergral part
            if (err_cur == 0)	/* Neu sai so la 0 (tuc line da vao giua => xoa bo I de tranh dao dong vi ban than
                                PD da giup robot chay thang duoc, I chi tot cho khuc cong */	
                err_sum = 0;
            else err_sum += err_cur;
            
            // Backup error
            err_old = err_cur;
            
            // Gioi han gia tri sai so cong don de tranh tich luy lam cho gia tri dieu khien bi bao hoa (threshold)
            if (err_sum > 500) err_sum = 500;
            if (err_sum < -500) err_sum = -500;
            
            
            // Phan hoach vung gia tri dieu khien
            if (speed < 4000)
            {
                step = 0;
            }
            else if (speed <= 8000)
            {
                step = 1;
            }
            else if (speed > 8000)
            {
                step = 2;
            }
            
            // Calculate control value
            if (abs(err_cur) <= 2)	// Khi sai so cam bien < 3 thi bor thanh phan I vi no tich luy lam cho xe bi dao dong
            {
                err_control = Kp_LINE[step]*err_cur + 0*(err_sum / 10) + Kd_LINE[step]*err_del;
            }
            else	// Khi sai so lon thi co them thanh phan I se giup xe cua tot hon
            {
                err_control = Kp_LINE[step]*err_cur + Ki_LINE[step]*(err_sum / 10) + Kd_LINE[step]*err_del;
            }
            
            l_speed = (int16_t)(speed + err_control);
            r_speed = (int16_t)(speed - err_control);
            
            if (l_speed >  DUTY_MAX) l_speed = DUTY_MAX;
            if (l_speed <  -DUTY_MAX) l_speed = -DUTY_MAX;
            
            if (r_speed >  DUTY_MAX) r_speed = DUTY_MAX;
            if (r_speed <  -DUTY_MAX) r_speed = -DUTY_MAX;
            
            Motor_Speed(l_speed, r_speed);
        }
        else    // Cross detect
        {
            PID_flag = 0;
            Motor_Stop();
        }
    }
    else
    {
        // Reset all error
        err_cur = 0;
        err_old = 0;
        err_del = 0;
        err_sum = 0;
    }
}



