import numpy as np
import math
from math import sqrt, sin, cos, atan2, pi


def DHMatrix(alf, a, d, theta):
    sa = math.sin(alf)
    ca = math.cos(alf)
    st = math.sin(theta)
    ct = math.cos(theta)
    t = np.array([[ct, -st * ca, st * sa, a * ct],
                 [st, ct * ca, -ct * sa, a * st],
                 [0, sa, ca, d],
                 [0, 0, 0, 1]])
    return t


def RobotOnKine(alf, a, d, theta):  # robot forward kinematics trans matrix calculate
    m = []
    trans = []
    for i in range(1, 7):
        m.append(DHMatrix(alf[i - 1], a[i - 1], d[i - 1], theta[i - 1]))
        if i == 1:
            trans = DHMatrix(alf[i - 1], a[i - 1], d[i - 1], theta[i - 1])
        else:
            trans = np.matmul(trans, DHMatrix(alf[i - 1], a[i - 1], d[i - 1], theta[i - 1]))
    return m, trans


def RobotOnKineInv(A, a, d):  # robot forward kinematics trans matrix calculate
    # target pose matrix
    nx = A[0, 0]
    ox = A[0, 1]
    ax = A[0, 2]
    px = A[0, 3]
    ny = A[1, 0]
    oy = A[1, 1]
    ay = A[1, 2]
    py = A[1, 3]
    # nz = A[2, 0]
    # oz = A[2, 1]
    az = A[2, 2]
    pz = A[2, 3]
    # calculate six theta
    # theta1
    theta1_1 = atan2((-d[3]), (sqrt((d[5] * ay - py) ** 2 + (px - d[5] * ax) ** 2 - d[3] ** 2))) - atan2(
        (d[5] * ay - py), (px - d[5] * ax))
    theta1_2 = atan2((-d[3]), (-sqrt((d[5] * ay - py) ** 2 + (px - d[5] * ax) ** 2 - d[3] ** 2))) - atan2(
        (d[5] * ay - py), (px - d[5] * ax))
    # theta5
    theta5_1 = atan2(
        sqrt((nx * sin(theta1_1) - ny * cos(theta1_1)) ** 2 + (ox * sin(theta1_1) - oy * cos(theta1_1)) ** 2),
        ax * sin(theta1_1) - ay * cos(theta1_1))
    theta5_2 = atan2(
        -sqrt((nx * sin(theta1_1) - ny * cos(theta1_1)) ** 2 + (ox * sin(theta1_1) - oy * cos(theta1_1)) ** 2),
        ax * sin(theta1_1) - ay * cos(theta1_1))
    theta5_3 = atan2(
        sqrt((nx * sin(theta1_2) - ny * cos(theta1_2)) ** 2 + (ox * sin(theta1_2) - oy * cos(theta1_2)) ** 2),
        ax * sin(theta1_2) - ay * cos(theta1_2))
    theta5_4 = atan2(
        -sqrt((nx * sin(theta1_2) - ny * cos(theta1_2)) ** 2 + (ox * sin(theta1_2) - oy * cos(theta1_2)) ** 2),
        ax * sin(theta1_2) - ay * cos(theta1_2))
    # theta6
    theta6_1 = atan2(-(ox * sin(theta1_1) - oy * cos(theta1_1)) / sin(theta5_1),
                     (nx * sin(theta1_1) - ny * cos(theta1_1)) / sin(theta5_1))
    theta6_2 = atan2(-(ox * sin(theta1_1) - oy * cos(theta1_1)) / sin(theta5_2),
                     (nx * sin(theta1_1) - ny * cos(theta1_1)) / sin(theta5_2))
    theta6_3 = atan2(-(ox * sin(theta1_2) - oy * cos(theta1_2)) / sin(theta5_3),
                     (nx * sin(theta1_2) - ny * cos(theta1_2)) / sin(theta5_3))
    theta6_4 = atan2(-(ox * sin(theta1_2) - oy * cos(theta1_2)) / sin(theta5_4),
                     (nx * sin(theta1_2) - ny * cos(theta1_2)) / sin(theta5_4))
    # theta2, theta3, theta4 calculate
    q234_1 = atan2(az / sin(theta5_1), -(ax * cos(theta1_1) + ay * sin(theta1_1)) / sin(theta5_1))
    q234_2 = atan2(az / sin(theta5_2), -(ax * cos(theta1_1) + ay * sin(theta1_1)) / sin(theta5_2))
    q234_3 = atan2(az / sin(theta5_3), -(ax * cos(theta1_2) + ay * sin(theta1_2)) / sin(theta5_3))
    q234_4 = atan2(az / sin(theta5_4), -(ax * cos(theta1_2) + ay * sin(theta1_2)) / sin(theta5_4))

    A_1 = px * cos(theta1_1) + py * sin(theta1_1) + d[4] * sin(q234_1) + d[5] * sin(theta5_1) * cos(q234_1)
    B_1 = d[0] - pz - d[4] * cos(q234_1) + d[5] * sin(theta5_1) * sin(q234_1)
    A_2 = px * cos(theta1_1) + py * sin(theta1_1) + d[4] * sin(q234_2) + d[5] * sin(theta5_2) * cos(q234_2)
    B_2 = d[0] - pz - d[4] * cos(q234_2) + d[5] * sin(theta5_2) * sin(q234_2)
    A_3 = px * cos(theta1_2) + py * sin(theta1_2) + d[4] * sin(q234_3) + d[5] * sin(theta5_3) * cos(q234_3)
    B_3 = d[0] - pz - d[4] * cos(q234_3) + d[5] * sin(theta5_3) * sin(q234_3)
    A_4 = px * cos(theta1_2) + py * sin(theta1_2) + d[4] * sin(q234_4) + d[5] * sin(theta5_4) * cos(q234_4)
    B_4 = d[0] - pz - d[4] * cos(q234_4) + d[5] * sin(theta5_4) * sin(q234_4)

    theta2_1 = atan2(A_1 ** 2 + B_1 ** 2 + a[1] ** 2 - a[2] ** 2,
                     sqrt(abs(4 * a[1] ** 2 * (A_1 ** 2 + B_1 ** 2) - (
                             A_1 ** 2 + B_1 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_1, B_1)
    theta2_2 = atan2(A_1 ** 2 + B_1 ** 2 + a[1] ** 2 - a[2] ** 2,
                     -sqrt(abs(4 * a[1] ** 2 * (A_1 ** 2 + B_1 ** 2) - (
                             A_1 ** 2 + B_1 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_1, B_1)
    theta2_3 = atan2(A_2 ** 2 + B_2 ** 2 + a[1] ** 2 - a[2] ** 2,
                     sqrt(abs(4 * a[1] ** 2 * (A_2 ** 2 + B_2 ** 2) - (
                             A_2 ** 2 + B_2 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_2, B_2)
    theta2_4 = atan2(A_2 ** 2 + B_2 ** 2 + a[1] ** 2 - a[2] ** 2,
                     -sqrt(abs(4 * a[1] ** 2 * (A_2 ** 2 + B_2 ** 2) - (
                             A_2 ** 2 + B_2 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_2, B_2)
    theta2_5 = atan2(A_3 ** 2 + B_3 ** 2 + a[1] ** 2 - a[2] ** 2,
                     sqrt(abs(4 * a[1] ** 2 * (A_3 ** 2 + B_3 ** 2) - (
                             A_3 ** 2 + B_3 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_3, B_3)
    theta2_6 = atan2(A_3 ** 2 + B_3 ** 2 + a[1] ** 2 - a[2] ** 2,
                     -sqrt(abs(4 * a[1] ** 2 * (A_3 ** 2 + B_3 ** 2) - (
                             A_3 ** 2 + B_3 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_3, B_3)
    theta2_7 = atan2(A_4 ** 2 + B_4 ** 2 + a[1] ** 2 - a[2] ** 2,
                     sqrt(abs(4 * a[1] ** 2 * (A_4 ** 2 + B_4 ** 2) - (
                             A_4 ** 2 + B_4 ** 2 + a[1] ** 2 - a[2] ** 2) ** 2))) - atan2(A_4, B_4)
    theta2_8 = atan2(A_4 ** 2 + B_4 ** 2 + a[1] ** 2 - a[2] ** 2,
                     -sqrt(abs(4 * a[1] ** 2 * (A_4 ** 2 + B_4 ** 2) - (
                             A_4 ** 2 + B_4 ** 2 + a[1] ** 2 - a[1] ** 2) ** 2))) - atan2(A_4, B_4)

    q23_1 = atan2(d[0] - pz - d[4] * cos(q234_1) + d[5] * sin(theta5_1) * sin(q234_1) - a[1] * sin(theta2_1),
                  px * cos(theta1_1) + py * sin(theta1_1) + d[4] * sin(q234_1) + d[5] * sin(theta5_1) * cos(
                      q234_1) - a[1] * cos(theta2_1))
    q23_2 = atan2(d[0] - pz - d[4] * cos(q234_1) + d[5] * sin(theta5_1) * sin(q234_1) - a[1] * sin(theta2_2),
                  px * cos(theta1_1) + py * sin(theta1_1) + d[4] * sin(q234_1) + d[5] * sin(theta5_1) * cos(
                      q234_1) - a[1] * cos(theta2_2))
    q23_3 = atan2(d[0] - pz - d[4] * cos(q234_2) + d[5] * sin(theta5_2) * sin(q234_2) - a[1] * sin(theta2_3),
                  px * cos(theta1_1) + py * sin(theta1_1) + d[4] * sin(q234_2) + d[5] * sin(theta5_2) * cos(
                      q234_2) - a[1] * cos(theta2_3))
    q23_4 = atan2(d[0] - pz - d[4] * cos(q234_2) + d[5] * sin(theta5_2) * sin(q234_2) - a[1] * sin(theta2_4),
                  px * cos(theta1_1) + py * sin(theta1_1) + d[4] * sin(q234_2) + d[5] * sin(theta5_2) * cos(
                      q234_2) - a[1] * cos(theta2_4))
    q23_5 = atan2(d[0] - pz - d[4] * cos(q234_3) + d[5] * sin(theta5_3) * sin(q234_3) - a[1] * sin(theta2_5),
                  px * cos(theta1_2) + py * sin(theta1_2) + d[4] * sin(q234_3) + d[5] * sin(theta5_3) * cos(
                      q234_3) - a[1] * cos(theta2_5))
    q23_6 = atan2(d[0] - pz - d[4] * cos(q234_3) + d[5] * sin(theta5_3) * sin(q234_3) - a[1] * sin(theta2_6),
                  px * cos(theta1_2) + py * sin(theta1_2) + d[4] * sin(q234_3) + d[5] * sin(theta5_3) * cos(
                      q234_3) - a[1] * cos(theta2_6))
    q23_7 = atan2(d[0] - pz - d[4] * cos(q234_4) + d[5] * sin(theta5_4) * sin(q234_4) - a[1] * sin(theta2_7),
                  px * cos(theta1_2) + py * sin(theta1_2) + d[4] * sin(q234_4) + d[5] * sin(theta5_4) * cos(
                      q234_4) - a[1] * cos(theta2_7))
    q23_8 = atan2(d[0] - pz - d[4] * cos(q234_4) + d[5] * sin(theta5_4) * sin(q234_4) - a[1] * sin(theta2_8),
                  px * cos(theta1_2) + py * sin(theta1_2) + d[4] * sin(q234_4) + d[5] * sin(theta5_4) * cos(
                      q234_4) - a[1] * cos(theta2_8))

    theta3_1 = q23_1 - theta2_1
    theta3_2 = q23_2 - theta2_2
    theta3_3 = q23_3 - theta2_3
    theta3_4 = q23_4 - theta2_4
    theta3_5 = q23_5 - theta2_5
    theta3_6 = q23_6 - theta2_6
    theta3_7 = q23_7 - theta2_7
    theta3_8 = q23_8 - theta2_8

    theta4_1 = q234_1 - q23_1
    theta4_2 = q234_1 - q23_2
    theta4_3 = q234_2 - q23_3
    theta4_4 = q234_2 - q23_4
    theta4_5 = q234_3 - q23_5
    theta4_6 = q234_3 - q23_6
    theta4_7 = q234_4 - q23_7
    theta4_8 = q234_4 - q23_8

    theta_STD = np.array([[theta1_1, theta2_1, theta3_1, theta4_1, theta5_1, theta6_1],
                          [theta1_1, theta2_2, theta3_2, theta4_2, theta5_1, theta6_1],
                          [theta1_1, theta2_3, theta3_3, theta4_3, theta5_2, theta6_2],
                          [theta1_1, theta2_4, theta3_4, theta4_4, theta5_2, theta6_2],
                          [theta1_2, theta2_5, theta3_5, theta4_5, theta5_3, theta6_3],
                          [theta1_2, theta2_6, theta3_6, theta4_6, theta5_3, theta6_3],
                          [theta1_2, theta2_7, theta3_7, theta4_7, theta5_4, theta6_4],
                          [theta1_2, theta2_8, theta3_8, theta4_8, theta5_4, theta6_4]])
    return theta_STD


def main1():
    ARMPI_HALF = math.pi / 2
    # O = np.zeros((4, 4))

    # D-H parameters
    alpha = np.array([ARMPI_HALF, 0, 0, ARMPI_HALF, -ARMPI_HALF, 0])
    a = np.array([0, -4.025, -0.39225, 0, 0, 0])
    d = np.array([0.089459, 0, 0, 0.10915, 0.09465, 0.0823])
    theta = np.array([0, 0, 0, 0, 0, 0])
    # test
    start_angle = np.array([pi / 4, pi / 4, pi / 4, pi / 4, pi / 4, pi / 4])
    matrix, M_trans = RobotOnKine(alpha, a, d, theta + start_angle)
    print('trans={}'.format(M_trans))


def main():
    # ARMPI_HALF = math.pi / 2
    # O = np.zeros((4, 4))

    # D-H parameters
    # alpha = np.array([ARMPI_HALF, 0, 0, ARMPI_HALF, -ARMPI_HALF, 0])
    a = np.array([0, -4.025, -0.39225, 0, 0, 0])
    d = np.array([0.089459, 0, 0, 0.10915, 0.09465, 0.0823])
    # theta = np.array([0, 0, 0, 0, 0, 0])
    A = np.array([[1, 0, 0, 1],
                  [0, 1, 0, 1],
                  [0, 0, 1, 1],
                  [0, 0, 0, 1]])
    theta_cal = RobotOnKineInv(A, a, d)
    print(theta_cal)


if __name__ == '__main__':
    main1()
