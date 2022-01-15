import os
import tempfile
import argparse
import json

parser = argparse.ArgumentParser()
parser.add_argument("-k", "--key", help="addressess to key, to read or write some value")
parser.add_argument("-v", "--val", help="assign a value to a key")

args = parser.parse_args()
storage_path = os.path.join(tempfile.gettempdir(), 'storage.data')

data = {}

if not os.path.exists(storage_path):
    with open(storage_path, 'w') as f:
        pass

with open(storage_path, 'r') as f:
    if os.path.getsize(storage_path):
        data = json.load(f)
    if args.key:
        if args.val:
            if args.key in data:
                data[args.key].append(args.val)
            else:
                data[args.key] = []
                data[args.key].append(args.val)
        else:
            if args.key in data:
                str = ""
                for value in data[args.key]:
                    str += value + ", "
                str = str[:-2]
                print(str)
                exit()
with open(storage_path, 'w') as f:
    json.dump(data, f)
