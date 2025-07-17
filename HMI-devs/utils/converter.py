import csv

def v2p(volts):
    data = csv.DictReader(open('FitsplineBatteria.csv'))
    data = list(data)
    for d in data: 
        d['pm'] = float(d['pm'])
        d['d'] = float(d['d'])
        d['p'] = float(d['p'])
        d['xi'] = float(d['xi'])

    for d in data: 
        if volts > d['d'] and volts <= d['xi']:
            break
    else: 
        return 0

    
    #print(f"({d['pm']}-{d['p']})/({d['xi']}-{d['d']})")
    a = (d['pm']-d['p'])/(d['xi']-d['d'])
    ret = a*(volts-d['d'])+d['p']
    return ret

if __name__ == "__main__":
    import sys
    print(v2p(float(sys.argv[1])))