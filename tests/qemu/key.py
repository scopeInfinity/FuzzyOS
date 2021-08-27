QEMU_CODES = {
    " " : "spc",
    "\n": "ret",
    "." : "dot",
    "," : "comma",
    "-" : "minus",
    "=" : "equal",
    "*" : "asterisk"
}

def char_to_key(char):
    if char in QEMU_CODES:
        return QEMU_CODES[char]

    if isinstance(char, str) and len(char) == 1 and char.isalnum():
        # single isalnum char
        return char
    raise Exception(f"char_to_key can't handle '{char}'")
