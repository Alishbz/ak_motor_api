#pragma once

struct AK80_9
{
    float P_min = -12.5f;
    float P_max = 12.5f;
    float V_min = -50.0f;
    float V_max = 50.0f;
    float T_min = -18.0f;
    float T_max = 18.0f;
    float Kp_min = 0.0f;
    float Kp_max = 500.0f;
    float Kd_min = 0.0f;
    float Kd_max = 5.0f;
    float Kt_TMotor = 0.091f;
    float Current_Factor = 0.59f;
    float Kt_Actual = 0.115f;
    int Gear_Ratio = 9;
    bool Use_derived_torque_constants = true;
    float a_hat[5] = { 0.0f, 1.15605006e+00f, 4.17389589e-04f, 2.68556072e-01f, 4.90424140e-02f };
};

struct AK10_9
{
    float P_min = -12.5f;
    float P_max = 12.5f;
    float V_min = -50.0f;
    float V_max = 50.0f;
    float T_min = -65.0f;
    float T_max = 65.0f;
    float Kp_min = 0.0f;
    float Kp_max = 500.0f;
    float Kd_min = 0.0f;
    float Kd_max = 5.0f;
    float Kt_TMotor = 0.16f;
    float Current_Factor = 0.59f;
    float Kt_Actual = 0.16f;
    int Gear_Ratio = 9;
    bool Use_derived_torque_constants = false;
    float a_hat[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

struct AK60_6
{
    float P_min = -12.5f;
    float P_max = 12.5f;
    float V_min = -45.0f;
    float V_max = 45.0f;
    float T_min = -15.0f;
    float T_max = 15.0f;
    float Kp_min = 0.0f;
    float Kp_max = 500.0f;
    float Kd_min = 0.0f;
    float Kd_max = 5.0f;
    float Kt_TMotor = 0.068f;
    float Current_Factor = 0.59f;
    float Kt_Actual = 0.087f;
    int Gear_Ratio = 9;
    bool Use_derived_torque_constants = false;
    float a_hat[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

struct AK70_10
{
    float P_min = -12.5f;
    float P_max = 12.5f;
    float V_min = -50.0f;
    float V_max = 50.0f;
    float T_min = -25.0f;
    float T_max = 25.0f;
    float Kp_min = 0.0f;
    float Kp_max = 500.0f;
    float Kd_min = 0.0f;
    float Kd_max = 5.0f;
    float Kt_TMotor = 0.095f;
    float Current_Factor = 0.59f;
    float Kt_Actual = 0.122f;
    int Gear_Ratio = 10;
    bool Use_derived_torque_constants = false;
    float a_hat[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

struct AK80_6
{
    float P_min = -12.5f;
    float P_max = 12.5f;
    float V_min = -76.0f;
    float V_max = 76.0f;
    float T_min = -12.0f;
    float T_max = 12.0f;
    float Kp_min = 0.0f;
    float Kp_max = 500.0f;
    float Kd_min = 0.0f;
    float Kd_max = 5.0f;
    float Kt_TMotor = 0.091f;
    float Current_Factor = 0.59f;
    float Kt_Actual = 0.017f;
    int Gear_Ratio = 6;
    bool Use_derived_torque_constants = false;
    float a_hat[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

struct AK80_64
{
    float P_min = -12.5f;
    float P_max = 12.5f;
    float V_min = -8.0f;
    float V_max = 8.0f;
    float T_min = -144.0f;
    float T_max = 144.0f;
    float Kp_min = 0.0f;
    float Kp_max = 500.0f;
    float Kd_min = 0.0f;
    float Kd_max = 5.0f;
    float Kt_TMotor = 0.119f;
    float Current_Factor = 0.59f;
    float Kt_Actual = 0.153f;
    int Gear_Ratio = 80;
    bool Use_derived_torque_constants = false;
    float a_hat[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

