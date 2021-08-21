import logging
import pexpect
import time

from tests.qemu import args
from tests.qemu import key

logger = logging.getLogger(__name__)

QEMU_PROMPT = "(qemu)"


class Monitor:

    def __init__(self, port):
        logger.info("Connecting to qemu, waiting for prompt")
        self.process = pexpect.spawn(f"telnet 127.0.0.1 {port}")
        self.wait_for_prompt()

    def wait_for_prompt(self):
        try:
            self.process.expect(QEMU_PROMPT, timeout=2)
        except pexpect.TIMEOUT as e:
            logger.error("[pexpect] Qemu prompt wasn't seen within timeout")
            raise e

    def send_qemu_command(self, cmd, wait_prompt=True):
        self.process.sendline(cmd)
        if wait_prompt:
            self.wait_for_prompt()

    def send_key(self, key):
        """Send a key to qemu."""
        self.process.sendline(f"sendkey {key}")
        self.wait_for_prompt()

    def type_string(self, msg):
        """Type string in qemu."""
        logger.info(f"[to_qemu] typing string: {repr(msg)}")
        for char in msg:
            self.send_key(key.char_to_key(char))

def handle_qemu_cmd(monitor, args):
    if not args.qemu_cmd:
        return
    logger.info("Handle: fuzzy_shell_cmd")
    cmd = " ".join(args.qemu_cmd) + "\n"
    monitor.send_qemu_command(cmd)

def handle_fuzzy_shell_cmd(monitor, args):
    if not args.fuzzy_shell_cmd:
        return
    logger.info("Handle: fuzzy_shell_cmd")
    cmd = " ".join(args.fuzzy_shell_cmd) + "\n"
    monitor.type_string(cmd)

def handle_qemu_quit(monitor, args):
    if not args.quit:
        return
    logger.info("Handle: qemu quit")
    monitor.send_qemu_command("quit\n", wait_prompt=False)

def main():
    logging.basicConfig(level=logging.INFO)
    parsed_args = args.get_args()

    monitor = Monitor(parsed_args.port)

    handle_fuzzy_shell_cmd(monitor, parsed_args)
    handle_qemu_cmd(monitor, parsed_args)
    handle_qemu_quit(monitor, parsed_args)

    logger.info("graceful exit")

if __name__ == '__main__':
    main()
