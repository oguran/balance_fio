/*
 * Copyright (C) 2019 SEG
 */

#ifndef BALANCE_FIO_HPP_
#define BALANCE_FIO_HPP_

#include <ros/ros.h>
#include <unistd.h>
#include "balance_msg/BalanceCmd.h"
#include "balance_msg/BalanceOdm.h"
//#include "balance_fio/PosOdm.h"
//#include "balance_fio/PosCmd.h"

#define BL_STATUS_ERR		-1
#define BL_STATUS_OK    0

#define BALANCE_CMD_PATH "/tmp/balance_cmd"
#define BALANCE_ODM_PATH "/tmp/balance_odm"

typedef struct BALANCE_CMD {
  // speed
  short left_speed;
  short right_speed;
  // condition for debug
  char   flag_1;
  char   flag_2;
} BALANCE_CMD_t;

typedef struct BALANCE_ODM {
  // imu
  float   ax;
  float   ay;
  float   az;

  float   gx;
  float   gy;
  float   gz;

  // enc
  short   left_enc;
  short   right_enc;

  // condition for debug
  char   status_1;
  char   status_2;

  // cmd_echo speed for debug
  short   left_speed;
  short   right_speed;
} BALANCE_ODM_t;

class BalanceFIO
{
public:
    BalanceFIO();
    virtual ~BalanceFIO();
    int Init();
    void Start();
    void Stop();
    void Odometry();

    //from UI postion cmd
    void CmdCallback(const balance_msg::BalanceCmd& cmd);

private:
    //ros::NodeHandle nh;
    //from UI postion cmd
    ros::Subscriber ui_sub;
    //To UI
    ros::Publisher pub_odm;
    //ros::Subscriber<balance_msg::BalanceOdm> baalance_sub("balance_odm", BalanceIF::BalanceCallback );
    int WriteBlanceCmd(BALANCE_CMD_t *balance_cmd_out);
    int ReadBlanceOdm(BALANCE_ODM_t *balance_odm_in);

	// fd for named fifo
    int balance_cmd_fd;


    //std::String m_test;		//

};

#endif /* BALANCE_FIO_HPP_ */
