# First networkx library is imported  
# along with matplotlib 
import networkx as nx 
import matplotlib.pyplot as plt 

import graphviz


g = graphviz.Digraph(comment='Input')

conjuctions = set()
flip_flops = set()
outputs = set()
generators = set()
edges = set()
with open("/home/nskeledz/projects/AdventOfCode2023/day20/input.txt", "r") as file:
    lines = file.read().split('\n')
    for line in lines:
        name, outputs = line.split('->')
        if name[0] == '&':
            name = name.strip()
            name = name[1:]
            conjuctions.add(name)
        elif name[0] == '%':
            name = name.strip()
            name = name[1:]
            flip_flops.add(name)
        else:
            name = name.strip()
            generators.add(name)
        for out in outputs.split(','):
            edges.add((name, out.strip()))


for n in conjuctions:
    g.node(n, color='blue')

for n in flip_flops:
    g.node(n, color='red')

for n in generators:
    g.node(n, color='green')

for e in edges:
    print(e)
    g.edge(e[0], e[1])

g.render(directory='doctest-output').replace('\\', '/')



