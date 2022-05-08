from tokenize import Double
from matplotlib import widgets
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.cm as cm
from matplotlib.colors import Normalize

with open('./out.txt', 'r') as file:
    lines = file.readlines()

clusters = {}

for line in lines:
    split_line = line.split()
    point = [float(x) for x in split_line[:-1]]
    cluster_id = int(split_line[-1])
    clusters.setdefault(cluster_id, []).append(point)


clusters_ids = clusters.keys()
cmap = cm.rainbow
norm = Normalize(vmin=0, vmax=max(clusters_ids))

for cluster_id in clusters_ids:

    if cluster_id >= 0:
        color = cmap(norm(cluster_id))
        label = "cluster " + str(cluster_id)
    elif cluster_id == -1:
        color = "white"
        label = "point isolé"
    else:
        color = "black"
        label = "point non défini"

    clusters[cluster_id] = (np.stack(clusters[cluster_id], 0), color, label)


fig, ax = plt.subplots(figsize=(6,6))
ax.set_xlabel('x')
ax.set_ylabel('x2')
ax.set_title('Exemple de clustering')

for cluster_id, (points, color, label) in clusters.items():
    ax.scatter(points[:,0], points[:,1], color=color, label=label, edgecolor="black")

handles, labels = ax.get_legend_handles_labels() 
handles, labels = zip(* sorted(zip(handles, labels), key=lambda t : t[1]))
ax.legend(handles, labels)
with open('./weight.txt', 'r') as f:
    lines = f.readlines()
    for line in lines:
        ws = line.split()
        weights = [float(x) for x in ws]
x = np.arange(-3.0,3.0,0.1)
y =  (-weights[0]+ weights[1]*x )
ax.plot(x,y)
plt.show()