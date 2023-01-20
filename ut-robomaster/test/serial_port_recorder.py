import serial
import time
# from numpy import Infinity
# import matplotlib.pyplot as plt

header = 'abc'
port = '/dev/ttyUSB0'
dump = 'serial_data.txt'
baud = 115200
interval = 0.1
read_delay = 0.001


def read_data(sp: serial.Serial, cmd: str):
    sp.write(cmd.encode('ascii'))
    time.sleep(read_delay)
    raw = sp.readline()
    data = raw.decode('ascii')
    vars = [float(x.strip()) for x in data.split(",")]
    return vars


# xa = []
# ya = [[], [], []]
# max_val = -Infinity
# min_val = Infinity
events = [(5, 0.25), (15, 0),
          (20, 0.5), (30, 0),
          (35, 0.75), (45, 0),
          (50, 1.0), (60, 0),
          (65, 0)]
arg = 0

try:
    with serial.Serial(port, baud) as sp, open(dump, ('w')) as file:
        # plt.xlabel('input')
        # plt.ylabel('output')
        # plt.title('Step Response Data')
        # plt.tight_layout()
        # ln = plt.plot([], [], '-r', [], [], '-g', [], [], '-b')

        print('start')

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

            vars = read_data(sp, f'{header} {arg}\n')
            vars_str = ', '.join([str(x) for x in vars])
            out_line = f'{t:.2f}, {arg}, {vars_str}\n'
            print(out_line, end='')
            file.write(out_line)
            # file.flush()

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
