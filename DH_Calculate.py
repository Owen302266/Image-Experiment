import numpy as np
from math import *


class DH:
    def __init__(self):
        # config DH parameters, represents theta, d, a, alpha
        self.DHarray = np.array([[0, 0.089459, 0, pi / 2],
                                 [0, 0, -0.425, 0],
                                 [0, 0, -0.39225, 0],
                                 [0, 0.10915, 0, pi / 2],
                                 [0, 0.09465, 0, -pi / 2],
                                 [0, 0.0823, 0, 0]])

    def theta(self, i):  # output theta parameter
        return self.DHarray[i - 1, 0]

    def d(self, i):  # output d parameter
        return self.DHarray[i - 1, 1]

    def a(self, i):  # output a parameter
        return self.DHarray[i - 1, 2]

    def alf(self, i):  # output alf parameter
        return self.DHarray[i - 1, 3]

    # noinspection PyMethodMayBeStatic
    def rotation_x(self, alfa):  # rotate of x-axis
        return np.array([[1, 0, 0, 0],
                        [0, cos(alfa), -sin(alfa), 0],
                        [0, sin(alfa), cos(alfa), 0],
                        [0, 0, 0, 1]])

    # noinspection PyMethodMayBeStatic
    def rotation_y(self, theta):  # rotate of y-axis
        return np.array([[cos(theta), 0, sin(theta), 0],
                        [0, 1, 0, 0],
                        [-sin(theta), 0, cos(theta), 0],
                        [0, 0, 0, 1]])

    # noinspection PyMethodMayBeStatic
    def rotation_z(self, angle):  # rotate of z-axis
        return np.array([[cos(angle), -sin(angle), 0, 0],
                        [sin(angle), cos(angle), 0, 0],
                        [0, 0, 1, 0],
                        [0, 0, 0, 1]])

    def pose(self, pose):  # pose x, y, z, roll, pitch, yaw
        matrix = []
        if len(pose) == 3:
            matrix = self.rotation_z(pose[5]) @ self.rotation_y(pose[4]) @ self.rotation_x(pose[3])
            matrix[0, 3] = pose[0]
            matrix[1, 3] = pose[1]
            matrix[2, 3] = pose[2]
        return matrix

    def Trans(self, i, angle):  # output transfer matrix
        sa = sin(self.alf(i))
        ca = cos(self.alf(i))
        st = sin(angle)
        ct = cos(angle)
        return np.array([[ct, -st * ca, st * sa, self.a(i) * ct],
                         [st, ct * ca, -ct * sa, self.a(i) * st],
                         [0, sa, ca, self.d(i)],
                         [0, 0, 0, 1]])

    def forward(self, matrix):  # forward kinematics, output transfer matrix product
        return self.Trans(1, matrix[0]) @ self.Trans(2, matrix[1]) @ self.Trans(3, matrix[2]) \
               @ self.Trans(4, matrix[3]) @ self.Trans(5, matrix[4]) @ self.Trans(6, matrix[5])

    # noinspection PyMethodMayBeStatic
    def output_range(self, matrix):  # output range in [-pi, pi]
        for item in matrix:
            if item > pi:
                item -= 2 * pi
            if item < -pi:
                item += 2 * pi
        return matrix

    def inverse_kine(self, pose):  # inverse kinematics, input pose matrix, output analytical solution
        # target pose matrix
        nx = pose[0, 0]
        ox = pose[0, 1]
        ax = pose[0, 2]
        px = pose[0, 3]
        ny = pose[1, 0]
        oy = pose[1, 1]
        ay = pose[1, 2]
        py = pose[1, 3]
        nz = pose[2, 0]
        oz = pose[2, 1]
        az = pose[2, 2]
        pz = pose[2, 3]
        '''pose matrix = [[nx, ox, ax, px]
                          [ny, oy, ay, py]
                          [nz, oz, az, pz]
                          [0, 0, 0, 1]]'''
        judge = np.empty(shape=4)
        theta_STD = list()
        # calculate analytical solutions
        # analytical solution matrix shape
        '''theta_STD = np.array([[theta1_1, theta2_1, theta3_1, theta4_1, theta5_1, theta6_1],
                                 [theta1_1, theta2_2, theta3_2, theta4_2, theta5_1, theta6_1],
                                 [theta1_1, theta2_3, theta3_3, theta4_3, theta5_2, theta6_2],
                                 [theta1_1, theta2_4, theta3_4, theta4_4, theta5_2, theta6_2],
                                 [theta1_2, theta2_5, theta3_5, theta4_5, theta5_3, theta6_3],
                                 [theta1_2, theta2_6, theta3_6, theta4_6, theta5_3, theta6_3],
                                 [theta1_2, theta2_7, theta3_7, theta4_7, theta5_4, theta6_4],
                                 [theta1_2, theta2_8, theta3_8, theta4_8, theta5_4, theta6_4]])'''
        # theta1 = arctan((d6ay-py)/(d6ax-px))-arctan(d4/+-(sqrt((d6ay-py)^2+(d6ax-px)^2-d4^2))
        theta1_1 = atan2(self.d(6) * ay - py, self.d(6) * ax - px) - atan2(self.d(4), sqrt(
            (self.d(6) * ay - py) ** 2 + (self.d(6) * ax - px) ** 2 - self.d(4) ** 2))
        theta1_2 = atan2(self.d(6) * ay - py, self.d(6) * ax - px) - atan2(self.d(4), -sqrt(
            (self.d(6) * ay - py) ** 2 + (self.d(6) * ax - px) ** 2 - self.d(4) ** 2))

        # theta5 = arctan(+-sqrt((nxs1-nyc1))^2 +(oxs1-oyc1)^2)/(axs1-ayc1))
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

        # theta6 = arctan((-oxs1-oyc1/s5)/(nxs1-nyc1/s5)
        theta6_1 = atan2(-(ox * sin(theta1_1) - oy * cos(theta1_1)) / sin(theta5_1),
                         (nx * sin(theta1_1) - ny * cos(theta1_1)) / sin(theta5_1))
        theta6_2 = atan2(-(ox * sin(theta1_1) - oy * cos(theta1_1)) / sin(theta5_2),
                         (nx * sin(theta1_1) - ny * cos(theta1_1)) / sin(theta5_2))
        theta6_3 = atan2(-(ox * sin(theta1_2) - oy * cos(theta1_2)) / sin(theta5_3),
                         (nx * sin(theta1_2) - ny * cos(theta1_2)) / sin(theta5_3))
        theta6_4 = atan2(-(ox * sin(theta1_2) - oy * cos(theta1_2)) / sin(theta5_4),
                         (nx * sin(theta1_2) - ny * cos(theta1_2)) / sin(theta5_4))
        # theta2, theta3, theta4
        # A = d5(s6nxc1+nys1)+c6(oxc1+oys1)+pxc1-d6(axc1+ays1+pys1)
        A_1 = self.d(5) * ((sin(theta6_1) * (nx * cos(theta1_1) + ny * sin(theta1_1))) + (
                cos(theta6_1) * (ox * cos(theta1_1) + oy * sin(theta1_1)))) + px * cos(theta1_1) - self.d(6) * (
                      ax * cos(theta1_1) + ay * sin(theta1_1)) + py * sin(theta1_1)
        A_2 = self.d(5) * ((sin(theta6_2) * (nx * cos(theta1_1) + ny * sin(theta1_1))) + (
                cos(theta6_2) * (ox * cos(theta1_1) + oy * sin(theta1_1)))) + px * cos(theta1_1) - self.d(6) * (
                      ax * cos(theta1_1) + ay * sin(theta1_1)) + py * sin(theta1_1)
        A_3 = self.d(5) * ((sin(theta6_3) * (nx * cos(theta1_2) + ny * sin(theta1_2))) + (
                cos(theta6_3) * (ox * cos(theta1_2) + oy * sin(theta1_2)))) + px * cos(theta1_2) - self.d(6) * (
                      ax * cos(theta1_2) + ay * sin(theta1_2)) + py * sin(theta1_2)
        A_4 = self.d(5) * ((sin(theta6_4) * (nx * cos(theta1_2) + ny * sin(theta1_2))) + (
                cos(theta6_4) * (ox * cos(theta1_2) + oy * sin(theta1_2)))) + px * cos(theta1_2) - self.d(6) * (
                      ax * cos(theta1_2) + ay * sin(theta1_2)) + py * sin(theta1_2)

        # B = d5(ozc6+nzs6)+pz-d-azd6
        B_1 = self.d(5) * (oz * cos(theta6_1) + nz * sin(theta6_1)) + pz - self.d(1) - az * self.d(6)
        B_2 = self.d(5) * (oz * cos(theta6_2) + nz * sin(theta6_2)) + pz - self.d(1) - az * self.d(6)
        B_3 = self.d(5) * (oz * cos(theta6_3) + nz * sin(theta6_3)) + pz - self.d(1) - az * self.d(6)
        B_4 = self.d(5) * (oz * cos(theta6_4) + nz * sin(theta6_4)) + pz - self.d(1) - az * self.d(6)

        # parameters for theta4
        # q4 = s6(nxc1+nys1)+c6(oxc1+oys1)
        q4_1 = sin(theta6_1) * (nx * cos(theta1_1) + ny * sin(theta1_1)) + cos(theta6_1) * (
                ox * cos(theta1_1) + oy * sin(theta1_1))
        q4_2 = sin(theta6_2) * (nx * cos(theta1_1) + ny * sin(theta1_1)) + cos(theta6_2) * (
                ox * cos(theta1_1) + oy * sin(theta1_1))
        q4_3 = sin(theta6_3) * (nx * cos(theta1_2) + ny * sin(theta1_2)) + cos(theta6_3) * (
                ox * cos(theta1_2) + oy * sin(theta1_2))
        q4_4 = sin(theta6_4) * (nx * cos(theta1_2) + ny * sin(theta1_2)) + cos(theta6_4) * (
                ox * cos(theta1_2) + oy * sin(theta1_2))

        # judge whether the algorithm is available, according to the domain of definition of arccos is [-1, 1]
        judge[0] = (A_1 ** 2 + B_1 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3))
        judge[1] = (A_2 ** 2 + B_2 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3))
        judge[2] = (A_3 ** 2 + B_3 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3))
        judge[3] = (A_4 ** 2 + B_4 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3))
        # with 4 group parameters, divide the algorithm into 4 categories
        if 1 > judge[0] > -1:
            # calculate theta3 with group1
            # theta3 = arccos((A^2+B^2-a2^2-a3^2)/(2a2a3))
            theta3_1 = acos((A_1 ** 2 + B_1 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            theta3_2 = - acos((A_1 ** 2 + B_1 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            # parameters for theta2
            # q2 = ((a3c3+a2)B-a3s3A)/(a2^2+a3^2+2a2a3c3)
            q2_1 = ((self.a(3) * cos(theta3_1) + self.a(2)) * B_1 - self.a(3) * sin(theta3_1) * A_1) / (
                        self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_1))
            q2_2 = ((self.a(3) * cos(theta3_2) + self.a(2)) * B_1 - self.a(3) * sin(theta3_2) * A_1) / (
                        self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_2))
            # calculate theta2 with group1
            # theta2 = arctan(q2/((A+a3s3q2)/a3c3+a2))
            theta2_1 = atan2(q2_1, ((A_1 + self.a(3) * sin(theta3_1) * q2_1) / (self.a(3) * cos(theta3_1) + self.a(2))))
            theta2_2 = atan2(q2_2, ((A_1 + self.a(3) * sin(theta3_2) * q2_2) / (self.a(3) * cos(theta3_2) + self.a(2))))
            # calculate theta4 with group1
            # theta4 = arctan(-q4/(ozc6+nzs6))-theta2-theta3
            theta4_1 = atan2(-q4_1, (oz * cos(theta6_1) + nz * sin(theta6_1))) - theta2_1 - theta3_1
            theta4_2 = atan2(-q4_1, (oz * cos(theta6_1) + nz * sin(theta6_1))) - theta2_2 - theta3_2
            # reshape theta as an analytical solution
            STD1 = np.array([theta1_1, theta2_1, theta3_1, theta4_1, theta5_1, theta6_1])
            STD2 = np.array([theta1_1, theta2_2, theta3_2, theta4_2, theta5_1, theta6_1])
            # add to output and correct the range
            theta_STD.append(self.output_range(STD1))
            theta_STD.append(self.output_range(STD2))
        else:
            print('solution group1 is unavailable')

        if 1 > judge[1] > -1:
            # calculate theta3 with group2
            theta3_3 = acos((A_2 ** 2 + B_2 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            theta3_4 = - acos((A_2 ** 2 + B_2 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            # parameters for theta2
            q2_3 = ((self.a(3) * cos(theta3_3) + self.a(2)) * B_2 - self.a(3) * sin(theta3_3) * A_2) / (
                    self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_3))
            q2_4 = ((self.a(3) * cos(theta3_4) + self.a(2)) * B_2 - self.a(3) * sin(theta3_4) * A_2) / (
                    self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_4))
            # calculate theta2 with group2
            theta2_3 = atan2(q2_3, ((A_2 + self.a(3) * sin(theta3_3) * q2_3) / (self.a(3) * cos(theta3_3) + self.a(2))))
            theta2_4 = atan2(q2_4, ((A_2 + self.a(3) * sin(theta3_4) * q2_4) / (self.a(3) * cos(theta3_4) + self.a(2))))
            # calculate theta4 with group2
            theta4_3 = atan2(-q4_2, (oz * cos(theta6_2) + nz * sin(theta6_2))) - theta2_3 - theta3_3
            theta4_4 = atan2(-q4_2, (oz * cos(theta6_2) + nz * sin(theta6_2))) - theta2_4 - theta3_4
            # reshape theta as an analytical solution
            STD3 = np.array([theta1_1, theta2_3, theta3_3, theta4_3, theta5_2, theta6_2])
            STD4 = np.array([theta1_1, theta2_4, theta3_4, theta4_4, theta5_2, theta6_2])
            # add to output and correct the range
            theta_STD.append(self.output_range(STD3))
            theta_STD.append(self.output_range(STD4))
        else:
            print('solution group2 is unavailable')

        if 1 > judge[2] > -1:
            # calculate theta3 with group3
            theta3_5 = acos((A_3 ** 2 + B_3 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            theta3_6 = - acos((A_3 ** 2 + B_3 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            # parameters for theta2
            q2_5 = ((self.a(3) * cos(theta3_5) + self.a(2)) * B_3 - self.a(3) * sin(theta3_5) * A_3) / (
                    self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_5))
            q2_6 = ((self.a(3) * cos(theta3_6) + self.a(2)) * B_3 - self.a(3) * sin(theta3_6) * A_3) / (
                    self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_6))
            # calculate theta2 with group3
            theta2_5 = atan2(q2_5, ((A_3 + self.a(3) * sin(theta3_5) * q2_5) / (self.a(3) * cos(theta3_5) + self.a(2))))
            theta2_6 = atan2(q2_6, ((A_3 + self.a(3) * sin(theta3_6) * q2_6) / (self.a(3) * cos(theta3_6) + self.a(2))))
            # calculate theta4 with group3
            theta4_5 = atan2(-q4_3, (oz * cos(theta6_3) + nz * sin(theta6_3))) - theta2_5 - theta3_5
            theta4_6 = atan2(-q4_3, (oz * cos(theta6_3) + nz * sin(theta6_3))) - theta2_6 - theta3_6
            # reshape theta as an analytical solution
            STD5 = np.array([theta1_2, theta2_5, theta3_5, theta4_5, theta5_3, theta6_3])
            STD6 = np.array([theta1_2, theta2_6, theta3_6, theta4_6, theta5_3, theta6_3])
            # add to output and correct the range
            theta_STD.append(self.output_range(STD5))
            theta_STD.append(self.output_range(STD6))
        else:
            print('solution group3 is unavailable')

        if 1 > judge[3] > -1:
            # calculate theta3 with group4
            theta3_7 = acos((A_4 ** 2 + B_4 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            theta3_8 = - acos((A_4 ** 2 + B_4 ** 2 - self.a(2) ** 2 - self.a(3) ** 2) / (2 * self.a(2) * self.a(3)))
            # parameters for theta2
            q2_7 = ((self.a(3) * cos(theta3_7) + self.a(2)) * B_4 - self.a(3) * sin(theta3_7) * A_4) / (
                    self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_7))
            q2_8 = ((self.a(3) * cos(theta3_8) + self.a(2)) * B_4 - self.a(3) * sin(theta3_8) * A_4) / (
                    self.a(2) ** 2 + self.a(3) ** 2 + 2 * self.a(2) * self.a(3) * cos(theta3_8))
            # calculate theta2 with group4
            theta2_7 = atan2(q2_7, ((A_4 + self.a(3) * sin(theta3_7) * q2_7) / (self.a(3) * cos(theta3_7) + self.a(2))))
            theta2_8 = atan2(q2_8, ((A_4 + self.a(3) * sin(theta3_8) * q2_8) / (self.a(3) * cos(theta3_8) + self.a(2))))
            # calculate theta4 with group4
            theta4_7 = atan2(-q4_4, (oz * cos(theta6_4) + nz * sin(theta6_4))) - theta2_7 - theta3_7
            theta4_8 = atan2(-q4_4, (oz * cos(theta6_4) + nz * sin(theta6_4))) - theta2_8 - theta3_8
            # reshape theta as an analytical solution
            STD7 = np.array([theta1_2, theta2_7, theta3_7, theta4_7, theta5_4, theta6_4])
            STD8 = np.array([theta1_2, theta2_8, theta3_8, theta4_8, theta5_4, theta6_4])
            # add to output and correct the range
            theta_STD.append(self.output_range(STD7))
            theta_STD.append(self.output_range(STD8))
        else:
            print('solution group4 is unavailable')
        # print('calculate finish')
        return theta_STD


def DH_forward_test():  # test forward kinematics
    output = DH()
    start_point = np.array([pi / 4, pi / 4, pi / 4, pi / 4, pi / 4, pi / 4])
    end_point = output.forward(start_point)
    print(end_point)


def inverse_kine_test():  # test backward kinematics
    output = DH()
    start_point = np.array([pi / 4, pi / 4, pi / 4, pi / 4, pi / 4, pi / 4])
    start_point_end = output.forward(start_point)
    end_point = output.inverse_kine(start_point_end)
    print(end_point)


if __name__ == '__main__':
    inverse_kine_test()
