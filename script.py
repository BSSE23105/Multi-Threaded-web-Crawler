# plot_performance.py
import matplotlib.pyplot as plt

configs = [
    {'threads':1, 'chunk':5, 'time':8.039},
    {'threads':2, 'chunk':5, 'time':4.302},
    {'threads':4, 'chunk':5, 'time':3.209},
    {'threads':8, 'chunk':5, 'time':2.162},
    {'threads':1, 'chunk':50, 'time':8.059},
    {'threads':2, 'chunk':50, 'time':4.330},
    {'threads':4, 'chunk':50, 'time':3.371},
    {'threads':8, 'chunk':50, 'time':2.508},
]

# Plot thread count vs time for chunk=1 and chunk=4
for chunk in [5,50]:
    xs = [c['threads'] for c in configs if c['chunk']==chunk]
    ys = [c['time']    for c in configs if c['chunk']==chunk]
    plt.plot(xs, ys, 'o-',
             label=f'chunk={chunk}')
plt.xlabel('NO of Threads')
plt.ylabel('Total Time in seconds')
plt.title('Thread Count vs. Total Time')
plt.legend()
plt.savefig('performanceInGraph.png')
