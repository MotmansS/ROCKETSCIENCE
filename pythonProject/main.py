

import matplotlib.pyplot as plt
import numpy as np
import serial
import serial.tools.list_ports
import warnings

ESP_32_PORTS = [
    p.device
    for p in serial.tools.list_ports.comports()
    if 'CP210x' in p.description  # may need tweaking to match new arduinos
]
if not ESP_32_PORTS:
    raise IOError("No ESP-32 found")
if len(ESP_32_PORTS) > 1:
    warnings.warn('Multiple ESP-32s found - using the first')
port = ESP_32_PORTS[0]  # Windows Find your Serial port via device manager


def read():
    _ser = serial.Serial(port, baudrate=115200)
    _ser.reset_input_buffer()
    _ser.reset_output_buffer()
    _ser.write(b"rocket.read?\n")
    sdata = _ser.readline()
    sdata = ([np.ushort(p) for p in sdata.split(b",")])
    altitude_ = ([np.ushort(p) for p in sdata[0].split(b",")])
    x_ = ([np.ushort(p) for p in sdata[1].split(b",")])
    y_ = ([np.ushort(p) for p in sdata[2].split(b",")])
    z_ = ([np.ushort(p) for p in sdata[3].split(b",")])
    return altitude, x, y, z


if __name__ == '__main__':
    altitude, x, y, z = read()
    fig = plt.figure(figsize=(4, 4))
    axx = fig.add_subplot(111)
    axx.plot(x)
    axx = fig.add_subplot(111)
    axx.plot(y)
    axx = fig.add_subplot(111)
    axx.plot(z)
    axAltitude = fig.add_subplot(111)
    axAltitude.plot(altitude)


