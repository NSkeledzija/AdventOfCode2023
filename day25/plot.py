import matplotlib.pyplot as plt 

import graphviz


g = graphviz.Graph(comment='Input')

nodes = set()
edges = set()
with open("/home/nskeledz/projects/AdventOfCode2023/day25/example.txt", "r") as file:
    lines = file.read().split('\n')
    for line in lines:
        name, connections = line.split(':')
        for connection in connections.split():
            edges.add((name.strip(), connection.strip()))
        
for n in nodes:
    g.node(n, color='green')

for e in edges:
    print(e)
    g.edge(e[0], e[1])

g.render(directory='doctest-output').replace('\\', '/')



