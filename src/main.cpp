/*
 * Copyright (C) 2019 SEG
 */

#include <stdio.h>
#include "balance_fio/balance_fio.h"

const static char VERSION[] = "0.0.001";

int
main(int argc, char **argv)
{
    printf("--- BalanceFIO ---V. %s\n", VERSION);
    ros::init(argc, argv, "BalanceFio");

    BalanceFIO bl_fio;

    if (0 == bl_fio.Init()) {
      bl_fio.Start();
      ros::Rate rate(5); //adjust frq
      while (ros::ok()) {
        ros::spinOnce();
        bl_fio.Odometry();
        rate.sleep();
      }
    }

    return 0;
}
