import requests
from pathlib import Path
import subprocess
import signal

def write_osms():
    import openstudio
    OSM_PATH = Path('/media/DataExt4/Software/Others/OS-build3/model.osm')
    OSM_PATH2 = Path('/media/DataExt4/Software/Others/OS-build3/model2.osm')

    model = openstudio.model.Model()
    model.save(str(OSM_PATH), True)
    model = openstudio.model.Model()
    model.save(str(OSM_PATH2), True)

HOST = 'localhost'
OPENSTUDIO_CLI = Path('/home/julien/Software/Others/OS-build/Products/openstudio')

def get_open_port_for_serving():
    import socket
    from contextlib import closing
    import errno
    port = 8089
    is_open = False
    while not is_open:
        port += 1
        # print(f"Testing {port=}")
        with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as sock:
            try:
                sock.bind((HOST, port))
                is_open = True
            except socket.error as e:
                if e.errno == errno.EADDRINUSE:
                    print(f"{port=} is already in use")
                else:
                    # something else raised the socket.error exception
                    raise e
    print(f"Using {port=}")
    return port


if __name__ == "__main__":
    port = get_open_port_for_serving()
    URL = f"http://{HOST}:{port}"
    p = subprocess.Popen([str(OPENSTUDIO_CLI), 'labs', 'measure', '-s', str(port)])


    r = requests.get(f"{URL}/internal_state")
    r.raise_for_status()
    assert r.json() == {'my_measures_dir': '', 'osm': [], 'status': 'running'}

    my_measures_dir = str(Path('~/OpenStudio/Measures').expanduser())
    r = requests.post(url=f"{URL}/set", json={"my_measures_dir": my_measures_dir})
    r.raise_for_status()

    r = requests.get(f"{URL}/internal_state")
    r.raise_for_status()
    assert r.json() == {'my_measures_dir': my_measures_dir, 'osm': [], 'status': 'running'}


    p.send_signal(signal.SIGINT)
    assert p.poll() == 0
