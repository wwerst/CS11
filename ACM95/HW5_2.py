import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np
from copy import deepcopy


def iterate(t=100, dt=0.001):
    bug_pos = np.array(
        [+0.5 + 0.5j,
         -0.5 + 0.5j,
         -0.5 - 0.5j,
         +0.5 - 0.5j],
        dtype=np.clongdouble)
    bug1_pos = []
    for i in range(int(t/dt)):
        bug_dt = np.roll(bug_pos, -1) - bug_pos
        bug_pos += bug_dt * dt
        bug1_pos.append(deepcopy(bug_pos[0]))
    return bug1_pos


def main():
    points = iterate()
    points = np.array(points)
    angles = np.log(points).imag
    delta_angles = np.diff(angles)
    sum_angles = np.mod(delta_angles, 2*np.pi).sum()
    print(sum_angles)
    line_integral = np.abs(np.diff(points)).sum()
    print(line_integral)
    plt.plot(points.real, points.imag)
    plt.show()


if __name__ == '__main__':
    main()
