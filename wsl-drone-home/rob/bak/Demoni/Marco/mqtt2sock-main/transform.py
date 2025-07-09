import json
import importlib

def convert(params, script):
    if script:
        fname = 'transformations.'+script
        mod = ".".join(fname.split('.')[:-1])
        sname = fname.split('.')[-1]
        conv = importlib.import_module(mod)
        print('calling ')
        print(fname, mod, sname)
        print(params)
        return getattr(conv, sname)(*params)
        

    


if __name__=="__main__":
    config = json.load(open('config.json'))

    
