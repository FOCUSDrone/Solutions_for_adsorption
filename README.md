
# Task for new member
## TASK1：完成传感器选型、购买，等待期间完成风机的硬件驱动层（将hal的pwm封装成风机的上下限度），学会利用已经有的c板陀螺仪读取数据姿态结算（此部分代码写好了）
## TASK2：传感器回来了，完成未完成的TASK1，利用ADC（取决于硬件）读取传感器数据，得到数据后进行控制外环——风机的PID（你也可以提出更好的算法）
## TASK3：完成控制内环
## TASK4：启动、紧急叫停报错等程序解决
## ！github我给的程序仅供参考和示意，把他当成伪代码，学习他的思路（因为他肯定也是跑不通的）
# DDL：
- Task1我认为三天之内可以完成选型（实在不行可以抄我的），风机驱动一周内可以完成，姿态数据不会可以去问谭焱天，也就是说，下一周汇报需要TASK1完成
- Task2读取数据可以给一周，调PID再给最多三周（毕竟我连模版都给了，只需要略改加调参）
- Task3再给最多三周，对齐接口


## INA 226调控
## 2025/3/16了解IIC的配置，理解对应代码
## 2025/3/17 找到一个库和教程，不知道为什么传不回ID号 附一下教程 https://www.bilibili.com/video/BV1HK411y7Bc/?spm_id_from=333.337.search-card.all.click&vd_source=195e644b6e3eba008e4a5705144b92df
调试的时候c板用的句柄要改一下
## 2025/3/21 可以传回ID号了。 但是接线好像不太对。
https://blog.csdn.net/qq_56632135/article/details/143844192?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522773ebf6a31eccda72901a7bae6393109%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=773ebf6a31eccda72901a7bae6393109&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-143844192-null-null.142^v102^pc_search_result_base8&utm_term=INA226&spm=1018.2226.3001.4187
#### 找到一个接线的教程，明天试一下。
## 2025/3/25 迁移到c板代码上可以读出数据，但是代码有待整理
