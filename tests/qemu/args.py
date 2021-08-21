import argparse

def get_args():
    parser = argparse.ArgumentParser(description="Fuzzy OS QEMU monitor client")
    parser.add_argument("-p", "--port",
                        type=int,
                        default=55555,
                        help="qemu monitor port")
    parser.add_argument('-qc', '--qemu-cmd',
                        metavar="qemu-token",
                        type=str,
                        nargs='+',
                        help="command to execute in qemu monitor")
    parser.add_argument('-sc', '--fuzzy-shell-cmd',
                        metavar="fuzzy-token",
                        type=str,
                        nargs='+',
                        help="command to execute in Fuzzy OS shell")
    parser.add_argument('-q', '--quit',
                        action='store_true',
                        help="kill qemu before leaving")
    args = parser.parse_args()
    return args
