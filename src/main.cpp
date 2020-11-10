/*
 * Copyright (C) 2019 SEG
 */

#include <stdio.h>
#include "balance_fio/balance_fio.h"

const static char VERSION[] = "0.0.001";

int
main(int argc, char **argv)
{
    printf("--- BalanceFIO ---V. %s Start\n", VERSION);

    ros::init(argc, argv, "BalanceFio");

    BalanceFIO bl_fio;

    if (0 == bl_fio.Init()) {
      bl_fio.Start();
      ros::AsyncSpinner aspinner(1);
      aspinner.start();
      ros::Rate rate(100); //adjust frq
      // ros::Time t0 = ros::Time::now();
      // int cnt = 0;
      while (ros::ok()) {
        bl_fio.Odometry();
        // cnt++;
        // ros::Time t1 = ros::Time::now();
        // ros::Duration duration = t1 - t0;
        // if ((duration) >= ros::Duration(1.0)) {
        //   printf("%f Hz (%d/%f)\n", (cnt / duration.toSec()), cnt, duration.toSec());
        //   t0 = t1;
        //   cnt = 0;
        // }
        rate.sleep();
      }
      aspinner.stop();
      bl_fio.Stop();
    }

    printf("--- BalanceFIO ---V. %s Exit\n", VERSION);

    return 0;
}
