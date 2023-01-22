import serial
import time
import argparse
import sys
import logging
# from numpy import Infinity
# import matplotlib.pyplot as plt


def read_data(sp: serial.Serial, cmd: str):
    sp.write(cmd.encode('ascii'))
    time.sleep(read_delay)
    raw = sp.readline()
    data = raw.decode('ascii')
    vars = [float(x.strip()) for x in data.split(",")]
    return vars


def event_type(s):
    try:
        t, a = s.split(',')
        return float(t), a
    except:
        raise argparse.ArgumentTypeError(
            "Events must be of the form: seconds,argument")


arg_parser = argparse.ArgumentParser(
    prog='serial_port_recorder',
    description='Requests and logs data from the chosen serial port',
)

arg_parser.add_argument('request', type=str,
                        help='format of request to send before each reading (example: \'test {}\')')
arg_parser.add_argument(
    'port', type=str, help='the serial port to connect to')
arg_parser.add_argument('events', nargs='+',
                        type=event_type, help='events to execute while logging (format: \'seconds,argument seconds,argument ...\')')
arg_parser.add_argument('-b', '--baud',
                        default=115200, type=int, help='desired baud rate')
arg_parser.add_argument('-i', '--interval',
                        default=0.5, type=float, help='reading interval (s)')
arg_parser.add_argument('-r', '--read-delay',
                        default=0.001, type=float, help='delay between sending request and reading response (s)')
arg_parser.add_argument('-v', '--verbose', action='store_true')


args = arg_parser.parse_args()
request = args.request
port = args.port
baud = args.baud
interval = args.interval
read_delay = args.read_delay
events = args.events
arg = None
# xa = []
# ya = [[], [], []]
# max_val = -Infinity
# min_val = Infinity

logging.basicConfig(format='%(message)s',
                    level=logging.INFO if args.verbose else logging.ERROR)

try:
    with serial.Serial(port, baud) as sp:
        # plt.xlabel('input')
        # plt.ylabel('output')
        # plt.title('Step Response Data')
        # plt.tight_layout()
        # ln = plt.plot([], [], '-r', [], [], '-g', [], [], '-b')

        logging.info('Connected')

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
                    logging.info(f'Event reached at {t}s. New argument: {arg}')
                    if len(events) == 0:
                        break

            vars = read_data(sp, request.format(arg) + '\n')
            vars_str = ', '.join([str(x) for x in vars])
            print(f'{t:.2f}, {arg}, {vars_str}')

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

    logging.info('Done')
except IOError as ex:
    logging.error(ex)
except KeyboardInterrupt:
    logging.info('Keyboard interrupt detected. Exiting...')
    pass

# plt.show()
