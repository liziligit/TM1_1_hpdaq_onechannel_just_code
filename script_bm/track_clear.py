#!/Library/Frameworks/Python.framework/Versions/3.7/bin/python3
# -*- coding: UTF-8 -*- 

import re

file = r'track_article.txt'

f = open(file).read().strip()
blocks = f.split('\n\n')
ok = []
for i, b in enumerate(blocks):
    resol = float(re.search('resol: (.*)\n', b).group(1))
    thetaResol = float(re.search('thetaResol: (.*)\n', b).group(1))
    if resol <= 5.5 and thetaResol <= 0.3 :
        ok.append(b)
        
        
for i, x in enumerate(ok):
    beam = re.search('(beam_\d+)', x).group(1)
    frame = re.search('", \d+,(\d+),', x).group(1)
    pede = re.search('(pede_\d+)\"', x).group(1)
    resol = re.search('resol: (.*?)\n', x).group(1)
    thetaResol = re.search('thetaResol: (.*?)\n', x).group(1)
    
    ok[i] = beam + '\\_frame' + frame + '\\_' + pede + ' & ' + resol + ' & ' + thetaResol + ' \\\\  \\hline\n'

with open(r'track_article.dat', 'w') as f:
    f.write(''.join(ok))
    
    