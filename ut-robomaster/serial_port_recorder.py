import serial
import time
# from numpy import Infinity
# import matplotlib.pyplot as plt

header = 'abc'
port = '/dev/ttyUSB0'
dump = 'serial_data.txt'
baud = 115200
interval = 0.1
read_delay = 0.1


def read_data(sp: serial.Serial, cmd):
    sp.write(cmd)
    time.sleep(read_delay)
    raw = sp.readline()
    data = raw.decode('ascii')
    return data


# xa = []
# ya = [[], [], []]
# max_val = -Infinity
# min_val = Infinity
events = [(5, 0.25), (10, 0),
          (15, 0.5), (20, 0),
          (25, 0.75), (30, 0),
          (35, 1), (40, 0),
          (45, 0)]
arg = 0

try:
    with serial.Serial(port, baud) as sp, open(dump, ('w')) as file:
        # plt.xlabel('input')
        # plt.ylabel('output')
        # plt.title('Step Response Data')
        # plt.tight_layout()
        # ln = plt.plot([], [], '-r', [], [], '-g', [], [], '-b')

        start_time = time.time()
        last_time = start_time

        while True:
            st = time.perf_counter()
            t = time.time() - start_time

            if len(events) > 0:
                event_time, new_arg = events[0]
                if t >= event_time:
                    arg = new_arg
                    events.pop(0)
                    if len(events) == 0:
                        break

            data = read_data(sp, f'{header} {arg}')
            file.write(f'{t:.2f}, {arg}, {data}')
            file.flush()

            # xa.append(t)

            # vals = get_temperature(device)
            # h_val = 1 if heaters_on else 0
            # file.write(f'{t:.1f}, {h_val}, {vals[0]}, {vals[1]}, {vals[2]}\n')
            # file.flush()

            # for (i, val) in enumerate(vals):
            #     max_val = max(max_val, val)
            #     min_val = min(min_val, val)

            #     ya[i].append(val)
            #     ln[i].set_data(xa, ya[i])

            # plt.xlim(0, t)
            # plt.ylim(min_val - 0.05, max_val + 0.05)

            wait_time = interval - (time.perf_counter() - st)
            time.sleep(wait_time)
            # plt.pause(wait_time)
except IOError as ex:
    print(ex)
except KeyboardInterrupt:
    pass

# plt.show()
