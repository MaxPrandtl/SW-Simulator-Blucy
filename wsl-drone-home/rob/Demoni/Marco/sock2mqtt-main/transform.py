import json

import struct


def convert(bte, config):
    ret = {}
    for part in config['parts']:
        try:
            bts = bte[:part['bytes']]
            bte = bte[part['bytes']:]
            if part['type'] == 'int':
                endian = "little"
                signed = part.get('signed', True)
                ret[part['label']] = int.from_bytes(bts, byteorder=endian, signed=signed)
            elif part['type'] == 'float':
                ret[part['label']] = struct.unpack('f', bts)[0]
        except:
           print('error')

    print(ret)
    return ret

if __name__=="__main__":
    config = json.load(open('config.json'))

    
