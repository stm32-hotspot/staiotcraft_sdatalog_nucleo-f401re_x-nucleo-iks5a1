import re

# Read the header file
with open("lsm6dsv16x_mlc.h", "r") as f:
    content = f.read()

# Find the lsm6dsv16x_mlc_conf_0 array
array_match = re.search(
    r"lsm6dsv16x_mlc_conf_0\[\]\s*=\s*\{(.*?)\};",
    content,
    re.DOTALL
)
if not array_match:
    raise ValueError("Array lsm6dsv16x_mlc_conf_0 not found.")

array_body = array_match.group(1)

# Find all .address and .data fields
pattern = re.compile(
    r"\.address\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*\.data\s*=\s*(0x[0-9A-Fa-f]+|\d+)",
    re.MULTILINE
)
matches = pattern.findall(array_body)

# Convert to hex (without 0x) and concatenate
result = ""
for address, data in matches:
    addr_val = int(address, 0)
    data_val = int(data, 0)
    result += f"{addr_val:02X}{data_val:02X}"

print(result)
print(f"Number of output characters: {len(result)}")