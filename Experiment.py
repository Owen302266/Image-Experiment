from Movement_Calculation import *
from DH_Calculate import *


def exp1():  # exp1
    output = DH()
    # start_point = np.array([pi / 4, pi / 4, pi / 4, pi / 4, pi / 4, pi / 4])  # input start theta
    start_point = np.array([0, pi / 2, -pi / 6, 0, 0, 0])  # input start theta
    end_point = output.forward(start_point)  # forward calculate
    print(end_point)


def exp3():  # exp3
    step = 0.001  # (m)
    total_step = 0.5  # (m)
    output = DH()
    start_point = np.array([pi / 4, pi / 4, pi / 4, pi / 4, pi / 4, pi / 4])  # input start theta
    start_point_end = output.forward(start_point)  # forward calculate
    with open('exp3_out.txt', 'w') as f:  # open txt file to save data
        for dim in range(3):  # 3 dimensions
            for count in range(int(total_step / step)):  # running step
                start_point_end[dim][3] += step  # endpoint change
                end = output.inverse_kine(start_point_end)  # inverse kinematics
                data = 360 / 2 / pi * np.array(end[0])  # change data type
                print(count + 1, ": ", data)  # output the solution
                # save output to txt
                # np.savetxt(f, [data], fmt="%.8f", delimiter=" ")  # this is available with warnings


def exp5():  # exp5
    # input start position and end position
    start_pos = np.array([0, 0, 0])
    end_pos = np.array([100, 0, 0])
    [a, b, c, d, t] = mov(start_pos, end_pos, 0, 0, 400, 20, 150)  # calculate
    # plot figures and save
    plotfig(a, t, 'x/mm', 'Change curve of movement relative to time')
    plotfig(b, t, 'v/(mm/ms)', 'Change curve of velocity relative to time')
    plotfig(c, t, 'a/(mm/ms^2)', 'Change curve of acceleration relative to time')
    plotfig(d, t, 'j/(mm/ms^3)', 'Change curve of jerk relative to time')


if __name__ == '__main__':
    exp1()
