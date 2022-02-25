#!/usr/bin/python3
# coding=utf-8

# Small utility to parse the hardware codec debugging info found in the /proc
# filesystem (e.g. at /proc/asound/cardX/codec#Y).
#
# Usage: asound-codec-display <path to proc file>

# pip3 install networkx matplotlib

import re
import sys
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from collections import namedtuple

Node = namedtuple('Node', 'nid ntype wcaps wcapsdescr')

re_node = re.compile(
    r"Node 0x(?P<nid>[0-9a-f]+) \[(?P<ntype>.*)\] "
    r"wcaps 0x(?P<wcaps>[0-9a-f]+): (?P<wcapsdescr>.*)"
)

re_cx = re.compile(r"Connection: (?P<nconn>\d+)")

graph = nx.DiGraph()
curnode = None

fp = open(sys.argv[1], "r") if len(sys.argv) > 1 else sys.stdin
try:
    for line in fp:
        line = line.strip()
        m_node = re_node.match(line)
        if m_node:
            curnode = Node(**m_node.groupdict())
            graph.add_node(
                curnode.nid,
                label='%s (%s)' % (curnode.nid, curnode.ntype),
                **m_node.groupdict()
            )
        if not curnode:
            continue
        m_cx = re_cx.match(line)
        if m_cx and int(m_cx.group("nconn")) > 0:
            nids = [
                s.replace("0x", "").replace("*", "")
                for s in fp.readline().strip().split()]
            for nid in nids:
                graph.add_edge(curnode.nid, nid)
finally:
    if fp != sys.stdin:
        fp.close()

nodelist = graph.nodes
nodetypes = set(graph.nodes[n]['ntype'] for n in nodelist)
cmap = plt.get_cmap('cool')
nodetypecolors = dict(zip(nodetypes, cmap(np.linspace(0, 1, len(nodetypes)))))
colors = [nodetypecolors[graph.nodes[k]['ntype']] for k in nodelist]
labels = {k: graph.nodes[k].get('label', k) for k in nodelist}
pos = nx.layout.spring_layout(graph, k=12./len(nodelist))
nx.draw(
    graph,
    with_labels=True,
    arrows=True,
    labels=labels,
    pos=pos,
    node_color=colors,
    edge_color="lightgray",
)
plt.show()
