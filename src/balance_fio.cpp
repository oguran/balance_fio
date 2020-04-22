/*
 * Copyright (C) 2013-2014 Sony Corporation
 */

#include <ros/ros.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "balance_fio/balance_fio.h"

#define TOPIC_CMD "/bl/balance_cmd"
#define TOPIC_ODM "/bl/balance_odm"


BalanceFIO::BalanceFIO()
{
}

BalanceFIO::~BalanceFIO()
{
}


int BalanceFIO::Init()
{
	int ret = mkfifo(BALANCE_CMD_PATH, 0666);
	printf("BalanceFIO::Init ret=%d\n", ret);
	ret |= mkfifo(BALANCE_ODM_PATH, 0666);
	printf("BalanceFIO::Init ret=%d\n", ret);
    return ret;

}

void BalanceFIO::Start()
{
  ros::NodeHandle nh;
//from UI postion cmd
  ui_sub = nh.subscribe(
              TOPIC_CMD, 1,
              &BalanceFIO::CmdCallback, this);
//To UI
  pub_odm = nh.advertise<balance_msg::BalanceOdm>(TOPIC_ODM, 1);

}

void BalanceFIO::Stop()
{
}

void BalanceFIO::CmdCallback(const balance_msg::BalanceCmd& cmd)
{
  int ret;
  BALANCE_CMD_t balance_cmd;
  //from UI cmd
  // printf("--- BLFIO:CmdCallback 0: ---left_speed=%d right_speed=%d flag_1=%d flag_2=%d \n",
    // cmd.left_speed, cmd.right_speed, cmd.flag_1, cmd.flag_2);

  //to Balance
  balance_cmd.left_speed = cmd.left_speed;
  balance_cmd.right_speed = cmd.right_speed;
  balance_cmd.flag_1 = cmd.flag_1;
  balance_cmd.flag_2 = cmd.flag_2;

  ret = WriteBlanceCmd(&balance_cmd);
  if (BL_STATUS_OK != ret) {
    printf("--- BLFIO:CmdCallback --- data error: \n");
  }
}

int BalanceFIO::WriteBlanceCmd(BALANCE_CMD_t *balance_cmd_out)
{
  // printf("open %s\n", BALANCE_CMD_PATH);
  balance_cmd_fd = open(BALANCE_CMD_PATH, O_WRONLY);
  if (balance_cmd_fd == -1) {
    printf("--- BLFIO:WriteBlanceCmd --- file open error: %s \n", BALANCE_CMD_PATH);
    return BL_STATUS_ERR;
  }

  // printf("write %s\n", BALANCE_CMD_PATH);
  write(balance_cmd_fd, balance_cmd_out, sizeof(BALANCE_CMD_t));
  close(balance_cmd_fd);

  return BL_STATUS_OK;
}

int BalanceFIO::ReadBlanceOdm(BALANCE_ODM_t *balance_odm_in)
{
  // printf("open %s\n", BALANCE_ODM_PATH);
  int balance_odm_fd = open(BALANCE_ODM_PATH, O_RDONLY/* | O_NONBLOCK*/);
  if (balance_odm_fd == -1) {
    // printf("--- BLFIO:ReadBlanceOdm --- file open error: %s \n", BALANCE_ODM_PATH);
    return BL_STATUS_ERR;
  }
  // printf("read %s\n", BALANCE_ODM_PATH);
  size_t size = read(balance_odm_fd, balance_odm_in, sizeof(BALANCE_ODM_t));
  close(balance_odm_fd);
  return size > 0 ? BL_STATUS_OK : BL_STATUS_ERR;
}

void BalanceFIO::Odometry()
{
  int ret;
  BALANCE_ODM_t odm_in;

  ret = ReadBlanceOdm(&odm_in);
  if (BL_STATUS_OK != ret) {
    // printf("--- BLFIO:Odometry 0: --- data error: \n");
    return;
  }

  //from balancer (notify odm)
  // printf("--- BLIF:Odometry 1: ---ax=%f ay=%f az%f gx=%f gy=%f gz=%f :le=%d re%d s1=%d s2=%d ls=%d rs=%d \n",
  //   odm_in.ax, odm_in.ay, odm_in.az, odm_in.gx, odm_in.gy, odm_in.gz,  odm_in.left_enc, odm_in.right_enc,
  //   odm_in.status_1, odm_in.status_2, odm_in.left_speed, odm_in.right_speed);

  //Calc balance Odometry to ui postion
  //pub_ui_odm
  balance_msg::BalanceOdm odm_msg;
  // pos
  odm_msg.ax = odm_in.ax;
  odm_msg.ay = odm_in.ay;
  odm_msg.az = odm_in.az;

  odm_msg.gx = odm_in.gx;
  odm_msg.gy = odm_in.gy;
  odm_msg.gz = odm_in.gz;

  odm_msg.left_enc = odm_in.left_enc;
  odm_msg.right_enc = odm_in.right_enc;

  odm_msg.status_1 = odm_in.status_1;
  odm_msg.status_2 = odm_in.status_2;

  odm_msg.left_speed = odm_in.left_speed;
  odm_msg.right_speed = odm_in.right_speed;

  pub_odm.publish(odm_msg);
  printf("%s publish\r", TOPIC_ODM);

}



////////////////////////// END /////////////////////////
