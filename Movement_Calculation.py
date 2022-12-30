import numpy as np
import math
import matplotlib.pyplot as plt


# movement calculation function, change one dimension in x, y, z in one step
def mov(start_point, end_point, start_v, end_v, v_max, a_max, j_max):
    # Hyperparameter preset
    change_dim = 0  # [x, y, z] -> [0, 1, 2]
    q0 = start_point[change_dim]  # start point, input np.array for 3 dimension, for example: [0, 0, 0]
    q1 = end_point[change_dim]  # end point, input np.array for 3 dimension, for example: [100, 0, 0]
    v0 = start_v  # start velocity
    v1 = end_v  # end velocity
    vmax = v_max  # maximum velocity, a limitation for velocity
    amax = a_max  # maximum acceleration, a limitation for acceleration
    jmax = j_max  # maximum jerk, a limitation for acceleration stage

    # assume it's possible to achieve max_v
    # judge whether achieve max_v from start point
    if (vmax - v0) * jmax < amax * amax:  # if it's impossible for velocity to achieve max_velocity
        if v0 > vmax:  # classified discussion, velocity keep the same in start stage
            Tj1 = 0
            Ta = 0
            alima = 0
        else:  # velocity increase in start stage
            Tj1 = math.sqrt((vmax - v0) / jmax)
            Ta = 2 * Tj1  # accelerating time
            alima = Tj1 * jmax  # acceleration limitation in the start stage
    else:  # it's possible for velocity to achieve the max_velocity
        Tj1 = amax / jmax
        Ta = Tj1 + (vmax - v0) / amax
        alima = amax

    # judge whether achieve max_v from end point
    if (vmax - v1) * jmax < amax * amax:  # if it's impossible for velocity to achieve max_velocity
        Tj2 = math.sqrt((vmax - v1) / jmax)
        Td = 2 * Tj1  # acceleration decrease time in end stage
        alimd = Tj2 * jmax  # acceleration limitation in the end stage
    else:  # it's possible for velocity to achieve the max_velocity
        Tj2 = amax / jmax
        Td = Tj2 + (vmax - v1) / amax
        alimd = amax
    # calculate constant velocity travel time through Ta and Td, use the v-t function image to calculate
    Tv = (q1 - q0) / vmax - Ta / 2 * (1 + v0 / vmax) - Td / 2 * (1 + v1 / vmax)
    # Above is calculated with the assumption that it's possible to achieve the maximum velocity,
    # so the calculated Tv might be negative,
    # which needs to calculate in other method
    if Tv > 0:  # Tv > 0 means being able to achieve max_v
        vlim = vmax  # the limitation of velocity is maximum velocity
        T = Tv + Ta + Td  # calculate all the time
    else:  # Tv < 0 means it's impossible to achieve max_v, recalculate
        # initialize the parameters
        Tv = 0
        amax_org = amax
        delta = (amax ** 4) / (jmax * jmax) + 2 * (v0 * v0 + v1 * v1) + amax * (
                4 * (q1 - q0) - 2 * amax / jmax * (v0 + v1))
        Tj1 = amax / jmax
        Ta = (amax * amax / jmax - 2 * v0 + math.sqrt(delta)) / 2 / amax
        Tj2 = amax / jmax
        Td = (amax * amax / jmax - 2 * v1 + math.sqrt(delta)) / 2 / amax
        vlim = v0 + (Ta - Tj1) * alima
        while Ta < 2 * Tj1 or Td < 2 * Tj2:  # if it's possible for the movement
            amax = amax - amax_org * 0.1  # set the learning rate
            alima = amax
            alimd = amax
            if amax > 0:  # classify according to acceleration and deceleration
                delta = (amax ** 4) / (jmax * jmax) + 2 * (v0 * v0 + v1 * v1) + amax * (
                            4 * (q1 - q0) - 2 * amax / jmax * (v0 + v1))
            else:
                delta = (amax ** 4) / (jmax * jmax) + 2 * (v0 * v0 + v1 * v1) - amax * (
                            4 * (q1 - q0) - 2 * amax / jmax * (v0 + v1))
            Tj1 = amax / jmax
            Ta = (amax * amax / jmax - 2 * v0 + math.sqrt(delta)) / 2 / amax
            Tj2 = amax / jmax
            Td = (amax * amax / jmax - 2 * v1 + math.sqrt(delta)) / 2 / amax
            vlim = v0 + (Ta - Tj1) * alima
        # if velocity hold the same in start stage or end stage, Ta or Td might become negative
        if Ta < 0 or Td < 0:
            if v0 > v1:
                Ta = 0
                Tj1 = 0
                alima = 0
                Td = 2 * (q1 - q0) / (v1 + v0)
                Tj2 = (jmax * (q1 - q0) - math.sqrt(
                    (jmax * (jmax * (q1 - q0) * (q1 - q0) + (v1 + v0) * (v1 + v0) * (v1 - v0))))) / jmax / (v1 + v0)
                alimd = -jmax * Tj2
                vlim = v1 - (Td - Tj2) * alimd
                alimd = -alimd
            else:
                Td = 0
                Tj2 = 0
                Ta = 2 * (q1 - q0) / (v1 + v0)
                Tj1 = (jmax * (q1 - q0) - math.sqrt(
                    (jmax * (jmax * (q1 - q0) * (q1 - q0) - (v1 + v0) * (v1 + v0) * (v1 - v0))))) / jmax / (v1 + v0)
                alima = jmax * Tj1
                vlim = v0 + (Ta - Tj1) * alima
        # calculate all the time
        T = Tv + Ta + Td

    # calculate p, v, a, j and output in each part
    pvaj = np.zeros((5, math.ceil(500 * T)))
    for i in range(1, math.ceil(500 * T)):
        t = i / 500
        pvaj[4, i] = i
        if t == T:
            break
        if not t < 0 and t < Tj1:  # part1: velocity increase with increasing acceleration
            q = q0 + v0 * t + jmax * t * t * t / 6
            v = v0 + jmax * t * t / 2
            a = jmax * t
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = jmax
        elif not t < Tj1 and t < (Ta - Tj1):  # part2: velocity increase with same acceleration
            q = q0 + v0 * t + alima / 6 * (3 * t * t - 3 * Tj1 * t + Tj1 * Tj1)
            v = v0 + alima * (t - Tj1 / 2)
            a = alima
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = 0
        elif not t < (Ta - Tj1) and t < Ta:  # part3: velocity increase with decreasing acceleration
            q = q0 + (vlim + v0) * Ta / 2 - vlim * (Ta - t) + jmax * (Ta - t) * (Ta - t) * (Ta - t) / 6
            v = vlim - jmax * (Ta - t) * (Ta - t) / 2
            a = jmax * (Ta - t)
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = -jmax
        elif not t < Ta and t < (Ta + Tv):  # part4: motivate with same velocity
            q = q0 + (vlim + v0) * Ta / 2 + vlim * (t - Ta)
            v = vlim
            a = 0
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = 0
        elif not t < (Ta + Tv) and t < (T - Td + Tj2):  # part5: velocity decrease with increasing acceleration
            q = q1 - (vlim + v1) * Td / 2 + vlim * (t - T + Td) - jmax * (t - T + Td) * (t - T + Td) * (t - T + Td) / 6
            v = vlim - jmax * (t - T + Td) * (t - T + Td) / 2
            a = -jmax * (t - T + Td)
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = -jmax
        elif not t < (T - Td + Tj2) and t < (T - Tj2):  # part6: velocity decrease with same acceleration
            q = q1 - (vlim + v1) * Td / 2 + vlim * (t - T + Td) - alimd / 6 * (
                        3 * (t - T + Td) * (t - T + Td) - 3 * Tj2 * (t - T + Td) + Tj2 * Tj2)
            v = vlim - alimd * (t - T + Td - Tj2 / 2)
            a = -alimd
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = 0
        elif not t < (T - Tj2) and t < T:  # part7: velocity decrease with decreasing acceleration
            q = q1 - v1 * (T - t) - jmax * (T - t) * (T - t) * (T - t) / 6
            v = v1 + jmax * (T - t) * (T - t) / 2
            a = -jmax * (T - t)
            pvaj[0, i] = q
            pvaj[1, i] = v
            pvaj[2, i] = a
            pvaj[3, i] = jmax
    return pvaj


# visualize the solution
def plotfig(y, x, label=None, title=None, alpha=0.5):  # plot figure function
    plt.plot(x * 2, y, alpha=alpha)
    plt.xlabel('time/ms')
    plt.ylabel(label)
    plt.title(title)
    # plt.savefig(title)
    # print('Already save figure{}'.format(title))
    plt.show()


def movement_test():
    # input start position and end position
    start_pos = np.array([0, 0, 0])
    end_pos = np.array([100, 0, 0])
    [a, b, c, d, t] = mov(start_pos, end_pos, 0, 0, 400, 20, 150)
    print(a, b, c, d, t)


if __name__ == '__main__':
    movement_test()
